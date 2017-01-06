#include <iostream>
using namespace std;
#include "client.h"	
Client::Client (LogReader& reader,
	LogSender& sender) :
	m_reader (reader), m_sender (sender) {}
void Client::dataMine (void)
	throw (ClientException) {
	cout << "Data mine begin..." << endl;
	m_sender.sendLog (m_reader.readLog ());
	cout << "Data mine end." << endl;
}
