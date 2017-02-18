@echo off
if exist *.bak del *.bak
if exist *.swp del *.swp
if exist *.wbk del *.wbk
if exist *.pbk del *.pbk
if exist *.err del *.err

uc as ada_4sys *.c *.h *.ww *.bat *.cpp *.hpp *.prj *.txt *.hlp *. *.inc
