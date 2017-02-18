@echo off
set path=F:\ADI_DSP\21XX\BIN
SET ADI_DSP=F:\ADI_DSP\
SET ADII=F:\ADI_DSP\21xx\INCLUDE\

bld21 upi_bios
asm21 -i upi_bios
if not exist upi_bios.obj goto done
ld21 upi_bios -a upi_bios -g -e upi_bios -lib
if not exist upi_bios.exe goto done
copy upi_bios.exe upi_bios.bin >nul
goto end
del *.cde
del *.int
del *.obj
del *.sym
del *.ach
del *.dsd
:end
del upi_bios.exe >nul
:done
