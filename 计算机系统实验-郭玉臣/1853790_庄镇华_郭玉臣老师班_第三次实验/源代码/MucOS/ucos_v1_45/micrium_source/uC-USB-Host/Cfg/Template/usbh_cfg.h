/*
*********************************************************************************************************
*                                             uC/USB-Host
*                                     The Embedded USB Host Stack
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
*                                    USB HOST STACK CONFIGURATION
*
* Filename : usbh_cfg.h
* Version  : V3.41.09
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                               MODULE
*********************************************************************************************************
*/

#ifndef  USBH_CFG_MODULE_PRESENT
#define  USBH_CFG_MODULE_PRESENT


/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                               EXTERNS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                               DEFINES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                    USB HOST STACK CONFIGURATION
*
* Notes:    (1) USB uses a tiered star topology. Each external hub has one upstream-facing port and one or
*               more downstream facing ports. Up to five external hubs can connect in series with a limit of
*               127 peripherals and hubs including root hub(s). There is one root hub per USB host controller.
*               USBH_CFG_MAX_NBR_DEVS is set to 1 if Host accepts only one single device and doesn't
*               use any external hub.
*
*                                           -------------------------------
*               TIER 1                      |            HOST             |
*                                           |  ------------------------   |
*                                           |  |       Root Hub        |  |
*                                           |  | []    []     []   []  |  |
*                                           -------------------------------
*                                                |      |      |     |
*                           ---------------------    ----      |     ------------------------------
*                           |                        |         |                                  |
*                        --------            -------------   --------                        ---------
*               TIER 2   |Device|            |    Hub    |   |Device|                        |  Hub  |
*                        --------            |[] [] [] []|   --------                        | [] [] |
*                                            -------------                                   ---------
*                                                |   |  |                                      |   |
*                                       ----------   |  ----------------                    ----   -----
*                                       |            |                 |                    |          |
*                                    ---------    --------    ---------------------     --------   --------
*               TIER 3               |  Hub  |    |Device|    |        Hub        |     |Device|   |Device|
*                                    | [] [] |    --------    |                   |     --------   --------
*                                    ---------                | -------  -------  |
*                                      |   |                  | |Device| |Device| |
*                                     --   -------            | -------  -------  |
*                                     |          |            |  Compound Device  |
*                                   --------  --------        ---------------------
*               TIER 4              |Device|  |Device|
*                                   --------  --------
*
*           (2) Host Stack Structure:
*
*               Host stack
*               |- Device 1
*                      |- Configuration 1
*                              |- Endpoint IN 0
*                              |- Endpoint OUT 0
*                              |-  Interface 1
*                                       |- Endpoint IN 1
*                                       |- Endpoint OUT 1
*               |- Device 2
*                      |- Configuration 1
*                              |- Endpoint IN 0
*                              |- Endpoint OUT 0
*                              |-  Interface 1
*                                       |- Endpoint IN 1
*                              |-  Interface 2
*                                       |- Endpoint IN 2
*                                       |- Endpoint OUT 2
*               |- Device 3
*                      |- Configuration 1
*                              |- Endpoint IN 0
*                              |- Endpoint OUT 0
*                              |-  Interface 1
*                                       |- Endpoint IN 1
*               |- ...
*
*               (a) Configuration descriptor gives information about a specific device configuration.
*                   USBH_CFG_MAX_NBR_CFGS, the number of configuration descriptors generally has the
*                   value of 1 because most device only has one configuration descriptor.
*
*               (b) Interface descriptors is the grouping of endpoints into a function group performing a
*                   single feature of the device. A device can have 1 or many interface enabled at once.
*
*                   CLASS                    REQUIRED INTERFACES
*                  -------                   -------------------
*                   MSC                      1 interface
*                   HID                      1 interface
*                   CDC ACM                  2 interfaces
*
*              (c)  Each interface descriptor has zero or more endpoint descriptors describing a unique
*                   set of endpoints within the interface. USBH_CFG_MAX_NBR_EPS represents the maximum
*                   number of endpoints that an interface can contain. The control enpoints shouldn't
*                   be considered here.
*
*           (3) USBH_CFG_MAX_HUB_PORTS specifies the maximum number of ports that can be active and
*               managed at the same time per external hub connected to the Host. If USBH_CFG_MAX_HUB_PORTS
*               is set to a value less than the actual number of ports available on a hub, then the rest
*               of the ports on this hub will be rendered unusable. Currently on the market, the common
*               number of ports for external hubs are 3, 4, 5 and 7. USBH_CFG_MAX_HUB_PORTS is
*               recommended to be set to 7.
*
*           (4) The standard request timeout is 5 secs (refer to Universal Serial Bus specification,
*               revision 2,0, sec 9.2.6.4).
*********************************************************************************************************
*/

                                                                /* ------ USB TOPOLOGY CFG. SEE NOTE #1 AND #2. ------- */

                                                                /*  Number of host controllers                          */
                                                                /*  The number of host controllers that will be ...     */
                                                                /*  ... added to the stack.                             */
#define  USBH_CFG_MAX_NBR_HC                               1u

                                                                /*  Maximum number of class driver supported            */
                                                                /*  The maximum number of class driver ...              */
                                                                /*  ... (MSC, HID, etc) you will use and add to the ... */
                                                                /*  ... core. The hub class is mandatory and must be ...*/
                                                                /*  ... accounted in the total number.                  */
#define  USBH_CFG_MAX_NBR_CLASS_DRVS                       4u

                                                                /*  Maximum number of devices                           */
                                                                /*  The maximum number of devices that the USB host ... */
                                                                /*  ... stack can accept. This must be < 127.           */
#define  USBH_CFG_MAX_NBR_DEVS                            10u

                                                                /*  Maximum number of configurations per device         */
                                                                /*  The maximum number of USB configurations that ...   */
                                                                /*  ... each device can contain. Most of the time, ...  */
                                                                /*  ... devices will have only one configuration.       */
#define  USBH_CFG_MAX_NBR_CFGS                             1u

                                                                /*  Maximum number of interfaces per USB configuration. */
                                                                /*  The maximum number of interface per USB ...         */
                                                                /*  ... configurations that each device can contain. ...*/
                                                                /*  ... Simple USB devices will generally have only ... */
                                                                /*  ... one interface. If you plan to use composite ... */
                                                                /*  ... devices, you should increase this value.        */
#define  USBH_CFG_MAX_NBR_IFS                              1u

                                                                /*  Max number of endpoints per interface.              */
                                                                /*  The maximum number of endpoints that an interface...*/
                                                                /*  ... can contain. The number of endpoints greatly ...*/
                                                                /*  ... depends on the device's class, but should ...   */
                                                                /*  ... generally be around two or three.               */
#define  USBH_CFG_MAX_NBR_EPS                              3u

                                                                /*  Maximum configuration descriptor length             */
                                                                /*  The maximum length of the buffer that is used to ...*/
                                                                /*  ... hold the USB configuration descriptor.          */
#define  USBH_CFG_MAX_CFG_DATA_LEN                       256u

                                                                /*  Maximum length of string descriptors                */
                                                                /*  The maximum length for string descriptors.          */
#define  USBH_CFG_MAX_STR_LEN                            256u

                                                                /*  Timeout for standard request (ms)                   */
                                                                /*  Timeout in ms for standard requests to complete.    */
#define  USBH_CFG_STD_REQ_TIMEOUT                       5000u

                                                                /*  Number of retries on stand requests fail            */
                                                                /*  Number of times the stack should retry to ...       */
                                                                /*  ... execute a standard request when it failed.      */
#define  USBH_CFG_STD_REQ_RETRY                            3u

                                                                /*  Maximum number of isochronous descriptor            */
                                                                /*  The maximum number of isochronous descriptor ...    */
                                                                /*  ... that will be shared between all isochronous ... */
                                                                /*  ... endpoints.                                      */
#define  USBH_CFG_MAX_ISOC_DESC                            1u

                                                                /*  Maximum number of extra URB                         */
                                                                /*  The maximum number of extra URB used for streaming. */
#define  USBH_CFG_MAX_EXTRA_URB_PER_DEV                    1u

                                                                /*  Maximum number of USB hub                           */
                                                                /*  The maximum number of external and root hub that ...*/
                                                                /*  ... can be connected.                               */
#define  USBH_CFG_MAX_HUBS                                 2u

                                                                /*  Maximum number of port per USB hub                  */
                                                                /*  The maximum number of supported ports per hub. ...  */
                                                                /*  ... See note #3.                                    */
#define  USBH_CFG_MAX_HUB_PORTS                            7u

                                                                /* Maximum number of connection retries                 */
                                                                /* The maximum number of reset and connection ...       */
                                                                /* ... retries before tearing down a device             */
#define  USBH_CFG_MAX_NUM_DEV_RECONN                       3u


/*
*********************************************************************************************************
*                           COMMUNICATION DEVICE CLASS (CDC) CONFIGURATION
*********************************************************************************************************
*/

                                                                /*  Maximum number of CDC device                        */
                                                                /*  The maximum number of CDC devices that can be ...   */
                                                                /*  ... connected at the same time.                     */
#define  USBH_CDC_CFG_MAX_DEV                              1u

                                                                /*  Maximum number of CDC ACM device                    */
                                                                /*  The maximum number of CDC ACM devices that can ...  */
                                                                /*  ... be connected at the same time.                  */
#define  USBH_CDC_ACM_CFG_MAX_DEV                          1u


/*
*********************************************************************************************************
*                          HUMAN INTERFACE DEVICE (HID) CLASS CONFIGURATION
*********************************************************************************************************
*/

                                                                /*  Maximum number of HID devices                       */
                                                                /*  The maximum number of HID devices that can be ...   */
                                                                /*  ... connected at the same time.                     */
#define  USBH_HID_CFG_MAX_DEV                              5u

                                                                /*  Maximum number of application collections           */
                                                                /*  The maximum number of HID application ...           */
                                                                /*  ... collections supported.                          */
#define  USBH_HID_CFG_MAX_NBR_APP_COLL                    10u

                                                                /*  Maximum number of report IDs                        */
                                                                /*  The maximum number of HID report IDs.               */
#define  USBH_HID_CFG_MAX_NBR_REPORT_ID                   15u

                                                                /*  Maximum number of report formats                    */
                                                                /*  The maximum number of HID report formats.           */
#define  USBH_HID_CFG_MAX_NBR_REPORT_FMT                  15u

                                                                /*  Maximum number of usage in local                    */
                                                                /*  The maximum number of HID usage in local.           */
#define  USBH_HID_CFG_MAX_NBR_USAGE                       15u

                                                                /*  Maximum length of transmission buffer               */
                                                                /*  The maximum length of buffer used for OUT reports.  */
#define  USBH_HID_CFG_MAX_TX_BUF_SIZE                     64u

                                                                /*  Maximum length of reception buffer                  */
                                                                /*  The maximum length of buffer used for IN reports.   */
#define  USBH_HID_CFG_MAX_RX_BUF_SIZE                    128u

                                                                /*  Maximum number of callbacks for device              */
                                                                /*  The maximum length of buffer used for IN reports.   */
#define  USBH_HID_CFG_MAX_NBR_RXCB                         2u

                                                                /*  Maximum length of report descriptor                 */
                                                                /*  The maximum length of buffer used to hold the ...   */
                                                                /*  ... report descriptor.                              */
#define  USBH_HID_CFG_MAX_REPORT_DESC_LEN                400u

                                                                /*  Maximum error count                                 */
                                                                /*  The maximum number of error that can occur.         */
#define  USBH_HID_CFG_MAX_ERR_CNT                          5u

                                                                /*  Maximum global collection for push/pop items        */
                                                                /*  The maximum number of global push/pop items.        */
#define  USBH_HID_CFG_MAX_GLOBAL                           2u

                                                                /*  Maximum collections for open/close collection       */
                                                                /*  The maximum number of collections for open/close ...*/
                                                                /*  ... collection.                                     */
#define  USBH_HID_CFG_MAX_COLL                            10u


/*
*********************************************************************************************************
*                               MASS STORAGE CLASS (MSC) CONFIGURATION
*********************************************************************************************************
*/

                                                                /*  Maximum number of MSC devices                       */
                                                                /*  The maximum number of MSC devices that can be ...   */
                                                                /*  ... connected at the same time.                     */
#define  USBH_MSC_CFG_MAX_DEV                              1u


/*
*********************************************************************************************************
*                    FUTURE TECHNOLOGY DEVICES INTERNATIONAL (FTDI) CONFIGURATION
*********************************************************************************************************
*/

                                                                /*  Maximum number of FTDI devices                      */
                                                                /*  The maximum number of FTDI devices that can be ...  */
                                                                /*  ... connected at the same time.                     */
#define  USBH_FTDI_CFG_MAX_DEV                             3u

                                                                /*  Custom vendor ID for FTDI device                    */
                                                                /*  Specifies a custom vendor ID for a FTDI device ...  */
                                                                /*  ... to accept.                                      */
#define  USBH_FTDI_CFG_ID_VENDOR_CUSTOM               0x0000u

                                                                /*  Custom product ID for FTDI device                   */
                                                                /*  Specifies a custom product ID for a FTDI device ... */
                                                                /*  ... to accept.                                      */
#define  USBH_FTDI_CFG_ID_PRODUCT_CUSTOM              0x0000u


/*
 *********************************************************************************************************
 *                                    TRACE / DEBUG CONFIGURATION
 *********************************************************************************************************
 */

#define  USBH_CFG_PRINT_LOG                      DEF_DISABLED
#define  USBH_CFG_PRINT_ERR                      DEF_DISABLED
#define  USBH_CFG_PRINT_FNAME                    DEF_DISABLED

#include <stdio.h>
#define  USBH_PRINTF                             printf

#if  (USBH_CFG_PRINT_LOG == DEF_ENABLED)
#define  USBH_PRINT_LOG(...)         USBH_PRINTF(__VA_ARGS__)
#endif

#if  (USBH_CFG_PRINT_ERR == DEF_ENABLED)
#define  USBH_PRINT_ERR(err)         USBH_PRINTF("ERR:%s:%d:err=%d\n", __FUNCTION__, __LINE__, err);
#else
#define  USBH_PRINT_ERR(err)
#endif


/*
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*/

#endif

