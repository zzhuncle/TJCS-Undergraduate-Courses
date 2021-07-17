#include "Common.h"
#include "FileSystem.h"
#include "UserCall.h"
#include "DiskDriver.h"
#include "OpenFileManager.h"
#include "CacheManager.h"
#include <ctime>

#include <QDebug>
#include <QTextBrowser>

extern DiskDriver myDiskDriver;
extern CacheManager myCacheManager;
extern SuperBlock mySuperBlock;
extern INodeTable myINodeTable;
extern UserCall myUserCall;
extern QTextBrowser *gCmd;

FileSystem::FileSystem() 
{
	diskDriver = &myDiskDriver;
	superBlock = &mySuperBlock;
	cacheManager = &myCacheManager;

	if (!diskDriver->Exists())
		FormatDevice();
	else
		LoadSuperBlock();
}

FileSystem::~FileSystem() 
{
	Update();
	diskDriver = NULL;
	superBlock = NULL;
}

//格式化SuperBlock
void FileSystem::FormatSuperBlock() 
{
	superBlock->s_isize = FileSystem::INODE_SECTOR_NUMBER;
	superBlock->s_fsize = FileSystem::DISK_SECTOR_NUMBER;
	superBlock->s_nfree = 0;
	superBlock->s_free[0] = -1;
	superBlock->s_ninode = 0;
	superBlock->s_fmod = 0;
	time((time_t*)&superBlock->s_time);
}

//格式化整个文件系统
void FileSystem::FormatDevice() {
	FormatSuperBlock();
	diskDriver->Construct();
	diskDriver->write((uint8*)(superBlock), sizeof(SuperBlock), 0);

	DiskINode emptyDINode, rootDINode;
	rootDINode.d_mode |= INode::IALLOC | INode::IFDIR;
	diskDriver->write((uint8*)&rootDINode, sizeof(rootDINode));
	for (int i = 1; i < FileSystem::INODE_NUMBER_ALL; ++i) {
		if (superBlock->s_ninode < SuperBlock::MAX_NUMBER_INODE)
			superBlock->s_inode[superBlock->s_ninode++] = i;
		diskDriver->write((uint8*)&emptyDINode, sizeof(emptyDINode));
	}
	char freeBlock[BLOCK_SIZE], freeBlock1[BLOCK_SIZE];
	memset(freeBlock, 0, BLOCK_SIZE);
	memset(freeBlock1, 0, BLOCK_SIZE);

	for (int i = 0; i < FileSystem::DATA_SECTOR_NUMBER; ++i) {
		if (superBlock->s_nfree >= SuperBlock::MAX_NUMBER_FREE) {
			memcpy(freeBlock1, &superBlock->s_nfree, sizeof(int) + sizeof(superBlock->s_free));
			diskDriver->write((uint8*)&freeBlock1, BLOCK_SIZE);
			superBlock->s_nfree = 0;
		}
		else
			diskDriver->write((uint8*)freeBlock, BLOCK_SIZE);
		superBlock->s_free[superBlock->s_nfree++] = i + FileSystem::DATA_START_SECTOR;
	}

	time((time_t*)&superBlock->s_time);
	diskDriver->write((uint8*)(superBlock), sizeof(SuperBlock), 0);
}

//系统初始化时读入SuperBlock
void FileSystem::LoadSuperBlock() 
{
	diskDriver->read((uint8*)(superBlock), sizeof(SuperBlock), SUPERBLOCK_START_SECTOR * BLOCK_SIZE);
}

//将SuperBlock对象的内存副本更新到存储设备的SuperBlock中去
void FileSystem::Update() {
	CacheBlock* pCache;
	superBlock->s_fmod = 0;
	superBlock->s_time = (int)time(NULL);
	for (int j = 0; j < 2; j++) {
		int* p = (int*)superBlock + j * 128;
		pCache = this->cacheManager->GetBlk(FileSystem::SUPERBLOCK_START_SECTOR + j);
		memcpy(pCache->addr, p, BLOCK_SIZE);
		this->cacheManager->Bwrite(pCache);
	}
	myINodeTable.UpdateINodeTable();
	this->cacheManager->Bflush();
}

//在存储设备上分配空闲磁盘块
CacheBlock* FileSystem::Alloc() 
{
	int blkno;
	CacheBlock* pCache;
	//从索引表“栈顶”获取空闲磁盘块编号
	blkno = superBlock->s_free[--superBlock->s_nfree];

	//若获取磁盘块编号为零，则表示已分配尽所有的空闲磁盘块
	if (blkno <= 0) {
		superBlock->s_nfree = 0;
		myUserCall.userErrorCode = UserCall::U_ENOSPC;
		return NULL;
	}

	//栈已空，新分配到空闲磁盘块中记录了下一组空闲磁盘块的编号
    //将下一组空闲磁盘块的编号读入SuperBlock的空闲磁盘块索引表s_free[100]中
	if (superBlock->s_nfree <= 0) {
		pCache = this->cacheManager->Bread(blkno);
		int* p = (int*)pCache->addr;
		superBlock->s_nfree = *p++;
		memcpy(superBlock->s_free, p, sizeof(superBlock->s_free));
		this->cacheManager->Brelse(pCache);
	}
	pCache = this->cacheManager->GetBlk(blkno);
	if (pCache)
		this->cacheManager->Bclear(pCache);
	superBlock->s_fmod = 1;
	return pCache;
}

//在存储设备dev上分配一个空闲外存INode，一般用于创建新的文件
INode* FileSystem::IAlloc() 
{
	CacheBlock* pCache;
	INode* pINode;
	int ino;
	//SuperBlock直接管理的空闲Inode索引表已空，必须到磁盘上搜索空闲Inode
	if (superBlock->s_ninode <= 0) {
		ino = -1;
		for (int i = 0; i < superBlock->s_isize; ++i) {
			pCache = this->cacheManager->Bread(FileSystem::INODE_START_SECTOR + i);
			int* p = (int*)pCache->addr;
			for (int j = 0; j < FileSystem::INODE_NUMBER_PER_SECTOR; ++j) {
				++ino;
				int mode = *(p + j * FileSystem::INODE_SIZE / sizeof(int));
				if (mode)
					continue;
				//如果外存inode的i_mode == 0，此时并不能确定该inode是空闲的，
			    //因为有可能是内存inode没有写到磁盘上, 所以要继续搜索内存inode中是否有相应的项
				if (myINodeTable.IsLoaded(ino) == -1) {
					superBlock->s_inode[superBlock->s_ninode++] = ino;
					if (superBlock->s_ninode >= SuperBlock::MAX_NUMBER_INODE)
						break;
				}
			}

			this->cacheManager->Brelse(pCache);
			if (superBlock->s_ninode >= SuperBlock::MAX_NUMBER_INODE)
				break;
		}
		if (superBlock->s_ninode <= 0) {
			myUserCall.userErrorCode = UserCall::U_ENOSPC;
			return NULL;
		}
	}
	ino = superBlock->s_inode[--superBlock->s_ninode];
	pINode = myINodeTable.IGet(ino);
	if (NULL == pINode) {
        QString input = "无空闲内存存储INode\n";
        gCmd->append(input);
		return NULL;
	}

	pINode->Clean();
	superBlock->s_fmod = 1;
	return pINode;
}

//释放编号为number的外存INode，一般用于删除文件
void FileSystem::IFree(int number) 
{
	if (superBlock->s_ninode >= SuperBlock::MAX_NUMBER_INODE)
		return;
	superBlock->s_inode[superBlock->s_ninode++] = number;
	superBlock->s_fmod = 1;
}

//释放存储设备dev上编号为blkno的磁盘块
void FileSystem::Free(int blkno) 
{
	CacheBlock* pCache;
	if (superBlock->s_nfree >= SuperBlock::MAX_NUMBER_FREE) {
		pCache = this->cacheManager->GetBlk(blkno);
		int* p = (int*)pCache->addr;
		*p++ = superBlock->s_nfree;
		memcpy(p, superBlock->s_free, sizeof(int) * SuperBlock::MAX_NUMBER_FREE);
		superBlock->s_nfree = 0;
		this->cacheManager->Bwrite(pCache);
	}

	superBlock->s_free[superBlock->s_nfree++] = blkno;
	superBlock->s_fmod = 1;
}
