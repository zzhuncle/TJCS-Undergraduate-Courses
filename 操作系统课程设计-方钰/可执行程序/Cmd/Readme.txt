一、Cmd方式
	开发平台: Windows 10
	开发工具：VS2019 x86
	采用命令行交互方法，双击FileSystem.exe运行，input.txt、Readme.txt、Report.pdf、
DennisRitchie.jpg是输入源文件，请不要删除。
二、Qt方式
	开发平台: Windows 10
	开发工具：Qt Creator 5.9.0
	采用图形界面交互方法，可以通过输入框和按钮进行操作，双击FileSystem.exe运行，
input.txt、Readme.txt、Report.pdf、DennisRitchie.jpg是输入源文件，请不要删除。
三、命令说明                                                                         
	[命令]:fformat                         [功能]:格式化文件系统                                       
	[命令]:ls                              [功能]:查看当前目录内容                                               
	[命令]:mkdir <dirname>                 [功能]:生成文件夹                                     
	[命令]:cd <dirname>                    [功能]:进入目录                                               
	[命令]:fcreate <filename>              [功能]:创建文件名为filename的文件                     
	[命令]:fopen <fd>                      [功能]:打开文件句柄为fd的文件                                 
	[命令]:fwrite <fd> <infile> <size>     [功能]:从infile输入，写入fd文件size字节       
	[命令]:fread <fd> <outfile> <size>     [功能]:从fd文件读取size字节，输出到outfile    
	[命令]:fread <fd> std <size>           [功能]:从fd文件读取size字节，输出到屏幕               
	[命令]:fseek <fd> <step> begin         [功能]:以begin模式把fd文件指针偏移step        
	[命令]:fseek <fd> <step> cur           [功能]:以cur模式把fd文件指针偏移step                  
	[命令]:fseek <fd> <step> end           [功能]:以end模式把fd文件指针偏移step                  
	[命令]:fclose <fd>                     [功能]:关闭文件句柄为fd的文件                                 
	[命令]:fdelete <filename>              [功能]:删除文件文件名为filename的文件或者文件夹
	[命令]:frename <filename> <filename1>  [功能]:将文件fliename重命名为filename1     
	[命令]:ftree <dirname>                 [功能]:列出dirname的文件目录树 
	[命令]:exit                            [功能]:退出系统，并将缓存内容存至磁盘       
四、命令提示
	Cmd方式可以通过 help <指令名字> 来获取命令格式提示
	Qt 方式可以通过点击 <功能帮助> 按钮来获取命令格式提示