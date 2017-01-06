#include <iostream>
using namespace std;
#include "consolesender.h"
void ConsoleSender::sendLog (list<MLogRec>& logs)
	throw () {
	cout << "Send log begin..." << endl;
	// 遍历匹配日志记录集
	for (list<MLogRec>::iterator it =
		logs.begin (); it != logs.end (); ++it)
		cout << *it << endl;
	cout << "Send log end." << endl;
}
