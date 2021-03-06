#*******************************************************************************
#* FILE PURPOSE: Top level makefile for Creating Processor SDK Component
#*               Libraries and binaries
#*******************************************************************************
#* FILE NAME: makefile
#*
#* DESCRIPTION: Makefile for SDK level components
#*
#* Copyright (C) 2015-2017, Texas Instruments, Inc.
#*
#* Redistribution and use in source and binary forms, with or without 
#* modification, are permitted provided that the following conditions 
#* are met:
#*
#*   Redistributions of source code must retain the above copyright 
#*   notice, this list of conditions and the following disclaimer.
#*
#*   Redistributions in binary form must reproduce the above copyright
#*   notice, this list of conditions and the following disclaimer in the 
#*   documentation and/or other materials provided with the   
#*   distribution.
#*
#*   Neither the name of Texas Instruments Incorporated nor the names of
#*   its contributors may be used to endorse or promote products derived
#*   from this software without specific prior written permission.
#*
#* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
#* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
#* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
#* A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
#* OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
#* SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
#* LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
#* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
#* THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
#* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
#* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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

all: pdk
clean: pdk_clean

help:
	@echo "Standard Targets:"
	@echo "    help      - Prints target information"
	@echo "    all       - Builds all Component targets"
	@echo "    clean     - Cleans all Component targets"
	@echo ""
	@echo "Component Targets:"
	@echo "    pdk       - Builds all targets within the PDK top-level makefile"
	@echo "    pdk_clean - Cleans all targets within the PDK top-level makefile"
	@echo ""
	@echo "    NOTE: Instructions for rebuilding all other components"
	@echo "          installed with Processor SDK can be found in each"
	@echo "          component's sub-directory"

pdk:
	$(MAKE) -C $(PDK_INSTALL_PATH) all

pdk_clean:
	$(MAKE) -C $(PDK_INSTALL_PATH) clean
