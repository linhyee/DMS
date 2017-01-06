// 声明文件数据访问对象类
#ifndef _FILEDAO_H
#define _FILEDAO_H
#include <fstream>
using namespace std;
#include "logdao.h"
// 文件数据访问对象类
class FileDao : public LogDao{
public:
	// 构造器
	FileDao (
		const string& path // 文件路径
	)	throw (DBException);
	// 析构器
	~FileDao (void);
	// 插入
	void insert (const MLogRec& log)
		throw (DBException);
private:
	ofstream m_ofs; // 输出文件流
};
#endif // _FILEDAO_H
