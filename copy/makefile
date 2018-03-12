#*******************************************************************************
#* FILE PURPOSE: Top level makefile for Creating Component Libraries and example 
#*               binaries
#*******************************************************************************
#* FILE NAME: makefile
#*
#* DESCRIPTION: Defines Compiler tools paths, libraries , Build Options 
#*
#*
#*******************************************************************************
#*
# (Mandatory) Specify where various tools are installed.

include Rules.make

ifndef MAKE
export MAKE = make
endif

ifndef ECHO
export ECHO = echo
endif

ifndef RM
export RM = rm -f
endif

ifndef CP
export CP = cp
endif

all: csl profiling board emac fatfs gpio gpmc i2c icss-emac mcasp mmcsd nimu nimu-icss osal pm spi starterware uart pruss 
clean: csl_clean profiling_clean board_clean emac_clean fatfs_clean gpio_clean gpmc_clean i2c_clean icss-emac_clean mcasp_clean mmcsd_clean nimu_clean nimu-icss_clean osal_clean pm_clean spi_clean starterware_clean uart_clean pruss_clean 

csl: 
	$(MAKE) -C ./ti/csl all

csl_clean:
	$(MAKE) -C ./ti/csl clean

profiling:
	$(MAKE) -C ./ti/utils/profiling all

profiling_clean:
	$(MAKE) -C ./ti/utils/profiling clean

board: board_lib board_diag

diag: board_diag

board_lib:
	$(MAKE) -C ./ti/board all

BOARD_DIAG_DEPS = csl profiling fatfs gpio i2c mmcsd spi uart osal
ALL_BOARDS = evmAM335x bbbAM335x
board_diag: board_lib $(BOARD_DIAG_DEPS)
	$(MAKE) -C ./ti/board/diag ALL_BOARDS="$(ALL_BOARDS)"

board_clean: board_diag_clean board_lib_clean

board_lib_clean:
	$(MAKE) -C ./ti/board clean

board_diag_clean:
	$(MAKE) -C ./ti/board/diag clean ALL_BOARDS="$(ALL_BOARDS)"

emac:
	$(MAKE) -C ./ti/drv/emac all

emac_clean:
	$(MAKE) -C ./ti/drv/emac clean

fatfs:
	$(MAKE) -C ./ti/fs/fatfs all

fatfs_clean:
	$(MAKE) -C ./ti/fs/fatfs clean

gpio:
	$(MAKE) -C ./ti/drv/gpio all

gpio_clean:
	$(MAKE) -C ./ti/drv/gpio clean

gpmc:
	$(MAKE) -C ./ti/drv/gpmc all

gpmc_clean:
	$(MAKE) -C ./ti/drv/gpmc clean

i2c:
	${MAKE} -C ./ti/drv/i2c all

i2c_clean:
	${MAKE} -C ./ti/drv/i2c clean

icss-emac:
	$(MAKE) -C ./ti/drv/icss_emac all

icss-emac_clean:
	$(MAKE) -C ./ti/drv/icss_emac clean

mcasp:
	$(MAKE) -C ./ti/drv/mcasp all

mcasp_clean:
	$(MAKE) -C ./ti/drv/mcasp clean

mmcsd:
	$(MAKE) -C ./ti/drv/mmcsd all

mmcsd_clean:
	$(MAKE) -C ./ti/drv/mmcsd clean

nimu:
	$(MAKE) -C ./ti/transport/ndk/nimu all

nimu_clean:
	$(MAKE) -C ./ti/transport/ndk/nimu clean

nimu-icss:
	$(MAKE) -C ./ti/transport/ndk/nimu_icss all

nimu-icss_clean:
	$(MAKE) -C ./ti/transport/ndk/nimu_icss clean

osal:
	$(MAKE) -C ./ti/osal all DISABLE_RECURSE_DEPS="yes"

osal_clean:
	$(MAKE) -C ./ti/osal clean

pm:
	$(MAKE) -C ./ti/drv/pm all DISABLE_RECURSE_DEPS="yes"

pm_clean:
	$(MAKE) -C ./ti/drv/pm clean

spi:
	$(MAKE) -C ./ti/drv/spi all

spi_clean:
	$(MAKE) -C ./ti/drv/spi clean

# Linux default build environment
ifeq ($(OS), )
    OS := linux
endif

ifeq ($(OS),Windows_NT)
    # Object clean on Windows
    CLEAN_RECURSIVE_OBJ=cmd /C del /s ti\\starterware\\*.o ti\\starterware\\*.a > NUL
else
    # Object clean on Linux
    CLEAN_RECURSIVE_OBJ=find ti/starterware -name "*.[ao]" -delete
endif

STARTERWARE_PLATFORM = am335x-evm

starterware: starterware_boot starterware_libs starterware_apps

starterware_boot:
	$(CLEAN_RECURSIVE_OBJ)
	$(MAKE) -C ./ti/starterware bootloader BUILDCFG=boot BOOTMODE=uart PLATFORM=$(STARTERWARE_PLATFORM) PROFILE=debug -s KW_BUILD=no
	$(CLEAN_RECURSIVE_OBJ)
	$(MAKE) -C ./ti/starterware bootloader BUILDCFG=boot BOOTMODE=uart PLATFORM=$(STARTERWARE_PLATFORM) PROFILE=release -s KW_BUILD=no
	$(CLEAN_RECURSIVE_OBJ)
	$(MAKE) -C ./ti/starterware bootloader BUILDCFG=boot BOOTMODE=mmcsd PLATFORM=$(STARTERWARE_PLATFORM) PROFILE=debug -s KW_BUILD=no
	$(CLEAN_RECURSIVE_OBJ)
	$(MAKE) -C ./ti/starterware bootloader BUILDCFG=boot BOOTMODE=mmcsd PLATFORM=$(STARTERWARE_PLATFORM) PROFILE=release -s KW_BUILD=no
	$(CLEAN_RECURSIVE_OBJ)
	$(MAKE) -C ./ti/starterware bootloader BUILDCFG=boot BOOTMODE=nand PLATFORM=$(STARTERWARE_PLATFORM) PROFILE=debug -s KW_BUILD=no
	$(CLEAN_RECURSIVE_OBJ)
	$(MAKE) -C ./ti/starterware bootloader BUILDCFG=boot BOOTMODE=nand PLATFORM=$(STARTERWARE_PLATFORM) PROFILE=release -s KW_BUILD=no
	$(CLEAN_RECURSIVE_OBJ)
ifeq ($(STARTERWARE_PLATFORM), am335x-evm)
	# am335x-evm
	$(MAKE) -C ./ti/starterware bootloader BUILDCFG=boot BOOTMODE=mcspi PLATFORM=$(STARTERWARE_PLATFORM) PROFILE=debug -s KW_BUILD=no
	$(CLEAN_RECURSIVE_OBJ)
	$(MAKE) -C ./ti/starterware bootloader BUILDCFG=boot BOOTMODE=mcspi PLATFORM=$(STARTERWARE_PLATFORM) PROFILE=release -s KW_BUILD=no
else ifeq ($(STARTERWARE_PLATFORM), am43xx-evm)
	# am437x-evm
	$(MAKE) -C ./ti/starterware bootloader BUILDCFG=boot BOOTMODE=qspi PLATFORM=$(STARTERWARE_PLATFORM) PROFILE=debug -s KW_BUILD=no
	$(CLEAN_RECURSIVE_OBJ)
	$(MAKE) -C ./ti/starterware bootloader BUILDCFG=boot BOOTMODE=qspi PLATFORM=$(STARTERWARE_PLATFORM) PROFILE=release -s KW_BUILD=no
endif

starterware_libs:
	$(MAKE) -C ./ti/starterware PROFILE=debug PLATFORM=$(STARTERWARE_PLATFORM) -s KW_BUILD=no
	$(MAKE) -C ./ti/starterware PROFILE=release PLATFORM=$(STARTERWARE_PLATFORM) -s KW_BUILD=no

starterware_apps:
ifeq ($(STARTERWARE_PLATFORM), am335x-evm)
	# am335x-evm
	$(MAKE) -C ./ti/starterware lcdc_app_raster PROFILE=debug PLATFORM=$(STARTERWARE_PLATFORM) -s KW_BUILD=no
	$(MAKE) -C ./ti/starterware lcdc_app_raster PROFILE=release PLATFORM=$(STARTERWARE_PLATFORM) -s KW_BUILD=no
	$(MAKE) -C ./ti/starterware mcspi_app_flash PROFILE=debug PLATFORM=$(STARTERWARE_PLATFORM) -s KW_BUILD=no
	$(MAKE) -C ./ti/starterware mcspi_app_flash PROFILE=release PLATFORM=$(STARTERWARE_PLATFORM) -s KW_BUILD=no
else ifeq ($(STARTERWARE_PLATFORM), am43xx-evm)
	# am437x-evm
	$(MAKE) -C ./ti/starterware vpfe_app_capture_display PROFILE=debug PLATFORM=$(STARTERWARE_PLATFORM) -s KW_BUILD=no
	$(MAKE) -C ./ti/starterware vpfe_app_capture_display PROFILE=release PLATFORM=$(STARTERWARE_PLATFORM) -s KW_BUILD=no
	$(MAKE) -C ./ti/starterware cap_tsc_app PROFILE=debug PLATFORM=$(STARTERWARE_PLATFORM) -s KW_BUILD=no
	$(MAKE) -C ./ti/starterware cap_tsc_app PROFILE=release PLATFORM=$(STARTERWARE_PLATFORM) -s KW_BUILD=no
	$(MAKE) -C ./ti/starterware dss_app_raster PROFILE=debug PLATFORM=$(STARTERWARE_PLATFORM) -s KW_BUILD=no
	$(MAKE) -C ./ti/starterware dss_app_raster PROFILE=release PLATFORM=$(STARTERWARE_PLATFORM) -s KW_BUILD=no
	$(MAKE) -C ./ti/starterware qspi_app_flash_writer PROFILE=debug PLATFORM=$(STARTERWARE_PLATFORM) -s KW_BUILD=no
	$(MAKE) -C ./ti/starterware qspi_app_flash_writer PROFILE=release PLATFORM=$(STARTERWARE_PLATFORM) -s KW_BUILD=no
	$(MAKE) -C ./ti/starterware qspi_app_read_write PROFILE=debug PLATFORM=$(STARTERWARE_PLATFORM) -s KW_BUILD=no
	$(MAKE) -C ./ti/starterware qspi_app_read_write PROFILE=release PLATFORM=$(STARTERWARE_PLATFORM) -s KW_BUILD=no
endif

starterware_clean:
	$(MAKE) -C ./ti/starterware clean PLATFORM=$(STARTERWARE_PLATFORM) KW_BUILD=no

uart:
	$(MAKE) -C ./ti/drv/uart all

uart_clean:
	$(MAKE) -C ./ti/drv/uart clean

pruss:
	$(MAKE) -C ./ti/drv/pruss all

pruss_clean:
	$(MAKE) -C ./ti/drv/pruss clean
