#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "SystemCall.h"
#include <string>
using namespace std;

class UserCall {
public:
	static const int EAX = 0;//u.ar0[EAX]；访问现场保护区中EAX寄存器的偏移量

	enum ErrorCode {
		U_NOERROR = 0,//没有错误
		U_ENOENT = 1, //找不到文件或者文件夹
		U_EBADF = 2,  //找不到文件句柄
		U_EACCES = 3, //权限不足
		U_ENOTDIR = 4,//文件夹不存在
		U_ENFILE = 5, //文件表溢出
		U_EMFILE = 6, //打开文件过多
		U_EFBIG = 7,  //文件过大
		U_ENOSPC = 8  //磁盘空间不足
	};

public:
	INode* nowDirINodePointer;        //指向当前目录的Inode指针
	INode* paDirINodePointer;         //指向父目录的Inode指针
	DirectoryEntry dent;              //当前目录的目录项
	char dbuf[DirectoryEntry::DIRSIZ];//当前路径分量
	string curDirPath;                //当前工作目录完整路径
	string dirp;                      //系统调用参数(一般用于Pathname)的指针

	int arg[5];                       //存放当前系统调用参数
	/* 系统调用相关成员 */
	uint32 ar0[5];                    //    指向核心栈现场保护区中EAX寄存器
                                      //存放的栈单元，本字段存放该栈单元的地址。
                                      //在V6中r0存放系统调用的返回值给用户程序，
                                      //x86平台上使用EAX存放返回值，替代u.ar0[R0]
	ErrorCode userErrorCode;          //存放错误码
	ProcessOpenFile ofiles;           //进程打开文件描述符表对象
	IOParameter IOParam;              //记录当前读、写文件的偏移量，用户目标区域和剩余字节数参数
	SystemCall* systemCall;
	string ls;

public:
	UserCall();
	~UserCall();

	void userLs();
	void userCd(string dirName);
	void userMkDir(string dirName);
	void userCreate(string fileName);
	void userDelete(string fileName);
	void userOpen(string fileName);
	void userClose(string fd);
	void userSeek(string fd, string offset, string origin);
	void userWrite(string fd, string inFile, string size);
	void userRead(string fd, string outFile, string size);
	void userRename(string ori, string cur);  //重命名文件、文件夹
	void userTree(string path);               //打印树状目录
	
private:
	bool checkError();
	bool checkPathName(string path);
	void __userCd__(string dirName);
	void __userTree__(string path, int depth);//内部打印树状目录
};