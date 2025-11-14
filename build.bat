@echo off
REM ---- Set MinGW path ----
SET PATH=C:\MinGW\bin;%PATH%

echo Current PATH: %PATH%
where g++
pause

echo Building CK + CKS interpreter...
g++ src/ck.c src/cks.c src/cck.c -o cck.exe

IF %ERRORLEVEL% NEQ 0 (
    echo Build failed!
    pause
    exit /b 1
)

echo Build successful!
pause
