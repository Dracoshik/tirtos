 
SRCDIR = . src/tirtos
INCDIR = . src/tirtos

SRCS_COMMON += SemaphoreP_tirtos.c HwiP_tirtos.c Utils_tirtos.c CacheP_tirtos.c RegisterIntr_tirtos.c EventCombinerP_tirtos.c Queue_tirtos.c

ifeq ($(SOC),$(filter $(SOC), am574x am572x am571x k2g k2l k2e k2h k2k c6678 c6657 am437x am335x omapl137 omapl138 c6747))
SRCS_COMMON += TimerP_tirtos.c
endif

PACKAGE_SRCS_COMMON = makefile HwiP.h MuxIntcP.h osal.h osal_component.mk SemaphoreP.h \
                      src/tirtos src/src_common_nonos.mk  src/src_common_tirtos.mk \
                      docs/OSAL_SoftwareManifest.html docs/ReleaseNotes_OSAL.pdf \
                      build/makefile_nonos_indp.mk build/makefile_nonos.mk build/makefile_tirtos_indp.mk build/makefile_tirtos.mk

