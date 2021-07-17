/*
*********************************************************************************************************
*                                                uC/FS
*                                      The Embedded File System
*
*                         (c) Copyright 2008-2019; Silicon Laboratories Inc.,
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
*                                      FILE SYSTEM DEVICE DRIVER
*
*                                             SD/MMC CARD
*                                              CARD MODE
*
* Filename     : fs_dev_sd_card.h
* Version      : V4.07.07
*********************************************************************************************************
* Reference(s) : (1) SD Card Association.  "Physical Layer Simplified Specification Version 2.00".
*                    July 26, 2006.
*
*                (2) JEDEC Solid State Technology Association.  "MultiMediaCard (MMC) Electrical
*                    Standard, High Capacity".  JESD84-B42.  July 2007.
*********************************************************************************************************
* Note(s)      : (1) This driver has been tested with MOST SD/MMC media types, including :
*
*                    (a) Standard capacity SD cards, v1.x & v2.0.
*                    (b) SDmicro cards.
*                    (c) High capacity SD cards (SDHC)
*                    (d) MMC
*                    (e) MMCplus
*
*                    It should also work with devices conformant to the relevant SD or MMC specifications.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                               MODULE
*********************************************************************************************************
*/

#ifndef  FS_DEV_SD_CARD_PRESENT
#define  FS_DEV_SD_CARD_PRESENT


/*
*********************************************************************************************************
*                                               EXTERNS
*********************************************************************************************************
*/

#ifdef   FS_DEV_SD_CARD_MODULE
#define  FS_DEV_SD_CARD_EXT
#else
#define  FS_DEV_SD_CARD_EXT  extern
#endif


/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#include  "../../../Source/fs_dev.h"
#include  "../fs_dev_sd.h"


/*
*********************************************************************************************************
*                                               DEFINES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                       RESPONSE TYPES DEFINES
*
* Note(s) : (a) The response type data stored in the FSDev_SD_Card_CmdRespType[] table encodes the
*               the SD response to the low nibble while the MMC response type is encoded as the
*               high nibble
*********************************************************************************************************
*/

#define  FS_DEV_SD_CARD_RESP_TYPE_UNKNOWN                  0u
#define  FS_DEV_SD_CARD_RESP_TYPE_NONE                     1u          /* No  Response.                                        */
#define  FS_DEV_SD_CARD_RESP_TYPE_R1                       2u          /* R1  Response: Normal Response Command.               */
#define  FS_DEV_SD_CARD_RESP_TYPE_R1B                      3u          /* R1b Response.                                        */
#define  FS_DEV_SD_CARD_RESP_TYPE_R2                       4u          /* R2  Response: CID, CSD Register.                     */
#define  FS_DEV_SD_CARD_RESP_TYPE_R3                       5u          /* R3  Response: OCR Register.                          */
#define  FS_DEV_SD_CARD_RESP_TYPE_R4                       6u          /* R4  Response: Fast I/O Response (MMC).               */
#define  FS_DEV_SD_CARD_RESP_TYPE_R5                       7u          /* R5  Response: Interrupt Request Response (MMC).      */
#define  FS_DEV_SD_CARD_RESP_TYPE_R5B                      8u          /* R5B Response.                                        */
#define  FS_DEV_SD_CARD_RESP_TYPE_R6                       9u          /* R6  Response: Published RCA Response.                */
#define  FS_DEV_SD_CARD_RESP_TYPE_R7                      10u          /* R7  Response: Card Interface Condition.              */

#define  FS_DEV_MMC_RESP_TYPE_UNKNOWN               (0u << 4u)
#define  FS_DEV_MMC_RESP_TYPE_NONE                  (1u << 4u)         /* No  Response.                                        */
#define  FS_DEV_MMC_RESP_TYPE_R1                    (2u << 4u)         /* R1  Response: Normal Response Command.               */
#define  FS_DEV_MMC_RESP_TYPE_R1B                   (3u << 4u)         /* R1b Response.                                        */
#define  FS_DEV_MMC_RESP_TYPE_R2                    (4u << 4u)         /* R2  Response: CID, CSD Register.                     */
#define  FS_DEV_MMC_RESP_TYPE_R3                    (5u << 4u)         /* R3  Response: OCR Register.                          */
#define  FS_DEV_MMC_RESP_TYPE_R4                    (6u << 4u)         /* R4  Response: Fast I/O Response (MMC).               */
#define  FS_DEV_MMC_RESP_TYPE_R5                    (7u << 4u)         /* R5  Response: Interrupt Request Response (MMC).      */
#define  FS_DEV_MMC_RESP_TYPE_R5B                   (8u << 4u)         /* R5B Response.                                        */
#define  FS_DEV_MMC_RESP_TYPE_R6                    (9u << 4u)         /* R6  Response: Published RCA Response.                */
#define  FS_DEV_MMC_RESP_TYPE_R7                    10u << 4u)         /* R7  Response: Card Interface Condition.              */

#define  FS_DEV_SD_CARD_RESP_TYPE_MASK                   0x0F          /* SD Card response mask.                               */
#define  FS_DEV_SD_CARD_RESP_TYPE_OFFSET                 0x00          /* SD Card response offset.                             */
#define  FS_DEV_MMC_RESP_TYPE_MASK                       0xF0          /* MMC response mask.                                   */
#define  FS_DEV_MMC_RESP_TYPE_OFFSET                     0x04          /* MCC response offset.                                 */

/*
*********************************************************************************************************
*                                          CMD FLAG DEFINES
*
* Note(s) : (a) If FS_DEV_SD_CARD_CMD_FLAG_INIT is set, then the controller should send the 80-clock
*               initialization sequence before transmitting the command.
*
*           (b) If FS_DEV_SD_CARD_CMD_FLAG_OPEN_DRAIN is set, then the command should be transmitted
*               with the command line in an open drain state.
*
*           (c) If FS_DEV_SD_CARD_CMD_FLAG_BUSY is set, then the controller should check for busy after
*               the response before transmitting any data.
*
*           (d) If FS_DEV_SD_CARD_CMD_FLAG_CRC_VALID is set, then CRC check should be enabled.
*
*           (e) If FS_DEV_SD_CARD_CMD_FLAG_IX_VALID is set, then index check should be enabled.
*
*           (f) If FS_DEV_SD_CARD_CMD_FLAG_START_DATA_TX is set, then this command will either start
*               transmitting data, or expect to start receiving data.
*
*           (g) If FS_DEV_SD_CARD_CMD_FLAG_STOP_DATA_TX is set, then this command is attempting to stop
*               (abort) data transmission/reception.
*
*           (h) If FS_DEV_SD_CARD_CMD_FLAG_RESP is set, then a response is expected for this command.
*
*           (i) If FS_DEV_SD_CARD_CMD_FLAG_RESP_LONG is set, then a long response is expected for this
*               command.
*********************************************************************************************************
*/

#define  FS_DEV_SD_CARD_CMD_FLAG_NONE             DEF_BIT_NONE
#define  FS_DEV_SD_CARD_CMD_FLAG_INIT             DEF_BIT_00    /* Initializaton sequence before command.               */
#define  FS_DEV_SD_CARD_CMD_FLAG_BUSY             DEF_BIT_01    /* Busy signal expected after command.                  */
#define  FS_DEV_SD_CARD_CMD_FLAG_CRC_VALID        DEF_BIT_02    /* CRC valid after command.                             */
#define  FS_DEV_SD_CARD_CMD_FLAG_IX_VALID         DEF_BIT_03    /* Index valid after command.                           */
#define  FS_DEV_SD_CARD_CMD_FLAG_OPEN_DRAIN       DEF_BIT_04    /* Command line is open drain.                          */
#define  FS_DEV_SD_CARD_CMD_FLAG_START_DATA_TX    DEF_BIT_05    /* Data start command.                                  */
#define  FS_DEV_SD_CARD_CMD_FLAG_STOP_DATA_TX     DEF_BIT_06    /* Data stop  command.                                  */
#define  FS_DEV_SD_CARD_CMD_FLAG_RESP             DEF_BIT_07    /* Response expected.                                   */
#define  FS_DEV_SD_CARD_CMD_FLAG_RESP_LONG        DEF_BIT_08    /* Long response expected.                              */

/*
*********************************************************************************************************
*                                       DATA DIR & TYPE DEFINES
*
* Note(s) : (a) The data direction will be otherwise than 'none' if & only if the command flag
*               FS_DEV_SD_CARD_CMD_FLAG_DATA_START is set.
*********************************************************************************************************
*/

#define  FS_DEV_SD_CARD_DATA_DIR_NONE                      0u   /* No data transfer.                                    */
#define  FS_DEV_SD_CARD_DATA_DIR_HOST_TO_CARD              1u   /* Transfer host-to-card (write).                       */
#define  FS_DEV_SD_CARD_DATA_DIR_CARD_TO_HOST              2u   /* Transfer card-to-host (read).                        */

#define  FS_DEV_SD_CARD_DATA_TYPE_NONE                     0u   /* No data transfer.                                    */
#define  FS_DEV_SD_CARD_DATA_TYPE_SINGLE_BLOCK             1u   /* Single data block.                                   */
#define  FS_DEV_SD_CARD_DATA_TYPE_MULTI_BLOCK              2u   /* Multiple data blocks.                                */
#define  FS_DEV_SD_CARD_DATA_TYPE_STREAM                   3u   /* Stream data.                                         */

/*
*********************************************************************************************************
*                                             CARD ERRORS
*
* Note(s) : (a) One of these errors should be returned from the BSP from 'FSDev_SD_Card_BSP_CmdStart()',
*               'FSDev_SD_Card_BSP_CmdWaitEnd()', 'FSDev_SD_Card_BSP_CmdDataRd()' & 'FSDev_SD_Card_BSP_CmdDataWr()'.
*********************************************************************************************************
*/

typedef enum fs_dev_sd_card_err {

    FS_DEV_SD_CARD_ERR_NONE                     =     0u,       /* No error.                                            */
    FS_DEV_SD_CARD_ERR_NO_CARD                  =     1u,       /* No card present.                                     */
    FS_DEV_SD_CARD_ERR_BUSY                     =     2u,       /* Controller is busy.                                  */
    FS_DEV_SD_CARD_ERR_UNKNOWN                  =     3u,       /* Unknown or other error.                              */
    FS_DEV_SD_CARD_ERR_WAIT_TIMEOUT             =     4u,       /* Timeout in waiting for cmd response/data.            */
    FS_DEV_SD_CARD_ERR_RESP_TIMEOUT             =     5u,       /* Timeout in receiving cmd response.                   */
    FS_DEV_SD_CARD_ERR_RESP_CHKSUM              =     6u,       /* Error in response checksum.                          */
    FS_DEV_SD_CARD_ERR_RESP_CMD_IX              =     7u,       /* Response command index err.                          */
    FS_DEV_SD_CARD_ERR_RESP_END_BIT             =     8u,       /* Response end bit error.                              */
    FS_DEV_SD_CARD_ERR_RESP                     =     9u,       /* Other response error.                                */
    FS_DEV_SD_CARD_ERR_DATA_UNDERRUN            =    10u,       /* Data underrun.                                       */
    FS_DEV_SD_CARD_ERR_DATA_OVERRUN             =    11u,       /* Data overrun.                                        */
    FS_DEV_SD_CARD_ERR_DATA_TIMEOUT             =    12u,       /* Timeout in receiving data.                           */
    FS_DEV_SD_CARD_ERR_DATA_CHKSUM              =    13u,       /* Error in data checksum.                              */
    FS_DEV_SD_CARD_ERR_DATA_START_BIT           =    14u,       /* Data start bit error.                                */
    FS_DEV_SD_CARD_ERR_DATA                     =    15u        /* Other data error.                                    */

} FS_DEV_SD_CARD_ERR;


/*
*********************************************************************************************************
*                                             DATA TYPES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                      SD CARD COMMAND DATA TYPE
*********************************************************************************************************
*/

typedef  struct  fs_dev_sd_card_cmd {
    CPU_INT08U  Cmd;                                            /* Command number.                                      */
    CPU_INT32U  Arg;                                            /* Command argument.                                    */
    FS_FLAGS    Flags;                                          /* Command flags.                                       */
    CPU_INT08U  RespType;                                       /* Response type.                                       */
    CPU_INT08U  DataDir;                                        /* Data transfer direction.                             */
    CPU_INT08U  DataType;                                       /* Data transfer type.                                  */
    CPU_INT32U  BlkSize;                                        /* Size of block(s) in data transfer.                   */
    CPU_INT32U  BlkCnt;                                         /* Number of blocks in data transfer.                   */
} FS_DEV_SD_CARD_CMD;

/*
*********************************************************************************************************
*                                          GLOBAL VARIABLES
*********************************************************************************************************
*/

extern             const  FS_DEV_API  FSDev_SD_Card;
FS_DEV_SD_CARD_EXT        FS_CTR      FSDev_SD_Card_UnitCtr;


/*
*********************************************************************************************************
*                                               MACRO'S
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/

void         FSDev_SD_Card_QuerySD           (CPU_CHAR            *name_dev,    /* Get info about SD/MMC card.          */
                                              FS_DEV_SD_INFO      *p_info,
                                              FS_ERR              *p_err);

void         FSDev_SD_Card_RdCID             (CPU_CHAR            *name_dev,    /* Read SD/MMC Card ID reg.             */
                                              CPU_INT08U          *p_dest,
                                              FS_ERR              *p_err);

void         FSDev_SD_Card_RdCSD             (CPU_CHAR            *name_dev,    /* Read SD/MMC Card-Specific Data reg.  */
                                              CPU_INT08U          *p_dest,
                                              FS_ERR              *p_err);

/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*                               DEFINED IN BSP'S 'fs_dev_sd_card_bsp.c'
*********************************************************************************************************
*/

CPU_BOOLEAN  FSDev_SD_Card_BSP_Open             (FS_QTY               unit_nbr);   /* Open (init) SD/MMC card interface.   */

void         FSDev_SD_Card_BSP_Close            (FS_QTY               unit_nbr);   /* Close (uninit) SD/MMC card interface.*/

void         FSDev_SD_Card_BSP_Lock             (FS_QTY               unit_nbr);   /* Acquire SD/MMC card bus lock.        */

void         FSDev_SD_Card_BSP_Unlock           (FS_QTY               unit_nbr);   /* Release SD/MMC card bus lock.        */

void         FSDev_SD_Card_BSP_CmdStart         (FS_QTY               unit_nbr,    /* Start a command.                     */
                                                 FS_DEV_SD_CARD_CMD  *p_cmd,
                                                 void                *p_data,
                                                 FS_DEV_SD_CARD_ERR  *p_err);

void         FSDev_SD_Card_BSP_CmdWaitEnd       (FS_QTY               unit_nbr,    /* Wait for command end (& response).   */
                                                 FS_DEV_SD_CARD_CMD  *p_cmd,
                                                 CPU_INT32U          *p_resp,
                                                 FS_DEV_SD_CARD_ERR  *p_err);

void         FSDev_SD_Card_BSP_CmdDataRd        (FS_QTY               unit_nbr,    /* Read data following command.         */
                                                 FS_DEV_SD_CARD_CMD  *p_cmd,
                                                 void                *p_dest,
                                                 FS_DEV_SD_CARD_ERR  *p_err);

void         FSDev_SD_Card_BSP_CmdDataWr        (FS_QTY               unit_nbr,    /* Write data following command.        */
                                                 FS_DEV_SD_CARD_CMD  *p_cmd,
                                                 void                *p_src,
                                                 FS_DEV_SD_CARD_ERR  *p_err);

CPU_INT32U   FSDev_SD_Card_BSP_GetBlkCntMax     (FS_QTY               unit_nbr,    /* Get max block cnt for block size.    */
                                                 CPU_INT32U           blk_size);

CPU_INT08U   FSDev_SD_Card_BSP_GetBusWidthMax   (FS_QTY               unit_nbr);   /* Get max bus width, in bits.          */

void         FSDev_SD_Card_BSP_SetBusWidth      (FS_QTY               unit_nbr,    /* Set bus width.                       */
                                                 CPU_INT08U           width);

void         FSDev_SD_Card_BSP_SetClkFreq       (FS_QTY               unit_nbr,    /* Set clock frequency.                 */
                                                 CPU_INT32U           freq);

void         FSDev_SD_Card_BSP_SetTimeoutData   (FS_QTY               unit_nbr,    /* Set data timeout.                    */
                                                 CPU_INT32U           to_clks);

void         FSDev_SD_Card_BSP_SetTimeoutResp   (FS_QTY               unit_nbr,    /* Set response timeout.                */
                                                 CPU_INT32U           to_ms);


/*
*********************************************************************************************************
*                                        CONFIGURATION ERRORS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*/

#endif                                                          /* End of SD CARD module include.                       */
