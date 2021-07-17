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
DIST_LIB=libucfs-${UCOS_OS}-${UCOS_ARCH}.a

LIBSOURCES:=uC-FS/Source/fs.c \
            uC-FS/Source/fs_api.c \
            uC-FS/Source/fs_buf.c \
            uC-FS/Source/fs_cache.c \
            uC-FS/Source/fs_dev.c \
            uC-FS/Source/fs_dir.c \
            uC-FS/Source/fs_entry.c \
            uC-FS/Source/fs_file.c \
            uC-FS/Source/fs_partition.c \
            uC-FS/Source/fs_sys.c \
            uC-FS/Source/fs_unicode.c \
            uC-FS/Source/fs_util.c \
            uC-FS/Source/fs_vol.c \
            uC-FS/FAT/fs_fat.c \
            uC-FS/FAT/fs_fat_dir.c \
            uC-FS/FAT/fs_fat_entry.c \
            uC-FS/FAT/fs_fat_fat12.c \
            uC-FS/FAT/fs_fat_fat16.c \
            uC-FS/FAT/fs_fat_fat32.c \
            uC-FS/FAT/fs_fat_file.c \
            uC-FS/FAT/fs_fat_journal.c \
            uC-FS/FAT/fs_fat_lfn.c \
            uC-FS/FAT/fs_fat_sfn.c \
            uC-FS/Dev/RAMDisk/fs_dev_ramdisk.c \
            uC-FS/Dev/NAND/fs_dev_nand.c \
            uC-FS/Dev/NAND/Ctrlr/fs_dev_nand_ctrlr_gen.c \
            uC-FS/Dev/NAND/Ctrlr/GenExt/fs_dev_nand_ctrlr_gen_micron_ecc.c \
            uC-FS/Dev/NAND/Ctrlr/GenExt/fs_dev_nand_ctrlr_gen_soft_ecc.c \
            uC-FS/Dev/NAND/Part/fs_dev_nand_part_onfi.c \
            uC-FS/Dev/NAND/Part/fs_dev_nand_part_static.c \
            uC-FS/Dev/SD/fs_dev_sd.c \
            uC-FS/Dev/SD/Card/fs_dev_sd_card.c \
            uC-FS/Dev/NOR/fs_dev_nor.c \
            uC-FS/OS/${UCOS_DIR_NAME}/fs_os.c

ifdef UCOS_USBH
LIBSOURCES+= uC-FS/Dev/MSC/fs_dev_msc.c
endif

BSPSOURCES:=bsp/fs_dev_sd_card_bsp.c


INCLUDEFILES:=fs_cfg.h \
              fs_dev_nand_cfg.h


ifeq (${strip ${UCOS_FS_SRC_BASE}},)
OBJECTS =
else
OBJECTS = $(addsuffix .o, $(basename ${LIBSOURCES}))
endif

BSPOBJECTS = $(addsuffix .o, $(basename ${BSPSOURCES}))

libs: banner lib_copy ucos_final_libs bsp clean

%.o: %.c
	${COMPILER} $(CC_FLAGS) $(ECC_FLAGS) $(INCLUDES) -o $@ $<

%.o: %.S
	${COMPILER} $(CC_FLAGS) $(ECC_FLAGS) $(INCLUDES) -o $@ $<

banner:
	echo "Compiling uC/FS"

lib_copy:
ifeq (${strip ${UCOS_FS_SRC_BASE}},)
	echo "uC/FS sources not found, using pre-compiled library"
	${CP} ${UCOS_LIB_LOC}/${DIST_LIB} ${RELEASEDIR}/
	touch ${RELEASEDIR}/${DIST_LIB}
else
	echo "Building uC/FS from sources at ${UCOS_FS_SRC_BASE}"
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
	${CP} ${UCOS_SRC_BASE}/uC-FS/Source/fs.h ${INCLUDEDIR}/Source
	${CP} ${UCOS_SRC_BASE}/uC-FS/Source/fs_api.h ${INCLUDEDIR}/Source
	${CP} ${UCOS_SRC_BASE}/uC-FS/Source/fs_buf.h ${INCLUDEDIR}/Source
	${CP} ${UCOS_SRC_BASE}/uC-FS/Source/fs_cache.h ${INCLUDEDIR}/Source
	${CP} ${UCOS_SRC_BASE}/uC-FS/Source/fs_cfg_fs.h ${INCLUDEDIR}/Source
	${CP} ${UCOS_SRC_BASE}/uC-FS/Source/fs_ctr.h ${INCLUDEDIR}/Source
	${CP} ${UCOS_SRC_BASE}/uC-FS/Source/fs_def.h ${INCLUDEDIR}/Source
	${CP} ${UCOS_SRC_BASE}/uC-FS/Source/fs_dev.h ${INCLUDEDIR}/Source
	${CP} ${UCOS_SRC_BASE}/uC-FS/Source/fs_dir.h ${INCLUDEDIR}/Source
	${CP} ${UCOS_SRC_BASE}/uC-FS/Source/fs_entry.h ${INCLUDEDIR}/Source
	${CP} ${UCOS_SRC_BASE}/uC-FS/Source/fs_err.h ${INCLUDEDIR}/Source
	${CP} ${UCOS_SRC_BASE}/uC-FS/Source/fs_file.h ${INCLUDEDIR}/Source
	${CP} ${UCOS_SRC_BASE}/uC-FS/Source/fs_partition.h ${INCLUDEDIR}/Source
	${CP} ${UCOS_SRC_BASE}/uC-FS/Source/fs_sys.h ${INCLUDEDIR}/Source
	${CP} ${UCOS_SRC_BASE}/uC-FS/Source/fs_type.h ${INCLUDEDIR}/Source
	${CP} ${UCOS_SRC_BASE}/uC-FS/Source/fs_unicode.h ${INCLUDEDIR}/Source
	${CP} ${UCOS_SRC_BASE}/uC-FS/Source/fs_util.h ${INCLUDEDIR}/Source
	${CP} ${UCOS_SRC_BASE}/uC-FS/Source/fs_vol.h ${INCLUDEDIR}/Source
	${CP} ${UCOS_SRC_BASE}/uC-FS/Source/fs.h ${INCLUDEDIR}/Source
	${CP} ${UCOS_SRC_BASE}/uC-FS/Source/fs_dev.h ${INCLUDEDIR}/Source

	${CP} ${UCOS_SRC_BASE}/uC-FS/FAT/fs_fat.h ${INCLUDEDIR}/FAT
	${CP} ${UCOS_SRC_BASE}/uC-FS/FAT/fs_fat_dir.h ${INCLUDEDIR}/FAT
	${CP} ${UCOS_SRC_BASE}/uC-FS/FAT/fs_fat_entry.h ${INCLUDEDIR}/FAT
	${CP} ${UCOS_SRC_BASE}/uC-FS/FAT/fs_fat_fat12.h ${INCLUDEDIR}/FAT
	${CP} ${UCOS_SRC_BASE}/uC-FS/FAT/fs_fat_fat16.h ${INCLUDEDIR}/FAT
	${CP} ${UCOS_SRC_BASE}/uC-FS/FAT/fs_fat_fat32.h ${INCLUDEDIR}/FAT
	${CP} ${UCOS_SRC_BASE}/uC-FS/FAT/fs_fat_file.h ${INCLUDEDIR}/FAT
	${CP} ${UCOS_SRC_BASE}/uC-FS/FAT/fs_fat_journal.h ${INCLUDEDIR}/FAT
	${CP} ${UCOS_SRC_BASE}/uC-FS/FAT/fs_fat_lfn.h ${INCLUDEDIR}/FAT/FAT
	${CP} ${UCOS_SRC_BASE}/uC-FS/FAT/fs_fat_sfn.h ${INCLUDEDIR}/FAT
	${CP} ${UCOS_SRC_BASE}/uC-FS/FAT/fs_fat_type.h ${INCLUDEDIR}/FAT

	${CP} ${UCOS_SRC_BASE}/uC-FS/Dev/RAMDisk/fs_dev_ramdisk.h ${INCLUDEDIR}/Dev/RAMDisk

	${CP} ${UCOS_SRC_BASE}/uC-FS/Dev/NAND/fs_dev_nand.h ${INCLUDEDIR}/Dev/NAND
	${CP} ${UCOS_SRC_BASE}/uC-FS/Dev/NAND/Ctrlr/fs_dev_nand_ctrlr_gen.h ${INCLUDEDIR}/Dev/NAND/Ctrlr
	${CP} ${UCOS_SRC_BASE}/uC-FS/Dev/NAND/Ctrlr/GenExt/fs_dev_nand_ctrlr_gen_micron_ecc.h ${INCLUDEDIR}/Dev/NAND/Ctrlr/GenExt
	${CP} ${UCOS_SRC_BASE}/uC-FS/Dev/NAND/Ctrlr/GenExt/fs_dev_nand_ctrlr_gen_soft_ecc.h ${INCLUDEDIR}/Dev/NAND/Ctrlr/GenExt
	${CP} ${UCOS_SRC_BASE}/uC-FS/Dev/NAND/Part/fs_dev_nand_part_onfi.h ${INCLUDEDIR}/Dev/NAND/Part
	${CP} ${UCOS_SRC_BASE}/uC-FS/Dev/NAND/Part/fs_dev_nand_part_static.h ${INCLUDEDIR}/Dev/NAND/Part

	${CP} ${UCOS_SRC_BASE}/uC-FS/Dev/SD/fs_dev_sd.h ${INCLUDEDIR}/Dev/SD
	${CP} ${UCOS_SRC_BASE}/uC-FS/Dev/SD/Card/fs_dev_sd_card.h ${INCLUDEDIR}/Dev/SD/Card

	${CP} ${UCOS_SRC_BASE}/uC-FS/Dev/NOR/fs_dev_nor.h ${INCLUDEDIR}/Dev/NOR

	${CP} ${UCOS_SRC_BASE}/uC-FS/Dev/MSC/fs_dev_msc.h ${INCLUDEDIR}/Dev/MSC

	${CP} ${UCOS_SRC_BASE}/uC-FS/OS/${UCOS_DIR_NAME}/fs_os.h ${INCLUDEDIR}/OS

ifdef UCOS_USBH_SRC_BASE
	${CP} ${UCOS_SRC_BASE}/uC-FS/Dev/MSC/fs_dev_msc.h ${INCLUDEDIR}/Dev/MSC/
endif

#${CP} uC-FS/OS/uCOS-III/fs_os.h ${INCLUDEDIR}/

clean:
	rm -rf ${OBJECTS}

