#
# Copyright (c) 2016-2017, Texas Instruments Incorporated
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

# File: nimu_component.mk
#       This file is component include make file of NIMU library.
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
#                             this module has no board dependent code and hence
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
ifeq ($(nimu_component_make_include), )

# under other list
drvnimu_BOARDLIST       = 
drvnimu_SOCLIST         = dra72x dra75x am574x am572x am571x k2h k2k k2l k2e k2g c6678 c6657 am437x am335x dra78x omapl137 omapl138
drvnimu_dra72x_CORELIST = a15_0 ipu1_0
drvnimu_dra75x_CORELIST = a15_0 ipu1_0
drvnimu_am574x_CORELIST = c66x a15_0 ipu1_0
drvnimu_am572x_CORELIST = c66x a15_0 ipu1_0
drvnimu_am571x_CORELIST = c66x a15_0 ipu1_0
drvnimu_k2h_CORELIST    = c66x a15_0
drvnimu_k2k_CORELIST    = c66x a15_0
drvnimu_k2l_CORELIST    = c66x a15_0
drvnimu_k2e_CORELIST    = c66x a15_0
drvnimu_k2g_CORELIST    = c66x a15_0
drvnimu_c6678_CORELIST  = c66x
drvnimu_c6657_CORELIST  = c66x
drvnimu_am437x_CORELIST = a9host
drvnimu_am335x_CORELIST = a8host
drvnimu_dra78x_CORELIST = ipu1_0
drvnimu_omapl137_CORELIST = c674x arm9_0
drvnimu_omapl138_CORELIST = c674x arm9_0

############################
# uart package
# List of components included under uart lib
# The components included here are built and will be part of uart lib
############################
nimu_LIB_LIST = nimu nimu_profile
drvnimu_LIB_LIST = $(nimu_LIB_LIST)

#
# NIMU Modules
#

# NIMU LIB
nimu_COMP_LIST = nimu
nimu_RELPATH = ti/transport/ndk/nimu
nimu_PATH = $(PDK_NIMU_COMP_PATH)
nimu_LIBNAME = ti.transport.ndk.nimu
export nimu_LIBNAME
nimu_LIBPATH = $(nimu_PATH)/lib
export nimu_LIBPATH
nimu_OBJPATH = $(nimu_RELPATH)/nimu
export nimu_OBJPATH
nimu_MAKEFILE = -f build/makefile.mk
export nimu_MAKEFILE
nimu_BOARD_DEPENDENCY = no
nimu_CORE_DEPENDENCY = no
nimu_SOC_DEPENDENCY = yes
export nimu_COMP_LIST
export nimu_BOARD_DEPENDENCY
export nimu_CORE_DEPENDENCY
export nimu_SOC_DEPENDENCY
nimu_PKG_LIST = nimu
export nimu_PKG_LIST
nimu_INCLUDE = $(nimu_PATH)
nimu_SOCLIST = $(drvnimu_SOCLIST)
export nimu_SOCLIST
nimu_$(SOC)_CORELIST = $(drvnimu_$(SOC)_CORELIST)
export nimu_$(SOC)_CORELIST

# NIMU PROFILING SOC LIB
nimu_profile_COMP_LIST = nimu_profile
nimu_profile_RELPATH = ti/transport/ndk/nimu
nimu_profile_PATH = $(PDK_NIMU_COMP_PATH)
nimu_profile_LIBNAME = ti.transport.ndk.nimu.profiling
export nimu_profile_LIBNAME
nimu_profile_LIBPATH = $(nimu_profile_PATH)/lib
export nimu_profile_LIBPATH
nimu_profile_OBJPATH = $(nimu_profile_RELPATH)/nimu_profile
export nimu_profile_OBJPATH
nimu_profile_MAKEFILE = -f build/makefile_profile.mk
export nimu_profile_MAKEFILE
nimu_profile_BOARD_DEPENDENCY = no
nimu_profile_CORE_DEPENDENCY = no
nimu_profile_SOC_DEPENDENCY = yes
export nimu_profile_COMP_LIST
export nimu_profile_BOARD_DEPENDENCY
export nimu_profile_CORE_DEPENDENCY
export nimu_profile_SOC_DEPENDENCY
nimu_profile_PKG_LIST = nimu_profile
export nimu_profile_PKG_LIST
nimu_profile_INCLUDE = $(nimu_profile_PATH)
nimu_profile_SOCLIST = $(drvnimu_SOCLIST)
export nimu_profile_SOCLIST
nimu_profile_$(SOC)_CORELIST = $(drvnimu_$(SOC)_CORELIST)
export nimu_profile_$(SOC)_CORELIST

# NIMU examples
nimu_EXAMPLE_LIST = nimu_test_app

nimu_test_app_COMP_LIST = nimu_test_app
nimu_test_app_RELPATH = ti/transport/ndk/nimu/example/dra7xx
nimu_test_app_PATH = $(PDK_NIMU_COMP_PATH)/example/dra7xx
nimu_test_app_BOARD_DEPENDENCY = yes
nimu_test_app_CORE_DEPENDENCY = no
nimu_test_app_XDC_CONFIGURO = yes
export nimu_test_app_COMP_LIST
export nimu_test_app_BOARD_DEPENDENCY
export nimu_test_app_CORE_DEPENDENCY
export nimu_test_app_XDC_CONFIGURO
nimu_test_app_PKG_LIST = nimu_test_app
nimu_test_app_INCLUDE = $(nimu_test_app_PATH)
nimu_test_app_BOARDLIST = evmDRA75x evmDRA78x
export nimu_test_app_BOARDLIST
nimu_test_app_$(SOC)_CORELIST = $(nimu_$(SOC)_CORELIST)
export nimu_test_app_$(SOC)_CORELIST

export drvnimu_LIB_LIST
export nimu_LIB_LIST
export nimu_EXAMPLE_LIST

nimu_component_make_include := 1
endif
