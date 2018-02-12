@REM ******************************************************************************
@REM * FILE PURPOSE: PDK Unit Test and Example Project Creator
@REM ******************************************************************************
@REM * FILE NAME: pdkAppImageCreate.bat
@REM *
@REM * DESCRIPTION: 
@REM *  The script file is used to create bootable application binaries from the
@REM *  executable generated by the CCS Projects.
@REM *
@REM * Syntax:
@REM *  pdkAppImageCreate.bat [PDK_PATH] [CG_TOOL_PATH] [APP_PATH] [APP_NAME] [SOC] [PROCESSOR]
@REM *
@REM *
@REM * Copyright (C) 2016-2017, Texas Instruments, Inc.
@REM *****************************************************************************
@echo off

set PDK_PATH=%1
set CG_TOOL_PATH=%2
set APP_PATH=%3
set APP_NAME=%4
set SOC=%5
set PROCESSOR=%6

@if %SOC% == AM437x (
    goto SOC_AM3_AM4
)
@if %SOC% == AM335x (
    goto SOC_AM3_AM4
)
@if %SOC% == AM572x (
    goto SOC_AM57XX
)
@if %SOC% == AM574x (
    goto SOC_AM57XX
)
@if %SOC% == AM571x (
    goto SOC_AM57XX
) 
@if %SOC% == K2G (
    goto SOC_K2G
)
@if %SOC% == OMAPL137 (
    goto SOC_OMAPL13X
)
@if %SOC% == OMAPL138 (
    goto SOC_OMAPL13X
)

@goto ENDSCRIPT

:SOC_AM57XX
    if %PROCESSOR% == arm (
        set BIN_PATH=%APP_PATH%
        set APP_MPU_CPU0=%APP_PATH%/%APP_NAME%.out
        set TOOLS_PATH=%PDK_PATH%/ti/boot/sbl/tools
        call %PDK_PATH%/ti/boot/sbl/tools/scripts/AM57xImageGen.bat
    )
    if %PROCESSOR% == dsp (
        set BIN_PATH=%APP_PATH%
        set APP_DSP1=%APP_PATH%/%APP_NAME%.out
        set TOOLS_PATH=%PDK_PATH%/ti/boot/sbl/tools
        call %PDK_PATH%/ti/boot/sbl/tools/scripts/AM57xImageGen.bat
    )
    if %PROCESSOR% == m4 (
        set BIN_PATH=%APP_PATH%
        set APP_IPU1_CPU0=%APP_PATH%/%APP_NAME%.out
        set TOOLS_PATH=%PDK_PATH%/ti/boot/sbl/tools
        call %PDK_PATH%/ti/boot/sbl/tools/scripts/AM57xImageGen.bat
    )
    goto ENDSCRIPT

:SOC_K2G
    if %PROCESSOR% == arm (
        set BIN_PATH=%APP_PATH%
        set APP_MPU_CPU0=%APP_PATH%/%APP_NAME%.out
        set TOOLS_PATH=%PDK_PATH%/ti/boot/sbl/tools
        call %PDK_PATH%/ti/boot/sbl/tools/scripts/K2GImageGen.bat
    )
    if %PROCESSOR% == dsp (
        set BIN_PATH=%APP_PATH%
        set APP_DSP0=%APP_PATH%/%APP_NAME%.out
        set TOOLS_PATH=%PDK_PATH%/ti/boot/sbl/tools
        call %PDK_PATH%/ti/boot/sbl/tools/scripts/K2GImageGen.bat
    )
    goto ENDSCRIPT

:SOC_AM3_AM4
    %CG_TOOL_PATH%/bin/arm-none-eabi-objcopy -O binary %APP_NAME%.out %APP_NAME%.bin
    %PDK_PATH%/ti/starterware/tools/ti_image/tiimage.exe 0x80000000 NONE %APP_NAME%.bin app )
    goto ENDSCRIPT

:SOC_OMAPL13X
    if %PROCESSOR% == arm (
        set BIN_PATH=%APP_PATH%
        set APP_MPU_CPU0=%APP_PATH%/%APP_NAME%.out
        set TOOLS_PATH=%PDK_PATH%/ti/boot/sbl/tools
        call %PDK_PATH%/ti/boot/sbl/tools/scripts/OMAPL13xImageGen.bat
    )
    if %PROCESSOR% == dsp (
        set BIN_PATH=%APP_PATH%
        set APP_DSP0=%APP_PATH%/%APP_NAME%.out
        set TOOLS_PATH=%PDK_PATH%/ti/boot/sbl/tools
        call %PDK_PATH%/ti/boot/sbl/tools/scripts/OMAPL13xImageGen.bat
    )
    goto ENDSCRIPT

:ENDSCRIPT