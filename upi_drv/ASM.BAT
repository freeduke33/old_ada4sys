@echo off
tasm /ml/z/l %1.asm,bin\%1.obj
if exist bin\%1.obj tlink /t/x bin\%1.obj
