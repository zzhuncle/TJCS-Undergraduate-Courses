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
INCLUDES=-I./. -I${INCLUDEDIR}

OUTS = *.o

include ../../../include/Makefile_def.inc

DIST_LIB=libuctcpip-${UCOS_OS}-${UCOS_ARCH}.a

LIBSOURCES= net_cfg.c \
            uC-TCPIP/Source/net.c \
            uC-TCPIP/Source/net_app.c \
            uC-TCPIP/Source/net_ascii.c \
            uC-TCPIP/Source/net_bsd.c \
            uC-TCPIP/Source/net_buf.c \
            uC-TCPIP/Source/net_cache.c \
            uC-TCPIP/Source/net_conn.c \
            uC-TCPIP/Source/net_ctr.c \
            uC-TCPIP/Source/net_dict.c \
            uC-TCPIP/Source/net_icmp.c \
            uC-TCPIP/Source/net_ip.c \
            uC-TCPIP/Source/net_mgr.c \
            uC-TCPIP/Source/net_sock.c \
            uC-TCPIP/Source/net_stat.c \
            uC-TCPIP/Source/net_tcp.c \
            uC-TCPIP/Source/net_tmr.c \
            uC-TCPIP/Source/net_udp.c \
            uC-TCPIP/Source/net_util.c \
            \
            uC-TCPIP/IF/net_if.c \
            uC-TCPIP/IF/net_if_802x.c \
            uC-TCPIP/IF/net_if_ether.c \
            uC-TCPIP/IF/net_if_loopback.c \
            uC-TCPIP/IF/net_if_wifi.c \
            \
            uC-TCPIP/IP/IPv4/net_arp.c \
            uC-TCPIP/IP/IPv4/net_icmpv4.c \
            uC-TCPIP/IP/IPv4/net_igmp.c \
            uC-TCPIP/IP/IPv4/net_ipv4.c \
            \
            uC-TCPIP/IP/IPv6/net_dad.c \
            uC-TCPIP/IP/IPv6/net_icmpv6.c \
            uC-TCPIP/IP/IPv6/net_ipv6.c \
            uC-TCPIP/IP/IPv6/net_mldp.c \
            uC-TCPIP/IP/IPv6/net_ndp.c \
            \
            uC-TCPIP/Modules/Common/net_base64.c \
            uC-TCPIP/Modules/Common/net_sha1.c \
            \
            uC-TCPIP/Dev/Ether/GEM/net_dev_gem.c \
            uC-TCPIP/Dev/Ether/GEM/net_dev_gem64.c \
            uC-TCPIP/Dev/Ether/GEM/net_dev_gem64_32.c \
            uC-TCPIP/Dev/Ether/XIL_ETHER_LITE/net_dev_xil_ether_lite.c \
            uC-TCPIP/Dev/Ether/PHY/88E1111/net_phy_88e1111.c \
            uC-TCPIP/Dev/Ether/PHY/DP83867IR/net_phy_dp83867ir.c \
            uC-TCPIP/Dev/Ether/PHY/Generic/net_phy.c \

ifneq (${strip ${UCOS_TCPIP_EXP_SRC_BASE}},)
LIBSOURCES+= uC-TCPIP/Source/net_task.c
endif

            #uC-TCPIP/Dev/Ether/XIL_ETHER_LITE

BSPSOURCES= bsp/net_zynq_gem_bsp.c \
            bsp/net_xil_ether_lite_bsp.c \
            bsp/net_xil_ether_lite_dev_cfg.c \
            bsp/net_xil_ether_bsp.c \
            bsp/net_ultrascale_gem_bsp.c

INCLUDEFILES:=./net_cfg.h \
              ./bsp/net_zynq_gem_bsp.h \
              ./bsp/net_xil_ether_lite_bsp.h \
              ./bsp/net_xil_ether_lite_dev_cfg.h \
              ./bsp/net_xil_ether_bsp.h \
              ./bsp/net_ultrascale_gem_bsp.h

ifeq (${strip ${UCOS_TCPIP_SRC_BASE}},)
OBJECTS =
else
OBJECTS = $(addsuffix .o, $(basename ${LIBSOURCES}))
endif

BSPOBJECTS = $(addsuffix .o, $(basename ${BSPSOURCES}))

libs: banner lib_copy ucos_final_libs bsp clean

%.o: %.c
	${MKDIR} -p $(@D)
	${COMPILER} $(CC_FLAGS) $(ECC_FLAGS) $(INCLUDES) -o $@ $<

%.o: %.S
	${MKDIR} -p $(@D)
	${COMPILER} $(CC_FLAGS) $(ECC_FLAGS) $(INCLUDES) -o $@ $<

banner:
	echo "Compiling uC/TCP-IP"

lib_copy:
ifeq (${strip ${UCOS_TCPIP_SRC_BASE}},)
	echo "uC/TCP-IP sources not found, using pre-compiled library"
	${CP} ${UCOS_LIB_LOC}/${DIST_LIB} ${RELEASEDIR}/
	touch ${RELEASEDIR}/${DIST_LIB}
else
	echo "Building uC/TCP-IP from sources at ${UCOS_TCPIP_SRC_BASE}"
endif

${RELEASEDIR}/${DIST_LIB}: ${OBJECTS}
		$(ARCHIVER) -cr ${RELEASEDIR}/${DIST_LIB} ${OBJECTS}

ucos_final_libs: ${RELEASEDIR}/${DIST_LIB}
		rm -rf ${OBJECTS}
		$(ARCHIVER) -x ${RELEASEDIR}/${DIST_LIB}
		$(ARCHIVER) -r ${RELEASEDIR}/${LIB} *.o
		rm -rf *.o

bsp: ${BSPOBJECTS}
	$(ARCHIVER) -r ${RELEASEDIR}/${LIB} ${BSPOBJECTS}

.PHONY: include
include:
	${CP} ${INCLUDEFILES} ${INCLUDEDIR}
	${CP} ${UCOS_SRC_BASE}/uC-TCPIP/Source/net.h ${INCLUDEDIR}/Source
	${CP} ${UCOS_SRC_BASE}/uC-TCPIP/Source/net_app.h ${INCLUDEDIR}/Source
	${CP} ${UCOS_SRC_BASE}/uC-TCPIP/Source/net_ascii.h ${INCLUDEDIR}/Source
	${CP} ${UCOS_SRC_BASE}/uC-TCPIP/Source/net_bsd.h ${INCLUDEDIR}/Source
	${CP} ${UCOS_SRC_BASE}/uC-TCPIP/Source/net_buf.h ${INCLUDEDIR}/Source
	${CP} ${UCOS_SRC_BASE}/uC-TCPIP/Source/net_cache.h ${INCLUDEDIR}/Source
	${CP} ${UCOS_SRC_BASE}/uC-TCPIP/Source/net_cfg_net.h ${INCLUDEDIR}/Source
	${CP} ${UCOS_SRC_BASE}/uC-TCPIP/Source/net_conn.h ${INCLUDEDIR}/Source
	${CP} ${UCOS_SRC_BASE}/uC-TCPIP/Source/net_ctr.h ${INCLUDEDIR}/Source
	${CP} ${UCOS_SRC_BASE}/uC-TCPIP/Source/net_def.h ${INCLUDEDIR}/Source
	${CP} ${UCOS_SRC_BASE}/uC-TCPIP/Source/net_dict.h ${INCLUDEDIR}/Source
	${CP} ${UCOS_SRC_BASE}/uC-TCPIP/Source/net_err.h ${INCLUDEDIR}/Source
	${CP} ${UCOS_SRC_BASE}/uC-TCPIP/Source/net_icmp.h ${INCLUDEDIR}/Source
	${CP} ${UCOS_SRC_BASE}/uC-TCPIP/Source/net_ip.h ${INCLUDEDIR}/Source
	${CP} ${UCOS_SRC_BASE}/uC-TCPIP/Source/net_mgr.h ${INCLUDEDIR}/Source
	${CP} ${UCOS_SRC_BASE}/uC-TCPIP/Source/net_sock.h ${INCLUDEDIR}/Source
	${CP} ${UCOS_SRC_BASE}/uC-TCPIP/Source/net_stat.h ${INCLUDEDIR}/Source
	${CP} ${UCOS_SRC_BASE}/uC-TCPIP/Source/net_tcp.h ${INCLUDEDIR}/Source
	${CP} ${UCOS_SRC_BASE}/uC-TCPIP/Source/net_tmr.h ${INCLUDEDIR}/Source
	${CP} ${UCOS_SRC_BASE}/uC-TCPIP/Source/net_type.h ${INCLUDEDIR}/Source
	${CP} ${UCOS_SRC_BASE}/uC-TCPIP/Source/net_udp.h ${INCLUDEDIR}/Source
	${CP} ${UCOS_SRC_BASE}/uC-TCPIP/Source/net_util.h ${INCLUDEDIR}/Source
ifneq (${strip ${UCOS_TCPIP_EXP_SRC_BASE}},)
	${CP} ${UCOS_SRC_BASE}/uC-TCPIP/Source/net_task.h ${INCLUDEDIR}/Source
endif

	${CP} ${UCOS_SRC_BASE}/uC-TCPIP/IF/net_if.h ${INCLUDEDIR}/IF
	${CP} ${UCOS_SRC_BASE}/uC-TCPIP/IF/net_if_802x.h ${INCLUDEDIR}/IF
	${CP} ${UCOS_SRC_BASE}/uC-TCPIP/IF/net_if_ether.h ${INCLUDEDIR}/IF
	${CP} ${UCOS_SRC_BASE}/uC-TCPIP/IF/net_if_loopback.h ${INCLUDEDIR}/IF
	${CP} ${UCOS_SRC_BASE}/uC-TCPIP/IF/net_if_wifi.h ${INCLUDEDIR}/IF

	${CP} ${UCOS_SRC_BASE}/uC-TCPIP/IP/IPv4/net_arp.h ${INCLUDEDIR}/IP/IPv4
	${CP} ${UCOS_SRC_BASE}/uC-TCPIP/IP/IPv4/net_icmpv4.h ${INCLUDEDIR}/IP/IPv4
	${CP} ${UCOS_SRC_BASE}/uC-TCPIP/IP/IPv4/net_igmp.h ${INCLUDEDIR}/IP/IPv4
	${CP} ${UCOS_SRC_BASE}/uC-TCPIP/IP/IPv4/net_ipv4.h ${INCLUDEDIR}/IP/IPv4

	${CP} ${UCOS_SRC_BASE}/uC-TCPIP/IP/IPv6/net_dad.h ${INCLUDEDIR}/IP/IPv6
	${CP} ${UCOS_SRC_BASE}/uC-TCPIP/IP/IPv6/net_icmpv6.h ${INCLUDEDIR}/IP/IPv6
	${CP} ${UCOS_SRC_BASE}/uC-TCPIP/IP/IPv6/net_ipv6.h ${INCLUDEDIR}/IP/IPv6
	${CP} ${UCOS_SRC_BASE}/uC-TCPIP/IP/IPv6/net_mldp.h ${INCLUDEDIR}/IP/IPv6
	${CP} ${UCOS_SRC_BASE}/uC-TCPIP/IP/IPv6/net_ndp.h ${INCLUDEDIR}/IP/IPv6

	${CP} ${UCOS_SRC_BASE}/uC-TCPIP/Modules/Common/net_base64.h ${INCLUDEDIR}/Modules/Common
	${CP} ${UCOS_SRC_BASE}/uC-TCPIP/Modules/Common/net_sha1.h ${INCLUDEDIR}/Modules/Common

	${CP} ${UCOS_SRC_BASE}/uC-TCPIP/FS/net_fs.h ${INCLUDEDIR}/FS

	${CP} ${UCOS_SRC_BASE}/uC-TCPIP/Dev/Ether/GEM/net_dev_gem.h ${INCLUDEDIR}/Dev/Ether/GEM/
	${CP} ${UCOS_SRC_BASE}/uC-TCPIP/Dev/Ether/XIL_ETHER_LITE/net_dev_xil_ether_lite.h ${INCLUDEDIR}/Dev/Ether/XIL_ETHER_LITE/
	${CP} ${UCOS_SRC_BASE}/uC-TCPIP/Dev/Ether/PHY/88E1111/net_phy_88e1111.h ${INCLUDEDIR}/Dev/Ether/PHY/88E1111
	${CP} ${UCOS_SRC_BASE}/uC-TCPIP/Dev/Ether/PHY/DP83867IR/net_phy_dp83867ir.h ${INCLUDEDIR}/Dev/Ether/PHY/DP83867IR
	${CP} ${UCOS_SRC_BASE}/uC-TCPIP/Dev/Ether/PHY/Generic/net_phy.h ${INCLUDEDIR}/Dev/Ether/PHY/Generic

clean:
	rm -rf ${OBJECTS}

