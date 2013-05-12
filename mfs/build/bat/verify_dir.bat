@echo off

rem Without any argument, do nothing and assume the directory is already set
rem (this is how CodeWarrior default projects are set)
if "%~1" == "" goto nothing

rem Argument as full path should exist
if EXIST "%~f1" goto path_exists

rem Assuming the parameter is the path, try to create it
mkdir "%~1"

rem Try again
if NOT EXIST "%~f1" goto error

:path_exists

rem May be either directory... 
set ATR=%~a1
if %ATR:~0,1% == d goto isdir

rem ...Or file, with valid parent directory
if NOT EXIST "%~dp1" goto error

rem Argument was file path, set current working drive and change directory
echo Setting working directory to %~dp1
%~d1
cd %~dp1
if %ERRORLEVEL% NEQ 0 goto errcd
goto done

rem Argumment was directory, set current working drive and change directory
:isdir
echo Setting working directory to %~f1
%~d1
cd %~f1
if %ERRORLEVEL% NEQ 0 goto errcd
goto done

:nothing
echo Assuming working directory already set by caller

:done
exit /B 0

:error
echo Invalid path argument (should be a path to linker output file or directory)
exit /B 2

:errcd
echo Can not change directory (probably invalid path specified)
exit /B 4
