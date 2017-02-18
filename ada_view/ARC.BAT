@echo off
if exist *.bak del *.bak
if exist *.wbk del *.wbk
if exist *.err del *.err
arj a -u ada_view *.c *.h *.prj *.bat *.ww
