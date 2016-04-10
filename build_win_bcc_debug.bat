echo Please check the batch file first.
pause
exit


REM ===== Options
set cpp_opt=-3 -P -RT -c -tW -v -R -d -n"../bin" -u -w -y
set link_opt=-C -E50 -r -c -v -Gn -s -aa -L"../src/sr_tools"
set link_windows=kernel32.lib
set link_wx=wxmsw28_core.lib wxbase28.lib wxmsw28_adv.lib
set link_sr=ec_engine.obj ec_plot.obj sr_tools.lib engine.obj http_prng.obj random.obj instruction.obj sequence.obj debugger.obj roulette.obj table.obj frame.obj main.obj
set link_stc=stclib_AutoComplete.obj stclib_CallTip.obj stclib_CellBuffer.obj stclib_CharClassify.obj stclib_ContractionState.obj stclib_Document.obj stclib_DocumentAccessor.obj stclib_Editor.obj stclib_ExternalLexer.obj stclib_Indicator.obj stclib_KeyMap.obj stclib_KeyWords.obj stclib_LexAda.obj stclib_LexAPDL.obj stclib_LexAsm.obj stclib_LexAsn1.obj stclib_LexAU3.obj stclib_LexAVE.obj stclib_LexBaan.obj stclib_LexBash.obj stclib_LexBasic.obj stclib_LexBullant.obj stclib_LexCaml.obj stclib_LexCLW.obj stclib_LexConf.obj stclib_LexCPP.obj stclib_LexCrontab.obj stclib_LexCsound.obj stclib_LexCSS.obj stclib_LexEiffel.obj stclib_LexErlang.obj stclib_LexEScript.obj stclib_LexFlagship.obj stclib_LexForth.obj stclib_LexFortran.obj stclib_LexGui4Cli.obj stclib_LexHaskell.obj stclib_LexHTML.obj stclib_LexInno.obj stclib_LexKix.obj stclib_LexLisp.obj stclib_LexLout.obj stclib_LexLua.obj stclib_LexMatlab.obj stclib_LexMetapost.obj stclib_LexMMIXAL.obj stclib_LexMPT.obj stclib_LexMSSQL.obj stclib_LexNsis.obj stclib_LexOpal.obj stclib_LexOthers.obj stclib_LexPascal.obj stclib_LexPB.obj stclib_LexPerl.obj stclib_LexPOV.obj stclib_LexPS.obj stclib_LexPython.obj stclib_LexRebol.obj stclib_LexRuby.obj stclib_LexScriptol.obj stclib_LexSmalltalk.obj stclib_LexSpecman.obj stclib_LexSpice.obj stclib_LexSQL.obj stclib_LexTADS3.obj stclib_LexTCL.obj stclib_LexTeX.obj stclib_LexVB.obj stclib_LexVerilog.obj stclib_LexVHDL.obj stclib_LexYAML.obj stclib_LineMarker.obj stclib_PlatWX.obj stclib_PropSet.obj stclib_RESearch.obj stclib_ScintillaBase.obj stclib_ScintillaWX.obj stclib_stc.obj stclib_Style.obj stclib_StyleContext.obj stclib_UniConversion.obj stclib_ViewStyle.obj stclib_WindowAccessor.obj stclib_XPM.obj
set link_extra=c0w32.obj import32.lib cw32.lib
set output_file=scripted_roulette.exe


REM ==== Preparation
cls
if not exist "bin\" mkdir "bin"
if exist "bin\%output_file%" del "bin\%output_file%"


REM ===== Compile resources
brc32 -r -32 -fo "bin\res.res" "src\res\res.rc"


REM ===== Compile wxEcMath & Scripted Roulette
cd src
bcc32 %cpp_opt%         -o"..\bin\ec_engine.obj"      wxEcMath\ec_engine.cpp
bcc32 %cpp_opt% -w-8071 -o"..\bin\ec_plot.obj"        wxEcMath\ec_plot.cpp
bcc32 %cpp_opt%         -o"..\bin\engine.obj"         engine.cpp
bcc32 %cpp_opt%         -o"..\bin\http_prng.obj"      http_prng.cpp
bcc32 %cpp_opt% -w-8004 -o"..\bin\random.obj"         random.cpp
bcc32 %cpp_opt%         -o"..\bin\instruction.obj"    instruction.cpp
bcc32 %cpp_opt%         -o"..\bin\sequence.obj"       sequence.cpp
bcc32 %cpp_opt%         -o"..\bin\debugger.obj"       debugger.cpp
bcc32 %cpp_opt%         -o"..\bin\roulette.obj"       roulette.cpp
bcc32 %cpp_opt% -w-8057 -o"..\bin\table.obj"          table.cpp
bcc32 %cpp_opt%         -o"..\bin\frame.obj"          frame.cpp
bcc32 %cpp_opt%         -o"..\bin\main.obj"           main.cpp


REM ===== Link
cd ../bin
if not exist sr_tools.dll copy /B "..\src\sr_tools\sr_tools.dll" sr_tools.dll
ilink32 %link_opt% %link_sr%, %output_file%, ,%link_windows% %link_wx% %link_stc% %link_extra%,, res.res


REM ===== Clean-up
del *.obj res.res


REM ===== Final
cd ..
if exist bin\%output_file% (
    echo The compilation is complete. Congratulations !
) else (
    echo The compilation failed. Please fix the errors and warnings...
)
pause
