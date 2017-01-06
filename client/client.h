#ifndef _CLIENT_H
#define _CLIENT_H
#include "logreader.h"
#include "logsender.h"
class Client {
public:
	Client (LogReader& reader, LogSender& sender);
	void dataMine (void) throw (ClientException);
private:
	LogReader& m_reader;
	LogSender& m_sender;
};
#endif // _CLIENT_H
