@echo off
hstart "UPI's Emulator" /B /WIN /C %VAZROOT%\emul\upi_pipe\upi_pipe.exe %VAZROOT%\emul\upi_pipe\cfg >nul
..\upi\bin\vdm_pipe /i