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

copy /Y ..\..\..\rtcs\source\include\select.h .
copy /Y ..\..\..\rtcs\source\include\ftp.h .
copy /Y ..\..\..\rtcs\source\include\ftpc.h .
copy /Y ..\..\..\rtcs\source\include\ftpd.h .
copy /Y ..\..\..\rtcs\source\include\ftpd_msg.h .
copy /Y ..\..\..\rtcs\source\include\rtcsvers.h .
copy /Y ..\..\..\rtcs\source\include\rtcs.h .
copy /Y ..\..\..\rtcs\source\include\rtcsrtos.h .
copy /Y ..\..\..\rtcs\source\include\rtcstime.h .
copy /Y ..\..\..\rtcs\source\include\rtcscfg.h .
copy /Y ..\..\..\rtcs\source\include\rtcs_in.h .
copy /Y ..\..\..\rtcs\source\include\rtcshdlc.h .
copy /Y ..\..\..\rtcs\source\include\pppfsm.h .
copy /Y ..\..\..\rtcs\source\include\ppphdlc.h .
copy /Y ..\..\..\rtcs\source\include\ppp.h .
copy /Y ..\..\..\rtcs\source\include\dns.h .
copy /Y ..\..\..\rtcs\source\include\svc.h .
copy /Y ..\..\..\rtcs\source\include\svcauth.h .
copy /Y ..\..\..\rtcs\source\include\xdr.h .
copy /Y ..\..\..\rtcs\source\include\dhcp.h .
copy /Y ..\..\..\rtcs\source\include\rpctypes.h .
copy /Y ..\..\..\rtcs\source\include\rtcsrpc.h .
copy /Y ..\..\..\rtcs\source\include\rpcmsg.h .
copy /Y ..\..\..\rtcs\source\include\clnt.h .
copy /Y ..\..\..\rtcs\source\include\pmap.h .
copy /Y ..\..\..\rtcs\source\include\pmapif.h .
copy /Y ..\..\..\rtcs\source\include\auth.h .
rem copy /Y ..\..\..\rtcs\source\include\iodun.h .
copy /Y ..\..\..\rtcs\source\include\rtcs25x.h .
copy /Y ..\..\..\rtcs\source\include\rtcslog.h .
copy /Y ..\..\..\rtcs\source\include\nat.h .
copy /Y ..\..\..\rtcs\source\include\nat_session.h .
copy /Y ..\..\..\rtcs\source\include\ipcfg.h .
copy /Y ..\..\..\rtcs\source\include\iwcfg.h .

copy /Y ..\..\..\rtcs\source\httpd\httpd.h .
copy /Y ..\..\..\rtcs\source\httpd\httpd_types.h .
copy /Y ..\..\..\rtcs\source\httpd\httpd_wrapper.h .
copy /Y ..\..\..\rtcs\source\httpd\httpd_mqx.h .
copy /Y ..\..\..\rtcs\source\httpd\httpd_supp.h .
copy /Y ..\..\..\rtcs\source\httpd\httpd_cnfg.h .

copy /Y ..\..\..\rtcs\source\include\snmp.h .
copy /Y ..\..\..\rtcs\source\include\snmpcfg.h .
copy /Y ..\..\..\rtcs\source\include\asn1.h .
copy /Y ..\..\..\rtcs\source\include\rtcspcb.h .

copy /Y ..\..\..\rtcs\source\include\tcp.h .
copy /Y ..\..\..\rtcs\source\include\ip.h .

copy /Y ..\..\..\rtcs\source\include\telnet.h .

copy /Y ..\..\..\rtcs\source\include\tftp.h .

copy /Y ..\..\..\rtcs\source\include\ipc_udp.h .

rem  Compiler-specific files
goto comp_%compiler%

rem  Keil Compiler
:comp_keil
:comp_uv4
if /I "%target%" == "debug"   copy /Y ..\..\..\rtcs\build\uv4\%4_IntFlashDebug\rtcs_%4_d.lib .
if /I "%target%" == "release" copy /Y ..\..\..\rtcs\build\uv4\%4_IntFlashRelease\rtcs_%4.lib .
goto comp_done

rem Nothing to do for other compilers
:comp_cw
:comp_iar
:comp_ewarm
:comp_ewcf

:comp_done

echo RTCS Post-build operations done
