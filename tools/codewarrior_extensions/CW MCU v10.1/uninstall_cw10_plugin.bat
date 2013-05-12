@ECHO OFF

IF "%~1"=="" GOTO HelpScreen 

:Install
echo  **** Uninstalling all common and MQX 3.8 version dependent plug-ins ***
echo  **** It can take up to 60 seconds ***  

"%~1/eclipse/cwidec.exe" -nosplash -application org.eclipse.equinox.p2.director -repository file:"%CD%/mqx_p2_repository/" -uninstallIU com.freescale.mqx.v3.8.feature.group -destination "%~1/eclipse" -profile PlatformProfile

"%~1/eclipse/cwidec.exe" -nosplash -application org.eclipse.equinox.p2.director -repository file:"%CD%/mqx_p2_repository/" -uninstallIU com.freescale.mqx.common.feature.group -destination "%~1/eclipse" -profile PlatformProfile

echo  **** Done ***
GOTO End

:HelpScreen
echo This batch file uninstalls CW10 Task Aware Debugging and New Project wizard plugins
echo     Usage:   uninstall_all.bat "<Path to CW10.1 installation>"
echo     Example  uninstall_all.bat "c:\Program Files\Freescale\CW MCU v10.1"
 
:End  