#############################################################################
#                               uCOS XSDK BSP                               #
#                                                                           #
#        (c) Copyright 2014-2020; Silicon Laboratories Inc.,                #
#               400 W. Cesar Chavez, Austin, TX 78701                       #
#                                                                           #
#  All rights reserved. Protected by international copyright laws.          #
#                                                                           #
# Your use of this software is subject to your acceptance of the terms      #
# of a Silicon Labs Micrium software license, which can be obtained by      #
# contacting info@micrium.com. If you do not agree to the terms of this     #
# license, you may not use this software.                                   #
#                                                                           #
# Please help us continue to provide the Embedded community with the finest #
# software available. Your honesty is greatly appreciated.                  #
#                                                                           #
#   You can find our product's documentation at: doc.micrium.com            #
#                                                                           #
#         For more information visit us at: www.micrium.com                 #
#############################################################################


ARCH_LIBSOURCES:=${UCOS_SRC_BASE}/uC-CPU/ARM-Cortex-A/ARMv7-A/GNU/cpu_a.S \
                 ${UCOS_SRC_BASE}/uC-CPU/Cache/ARM/armv7_generic_l1_l2c310_l2/cpu_cache_armv7_generic_l1_l2c310_l2.c \
                 ${UCOS_SRC_BASE}/uC-CPU/Cache/ARM/armv7_generic_l1_l2c310_l2/GNU/cpu_cache_armv7_generic_l1_l2c310_l2_a.S \
                 ps7/cpu_bsp.c

ARCH_INCLUDEFILES:=${UCOS_SRC_BASE}/uC-CPU/ARM-Cortex-A/ARMv7-A/GNU/cpu.h
