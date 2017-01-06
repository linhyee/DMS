// 实现日志队列类
#include <iostream>
using namespace std;
#include "logqueue.h"
// 构造器
LogQueue::LogQueue (void) {
	// 初始化同步互斥量
	pthread_mutex_init (&m_mutex, 0);
	// 初始化同步条件量
	pthread_cond_init (&m_cond, 0);
}
// 压入日志记录
LogQueue& LogQueue::operator<< (
	const MLogRec& log) {
	cout << "压入日志记录开始..." << endl;
	pthread_mutex_lock (&m_mutex);
	m_logs.push_back (log);
	pthread_cond_signal (&m_cond);
	pthread_mutex_unlock (&m_mutex);
	cout << "压入日志记录完成。" << endl;
	return *this;
}
// 弹出日志记录
LogQueue& LogQueue::operator>> (MLogRec& log) {
	cout << "弹出日志记录开始..." << endl;
	pthread_mutex_lock (&m_mutex);
	while (m_logs.empty ()) {
		cout << "等待日志记录..." << endl;
		pthread_cond_wait (&m_cond, &m_mutex);
	}
	log = m_logs.front ();
	m_logs.pop_front ();
	pthread_mutex_unlock (&m_mutex);
	cout << "弹出日志记录完成。" << endl;
	return *this;
}
LogQueue g_logQueue;
