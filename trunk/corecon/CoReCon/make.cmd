@echo off
setlocal

if ~%1 == ~flake goto p1ok
if ~%1 == ~flake2 goto p1ok
if ~%1 == ~hoof goto p1ok
if ~%1 == ~paw goto p1ok
if ~%1 == ~petal goto p1ok
if ~%1 == ~squid goto p1ok
if ~%1 == ~cleanup goto cleanup

goto usage

:p1ok

if ~%2 == ~ goto p2ok
if ~%2 == ~all goto p2ok
if ~%2 == ~clean goto p2ok

:p2ok

if ~%1 == ~flake set MODEL=FrostyFlake
if ~%1 == ~flake2 set MODEL=FrostyFlake2
if ~%1 == ~hoof set MODEL=PegasusHoof
if ~%1 == ~paw set MODEL=KittenPaw
if ~%1 == ~petal set MODEL=BlackPetal
if ~%1 == ~squid set MODEL=GhostSquid

copy Makefile bin >NUL

cd bin

set path=C:\Tools\avrgcc\bin;%path%

if not ~%2 == ~clean echo.
if not ~%2 == ~clean C:\Tools\avrgcc\bin\make clean
echo.
C:\Tools\avrgcc\bin\make %2

goto bye

:cleanup

del /Q bin\dep\*
del /Q bin\*

goto bye

:usage

echo.
echo. Syntax to build the firmware for Costar Replacement Controllers
echo. by Bathroom Epiphanies Inc.:
echo.
echo. make {flake^|flake2^|hoof^|paw^|petal^|squid} [all^|clean]

:bye
echo.
echo. Done.
