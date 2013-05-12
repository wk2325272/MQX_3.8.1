@ECHO OFF

IF "%~1"=="" GOTO HelpScreen 

:Install
echo  **** Installing MQX plug-ins for CodeWarrior 10.2 ***
echo  **** It can take up to 60 seconds ***  

REM this is to support CW 10.1 plug-ins installed into CW 10.2
"%~1/eclipse/cwidec.exe" -nosplash -application org.eclipse.equinox.p2.director -repository file:"%CD%/mqx_p2_repository/" -uninstallIU com.freescale.mqx.feature.group > NUL 2>&1

if exist "%~1/MCU/lib/wizard_data/mqx/3.8" (
    echo **** Uninstalling old plug-ins for MQX 3.8 ****
    "%~1/eclipse/cwidec.exe" -nosplash -application org.eclipse.equinox.p2.director -repository file:"%CD%/mqx_p2_repository/" -uninstallIU com.freescale.mqx.v3.8.feature.group > NUL 2>&1
)
echo **** Installing plug-ins for MQX 3.8 ****
"%~1/eclipse/cwidec.exe" -nosplash -application org.eclipse.equinox.p2.director -repository file:"%CD%/mqx_p2_repository/" -installIU com.freescale.mqx.v3.8.feature.group

if exist "%~1/MCU/bin/plugins/Debugger/rtos/mqx/tools/tad/mqx.tad" (
    echo **** Uninstalling old plug-ins for MQX - all versions ****
    "%~1/eclipse/cwidec.exe" -nosplash -application org.eclipse.equinox.p2.director -repository file:"%CD%/mqx_p2_repository/" -uninstallIU com.freescale.mqx.common.feature.group
)
echo **** Installing plug-ins for MQX - all versions ****
"%~1/eclipse/cwidec.exe" -nosplash -application org.eclipse.equinox.p2.director -repository file:"%CD%/mqx_p2_repository/" -installIU com.freescale.mqx.common.feature.group

REM Workaround for morpho plug-in problem
DEL /F /Q "%~1\eclipse\plugins\com.freescale.morpho.core_7.0.0.106089-201110132018\xpcom\bin\components\compreg.dat" > NUL 2>&1
DEL /F /Q "%~1\eclipse\plugins\com.freescale.morpho.core_7.0.0.106089-201110132018\xpcom\bin\components\xpti.dat" > NUL 2>&1

echo  **** Installing CW10.2 Processor Expert MQX RTOS Adapter update ****
xcopy /S /I /F /Y /Q "%CD%/mqx_rtos_adapter/*" "%~1/MCU/ProcessorExpert"
echo  **** Done ****
GOTO End

:HelpScreen
echo This batch file (re-)installs MQX plug-ins for CodeWarrior 10.2
echo     Usage:   install_cw10.2_plugin.bat "<Path to CW10.2 installation>"
echo     Example  install_cw10.2_plugin.bat "c:\Freescale\CW MCU v10.2"

:End