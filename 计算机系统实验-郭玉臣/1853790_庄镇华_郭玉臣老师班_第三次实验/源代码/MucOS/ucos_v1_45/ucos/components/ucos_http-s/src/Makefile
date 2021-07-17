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

DIST_LIB=libuchttps-${UCOS_OS}-${UCOS_ARCH}.a

LIBSOURCES:=uC-HTTP/Common/http.c \
            uC-HTTP/Common/http_dict.c \
            uC-HTTP/Server/Source/http-s.c \
            uC-HTTP/Server/Source/http-s_conn.c \
            uC-HTTP/Server/Source/http-s_mem.c \
            uC-HTTP/Server/Source/http-s_req.c \
            uC-HTTP/Server/Source/http-s_resp.c \
            uC-HTTP/Server/Source/http-s_sock.c \
            uC-HTTP/Server/Source/http-s_str.c \
            uC-HTTP/Server/Source/http-s_task.c \
            uC-HTTP/Server/FS/Static/http-s_fs_static.c



INCLUDEFILES:=http-s_cfg.h \
              http-s_fs_static_cfg.h


ifeq (${strip ${UCOS_HTTPS_SRC_BASE}},)
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
	echo "Compiling uC/HTTPs"

lib_copy:
ifeq (${strip ${UCOS_HTTPC_SRC_BASE}},)
	echo "uC/HTTPs sources not found, using pre-compiled library"
	${CP} ${UCOS_LIB_LOC}/${DIST_LIB} ${RELEASEDIR}/
	touch ${RELEASEDIR}/${DIST_LIB}
else
	echo "Building uC/HTTPs from sources at ${UCOS_HTTPS_SRC_BASE}"
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
	${CP} uC-HTTP/Server/Source/http-s.h ${INCLUDEDIR}/Server/Source/
	${CP} uC-HTTP/Server/Source/http-s_conn.h ${INCLUDEDIR}/Server/Source/
	${CP} uC-HTTP/Server/Source/http-s_mem.h ${INCLUDEDIR}/Server/Source/
	${CP} uC-HTTP/Server/Source/http-s_req.h ${INCLUDEDIR}/Server/Source/
	${CP} uC-HTTP/Server/Source/http-s_resp.h ${INCLUDEDIR}/Server/Source/
	${CP} uC-HTTP/Server/Source/http-s_sock.h ${INCLUDEDIR}/Server/Source/
	${CP} uC-HTTP/Server/Source/http-s_task.h ${INCLUDEDIR}/Server/Source/
	${CP} uC-HTTP/Server/FS/Static/http-s_fs_static.h ${INCLUDEDIR}

clean:
	rm -rf ${OBJECTS}

