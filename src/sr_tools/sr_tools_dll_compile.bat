@echo off
if exist sr_tools.obj del sr_tools.obj
if exist sr_tools.lib del sr_tools.lib
if exist sr_tools.dll del sr_tools.dll
\masm32\bin\ml /c /coff sr_tools.asm
\masm32\bin\Link /SUBSYSTEM:WINDOWS /DLL /DEF:sr_tools.def sr_tools.obj 
del sr_tools.obj sr_tools.exp sr_tools.lib
dir sr_tools.*
pause
