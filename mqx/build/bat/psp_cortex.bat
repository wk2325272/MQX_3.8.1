@echo off

rem 1st parameter is target path (in the <mqx>/lib/<board>/<dir>)
rem 2nd parameter is compiler name (cw, iar, ewarm, ewcf, keil, uv4), default is cw
set compilerx=%~2
if /I "%compilerx%" == "" set compilerx=cw 

attrib -R * >NUL 2>&1

rem  Cortex specific files

copy /Y ..\..\..\mqx\source\psp\cortex\psp.h  .
copy /Y ..\..\..\mqx\source\psp\cortex\psptypes.h  .
copy /Y ..\..\..\mqx\source\psp\cortex\psp_cpu.h  .
copy /Y ..\..\..\mqx\source\psp\cortex\psp_rev.h  .
copy /Y ..\..\..\mqx\source\psp\cortex\psp_time.h  .
copy /Y ..\..\..\mqx\source\psp\cortex\psp_math.h  .
copy /Y ..\..\..\mqx\source\psp\cortex\psp_comp.h  .

copy /Y ..\..\..\mqx\source\psp\cortex\cortex.h  .

copy /Y ..\..\..\mqx\source\psp\cortex\psp_cpudef.h  .

copy /Y ..\..\..\mqx\source\psp\cortex\psp_supp.h  .

copy /Y ..\..\..\mqx\source\include\eds.h  .
copy /Y ..\..\..\mqx\source\include\edserial.h  .
copy /Y ..\..\..\mqx\source\include\event.h  .
copy /Y ..\..\..\mqx\source\include\fio.h  .
copy /Y ..\..\..\mqx\source\include\gen_rev.h  .
copy /Y ..\..\..\mqx\source\include\io.h  .
copy /Y ..\..\..\mqx\source\include\io_rev.h  .
copy /Y ..\..\..\mqx\source\include\ipc.h  .
copy /Y ..\..\..\mqx\source\include\ipc_pcb.h  .
copy /Y ..\..\..\mqx\source\include\klog.h  .
copy /Y ..\..\..\mqx\source\include\log.h  .
copy /Y ..\..\..\mqx\source\include\lwevent.h  .
copy /Y ..\..\..\mqx\source\include\lwlog.h  .
copy /Y ..\..\..\mqx\source\include\lwmem.h  .
copy /Y ..\..\..\mqx\source\include\lwmsgq.h  .
copy /Y ..\..\..\mqx\source\include\lwtimer.h  .
copy /Y ..\..\..\mqx\source\include\message.h  .
copy /Y ..\..\..\mqx\source\include\mqx.h  .
copy /Y ..\..\..\mqx\source\include\mqx_cpudef.h  .
copy /Y ..\..\..\mqx\source\include\mqx_cnfg.h .
copy /Y ..\..\..\mqx\source\include\mqx_ioc.h  .
copy /Y ..\..\..\mqx\source\include\mqx_str.h  .
copy /Y ..\..\..\mqx\source\include\mqx_macros.h  .
copy /Y ..\..\..\mqx\source\include\mutex.h  .
copy /Y ..\..\..\mqx\source\include\name.h  .
copy /Y ..\..\..\mqx\source\include\part.h  .
copy /Y ..\..\..\mqx\source\include\pcb.h  .
copy /Y ..\..\..\mqx\source\include\posix.h  .
copy /Y ..\..\..\mqx\source\include\sem.h  .
copy /Y ..\..\..\mqx\source\include\timer.h  .
copy /Y ..\..\..\mqx\source\include\watchdog.h .
copy /Y ..\..\..\mqx\source\include\ioctl.h .

rem Required by RTCS
copy /Y ..\..\..\mqx\source\io\serial\serial.h .

rem Required by SHELL
copy /Y ..\..\..\mqx\source\tad\tad.h .

rem  Compiler-specific PSP files
goto comp_%compilerx%

rem  CodeWarrior Compiler
:comp_cw
copy /Y ..\..\..\mqx\source\psp\cortex\cw_comp.h  .
goto comp_done

rem  IAR EWARM Compiler
:comp_iar
:comp_ewarm
:comp_ewcf
copy /Y ..\..\..\mqx\source\psp\cortex\iar_comp.h  .
goto comp_done

rem  Keil uv4 Compiler
:comp_keil
:comp_uv4
copy /Y ..\..\..\mqx\source\psp\cortex\keil_comp.h  .
goto comp_done

rem  End of Compiler-specific files

:comp_done

