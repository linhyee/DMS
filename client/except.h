// Exception classes
#ifndef _EXCEPT_H
#define _EXCEPT_H
#include <string>
using namespace std;
// Client exception
class ClientException : public exception {
public:
	ClientException (void) :
		m_msg ("Client Exception !") {}
	ClientException (const string& msg) :
		m_msg ("Client Exception : ") {
		m_msg += msg;
		m_msg += " !";
	}
	~ClientException (void) throw () {}
	const char* what (void) const throw () {
		return m_msg.c_str ();
	}
private:
	string m_msg;
};
// Backup exception
class BackupException : public ClientException {
public:
	BackupException (void) :
		ClientException ("Backup Exception") {}
	BackupException (const string& msg) :
		ClientException (msg) {}
};
// Read exception
class ReadException : public ClientException {
public:
	ReadException (void) :
		ClientException ("Read Exception") {}
	ReadException (const string& msg) :
		ClientException (msg) {}
};
// Save exception
class SaveException : public ClientException {
public:
	SaveException (void) :
		ClientException ("Save Exception") {}
	SaveException (const string& msg) :
		ClientException (msg) {}
};
// Socket exception
class SocketException : public ClientException {
public:
	SocketException (void) :
		ClientException ("Socket Exception") {}
	SocketException (const string& msg) :
		ClientException (msg) {}
};
// Send exception
class SendException : public ClientException {
public:
	SendException (void) :
		ClientException ("Send Exception") {}
	SendException (const string& msg) :
		ClientException (msg) {}
};
#endif // _EXCEPT_H
