#include "Common.h"
#include "File.h"
#include "UserCall.h"

extern UserCall myUserCall;

File::File() 
{
	count = 0;
	inode = NULL;
	offset = 0;
}

File::~File() { }

void File::Reset() 
{
	count = 0;
	inode = NULL;
	offset = 0;
}

ProcessOpenFile::ProcessOpenFile() 
{
	memset(processOpenFileTable, 0, sizeof(processOpenFileTable));
}

ProcessOpenFile::~ProcessOpenFile() 
{
}

//进程请求打开文件时，在打开文件描述符表中分配一个空闲表项
int ProcessOpenFile::AllocFreeSlot() 
{
	for (int i = 0; i < ProcessOpenFile::MAX_FILES; i++)
		//进程打开文件描述符表中找到空闲项，则返回之
		if (!processOpenFileTable[i]) {
			myUserCall.ar0[UserCall::EAX] = i;
			return i;
		}

	myUserCall.ar0[UserCall::EAX] = -1; //Open1，需要一个标志。当打开文件结构创建失败时，可以回收系统资源
	myUserCall.userErrorCode = UserCall::U_EMFILE;
	return -1;
}

//根据用户系统调用提供的文件描述符参数fd，找到对应的打开文件控制块File结构
File* ProcessOpenFile::GetF(int fd) 
{
	File* pFile;

	if (fd < 0 || fd >= ProcessOpenFile::MAX_FILES) {
		myUserCall.userErrorCode = UserCall::U_EBADF;
		return NULL;
	}

	pFile = this->processOpenFileTable[fd];
	if (pFile == NULL)
		myUserCall.userErrorCode = UserCall::U_EBADF;
	return pFile;
}

//为已分配到的空闲描述符fd和已分配的打开文件表中空闲File对象建立勾连关系
void ProcessOpenFile::SetF(int fd, File* pFile) 
{
	if (fd < 0 || fd >= ProcessOpenFile::MAX_FILES)
		return;
	this->processOpenFileTable[fd] = pFile;
}
