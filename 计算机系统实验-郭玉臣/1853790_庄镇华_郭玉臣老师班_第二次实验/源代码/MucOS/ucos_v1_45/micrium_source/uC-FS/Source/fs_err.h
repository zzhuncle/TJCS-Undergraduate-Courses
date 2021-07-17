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
*                                  FILE SYSTEM ERROR CODE MANAGEMENT
*
* Filename : fs_err.h
* Version  : V4.07.07
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                               MODULE
*********************************************************************************************************
*/

#ifndef  FS_ERR_H
#define  FS_ERR_H


/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#include  <cpu.h>


/*
*********************************************************************************************************
*                                               EXTERNS
*********************************************************************************************************
*/

#ifdef   FS_ERR_MODULE
#define  FS_ERR_EXT
#else
#define  FS_ERR_EXT  extern
#endif


/*
*********************************************************************************************************
*                                               DEFINES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                       FILE SYSTEM ERROR CODES
*
* Note(s) : (1) All generic file system error codes are #define'd in 'fs_err.h';
*               Any device-specific     error codes are #define'd in device-specific header files.
*********************************************************************************************************
*/

typedef enum fs_err {

    FS_ERR_NONE                                 =     0u,

    FS_ERR_INVALID_ARG                          =    10u,       /* Invalid argument.                                    */
    FS_ERR_INVALID_CFG                          =    11u,       /* Invalid configuration.                               */
    FS_ERR_INVALID_CHKSUM                       =    12u,       /* Invalid checksum.                                    */
    FS_ERR_INVALID_LEN                          =    13u,       /* Invalid length.                                      */
    FS_ERR_INVALID_TIME                         =    14u,       /* Invalid date/time.                                   */
    FS_ERR_INVALID_TIMESTAMP                    =    15u,       /* Invalid timestamp.                                   */
    FS_ERR_INVALID_TYPE                         =    16u,       /* Invalid object type.                                 */
    FS_ERR_MEM_ALLOC                            =    17u,       /* Mem could not be alloc'd.                            */
    FS_ERR_NULL_ARG                             =    18u,       /* Arg(s) passed NULL val(s).                           */
    FS_ERR_NULL_PTR                             =    19u,       /* Ptr arg(s) passed NULL ptr(s).                       */
    FS_ERR_OS                                   =    20u,       /* OS err.                                              */
    FS_ERR_OVF                                  =    21u,       /* Value too large to be stored in type.                */
    FS_ERR_EOF                                  =    22u,       /* EOF reached.                                         */

    FS_ERR_WORKING_DIR_NONE_AVAIL               =    30u,       /* No working dir avail.                                */
    FS_ERR_WORKING_DIR_INVALID                  =    31u,       /* Working dir invalid.                                 */


/*
*********************************************************************************************************
*                                         BUFFER ERROR CODES
*********************************************************************************************************
*/

    FS_ERR_BUF_NONE_AVAIL                       =   100u,       /* No buf avail.                                        */


/*
*********************************************************************************************************
*                                          CACHE ERROR CODES
*********************************************************************************************************
*/

    FS_ERR_CACHE_INVALID_MODE                   =   200u,       /* Mode specified invalid.                              */
    FS_ERR_CACHE_INVALID_SEC_TYPE               =   201u,       /* Sector type specified invalid.                       */
    FS_ERR_CACHE_TOO_SMALL                      =   202u,       /* Cache specified too small.                           */


/*
*********************************************************************************************************
*                                         DEVICE ERROR CODES
*********************************************************************************************************
*/

    FS_ERR_DEV                                  =   300u,       /* Device access error.                                 */
    FS_ERR_DEV_ALREADY_OPEN                     =   301u,       /* Device already open.                                 */
    FS_ERR_DEV_CHNGD                            =   302u,       /* Device has changed.                                  */
    FS_ERR_DEV_FIXED                            =   303u,       /* Device is fixed (cannot be closed).                  */
    FS_ERR_DEV_FULL                             =   304u,       /* Device is full (no space could be allocated).        */
    FS_ERR_DEV_INVALID                          =   310u,       /* Invalid device.                                      */
    FS_ERR_DEV_INVALID_CFG                      =   311u,       /* Invalid dev cfg.                                     */
    FS_ERR_DEV_INVALID_ECC                      =   312u,       /* Invalid ECC.                                         */
    FS_ERR_DEV_INVALID_IO_CTRL                  =   313u,       /* I/O control invalid.                                 */
    FS_ERR_DEV_INVALID_LOW_FMT                  =   314u,       /* Low format invalid.                                  */
    FS_ERR_DEV_INVALID_LOW_PARAMS               =   315u,       /* Invalid low-level device parameters.                 */
    FS_ERR_DEV_INVALID_MARK                     =   316u,       /* Invalid mark.                                        */
    FS_ERR_DEV_INVALID_NAME                     =   317u,       /* Invalid device name.                                 */
    FS_ERR_DEV_INVALID_OP                       =   318u,       /* Invalid operation.                                   */
    FS_ERR_DEV_INVALID_SEC_NBR                  =   319u,       /* Invalid device sec nbr.                              */
    FS_ERR_DEV_INVALID_SEC_SIZE                 =   320u,       /* Invalid device sec size.                             */
    FS_ERR_DEV_INVALID_SIZE                     =   321u,       /* Invalid device size.                                 */
    FS_ERR_DEV_INVALID_UNIT_NBR                 =   322u,       /* Invalid device unit nbr.                             */
    FS_ERR_DEV_IO                               =   323u,       /* Device I/O error.                                    */
    FS_ERR_DEV_NONE_AVAIL                       =   324u,       /* No device avail.                                     */
    FS_ERR_DEV_NOT_OPEN                         =   325u,       /* Device not open.                                     */
    FS_ERR_DEV_NOT_PRESENT                      =   326u,       /* Device not present.                                  */
    FS_ERR_DEV_TIMEOUT                          =   327u,       /* Device timeout.                                      */
    FS_ERR_DEV_UNIT_NONE_AVAIL                  =   328u,       /* No unit avail.                                       */
    FS_ERR_DEV_UNIT_ALREADY_EXIST               =   329u,       /* Unit already exists.                                 */
    FS_ERR_DEV_UNKNOWN                          =   330u,       /* Unknown.                                             */
    FS_ERR_DEV_VOL_OPEN                         =   331u,       /* Vol open on dev.                                     */
    FS_ERR_DEV_INCOMPATIBLE_LOW_PARAMS          =   332u,       /* Incompatible low-level device parameters.            */
    FS_ERR_DEV_INVALID_METADATA                 =   333u,       /* Device driver metadata is invalid.                   */
    FS_ERR_DEV_OP_ABORTED                       =   334u,       /* Operation aborted.                                   */
    FS_ERR_DEV_CORRUPT_LOW_FMT                  =   335u,       /* Corrupted low-level fmt.                             */
    FS_ERR_DEV_INVALID_SEC_DATA                 =   336u,       /* Retrieved sec data is invalid.                       */
    FS_ERR_DEV_WR_PROT                          =   337u,       /* Device is write protected.                           */
    FS_ERR_DEV_OP_FAILED                        =   338u,       /* Operation failed.                                    */

    FS_ERR_DEV_NAND_NO_AVAIL_BLK                =   350u,       /* No blk avail.                                        */
    FS_ERR_DEV_NAND_NO_SUCH_SEC                 =   351u,       /* This sector is not available.                        */
    FS_ERR_DEV_NAND_ECC_NOT_SUPPORTED           =   352u,       /* The needed ECC scheme is not supported.              */


    FS_ERR_DEV_NAND_ONFI_EXT_PARAM_PAGE         =   362u,       /* NAND device extended parameter page must be read.    */


/*
*********************************************************************************************************
*                                      DEVICE DRIVER ERROR CODES
*********************************************************************************************************
*/

    FS_ERR_DEV_DRV_ALREADY_ADDED                =   400u,       /* Dev drv already added.                               */
    FS_ERR_DEV_DRV_INVALID_NAME                 =   401u,       /* Invalid dev drv name.                                */
    FS_ERR_DEV_DRV_NONE_AVAIL                   =   402u,       /* No driver available.                                 */


/*
*********************************************************************************************************
*                                        DIRECTORY ERROR CODES
*********************************************************************************************************
*/

    FS_ERR_DIR_ALREADY_OPEN                     =   500u,       /* Directory already open.                              */
    FS_ERR_DIR_DIS                              =   501u,       /* Directory module disabled.                           */
    FS_ERR_DIR_FULL                             =   502u,       /* Directory is full.                                   */
    FS_ERR_DIR_NONE_AVAIL                       =   503u,       /* No directory  avail.                                 */
    FS_ERR_DIR_NOT_OPEN                         =   504u,       /* Directory not open.                                  */


/*
*********************************************************************************************************
*                                           ECC ERROR CODES
*********************************************************************************************************
*/

    FS_ERR_ECC_CORR                             =   600u,       /* Correctable ECC error.                               */
    FS_ERR_ECC_CRITICAL_CORR                    =   601u,       /* Critical correctable ECC error (should refresh data).*/
    FS_ERR_ECC_UNCORR                           =   602u,       /* Uncorrectable ECC error.                             */


/*
*********************************************************************************************************
*                                          ENTRY ERROR CODES
*********************************************************************************************************
*/

    FS_ERR_ENTRIES_SAME                         =   700u,       /* Paths specify same file system entry.                */
    FS_ERR_ENTRIES_TYPE_DIFF                    =   701u,       /* Paths do not both specify files OR directories.      */
    FS_ERR_ENTRIES_VOLS_DIFF                    =   702u,       /* Paths specify file system entries on different vols. */
    FS_ERR_ENTRY_CORRUPT                        =   703u,       /* File system entry is corrupt.                        */
    FS_ERR_ENTRY_EXISTS                         =   704u,       /* File system entry exists.                            */
    FS_ERR_ENTRY_INVALID                        =   705u,       /* File system entry invalid.                           */
    FS_ERR_ENTRY_NOT_DIR                        =   706u,       /* File system entry NOT a directory.                   */
    FS_ERR_ENTRY_NOT_EMPTY                      =   707u,       /* File system entry NOT empty.                         */
    FS_ERR_ENTRY_NOT_FILE                       =   708u,       /* File system entry NOT a file.                        */
    FS_ERR_ENTRY_NOT_FOUND                      =   709u,       /* File system entry NOT found.                         */
    FS_ERR_ENTRY_PARENT_NOT_FOUND               =   710u,       /* Entry parent NOT found.                              */
    FS_ERR_ENTRY_PARENT_NOT_DIR                 =   711u,       /* Entry parent NOT a directory.                        */
    FS_ERR_ENTRY_RD_ONLY                        =   712u,       /* File system entry marked read-only.                  */
    FS_ERR_ENTRY_ROOT_DIR                       =   713u,       /* File system entry is a root directory.               */
    FS_ERR_ENTRY_TYPE_INVALID                   =   714u,       /* File system entry type is invalid.                   */
    FS_ERR_ENTRY_OPEN                           =   715u,       /* Operation not allowed on already open entry          */
    FS_ERR_ENTRY_CLUS                           =   716u,       /* No clus allocated to a directory entry               */

/*
*********************************************************************************************************
*                                          FILE ERROR CODES
*********************************************************************************************************
*/

    FS_ERR_FILE_ALREADY_OPEN                    =   800u,       /* File already open.                                   */
    FS_ERR_FILE_BUF_ALREADY_ASSIGNED            =   801u,       /* Buf already assigned.                                */
    FS_ERR_FILE_ERR                             =   802u,       /* Error indicator set on file.                         */
    FS_ERR_FILE_INVALID_ACCESS_MODE             =   803u,       /* Access mode is specified invalid.                    */
    FS_ERR_FILE_INVALID_ATTRIB                  =   804u,       /* Attributes are specified invalid.                    */
    FS_ERR_FILE_INVALID_BUF_MODE                =   805u,       /* Buf mode is specified invalid or unknown.            */
    FS_ERR_FILE_INVALID_BUF_SIZE                =   806u,       /* Buf size is specified invalid.                       */
    FS_ERR_FILE_INVALID_DATE_TIME               =   807u,       /* Date/time is specified invalid.                      */
    FS_ERR_FILE_INVALID_DATE_TIME_TYPE          =   808u,       /* Date/time type flag is specified invalid.            */
    FS_ERR_FILE_INVALID_NAME                    =   809u,       /* Name is specified invalid.                           */
    FS_ERR_FILE_INVALID_ORIGIN                  =   810u,       /* Origin is specified invalid or unknown.              */
    FS_ERR_FILE_INVALID_OFFSET                  =   811u,       /* Offset is specified invalid.                         */
    FS_ERR_FILE_INVALID_FILES                   =   812u,       /* Invalid file arguments.                              */
    FS_ERR_FILE_INVALID_OP                      =   813u,       /* File operation invalid.                              */
    FS_ERR_FILE_INVALID_OP_SEQ                  =   814u,       /* File operation sequence invalid.                     */
    FS_ERR_FILE_INVALID_POS                     =   815u,       /* File position invalid.                               */
    FS_ERR_FILE_LOCKED                          =   816u,       /* File locked.                                         */
    FS_ERR_FILE_NONE_AVAIL                      =   817u,       /* No file available.                                   */
    FS_ERR_FILE_NOT_OPEN                        =   818u,       /* File NOT open.                                       */
    FS_ERR_FILE_NOT_LOCKED                      =   819u,       /* File NOT locked.                                     */
    FS_ERR_FILE_OVF                             =   820u,       /* File size overflowed max file size.                  */
    FS_ERR_FILE_OVF_OFFSET                      =   821u,       /* File offset overflowed max file offset.              */


/*
*********************************************************************************************************
*                                          NAME ERROR CODES
*********************************************************************************************************
*/

    FS_ERR_NAME_BASE_TOO_LONG                   =   900u,       /* Base name too long.                                  */
    FS_ERR_NAME_EMPTY                           =   901u,       /* Name empty.                                          */
    FS_ERR_NAME_EXT_TOO_LONG                    =   902u,       /* Extension too long.                                  */
    FS_ERR_NAME_INVALID                         =   903u,       /* Invalid file name or path.                           */
    FS_ERR_NAME_MIXED_CASE                      =   904u,       /* Name is mixed case.                                  */
    FS_ERR_NAME_NULL                            =   905u,       /* Name ptr arg(s) passed NULL ptr(s).                  */
    FS_ERR_NAME_PATH_TOO_LONG                   =   906u,       /* Entry path is too long.                              */
    FS_ERR_NAME_BUF_TOO_SHORT                   =   907u,       /* Buffer for name is too short.                        */
    FS_ERR_NAME_TOO_LONG                        =   908u,       /* Full name is too long.                               */


/*
*********************************************************************************************************
*                                        PARTITION ERROR CODES
*********************************************************************************************************
*/

    FS_ERR_PARTITION_INVALID                    =  1001u,       /* Partition invalid.                                   */
    FS_ERR_PARTITION_INVALID_NBR                =  1002u,       /* Partition nbr specified invalid.                     */
    FS_ERR_PARTITION_INVALID_SIG                =  1003u,       /* Partition sig invalid.                               */
    FS_ERR_PARTITION_INVALID_SIZE               =  1004u,       /* Partition size invalid.                              */
    FS_ERR_PARTITION_MAX                        =  1005u,       /* Max nbr partitions have been created in MBR.         */
    FS_ERR_PARTITION_NOT_FINAL                  =  1006u,       /* Prev partition is not final partition.               */
    FS_ERR_PARTITION_NOT_FOUND                  =  1007u,       /* Partition NOT found.                                 */
    FS_ERR_PARTITION_ZERO                       =  1008u,       /* Partition zero.                                      */


/*
*********************************************************************************************************
*                                          POOLS ERROR CODE
*********************************************************************************************************
*/

    FS_ERR_POOL_EMPTY                           =  1100u,       /* Pool is empty.                                       */
    FS_ERR_POOL_FULL                            =  1101u,       /* Pool is full.                                        */
    FS_ERR_POOL_INVALID_BLK_ADDR                =  1102u,       /* Block not found in used pool pointers.               */
    FS_ERR_POOL_INVALID_BLK_IN_POOL             =  1103u,       /* Block found in free pool pointers.                   */
    FS_ERR_POOL_INVALID_BLK_IX                  =  1104u,       /* Block index invalid.                                 */
    FS_ERR_POOL_INVALID_BLK_NBR                 =  1105u,       /* Number blocks specified invalid.                     */
    FS_ERR_POOL_INVALID_BLK_SIZE                =  1106u,       /* Block size specified invalid.                        */


/*
*********************************************************************************************************
*                                       FILE SYSTEM ERROR CODES
*********************************************************************************************************
*/

    FS_ERR_SYS_TYPE_NOT_SUPPORTED               =  1301u,       /* File sys type not supported.                         */
    FS_ERR_SYS_INVALID_SIG                      =  1309u,       /* Sec has invalid OR illegal sig.                      */
    FS_ERR_SYS_DIR_ENTRY_PLACE                  =  1330u,       /* Dir entry could not be placed.                       */
    FS_ERR_SYS_DIR_ENTRY_NOT_FOUND              =  1331u,       /* Dir entry not found.                                 */
    FS_ERR_SYS_DIR_ENTRY_NOT_FOUND_YET          =  1332u,       /* Dir entry not found (yet).                           */
    FS_ERR_SYS_SEC_NOT_FOUND                    =  1333u,       /* Sec not found.                                       */
    FS_ERR_SYS_CLUS_CHAIN_END                   =  1334u,       /* Cluster chain ended.                                 */
    FS_ERR_SYS_CLUS_CHAIN_END_EARLY             =  1335u,       /* Cluster chain ended before number clusters traversed.*/
    FS_ERR_SYS_CLUS_INVALID                     =  1336u,       /* Cluster invalid.                                     */
    FS_ERR_SYS_CLUS_NOT_AVAIL                   =  1337u,       /* Cluster not avail.                                   */
    FS_ERR_SYS_SFN_NOT_AVAIL                    =  1338u,       /* SFN is not avail.                                    */
    FS_ERR_SYS_LFN_ORPHANED                     =  1339u,       /* LFN entry orphaned.                                  */

/*
*********************************************************************************************************
*                                           VOLUME ERROR CODES
*********************************************************************************************************
*/

    FS_ERR_VOL_INVALID_NAME                     =  1400u,       /* Invalid volume name.                                 */
    FS_ERR_VOL_INVALID_SIZE                     =  1401u,       /* Invalid volume size.                                 */
    FS_ERR_VOL_INVALID_SEC_SIZE                 =  1403u,       /* Invalid volume sector size.                          */
    FS_ERR_VOL_INVALID_CLUS_SIZE                =  1404u,       /* Invalid volume cluster size.                         */
    FS_ERR_VOL_INVALID_OP                       =  1405u,       /* Volume operation invalid.                            */
    FS_ERR_VOL_INVALID_SEC_NBR                  =  1406u,       /* Invalid volume sector number.                        */
    FS_ERR_VOL_INVALID_SYS                      =  1407u,       /* Invalid file system on volume.                       */
    FS_ERR_VOL_NO_CACHE                         =  1408u,       /* No cache assigned to volume.                         */

    FS_ERR_VOL_NONE_AVAIL                       =  1410u,       /* No vol  avail.                                       */
    FS_ERR_VOL_NONE_EXIST                       =  1411u,       /* No vols exist.                                       */
    FS_ERR_VOL_NOT_OPEN                         =  1413u,       /* Vol NOT open.                                        */
    FS_ERR_VOL_NOT_MOUNTED                      =  1414u,       /* Vol NOT mounted.                                     */
    FS_ERR_VOL_ALREADY_OPEN                     =  1415u,       /* Vol already open.                                    */
    FS_ERR_VOL_FILES_OPEN                       =  1416u,       /* Files open on vol.                                   */
    FS_ERR_VOL_DIRS_OPEN                        =  1417u,       /* Dirs open on vol.                                    */

    FS_ERR_VOL_JOURNAL_ALREADY_OPEN             =  1420u,       /* Journal already open.                                */
    FS_ERR_VOL_JOURNAL_CFG_CHNGD                =  1421u,       /* File system suite cfg changed since log created.     */
    FS_ERR_VOL_JOURNAL_FILE_INVALID             =  1422u,       /* Journal file invalid.                                */
    FS_ERR_VOL_JOURNAL_FULL                     =  1423u,       /* Journal full.                                        */
    FS_ERR_VOL_JOURNAL_LOG_INVALID_ARG          =  1424u,       /* Invalid arg read from journal log.                   */
    FS_ERR_VOL_JOURNAL_LOG_INCOMPLETE           =  1425u,       /* Log not completely entered in journal.               */
    FS_ERR_VOL_JOURNAL_LOG_NOT_PRESENT          =  1426u,       /* Log not present in journal.                          */
    FS_ERR_VOL_JOURNAL_NOT_OPEN                 =  1427u,       /* Journal not open.                                    */
    FS_ERR_VOL_JOURNAL_NOT_REPLAYING            =  1428u,       /* Journal not being replayed.                          */
    FS_ERR_VOL_JOURNAL_NOT_STARTED              =  1429u,       /* Journaling not started.                              */
    FS_ERR_VOL_JOURNAL_NOT_STOPPED              =  1430u,       /* Journaling not stopped.                              */
    FS_ERR_VOL_JOURNAL_REPLAYING                =  1431u,       /* Journal being replayed.                              */
    FS_ERR_VOL_JOURNAL_MARKER_NBR_MISMATCH      =  1432u,       /* Marker nbr mismatch.                                 */

    FS_ERR_VOL_LABEL_INVALID                    =  1440u,       /* Volume label is invalid.                             */
    FS_ERR_VOL_LABEL_NOT_FOUND                  =  1441u,       /* Volume label was not found.                          */
    FS_ERR_VOL_LABEL_TOO_LONG                   =  1442u,       /* Volume label is too long.                            */


/*
*********************************************************************************************************
*                                    FILE SYSTEM-OS LAYER ERROR CODES
*********************************************************************************************************
*/

    FS_ERR_OS_LOCK                              =  1501u,
    FS_ERR_OS_LOCK_TIMEOUT                      =  1502u,
    FS_ERR_OS_INIT                              =  1510u,
    FS_ERR_OS_INIT_LOCK                         =  1511u,
    FS_ERR_OS_INIT_LOCK_NAME                    =  1512u

} FS_ERR;


/*
*********************************************************************************************************
*                                               MACRO'S
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                             DATA TYPES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                          GLOBAL VARIABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/


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

#endif
