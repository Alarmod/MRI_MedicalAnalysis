@echo off

cd /d %~dp0

@REM rigel.exe must be run with administrator rights

start /B rigel.exe -a kawpow --cclock 15 --lock-cclock 1920 --mclock 750 --lock-mclock 22000 --pl 170 -o [1]stratum+tcp://dummy.server.com:3112 -u X --no-tui --no-watchdog

TIMEOUT /T 15

wmic process where name="rigel.exe" terminate >nul 2> nul

exit
