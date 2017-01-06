// Exception classes
#ifndef _EXCEPT_H
#define _EXCEPT_H
#include <string>
using namespace std;
// Server exception
class ServerException : public exception {
public:
	ServerException (void) :
		m_msg ("Server Exception !") {}
	ServerException (const string& msg) :
		m_msg ("Server Exception : ") {
		m_msg += msg;
		m_msg += " !";
	}
	~ServerException (void) throw () {}
	const char* what (void) const throw () {
		return m_msg.c_str ();
	}
private:
	string m_msg;
};
// Database exception
class DBException : public ServerException {
public:
	DBException (void) :
		ServerException ("Database Exception") {}
	DBException (const string& msg) :
		ServerException (msg) {}
};
// Socket exception
class SocketException : public ServerException {
public:
	SocketException (void) :
		ServerException ("Socket Exception") {}
	SocketException (const string& msg) :
		ServerException (msg) {}
};
// Thread exception
class ThreadException : public ServerException {
public:
	ThreadException (void) :
		ServerException ("Thread Exception") {}
	ThreadException (const string& msg) :
		ServerException (msg) {}
};
#endif // _EXCEPT_H
