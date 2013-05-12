@ECHO OFF

IF "%~1"=="" GOTO HelpScreen 

:Install
echo  **** Uninstalling MQX plug-ins for CodeWarrior 10.2 ***
echo  **** It can take up to 60 seconds ***  

"%~1/eclipse/cwidec.exe" -nosplash -application org.eclipse.equinox.p2.director -repository file:"%CD%/mqx_p2_repository/" -uninstallIU com.freescale.mqx.v3.8.feature.group

"%~1/eclipse/cwidec.exe" -nosplash -application org.eclipse.equinox.p2.director -repository file:"%CD%/mqx_p2_repository/" -uninstallIU com.freescale.mqx.common.feature.group

echo  **** Done ***
GOTO End

:HelpScreen
echo This batch file uninstalls MQX plug-ins for CodeWarrior 10.2
echo     Usage:   uninstall_cw10.2_plugin.bat "<Path to CW10.2 installation>"
echo     Example  uninstall_cw10.2_plugin.bat "c:\Freescale\CW MCU v10.2"
 
:End  