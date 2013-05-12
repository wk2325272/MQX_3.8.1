@echo off

set bat_file_dir=%~dp0

rem 1st parameter is target path (in the <mqx>/lib/<board>/<dir>)
rem or it may be also a name of the library file created by the linker

rem 2nd parameter is compiler name (cw, iar, ewarm, ewcf, keil, uv4), default is cw
set compiler=%~2
if /I "%compiler%" == "" set compiler=cw 

rem 3rd parameter is target name (debug, release) - so fas for Keil compiler only
set target=%~3

rem 4th parameter is board name - so far for Keil compiler only

rem Make sure we are in target directory  (create if needed)
call "%bat_file_dir%\verify_dir.bat" "%~f1"
if %ERRORLEVEL% NEQ 0 exit 1

attrib -R * >NUL 2>&1

rem  Common non-private header files

call "%bat_file_dir%\psp_cortex.bat" "%~f1" %2

rem  Configuration header files

for %%F in (..\..\..\config\common\*.h)     do copy /Y /B ..\..\..\mqx\source\include\dontchg.h + %%F ..\%%~nF%%~xF
for %%F in (..\..\..\config\twrk60n512\*.h)  do copy /Y /B ..\..\..\mqx\source\include\dontchg.h + %%F ..\%%~nF%%~xF

rem  Processor-specific PSP files

copy /Y ..\..\..\mqx\source\psp\cortex\kinetis.h  .
copy /Y ..\..\..\mqx\source\psp\cortex\reg_enet.h  .
copy /Y ..\..\..\mqx\source\psp\cortex\regfile.h  .
rem copy /Y ..\..\..\mqx\source\psp\cortex\MK60N512VMD100.h  .
copy /Y ..\..\..\mqx\source\psp\cortex\MK60DZ10.h  .
copy /Y ..\..\..\mqx\source\psp\cortex\kinetis_mpu.h  .
copy /Y ..\..\..\mqx\source\psp\cortex\kinetis_fcan.h  .

rem  Compiler-specific PSP files
goto comp_%compiler%

rem  CodeWarrior Compiler
:comp_cw
goto comp_done

rem  IAR Compiler
:comp_iar
:comp_ewarm
:comp_ewcf
goto comp_done

rem  Keil Compiler
:comp_keil
:comp_uv4
rem Keil is not able to build directly into lib folder. Need to copy
if /I "%target%" == "debug"   copy /Y ..\..\..\mqx\build\uv4\twrk60n512_IntFlashDebug\psp_twrk60n512_d.lib .
if /I "%target%" == "release" copy /Y ..\..\..\mqx\build\uv4\twrk60n512_IntFlashRelease\psp_twrk60n512.lib .
goto comp_done

rem  End of Compiler-specific files
:comp_done

echo PSP Post-build operations done
