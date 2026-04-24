@echo off
setlocal EnableDelayedExpansion

:: Always run from the directory where run.bat lives
cd /d "%~dp0"

echo [DEBUG] Script started. Working dir: %CD%

:: APEIRON Framework Build and Run Script

title APEIRON Build System

:: Configuration
set "BUILD_DIR=build"
set "BUILD_TYPE=Release"
set "GENERATOR=Visual Studio 18 2026"
set "PLATFORM=x64"

:: Check for CMake
where cmake > nul 2> nul
if not "%errorlevel%"=="0" (
    echo [ERROR] CMake not found in PATH
    echo Please install CMake 3.25+ from https://cmake.org/download/
    pause
    exit /b 1
)

echo.
echo ============================================================
echo                APEIRON-OMEGA v0.2.0
echo         Build and Run Script (Windows)
echo ============================================================
echo.

:: Try to find Visual Studio using vswhere
set "VSWHERE=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
set "VCVARS_PATH="
set "VS_PATH="
set "GENERATOR="
if exist "%VSWHERE%" (
    echo [INFO] Found vswhere.exe, locating Visual Studio...

    :: Get installation path and version
    for /f "usebackq tokens=*" %%i in (`"%VSWHERE%" -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath 2^>nul`) do (
        set "VS_PATH=%%i"
    )

    :: Get VS version number
    for /f "usebackq tokens=*" %%i in (`"%VSWHERE%" -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationVersion 2^>nul`) do (
        set "VS_VERSION=%%i"
    )

    if defined VS_PATH (
        set "VCVARS_PATH=!VS_PATH!\VC\Auxiliary\Build\vcvars64.bat"
        if exist "!VCVARS_PATH!" (
            echo [INFO] Found Visual Studio at: !VS_PATH!
            echo [INFO] Version: !VS_VERSION!
            :: Extract major version and set generator
            for /f "tokens=1 delims=." %%a in ("!VS_VERSION!") do (
                set "VS_MAJOR=%%a"
            )
            if "!VS_MAJOR!"=="18" set "GENERATOR=Visual Studio 18 2026"
            if "!VS_MAJOR!"=="17" set "GENERATOR=Visual Studio 17 2022"
            if "!VS_MAJOR!"=="16" set "GENERATOR=Visual Studio 16 2019"
            if "!VS_MAJOR!"=="15" set "GENERATOR=Visual Studio 15 2017"
            echo [INFO] Using generator: !GENERATOR!
            goto :vs_found
        )
    )
)

:: If we get here, try to find any VS version
for /d %%D in ("C:\Program Files\Microsoft Visual Studio\2*") do (
    if exist "%%~D\Community\VC\Auxiliary\Build\vcvars64.bat" (
        set "VS_PATH=%%~D\Community"
        set "VCVARS_PATH=!VS_PATH!\VC\Auxiliary\Build\vcvars64.bat"
        goto :vs_found
    )
    if exist "%%~D\Professional\VC\Auxiliary\Build\vcvars64.bat" (
        set "VS_PATH=%%~D\Professional"
        set "VCVARS_PATH=!VS_PATH!\VC\Auxiliary\Build\vcvars64.bat"
        goto :vs_found
    )
    if exist "%%~D\Enterprise\VC\Auxiliary\Build\vcvars64.bat" (
        set "VS_PATH=%%~D\Enterprise"
        set "VCVARS_PATH=!VS_PATH!\VC\Auxiliary\Build\vcvars64.bat"
        goto :vs_found
    )
)

for /d %%D in ("C:\Program Files (x86)\Microsoft Visual Studio\2*") do (
    if exist "%%~D\Community\VC\Auxiliary\Build\vcvars64.bat" (
        set "VS_PATH=%%~D\Community"
        set "VCVARS_PATH=!VS_PATH!\VC\Auxiliary\Build\vcvars64.bat"
        goto :vs_found
    )
    if exist "%%~D\Professional\VC\Auxiliary\Build\vcvars64.bat" (
        set "VS_PATH=%%~D\Professional"
        set "VCVARS_PATH=!VS_PATH!\VC\Auxiliary\Build\vcvars64.bat"
        goto :vs_found
    )
    if exist "%%~D\Enterprise\VC\Auxiliary\Build\vcvars64.bat" (
        set "VS_PATH=%%~D\Enterprise"
        set "VCVARS_PATH=!VS_PATH!\VC\Auxiliary\Build\vcvars64.bat"
        goto :vs_found
    )
    if exist "%%~D\BuildTools\VC\Auxiliary\Build\vcvars64.bat" (
        set "VS_PATH=%%~D\BuildTools"
        set "VCVARS_PATH=!VS_PATH!\VC\Auxiliary\Build\vcvars64.bat"
        goto :vs_found
    )
)

:vs_found
:: Check if we found VS via vswhere and already have generator set
if defined VCVARS_PATH (
    if exist "!VCVARS_PATH!" (
        call "!VCVARS_PATH!"
        echo [INFO] Visual Studio environment loaded
        goto :vs_ok
    )
)

:: Fallback: check if already in VS environment
if defined VSINSTALLDIR (
    echo [INFO] Using existing Visual Studio environment
    goto :vs_ok
)

echo.
echo [ERROR] Visual Studio not found automatically!
echo.
echo Try one of these options:
echo.
echo 1. Run this script from "Developer Command Prompt for VS"
echo    (Start Menu -^> Visual Studio -^> Developer Command Prompt)
echo.
echo 2. Install Visual Studio with C++ workload:
echo    https://visualstudio.microsoft.com/downloads/
echo    Required: "Desktop development with C++" workload
echo.
echo 3. If VS is installed in custom location, edit this script
echo    and add your vcvars64.bat path
echo.
pause
exit /b 1

:vs_ok
:: Verify compiler is available
where cl > nul 2> nul
if not exist "%VSWHERE%" (
    echo [WARNING] vswhere not found after VS setup
)
echo [INFO] VS environment ready

:: Menu
:menu
echo.
echo [1] Build from scratch (clean + configure + build)
echo [2] Quick rebuild (incremental)
echo [3] Run interactive simulation
echo [4] Run GUI application (OpenGL window)
echo [5] Run at 1000x acceleration
echo [6] Run tests
echo [7] Run with awakening
echo [8] Clean build directory
echo [0] Exit
echo.

set "choice="
set /p "choice=Select option (1-9): "
if not defined choice goto menu

if "%choice%"=="1" goto build_clean
if "%choice%"=="2" goto build_quick
if "%choice%"=="3" goto run_interactive
if "%choice%"=="4" goto run_gui
if "%choice%"=="5" goto run_fast
if "%choice%"=="6" goto run_tests
if "%choice%"=="7" goto run_awakening
if "%choice%"=="8" goto clean
if "%choice%"=="0" goto exit_script

echo [ERROR] Invalid choice: %choice%
pause
goto menu

:build_clean
:: Check if generator was auto-detected
if not defined GENERATOR (
    echo [ERROR] Could not auto-detect Visual Studio version!
    echo Please run from Developer Command Prompt instead.
    pause
    exit /b 1
)

echo.
echo [INFO] Cleaning build directory...
if exist "%BUILD_DIR%" (
    rmdir /s /q "%BUILD_DIR%"
    if not "!errorlevel!"=="0" (
        echo [WARNING] Could not fully clean build directory (may be in use)
    )
)
mkdir "%BUILD_DIR%" 2>nul
cd "%BUILD_DIR%"

echo [INFO] Configuring CMake with %GENERATOR%...
cmake .. -G "%GENERATOR%" -A %PLATFORM% -DCMAKE_BUILD_TYPE=%BUILD_TYPE% -DBUILD_TESTS=ON
if errorlevel 1 (
    echo.
    echo [ERROR] CMake configuration failed!
    echo.
    cd ..
    pause
    exit /b 1
)

goto build

:build_quick
echo.
if not exist "%BUILD_DIR%" (
    echo [ERROR] Build directory not found. Please select option 1 first.
    pause
    goto menu
)
cd "%BUILD_DIR%"

goto build

:build
echo.
echo [INFO] Building APEIRON (this may take a few minutes)...
echo [INFO] Using generator: %GENERATOR%
cmake --build . --config %BUILD_TYPE% --parallel
if errorlevel 1 (
    echo.
    echo [ERROR] Build failed!
    echo.
    cd ..
    pause
    exit /b 1
)

echo.
echo [SUCCESS] Build completed successfully!
echo.
echo Executable location: %CD%\bin\%BUILD_TYPE%\apeiron-cli.exe
echo.
cd ..
goto menu_end

:run_interactive
echo.
echo [INFO] === DIAGNOSTIC INFO ===
echo [INFO] Working dir : %CD%
echo [INFO] BUILD_DIR   : %BUILD_DIR%
echo [INFO] BUILD_TYPE  : %BUILD_TYPE%
echo [INFO] Full path   : %CD%\%BUILD_DIR%\bin\%BUILD_TYPE%\apeiron-cli.exe
echo.

if not exist "%BUILD_DIR%\bin\%BUILD_TYPE%\apeiron-cli.exe" (
    echo [ERROR] Executable NOT FOUND at:
    echo         %CD%\%BUILD_DIR%\bin\%BUILD_TYPE%\apeiron-cli.exe
    echo.
    echo [INFO] Files in build\bin\Release\ :
    dir "%BUILD_DIR%\bin\%BUILD_TYPE%\" 2>nul || echo   (directory does not exist^)
    echo.
    echo [INFO] Please run option [1] to build first.
    pause
    goto menu
)

echo [INFO] Exe found. Starting simulation (10000 cycles, HIGH)...
echo [INFO] Press Ctrl+C to stop early
echo.
"%BUILD_DIR%\bin\%BUILD_TYPE%\apeiron-cli.exe" -c 10000 -s 2
echo.
echo [INFO] Exit code: %errorlevel%
echo [INFO] Simulation ended.
goto menu_end

:run_gui
echo.
echo [INFO] Starting GUI application...
echo [INFO] Press ESC to exit, F1 to pause/resume
echo.
if not exist "%BUILD_DIR%\bin\%BUILD_TYPE%\apeiron-app.exe" (
    echo [ERROR] GUI executable not found. Please build first (option 1).
    pause
    goto menu
)
"%BUILD_DIR%\bin\%BUILD_TYPE%\apeiron-app.exe"
echo.
echo [INFO] GUI application closed. Press any key to continue...
goto menu_end

:run_fast
echo.
echo [INFO] Running at HIGH (1Mx) acceleration (100000 cycles)...
echo.
if not exist "%BUILD_DIR%\bin\%BUILD_TYPE%\apeiron-cli.exe" (
    echo [ERROR] Executable not found. Please build first (option 1).
    pause
    goto menu
)
"%BUILD_DIR%\bin\%BUILD_TYPE%\apeiron-cli.exe" -c 100000 -s 2
echo.
echo [INFO] Run completed.
goto menu_end

:run_tests
echo.
echo [INFO] Running unit tests...
if not exist "%BUILD_DIR%\tests\%BUILD_TYPE%\apeiron-tests.exe" (
    echo [ERROR] Tests not found. Please build with tests enabled (option 1).
    pause
    goto menu
)
"%BUILD_DIR%\tests\%BUILD_TYPE%\apeiron-tests.exe"
echo.
if errorlevel 1 (
    echo [WARNING] Some tests failed
) else (
    echo [SUCCESS] All tests passed!
)
echo.
echo [INFO] Press any key to continue...
goto menu_end

:run_awakening
echo.
echo [INFO] Running awakening simulation (50000 cycles, HIGH acceleration)...
echo.
if not exist "%BUILD_DIR%\bin\%BUILD_TYPE%\apeiron-cli.exe" (
    echo [ERROR] Executable not found. Please build first (option 1).
    pause
    goto menu
)
"%BUILD_DIR%\bin\%BUILD_TYPE%\apeiron-cli.exe" -c 50000 -s 2 -a
echo.
echo [INFO] Simulation ended.
goto menu_end

:clean
echo.
echo [INFO] Cleaning build directory...
if exist "%BUILD_DIR%" (
    rmdir /s /q "%BUILD_DIR%"
    if "!errorlevel!"=="0" (
        echo [SUCCESS] Build directory cleaned
    ) else (
        echo [WARNING] Some files could not be deleted (may be in use)
    )
) else (
    echo [INFO] Nothing to clean
)
goto menu_end

:menu_end
echo.
pause
goto menu

:exit_script
echo.
echo [INFO] Exiting APEIRON Build System...
pause
exit /b 0
