#ifndef _CONSOLESENDER_H
#define _CONSOLESENDER_H
#include "logsender.h"
class ConsoleSender : public LogSender {
public:
	void sendLog (list<MLogRec>& logs) throw ();
};
#endif // _CONSOLESENDER_H
