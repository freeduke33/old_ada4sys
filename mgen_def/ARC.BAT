@echo off
if exist *.bak del *.bak
if exist *.err del *.err
if exist *.wbk del *.wbk
if exist *.pbk del *.pbk
arj a -u mgen_def *.c *.h *.prj *.bat *.ww
