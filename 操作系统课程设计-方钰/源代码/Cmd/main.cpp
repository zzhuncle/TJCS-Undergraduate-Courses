#define _CRT_SECURE_NO_WARNINGS
#include "DiskDriver.h"
#include "CacheManager.h"
#include "OpenFileManager.h"
#include "SystemCall.h"
#include "UserCall.h"
#include <iostream>
#include <sstream>
#include <unordered_map>
using namespace std;

DiskDriver myDiskDriver;
CacheManager myCacheManager;
OpenFileTable myOpenFileTable;
SuperBlock mySuperBlock;
FileSystem myFileSystem;
INodeTable myINodeTable;
SystemCall mySystemCall;
UserCall myUserCall;

bool AutoTest()
{
	UserCall& User = myUserCall;
	cout << "注意：自动测试不包含格式化操作" << endl;
	cout << "由于测试程序中的文件句柄写定，所以如果之前打开过文件可能会执行出错，建议第一步就自动测试" << endl;
	cout << "[1853790-ZZH-OS " << User.curDirPath << " ]$ " << "mkdir /bin" << endl;
	User.userMkDir("/bin");
	cout << "[1853790-ZZH-OS " << User.curDirPath << " ]$ " << "mkdir /etc" << endl;
	User.userMkDir("/etc");
	cout << "[1853790-ZZH-OS " << User.curDirPath << " ]$ " << "mkdir /home" << endl;
	User.userMkDir("/home");
	cout << "[1853790-ZZH-OS " << User.curDirPath << " ]$ " << "mkdir /dev" << endl;
	User.userMkDir("/dev");
	cout << "[1853790-ZZH-OS " << User.curDirPath << " ]$ " << "ls" << endl;
	User.userLs();

	cout << "[1853790-ZZH-OS " << User.curDirPath << " ]$ " << "mkdir /home/texts" << endl;
	User.userMkDir("/home/texts");
	cout << "[1853790-ZZH-OS " << User.curDirPath << " ]$ " << "mkdir /home/reports" << endl;
	User.userMkDir("/home/reports");
	cout << "[1853790-ZZH-OS " << User.curDirPath << " ]$ " << "mkdir /home/photos" << endl;
	User.userMkDir("/home/photos");
	cout << "[1853790-ZZH-OS " << User.curDirPath << " ]$ " << "ftree /" << endl;
	User.userTree("/");

	cout << "[1853790-ZZH-OS " << User.curDirPath << " ]$ " << "cd /home/texts" << endl;
	User.userCd("/home/texts");
	cout << "[1853790-ZZH-OS " << User.curDirPath << " ]$ " << "fcreate Readme.txt" << endl;
	User.userCreate("Readme.txt");
	cout << "[1853790-ZZH-OS " << User.curDirPath << " ]$ " << "fopen Readme.txt" << endl;
	User.userOpen("Readme.txt");
	cout << "[1853790-ZZH-OS " << User.curDirPath << " ]$ " << "fwrite 8 Readme.txt 2609" << endl;
	User.userWrite("8", "Readme.txt", "2609");
	cout << "[1853790-ZZH-OS " << User.curDirPath << " ]$ " << "fseek 8 0 begin" << endl;
	User.userSeek("8", "0", "0");
	cout << "[1853790-ZZH-OS " << User.curDirPath << " ]$ " << "fread 8 ReadmeOut.txt 2609" << endl;
	User.userRead("8", "ReadmeOut.txt", "2609");
	cout << "[1853790-ZZH-OS " << User.curDirPath << " ]$ " << "fclose 8" << endl;
	User.userClose("8");

	cout << "[1853790-ZZH-OS " << User.curDirPath << " ]$ " << "cd /home/reports" << endl;
	User.userCd("/home/reports");
	cout << "[1853790-ZZH-OS " << User.curDirPath << " ]$ " << "fcreate Report.pdf" << endl;
	User.userCreate("Report.pdf");
	cout << "[1853790-ZZH-OS " << User.curDirPath << " ]$ " << "fopen Report.pdf" << endl;
	User.userOpen("Report.pdf");
	cout << "[1853790-ZZH-OS " << User.curDirPath << " ]$ " << "fwrite 9 Report.pdf 1604288" << endl; 
	User.userWrite("9", "Report.pdf", "1604288");
	cout << "[1853790-ZZH-OS " << User.curDirPath << " ]$ " << "fseek 9 0 begin" << endl; 
	User.userSeek("9", "0", "0");
	cout << "[1853790-ZZH-OS " << User.curDirPath << " ]$ " << "fread 9 ReportOut.pdf 1604288" << endl;
	User.userRead("9", "ReportOut.pdf", "1604288");
	cout << "[1853790-ZZH-OS " << User.curDirPath << " ]$ " << "fclose 9" << endl;
	User.userClose("9");

	cout << "[1853790-ZZH-OS " << User.curDirPath << " ]$ " << "cd /home/photos" << endl;
	User.userCd("/home/photos");
	cout << "[1853790-ZZH-OS " << User.curDirPath << " ]$ " << "fcreate DennisRitchie.jpg" << endl;
	User.userCreate("DennisRitchie.jpg");
	cout << "[1853790-ZZH-OS " << User.curDirPath << " ]$ " << "fopen DennisRitchie.jpg" << endl;
	User.userOpen("DennisRitchie.jpg");
	cout << "[1853790-ZZH-OS " << User.curDirPath << " ]$ " << "fwrite 10 DennisRitchie.jpg 7402" << endl;
	User.userWrite("10", "DennisRitchie.jpg", "7402");
	cout << "[1853790-ZZH-OS " << User.curDirPath << " ]$ " << "fread 10 DennisRitchieOut.jpg 7402" << endl;
	User.userSeek("10", "0", "0");
	cout << "[1853790-ZZH-OS " << User.curDirPath << " ]$ " << "fclose 10" << endl;
	User.userRead("10", "DennisRitchieOut.jpg", "7402");

	cout << "[1853790-ZZH-OS " << User.curDirPath << " ]$ " << "mkdir /test" << endl; 
	User.userMkDir("/test");
	cout << "[1853790-ZZH-OS " << User.curDirPath << " ]$ " << "cd /test" << endl; 
	User.userCd("/test");
	cout << "[1853790-ZZH-OS " << User.curDirPath << " ]$ " << "fcreate Jerry" << endl;
	User.userCreate("Jerry");
	cout << "[1853790-ZZH-OS " << User.curDirPath << " ]$ " << "fopen Jerry" << endl; 
	User.userOpen("Jerry");
	cout << "[1853790-ZZH-OS " << User.curDirPath << " ]$ " << "fwrite 13 input.txt 800" << endl;
	User.userWrite("13", "input.txt", "800");
	cout << "[1853790-ZZH-OS " << User.curDirPath << " ]$ " << "fseek 13 500 begin" << endl;
	User.userSeek("13", "500", "0");
	cout << "[1853790-ZZH-OS " << User.curDirPath << " ]$ " << "fread 13 std 500" << endl; 
	User.userRead("13", "std", "500");
	cout << "[1853790-ZZH-OS " << User.curDirPath << " ]$ " << "fseek 13 500 begin" << endl;
	User.userSeek("13", "500", "0");
	cout << "[1853790-ZZH-OS " << User.curDirPath << " ]$ " << "fread 13 abc.txt 500" << endl; 
	User.userRead("13", "abc.txt", "500");
	cout << "[1853790-ZZH-OS " << User.curDirPath << " ]$ " << "fwrite 13 abc.txt 300" << endl;
	User.userWrite("13", "abc.txt", "300");
	cout << "[1853790-ZZH-OS " << User.curDirPath << " ]$ " << "fclose 13" << endl;
	User.userClose("13");
	cout << "[1853790-ZZH-OS " << User.curDirPath << " ]$ " << "ftree /" << endl;
	User.userTree("/");
	cout << "[1853790-ZZH-OS " << User.curDirPath << " ]$ " << "cd /test" << endl;
	User.userCd("/test");
	cout << "[1853790-ZZH-OS " << User.curDirPath << " ]$ " << "frename Jerry Larry" << endl;
	User.userRename("Jerry", "Larry");
	cout << "[1853790-ZZH-OS " << User.curDirPath << " ]$ " << "ls" << endl;
	User.userLs();
	cout << "[1853790-ZZH-OS " << User.curDirPath << " ]$ " << "ftree /" << endl;
	User.userTree("/");

	cout << "自动测试结束" << endl << endl;
	return true;
}

int main() 
{
	UserCall& User = myUserCall;
	cout << "***************************************************************************************" << endl
		<< "*                                                                                     *" << endl
		<< "*                                   类Unix文件系统                                    *" << endl
		<< "*                                                                                     *" << endl
		<< "* [操作说明]:                                                                         *" << endl
		<< "* [命令]:help <op_name>\t[功能]:命令提示                                               *" << endl
		<< "* [命令]:test\t[功能]:自动测试                                                       *" << endl
		<< "* [命令]:fformat\t[功能]:格式化文件系统                                         *" << endl
		<< "* [命令]:ls\t[功能]:查看当前目录内容                                               *" << endl
		<< "* [命令]:mkdir <dirname>\t[功能]:生成文件夹                                     *" << endl
		<< "* [命令]:cd <dirname>\t[功能]:进入目录                                               *" << endl
		<< "* [命令]:fcreate <filename>\t[功能]:创建文件名为filename的文件                     *" << endl
		<< "* [命令]:fopen <filename>\t[功能]:打开文件名为filename的文件                     *" << endl
		<< "* [命令]:fwrite <fd> <infile> <size>\t[功能]:从infile输入，写入fd文件size字节       *" << endl
		<< "* [命令]:fread <fd> <outfile> <size>\t[功能]:从fd文件读取size字节，输出到outfile    *" << endl
		<< "* [命令]:fread <fd> std <size>\t[功能]:从fd文件读取size字节，输出到屏幕               *" << endl
		<< "* [命令]:fseek <fd> <step> begin\t[功能]:以begin模式把fd文件指针偏移step        *" << endl
		<< "* [命令]:fseek <fd> <step> cur\t[功能]:以cur模式把fd文件指针偏移step                  *" << endl
		<< "* [命令]:fseek <fd> <step> end\t[功能]:以end模式把fd文件指针偏移step                  *" << endl
		<< "* [命令]:fclose <fd>\t[功能]:关闭文件句柄为fd的文件                                 *" << endl
		<< "* [命令]:fdelete <filename>\t[功能]:删除文件文件名为filename的文件或者文件夹       *" << endl
		<< "* [命令]:frename <filename> <filename1>\t[功能]:将文件fliename重命名为filename1        *" << endl
		<< "* [命令]:ftree <dirname>\t[功能]:列出dirname的文件目录树                        *" << endl
		<< "* [命令]:exit \t[功能]:退出系统，并将缓存内容存至磁盘                                 *" << endl
		<< "***************************************************************************************" << endl;

	string line, opt, val[3];
	while (true) 
	{
		cout << "[1853790-ZZH-OS " << User.curDirPath << " ]$ ";
		getline(cin, line);
		if (line.size() == 0) 
			continue;

		stringstream in(line);
		in >> opt;
		val[0] = val[1] = val[2] = "";
		
		//格式化文件系统
		if (opt == "fformat") {
			//Us.userCd("/");
			myOpenFileTable.Reset();
			myINodeTable.Reset();
			myCacheManager.FormatBuffer();
			myFileSystem.FormatDevice();
			//myUserCall.ofiles.Reset();
			cout << "格式化完毕，文件系统已退出，请重新启动！" << endl;
			return 0;
		}
		//查看当前目录内容
		else if (opt == "ls")
			User.userLs();
		//生成文件夹
		else if (opt == "mkdir") {
			in >> val[0];
			if (val[0][0] != '/')
				val[0] = User.curDirPath + val[0];
			User.userMkDir(val[0]);
		}
		//进入目录
		else if (opt == "cd") {
			in >> val[0];
			User.userCd(val[0]);
		}
		//创建文件名为filename的文件
		else if (opt == "fcreate") {
			in >> val[0];
			if (val[0][0] != '/')
				val[0] = User.curDirPath + val[0];
			User.userCreate(val[0]);
		}
		//打开文件名为filename的文件
		else if (opt == "fopen") {
			in >> val[0];
			if (myUserCall.ar0[UserCall::EAX] == 0) {
				User.userMkDir("demo");
				User.userDelete("demo");
			}
			if (val[0][0] != '/')
				val[0] = User.curDirPath + val[0];
			
			User.userOpen(val[0]);
		}
		//退出系统，并将缓存内容存至磁盘
		else if (opt == "exit")
			return 0;
		//关闭文件句柄为fd的文件
		else if (opt == "fclose") {
			in >> val[0];
			User.userClose(val[0]);
		}
		else if (opt == "fseek") {
			in >> val[0] >> val[1] >> val[2];
			//以begin模式把fd文件指针偏移step
			if (val[2] == "begin")
				User.userSeek(val[0], val[1], string("0"));
			//以cur模式把fd文件指针偏移step
			else if (val[2] == "cur")
				User.userSeek(val[0], val[1], string("1"));
			//以end模式把fd文件指针偏移step
			else if (val[2] == "end")
				User.userSeek(val[0], val[1], string("2"));
		}
		//从fd文件读取size字节，输出到outfile
		//从fd文件读取size字节，输出到屏幕
		else if (opt == "fread") {
			in >> val[0] >> val[1] >> val[2];
			User.userRead(val[0], val[1], val[2]);
		}
		//从infile输入，写入fd文件size字节
		else if (opt == "fwrite") {
			in >> val[0] >> val[1] >> val[2];
			User.userWrite(val[0], val[1], val[2]);
		}
		//删除文件文件名为filename的文件或者文件夹
		else if (opt == "fdelete") {
			in >> val[0];
			if (val[0][0] != '/')
				val[0] = User.curDirPath + val[0];
			User.userDelete(val[0]);
		}
		else if (opt == "test")
			AutoTest();
		//重命名文件或文件夹
		else if (opt == "frename") {
			in >> val[0] >> val[1];
			User.userRename(val[0], val[1]);
		}
		else if (opt == "ftree") {
			in >> val[0];
			User.userTree(val[0]);
		}
		else if (opt == "help") {
			in >> val[0];
			if (val[0] == "" || val[0] == "?") {
				cout << "[命令]:test\t[功能]:自动测试" << endl
					<< "[命令]:fformat\t[功能]:格式化文件系统" << endl
					<< "[命令]:ls\t[功能]:查看当前目录内容" << endl
					<< "[命令]:mkdir <dirname>\t[功能]:生成文件夹" << endl
					<< "[命令]:cd <dirname>\t[功能]:进入目录" << endl
					<< "[命令]:fcreate <filename>\t[功能]:创建文件名为filename的文件" << endl
					<< "[命令]:fopen <filename>\t[功能]:打开文件名为filename的文件" << endl
					<< "[命令]:fwrite <fd> <infile> <size>\t[功能]:从infile输入，写入fd文件size字节" << endl
					<< "[命令]:fread <fd> <outfile> <size>\t[功能]:从fd文件读取size字节，输出到outfile" << endl
					<< "[命令]:fread <fd> std <size>\t[功能]:从fd文件读取size字节，输出到屏幕" << endl
					<< "[命令]:fseek <fd> <step> begin\t[功能]:以begin模式把fd文件指针偏移step" << endl
					<< "[命令]:fseek <fd> <step> cur\t[功能]:以cur模式把fd文件指针偏移step" << endl
					<< "[命令]:fseek <fd> <step> end\t[功能]:以end模式把fd文件指针偏移step" << endl
					<< "[命令]:fclose <fd>\t[功能]:关闭文件句柄为fd的文件" << endl
					<< "[命令]:fdelete <filename>\t[功能]:删除文件文件名为filename的文件或者文件夹" << endl
					<< "[命令]:frename <filename> <filename1>\t[功能]:将文件fliename重命名为filename1" << endl
					<< "[命令]:ftree <dirname>\t[功能]:列出dirname的文件目录树" << endl
					<< "[命令]:exit\t[功能]:退出系统，并将缓存内容存至磁盘" << endl;
			}
			else if (val[0] == "test")
				cout << "[命令]:test\t[功能]:自动测试" << endl;
			else if (val[0] == "fformat")
				cout << "[命令]:fformat\t[功能]:格式化文件系统" << endl;
			else if (val[0] == "ls")
				cout << "[命令]:ls\t[功能]:查看当前目录内容" << endl;
			else if (val[0] == "mkdir")
				cout << "[命令]:mkdir <dirname>\t[功能]:生成文件夹" << endl;
			else if (val[0] == "cd")
				cout << "[命令]:cd <dirname>\t[功能]:进入目录" << endl;
			else if (val[0] == "fcreate")
				cout << "[命令]:fcreate <filename>\t[功能]:创建文件名为filename的文件" << endl;
			else if (val[0] == "fopen")
				cout << "[命令]:fopen <filename>\t[功能]:打开文件名为filename的文件" << endl;
			else if (val[0] == "fwrite")
				cout << "[命令]:fwrite <fd> <infile> <size>\t[功能]:从infile输入，写入fd文件size字节" << endl;
			else if (val[0] == "fread")
				cout << "[命令]:fread <fd> <outfile> <size>\t[功能]:从fd文件读取size字节，输出到outfile" << endl
				<< "[命令]:fread <fd> std <size>\t[功能]:从fd文件读取size字节，输出到屏幕" << endl;
			else if (val[0] == "fseek")
				cout << "[命令]:fseek <fd> <step> begin\t[功能]:以begin模式把fd文件指针偏移step" << endl
				<< "[命令]:fseek <fd> <step> cur\t[功能]:以cur模式把fd文件指针偏移step" << endl
				<< "[命令]:fseek <fd> <step> end\t[功能]:以end模式把fd文件指针偏移step" << endl;
			else if (val[0] == "fclose")
				cout << "[命令]:fclose <fd>\t[功能]:关闭文件句柄为fd的文件" << endl;
			else if (val[0] == "fdelete")
				cout << "[命令]:fdelete <filename>\t[功能]:删除文件文件名为filename的文件或者文件夹" << endl;
			else if (val[0] == "frename")
				cout << "[命令]:frename <filename> <filename1>\t[功能]:将文件fliename重命名为filename1" << endl;
			else if (val[0] == "ftree")
				cout << "[命令]:ftree <dirname>\t[功能]:列出dirname的文件目录树" << endl;
			else if (val[0] == "exit")
				cout << "[命令]:exit\t[功能]:退出系统，并将缓存内容存至磁盘" << endl;
		}
	}
	return 0;
}