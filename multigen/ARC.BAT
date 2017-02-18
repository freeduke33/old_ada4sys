@echo off
if exist *.bak del *.bak
uc as multigen *.cpp *.hpp *.bat *.h *.c *.ww makefile *.prj
