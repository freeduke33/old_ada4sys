@echo off
if exist *.bak del *.bak
uc as upi_drv *.asm *.inc *.mac *.c *.h *.bat dirinfo *.txt *.dsp *.sys *.prj *.com *.hex
