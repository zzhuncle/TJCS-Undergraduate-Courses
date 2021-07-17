#include "Common.h"
#include "OpenFileManager.h"
#include "UserCall.h"
#include <ctime>

extern UserCall myUserCall;
extern INodeTable myINodeTable;
extern FileSystem myFileSystem;
extern CacheManager myCacheManager;

OpenFileTable::OpenFileTable() 
{
}

OpenFileTable::~OpenFileTable() 
{
}


void OpenFileTable::Reset() 
{
	for (int i = 0; i < OpenFileTable::MAX_FILES; ++i)
		sysFileTable[i].Reset();
}

//作用：进程打开文件描述符表中找的空闲项之下标写入 ar0[EAX]
File* OpenFileTable::FAlloc() 
{
	int fd = myUserCall.ofiles.AllocFreeSlot();
	if (fd < 0) 
		return NULL;
	for (int i = 0; i < OpenFileTable::MAX_FILES; ++i) {
		//count == 0表示该项空闲
		if (this->sysFileTable[i].count == 0) {
			myUserCall.ofiles.SetF(fd, &this->sysFileTable[i]);
			this->sysFileTable[i].count++;
			this->sysFileTable[i].offset = 0;
			return (&this->sysFileTable[i]);
		}
	}
	myUserCall.userErrorCode = UserCall::U_ENFILE;
	return NULL;
}

//对打开文件控制块File结构的引用计数count减1，若引用计数count为0，则释放File结构
void OpenFileTable::CloseF(File* pFile) 
{
	pFile->count--;
	if (pFile->count <= 0)
		myINodeTable.IPut(pFile->inode);
}

INodeTable::INodeTable() 
{
	fileSystem = &myFileSystem;
}

INodeTable::~INodeTable() 
{

}

void INodeTable::Reset() 
{
	INode emptyINode;
	for (int i = 0; i < INodeTable::NINODE; ++i)
		m_INodeTable[i].Reset();
}

//检查编号为inumber的外存INode是否有内存拷贝，如果有则返回该内存INode在内存INode表中的索引
int INodeTable::IsLoaded(int inumber) 
{
	for (int i = 0; i < NINODE; ++i) 
		if (m_INodeTable[i].i_number == inumber && m_INodeTable[i].i_count)
			return i;
	return -1;
}

//在内存INode表中寻找一个空闲的内存INode
INode* INodeTable::GetFreeINode() 
{
	for (int i = 0; i < INodeTable::NINODE; i++) 
		if (this->m_INodeTable[i].i_count == 0)
			return m_INodeTable + i;
	return NULL;
}

//根据外存INode编号获取对应INode。如果该INode已经在内存中，返回该内存INode；
//如果不在内存中，则将其读入内存后上锁并返回该内存INode，返回NULL:INode Table OverFlow
INode* INodeTable::IGet(int inumber) 
{
	INode* pINode;
	int index = IsLoaded(inumber);
	if (index >= 0) {
		pINode = m_INodeTable + index;
		++pINode->i_count;
		return pINode;
	}

	pINode = GetFreeINode();
	if (NULL == pINode) {
		cout << "内存 INode 表溢出!" << endl;
		myUserCall.userErrorCode = UserCall::U_ENFILE;
		return NULL;
	}

	pINode->i_number = inumber;
	pINode->i_count++;
	CacheBlock* pCache = myCacheManager.Bread(FileSystem::INODE_START_SECTOR + inumber / FileSystem::INODE_NUMBER_PER_SECTOR);
	pINode->ICopy(pCache, inumber);
	myCacheManager.Brelse(pCache);
	return pINode;
}

//减少该内存INode的引用计数，如果此INode已经没有目录项指向它，
//且无进程引用该INode，则释放此文件占用的磁盘块。
void INodeTable::IPut(INode* pINode) 
{
	//当前进程为引用该内存INode的唯一进程，且准备释放该内存INode
	if (pINode->i_count == 1) {
		//该文件已经没有目录路径指向它
		if (pINode->i_nlink <= 0) {
			//释放该文件占据的数据盘块
			pINode->ITrunc();
			pINode->i_mode = 0;
			//释放对应的外存INode
			this->fileSystem->IFree(pINode->i_number);
		}
		//更新外存INode信息
		pINode->IUpdate((int)time(NULL));
		//清除内存INode的所有标志位
		pINode->i_flag = 0;
		//这是内存inode空闲的标志之一，另一个是i_count == 0
		pINode->i_number = -1;
	}

	pINode->i_count--;
}

//将所有被修改过的内存INode更新到对应外存INode中
void INodeTable::UpdateINodeTable() 
{
	for (int i = 0; i < INodeTable::NINODE; ++i) 
		if (this->m_INodeTable[i].i_count) 
			this->m_INodeTable[i].IUpdate((int)time(NULL));
}