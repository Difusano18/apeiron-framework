@echo off
cd /d "%~dp0"

echo ============================================
echo  APEIRON DEBUG LAUNCHER
echo ============================================
echo.
echo [1] Робочий каталог : %CD%
echo [2] Шукаю exe...

set "EXE=build\bin\Release\apeiron-cli.exe"

if exist "%EXE%" (
    echo [OK] Exe знайдено: %EXE%
) else (
    echo [ПОМИЛКА] Exe НЕ знайдено за шляхом: %CD%\%EXE%
    echo.
    echo Вміст build\bin\Release\ :
    dir "build\bin\Release\" 2>nul || echo   ^(папка не існує^)
    echo.
    echo Вміст build\ :
    dir "build\" 2>nul || echo   ^(build папки немає взагалі^)
    echo.
    echo Запусти спочатку run.bat -^> опція [1] Build from scratch
    echo.
    pause
    exit /b 1
)

echo.
echo ============================================
echo  Запускаю симуляцію: 5000 циклів, HIGH
echo ============================================
echo.

"%EXE%" -c 5000 -s 2 -a

echo.
echo ============================================
echo  EXIT CODE: %errorlevel%
echo ============================================
echo.
pause
