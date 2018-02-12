#
# Copyright (c) 2016, Texas Instruments Incorporated
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
# *  Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
#
# *  Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
#
# *  Neither the name of Texas Instruments Incorporated nor the names of
#    its contributors may be used to endorse or promote products derived
#    from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
# THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
# PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
# CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
# EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
# PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
# OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
# WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
# OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
# EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#

# File: osal_component.mk
#       This file is component include make file of OSAL library.
# List of variables set in this file and their purpose:
# <mod>_RELPATH        - This is the relative path of the module, typically from
#                        top-level directory of the package
# <mod>_PATH           - This is the absolute path of the module. It derives from
#                        absolute path of the top-level directory (set in env.mk)
#                        and relative path set above
# <mod>_INCLUDE        - This is the path that has interface header files of the
#                        module. This can be multiple directories (space separated)
# <mod>_PKG_LIST       - Names of the modules (and sub-modules) that are a part
#                        part of this module, including itself.
# <mod>_BOARD_DEPENDENCY - "yes": means the code for this module depends on
#                             board and the compiled obj/lib has to be kept
#                             under <board> directory
#                             "no" or "" or if this variable is not defined: means
#                             this module has no baord dependent code and hence
#                             the obj/libs are not kept under <board> dir.
# <mod>_CORE_DEPENDENCY     - "yes": means the code for this module depends on
#                             core and the compiled obj/lib has to be kept
#                             under <core> directory
#                             "no" or "" or if this variable is not defined: means
#                             this module has no core dependent code and hence
#                             the obj/libs are not kept under <core> dir.
# <mod>_APP_STAGE_FILES     - List of source files that belongs to the module
#                             <mod>, but that needs to be compiled at application
#                             build stage (in the context of the app). This is
#                             primarily for link time configurations or if the
#                             source file is dependent on options/defines that are
#                             application dependent. This can be left blank or
#                             not defined at all, in which case, it means there
#                             no source files in the module <mod> that are required
#                             to be compiled in the application build stage.
#
ifeq ($(osal_component_make_include), )

libosal_BOARDLIST       = evmAM572x evmAM335x evmAM437x evmDRA72x evmDRA75x evmDRA78x iceK2G idkAM574x idkAM572x idkAM571x idkAM437x
libosal_SOCLIST         = tda2xx tda2px tda2ex tda3xx dra78x dra72x dra75x am574x am572x am571x k2h k2k k2l k2e k2g c6678 c6657 am437x am335x omapl137 omapl138
libosal_tda2xx_CORELIST = a15_0 ipu1_0
libosal_tda2px_CORELIST = a15_0 ipu1_0
libosal_tda2ex_CORELIST = a15_0 ipu1_0
libosal_tda3xx_CORELIST = ipu1_0
libosal_dra72x_CORELIST = c66x a15_0 ipu1_0
libosal_dra75x_CORELIST = c66x a15_0 ipu1_0
libosal_dra78x_CORELIST = c66x ipu1_0
libosal_am574x_CORELIST = c66x a15_0 ipu1_0
libosal_am572x_CORELIST = c66x a15_0 ipu1_0
libosal_am571x_CORELIST = c66x a15_0 ipu1_0
libosal_k2h_CORELIST    = c66x a15_0
libosal_k2k_CORELIST    = c66x a15_0
libosal_k2l_CORELIST    = c66x a15_0
libosal_k2e_CORELIST    = c66x a15_0
libosal_k2g_CORELIST    = c66x a15_0
libosal_omapl137_CORELIST    = c674x arm9_0
libosal_omapl138_CORELIST    = c674x arm9_0
libosal_c6678_CORELIST  = c66x
libosal_c6657_CORELIST  = c66x
libosal_am437x_CORELIST = a9host
libosal_am335x_CORELIST = a8host

############################
# osal package
# List of components included under osal lib
# The components included here are built and will be part of osal lib
############################
osal_LIB_LIST = osal_nonos osal_nonos_indp osal_tirtos osal_tirtos_indp
libosal_LIB_LIST = $(osal_LIB_LIST)

############################
# osal examples
# List of examples under osal
# All the tests mentioned in list are built when test target is called
# List below all examples for allowed values
############################
osal_EXAMPLE_LIST =

#
# OSAL Modules
#

# OSAL NONOS LIB
osal_nonos_COMP_LIST = osal_nonos
osal_nonos_RELPATH = ti/osal
osal_nonos_PATH = $(PDK_OSAL_COMP_PATH)
osal_nonos_LIBNAME = ti.osal
export osal_nonos_LIBNAME
osal_nonos_LIBPATH = $(osal_nonos_PATH)/lib/nonos
export osal_nonos_LIBPATH
osal_nonos_OBJPATH = $(osal_nonos_RELPATH)/osal_nonos
export osal_nonos_OBJPATH
osal_nonos_MAKEFILE = -f build/makefile_nonos.mk
export osal_nonos_MAKEFILE
osal_nonos_PLATFORM_DEPENDENCY = no
osal_nonos_CORE_DEPENDENCY = no
osal_nonos_SOC_DEPENDENCY = yes
export osal_nonos_COMP_LIST
export osal_nonos_PLATFORM_DEPENDENCY
export osal_nonos_CORE_DEPENDENCY
export osal_nonos_SOC_DEPENDENCY
osal_nonos_PKG_LIST = osal_nonos
export osal_nonos_PKG_LIST
osal_nonos_INCLUDE = $(osal_nonos_PATH)
osal_nonos_SOCLIST = $(libosal_SOCLIST)
export osal_nonos_SOCLIST
osal_nonos_$(SOC)_CORELIST = $(libosal_$(SOC)_CORELIST)
export osal_nonos_$(SOC)_CORELIST

# OSAL NONOS LIB DEVICE INDEPENDENT
osal_nonos_indp_COMP_LIST = osal_nonos_indp
osal_nonos_indp_RELPATH = ti/osal
osal_nonos_indp_PATH = $(PDK_OSAL_COMP_PATH)
osal_nonos_indp_LIBNAME = ti.osal
export osal_nonos_indp_LIBNAME
osal_nonos_indp_LIBPATH = $(osal_nonos_indp_PATH)/lib/nonos
export osal_nonos_indp_LIBPATH
osal_nonos_indp_OBJPATH = $(osal_nonos_indp_RELPATH)/osal_nonos_indp
export osal_nonos_indp_OBJPATH
osal_nonos_indp_MAKEFILE = -f build/makefile_nonos_indp.mk
export osal_nonos_indp_MAKEFILE
osal_nonos_indp_PLATFORM_DEPENDENCY = no
osal_nonos_indp_CORE_DEPENDENCY = no
osal_nonos_indp_SOC_DEPENDENCY = no
export osal_nonos_indp_COMP_LIST
export osal_nonos_indp_PLATFORM_DEPENDENCY
export osal_nonos_indp_CORE_DEPENDENCY
export osal_nonos_indp_SOC_DEPENDENCY
osal_nonos_indp_PKG_LIST = osal_nonos_indp
export osal_nonos_indp_PKG_LIST
osal_nonos_indp_INCLUDE = $(osal_nonos_indp_PATH)
osal_nonos_indp_SOCLIST = $(libosal_SOCLIST)
export osal_nonos_indp_SOCLIST
osal_nonos_indp_$(SOC)_CORELIST = $(libosal_$(SOC)_CORELIST)
export osal_nonos_indp_$(SOC)_CORELIST

# OSAL TIRTOS LIB
osal_tirtos_COMP_LIST = osal_tirtos
osal_tirtos_RELPATH = ti/osal
osal_tirtos_PATH = $(PDK_OSAL_COMP_PATH)
osal_tirtos_LIBNAME = ti.osal
export osal_tirtos_LIBNAME
osal_tirtos_LIBPATH = $(osal_tirtos_PATH)/lib/tirtos
export osal_tirtos_LIBPATH
osal_tirtos_OBJPATH = $(osal_tirtos_RELPATH)/osal_tirtos
export osal_tirtos_OBJPATH
osal_tirtos_MAKEFILE = -f build/makefile_tirtos.mk
export osal_tirtos_MAKEFILE
osal_tirtos_PLATFORM_DEPENDENCY = no
osal_tirtos_CORE_DEPENDENCY = no
osal_tirtos_SOC_DEPENDENCY = yes
export osal_tirtos_COMP_LIST
export osal_tirtos_PLATFORM_DEPENDENCY
export osal_tirtos_CORE_DEPENDENCY
export osal_tirtos_SOC_DEPENDENCY
osal_tirtos_PKG_LIST = osal_tirtos
export osal_tirtos_PKG_LIST
osal_tirtos_INCLUDE = $(osal_tirtos_PATH)
osal_tirtos_SOCLIST = $(libosal_SOCLIST)
export osal_tirtos_SOCLIST
osal_tirtos_$(SOC)_CORELIST = $(libosal_$(SOC)_CORELIST)
export osal_tirtos_$(SOC)_CORELIST

# OSAL TIRTOS DEVICE INDEPENDENT
osal_tirtos_indp_COMP_LIST = osal_tirtos_indp
osal_tirtos_indp_RELPATH = ti/osal
osal_tirtos_indp_PATH = $(PDK_OSAL_COMP_PATH)
osal_tirtos_indp_LIBNAME = ti.osal
export osal_tirtos_indp_LIBNAME
osal_tirtos_indp_LIBPATH = $(osal_tirtos_indp_PATH)/lib/tirtos
export osal_tirtos_indp_LIBPATH
osal_tirtos_indp_OBJPATH = $(osal_tirtos_indp_RELPATH)/osal_tirtos_indp
export osal_tirtos_indp_OBJPATH
osal_tirtos_indp_MAKEFILE = -f build/makefile_tirtos_indp.mk
export osal_tirtos_indp_MAKEFILE
osal_tirtos_indp_PLATFORM_DEPENDENCY = no
osal_tirtos_indp_CORE_DEPENDENCY = no
osal_tirtos_indp_SOC_DEPENDENCY = no
export osal_tirtos_indp_COMP_LIST
export osal_tirtos_indp_PLATFORM_DEPENDENCY
export osal_tirtos_indp_CORE_DEPENDENCY
export osal_tirtos_indp_SOC_DEPENDENCY
osal_tirtos_indp_PKG_LIST = osal_tirtos_indp
export osal_tirtos_indp_PKG_LIST
osal_tirtos_indp_INCLUDE = $(osal_tirtos_indp_PATH)
osal_tirtos_indp_SOCLIST = $(libosal_SOCLIST)
export osal_tirtos_indp_SOCLIST
osal_tirtos_indp_$(SOC)_CORELIST = $(libosal_$(SOC)_CORELIST)
export osal_tirtos_indp_$(SOC)_CORELIST

#
# OSAL Examples
#
osal_baremetal_test_COMP_LIST = osal_baremetal_test
osal_baremetal_test_RELPATH = ti/osal/test/baremetal
osal_baremetal_test_PATH = $(PDK_OSAL_COMP_PATH)/test/baremetal
osal_baremetal_test_BOARD_DEPENDENCY = yes
osal_baremetal_test_CORE_DEPENDENCY = no
export osal_baremetal_test_COMP_LIST
export osal_baremetal_test_BOARD_DEPENDENCY
export osal_baremetal_test_CORE_DEPENDENCY
osal_baremetal_test_PKG_LIST = osal_baremetal_test
osal_baremetal_test_INCLUDE = $(osal_baremetal_test_PATH)
osal_baremetal_test_BOARDLIST = $(libosal_BOARDLIST)
export osal_baremetal_test_BOARDLIST

#Default Core List
osal_baremetal_test_$(SOC)_CORELIST = a15_0 c66x

# Overwrite default core list for below SoCs
ifeq ($(BOARD),$(filter $(BOARD), idkAM574x evmAM572x idkAM572x idkAM571x))
osal_baremetal_test_$(SOC)_CORELIST = a15_0 ipu1_0 c66x
endif

ifeq ($(BOARD),$(filter $(BOARD), evmAM335x))
osal_baremetal_test_$(SOC)_CORELIST = a8host
endif


ifeq ($(BOARD),$(filter $(BOARD), evmAM437x idkAM437x))
osal_baremetal_test_$(SOC)_CORELIST = a9host
endif

ifeq ($(SOC),$(filter $(SOC), dra72x dra75x))
osal_baremetal_test_$(SOC)_CORELIST = a15_0 ipu1_0 c66x
endif

ifeq ($(SOC),$(filter $(SOC), dra78x))
osal_baremetal_test_$(SOC)_CORELIST = ipu1_0 c66x
endif

export osal_baremetal_test_$(SOC)_CORELIST
osal_EXAMPLE_LIST += osal_baremetal_test

export osal_LIB_LIST
export libosal_LIB_LIST
export osal_EXAMPLE_LIST

libosal_EXAMPLE_LIST = $(osal_EXAMPLE_LIST)
export libosal_EXAMPLE_LIST

osal_component_make_include := 1
endif
