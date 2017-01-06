// 实现客户线程类
#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
using namespace std;
#include "clientthread.h"
#include "logqueue.h"
// 构造器
ClientThread::ClientThread (
	int connfd // 通信套接字
) : m_connfd (connfd) {}
// 线程处理
void ClientThread::run (void) {
	unsigned long long logs;
	MLogRec log;
	for (logs = 0; ; ++logs)
	{
		ssize_t rlen;
		for (size_t len = sizeof (log); len;
			len -= rlen)
			if ((rlen = recv (m_connfd,
				(char*)&log + (sizeof (log) - len),
				len, 0)) <= 0)
				goto escape;
		log.pid = ntohl (log.pid);
		log.logintime = ntohl (log.logintime);
		log.logouttime = ntohl (log.logouttime);
		log.durations = ntohl (log.durations);
		g_logQueue << log;
	}
escape:
	cout << "接收到" << logs << "条匹配日志记录。"
		<< endl;
	close (m_connfd);
	delete this; // 线程结束，自毁
}





