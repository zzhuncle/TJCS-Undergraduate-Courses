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


ARCH_LIBSOURCES:=${UCOS_SRC_BASE}/uCOS-II/Ports/MicroBlaze/GNU/os_cpu_a.S \
                 ${UCOS_SRC_BASE}/uCOS-II/Ports/MicroBlaze/GNU/os_cpu_c.c \
                 ./bsp/mb/ucos_osii_bsp.c


ARCH_INCLUDEFILES:=${UCOS_SRC_BASE}/uCOS-II/Ports/MicroBlaze/GNU/os_cpu.h \
                   ./bsp/mb/ucos_osii_bsp.h
