rem @echo off
tasm /ml/z/l %1.asm,%1.obj
if exist %1.obj tlink /t/x %1.obj
