@echo off
title SysLoader - Builder
color 0A

set "ROOT=%~dp0"
set "SRC=%ROOT%src"
set "BIN=%ROOT%bin\release"

if not exist "%BIN%" mkdir "%BIN%"

echo ============================================================
echo     SysLoader - Builder
echo ============================================================
echo.

:menu
echo [1] Build EXE
echo [2] Build DLL
echo [3] Build Both
echo [4] Clean
echo [5] Exit
echo.
set /p choice="Select option: "

if "%choice%"=="1" goto build_exe
if "%choice%"=="2" goto build_dll
if "%choice%"=="3" goto build_both
if "%choice%"=="4" goto clean
if "%choice%"=="5" exit
goto menu

:build_exe
echo.
echo [*] Building EXE...
g++ -O2 -std=c++17 -o "%BIN%\SysLoader.exe" "%SRC%\main.cpp" -lcomctl32 -ladvapi32 -lshlwapi -luser32 -lgdi32 -lws2_32 -lcomdlg32 -static
if %errorlevel% equ 0 (
    echo [OK] EXE built: bin\release\SysLoader.exe
) else (
    echo [X] EXE failed!
)
pause
goto menu

:build_dll
echo.
echo [*] Building DLL...
g++ -O2 -shared -std=c++17 -o "%BIN%\SysLoader.dll" "%SRC%\main.cpp" -D_DLL -lcomctl32 -ladvapi32 -lshlwapi -luser32 -lgdi32 -lws2_32 -lcomdlg32 -static
if %errorlevel% equ 0 (
    echo [OK] DLL built: bin\release\SysLoader.dll
    echo [*] Creating sys.ini...
    (echo [Settings] & echo Driver=MyDriver.sys) > "%BIN%\sys.ini"
    echo [OK] sys.ini created
) else (
    echo [X] DLL failed!
)
pause
goto menu

:build_both
echo.
echo [*] Building EXE and DLL...
call :build_exe
call :build_dll
pause
goto menu

:clean
echo.
echo [*] Cleaning...
if exist "%BIN%" (
    del /q "%BIN%\*.*" 2>nul
    echo [OK] Cleaned
)
pause
goto menu
