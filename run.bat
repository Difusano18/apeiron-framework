@echo off
chcp 65001 > nul
setlocal EnableDelayedExpansion

:: APEIRON Framework Build and Run Script
:: Phase 1-2: Foundation + Hyper-acceleration

title APEIRON Build System

:: Configuration
set "BUILD_DIR=build"
set "BUILD_TYPE=Release"
set "GENERATOR=Ninja"

:: Check for CMake
where cmake > nul 2> nul
if %errorlevel% neq 0 (
    echo [ERROR] CMake not found in PATH
    echo Please install CMake 3.25+ from https://cmake.org/download/
    pause
    exit /b 1
)

:: Check for compiler
where cl > nul 2> nul
if %errorlevel% neq 0 (
    echo [WARNING] MSVC compiler not found
    echo Trying to find Visual Studio...

    :: Try to find VS 2022
    if exist "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat" (
        call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
    ) else if exist "C:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\Build\vcvars64.bat" (
        call "C:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\Build\vcvars64.bat"
    ) else if exist "C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build\vcvars64.bat" (
        call "C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build\vcvars64.bat"
    ) else (
        echo [ERROR] Visual Studio 2022 not found
        echo Please install Visual Studio with C++ support
        pause
        exit /b 1
    )
)

echo.
echo ╔═══════════════════════════════════════════════════════════════╗
echo ║                    APEIRON-Ω v0.2.0                           ║
echo ║              Build and Run Script (Windows)                  ║
echo ╚═══════════════════════════════════════════════════════════════╝
echo.

:: Menu
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

set /p choice="Select option (1-9): "

if "%choice%"=="1" goto build_clean
if "%choice%"=="2" goto build_quick
if "%choice%"=="3" goto run_interactive
if "%choice%"=="4" goto run_sphere
if "%choice%"=="5" goto run_fast
if "%choice%"=="6" goto run_tests
if "%choice%"=="7" goto run_awakening
if "%choice%"=="8" goto clean
if "%choice%"=="9" exit /b 0

echo [ERROR] Invalid choice
pause
exit /b 1

:build_clean
echo.
echo [INFO] Cleaning build directory...
if exist "%BUILD_DIR%" rmdir /s /q "%BUILD_DIR%"
mkdir "%BUILD_DIR%"
cd "%BUILD_DIR%"

echo [INFO] Configuring CMake...
cmake .. -G "%GENERATOR%" -DCMAKE_BUILD_TYPE=%BUILD_TYPE% -DBUILD_TESTS=ON
if %errorlevel% neq 0 (
    echo [ERROR] CMake configuration failed
    pause
    exit /b 1
)

goto build

:build_quick
echo.
if not exist "%BUILD_DIR%" (
    echo [ERROR] Build directory not found. Please select option 1 first.
    pause
    exit /b 1
)
cd "%BUILD_DIR%"

goto build

:build
echo [INFO] Building APEIRON...
cmake --build . --parallel --config %BUILD_TYPE%
if %errorlevel% neq 0 (
    echo [ERROR] Build failed
    pause
    exit /b 1
)

echo.
echo [SUCCESS] Build completed!
echo.
cd ..
goto menu_end

:run_interactive
echo.
echo [INFO] Starting interactive simulation...
echo [INFO] Use P=pause, Q=quit, +=faster, -=slower
echo.
cd "%BUILD_DIR%"
if not exist "bin\apeiron-cli.exe" (
    echo [ERROR] Executable not found. Please build first.
    pause
    exit /b 1
)
.\bin\apeiron-cli.exe -i -s 1000
cd ..
goto menu_end

:run_sphere
echo.
echo [INFO] Starting sphere animation...
echo.
cd "%BUILD_DIR%"
if not exist "bin\apeiron-cli.exe" (
    echo [ERROR] Executable not found. Please build first.
    pause
    exit /b 1
)
.\bin\apeiron-cli.exe --sphere
cd ..
goto menu_end

:run_fast
echo.
echo [INFO] Running at 1000x acceleration (100000 cycles)...
cd "%BUILD_DIR%"
if not exist "bin\apeiron-cli.exe" (
    echo [ERROR] Executable not found. Please build first.
    pause
    exit /b 1
)
.\bin\apeiron-cli.exe -c 100000 -s 1000 -a
cd ..
goto menu_end

:run_tests
echo.
echo [INFO] Running unit tests...
cd "%BUILD_DIR%"
if not exist "tests\apeiron-tests.exe" (
    echo [ERROR] Tests not found. Please build with -DBUILD_TESTS=ON
    pause
    exit /b 1
)
.\tests\apeiron-tests.exe
echo.
if %errorlevel% neq 0 (
    echo [ERROR] Tests failed
) else (
    echo [SUCCESS] All tests passed!
)
cd ..
goto menu_end

:run_awakening
echo.
echo [INFO] Running awakening simulation (1000 cycles)...
cd "%BUILD_DIR%"
if not exist "bin\apeiron-cli.exe" (
    echo [ERROR] Executable not found. Please build first.
    pause
    exit /b 1
)
.\bin\apeiron-cli.exe -c 1000 -a -s 100
cd ..
goto menu_end

:clean
echo.
echo [INFO] Cleaning build directory...
if exist "%BUILD_DIR%" (
    rmdir /s /q "%BUILD_DIR%"
    echo [SUCCESS] Build directory cleaned
) else (
    echo [INFO] Nothing to clean
)
goto menu_end

:menu_end
echo.
pause
goto :eof
