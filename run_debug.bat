@echo off
cd /d "%~dp0"

echo ============================================
echo  APEIRON DEBUG LAUNCHER
echo ============================================
echo.
echo [1] Working dir : %CD%
echo [2] Looking for exe...

set "EXE=build\bin\Release\apeiron-cli.exe"

if exist "%EXE%" (
    echo [OK] Found: %EXE%
) else (
    echo [ERROR] NOT FOUND: %CD%\%EXE%
    echo.
    echo Contents of build\bin\Release\ :
    dir "build\bin\Release\" 2>nul || echo   directory does not exist
    echo.
    echo Contents of build\ :
    dir "build\" 2>nul || echo   build dir missing - run option 1 first
    echo.
    pause
    exit /b 1
)

echo.
echo ============================================
echo  Running: 5000 cycles, HIGH speed, awakening
echo ============================================
echo.

"%EXE%" -c 5000 -s 2 -a

echo.
echo ============================================
echo  EXIT CODE: %errorlevel%
echo ============================================
echo.
pause
