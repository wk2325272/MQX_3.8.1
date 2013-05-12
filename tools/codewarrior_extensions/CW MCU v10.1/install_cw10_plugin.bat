@ECHO OFF

IF "%~1"=="" GOTO HelpScreen 

:Install
echo  **** Installing CW10.1 MQX Plugins ****
echo  **** It can take up to 60 seconds ****  

echo **** Preparing for plug-ins installation process ****
set mqx_3_7_installed=0
if exist "%~1/MCU/lib/wizard_data/mqx/3.7" (
    set mqx_3_7_installed=1
)

"%~1/eclipse/cwidec.exe" -nosplash -application org.eclipse.equinox.p2.director -repository file:"%CD%/mqx_p2_repository/" -uninstallIU com.freescale.mqx.feature.group -destination "%~1/eclipse" -profile PlatformProfile > NUL 2>&1

echo **** Preparation has been successfully done ****

if %mqx_3_7_installed%==1 (
    echo **** Installing plug-ins for MQX 3.7 ****
    "%~1/eclipse/cwidec.exe" -nosplash -application org.eclipse.equinox.p2.director -repository file:"%CD%/mqx_p2_repository/" -installIU com.freescale.mqx.feature.group -destination "%~1/eclipse" -profile PlatformProfile
)

if exist "%~1/MCU/lib/wizard_data/mqx/3.8" (
    echo **** Uninstalling old plug-ins for MQX 3.8 ****
    "%~1/eclipse/cwidec.exe" -nosplash -application org.eclipse.equinox.p2.director -repository file:"%CD%/mqx_p2_repository/" -uninstallIU com.freescale.mqx.v3.8.feature.group -destination "%~1/eclipse" -profile PlatformProfile > NUL 2>&1
)
echo **** Installing plug-ins for MQX 3.8 ****
"%~1/eclipse/cwidec.exe" -nosplash -application org.eclipse.equinox.p2.director -repository file:"%CD%/mqx_p2_repository/" -installIU com.freescale.mqx.v3.8.feature.group -destination "%~1/eclipse" -profile PlatformProfile

if exist "%~1/MCU/bin/plugins/Debugger/rtos/mqx/tools/tad/mqx.tad" (
echo **** Uninstalling old plug-ins for MQX - all versions ****
    "%~1/eclipse/cwidec.exe" -nosplash -application org.eclipse.equinox.p2.director -repository file:"%CD%/mqx_p2_repository/" -uninstallIU com.freescale.mqx.common.feature.group -destination "%~1/eclipse" -profile PlatformProfile
)
echo **** Installing plug-ins for MQX - all versions ****
"%~1/eclipse/cwidec.exe" -nosplash -application org.eclipse.equinox.p2.director -repository file:"%CD%/mqx_p2_repository/" -installIU com.freescale.mqx.common.feature.group -destination "%~1/eclipse" -profile PlatformProfile


echo  **** Installing CW10.1 Processor Expert MQX RTOS Adapter update ****
xcopy /S /I /F /Y /Q "%CD%/mqx_rtos_adapter/*" "%~1/MCU/ProcessorExpert"
echo  **** Done ****
GOTO End

:HelpScreen
echo This batch file re-installs CW10 Task Aware Debugging and New Project wizard plugins
echo     Usage:   install_cw10_plugin.bat "<Path to CW10.1 installation>"
echo     Example  install_cw10_plugin.bat "c:\Program Files\Freescale\CW MCU v10.1"

:End