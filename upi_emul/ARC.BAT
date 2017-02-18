@echo off
if exist *.bak del *.bak
if exist dsys\*.bak del dsys\*.bak
if exist upi_3w\*.bak del upi_3w\*.bak
if exist upi_3m\*.bak del upi_3m\*.bak
if exist upi_3m\*.bak del upi_pipe\*.bak
uc as emul_upi *.cpp *.hpp *.asm *.c *.h *.prj *.bat *.ww *.inc dirinfo makefile.

