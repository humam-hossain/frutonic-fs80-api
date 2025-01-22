@echo off
setlocal

set SRC=src\main.c
set OUT=bin\main.exe
set DLL=bin\windows\amd64\libusb0.dll

gcc -o %OUT% %SRC% -Iinclude -Llib\windows\msvc_x64 -lusb

if errorlevel 1 (
    echo Error: Compilation failed
    exit /b 1
)

if exist %DLL% (
    copy /Y %DLL% bin\ > nul
) else (
    echo Warning: libusb0.dll not found
)

echo INFO: Build completed