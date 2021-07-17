#include "SystemCall.h"
#include "UserCall.h"
#include "Common.h"
#include <iostream>
#include <fstream>
#include <QDebug>
#include <QTextBrowser>
using namespace std;

extern SystemCall mySystemCall;
extern QTextBrowser *gCmd;
//    extern是一个关键字，它告诉编译器存在着一个变量或者一个函数，如果在当前编译语句的前面
//中没有找到相应的变量或者函数，也会在当前文件的后面或者其它文件中定义

UserCall::UserCall() 
{
	userErrorCode = U_NOERROR;  //存放错误码
	systemCall = &mySystemCall; 

	dirp = "/";                                   //系统调用参数(一般用于Pathname)的指针
	curDirPath = "/";                             //当前工作目录完整路径
	nowDirINodePointer = systemCall->rootDirINode;//指向当前目录的Inode指针
	paDirINodePointer = NULL;                     //指向父目录的Inode指针
	memset(arg, 0, sizeof(arg));                  //指向核心栈现场保护区中EAX寄存器
}

UserCall::~UserCall() {}

//此函数改变Usercall对象的dirp(系统调用参数 一般用于Pathname)数据成员
//只检查文件名是否过长
bool UserCall::checkPathName(string path)
{
    QString input;
    if (path.empty()) {
        input = "参数路径为空\n";
        gCmd->append(input);
        return false;
    }

    if (path[0] == '/' || path.substr(0, 2) != "..")
        dirp = path;            //系统调用参数(一般用于Pathname)的指针
    else {
        if (curDirPath.back() != '/')
            curDirPath += '/';
        string pre = curDirPath;//当前工作目录完整路径 cd命令才会改变curDirPath的值
        unsigned int p = 0;
        //可以多重相对路径 .. ../../
        for (; pre.length() > 3 && p < path.length() && path.substr(p, 2) == ".."; ) {
            pre.pop_back();
            pre.erase(pre.find_last_of('/') + 1);
            p += 2;
            p += p < path.length() && path[p] == '/';
        }
        dirp = pre + path.substr(p);
    }

    if (dirp.length() > 1 && dirp.back() == '/')
        dirp.pop_back();

    for (unsigned int p = 0, q = 0; p < dirp.length(); p = q + 1) {
        q = dirp.find('/', p);
        q = Common::min(q, (unsigned int)dirp.length());
        if (q - p > DirectoryEntry::DIRSIZ) {
            input = "文件名或文件夹名过长\n";
            gCmd->append(input);
            return false;
        }
    }
    return true;
}
void UserCall::userMkDir(string dirName) 
{
	if (!checkPathName(dirName))
		return;
	arg[1] = INode::IFDIR;//存放当前系统调用参数 文件类型：目录文件
	systemCall->Creat();
	checkError();
}

void UserCall::userLs() 
{
	ls.clear();
	systemCall->Ls();
	if (checkError())
		return;
    QString input = QString::fromStdString(ls);
    gCmd->append(input);
}

void UserCall::userCd(string dirName) 
{
	if (!checkPathName(dirName))
		return;
	systemCall->ChDir();
	checkError();
}

void UserCall::userCreate(string fileName) 
{
	if (!checkPathName(fileName))
		return;
	arg[1] = (INode::IREAD | INode::IWRITE);//存放当前系统调用参数
	systemCall->Creat();
	checkError();
}

void UserCall::userDelete(string fileName) 
{
	if (!checkPathName(fileName))
		return;
	systemCall->UnLink();
	checkError();
}

void UserCall::userOpen(string fileName) 
{
	if (!checkPathName(fileName))
		return;
	arg[1] = (File::FREAD | File::FWRITE);//存放当前系统调用参数
	systemCall->Open();
	if (checkError())
		return;
    QString input = "打开文件成功，返回的文件句柄fd为" + QString::number(ar0[UserCall::EAX]) + "\n";
    gCmd->append(input);
}

//传入sfd句柄
void UserCall::userClose(string sfd) 
{
	arg[0] = stoi(sfd);//存放当前系统调用参数
	systemCall->Close();
	checkError();
}

void UserCall::userSeek(string sfd, string offset, string origin) 
{
	arg[0] = stoi(sfd);
	arg[1] = stoi(offset);
	arg[2] = stoi(origin);
	systemCall->Seek();
	checkError();
}

void UserCall::userWrite(string sfd, string inFile, string size) 
{
    QString input;
	int fd = stoi(sfd), usize = 0;
	if (size.empty() || (usize = stoi(size)) < 0) {
        input = "参数必须大于等于零 ! \n";
        gCmd->append(input);
		return;
	}
	char* buffer = new char[usize];
	fstream fin(inFile, ios::in | ios::binary);
	if (!fin.is_open()) {
        input = "打开文件" + QString::fromStdString(inFile) + "失败\n";
        gCmd->append(input);
		return;
	}
	fin.read(buffer, usize);
	fin.close();
	arg[0] = fd;
	arg[1] = (int)buffer;
	arg[2] = usize;
	systemCall->Write();

	if (checkError())
		return;
    input = "成功写入" + QString::number(ar0[UserCall::EAX]) + "字节\n";
    gCmd->append(input);
	delete[] buffer;
}

void UserCall::userRead(string sfd, string outFile, string size) 
{
	int fd = stoi(sfd);
	int usize = stoi(size);
	char* buffer = new char[usize];
	arg[0] = fd;
	arg[1] = (int)buffer;
	arg[2] = usize;
	systemCall->Read();
	if (checkError())
		return;

    QString input = "成功读出" + QString::number(ar0[UserCall::EAX]) + "字节\n";
    gCmd->append(input);
    QString loopStr = "";
	if (outFile == "std") {
		for (uint32 i = 0; i < ar0[UserCall::EAX]; ++i)
            loopStr = loopStr + (char)buffer[i];
        gCmd->append(loopStr);
		delete[] buffer;
		return;
	}
	else {
		fstream fout(outFile, ios::out | ios::binary);
		if (!fout) {
            QString errStr = "打开文件" + QString::fromStdString(outFile) + "失败\n";
            gCmd->append(errStr);
			return;
		}
		fout.write(buffer, ar0[UserCall::EAX]);
		fout.close();
		delete[] buffer;
		return;
	}
}

bool UserCall::checkError() 
{
    QString input;
	if (userErrorCode != U_NOERROR) {
		switch (userErrorCode) {
		case UserCall::U_ENOENT:
            input = "找不到文件或者文件夹\n";
            gCmd->append(input);
			break;
		case UserCall::U_EBADF:
            input = "找不到文件句柄\n";
            gCmd->append(input);
			break;
		case UserCall::U_EACCES:
            input = "权限不足\n";
            gCmd->append(input);
			break;
		case UserCall::U_ENOTDIR:
            input = "文件夹不存在\n";
            gCmd->append(input);
			break;
		case UserCall::U_ENFILE:
            input = "文件表溢出\n";
            gCmd->append(input);
			break;
		case UserCall::U_EMFILE:
            input = "打开文件过多\n";
            gCmd->append(input);
			break;
		case UserCall::U_EFBIG:
            input = "文件过大\n";
            gCmd->append(input);
			break;
		case UserCall::U_ENOSPC:
            input = "磁盘空间不足\n";
            gCmd->append(input);
			break;
		default:
			break;
		}

		userErrorCode = U_NOERROR;
		return true;
	}
	return false;
}

void UserCall::userRename(string ori, string cur)
{
    string curDir = curDirPath;
    if (!checkPathName(ori))
        return;
    if (!checkPathName(cur))
        return;
    if (ori.find('/') != string::npos) {
        string nextDir = ori.substr(0, ori.find_last_of('/'));
        userCd(nextDir);
        ori = ori.substr(ori.find_last_of('/') + 1);
        if (cur.find('/') != string::npos)
            cur = cur.substr(cur.find_last_of('/') + 1);
    }
    systemCall->Rename(ori, cur);
    userCd(curDir);
    if (checkError())
        return;
}

void UserCall::__userTree__(string path, int depth)
{
    QString input = "";
    vector<string> dirs;
    string nextDir;
    ls.clear();
    systemCall->Ls();
    for (int i = 0, p = 0; i < ls.length(); ) {
        p = ls.find('\n', i);
        dirs.emplace_back(ls.substr(i, p - i));
        i = p + 1;
    }
    for (int i = 0; i < dirs.size(); i++) {
        input = "";
        nextDir = (path == "/" ? "" : path) + '/' + dirs[i];
        for (int j = 0; j < depth + 1; j++)
            input += "|   ";
        input += "|---" + QString::fromStdString(dirs[i]) + "\n";
        gCmd->insertPlainText(input);
        __userCd__(nextDir);
        if (userErrorCode != UserCall::U_NOERROR) {//访问到文件
            userErrorCode = UserCall::U_NOERROR;
            continue;
        }
        __userTree__(nextDir, depth + 1);
    }
    __userCd__(path);
    return;
}

void UserCall::userTree(string path)
{
    QString input = "";
    if (curDirPath.length() > 1 && curDirPath.back() == '/')
        curDirPath.pop_back();
    string curDir = curDirPath;

    if (path == "")
        path = curDir;

    if (!checkPathName(path))
        return;

    path = dirp;
    __userCd__(path);
    if (userErrorCode != UserCall::U_NOERROR) {//访问到文件
        input = "目录路径不存在！\n";
        gCmd->append(input);
        userErrorCode = UserCall::U_NOERROR;
        __userCd__(curDir);
        return;
    }
    input += "\n|---" + (path == "/" ? QString("/") : QString::fromStdString(path.substr(path.find_last_of('/') + 1))) + "\n";
    gCmd->insertPlainText(input);
    __userTree__(path, 0);
    __userCd__(curDir);
}

void UserCall::__userCd__(string dirName)
{
    if (!checkPathName(dirName))
        return;
    systemCall->ChDir();
}
