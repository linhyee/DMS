#ifndef _LOGREADER_H
#define _LOGREADER_H
#include <string>
#include <list>
using namespace std;
#include "data.h"
#include "except.h"
class LogReader {
public:
	LogReader (const string& logFile,
		const string& loginsFile);
	list<MLogRec>& readLog (void)
		throw (ClientException);
private:
	void backup (void)
		throw (BackupException);
	void readLoginsFile (void)
		throw (ReadException);
	void readBackupFile (void)
		throw (ReadException);
	void match (void);
	void saveLoginsFile (void)
		throw (SaveException);
	string m_logFile; // Log file
	string m_loginsFile; // Login file
	string m_backupFile; // Backup file
	list<LogRec> m_logins; // Login records list
	list<LogRec> m_logouts; // Logout records list
	list<MLogRec> m_logs; // Matched records list
};
#endif // _LOGREADER_H
