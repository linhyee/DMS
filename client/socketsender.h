#ifndef _SOCKETSENDER_H
#define _SOCKETSENDER_H
#include "logsender.h"
#include "workthread.h"
class SocketSender : public LogSender {
public:
	SocketSender (const string& ip, short port,
		const string& failFile,
		WorkThread* work = NULL);
	void sendLog (list<MLogRec>& logs)
		throw (ClientException);
private:
	void readFailFile (list<MLogRec>& logs)
		throw (ReadException);
	void connectServer (void)
		throw (SocketException);
	void sendData (list<MLogRec>& logs)
		throw (SendException);
	void saveFailFile (list<MLogRec>& logs)
		throw (SaveException);
	string m_ip;
	short  m_port;
	string m_failFile;
	int    m_sockfd;
	WorkThread* m_work; // 工作线程对象
};
#endif // _SOCKETSENDER_H
