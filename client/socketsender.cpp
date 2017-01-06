#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
using namespace std;
#include "socketsender.h"
SocketSender::SocketSender (const string& ip,
	short port, const string& failFile,
	WorkThread* work) :
	m_ip (ip), m_port (port),
	m_failFile (failFile), m_work (work) {}
void SocketSender::sendLog (list<MLogRec>& logs)
	throw (ClientException) {
	cout << "Send log begin..." << endl;
	try {
		try {
			readFailFile (logs);
		}
		catch (ReadException& ex) {
			cout << "No fail file !" << endl;
		}
		connectServer ();
		sendData (logs);
	}
	catch (SocketException& ex) {
		saveFailFile (logs);
		throw;
	}
	catch (SendException& ex) {
		saveFailFile (logs);
		throw;
	}
	cout << "Send log end." << endl;
}
void SocketSender::readFailFile (
	list<MLogRec>& logs) throw (ReadException) {
	cout << "Read fail file begin..." << endl;
	ifstream ifs (m_failFile.c_str (),
		ios::binary);
	if (! ifs)
		throw ReadException ();
	MLogRec log;
	while (ifs.read ((char*)&log, sizeof (log)))
		logs.push_front (log);
	if (! ifs.eof ())
		throw ReadException ();
	ifs.close ();
	unlink (m_failFile.c_str ());
	cout << "Read fail file end." << endl;
}
void SocketSender::connectServer (void)
	throw (SocketException) {
	cout << "Connect server begin..." << endl;
	if ((m_sockfd = socket (AF_INET, SOCK_STREAM,
		0)) == -1)
		throw SocketException ();
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons (m_port);
	addr.sin_addr.s_addr = inet_addr (
		m_ip.c_str ());
	if (connect (m_sockfd, (sockaddr*)&addr,
		sizeof (addr)) == -1)
		throw SocketException ();
	cout << "Connect server end." << endl;
}
void SocketSender::sendData (list<MLogRec>& logs)
	throw (SendException) {
	cout << "Send data begin..." << endl;
	while (! logs.empty ()) {
		MLogRec log = logs.front ();
		log.pid = htonl (log.pid);
		log.logintime = htonl (log.logintime);
		log.logouttime = htonl (log.logouttime);
		log.durations = htonl (log.durations);
		if (send (m_sockfd, &log, sizeof (log),
			0) == -1) {
			close (m_sockfd);
			throw SendException ();
		}
		if (m_work) {
			ostringstream oss;
			oss << logs.front ();
			m_work->update (oss.str ());
		}
		// 删除发送成功的匹配日志记录，
		// 余者存入发送失败文件，以备下次重发
		logs.pop_front ();
	}
	close (m_sockfd);
	cout << "Send data end." << endl;
}
void SocketSender::saveFailFile (
	list<MLogRec>& logs) throw (SaveException) {
	cout << "Save fail file begin..." << endl;
	if (! logs.empty ()) {
		ofstream ofs (m_failFile.c_str (),
			ios::binary);
		if (! ofs)
			throw SaveException ();
		while (! logs.empty ()) {
			ofs.write ((char*)&logs.front (),
				sizeof (MLogRec));
			logs.pop_front ();
		}
		ofs.close ();
	}
	cout << "Save fail file end." << endl;
}
