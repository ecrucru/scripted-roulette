@echo off
if exist sr_tools.obj del sr_tools.obj
REM No /coff for BCC
\masm32\bin\ml /c sr_tools.asm
dir sr_tools.*
pause
