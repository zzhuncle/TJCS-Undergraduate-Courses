#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <iostream>
using namespace std;

/* 缓存块
*     缓存对应的物理盘号，缓存的标志（是否已经和硬盘同步，或者是延迟写），
* 缓存编号以及缓存的前后指针等信息。
*/
//由于本次课程设计中不会存在多个设备，于是我取消了所有的设备队列，缓存块只会存在与 NODEV 队列中。
//分配和释放的操作也非常简单，分配是从队列头取第一个缓存块，释放时将该缓存块标志位置换后放在队列尾部。
class CacheBlock
{
public:
	//flags中的标志位
	enum CacheBlockFlag
	{
		CB_DONE = 0x1,    //I/O操作结束
		CB_DELWRI = 0x2   //延迟写，对应的缓存有其他用途时，将其内容写到对应的块设备上
	};
	unsigned int flags;   //缓存控制块标志位

	CacheBlock* forw;    
	CacheBlock* back;

	int wcount;		      //需传送的字节数
	unsigned char* addr;  //指向该缓存控制块所管理的缓冲区的首地址
	int	blkno;		      //磁盘逻辑块号
	int no;

	CacheBlock() 
	{
		flags = 0;
		forw = NULL;
		back = NULL;
		wcount = 0;
		addr = NULL;
		blkno = -1;
		no = 0;
	}

	void Reset() 
	{
		flags = 0;
		forw = NULL;
		back = NULL;
		wcount = 0;
		addr = NULL;
		blkno = -1;
		no = 0;
	}
};