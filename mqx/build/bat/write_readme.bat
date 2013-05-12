@echo off
echo.  
echo.!!! DO NOT CHANGE FILES IN THIS DIRECTORY !!!
echo.!!! ALL FILES ARE GENERATED DURING BUILD  !!!
echo.  
echo.  
echo.The MQX Libraries Directory
echo.===========================
echo.  
echo.The [MQX]/lib/[board] directory contains output files (.a and .h files) 
echo.created when building key MQX libraries: MQX PSP and BSP, MFS, RTCS, USB, 
echo.etc. This is the sole directory which needs to be added to "Access Paths" 
echo.for end-user application targeting this [board] platform. 
echo.
echo.You can drag and drop this text file into your project opened in the  
echo.CodeWarrior IDE to add correct MQX "Access Path" automatically to the
echo.project search paths. Even if you remove the file from project later, 
echo.the search path remains registered.
echo.
echo.
echo.Configuring the MQX Libraries
echo.=============================
echo.
echo.Please use the /config/[board]/user_config.h file to specify required 
echo.configuration options for the MQX, RTCS, MFS and other key libraries.
echo.
echo.This user's configuration file gets copied into /lib/[board] directory 
echo.during the MQX PSP or BSP build process. It is accessed from THIS location 
echo.by other libraries (RTCS, MFS, ...) so their configuration is consistent 
echo.with the MQX configuration. 
echo.
echo.!!! DO NOT CHANGE FILES IN THIS /lib/[board] DIRECTORY.
echo.
echo.It is recommended to rebuild all libraries after any change in the 
echo./config/[board]/user_config.h file. 
echo.
echo.+------------------------------------------------------------------------+
echo.!  Recommended compilation order is BSP, PSP, MFS, RTCS, USB, Shell, ... !
echo.!  Use /config/[board]/build_libs.mcp project for convenient mass build. !
echo.+------------------------------------------------------------------------+
echo.





