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
INCLUDES=-I./. -I./uC-TCPIP/Source -I${INCLUDEDIR}

OUTS = *.o

include ../../../include/Makefile_def.inc

DIST_LIB=libucdhcpc-${UCOS_OS}-${UCOS_ARCH}.a

LIBSOURCES:=uC-DHCPc/Source/dhcp-c.c \
            uC-DHCPc/OS/${UCOS_DIR_NAME}/dhcp-c_os.c


INCLUDEFILES:=dhcp-c_cfg.h


ifeq (${strip ${UCOS_DHCPC_SRC_BASE}},)
OBJECTS =
else
OBJECTS =	$(addsuffix .o, $(basename ${LIBSOURCES}))
endif

libs: banner lib_copy ucos_final_libs clean

%.o: %.c
	${COMPILER} $(CC_FLAGS) $(ECC_FLAGS) $(INCLUDES) -o $@ $<

%.o: %.s
	${COMPILER} $(CC_FLAGS) $(ECC_FLAGS) $(INCLUDES) -o $@ $<

banner:
	echo "Compiling uC/DHCPc"

lib_copy:
ifeq (${strip ${UCOS_DHCPC_SRC_BASE}},)
	echo "uC/DHCPc sources not found, using pre-compiled library"
	${CP} ${UCOS_LIB_LOC}/${DIST_LIB} ${RELEASEDIR}/
	touch ${RELEASEDIR}/${DIST_LIB}
else
	echo "Building uC/DHCPc from sources at ${UCOS_DHCPC_SRC_BASE}"
endif

${RELEASEDIR}/${DIST_LIB}: ${OBJECTS}
		$(ARCHIVER) -cr ${RELEASEDIR}/${DIST_LIB} ${OBJECTS}

ucos_final_libs: ${RELEASEDIR}/${DIST_LIB}
		rm -rf ${OBJECTS}
		$(ARCHIVER) -x ${RELEASEDIR}/${DIST_LIB}
		$(ARCHIVER) -r ${RELEASEDIR}/${LIB} *.o
		rm -rf *.o

.PHONY: include
include:
	${CP} ${INCLUDEFILES} ${INCLUDEDIR}
	${CP} ${UCOS_SRC_BASE}/uC-DHCPc/Source/dhcp-c.h ${INCLUDEDIR}/Source


clean:
	rm -rf ${OBJECTS}

