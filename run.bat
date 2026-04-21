@echo off
setlocal EnableDelayedExpansion

:: APEIRON Framework Build and Run Script
:: Phase 1-2: Foundation + Hyper-acceleration

title APEIRON Build System

:: Configuration
set "BUILD_DIR=build"
set "BUILD_TYPE=Release"
set "GENERATOR=Visual Studio 17 2022"
set "PLATFORM=x64"

:: Check for CMake
where cmake > nul 2> nul
if %errorlevel% neq 0 (
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

:: Check for Visual Studio
if not defined VSINSTALLDIR (
    echo [INFO] Trying to find Visual Studio 2022...

    if exist "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat" (
        call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
    ) else if exist "C:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\Build\vcvars64.bat" (
        call "C:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\Build\vcvars64.bat"
    ) else if exist "C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build\vcvars64.bat" (
        call "C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build\vcvars64.bat"
    ) else if exist "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvars64.bat" (
        call "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
    ) else (
        echo [ERROR] Visual Studio 2022 not found!
        echo Please install Visual Studio 2022 with "Desktop development with C++" workload
        echo Or run this script from "Developer Command Prompt for VS 2022"
        pause
        exit /b 1
    )
)

echo [INFO] Visual Studio environment loaded

:: Menu
:menu
echo.
echo [1] Build from scratch (clean + configure + build)
echo [2] Quick rebuild (incremental)
echo [3] Run interactive simulation
echo [4] Run sphere animation
echo [5] Run at 1000x acceleration
echo [6] Run tests
echo [7] Run with awakening
echo [8] Clean build directory
echo [9] Exit
echo.

set /p "choice=Select option (1-9): "

if "%choice%"=="1" goto build_clean
if "%choice%"=="2" goto build_quick
if "%choice%"=="3" goto run_interactive
if "%choice%"=="4" goto run_sphere
if "%choice%"=="5" goto run_fast
if "%choice%"=="6" goto run_tests
if "%choice%"=="7" goto run_awakening
if "%choice%"=="8" goto clean
if "%choice%"=="9" exit /b 0

echo [ERROR] Invalid choice: %choice%
pause
goto menu

:build_clean
echo.
echo [INFO] Cleaning build directory...
if exist "%BUILD_DIR%" (
    rmdir /s /q "%BUILD_DIR%"
    if %errorlevel% neq 0 (
        echo [WARNING] Could not fully clean build directory
    )
)
mkdir "%BUILD_DIR%" 2>nul
cd "%BUILD_DIR%"

echo [INFO] Configuring CMake with Visual Studio 2022...
echo cmake .. -G "%GENERATOR%" -A %PLATFORM% -DCMAKE_BUILD_TYPE=%BUILD_TYPE% -DBUILD_TESTS=ON
cmake .. -G "%GENERATOR%" -A %PLATFORM% -DCMAKE_BUILD_TYPE=%BUILD_TYPE% -DBUILD_TESTS=ON
if %errorlevel% neq 0 (
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
cmake --build . --config %BUILD_TYPE% --parallel
if %errorlevel% neq 0 (
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
echo [INFO] Starting interactive simulation...
echo [INFO] This will show real-time consciousness state
echo.
if not exist "%BUILD_DIR%\bin\%BUILD_TYPE%\apeiron-cli.exe" (
    echo [ERROR] Executable not found. Please build first (option 1).
    pause
    goto menu
)
cd "%BUILD_DIR%"
.\bin\%BUILD_TYPE%\apeiron-cli.exe -i -s 1000
cd ..
goto menu_end

:run_sphere
echo.
echo [INFO] Starting sphere animation...
echo.
if not exist "%BUILD_DIR%\bin\%BUILD_TYPE%\apeiron-cli.exe" (
    echo [ERROR] Executable not found. Please build first (option 1).
    pause
    goto menu
)
cd "%BUILD_DIR%"
.\bin\%BUILD_TYPE%\apeiron-cli.exe --sphere
cd ..
goto menu_end

:run_fast
echo.
echo [INFO] Running at 1000x acceleration (100000 cycles)...
cd "%BUILD_DIR%"
if not exist "bin\%BUILD_TYPE%\apeiron-cli.exe" (
    echo [ERROR] Executable not found. Please build first (option 1).
    pause
    goto menu
)
.\bin\%BUILD_TYPE%\apeiron-cli.exe -c 100000 -s 1000 -a
cd ..
goto menu_end

:run_tests
echo.
echo [INFO] Running unit tests...
cd "%BUILD_DIR%"
if not exist "tests\%BUILD_TYPE%\apeiron-tests.exe" (
    echo [ERROR] Tests not found. Please build with tests enabled (option 1).
    pause
    cd ..
    goto menu
)
.\tests\%BUILD_TYPE%\apeiron-tests.exe
echo.
if %errorlevel% neq 0 (
    echo [WARNING] Some tests failed
) else (
    echo [SUCCESS] All tests passed!
)
cd ..
goto menu_end

:run_awakening
echo.
echo [INFO] Running awakening simulation (1000 cycles)...
cd "%BUILD_DIR%"
if not exist "bin\%BUILD_TYPE%\apeiron-cli.exe" (
    echo [ERROR] Executable not found. Please build first (option 1).
    pause
    cd ..
    goto menu
)
.\bin\%BUILD_TYPE%\apeiron-cli.exe -c 1000 -a -s 100
cd ..
goto menu_end

:clean
echo.
echo [INFO] Cleaning build directory...
if exist "%BUILD_DIR%" (
    rmdir /s /q "%BUILD_DIR%"
    if %errorlevel% equ 0 (
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
