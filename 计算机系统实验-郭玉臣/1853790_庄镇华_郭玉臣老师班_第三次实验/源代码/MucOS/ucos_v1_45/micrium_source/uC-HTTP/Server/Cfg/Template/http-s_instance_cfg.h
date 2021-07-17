/*
*********************************************************************************************************
*                                               uC/HTTP
*                                     Hypertext Transfer Protocol
*
*                         (c) Copyright 2004-2019; Silicon Laboratories Inc.,
*                                400 W. Cesar Chavez, Austin, TX 78701
*
*                   All rights reserved. Protected by international copyright laws.
*
*                  Your use of this software is subject to your acceptance of the terms
*                  of a Silicon Labs Micrium software license, which can be obtained by
*                  contacting info@micrium.com. If you do not agree to the terms of this
*                  license, you may not use this software.
*
*                  Please help us continue to provide the Embedded community with the finest
*                  software available. Your honesty is greatly appreciated.
*
*                    You can find our product's documentation at: doc.micrium.com
*
*                          For more information visit us at: www.micrium.com
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                               HTTP INSTANCE SERVER CONFIGURATION FILE
*
*                                               TEMPLATE
*
* Filename : http-s_instance_cfg.h
* Version  : V3.00.08
*********************************************************************************************************
*/

#include  <Server/Source/http-s.h>
#include  <Source/net_type.h>


/*
*********************************************************************************************************
*********************************************************************************************************
*                                   FILES & FOLDERS STRING DEFINES
*********************************************************************************************************
*********************************************************************************************************
*/

#define  HTTPs_CFG_INSTANCE_STR_FOLDER_ROOT               "\\"

#define  HTTPs_CFG_INSTANCE_STR_FILE_DEFAULT              "index.html"
#define  HTTPs_CFG_INSTANCE_STR_FILE_ERR_404              "404.html"

#define  HTTPs_CFG_INSTANCE_STR_FOLDER_UPLOAD             "\\"


/*
*********************************************************************************************************
*********************************************************************************************************
*                                   HTTP SERVER INSTANCE CONFIGURATION
*********************************************************************************************************
*********************************************************************************************************
*/

extern  const  NET_TASK_CFG  HTTPs_TaskCfgInstance;
extern  const  HTTPs_CFG     HTTPs_CfgInstance;

