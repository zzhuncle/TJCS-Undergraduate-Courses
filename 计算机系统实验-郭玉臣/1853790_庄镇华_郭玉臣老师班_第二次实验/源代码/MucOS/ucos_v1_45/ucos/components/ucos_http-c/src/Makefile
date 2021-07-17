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
INCLUDES=-I./. -I./uC-TCPIP/Source -I${INCLUDEDIR}

OUTS = *.o

include ../../../include/Makefile_def.inc

DIST_LIB=libuchttpc-${UCOS_OS}-${UCOS_ARCH}.a

LIBSOURCES:=http-c_cfg.c \
            uC-HTTP/Common/http.c \
            uC-HTTP/Common/http_dict.c \
            uC-HTTP/Client/Source/http-c.c \
            uC-HTTP/Client/Source/http-c_conn.c \
            uC-HTTP/Client/Source/http-c_mem.c \
            uC-HTTP/Client/Source/http-c_req.c \
            uC-HTTP/Client/Source/http-c_resp.c \
            uC-HTTP/Client/Source/http-c_sock.c \
            uC-HTTP/Client/Source/http-c_task.c \
            uC-HTTP/Client/Source/http-c_websock.c


INCLUDEFILES:=http-c_cfg.h


ifeq (${strip ${UCOS_HTTPC_SRC_BASE}},)
OBJECTS =
else
OBJECTS =	$(addsuffix .o, $(basename ${LIBSOURCES}))
endif

libs: banner lib_copy ucos_final_libs clean

%.o: %.c
	${COMPILER} $(CC_FLAGS) $(ECC_FLAGS) $(INCLUDES) -o $@ $<

%.o: %.S
	${COMPILER} $(CC_FLAGS) $(ECC_FLAGS) $(INCLUDES) -o $@ $<

banner:
	echo "Compiling uC/HTTPc"

lib_copy:
ifeq (${strip ${UCOS_HTTPC_SRC_BASE}},)
	echo "uC/HTTPc sources not found, using pre-compiled library"
	${CP} ${UCOS_LIB_LOC}/${DIST_LIB} ${RELEASEDIR}/
	touch ${RELEASEDIR}/${DIST_LIB}
else
	echo "Building uC/HTTPc from sources at ${UCOS_HTTPC_SRC_BASE}"
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
	${CP} uC-HTTP/Common/http.h ${INCLUDEDIR}/Common/
	${CP} uC-HTTP/Common/http_dict.h ${INCLUDEDIR}/Common/
	${CP} uC-HTTP/Client/Source/http-c.h ${INCLUDEDIR}/Client/Source/
	${CP} uC-HTTP/Client/Source/http-c_conn.h ${INCLUDEDIR}/Client/Source/
	${CP} uC-HTTP/Client/Source/http-c_mem.h ${INCLUDEDIR}/Client/Source/
	${CP} uC-HTTP/Client/Source/http-c_req.h ${INCLUDEDIR}/Client/Source/
	${CP} uC-HTTP/Client/Source/http-c_resp.h ${INCLUDEDIR}/Client/Source/
	${CP} uC-HTTP/Client/Source/http-c_sock.h ${INCLUDEDIR}/Client/Source/
	${CP} uC-HTTP/Client/Source/http-c_task.h ${INCLUDEDIR}/Client/Source/
	${CP} uC-HTTP/Client/Source/http-c_type.h ${INCLUDEDIR}/Client/Source/
	${CP} uC-HTTP/Client/Source/http-c_websock.h ${INCLUDEDIR}/Client/Source/

clean:
	rm -rf ${OBJECTS}

