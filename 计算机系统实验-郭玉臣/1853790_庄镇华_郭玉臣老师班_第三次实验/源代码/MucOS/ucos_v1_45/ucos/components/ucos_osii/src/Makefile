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


COMPILER=
CP=cp
COMPILER_FLAGS=
EXTRA_COMPILER_FLAGS=
LIB=libucos.a

CC_FLAGS = $(COMPILER_FLAGS)
ECC_FLAGS = $(EXTRA_COMPILER_FLAGS)

RELEASEDIR=../../../lib
INCLUDEDIR=../../../include
INCLUDES=-I${UCOS_SRC_BASE}/uCOS-II/Source -I${UCOS_SRC_BASE}/uCOS-II -I${INCLUDEDIR}

OUTS = *.o

include ../../../include/Makefile_def.inc
include Makefile.inc

LIBSOURCES:=${UCOS_SRC_BASE}/uCOS-II/Source/os_core.c \
            ${UCOS_SRC_BASE}/uCOS-II/Source/os_dbg_r.c \
            ${UCOS_SRC_BASE}/uCOS-II/Source/os_flag.c \
            ${UCOS_SRC_BASE}/uCOS-II/Source/os_mbox.c \
            ${UCOS_SRC_BASE}/uCOS-II/Source/os_mem.c \
            ${UCOS_SRC_BASE}/uCOS-II/Source/os_mutex.c \
            ${UCOS_SRC_BASE}/uCOS-II/Source/os_q.c \
            ${UCOS_SRC_BASE}/uCOS-II/Source/os_sem.c \
            ${UCOS_SRC_BASE}/uCOS-II/Source/os_task.c \
            ${UCOS_SRC_BASE}/uCOS-II/Source/os_time.c \
            ${UCOS_SRC_BASE}/uCOS-II/Source/os_tmr.c

LIBSOURCES += $(ARCH_LIBSOURCES)


INCLUDEFILES:=os_cfg.h

INCLUDEFILES += $(ARCH_INCLUDEFILES)


OBJECTS =	$(addsuffix .o, $(basename ${LIBSOURCES}))

libs: banner ucos_osii_libs clean

%.o: %.c
	${COMPILER} $(CC_FLAGS) $(ECC_FLAGS) $(INCLUDES) -o $@ $<

%.o: %.S
	${COMPILER} $(CC_FLAGS) $(ECC_FLAGS) $(INCLUDES) -o $@ $<

banner:
	echo "Compiling uC/OS-II"

ucos_osii_libs: ${OBJECTS}
	$(ARCHIVER) -r ${RELEASEDIR}/${LIB} ${OBJECTS}

.PHONY: include
include:
	${CP} ${INCLUDEFILES} ${INCLUDEDIR}

	${CP} ${UCOS_SRC_BASE}/uCOS-II/Source/os_trace.h ${INCLUDEDIR}
	${CP} ${UCOS_SRC_BASE}/uCOS-II/Source/os.h ${INCLUDEDIR}
	${CP} ${UCOS_SRC_BASE}/uCOS-II/Source/ucos_ii.h ${INCLUDEDIR}

	${CP} ${UCOS_SRC_BASE}/uCOS-II/Source/os_trace.h ${INCLUDEDIR}/Source
	${CP} ${UCOS_SRC_BASE}/uCOS-II/Source/os.h ${INCLUDEDIR}/Source
	${CP} ${UCOS_SRC_BASE}/uCOS-II/Source/ucos_ii.h ${INCLUDEDIR}/Source

clean:
	rm -rf ${OBJECTS}

