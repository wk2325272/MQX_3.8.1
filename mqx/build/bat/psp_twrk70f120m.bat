@echo off

set bat_file_dir=%~dp0

rem 2nd parameter is compiler name (cw, iar, keil), default is cw
set compiler=%~2
if /I "%compiler%" == "" set compiler=cw 

rem 3rd parameter is target name (debug, release)
set target=%~3


rem make sure we are in target directory (create if needed)
call "%bat_file_dir%/verify_dir.bat" "%~f1"
if %ERRORLEVEL% NEQ 0 exit 1

rem  Common non-private header files

call "%bat_file_dir%\psp_cortex.bat" "%~f1" %2

rem  Configuration header files

for %%F in (..\..\..\config\common\*.h)     do copy /Y /B ..\..\..\mqx\source\include\dontchg.h + %%F ..\%%~nF%%~xF
for %%F in (..\..\..\config\twrk70f120m\*.h)  do copy /Y /B ..\..\..\mqx\source\include\dontchg.h + %%F ..\%%~nF%%~xF

rem  Processor-specific PSP files

copy /Y ..\..\..\mqx\source\psp\cortex\kinetis.h  .
copy /Y ..\..\..\mqx\source\psp\cortex\reg_enet.h  .
copy /Y ..\..\..\mqx\source\psp\cortex\regfile.h  .
copy /Y ..\..\..\mqx\source\psp\cortex\MK70F12.h  .
copy /Y ..\..\..\mqx\source\psp\cortex\kinetis_mpu.h  .
copy /Y ..\..\..\mqx\source\psp\cortex\kinetis_fcan.h  .

rem  Compiler-specific PSP files
goto comp_%compiler%

rem  CodeWarrior Compiler
:comp_cw
goto comp_done

rem  IAR Compiler
:comp_iar
goto comp_done

rem  Keil Compiler
:comp_keil
:comp_uv4
rem TODO: Keil is not able to build directly into lib folder. Need to copy
if /I "%target%" == "debug"   copy /Y ..\..\..\mqx\build\uv4\twrk70f120m_IntFlashDebug\psp_twrk70f120m_d.lib .
if /I "%target%" == "release" copy /Y ..\..\..\mqx\build\uv4\twrk70f120m_IntFlashRelease\psp_twrk70f120m.lib .
goto comp_done

rem  End of Compiler-specific files
:comp_done

echo PSP Post-build operations done
