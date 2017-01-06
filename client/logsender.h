#ifndef _LOGSENDER_H
#define _LOGSENDER_H
#include <list>
using namespace std;
#include "data.h"
#include "except.h"
class LogSender {
public:
	virtual ~LogSender (void) {}
	virtual void sendLog (list<MLogRec>& logs)
		throw (ClientException) = 0;
};
#endif // _LOGSENDER_H
