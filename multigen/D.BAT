@echo off
:l
copy /b qqq01.dat+qqq01.dat q
del qqq01.dat
ren q qqq01.dat
pause
goto l