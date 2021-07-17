#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "Common.h"
#include "FileSystem.h"
#include "INode.h"
#include "OpenFileManager.h"

/*
*     SystemCall
*    封装了文件系统的各种系统调用在核心态下处理过程，
* 如对文件的Open()、Close()、Read()、Write()等等
* 封装了对文件系统访问的具体细节。
*/
class SystemCall
{
public:
	//目录搜索模式，用于NameI()函数
	enum DirectorySearchMode
	{
		OPEN = 0,   //以打开文件方式搜索目录
		CREATE = 1, //以新建文件方式搜索目录
		DELETE = 2  //以删除文件方式搜索目录
	};

public:

	INode* rootDirINode;         //根目录内存INode
	FileSystem* fileSystem;      //对全局对象g_FileSystem的引用，该对象负责管理文件系统存储资源
	INodeTable* inodeTable;      //对全局对象g_INodeTable的引用，该对象负责内存INode表的管理
	OpenFileTable* openFileTable;//对全局对象g_OpenFileTable的引用，该对象负责打开文件表项的管理

public:
	SystemCall();
	~SystemCall();
	void Open();                          //Open()系统调用处理过程
	void Creat();                         //Creat()系统调用处理过程
	void Open1(INode* pINode, int trf);   //Open()、Creat()系统调用的公共部分
	void Close();                         //Close()系统调用处理过程                  
	void Seek();                          //Seek()系统调用处理过程
	void Read();                          //Read()系统调用处理过程
	void Write();                         //Write()系统调用处理过程
	void Rdwr(enum File::FileFlags mode); //读写系统调用公共部分代码
	INode* NameI(enum DirectorySearchMode mode);//目录搜索，将路径转化为相应的INode返回上锁后的INode
	INode* MakNode(int mode);             //被Creat()系统调用使用，用于为创建新文件分配内核资源
	void UnLink();                        //取消文件
	void WriteDir(INode* pINode);         //向父目录的目录文件写入一个目录项
	void ChDir();                         //改变当前工作目录
	void Ls();                            //列出当前INode节点的文件项
	void Rename(string ori, string cur);  //重命名文件、文件夹
};
