/*
*********************************************************************************************************
*                                             uCOS XSDK BSP
*
*                      (c) Copyright 2014-2020; Silicon Laboratories Inc.,
*                             400 W. Cesar Chavez, Austin, TX 78701
*
*                All rights reserved. Protected by international copyright laws.
*
*               Your use of this software is subject to your acceptance of the terms
*               of a Silicon Labs Micrium software license, which can be obtained by
*               contacting info@micrium.com. If you do not agree to the terms of this
*               license, you may not use this software.
*
*               Please help us continue to provide the Embedded community with the finest
*               software available. Your honesty is greatly appreciated.
*
*                 You can find our product's documentation at: doc.micrium.com
*
*                       For more information visit us at: www.micrium.com
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                          SETUP INSTRUCTIONS
*
*   This demonstration project illustrate a basic uC/FS project with simple file operations.
*
*   By default some configuration steps are required to compile this example :
*
*   1. Include the require Micrium software components
*       In the BSP setting dialog in the "overview" section of the left pane the following libraries
*       should be added to the BSP :
*
*           ucos_common
*           ucos_osiii
*           ucos_fs
*           ucos_standalone
*
*   2. Kernel tick source - (Not required on the Zynq-7000 PS)
*       If a suitable timer is available in your FPGA design it can be used as the kernel tick source.
*       To do so, in the "ucos" section select a timer for the "kernel_tick_src" configuration option.
*
*   3. STDOUT configuration
*       Output from the print() and UCOS_Print() functions can be redirected to a supported UART. In
*       the "ucos_standalone" section the stdout configuration will list the available UARTs.
*
*   4. File system volume configuration
*       Two different storage media can be used for this example either a temporary ram based disk or
*       an sd card. The sd card example is only supported on the Zynq-7000 and Zynq UltraScale+ MPSoC at this moment.
*
*       a. To configure a ram disk from the ucos BSP configuration dialog expand the RAMDISK category. Then set the
*          UCOS_RAMDISK configuration to true.
*
*       b. To configure and SD Card from the ucos BSP configuration dialog expand the SD Card cateory. From there,
*       select the desired SD Card interface available in your hardware design.
*
*   5. (Optional) Additional information can be printed on the STDOUT terminal by uC/FS. Trace output can be enabled from
*      the ucos_fs configuration with the FS_TRACE_LEVEL configuration.
*
*   Troubleshooting :
*       By default the Xilinx SDK may not have selected the Micrium drivers for the timer, UART or SD Card.
*       If that is the case they must be manually selected in the drivers configuration section.
*
*       The default configuration for the ucos BSP only allocated one device and volume to reduce ram
*       usage. If both the ram disk and sd card are needed at the same time the ucos_fs runtime configuration
*       must be adjusted accordingly.
*
*       Finally make sure the FPGA is programmed before debugging.
*
*
*   Remember that this example is provided for evaluation purposes only. Commercial development requires
*   a valid license from Micrium.
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#include  <stdio.h>
#include  <Source/os.h>
#include  <ucos_bsp.h>

#include  <Source/fs.h>
#include  <Source/fs_file.h>
#include  <Source/fs_vol.h>
#include  <Source/fs_entry.h>

#include  <xparameters.h>

/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/

void  MainTask (void *p_arg);


/*
*********************************************************************************************************
*                                               main()
*
* Description : Entry point for C code.
*
*********************************************************************************************************
*/

int main()
{

    UCOSStartup(MainTask);

    return 0;
}


/*
*********************************************************************************************************
*                                             MainTask()
*
* Description : Startup task example code.
*
* Returns     : none.
*
* Created by  : main().
*********************************************************************************************************
*/

void  MainTask (void *p_arg)
{
    OS_ERR       os_err;
    CPU_CHAR     *vol_name;
    FS_VOL_INFO   vol_info;
    FS_ENTRY_INFO entry_info;
    FS_ERR        fs_err;
    FS_FILE      *test_file;
    CPU_CHAR      file_name[32] = {0};
    CPU_CHAR     *test_string;
    CPU_CHAR      rd_buf[64] = {0};
    CPU_SIZE_T    rd_cnt;

    UCOS_Print ("Application start\r\n");

#if (UCOS_SDCARD_EN == DEF_ENABLED)
    vol_name = "sdcard:0:";
#elif (UCOS_RAMDISK_EN == DEF_ENABLED)
    vol_name = "ram:0:";
#else
    UCOS_Print("No RAMDisk or SD Card configured\r\n");
#endif

    UCOS_Printf("Querying volume %s...\r\n", vol_name);

    FSVol_Query(vol_name, &vol_info, &fs_err);
    if (fs_err != FS_ERR_NONE) {
        UCOS_Printf("Error querying volume. FSVol_Query returned with error code %d\r\n", fs_err);
    }

    UCOS_Printf("Total volume size in sectors : %d\r\n", vol_info.VolTotSecCnt);
    UCOS_Printf("Used size in sectors : %d\r\n", vol_info.VolUsedSecCnt);

    UCOS_Print("Creating file micrium_test.txt\r\n");
    Str_Cat(file_name, vol_name);
    Str_Cat(file_name, "\\micrium_test.txt");
    test_file = FSFile_Open(file_name, FS_FILE_ACCESS_MODE_TRUNCATE | FS_FILE_ACCESS_MODE_CREATE | FS_FILE_ACCESS_MODE_WR, &fs_err);
    if (fs_err != FS_ERR_NONE) {
        UCOS_Printf("Error opening file. FSFile_Open returned with error code %d\r\n", fs_err);
        }

    UCOS_Print("Writing hello world to test file\r\n");
    test_string = "hello world";
    FSFile_Wr(test_file, test_string, Str_Len(test_string), &fs_err);
    if (fs_err != FS_ERR_NONE) {
        UCOS_Printf("Error writing to file. FSFile_Wr returned with error code %d\r\n", fs_err);
    }

    UCOS_Print("Closing test file\r\n");
    FSFile_Close(test_file, &fs_err);
    if (fs_err != FS_ERR_NONE) {
        UCOS_Printf("Error closing file. FSFile_Close returned with error code %d\r\n", fs_err);
    }

    UCOS_Print("Re-opening file in read-only mode to check result\r\n");
    test_file = FSFile_Open(file_name, FS_FILE_ACCESS_MODE_RD, &fs_err);
    if (fs_err != FS_ERR_NONE) {
        UCOS_Printf("Error opening file. FSFile_Open returned with error code %d\r\n", fs_err);
    }

    UCOS_Print("Querying file info\r\n");
    FSFile_Query(test_file, &entry_info, &fs_err);
    if (fs_err != FS_ERR_NONE) {
        UCOS_Printf("Error querying file. FSFile_Query returned with error code %d\r\n", fs_err);
    }

    UCOS_Printf("Size of file micrium_test.txt: %d bytes\r\n", entry_info.Size);

    UCOS_Print("Reading from file.\r\n");
    rd_cnt = FSFile_Rd(test_file, rd_buf, 64, &fs_err);
    if (fs_err != FS_ERR_NONE) {
        UCOS_Printf("Error reading from file. FSFile_Rd returned with error code %d\r\n", fs_err);
    }

    UCOS_Printf("Read %d bytes from file. Read data : %s\r\n", rd_cnt, rd_buf);

    UCOS_Print("Closing file.\r\n");
    FSFile_Close(test_file, &fs_err);
    if (fs_err != FS_ERR_NONE) {
        UCOS_Printf("Error closing file. FSFile_Close returned with error code %d\r\n", fs_err);
    }

    while (DEF_TRUE) {
        OSTimeDlyHMSM(0, 0, 10, 0, OS_OPT_TIME_HMSM_STRICT, &os_err);
        UCOS_Print("Periodic output every 10 seconds from the main task\r\n");
    }

}

