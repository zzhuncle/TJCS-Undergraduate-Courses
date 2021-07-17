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
* Filename : auth.c
* Version  : V1.01.02
*********************************************************************************************************
* Note(s)  : (1) 'goto' statements were used in this software module. Their usage
*                is restricted to cleanup purposes in exceptional program flow (e.g.
*                error handling), in compliance with CERT MEM12-C and MISRA C:2012
*                rules 15.2, 15.3 and 15.4.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*********************************************************************************************************
*/

#include  <lib_str.h>
#include  <KAL/kal.h>
#include  "auth.h"


/*
*********************************************************************************************************
*********************************************************************************************************
*                                           LOCAL DATA TYPES
*********************************************************************************************************
*********************************************************************************************************
*/

typedef  struct  auth_user_credentials {                        /* --------------- AUTH USER CREDENTIALS -------------- */
    AUTH_USER  User;                                            /* User structure.                                      */
    CPU_CHAR   Pwd[AUTH_PWD_MAX_LENGTH];                        /* Password for this user.                              */
} AUTH_USER_CREDENTIALS;


/*
*********************************************************************************************************
*********************************************************************************************************
*                                           GLOBAL VARIABLES
*********************************************************************************************************
*********************************************************************************************************
*/

AUTH_USER  Auth_RootUser = {
        { 'r', 'o', 'o', 't', '\0' },
        AUTH_RIGHT_ROOT
};


/*
*********************************************************************************************************
*********************************************************************************************************
*                                        LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*********************************************************************************************************
*/

static  KAL_LOCK_HANDLE        Auth_LockHandle;

static  CPU_SIZE_T             Auth_UserNbr = 1;

static  AUTH_USER_CREDENTIALS  Auth_UsersCredentials[AUTH_NB_USERS_MAX] = {
        {
            {
                    { 'r', 'o', 'o', 't', '\0' },
                    AUTH_RIGHT_ROOT
            },
            { 'a', 'd', 'm', 'i', 'n', '\0' }
        }
};


/*
*********************************************************************************************************
*********************************************************************************************************
*                                       LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*********************************************************************************************************
*/

static  CPU_BOOLEAN  Auth_GetUserHandler (const  CPU_CHAR   *p_name,
                                                 AUTH_USER  *p_user,
                                                 RTOS_ERR   *p_err);


/*
*********************************************************************************************************
*********************************************************************************************************
*                                            GLOBAL FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                              Auth_Init()
*
* Description : (1) Initialize Authentication module:
*
*                   (a) Create Lock.
*
* Argument(s) : p_err       Pointer to variable that will receive the return error code from this function :
*
*                               --------------- RETURNED BY KAL_LockCreate() --------------
*                               See KAL_LockCreate() for additional return error codes.
*
* Return(s)   : DEF_OK,   if initialization was successful.
*               DEF_FAIL, otherwise.
*
* Note(s)     : none.
*********************************************************************************************************
*/

CPU_BOOLEAN  Auth_Init (RTOS_ERR  *p_err)
{
    CPU_BOOLEAN  res = DEF_OK;


    Auth_LockHandle = KAL_LockCreate("Auth Lock",
                                      KAL_OPT_CREATE_NONE,
                                      p_err);
    if (*p_err != RTOS_ERR_NONE) {
        res = DEF_FAIL;
    }

    return (res);
}


/*
*********************************************************************************************************
*                                         Auth_CreateUser()
*
* Description : Create a user and fill the user structure provided.
*
* Argument(s) : p_name  Pointer to user name string.
*
*               p_pwd   Pointer to password string.
*
*               p_user  Pointer to user object to fill.
*
*               p_err   Pointer to variable that will receive the return error code from this function :
*
*                           RTOS_ERR_NONE
*                           RTOS_ERR_INVALID_STR_LEN
*                           RTOS_ERR_NO_MORE_RSRC
*                           RTOS_ERR_ALREADY_EXISTS
*
*                               ----------- RETURNED BY KAL_LockAcquire/Release() ---------
*                               See KAL_LockAcquire/Release() for additional return error codes.
*
* Return(s)   : DEF_OK,   if user created successfully.
*               DEF_FAIL, otherwise.
*
* Note(s)     : None.
*********************************************************************************************************
*/

CPU_BOOLEAN  Auth_CreateUser (const  CPU_CHAR   *p_name,
                              const  CPU_CHAR   *p_pwd,
                                     AUTH_USER  *p_user,
                                     RTOS_ERR   *p_err)
{
    AUTH_USER_CREDENTIALS  *p_user_cred;
    CPU_SIZE_T              name_len;
    CPU_SIZE_T              pwd_len;
    CPU_SIZE_T              i;
    CPU_INT16S              cmp_result;
    CPU_BOOLEAN             result     = DEF_FAIL;
    RTOS_ERR                local_err;


    KAL_LockAcquire(Auth_LockHandle, KAL_OPT_PEND_NONE, KAL_TIMEOUT_INFINITE, p_err);
    if (*p_err != RTOS_ERR_NONE) {
        goto exit;
    }

    name_len = Str_Len_N(p_name, AUTH_NAME_MAX_LENGTH + 1);
    pwd_len  = Str_Len_N(p_pwd,  AUTH_PWD_MAX_LENGTH  + 1);

    if (name_len > AUTH_NAME_MAX_LENGTH) {
       *p_err = RTOS_ERR_INVALID_STR_LEN;
        goto exit_release;
    }

    if (pwd_len > AUTH_PWD_MAX_LENGTH) {
       *p_err = RTOS_ERR_INVALID_STR_LEN;
        goto exit_release;
    }


    if (Auth_UserNbr >= AUTH_NB_USERS_MAX) {
       *p_err = RTOS_ERR_NO_MORE_RSRC;
        goto exit_release;
    }

    for (i = 0; i < Auth_UserNbr; ++i) {

        cmp_result = Str_Cmp_N(Auth_UsersCredentials[i].User.Name, p_name, name_len);
        if (cmp_result == 0) {
           *p_err = RTOS_ERR_ALREADY_EXISTS;
            goto exit_release;
        }
    }

    p_user_cred = &Auth_UsersCredentials[Auth_UserNbr];
    ++Auth_UserNbr;

    name_len = DEF_MIN(name_len + 1, AUTH_NAME_MAX_LENGTH);
    pwd_len  = DEF_MIN(pwd_len + 1, AUTH_PWD_MAX_LENGTH);

    (void)Str_Copy_N(p_user_cred->User.Name,
                     p_name,
                     name_len);

    (void)Str_Copy_N(p_user_cred->Pwd,
                     p_pwd,
                     pwd_len);

    p_user_cred->User.Rights = AUTH_RIGHT_NONE;

    (void)Str_Copy_N(p_user->Name,
                     p_name,
                     name_len);

    p_user->Rights = AUTH_RIGHT_NONE;


    result = DEF_OK;
   *p_err  = RTOS_ERR_NONE;


exit_release:
    KAL_LockRelease(Auth_LockHandle, &local_err);
    (void)local_err;

exit:
    return (result);
}


/*
*********************************************************************************************************
*                                        Auth_ChangePassword()
*
* Description : Change the user's password.
*
* Argument(s) : p_user      Pointer to user object.
*
*               p_pwd       Pointer to the new password.
*
*               p_as_user   Pointer to user that have the permission level to do the action.
*                               Must be the same as p_user or the ROOT user.
*
*               p_err       Pointer to variable that will receive the return error code from this function :
*
*                               RTOS_ERR_NONE
*                               RTOS_ERR_INVALID_STR_LEN
*                               RTOS_ERR_PERMISSION
*                               RTOS_ERR_NOT_FOUND
*
*                               ----------- RETURNED BY KAL_LockAcquire/Release() ---------
*                               See KAL_LockAcquire/Release() for additional return error codes.
*
* Return(s)   : DEF_OK,   if password changed successfully.
*               DEF_FAIL, otherwise.
*
* Note(s)     : none.
*********************************************************************************************************
*/

#if 0
CPU_BOOLEAN  Auth_ChangePassword (       AUTH_USER  *p_user,
                                  const  CPU_CHAR   *p_pwd,
                                  const  AUTH_USER  *p_as_user,
                                         RTOS_ERR   *p_err)
{
    CPU_SIZE_T    name_len;
    CPU_SIZE_T    pwd_len;
    CPU_SIZE_T    i;
    CPU_INT16S    cmp_result;
    CPU_BOOLEAN   result     = DEF_FAIL;
    RTOS_ERR      local_err;


    KAL_LockAcquire(Auth_LockHandle, KAL_OPT_PEND_NONE, KAL_TIMEOUT_INFINITE, p_err);
    if (*p_err != RTOS_ERR_NONE) {
        goto exit;
    }

    name_len = Str_Len_N(p_user->Name, AUTH_NAME_MAX_LENGTH + 1);
    pwd_len  = Str_Len_N(p_pwd,  AUTH_PWD_MAX_LENGTH  + 1);

    if (pwd_len > AUTH_PWD_MAX_LENGTH) {
       *p_err = RTOS_ERR_INVALID_STR_LEN;
        goto exit_release;
    }

    if ((Str_Cmp(p_as_user->Name, p_user->Name)             != 0)      &&
        (DEF_BIT_IS_CLR(p_as_user->Rights, AUTH_RIGHT_ROOT) == DEF_YES)) {
       *p_err = RTOS_ERR_PERMISSION;
        goto exit_release;
    }

    for (i = 0; i < Auth_UserNbr; ++i) {

        cmp_result = Str_Cmp_N(Auth_UsersCredentials[i].User.Name, p_user->Name, name_len);
        if (cmp_result == 0) {

            (void)Str_Copy_N(Auth_UsersCredentials[i].Pwd,
                             p_pwd,
                             pwd_len);

            result = DEF_OK;
           *p_err  = RTOS_ERR_NONE;
            goto exit_release;
        }
    }

   *p_err = RTOS_ERR_NOT_FOUND;


exit_release:
    KAL_LockRelease(Auth_LockHandle, &local_err);
    (void)local_err;

exit:
    return (result);
}
#endif


/*
*********************************************************************************************************
*                                           Auth_GetUser()
*
* Description : Get the user structure with the given name.
*
* Argument(s) : p_name      Pointer to user name string to retrieve.
*
*               p_user      Pointer to user object that will be filled with the data retrieved.
*
*               p_err       Pointer to variable that will receive the return error code from this function :
*
*                               RTOS_ERR_NONE
*
*                               ----------- RETURNED BY KAL_LockAcquire/Release() ---------
*                               See KAL_LockAcquire/Release() for additional return error codes.
*
*                               ------------ RETURNED BY Auth_GetUserHandler() ------------
*                               See Auth_GetUserHandler() for additional return error codes.
*
* Return(s)   : DEF_OK,   if user was successfully found.
*               DEF_FAIL, otherwise.
*
* Note(s)     : none.
*********************************************************************************************************
*/

CPU_BOOLEAN  Auth_GetUser (const  CPU_CHAR   *p_name,
                                  AUTH_USER  *p_user,
                                  RTOS_ERR   *p_err)
{
    CPU_BOOLEAN  result;
    RTOS_ERR     local_err;


    KAL_LockAcquire(Auth_LockHandle, KAL_OPT_PEND_NONE, KAL_TIMEOUT_INFINITE, p_err);
    if (*p_err != RTOS_ERR_NONE) {
        result = DEF_FAIL;
        goto exit;
    }

    result = Auth_GetUserHandler(p_name, p_user, p_err);

    KAL_LockRelease(Auth_LockHandle, &local_err);
    (void)local_err;

exit:
    return (result);
}


/*
*********************************************************************************************************
*                                     Auth_ValidateCredentials()
*
* Description : Validates the user and password tuple with known users.
*
* Argument(s) : p_name      Pointer to user name string.
*
*               p_pwd       Pointer to password string.
*
*               p_user      Pointer to user object that will be filled with the data retrieved.
*
*               p_err       Pointer to variable that will receive the return error code from this function :
*
*                               RTOS_ERR_NONE
*                               RTOS_ERR_INVALID_STR_LEN
*                               RTOS_ERR_INVALID_CREDENTIALS
*
*                               ----------- RETURNED BY KAL_LockAcquire/Release() ---------
*                               See KAL_LockAcquire/Release() for additional return error codes.
*
* Return(s)   : DEF_OK,   if credentials are valid.
*               DEF_FAIL, otherwise.
*
* Note(s)     : none.
*********************************************************************************************************
*/

CPU_BOOLEAN  Auth_ValidateCredentials (const  CPU_CHAR   *p_name,
                                       const  CPU_CHAR   *p_pwd,
                                              AUTH_USER  *p_user,
                                              RTOS_ERR   *p_err)
{
    CPU_SIZE_T   name_len;
    CPU_SIZE_T   pwd_len;
    CPU_SIZE_T   i;
    CPU_INT16S   cmp_result;
    CPU_BOOLEAN  result     = DEF_FAIL;
    RTOS_ERR     local_err;


    KAL_LockAcquire(Auth_LockHandle, KAL_OPT_PEND_NONE, KAL_TIMEOUT_INFINITE, p_err);
    if (*p_err != RTOS_ERR_NONE) {
        goto exit;
    }

    name_len = Str_Len_N(p_name, AUTH_NAME_MAX_LENGTH + 1);
    pwd_len  = Str_Len_N(p_pwd,  AUTH_PWD_MAX_LENGTH  + 1);

    if (name_len > AUTH_NAME_MAX_LENGTH) {
       *p_err = RTOS_ERR_INVALID_STR_LEN;
        goto exit_release;
    }

    if (pwd_len > AUTH_PWD_MAX_LENGTH) {
       *p_err = RTOS_ERR_INVALID_STR_LEN;
        goto exit_release;
    }

    for (i = 0; i < Auth_UserNbr; ++i) {

        cmp_result = Str_Cmp_N(Auth_UsersCredentials[i].User.Name, p_name, name_len);
        if (cmp_result == 0) {

            cmp_result = Str_Cmp_N(Auth_UsersCredentials[i].Pwd, p_pwd, pwd_len);
            if (cmp_result == 0) {

                (void)Str_Copy_N(p_user->Name, p_name, name_len);
                p_user->Rights = Auth_UsersCredentials[i].User.Rights;

                result = DEF_OK;
               *p_err  = RTOS_ERR_NONE;
                goto exit_release;
            }
            break;
        }
    }

   *p_err = RTOS_ERR_INVALID_CREDENTIALS;


exit_release:
    KAL_LockRelease(Auth_LockHandle, &local_err);
    (void)local_err;

exit:
    return (result);
}


/*
*********************************************************************************************************
*                                         Auth_GrantRight()
*
* Description : Grants a right to a user as another user (limits the rights granted).
*
* Argument(s) : right       New right to grant.
*
*               p_user      Pointer to user object that will received the new right.
*
*               p_as_user   Pointer to user that has the permission level to do the action.
*
*               p_err       Pointer to variable that will receive the return error code from this function :
*
*                               RTOS_ERR_NONE
*                               RTOS_ERR_PERMISSION
*                               RTOS_ERR_NOT_FOUND
*
*                               ----------- RETURNED BY KAL_LockAcquire/Release() ---------
*                               See KAL_LockAcquire/Release() for additional return error codes.
*
*                               ------------ RETURNED BY Auth_GetUserHandler() ------------
*                               See Auth_GetUserHandler() for additional return error codes.
*
* Return(s)   : DEF_OK,   if right was granted successfully.
*               DEF_FAIL, otherwise.
*
* Note(s)     : none.
*********************************************************************************************************
*/

CPU_BOOLEAN  Auth_GrantRight (AUTH_RIGHT   right,
                              AUTH_USER   *p_user,
                              AUTH_USER   *p_as_user,
                              RTOS_ERR    *p_err)
{
    CPU_SIZE_T   name_len;
    CPU_SIZE_T   i;
    CPU_INT16S   cmp_result;
    CPU_BOOLEAN  result     = DEF_FAIL;
    RTOS_ERR     local_err;


    KAL_LockAcquire(Auth_LockHandle, KAL_OPT_PEND_NONE, KAL_TIMEOUT_INFINITE, p_err);
    if (*p_err != RTOS_ERR_NONE) {
        goto exit;
    }

    (void)Auth_GetUserHandler(p_as_user->Name, p_as_user, p_err);
    if (*p_err != RTOS_ERR_NONE) {
        goto exit_release;
    }

    if (((DEF_BIT_IS_SET(p_as_user->Rights, AUTH_RIGHT_MNG)  == DEF_NO)  ||
         (DEF_BIT_IS_SET(p_as_user->Rights, right)           == DEF_NO)) &&
        (DEF_BIT_IS_SET(p_as_user->Rights, AUTH_RIGHT_ROOT)  == DEF_NO))  {
       *p_err = RTOS_ERR_PERMISSION;
        goto exit_release;
    }

    name_len = Str_Len_N(p_user->Name, AUTH_NAME_MAX_LENGTH + 1);

    for (i = 0; i < Auth_UserNbr; ++i) {

        cmp_result = Str_Cmp_N(Auth_UsersCredentials[i].User.Name, p_user->Name, name_len);
        if (cmp_result == 0) {

            DEF_BIT_SET(Auth_UsersCredentials[i].User.Rights, right);

            p_user->Rights = Auth_UsersCredentials[i].User.Rights;

            result = DEF_OK;
           *p_err  = RTOS_ERR_NONE;
            goto exit_release;
        }
    }

   *p_err = RTOS_ERR_NOT_FOUND;


exit_release:
    KAL_LockRelease(Auth_LockHandle, &local_err);
    (void)local_err;

exit:
    return (result);
}


/*
*********************************************************************************************************
*                                          Auth_RevokeRight()
*
* Description : Revokes the right of a specified user.
*
* Argument(s) : right       Right to revoke.
*
*               p_user      Pointer to user object which right will be revoked.
*
*               p_as_user   Pointer to user that has the permission level to do the action.
*
*               p_err       Pointer to variable that will receive the return error code from this function :
*
*                               RTOS_ERR_NONE
*                               RTOS_ERR_PERMISSION
*                               RTOS_ERR_NOT_FOUND
*
*                               ----------- RETURNED BY KAL_LockAcquire/Release() ---------
*                               See KAL_LockAcquire/Release() for additional return error codes.
*
*                               ------------ RETURNED BY Auth_GetUserHandler() ------------
*                               See Auth_GetUserHandler() for additional return error codes.
*
* Return(s)   : DEF_OK,   if right was revoked successfully.
*               DEF_FAIL, otherwise.
*
* Note(s)     : none.
*********************************************************************************************************
*/

CPU_BOOLEAN  Auth_RevokeRight (AUTH_RIGHT   right,
                               AUTH_USER   *p_user,
                               AUTH_USER   *p_as_user,
                               RTOS_ERR    *p_err)
{
    CPU_SIZE_T   name_len;
    CPU_SIZE_T   i;
    CPU_INT16S   cmp_result;
    CPU_BOOLEAN  result     = DEF_FAIL;
    RTOS_ERR     local_err;


    KAL_LockAcquire(Auth_LockHandle, KAL_OPT_PEND_NONE, KAL_TIMEOUT_INFINITE, p_err);
    if (*p_err != RTOS_ERR_NONE) {
        goto exit;
    }

    (void)Auth_GetUserHandler(p_as_user->Name, p_as_user, p_err);
    if (*p_err != RTOS_ERR_NONE) {
        goto exit_release;
    }

                                                                /* This implementation allows the ROOT user ...         */
                                                                /* ... to revoke it's own ROOT right.                   */
    if (((DEF_BIT_IS_SET(p_as_user->Rights, AUTH_RIGHT_MNG)  == DEF_NO) ||
         (DEF_BIT_IS_SET(p_as_user->Rights, right)           == DEF_NO)   ) &&
        (DEF_BIT_IS_SET(p_as_user->Rights, AUTH_RIGHT_ROOT)  == DEF_NO    )    ) {
       *p_err = RTOS_ERR_PERMISSION;
        goto exit_release;
    }

    name_len = Str_Len_N(p_user->Name, AUTH_NAME_MAX_LENGTH + 1);

    for (i = 0; i < Auth_UserNbr; ++i) {

        cmp_result = Str_Cmp_N(Auth_UsersCredentials[i].User.Name, p_user->Name, name_len);
        if (cmp_result == 0) {

            DEF_BIT_CLR(Auth_UsersCredentials[i].User.Rights, right);

            p_user->Rights = Auth_UsersCredentials[i].User.Rights;

            result = DEF_OK;
           *p_err  = RTOS_ERR_NONE;
            goto exit_release;
        }
    }

   *p_err = RTOS_ERR_NOT_FOUND;


exit_release:
    KAL_LockRelease(Auth_LockHandle, &local_err);
    (void)local_err;

exit:
    return (result);
}


/*
*********************************************************************************************************
*********************************************************************************************************
*                                          LOCAL FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                         Auth_GetUserHandler()
*
* Description : Get the user structure with the given name.
*
* Argument(s) : p_name      Pointer to user name string to retrieve.
*
*               p_user      Pointer to user object that will be filled with the data retrieved.
*
*               p_err       Pointer to variable that will receive the return error code from this function :
*
*                               RTOS_ERR_NONE
*                               RTOS_ERR_NOT_FOUND
*
* Return(s)   : DEF_OK,   if user was successfully found.
*               DEF_FAIL, otherwise.
*
* Note(s)     : None.
*********************************************************************************************************
*/

static  CPU_BOOLEAN  Auth_GetUserHandler (const  CPU_CHAR   *p_name,
                                                 AUTH_USER  *p_user,
                                                 RTOS_ERR   *p_err)
{
    CPU_SIZE_T   name_len;
    CPU_SIZE_T   i;
    CPU_INT16S   cmp_result;
    CPU_BOOLEAN  result;


    name_len = Str_Len_N(p_name, AUTH_NAME_MAX_LENGTH + 1);

    for (i = 0; i < Auth_UserNbr; ++i) {

        cmp_result = Str_Cmp_N(Auth_UsersCredentials[i].User.Name, p_user->Name, name_len);
        if (cmp_result == 0) {

            (void)Str_Copy_N(p_user->Name,
                             Auth_UsersCredentials[i].User.Name,
                             name_len);

            p_user->Rights = Auth_UsersCredentials[i].User.Rights;

            result = DEF_OK;
           *p_err  = RTOS_ERR_NONE;
            goto exit;
        }
    }

    result = DEF_FAIL;
   *p_err  = RTOS_ERR_NOT_FOUND;


exit:
   return (result);
}
