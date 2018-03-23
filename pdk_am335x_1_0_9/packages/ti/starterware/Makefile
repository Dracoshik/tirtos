# Filename: Makefile
#
# This file is the makefile for building this module.
#
# Copyright (C) 2013 Texas Instruments Incorporated - http://www.ti.com/
#
#
#  Redistribution and use in source and binary forms, with or without
#  modification, are permitted provided that the following conditions
#  are met:
#
#    Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
#
#    Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the
#    distribution.
#
#    Neither the name of Texas Instruments Incorporated nor the names of
#    its contributors may be used to endorse or promote products derived
#    from this software without specific prior written permission.
#
#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
#  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
#  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
#  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
#  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
#  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
#  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
#  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
#  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
#  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
#  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#******************************************************************************


#
# StarterWare
#
include Rules.make

CLEANALL_TARGETS = $(addsuffix _clean, $(starterware_PKG_LIST_ALL))
CLEANALL_EXAMPLES = $(addsuffix _clean, $(starterware_EXAMPLES_LIST))


.PHONY : all
all: version libs examples kw_check
#all: version libs kw_check

.PHONY : libs
libs: $(starterware_PKG_LIST_ALL)

.PHONY : examples
examples: $(starterware_EXAMPLES_LIST)

.PHONY : clean libs_clean examples_clean
clean: libs_clean examples_clean kw_invoke
#clean: libs_clean kw_invoke
libs_clean: $(CLEANALL_TARGETS)
examples_clean: $(CLEANALL_EXAMPLES)

.PHONY : kw_check kw_invoke kw_clean
kw_clean:
ifneq ($(KW_BUILD),no)
	$(RM) -rf $(DEST_ROOT)/.kwlp $(DEST_ROOT)/.kwps
endif

kw_check:
ifneq ($(KW_BUILD),no)
	$(KW_PATH)/kwcheck run -pd "$(DEST_ROOT)/.kwlp"
	$(KW_PATH)/kwcheck list -pd "$(DEST_ROOT)/.kwlp" --report "Klocwork_issues_all.txt"
	exit
endif

kw_invoke: kw_clean
ifneq ($(KW_BUILD),no)
	$(KW_PATH)/kwcheck create -pd "$(DEST_ROOT)/.kwlp" -sd "$(DEST_ROOT)/.kwps"
	$(KW_PATH)/kwshell -pn "$(DEST_ROOT)/.kwlp"
endif

.PHONY : version
version:
	$(ECHO) ------------------------------------------------------
	$(ECHO) \# Starterware 02.01.00.01
	$(ECHO) ------------------------------------------------------

.PHONY : help
help:
	$(ECHO) ------------------------------------------------------
	$(ECHO) \# Starterware make help
	$(ECHO) ------------------------------------------------------
	$(ECHO) make -s [OPTIONAL MAKE VARIABLES]
	$(ECHO)
	$(ECHO) "Supported targets:"
	$(ECHO) "all: Default - Build all libraries and examples"
	$(ECHO) "clean: Clean all libraries and examples"
	$(ECHO) "libs/libs_clean: Build/clean all libraries"
	$(ECHO) "examples / examples_clean: Build/clean all examples"
	$(ECHO) "<mod_name> / <mod_name>_clean: Build/clean <mod_name> module"
	$(ECHO) "    For example: soc / soc_clean"
	$(ECHO) "<mod_name>_app_<use_case> / <mod_name>_app_<use_case>_clean: Build/clean <mod_name> module"
	$(ECHO) "    For example: uart_app_echo / uart_app_echo_clean"
	$(ECHO) ""
	$(ECHO) "Optional make variables:"
	$(ECHO) "PLATFORM=[am335x-evm / am43xx-evm]"
	$(ECHO) "    Default: am43xx-evm"
	$(ECHO) "SOC=[<SOC name>] Options depend on chosen PLATFORM"
	$(ECHO) "    For am335x-evm: [am335x]"
	$(ECHO) "    For am43xx-evm: [am437x]"
	$(ECHO) "    Default: am437x"
	$(ECHO) "PROFILE=[debug / release / prod_release]"
	$(ECHO) "    Default: debug"
	$(ECHO) "CONSOLE=[SEMIHOSTING / UART]"
	$(ECHO) "    Default: UART"
	$(ECHO) "BUILDCFG=[all / boot]"
	$(ECHO) "    Default: all"
	$(ECHO) "BOOTMODE=[mmcsd / uart / qspi / nand / mcspi]"
	$(ECHO) "    Default: mmcsd"
	$(ECHO) "KW_BUILD=[yes / no]"
	$(ECHO) "    Default: no"


#=================================================================
#
# Rules to build all modules and examples
#
#=================================================================
# Library targets
.PHONY : dal soc board device utils mmcsd_lib ff9b_lib xmodem_lib qspi_lib nand_lib
dal soc board device utils mmcsd_lib ff9b_lib xmodem_lib qspi_lib nand_lib:
	$(MAKE) -C $($@_PATH)

.PHONY : dal_clean soc_clean board_clean device_clean utils_clean mmcsd_lib_clean ff9b_lib_clean xmodem_lib_clean qspi_lib_clean nand_lib_clean
dal_clean soc_clean board_clean device_clean utils_clean mmcsd_lib_clean ff9b_lib_clean xmodem_lib_clean qspi_lib_clean nand_lib_clean:
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean

# Bootloader targets
.PHONY : bootloader bootloader_kw bootloader_clean bootloader_kw_clean bootloader_obj_clean
bootloader_kw: bootloader kw_check
bootloader:
	$(MAKE) -C $($@_PATH)
bootloader_kw_clean: bootloader_clean kw_invoke
bootloader_clean: kw_invoke
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean
bootloader_obj_clean: kw_invoke
	$(MAKE) -C $($(subst _obj_clean,,$@)_PATH) obj_clean

# Example targets
.PHONY : example_utils test_app uart_app_echo uart_app_console cache_mmu_app gpio_app_led_blink gpio_app_buzzer dmtimer_app_cdt i2c_app_eeprom_read epwm_app_haptics_motor qspi_app_read_write wdt_app_cpu_reset qspi_app_flash_writer dss_app_raster adc_app_volt_measure rtc_app_clock lcdc_app_raster dcan_app_frame_tx_rx dcan_app_loopback cap_tsc_app vpfe_app_capture_display nand_app_read_write mmcsd_app_fs_shell ddr_app_diag_test rtc_app_only pm_app_demo
example_utils test_app uart_app_echo uart_app_console cache_mmu_app gpio_app_led_blink gpio_app_buzzer dmtimer_app_cdt i2c_app_eeprom_read epwm_app_haptics_motor mcspi_app_flash qspi_app_read_write wdt_app_cpu_reset qspi_app_flash_writer dss_app_raster adc_app_volt_measure rtc_app_clock lcdc_app_raster dcan_app_frame_tx_rx dcan_app_loopback cap_tsc_app vpfe_app_capture_display nand_app_read_write mmcsd_app_fs_shell ddr_app_diag_test rtc_app_only pm_app_demo:
	$(MAKE) -C $($@_PATH)

.PHONY : example_utils_clean test_app_clean uart_app_echo_clean uart_app_console_clean cache_mmu_app_clean gpio_app_led_blink_clean gpio_app_buzzer_clean dmtimer_app_cdt_clean i2c_app_eeprom_read_clean epwm_app_haptics_motor_clean qspi_app_read_write_clean wdt_app_cpu_reset_clean qspi_app_flash_writer_clean dss_app_raster_clean adc_app_volt_measure_clean rtc_app_clock_clean lcdc_app_raster_clean dcan_app_frame_tx_rx_clean dcan_app_loopback_clean cap_tsc_app_clean vpfe_app_capture_display_clean nand_app_read_write_clean mmcsd_app_fs_shell_clean ddr_app_diag_test_clean rtc_app_only_clean pm_app_demo_clean
example_utils_clean test_app_clean uart_app_echo_clean uart_app_console_clean cache_mmu_app_clean gpio_app_led_blink_clean gpio_app_buzzer_clean dmtimer_app_cdt_clean i2c_app_eeprom_read_clean epwm_app_haptics_motor_clean mcspi_app_flash_clean qspi_app_read_write_clean wdt_app_cpu_reset_clean qspi_app_flash_writer_clean dss_app_raster_clean adc_app_volt_measure_clean rtc_app_clock_clean lcdc_app_raster_clean dcan_app_frame_tx_rx_clean dcan_app_loopback_clean cap_tsc_app_clean vpfe_app_capture_display_clean nand_app_read_write_clean mmcsd_app_fs_shell_clean ddr_app_diag_test_clean rtc_app_only_clean pm_app_demo_clean:
	$(MAKE) -C $($(subst _clean,,$@)_PATH) clean

# Nothing beyond this point
