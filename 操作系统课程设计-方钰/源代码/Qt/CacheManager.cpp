#include "CacheManager.h"
#include "Common.h"
#include <QDebug>
#include <QTextBrowser>

extern DiskDriver myDiskDriver;
extern QTextBrowser *gCmd;

//CacheBlock只用到了两个标志，B_DONE和B_DELWRI，分别表示已经完成IO和延迟写的标志。
//空闲Buffer无任何标志
CacheManager::CacheManager() 
{
	bufferList = new CacheBlock;
	InitList();
	diskDriver = &myDiskDriver;
}

CacheManager::~CacheManager() 
{
	Bflush();
	delete bufferList;
}

void CacheManager::FormatBuffer() 
{
	for (int i = 0; i < NBUF; ++i)
		nBuffer[i].Reset();
	InitList();
}

void CacheManager::InitList() 
{
	for (int i = 0; i < NBUF; ++i) {
		if (i) 
			nBuffer[i].forw = nBuffer + i - 1;
		else {
			nBuffer[i].forw = bufferList;
			bufferList->back = nBuffer + i;
		}

		if (i + 1 < NBUF)
			nBuffer[i].back = nBuffer + i + 1;
		else {
			nBuffer[i].back = bufferList;
			bufferList->forw = nBuffer + i;
		}
		nBuffer[i].addr = buffer[i];
		nBuffer[i].no = i;
	}
}

//采用LRU Cache 算法，每次从头部取出，使用后放到尾部
void CacheManager::DetachNode(CacheBlock* pb) 
{
	if (pb->back == NULL)
		return;
	pb->forw->back = pb->back;
	pb->back->forw = pb->forw;
	pb->back = NULL;
	pb->forw = NULL;
}

void CacheManager::InsertTail(CacheBlock* pb) 
{
	if (pb->back != NULL)
		return;
	pb->forw = bufferList->forw;
	pb->back = bufferList;
	bufferList->forw->back = pb;
	bufferList->forw = pb;
}

//申请一块缓存，从缓存队列中取出，用于读写设备上的块blkno
CacheBlock* CacheManager::GetBlk(int blkno) 
{
	CacheBlock* pb;
	if (map.find(blkno) != map.end()) {
		pb = map[blkno];
		DetachNode(pb);
		return pb;
	}
	pb = bufferList->back;
	if (pb == bufferList) {
        QString input = "无缓存块可供使用\n";
        gCmd->append(input);
		return NULL;
	}
	DetachNode(pb);
	map.erase(pb->blkno);
	if (pb->flags & CacheBlock::CB_DELWRI)
		diskDriver->write(pb->addr, BUFFER_SIZE, pb->blkno * BUFFER_SIZE);
	pb->flags &= ~(CacheBlock::CB_DELWRI | CacheBlock::CB_DONE);
	pb->blkno = blkno;
	map[blkno] = pb;
	return pb;
}

//释放缓存控制块buf
void CacheManager::Brelse(CacheBlock* pb) 
{
	InsertTail(pb);
}

//读一个磁盘块，blkno为目标磁盘块逻辑块号
CacheBlock* CacheManager::Bread(int blkno) 
{
	CacheBlock* pb = GetBlk(blkno);
	if (pb->flags & (CacheBlock::CB_DONE | CacheBlock::CB_DELWRI)) 
		return pb;
	diskDriver->read(pb->addr, BUFFER_SIZE, pb->blkno * BUFFER_SIZE);
	pb->flags |= CacheBlock::CB_DONE;
	return pb;
}

//写一个磁盘块
void CacheManager::Bwrite(CacheBlock* pb) 
{
	pb->flags &= ~(CacheBlock::CB_DELWRI);
	diskDriver->write(pb->addr, BUFFER_SIZE, pb->blkno * BUFFER_SIZE);
	pb->flags |= (CacheBlock::CB_DONE);
	this->Brelse(pb);
}

//延迟写磁盘块
void CacheManager::Bdwrite(CacheBlock* bp) 
{
	bp->flags |= (CacheBlock::CB_DELWRI | CacheBlock::CB_DONE);
	this->Brelse(bp);
	return;
}

//清空缓冲区内容
void CacheManager::Bclear(CacheBlock* bp) 
{
	memset(bp->addr, 0, CacheManager::BUFFER_SIZE);
	return;
}

//将队列中延迟写的缓存全部输出到磁盘
void CacheManager::Bflush() 
{
	CacheBlock* pb = NULL;
	for (int i = 0; i < NBUF; ++i) {
		pb = nBuffer + i;
		if ((pb->flags & CacheBlock::CB_DELWRI)) {
			pb->flags &= ~(CacheBlock::CB_DELWRI);
			diskDriver->write(pb->addr, BUFFER_SIZE, pb->blkno * BUFFER_SIZE);
			pb->flags |= (CacheBlock::CB_DONE);
		}
	}
}
