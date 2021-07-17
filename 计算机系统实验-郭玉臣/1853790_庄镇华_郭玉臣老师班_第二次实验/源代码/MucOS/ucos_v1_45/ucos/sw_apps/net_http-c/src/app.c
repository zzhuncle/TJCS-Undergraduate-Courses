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
*   This demonstration project illustrate a basic HTTP client demo project.
*
*   By default some configuration steps are required to compile this example :
*
*   1. Include the require Micrium software components
*       In the BSP setting dialog in the "overview" section of the left pane the following libraries
*       should be added to the BSP :
*
*           ucos_common
*           ucos_osiii
*           ucos_tcpip
*           ucos_dhcp-c
*           ucos_dns-c
*           ucos_http-c
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
*   4. Network interface configuration
*       In the BSP network interface section selects the desired network intergace and ip configuration.
*       Leave dhcp enabled if you'd like the embbeded dhcp client to configure the interface.
*
*   Troubleshooting :
*       By default the Xilinx SDK may not have selected the Micrium drivers for the timer and UART.
*       If that is the case they must be manually selected in the drivers configuration section.
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
#include  <Client/Source/http-c.h>

/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/

void  MainTask (void *p_arg);

#define  HTTP_SERVER_HOSTNAME                            "nist.time.gov"

#define  HTTPc_APP_CFG_CONN_NBR_MAX                       5u
#define  HTTPc_APP_CFG_REQ_NBR_MAX                        5u
#define  HTTPc_APP_CFG_CONN_BUF_SIZE                    512u


HTTPc_CONN_OBJ        HTTPcApp_ConnTbl[HTTPc_APP_CFG_CONN_NBR_MAX];
HTTPc_REQ_OBJ         HTTPcApp_ReqTbl[HTTPc_APP_CFG_REQ_NBR_MAX];
HTTPc_RESP_OBJ        HTTPcApp_RespTbl[HTTPc_APP_CFG_REQ_NBR_MAX];

CPU_CHAR              HTTPcApp_ConnBufTbl[HTTPc_APP_CFG_CONN_NBR_MAX][HTTPc_APP_CFG_CONN_BUF_SIZE];

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
    HTTPc_CONN_OBJ     *p_conn;
    HTTPc_REQ_OBJ      *p_req;
    HTTPc_RESP_OBJ     *p_resp;
    CPU_CHAR           *p_buf;
    HTTPc_FLAGS         flags;
    CPU_SIZE_T          str_len;
    CPU_BOOLEAN         result;
    HTTPc_ERR           err_httpc;
    NET_ERR             net_err;
    OS_ERR              os_err;

    UCOS_Print ("Application start\r\n");

    UCOS_Print("Delaying until the link is up.\r\n");
    NetIF_LinkStateWaitUntilUp (1, 1000, 500, &net_err);
    OSTimeDlyHMSM(0, 0, 1, 0, OS_OPT_TIME_HMSM_STRICT, &os_err);

    p_conn = &HTTPcApp_ConnTbl[0];
    p_req  = &HTTPcApp_ReqTbl[0];
    p_resp = &HTTPcApp_RespTbl[0];
    p_buf  = &HTTPcApp_ConnBufTbl[0][0];


    /* ----------- INIT NEW CONNECTION & REQUEST ---------- */
    HTTPc_ConnClr(p_conn, &err_httpc);
    if (err_httpc != HTTPc_ERR_NONE) {
        UCOS_Printf("Error initializing new connection. HTTPc_ConnClr() return error code %d\r\n", &err_httpc);
    }

    HTTPc_ReqClr(p_req, &err_httpc);
    if (err_httpc != HTTPc_ERR_NONE) {
        UCOS_Printf("Error initializing new request. HTTPc_ConnClr() return error code %d\r\n", &err_httpc);
    }

        /* ----------------- OPEN CONNECTION ------------------ */
    str_len = Str_Len(HTTP_SERVER_HOSTNAME);
    flags = HTTPc_FLAG_NONE;
    result = HTTPc_ConnOpen(p_conn,
            p_buf,
            HTTPc_APP_CFG_CONN_BUF_SIZE,
            HTTP_SERVER_HOSTNAME,
            str_len,
            flags,
            &err_httpc);
    if (err_httpc != HTTPc_ERR_NONE) {
        UCOS_Printf("Error opening connection. HTTPc_ConnOpen() returned error code %d\r\n", err_httpc);
    }

    if (result == DEF_OK) {
        UCOS_Printf("Connection to server succeeded.\n\r");
    } else {
        UCOS_Printf("Connection to server failed.\r\n");
    }

    if (result == DEF_OK) {
        UCOS_Print ("Querying current time from nist.time.gov/actualtime.cgi\r\n");
                                                                /* ---------------- SEND HTTP REQUEST ----------------- */
        str_len = Str_Len("/actualtime.cgi");
        flags = HTTPc_FLAG_NONE;
        result = HTTPc_ReqSend(p_conn,
                p_req,
                p_resp,
                HTTP_METHOD_GET,
                "/actualtime.cgi",
                str_len,
                flags,
                &err_httpc);
        if (err_httpc != HTTPc_ERR_NONE) {
            UCOS_Printf("Error sending request. HTTPc_ReqSend() returned error code %d\r\n", err_httpc);
        }

        if (result == DEF_OK) {
            UCOS_Printf("Request successful. Response content:\r\n");
            UCOS_Printf("%s\n\r", p_buf);
        }
    }


    while (DEF_TRUE) {
        OSTimeDlyHMSM(0, 0, 10, 0, OS_OPT_TIME_HMSM_STRICT, &os_err);
        UCOS_Print("Periodic output every 10 seconds from the main task\r\n");
    }

}
