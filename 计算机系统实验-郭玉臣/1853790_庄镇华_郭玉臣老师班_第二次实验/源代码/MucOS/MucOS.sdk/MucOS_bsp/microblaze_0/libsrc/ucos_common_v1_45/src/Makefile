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
ARCHIVER=
CP=cp
COMPILER_FLAGS=
EXTRA_COMPILER_FLAGS=
LIB=libucos.a

CC_FLAGS = $(COMPILER_FLAGS)
ECC_FLAGS = $(EXTRA_COMPILER_FLAGS)

RELEASEDIR=../../../lib
INCLUDEDIR=../../../include
INCLUDES=-I./. -I./uC-CPU -I./uC-LIB  -I${INCLUDEDIR}

OUTS = *.o

include ../../../include/Makefile_def.inc
include Makefile_common_def.inc
include Makefile.inc

LIBSOURCES:=${UCOS_SRC_BASE}/uC-CPU/cpu_core.c \
            ${UCOS_SRC_BASE}/uC-LIB/lib_ascii.c \
            ${UCOS_SRC_BASE}/uC-LIB/lib_math.c \
            ${UCOS_SRC_BASE}/uC-LIB/lib_mem.c \
            ${UCOS_SRC_BASE}/uC-LIB/lib_str.c \
            ${UCOS_SRC_BASE}/uC-Common/KAL/${UCOS_DIR_NAME}/kal.c \
            ${UCOS_SRC_BASE}/uC-Common/Collections/slist.c

LIBSOURCES += $(ARCH_LIBSOURCES)

ifeq (${strip ${UCOS_CLK_EN}},"TRUE")
LIBSOURCES += ${UCOS_SRC_BASE}/uC-Clk/Source/clk.c \
              ${UCOS_SRC_BASE}/uC-Clk/OS/${UCOS_DIR_NAME}/clk_os.c
endif

ifeq (${strip ${UCOS_SHELL_EN}},"TRUE")
LIBSOURCES += ${UCOS_SRC_BASE}/uC-Shell/Source/shell.c
endif

ifeq (${strip ${UCOS_CRC_EN}},"TRUE")
LIBSOURCES += ${UCOS_SRC_BASE}/uC-CRC/Source/crc_util.c \
              ${UCOS_SRC_BASE}/uC-CRC/Source/edc_crc.c \
              ${UCOS_SRC_BASE}/uC-CRC/Source/ecc_hamming.c
endif

INCLUDEFILES:=${UCOS_SRC_BASE}/uC-CPU/cpu_core.h \
              ${UCOS_SRC_BASE}/uC-CPU/cpu_def.h \
              ${UCOS_SRC_BASE}/uC-CPU/cpu_cache.h \
              ${UCOS_SRC_BASE}/uC-LIB/lib_ascii.h \
              ${UCOS_SRC_BASE}/uC-LIB/lib_def.h \
              ${UCOS_SRC_BASE}/uC-LIB/lib_math.h \
              ${UCOS_SRC_BASE}/uC-LIB/lib_mem.h \
              ${UCOS_SRC_BASE}/uC-LIB/lib_str.h \
              cpu_cfg.h \
              lib_cfg.h \

ifeq (${strip ${UCOS_CLK_EN}},"TRUE")
INCLUDEFILES += clk_cfg.h
endif

ifeq (${strip ${UCOS_SHELL_EN}},"TRUE")
INCLUDEFILES += shell_cfg.h
endif

ifeq (${strip ${UCOS_CRC_EN}},"TRUE")
INCLUDEFILES += crc_cfg.h
endif

INCLUDEFILES += $(ARCH_INCLUDEFILES)

OBJECTS =	$(addsuffix .o, $(basename ${LIBSOURCES}))

libs: banner ucos_common_libs clean

%.o: %.c
	${MKDIR} -p $(@D)
	${COMPILER} $(CC_FLAGS) $(ECC_FLAGS) $(INCLUDES) -o $@ $<

%.o: %.S
	${MKDIR} -p $(@D)
	${COMPILER} $(CC_FLAGS) $(ECC_FLAGS) $(INCLUDES) -o $@ $<


banner:
	echo "Compiling uC/OS Common Components"

ucos_common_libs: ${OBJECTS}
	$(ARCHIVER) -r ${RELEASEDIR}/${LIB} ${OBJECTS}

.PHONY: include
include:
	${CP} ${INCLUDEFILES} ${INCLUDEDIR}
	${CP} ${UCOS_SRC_BASE}/uC-Common/common.h  ${INCLUDEDIR}
	${CP} ${UCOS_SRC_BASE}/uC-Common/common_err.h  ${INCLUDEDIR}
	${CP} ${UCOS_SRC_BASE}/uC-Common/KAL/kal.h  ${INCLUDEDIR}/KAL
	${CP} ${UCOS_SRC_BASE}/uC-Common/Collections/slist.h ${INCLUDEDIR}/Collections/slist.h
ifeq (${strip ${UCOS_CLK_EN}},"TRUE")
	${CP} ${UCOS_SRC_BASE}/uC-Clk/Source/clk.h ${INCLUDEDIR}/Source
	${CP} ${UCOS_SRC_BASE}/uC-Clk/Source/clk.h ${INCLUDEDIR}
endif
ifeq (${strip ${UCOS_CRC_EN}},"TRUE")
	${CP} ${UCOS_SRC_BASE}/uC-CRC/Source/crc_util.h ${INCLUDEDIR}/Source
	${CP} ${UCOS_SRC_BASE}/uC-CRC/Source/crc_util.h ${INCLUDEDIR}
	${CP} ${UCOS_SRC_BASE}/uC-CRC/Source/ecc.h ${INCLUDEDIR}/Source
	${CP} ${UCOS_SRC_BASE}/uC-CRC/Source/ecc.h ${INCLUDEDIR}
	${CP} ${UCOS_SRC_BASE}/uC-CRC/Source/edc_crc.h ${INCLUDEDIR}/Source
	${CP} ${UCOS_SRC_BASE}/uC-CRC/Source/edc_crc.h ${INCLUDEDIR}
	${CP} ${UCOS_SRC_BASE}/uC-CRC/Source/ecc_hamming.h ${INCLUDEDIR}/Source
	${CP} ${UCOS_SRC_BASE}/uC-CRC/Source/ecc_hamming.h ${INCLUDEDIR}
endif
ifeq (${strip ${UCOS_SHELL_EN}},"TRUE")
	${CP} ${UCOS_SRC_BASE}/uC-Shell/Source/shell.h ${INCLUDEDIR}/Source
	${CP} ${UCOS_SRC_BASE}/uC-Shell/Source/shell.h ${INCLUDEDIR}
endif

clean:
	rm -rf ${OBJECTS}

