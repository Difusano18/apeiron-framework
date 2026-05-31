@echo off
setlocal EnableExtensions EnableDelayedExpansion

cd /d "%~dp0"
title APEIRON GUI

set "BUILD_DIR=build"
set "BUILD_TYPE=Release"
set "PLATFORM=x64"
set "GENERATOR="

where cmake >nul 2>nul
if errorlevel 1 (
    echo [ERROR] CMake not found in PATH.
    pause
    exit /b 1
)

set "VSWHERE=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
set "VCVARS_PATH="

if exist "%VSWHERE%" (
    for /f "usebackq tokens=*" %%i in (`"%VSWHERE%" -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath 2^>nul`) do (
        set "VS_PATH=%%i"
    )
    for /f "usebackq tokens=*" %%i in (`"%VSWHERE%" -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationVersion 2^>nul`) do (
        set "VS_VERSION=%%i"
    )
    if defined VS_PATH (
        set "VCVARS_PATH=!VS_PATH!\VC\Auxiliary\Build\vcvars64.bat"
        for /f "tokens=1 delims=." %%a in ("!VS_VERSION!") do set "VS_MAJOR=%%a"
        if "!VS_MAJOR!"=="18" set "GENERATOR=Visual Studio 18 2026"
        if "!VS_MAJOR!"=="17" set "GENERATOR=Visual Studio 17 2022"
        if "!VS_MAJOR!"=="16" set "GENERATOR=Visual Studio 16 2019"
    )
)

if defined VCVARS_PATH if exist "!VCVARS_PATH!" call "!VCVARS_PATH!" >nul

where cl >nul 2>nul
if errorlevel 1 (
    echo [ERROR] Visual Studio C++ compiler not found.
    echo Install Desktop development with C++ or run from a Developer Command Prompt.
    pause
    exit /b 1
)

if not defined GENERATOR (
    set "GENERATOR=Visual Studio 17 2022"
)

if not exist "%BUILD_DIR%" mkdir "%BUILD_DIR%"

echo [APEIRON] Configuring...
cmake -S . -B "%BUILD_DIR%" -G "%GENERATOR%" -A %PLATFORM% -DBUILD_TESTS=ON
if errorlevel 1 (
    echo [ERROR] CMake configure failed.
    pause
    exit /b 1
)

echo [APEIRON] Building GUI...
cmake --build "%BUILD_DIR%" --config %BUILD_TYPE% --target apeiron-gui --parallel
if errorlevel 1 (
    echo [ERROR] GUI build failed.
    pause
    exit /b 1
)

set "GUI_EXE=%BUILD_DIR%\bin\%BUILD_TYPE%\apeiron-gui.exe"
if not exist "%GUI_EXE%" (
    echo [ERROR] GUI executable missing: %CD%\%GUI_EXE%
    pause
    exit /b 1
)

echo [APEIRON] Launching GUI...
start "" "%GUI_EXE%"
exit /b 0
