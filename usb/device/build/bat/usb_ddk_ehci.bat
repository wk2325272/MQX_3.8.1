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

rem  Remove old headers

attrib -R * >NUL 2>&1
if exist *.h  del /F /Q *.h

rem Copy common header files
call "%bat_file_dir%\usb_ddk.bat"

rem  Copy HS-specific header files

copy /Y ..\..\..\..\usb\device\source\include\ehci\ehci_usbprv.h .
copy /Y ..\..\..\..\usb\device\source\include\ehci\ehci_dev_main.h .

rem  Compiler-specific files
goto comp_%compiler%

rem  Keil Compiler
:comp_keil
:comp_uv4
if /I "%target%" == "debug"   copy /Y ..\..\..\..\usb\device\build\uv4\%4_IntFlashDebug\usb_ddk_%4_d.lib .
if /I "%target%" == "release" copy /Y ..\..\..\..\usb\device\build\uv4\%4_IntFlashRelease\usb_ddk_%4.lib .
goto comp_done

rem Nothing to do for other compilers
:comp_cw
:comp_iar
:comp_ewarm
:comp_ewcf

:comp_done

echo USB_DDK Post-build operations done
