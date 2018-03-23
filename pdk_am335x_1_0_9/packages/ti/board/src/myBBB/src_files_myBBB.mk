
SRCDIR += src/myBBB src/myBBB/device src/myBBB/include
INCDIR += src/myBBB src/myBBB/device src/myBBB/include

# Common source files across all platforms and cores
SRCS_COMMON += myBBB.c myBBB_info.c myBBB_lld_init.c myBBB_pinmux.c enet_phy.c

PACKAGE_SRCS_COMMON =   src/myBBB/src_files_myBBB.mk
