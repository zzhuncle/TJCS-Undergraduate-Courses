#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "Common.h"
#include "CacheBlock.h"

/* 内存INode
*     实现 INode 的各种操作，包括将文件的逻辑块号转换成对应的物
* 理盘块号等功能。
*/
class INode
{
public:
	//INodeFlag中的标志位
	enum INodeFlag 
	{
		IUPD = 0x1,//内存INode被修改过，需要更新对应外存INode
		IACC = 0x2 //内存INode被访问过，需要修改最近一次访问时间
	};

	static const unsigned int IALLOC = 0x8000;    //文件被使用
	static const unsigned int IFMT = 0x6000;      //文件类型掩码
	static const unsigned int IFDIR = 0x4000;     //文件类型：目录文件
	static const unsigned int ILARG = 0x1000;     //文件长度类型：大型或巨型文件
	static const unsigned int IREAD = 0x100;      //对文件的读权限
	static const unsigned int IWRITE = 0x80;      //对文件的写权限

	static const int BLOCK_SIZE = 512;                                        //文件逻辑块大小：512字节
	static const int ADDRESS_PER_INDEX_BLOCK = BLOCK_SIZE / sizeof(int);      //每个间接索引表（或索引块）包含的物理盘块号
	
	static const int SMALL_FILE_BLOCK = 6;                                    //小型文件：直接索引表最多可寻址的逻辑块号
	static const int LARGE_FILE_BLOCK = 128 * 2 + 6;                          //大型文件：经一次间接索引表最多可寻址的逻辑块号
	static const int HUGE_FILE_BLOCK = 128 * 128 * 2 + 128 * 2 + 6;           //巨型文件：经二次间接索引最大可寻址文件逻辑块号
    
	unsigned int i_flag;	//状态的标志位，定义见enum INodeFlag
	unsigned int i_mode;	//文件工作方式信息
	int		i_count;		//引用计数
	int		i_nlink;		//文件联结计数，即该文件在目录树中不同路径名的数量
	int		i_number;		//*** 外存INode区中的编号 *** 
	                          //被拷贝到内存的 Inode 中的索引节点数据需要知道它来自于哪个外存 DiskInode，
	                          //以便于将来内存副本被修改之后更新到外存对应的 DiskInode 中去
	int		i_size;			//文件大小，字节为单位
	int		i_addr[10];		//用于文件逻辑块号和物理块号转换的基本索引表
	int		i_lastr;		//存放最近一次读取文件的逻辑块号，用于判断是否需要预读
public:
	INode();
	~INode();

	void Reset()
	{
		i_mode = 0;
		i_count = 0;
		i_number = -1;
		i_size = 0;
		memset(i_addr, 0, sizeof(i_addr));
	}
	void ReadI();                           //根据Inode对象中的物理磁盘块索引表，读取相应的文件数据
	void WriteI();                          //根据Inode对象中的物理磁盘块索引表，将数据写入文件
	int Bmap(int lbn);                      //将文件的逻辑块号转换成对应的物理盘块号
	void IUpdate(int time);                 //更新外存Inode的最后的访问时间、修改时间
	void ITrunc();                          //释放Inode对应文件占用的磁盘块
	void Clean();                           //清空Inode对象中的数据
	void ICopy(CacheBlock* bp, int inumber);//将包含外存Inode字符块中信息拷贝到内存Inode中
};

/* 磁盘INode
*      磁盘中 INode 结构，是 INode 的简化版本
*/
class DiskINode//64字节 
{
public:
	unsigned int d_mode;//状态的标志位，定义见enum INodeFlag
	int	d_nlink;		//文件联结计数，即该文件在目录树中不同路径名的数量
	int	d_size;			//文件大小，字节为单位
	int	d_addr[10];		//用于文件逻辑块号和物理块号转换的基本索引表
	int	d_atime;		//最后访问时间
	int	d_mtime;		//最后修改时间
	
	int padding;
public:
	DiskINode();
	~DiskINode();
};
