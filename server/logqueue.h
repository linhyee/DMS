// 声明日志队列类
#ifndef _LOGQUEUE_H
#define _LOGQUEUE_H
#include <pthread.h>
#include <list>
using namespace std;
#include "data.h"
// 日志队列类
class LogQueue {
public:
	// 构造器
	LogQueue (void);
	// 压入日志记录
	LogQueue& operator<< (const MLogRec& log);
	// 弹出日志记录
	LogQueue& operator>> (MLogRec& log);
private:
	pthread_mutex_t m_mutex; // 同步互斥量
	pthread_cond_t  m_cond;  // 同步条件量
	list<MLogRec>   m_logs;  // 日志记录集
};
extern LogQueue g_logQueue;
#endif // _LOGQUEUE_H
