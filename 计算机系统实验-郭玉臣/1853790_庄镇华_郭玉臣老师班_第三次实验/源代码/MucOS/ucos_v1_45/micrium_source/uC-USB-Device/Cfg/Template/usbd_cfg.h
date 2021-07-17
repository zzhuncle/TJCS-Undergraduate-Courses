/*
*********************************************************************************************************
*                                            uC/USB-Device
*                                    The Embedded USB Device Stack
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
*                                    USB DEVICE CONFIGURATION FILE
*
*                                              TEMPLATE
*
* Filename : usbd_cfg.h
* Version  : V4.05.06
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                               MODULE
*
* Note(s) : (1) This USB configuration header file is protected from multiple pre-processor inclusion
*               through use of the USB configuration module present pre-processor macro definition.
*********************************************************************************************************
*/

#ifndef  USBD_CFG_MODULE_PRESENT                                /* See Note #1.                                         */
#define  USBD_CFG_MODULE_PRESENT


/*
*********************************************************************************************************
*                                  USB DEVICE GENERIC CONFIGURATION
*********************************************************************************************************
*/

                                                                /* uC/USB-Device Code Optimization.                     */
#define  USBD_CFG_OPTIMIZE_SPD                  DEF_ENABLED
                                                                /* DEF_ENABLED  Optimizes uC/USBD for speed performance.*/
                                                                /* DEF_DISABLED Optimizes uC/USBD for memory footprint. */

                                                                /* Maximum Number of Devices.                           */
#define  USBD_CFG_MAX_NBR_DEV                              1u
                                                                /* Must be between 1u and 255u.                         */

                                                                /* Buffer Alignment in uC/USB-Device.                   */
#define  USBD_CFG_BUF_ALIGN_OCTETS                         4u
                                                                /* Must be between 1u and 2^(CPU_CFG_DATA_SIZE * 8).    */


/*
*********************************************************************************************************
*                                USB DEVICE ARGUMENT CHECK CONFIGURATION
*
* Note(s) : (1) Configure USBD_ERR_CFG_ARG_CHK_EXT_EN to enable or disable argument checks.
*
*               (a) When DEF_ENABLED,  ALL arguments received from any API provided by the developer
*                   or application are checked/validated.
*               (b) When DEF_DISABLED, NO  arguments received from any API provided by the developer
*                   or application are checked/validated.
*********************************************************************************************************
*/

                                                                /* Argument Check Feature.                              */
#define  USBD_CFG_ERR_ARG_CHK_EXT_EN            DEF_ENABLED
                                                                /* See Note #1.                                         */


/*
*********************************************************************************************************
*                          USB DEVICE MICROSOFT OS DESCRIPTOR CONFIGURATION
*
* Note(s) : (1) Configure USBD_CFG_MS_OS_DESC_EN to enable or disable Microsoft OS descriptor.
*
*               (a) When DEF_ENABLED,  Microsoft descriptors and MS OS string descriptor are enabled.
*               (b) When DEF_DISABLED, Microsoft descriptors and MS OS string descriptor are disabled.
*********************************************************************************************************
*/

                                                                /* Microsoft Descriptor Support.                        */
#define  USBD_CFG_MS_OS_DESC_EN                 DEF_DISABLED
                                                                /* See Note #1.                                         */


/*
*********************************************************************************************************
*                                      USB DEVICE CONFIGURATIONS
*
* Note(s) : (1) This configuration should be set to DEF_ENABLED if the USB device controller supports
*               high-speed, or to DEF_DISABLED if otherwise.
*********************************************************************************************************
*/

                                                                /* Maximum Number of Configurations.                    */
#define  USBD_CFG_MAX_NBR_CFG                              2u
                                                                /* Must be between 1u and 255u.                         */

                                                                /* Configure Isochronous Support in uC/USB-Device.      */
#define  USBD_CFG_EP_ISOC_EN                    DEF_DISABLED
                                                                /* DEF_ENABLED  Isochronous enpoints are     available. */
                                                                /* DEF_DISABLED Isochronous enpoints are not available. */

                                                                /* Configure High-Speed Support in uC/USB-Device.       */
#define  USBD_CFG_HS_EN                         DEF_ENABLED
                                                                /* See Note #1.                                         */

                                                                /* Timeout for Data/Status Phases of Control Transfer.  */
#define  USBD_CFG_CTRL_REQ_TIMEOUT_mS                   5000u
                                                                /* Must be between 1u and 65535u.                       */


/*
*********************************************************************************************************
*                                 USB DEVICE INTERFACES CONFIGURATION
*********************************************************************************************************
*/

                                                                /* Maximum Number of Interfaces.                        */
#define  USBD_CFG_MAX_NBR_IF                               2u
                                                                /* Must be between 1u and 255u.                         */

                                                                /* Maximum Number of Alternate Interfaces.              */
#define  USBD_CFG_MAX_NBR_IF_ALT                           2u
                                                                /* Must be between 1u and 255u.                         */

                                                                /* Maximum Number of Interface Groups.                  */
#define  USBD_CFG_MAX_NBR_IF_GRP                           1u
                                                                /* Must be between 0u and 255u.                         */

                                                                /* Maximum Number of Endpoint Descriptors.              */
#define  USBD_CFG_MAX_NBR_EP_DESC                          2u
                                                                /* Must be between 1u and 255u.                         */

                                                                /* Maximum Number of Opened Endpoints.                  */
#define  USBD_CFG_MAX_NBR_EP_OPEN                          4u
                                                                /* Must be between 2u and 255u.                         */

                                                                /* Maximum Number of Additional URBs.                   */
                                                                /* These URBs are used for async queueing.              */
#define  USBD_CFG_MAX_NBR_URB_EXTRA                        0u
                                                                /* Must be between 0u and 255u.                         */


/*
*********************************************************************************************************
*                                   USB DEVICE STRING CONFIGURATION
*********************************************************************************************************
*/

                                                                /* Maximum Number of String Descriptors.                */
#define  USBD_CFG_MAX_NBR_STR                             10u
                                                                /* Must be between 0u and 255u.                         */


/*
*********************************************************************************************************
*                                   USB DEVICE DEBUG CONFIGURATION
*********************************************************************************************************
*/

                                                                /* Debug Module Trace Support.                          */
#define  USBD_CFG_DBG_TRACE_EN                  DEF_DISABLED
                                                                /* DEF_ENABLED  Trace debug module is     available.    */
                                                                /* DEF_DISABLED Trace debug module is not available.    */

                                                                /* Maximum Number of Trace Events.                      */
#define  USBD_CFG_DBG_TRACE_NBR_EVENTS                    10u
                                                                /* Must be between 1u and 255u.                         */

                                                                /* Debug Module Built-In Statistics Support.            */
#define  USBD_CFG_DBG_STATS_EN                  DEF_DISABLED
                                                                /* DEF_ENABLED  Built-in statistics are     available.  */
                                                                /* DEF_DISABLED Built-in statistics are not available.  */

                                                                /* Debug Module Built-In Statistics Counter Data Type.  */
#define  USBD_CFG_DBG_STATS_CNT_TYPE            CPU_INT08U
                                                                /* CPU_INT08U, CPU_INT16U or CPU_INT32U.                */


/*
*********************************************************************************************************
*                                      AUDIO CLASS CONFIGURATION
*
* Note(s) : (1) Among all class-specific requests supported by Audio 1.0 class, Graphic Equalizer control
*               of the Feature Unit use the longest payload size for the SET_CUR request.
*               See 'USB Device Class Definition for Audio Devices, Release 1.0, March 18, 1998',
*               Table 5-27 for more details.
*
*           (2) Audio buffers allocated for each AudioStreaming interface requires to be aligned
*               properly according to DMA requirement. Most of the time, a DMA is used to transfer
*               audio data between the codec and the USB stack in order to offload the CPU.
*********************************************************************************************************
*/

                                                                /* Audio Playback Support.                              */
#define  USBD_AUDIO_CFG_PLAYBACK_EN               DEF_ENABLED
                                                                /* DEF_ENABLED  Enable  audio playback capabilities.    */
                                                                /* DEF_DISABLED Disable audio playback capabilities.    */

                                                                /* Audio Record Support.                                */
#define  USBD_AUDIO_CFG_RECORD_EN                 DEF_ENABLED
                                                                /* DEF_ENABLED  Enable  audio record capabilities.      */
                                                                /* DEF_DISABLED Disable audio record capabilities.      */

                                                                /* Feature Unit with all or Minimum of Controls.        */
#define  USBD_AUDIO_CFG_FU_MAX_CTRL               DEF_DISABLED
                                                                /* DEF_ENABLED  FU with all ctrls.                      */
                                                                /* DEF_DISABLED FU with only mute & vol ctrls.          */

                                                                /* Maximum Number of Audio Class Instances.             */
#define  USBD_AUDIO_CFG_MAX_NBR_AIC                        1u
                                                                /* Must be between 1u and 254u.                         */

                                                                /* Maximum Number of Configurations per Class Instance. */
#define  USBD_AUDIO_CFG_MAX_NBR_CFG                        2u
                                                                /* Must be between 1u and 254u.                         */

                                                                /* Maximum Number of Input Terminals.                   */
#define  USBD_AUDIO_CFG_MAX_NBR_IT                         2u
                                                                /* Must be between 1u and 255u.                         */

                                                                /* Maximum Number of Output Terminals.                  */
#define  USBD_AUDIO_CFG_MAX_NBR_OT                         2u
                                                                /* Must be between 1u and 255u.                         */

                                                                /* Maximum Number of Feature Units.                     */
#define  USBD_AUDIO_CFG_MAX_NBR_FU                         2u
                                                                /* Must be between 1u and 255u.                         */

                                                                /* Maximum Number of Mixer Units.                       */
#define  USBD_AUDIO_CFG_MAX_NBR_MU                         0u
                                                                /* Must be between 1u and 255u.                         */

                                                                /* Maximum Number of Selector Units.                    */
#define  USBD_AUDIO_CFG_MAX_NBR_SU                         0u
                                                                /* Must be between 1u and 255u.                         */

                                                                /* Maximum Number of Playback AudioStreaming Interfaces */
                                                                /* per Class Instance.                                  */
#define  USBD_AUDIO_CFG_MAX_NBR_AS_IF_PLAYBACK             1u
                                                                /* Must be between 1u and 255u.                         */

                                                                /* Maximum Number of Record AudioStreaming Interfaces   */
                                                                /* per Class Instance.                                  */
#define  USBD_AUDIO_CFG_MAX_NBR_AS_IF_RECORD               1u
                                                                /* Must be between 1u and 255u.                         */

                                                                /* Maximum Number of Operational Alternate Interfaces   */
                                                                /* per Class Instance.                                  */
#define  USBD_AUDIO_CFG_MAX_NBR_IF_ALT                     2u
                                                                /* Must be between 1u and 255u.                         */

                                                                /* Maximum Class Specific Payload Length.               */
#define  USBD_AUDIO_CFG_CLASS_REQ_MAX_LEN                  4u
                                                                /* Must be between 1u and 34u (see Note #1).            */

                                                                /* Audio Buffer Alignment Requirement.                  */
#define  USBD_AUDIO_CFG_BUF_ALIGN_OCTETS          USBD_CFG_BUF_ALIGN_OCTETS
                                                                /* See Note #2.                                         */

                                                                /* Playback Feedback Support.                           */
#define  USBD_AUDIO_CFG_PLAYBACK_FEEDBACK_EN      DEF_DISABLED
                                                                /* DEF_ENABLED  Enable  playback feedback.              */
                                                                /* DEF_DISABLED Disable playback feedback.              */

                                                                /* Playback Stream Correction Support.                  */
#define  USBD_AUDIO_CFG_PLAYBACK_CORR_EN          DEF_DISABLED
                                                                /* DEF_ENABLED  Enable  playback stream correction.     */
                                                                /* DEF_DISABLED Disable playback stream correction.     */

                                                                /* Record Stream Correction Support.                    */
#define  USBD_AUDIO_CFG_RECORD_CORR_EN            DEF_DISABLED
                                                                /* DEF_ENABLED  Enable  record stream correction.       */
                                                                /* DEF_DISABLED Disable record stream correction.       */

                                                                /* Audio Statistics Support.                            */
#define  USBD_AUDIO_CFG_STAT_EN                   DEF_DISABLED
                                                                /* DEF_ENABLED  Enable  audio class statistics.         */
                                                                /* DEF_DISABLED Disable audio class statistics.         */


/*
*********************************************************************************************************
*                           COMMUNICATION DEVICE CLASS (CDC) CONFIGURATION
*********************************************************************************************************
*/

                                                                /* Maximum Number of CDC Class Instances.               */
#define  USBD_CDC_CFG_MAX_NBR_DEV                          1u
                                                                /* Must be between 1u and 255u.                         */

                                                                /* Maximum Number of Configurations per Class Instance. */
#define  USBD_CDC_CFG_MAX_NBR_CFG                          2u
                                                                /* Must be between 1u and 255u.                         */

                                                                /* Maximum Number of CDC Data Interfaces.               */
#define  USBD_CDC_CFG_MAX_NBR_DATA_IF                      1u
                                                                /* Must be between 1u and 255u.                         */


/*
*********************************************************************************************************
*                     CDC ABSTRACT CONTROL MODEL (ACM) SERIAL CLASS CONFIGURATION
*********************************************************************************************************
*/

                                                                /* Maximum Number of ACM Sub-Class Instances.           */
#define  USBD_ACM_SERIAL_CFG_MAX_NBR_DEV                   1u
                                                                /* Must be between 1u and 255u.                         */


/*
*********************************************************************************************************
*                       CDC ETHERNET EMULATION MODEL (EEM) CLASS CONFIGURATION
*********************************************************************************************************
*/

                                                                /* Maximum Number of Class Instances                    */
#define  USBD_CDC_EEM_CFG_MAX_NBR_DEV                      1u

                                                                /* Maximum Number of Configurations per Class Instance  */
#define  USBD_CDC_EEM_CFG_MAX_NBR_CFG                      2u

                                                                /* Length of receive buffer(s).                         */
#define  USBD_CDC_EEM_CFG_RX_BUF_LEN                    1520u

                                                                /* Length of buffer used for echo response command.     */
#define  USBD_CDC_EEM_CFG_ECHO_BUF_LEN                    64u


/*
*********************************************************************************************************
*                                       HID CLASS CONFIGURATION
*********************************************************************************************************
*/

                                                                /* Maximum Number of HID Class Instances.               */
#define  USBD_HID_CFG_MAX_NBR_DEV                          1u
                                                                /* Must be between 1u and 255u.                         */

                                                                /* Maximum Number of Configurations per Class Instance. */
#define  USBD_HID_CFG_MAX_NBR_CFG                          2u
                                                                /* Must be between 1u and 255u.                         */

                                                                /* Maximum Number of Report ID.                         */
#define  USBD_HID_CFG_MAX_NBR_REPORT_ID                    1u
                                                                /* Must be between 1u and 255u.                         */

                                                                /* Maximum Number of Push/Pop Items.                    */
#define  USBD_HID_CFG_MAX_NBR_REPORT_PUSHPOP               0u
                                                                /* Must be between 0u and 255u.                         */


/*
*********************************************************************************************************
*                               MASS STORAGE CLASS (MSC) CONFIGURATION
*
* Note(s) : (1) USBD_MSC_CFG_MICRIUM_FS is used to enable the Mass Storage Class interface with the
*               various storage media supported by uC/FS such as SD-Card, NAND, etc.
*
*               DEF_ENABLED      Use the uC/FS' MSC class interface.
*               DEF_DISABLED     Use uC/USB-Device's own RAMDisk implementation without a File System.
*
*           (2) USBD_MSC_CFG_FS_REFRESH_TASK_EN is only used when MSC class interfaces with uC/FS. It
*               should be DEF_ENABLED when using a removable media such as SD card. Fixed media such as
*               RAMDisk, NAND should have this constant set to DEF_DISABLED.
*
*               DEF_ENABLED      Create FS Refresh task for polling media status.
*               DEF_DISABLED     Do not create FS Refresh task.
*
*           (3) USBD_RAMDISK_CFG_BASE_ADDR is used to define the data area of the RAM disk. If it is
*               defined with a value other than 0, the RAM disk data area will be set from this base
*               address directly. Conversely, if it is equal to 0, the RAM disk data area will be
*               represented as a table from the program's data area.
*********************************************************************************************************
*/

                                                                /* Maximum Number of MSC Class Instances.               */
#define  USBD_MSC_CFG_MAX_NBR_DEV                          1u
                                                                /* Must be between 1u and 255u.                         */

                                                                /* Maximum Number of Configurations per Class Instance. */
#define  USBD_MSC_CFG_MAX_NBR_CFG                          2u
                                                                /* Must be between 1u and 255u.                         */

                                                                /* Maximum Number of Logical Units.                     */
#define  USBD_MSC_CFG_MAX_LUN                              1u
                                                                /* Must be between 1u and 255u.                         */

                                                                /* Data Buffer Length, in octets.                       */
#define  USBD_MSC_CFG_DATA_LEN                          2048u
                                                                /* Must be between 1u and DEF_INT_32U_MAX_VAL.          */

                                                                /* Use uC/FS MSC class interface.                       */
#define  USBD_MSC_CFG_MICRIUM_FS                DEF_DISABLED
                                                                /* See Note #1.                                         */

                                                                /* Removable Device Refresh Task.                       */
#define  USBD_MSC_CFG_FS_REFRESH_TASK_EN        DEF_DISABLED
                                                                /* See Note #2.                                         */

                                                                /* Removable Device Refresh Task Polling Delay.         */
#define  USBD_MSC_CFG_DEV_POLL_DLY_mS                    100u
                                                                /* Must be between 1u and DEF_INT_32U_MAX_VAL.          */

                                                                /* Number of RAMDisk units.                             */
#define  USBD_RAMDISK_CFG_NBR_UNITS                        1u
                                                                /* Must be at least 1.                                  */

                                                                /* RAMDisk block size.                                  */
#define  USBD_RAMDISK_CFG_BLK_SIZE                       512u
                                                                /* Must be at least 1.                                  */

                                                                /* RAMDisk number of blocks.                            */
#define  USBD_RAMDISK_CFG_NBR_BLKS                        44u
                                                                /* Must be at least 1.                                  */

                                                                /* RAMDisk base address in memory.                      */
#define  USBD_RAMDISK_CFG_BASE_ADDR                        0u
                                                                /* See Note #3.                                         */


/*
*********************************************************************************************************
*                        PERSONAL HEALTHCARE DEVICE CLASS (PHDC) CONFIGURATION
*********************************************************************************************************
*/

                                                                /* Maximum Number of PHDC Class Instances.              */
#define  USBD_PHDC_CFG_MAX_NBR_DEV                         1u
                                                                /* Must be between 1u and 255u.                         */

                                                                /* Maximum Number of Configurations per Class Instance. */
#define  USBD_PHDC_CFG_MAX_NBR_CFG                         2u
                                                                /* Must be between 1u and 255u.                         */

                                                                /* Opaque Data Buffer Length.                           */
#define  USBD_PHDC_CFG_DATA_OPAQUE_MAX_LEN                43u
                                                                /* Must be between 1u and 255u.                         */

                                                                /* QoS Scheduler.                                       */
#define  USBD_PHDC_OS_CFG_SCHED_EN              DEF_ENABLED
                                                                /* DEF_ENABLED  Enable  QoS scheduler.                  */
                                                                /* DEF_DISABLED Disable QoS scheduler.                  */


/*
*********************************************************************************************************
*                                     VENDOR CLASS CONFIGURATION
*********************************************************************************************************
*/

                                                                /* Maximum Number of Vendor Class Instances.            */
#define  USBD_VENDOR_CFG_MAX_NBR_DEV                       1u
                                                                /* Must be between 1u and 255u.                         */

                                                                /* Maximum Number of Configurations per Class Instance. */
#define  USBD_VENDOR_CFG_MAX_NBR_CFG                       2u
                                                                /* Must be between 1u and 255u.                         */

                                                                /* Maximum Number of Microsoft Extended Properties.     */
#define  USBD_VENDOR_CFG_MAX_NBR_MS_EXT_PROPERTY           1u
                                                                /* Must be between 1u and 255u.                         */


/*
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*/

#endif
