UCOS III操作系统移植+应用程序开发

一、底层开发板
    本项目基于Nexys 4 ddr开发板进行开发。使用时将开发板和计算机用USB连接到指定的USB端口以获取数据。

二、开发平台和开发工具
1. Vivado 2019.1
2. Xilinx SDK
3. vivado boards-support-packet
4. ucos_v1_45

三、文件结构
1. 板机CPU开发：MucOS.srcs/sources_1
2. 管脚约束：MucOS.srcs/constrs_1
3. UCOS III操作系统包：MucOS/ucos_v1_45/ucos
4. 应用程序：MucOS/MucOS.sdk/MucOS
5. 应用程序板级支持包：MucOS/MucOS.sdk/MucOS_bsp

四、使用方法
    使用Vivado生成二进制比特流，使用Xilinx SDK打开基于该比特流的工程文件，将ucos系统添加到库文件中，开发完成后编程FPGA下板。
以上工作均已全部完成，如果仅使用本项目成果，使用Xilink SDK打开工程文件，编程FPGA下板即可。