@echo off
xcopy /m graph\ada_graf.exe *.ovl    >nul
xcopy /m run\wbin\ada_loop.exe *.ovl >nul
%1 %2 %3 %4 %5 bin\ada_4sys