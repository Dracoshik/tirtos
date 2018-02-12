#
_XDCBUILDCOUNT = 
ifneq (,$(findstring path,$(_USEXDCENV_)))
override XDCPATH = /oe/bld/build-CORTEX_1/arago-tmp-external-linaro-toolchain/sysroots/am335x-evm/usr/share/ti/ti-sysbios-tree/packages;/oe/bld/build-CORTEX_1/arago-tmp-external-linaro-toolchain/sysroots/am335x-evm/usr/share/ti/ti-xdctools-tree/packages;/opt/tools/edma3/edma3_lld_02_12_00_20/packages;/opt/tools/ndk/ndk_2_24_03_35/packages;/oe/bld/build-CORTEX_1/arago-tmp-external-linaro-toolchain/sysroots/am335x-evm/usr/share/ti/ti-pdk-tree/packages;
override XDCROOT = /oe/bld/build-CORTEX_1/arago-tmp-external-linaro-toolchain/sysroots/am335x-evm/usr/share/ti/ti-xdctools-tree
override XDCBUILDCFG = config_mk.bld
endif
ifneq (,$(findstring args,$(_USEXDCENV_)))
override XDCARGS = 
override XDCTARGETS = 
endif
#
ifeq (0,1)
PKGPATH = /oe/bld/build-CORTEX_1/arago-tmp-external-linaro-toolchain/sysroots/am335x-evm/usr/share/ti/ti-sysbios-tree/packages;/oe/bld/build-CORTEX_1/arago-tmp-external-linaro-toolchain/sysroots/am335x-evm/usr/share/ti/ti-xdctools-tree/packages;/opt/tools/edma3/edma3_lld_02_12_00_20/packages;/opt/tools/ndk/ndk_2_24_03_35/packages;/oe/bld/build-CORTEX_1/arago-tmp-external-linaro-toolchain/sysroots/am335x-evm/usr/share/ti/ti-pdk-tree/packages;/oe/bld/build-CORTEX_1/arago-tmp-external-linaro-toolchain/sysroots/am335x-evm/usr/share/ti/ti-xdctools-tree/packages;../..
HOSTOS = Linux
endif
