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
*                               uC/Common - Singly-linked Lists (SList)
*
* Filename : slist.c
* Version  : V1.01.02
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*********************************************************************************************************
*/

#define  MICRIUM_SOURCE
#define  COLL_SLIST_MODULE

#include  "slist.h"
#include  <lib_def.h>
#include  <cpu_core.h>


/*
*********************************************************************************************************
*********************************************************************************************************
*                                           GLOBAL FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             SList_Init()
*
* Description : Initializes a singly-linked list.
*
* Argument(s) : p_head_ptr      Pointer to pointer of head element of list.
*
* Return(s)   : none.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  SList_Init (SLIST_MEMBER  **p_head_ptr)
{
   *p_head_ptr = DEF_NULL;
}


/*
*********************************************************************************************************
*                                             SList_Push()
*
* Description : Add given item at beginning of list.
*
* Argument(s) : p_head_ptr      Pointer to pointer of head element of list.
*
*               p_item          Pointer to item to add.
*
* Return(s)   : none.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  SList_Push (SLIST_MEMBER  **p_head_ptr,
                  SLIST_MEMBER   *p_item)
{
    p_item->p_next = *p_head_ptr;
   *p_head_ptr     =  p_item;
}


/*
*********************************************************************************************************
*                                           SList_PushBack()
*
* Description : Add item at end of list.
*
* Argument(s) : p_head_ptr      Pointer to pointer of head element of list.
*
*               p_item          Pointer to item to add.
*
* Return(s)   : none.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  SList_PushBack (SLIST_MEMBER  **p_head_ptr,
                      SLIST_MEMBER   *p_item)
{
    SLIST_MEMBER  **p_next_ptr = p_head_ptr;


    while (*p_next_ptr != DEF_NULL) {
        p_next_ptr = &((*p_next_ptr)->p_next);
    }

    p_item->p_next = DEF_NULL;
   *p_next_ptr     = p_item;
}


/*
*********************************************************************************************************
*                                              SList_Pop()
*
* Description : Removes and returns first element of list.
*
* Argument(s) : p_head_ptr      Pointer to pointer of head element of list.
*
* Return(s)   : Pointer to item that was at top of the list.
*
* Note(s)     : none.
*********************************************************************************************************
*/

SLIST_MEMBER  *SList_Pop (SLIST_MEMBER  **p_head_ptr)
{
    SLIST_MEMBER  *p_item;

    p_item = *p_head_ptr;
    if (p_item == DEF_NULL) {
        return (DEF_NULL);
    }

   *p_head_ptr = p_item->p_next;

    p_item->p_next = DEF_NULL;

    return (p_item);
}


/*
*********************************************************************************************************
*                                              SList_Add()
*
* Description : Add item after given item.
*
* Argument(s) : p_item      Pointer to item to add.
*
*               p_pos       Pointer to item after which the item to add will be inserted.
*
* Return(s)   : none.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  SList_Add (SLIST_MEMBER  *p_item,
                 SLIST_MEMBER  *p_pos)
{
    p_item->p_next = p_pos->p_next;
    p_pos->p_next  = p_item;
}


/*
*********************************************************************************************************
*                                              SList_Rem()
*
* Description : Remove item from list.
*
* Argument(s) : p_head_ptr      Pointer to pointer of head element of list.
*
*               p_item          Pointer to item to remove.
*
* Return(s)   : none.
*
* Note(s)     : (1) A CPU_SW_EXCEPTION() is thrown if the item is not found within the list.
*********************************************************************************************************
*/

void  SList_Rem (SLIST_MEMBER  **p_head_ptr,
                 SLIST_MEMBER   *p_item)
{
    SLIST_MEMBER  **p_next_ptr;


    for (p_next_ptr = p_head_ptr; *p_next_ptr != DEF_NULL; p_next_ptr = &((*p_next_ptr)->p_next)) {
        if (*p_next_ptr == p_item) {
           *p_next_ptr = p_item->p_next;
            return;
        }
    }

    CPU_SW_EXCEPTION(;);                                        /* See Note #1.                                         */
}


/*
*********************************************************************************************************
*                                             SList_Sort()
*
* Description : Sorts list items.
*
* Argument(s) : p_head_ptr      Pointer to pointer of head element of list.
*
*               cmp_fnct        Pointer to function to use for sorting the list.
*                                   p_item_l    Pointer to left  item.
*                                   p_item_r    Pointer to right item.
*                                   Returns whether the two items are ordered (DEF_YES) or not (DEF_NO).
*
* Return(s)   : none.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  SList_Sort (SLIST_MEMBER  **p_head_ptr,
                  CPU_BOOLEAN   (*cmp_fnct)(SLIST_MEMBER  *p_item_l,
                                            SLIST_MEMBER  *p_item_r))
{
    CPU_BOOLEAN     swapped;
    SLIST_MEMBER  **pp_item_l;


    do {
        swapped = DEF_NO;

        pp_item_l = p_head_ptr;
                                                                /* Loop until end of list is found.                     */
        while ((*pp_item_l != DEF_NULL) && ((*pp_item_l)->p_next != DEF_NULL)) {
            SLIST_MEMBER  *p_item_r = (*pp_item_l)->p_next;
            CPU_BOOLEAN    ordered;


            ordered = cmp_fnct(*pp_item_l, p_item_r);           /* Call provided compare fnct.                          */
            if (ordered == DEF_NO) {                            /* If order is not correct, swap items.                 */
                SLIST_MEMBER  *p_tmp = p_item_r->p_next;


                p_item_r->p_next   =  *pp_item_l;               /* Swap the two items.                                  */
              (*pp_item_l)->p_next =   p_tmp;
               *pp_item_l          =   p_item_r;
                pp_item_l          = &(p_item_r->p_next);
                swapped            =   DEF_YES;                 /* Indicate a swap has been done.                       */
            } else {
                pp_item_l = &((*pp_item_l)->p_next);
            }
        }
    } while (swapped == DEF_YES);                               /* Re-loop until no items have been swapped.            */
}
