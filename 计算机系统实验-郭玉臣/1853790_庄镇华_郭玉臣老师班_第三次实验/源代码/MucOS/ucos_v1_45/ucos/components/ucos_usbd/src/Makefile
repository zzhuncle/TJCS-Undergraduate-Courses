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
INCLUDES=-I./. -I${INCLUDEDIR}

OUTS = *.o

include ../../../include/Makefile_def.inc
include Makefile_def_usbd.inc

DIST_LIB=libucusbd-${UCOS_OS}-${UCOS_ARCH}.a

LIBSOURCES:=uC-USB-Device/Source/usbd_core.c \
            uC-USB-Device/Source/usbd_ep.c \
            uC-USB-Device/Class/CDC/usbd_cdc.c \
            uC-USB-Device/Class/CDC/ACM/usbd_acm_serial.c \
            uC-USB-Device/Class/CDC-EEM/usbd_cdc_eem.c \
            uC-USB-Device/Class/HID/usbd_hid.c \
            uC-USB-Device/Class/HID/usbd_hid_report.c \
            uC-USB-Device/Class/HID/OS/${UCOS_DIR_NAME}/usbd_hid_os.c \
            uC-USB-Device/Class/Vendor/usbd_vendor.c \
            uC-USB-Device/Class/MSC/usbd_msc.c \
            uC-USB-Device/Class/MSC/usbd_scsi.c \
            uC-USB-Device/Class/MSC/OS/${UCOS_DIR_NAME}/usbd_msc_os.c \
            uC-USB-Device/OS/${UCOS_DIR_NAME}/usbd_os.c \
            uC-USB-Device/Drivers/Synopsys_OTG_HS/usbd_drv_synopsys_otg_hs.c

ifeq (${strip ${UCOS_USBD_STORAGE_DRV}},RAMDISK)
LIBSOURCES+=uC-USB-Device/Class/MSC/Storage/RAMDisk/usbd_storage.c
else
LIBSOURCES+=uC-USB-Device/Class/MSC/Storage/uC-FS/V4/usbd_storage.c
endif

BSPSOURCES=bsp/usbd_bsp_ps7_usb.c \
           bsp/usbd_dev_cfg.c

INCLUDEFILES:=usbd_cfg.h \
              bsp/usbd_bsp_ps7_usb.h \
              bsp/usbd_dev_cfg.h \
              usbd_storage.h \
              app_usbd_cfg.h


ifeq (${strip ${UCOS_USBD_SRC_BASE}},)
OBJECTS =
else
OBJECTS =	$(addsuffix .o, $(basename ${LIBSOURCES}))
endif

BSPOBJECTS = $(addsuffix .o, $(basename ${BSPSOURCES}))

libs: banner lib_copy ucos_final_libs bsp clean

%.o: %.c
	${COMPILER} $(CC_FLAGS) $(ECC_FLAGS) $(INCLUDES) -o $@ $<

%.o: %.S
	${COMPILER} $(CC_FLAGS) $(ECC_FLAGS) $(INCLUDES) -o $@ $<

banner:
	echo "Compiling uC/USB-Device"

lib_copy:
ifeq (${strip ${UCOS_USBD_SRC_BASE}},)
	echo "uC/USB-Device sources not found, using pre-compiled library"
	${CP} ${UCOS_LIB_LOC}/${DIST_LIB} ${RELEASEDIR}/
	touch ${RELEASEDIR}/${DIST_LIB}
else
	echo "Building uC/USB-Device from sources at ${UCOS_USBD_SRC_BASE}"
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
	${CP} uC-USB-Device/Source/usbd_core.h ${INCLUDEDIR}/Source
	${CP} uC-USB-Device/Source/usbd_internal.h ${INCLUDEDIR}/Source
	${CP} uC-USB-Device/Class/CDC/usbd_cdc.h ${INCLUDEDIR}/Class/CDC
	${CP} uC-USB-Device/Class/CDC/ACM/usbd_acm_serial.h ${INCLUDEDIR}/Class/CDC/ACM
	${CP} uC-USB-Device/Class/CDC-EEM/usbd_cdc_eem.h ${INCLUDEDIR}/Class/CDC-EEM
	${CP} uC-USB-Device/Class/HID/usbd_hid.h ${INCLUDEDIR}/Class/HID
	${CP} uC-USB-Device/Class/HID/usbd_hid_report.h ${INCLUDEDIR}/Class/HID
	${CP} uC-USB-Device/Class/HID/usbd_hid_os.h ${INCLUDEDIR}/Class/HID
	${CP} uC-USB-Device/Class/Vendor/usbd_vendor.h ${INCLUDEDIR}/Class/Vendor
	${CP} uC-USB-Device/Class/MSC/usbd_scsi.h ${INCLUDEDIR}/Class/MSC
	${CP} uC-USB-Device/Class/MSC/usbd_msc.h ${INCLUDEDIR}/Class/MSC
	${CP} uC-USB-Device/Class/MSC/usbd_msc_os.h ${INCLUDEDIR}/Class/MSC
	${CP} uC-USB-Device/Class/MSC/Storage/RAMDisk/usbd_storage.h ${INCLUDEDIR}/Class/MSC/Storage/RAMDisk
	${CP} uC-USB-Device/Class/MSC/Storage/uC-FS/V4/usbd_storage.h ${INCLUDEDIR}/Class/MSC/Storage/uC-FS/V4
	${CP} uC-USB-Device/Drivers/synopsys_otg_hs/usbd_drv_synopsys_otg_hs.h ${INCLUDEDIR}/Drivers/synopsys_otg_hs

clean:
	rm -rf ${OBJECTS}

