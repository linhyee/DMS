// Data object
#ifndef _DATA_H
#define _DATA_H
#include <unistd.h>
#include <iostream>
using namespace std;
// Matched log record
struct MLogRec {
	char  logname[32]; // Log name
	char  logip[32];   // Log ip
	pid_t pid;         // Log process ID
	long  logintime;   // Login time
	long  logouttime;  // Logout time
	long  durations;   // Online time
	friend ostream& operator<< (ostream& os,
		const MLogRec& log) {
		return os << log.logname << ','
			<< log.logip << ','
			<< log.pid << ','
			<< log.logintime << ','
			<< log.logouttime << ','
			<< log.durations;
	}
};
#endif // _DATA_H
