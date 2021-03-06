# Copyright (C) 2017 Texas Instruments Incorporated - http://www.ti.com/
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

# Output directories
DIAGOBJDIR = $(PDK_DIAG_DIR)/obj/$(BOARD)/$(TARGET)
DIAGBINDIR = $(PDK_DIAG_DIR)/bin/$(BOARD)/$(TARGET)

# ARMV7 link directory that contain built peripheral libraries
ARMV7LIBDIR ?= ./lib

# Cross tools
ifdef C6X_GEN_INSTALL_PATH
# Support backwards compatibility with KeyStone1 approach
 CC = $(C6X_GEN_INSTALL_PATH)/bin/cl6x
endif

# INCLUDE Directories
CSL_DIR = $(PDK_INSTALL_PATH)/ti/csl/
INCDIR := $(PDK_INSTALL_PATH)/
COMPILERDIR = $(C6X_GEN_INSTALL_PATH)/include

# Libraries
CSL_LIB = "$(PDK_INSTALL_PATH)/ti/csl/lib/omapl137/c674/release/ti.csl.ae674"
CSL_INTC_LIB = "$(PDK_INSTALL_PATH)/ti/csl/lib/omapl137/c674/release/ti.csl.intc.ae674"
RTSLIB_PATH = "$(C6X_GEN_INSTALL_PATH)/lib/libc.a"
LNK_LIBS = -l$(CSL_LIB) -l$(CSL_INTC_LIB) -l$(RTSLIB_PATH)

DEFFLAGS = --define=SOC_$(DEVICE) --define=BUILD_DSP
# Compiler options
INTERNALDEFS = -g $(DEBUG_FLAG) -mv64+ -g $(DEFFLAGS) --diag_wrap=off --diag_warning=225 --display_error_number 

# Linker options
INTERNALLINKDEFS = -mv64+ -g $(DEVFLAGS) --diag_wrap=off --diag_warning=225 --display_error_number -z

# Executable using device independent library and device object file
EXE=mcaspAudioDC_diagExample_$(BOARD)_$(TARGET).out

OBJEXT = o
ASMOBJEXT = ao
SRCDIR = $(PDK_DIAG_DIR)/diag/mcasp_audiodc/src

INCS = -I. -I$(strip $(subst ;, -I,$(INCDIR))) -I$(COMPILERDIR)

LINK_CMD_FILE = $(PDK_DIAG_DIR)/diag/common/$(DEVICE)/$(TARGET)/linker.cmd

VPATH=$(SRCDIR)

#List the Source Files
TEST_SRC = \
	mcasp_audiodc_test.c


# FLAGS for the SourceFiles
CFLAGS += -ms -oe --inline_recursion_limit=20 -O2
SRC_CFLAGS = -I. $(CFLAGS) -eo.$(OBJEXT) -ea.$(ASMOBJEXT) -g

# Make Rule for the SRC Files
TEST_SRC_OBJS = $(patsubst %.c, $(DIAGOBJDIR)/%.$(OBJEXT), $(TEST_SRC))

all:$(DIAGBINDIR)/$(EXE)

$(DIAGBINDIR)/$(EXE): $(TEST_SRC_OBJS)  $(DIAGBINDIR)/.created $(DIAGOBJDIR)/.created
	@echo linking $(TEST_SRC_OBJS) into $@ ...
	@$(CC) $(TEST_SRC_OBJS) $(INTERNALLINKDEFS) $(LINK_CMD_FILE) -o $@ -m"$@.map" $(LNK_LIBS) --rom_model

$(DIAGOBJDIR)/%.$(OBJEXT): %.c $(DIAGOBJDIR)/.created
	@echo compiling $< ...
	@$(CC) -c $(SRC_CFLAGS) $(INTERNALDEFS) $(INCS) -fr=$(DIAGOBJDIR) -fs=$(DIAGOBJDIR) -fc $<

$(DIAGOBJDIR)/.created:
	@mkdir -p $(DIAGOBJDIR)
	@touch $(DIAGOBJDIR)/.created

$(DIAGBINDIR)/.created:
	@mkdir -p $(DIAGBINDIR)
	@touch $(DIAGBINDIR)/.created

clean:
	@rm -f $(DIAGBINDIR)/$(EXE)
	@rm -f $(TEST_SRC_OBJS) $(TEST_SRC_DEV_OBJS) $(ENTRY_OBJ) $(DIAGBINDIR)/.created $(DIAGOBJDIR)/.created
