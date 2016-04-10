#!/bin/sh
# Written for g++ 4.9.2 (Debian 4.9.2-10)
# Written for Debian clang version 3.5.0-10 (tags/RELEASE_350/final) (based on LLVM 3.5.0)

#=== Warning
echo Please edit the options and disable that check.
exit

#=== Global variables
compiler=clang++
wxgtk_path=/home/your_account/wx_folder/buildgtk/
if [ $compiler = "g++" ]
then
	cpp_options="-s -O3 -Os -DNDEBUG -DwxDEBUG_LEVEL=0 -finput-charset=iso-8859-15 -march=i586 -std=gnu++11"
else
	cpp_options="-O3 -Os -DNDEBUG -DwxDEBUG_LEVEL=0"
fi
#For the console add: -DwxUSE_GUI=0 -D_CONSOLE
cpp_cmdline=`${wxgtk_path}wx-config --libs std,stc --cxxflags`
link_exe=scripted_roulette.elf

#=== Delete the output file
rm -f bin/$link_exe

#=== Compilation of the sources
$compiler src/wxEcMath/ec_engine.cpp -o bin/ec_engine.obj   $cpp_cmdline -c $cpp_options
$compiler src/wxEcMath/ec_plot.cpp   -o bin/ec_plot.obj     $cpp_cmdline -c $cpp_options
$compiler src/engine.cpp             -o bin/engine.obj      $cpp_cmdline -c $cpp_options
$compiler src/http_prng.cpp          -o bin/http_prng.obj   $cpp_cmdline -c $cpp_options
$compiler src/random.cpp             -o bin/random.obj      $cpp_cmdline -c $cpp_options
$compiler src/instruction.cpp        -o bin/instruction.obj $cpp_cmdline -c $cpp_options
$compiler src/sequence.cpp           -o bin/sequence.obj    $cpp_cmdline -c $cpp_options
$compiler src/debugger.cpp           -o bin/debugger.obj    $cpp_cmdline -c $cpp_options
$compiler src/roulette.cpp           -o bin/roulette.obj    $cpp_cmdline -c $cpp_options
$compiler src/table.cpp              -o bin/table.obj       $cpp_cmdline -c $cpp_options
$compiler src/frame.cpp              -o bin/frame.obj       $cpp_cmdline -c $cpp_options
$compiler src/main.cpp               -o bin/main.obj        $cpp_cmdline -c $cpp_options

#=== Link of the object files
$compiler bin/*.obj -o bin/$link_exe $cpp_cmdline $cpp_options -I ${wxgtk_path}lib
if [ -f bin/$link_exe ]
then
	rm -f bin/*.obj
else
	exit
fi

#=== Final step
strip bin/$link_exe --remove-section=.comment --remove-section=.note
