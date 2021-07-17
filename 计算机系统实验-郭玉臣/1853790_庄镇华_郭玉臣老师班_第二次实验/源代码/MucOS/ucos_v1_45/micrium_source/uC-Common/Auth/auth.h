/*
*********************************************************************************************************
*                                              uC/Common
*                                 Common Features for Micrium Stacks
*
*                         (c) Copyright 2013-2019; Silicon Laboratories Inc.,
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
*                              uC/Common - Authentication Module (Auth)
*
* Filename : auth.h
* Version  : V1.01.02
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*********************************************************************************************************
*                                               MODULE
*
* Note(s) : (1) This library header file is protected from multiple pre-processor inclusion through
*               use of the library module present pre-processor macro definition.
*********************************************************************************************************
*********************************************************************************************************
*/

#ifndef AUTH_MODULE_PRESENT                                     /* See Note #1.                                         */
#define AUTH_MODULE_PRESENT


/*
*********************************************************************************************************
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*********************************************************************************************************
*/

#include  <lib_mem.h>
#include  "../common_err.h"


/*
*********************************************************************************************************
*********************************************************************************************************
*                                               DEFINES
*********************************************************************************************************
*********************************************************************************************************
*/

#define  AUTH_PWD_MAX_LENGTH     32
#define  AUTH_NAME_MAX_LENGTH    32

#define  AUTH_NB_USERS_MAX       10


#define  AUTH_RIGHT_NONE         DEF_BIT_NONE
#define  AUTH_RIGHT_0            DEF_BIT_00
#define  AUTH_RIGHT_1            DEF_BIT_01
#define  AUTH_RIGHT_2            DEF_BIT_02
#define  AUTH_RIGHT_3            DEF_BIT_03
#define  AUTH_RIGHT_4            DEF_BIT_04
#define  AUTH_RIGHT_5            DEF_BIT_05
#define  AUTH_RIGHT_6            DEF_BIT_06
#define  AUTH_RIGHT_7            DEF_BIT_07
#define  AUTH_RIGHT_8            DEF_BIT_08
#define  AUTH_RIGHT_9            DEF_BIT_09
#define  AUTH_RIGHT_10           DEF_BIT_10
#define  AUTH_RIGHT_11           DEF_BIT_11
#define  AUTH_RIGHT_12           DEF_BIT_12
#define  AUTH_RIGHT_13           DEF_BIT_13
#define  AUTH_RIGHT_14           DEF_BIT_14
#define  AUTH_RIGHT_15           DEF_BIT_15
#define  AUTH_RIGHT_16           DEF_BIT_16
#define  AUTH_RIGHT_17           DEF_BIT_17
#define  AUTH_RIGHT_18           DEF_BIT_18
#define  AUTH_RIGHT_19           DEF_BIT_19
#define  AUTH_RIGHT_20           DEF_BIT_20
#define  AUTH_RIGHT_21           DEF_BIT_21
#define  AUTH_RIGHT_22           DEF_BIT_22
#define  AUTH_RIGHT_23           DEF_BIT_23
#define  AUTH_RIGHT_24           DEF_BIT_24
#define  AUTH_RIGHT_25           DEF_BIT_25
#define  AUTH_RIGHT_26           DEF_BIT_26
#define  AUTH_RIGHT_27           DEF_BIT_27

#define  AUTH_RIGHT_RSVD_1       DEF_BIT_28
#define  AUTH_RIGHT_RSVD_2       DEF_BIT_29

#define  AUTH_RIGHT_MNG          DEF_BIT_30
#define  AUTH_RIGHT_ROOT         DEF_BIT_31


/*
*********************************************************************************************************
*********************************************************************************************************
*                                              DATA TYPES
*********************************************************************************************************
*********************************************************************************************************
*/

typedef  CPU_INT32U  AUTH_RIGHT;                                /* Auth right is a 32-bit bitmap.                       */

typedef  struct  auth_user  {                                   /* --------------------- AUTH USER -------------------- */
    CPU_CHAR    Name[AUTH_NAME_MAX_LENGTH];                     /* Name of the user.                                    */
    AUTH_RIGHT  Rights;                                         /* Rights associated to this user.                      */
} AUTH_USER;


/*
*********************************************************************************************************
*********************************************************************************************************
*                                           GLOBAL VARIABLES
*********************************************************************************************************
*********************************************************************************************************
*/

extern  AUTH_USER  Auth_RootUser;


/*
*********************************************************************************************************
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*********************************************************************************************************
*/

CPU_BOOLEAN  Auth_Init                (        RTOS_ERR    *p_err);

CPU_BOOLEAN  Auth_CreateUser          (const   CPU_CHAR    *p_name,
                                       const   CPU_CHAR    *p_pwd,
                                               AUTH_USER   *p_user,
                                               RTOS_ERR    *p_err);

CPU_BOOLEAN  Auth_GetUser             (const   CPU_CHAR    *p_name,
                                               AUTH_USER   *p_user,
                                               RTOS_ERR    *p_err);

CPU_BOOLEAN  Auth_ValidateCredentials (const   CPU_CHAR    *p_name,
                                       const   CPU_CHAR    *p_pwd,
                                               AUTH_USER   *p_user,
                                               RTOS_ERR    *p_err);

CPU_BOOLEAN  Auth_GrantRight          (        AUTH_RIGHT   right,
                                               AUTH_USER   *p_user,
                                               AUTH_USER   *p_as_user,
                                               RTOS_ERR    *p_err);

CPU_BOOLEAN  Auth_RevokeRight         (        AUTH_RIGHT   right,
                                               AUTH_USER   *p_user,
                                               AUTH_USER   *p_as_user,
                                               RTOS_ERR    *p_err);


/*
*********************************************************************************************************
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*********************************************************************************************************
*/

#endif /* AUTH_MODULE_PRESENT */
