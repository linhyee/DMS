// 声明服务器套接字类
#ifndef _SERVERSOCKET_H
#define _SERVERSOCKET_H
#include "except.h"
// 服务器套接字类
class ServerSocket {
public:
	// 构造器
	ServerSocket (
		const string& ip,  // 绑定IP地址
		short         port // 绑定端口号
	)	throw (SocketException);
	// 等待客户机连接
	void acceptClient (void)
		throw (SocketException);
private:
	int m_sockfd; // 侦听套接字
};
#endif // _SERVERSOCKET_H
