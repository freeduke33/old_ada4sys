@echo off
if exist *.bak del *.bak
arj a -u grf *.c *.h *.prj font8x8.chr grf8x8.bin *.lib *.bat makefile.