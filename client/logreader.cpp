#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <sys/stat.h>
#include <arpa/inet.h>
using namespace std;
#include "logreader.h"
LogReader::LogReader (const string& logFile,
	const string& loginsFile) :
	m_logFile (logFile),
	m_loginsFile (loginsFile) {}
list<MLogRec>& LogReader::readLog (void)
	throw (ClientException) {
	cout << "Read log begin..." << endl;
	backup ();
	readLoginsFile ();
	readBackupFile ();
	match ();
	saveLoginsFile ();
	cout << "Read log end." << endl;
	return m_logs;
}
void LogReader::backup (void)
	throw (BackupException) {
	cout << "Backup log begin..." << endl;
	// 生成备份文件名
	time_t now = time (NULL);
	tm* local = localtime (&now);
	ostringstream oss;
	oss << m_logFile << '.' << setfill ('0')
		<< setw (4) << local->tm_year + 1900
		<< setw (2) << local->tm_mon + 1
		<< setw (2) << local->tm_mday
		<< setw (2) << local->tm_hour
		<< setw (2) << local->tm_min
		<< setw (2) << local->tm_sec;
	m_backupFile = oss.str ();
	// 生成备份指令
	string cmd ("./backup.sh ");
	cmd += m_logFile;
	cmd += " ";
	cmd += m_backupFile;
#ifdef _DEBUG
	cout << "备份指令：" << cmd << endl;
#endif
	// 执行备份指令
	int status = system (cmd.c_str ());
	if (WEXITSTATUS (status) == 1)
		throw BackupException ("拷贝错误");
	if (WEXITSTATUS (status) == 2)
		throw BackupException ("清空错误");
	cout << "Backup log end." << endl;
}
void LogReader::readLoginsFile (void)
	throw (ReadException) {
	cout << "Read logins file begin..." << endl;
	ifstream ifs (m_loginsFile.c_str (),
		ios::binary);
	if (ifs) {
		LogRec log;
		while (ifs.read ((char*)&log,
			sizeof (log)))
			m_logins.push_back (log);
		if (! ifs.eof ())
			throw ReadException ();
		ifs.close ();
		unlink (m_loginsFile.c_str ());
	}
	cout << "上次匹配失败的登入记录："
		<< m_logins.size () << endl;
	cout << "Read logins file end." << endl;
}
void LogReader::readBackupFile (void)
	throw (ReadException) {
	cout << "Read backup file begin..." << endl;
	// 打开备份文件
	ifstream ifs (m_backupFile.c_str (),
		ios::binary);
	if (! ifs)
		throw ReadException ("备份文件无法打开");
	// 计算记录条数
	struct stat st;
	if (stat (m_backupFile.c_str (), &st) == -1)
		throw ReadException (
			"无法获取备份文件大小");
	int records = st.st_size / 372;
	// 逐条读取记录
	for (int i = 0; i < records; ++i) {
		// 登录名
		ifs.seekg (i * 372, ios::beg);
		char logname[32];
		ifs.read (logname, 32);
		// 登录进程ID
		ifs.seekg (36, ios::cur);
		pid_t pid;
		ifs.read ((char*)&pid, sizeof (pid));
		pid = ntohl (pid);
		// 登录类型：7-登入/8-登出
		short type;
		ifs.read ((char*)&type, sizeof (type));
		type = ntohs (type);
		// 登入/登出时间
		ifs.seekg (6, ios::cur);
		long logtime;
		ifs.read ((char*)&logtime,
			sizeof (logtime));
		logtime = ntohl (logtime);
		// 登录IP长度
		ifs.seekg (28, ios::cur);
		short len;
		ifs.read ((char*)&len, sizeof (len));
		len = ntohs (len);
		// 登录IP或机器名
		char logip[len+1];
		ifs.read (logip, len);
		logip[len] = '\0';
#ifdef _DEBUG
		/*
		cout << logname << ','
			<< pid << ','
			<< type << ','
			<< logtime << ','
			<< len << ','
			<< logip << endl;
		*/
#endif
		if (logname[0] != '.') {
			LogRec log;
			strcpy (log.logname, logname);
			strcpy (log.logip, logip);
			log.pid = pid;
			log.logtime = logtime;
			if (type == 7) // 登入记录
				m_logins.push_back (log);
			else if (type == 8) // 登出记录
				m_logouts.push_back (log);
		}
	}
	ifs.close ();
	cout << "登入日志记录：" << m_logins.size ()
		<< endl;
	cout << "登出日志记录：" << m_logouts.size ()
		<< endl;
	cout << "Read backup file end." << endl;
}
void LogReader::match (void) {
	cout << "Match logins and logouts begin..."
		<< endl;
	// 对登出日志记录集中的每一条记录...
	for (list<LogRec>::iterator oit =
		m_logouts.begin ();
		oit != m_logouts.end (); ++oit)
		// 在登入日志记录集中查找匹配记录...
		for (list<LogRec>::iterator iit =
			m_logins.begin ();
			iit != m_logins.end (); ++iit)
			if (! strcmp (iit->logname,
				oit->logname) && // 登录名一致
				! strcmp (iit->logip,
				oit->logip) && // 登录IP一致
				iit->pid == oit->pid) { // 进程一致
				MLogRec log = {"","",0,0,0,0};
				strcpy (log.logname, oit->logname);
				strcpy (log.logip, oit->logip);
				log.pid = oit->pid;
				log.logintime = iit->logtime;
				log.logouttime = oit->logtime;
				log.durations = log.logouttime -
					log.logintime;
				m_logs.push_back (log);
				// 从登入日志记录集中删除与登出日志
				// 记录匹配的记录，余者都是没有登出
				// 日志记录与之匹配的登入日志记录
				m_logins.erase (iit);
				break;
			}
	cout << "匹配日志记录：" << m_logs.size ()
		<< endl;
	cout << "剩余登入记录：" << m_logins.size ()
		<< endl;
	cout << "Match logins and logouts end." << endl;
}
void LogReader::saveLoginsFile (void)
	throw (SaveException) {
	cout << "Save logins file begin..." << endl;
	if (! m_logins.empty ()) {
		ofstream ofs (m_loginsFile.c_str (),
			ios::binary);
		if (! ofs)
			throw SaveException ();
		while (! m_logins.empty ()) {
			ofs.write ((char*)&m_logins.front (),
				sizeof (LogRec));
			m_logins.pop_front ();
		}
		ofs.close ();
	}
	cout << "Save logins file end." << endl;
}
