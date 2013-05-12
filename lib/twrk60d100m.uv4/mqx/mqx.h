/* MQX library output files were changed in MQX 3.7.0 */ 

#error Your project uses incorrect paths.
#error The MQX BSP and PSP libraries and header files were moved from /lib/<board>/mqx to lib/<board>/bsp and lib/<board>/psp folders. 

/* 
* Following files are affected: (replace the old ones by new ones from psp and bsp folders)
*        linker command files - (*.lcf) 
*        psp libraries  - <board_name>_regabi_d.a, <board_name>_regabi.a
*        bsp libraries  - mqx_regabi.a, mqx_regabi_d.a
*        debuger config files - <board>.cfg, <board>.mem
*         
* Modify your project setting:
*     Add project to you  
*     (CW6.3, CW7.2, CW9.1 - Target/Access Path/)
*     (CW10 - Project Properties/Settings/Linker + Compiler + Assembler/Input)
*     (IAR - Project Options/C\C++ Compiler/Preprocessor)
*        lib\<board>\bsp
*        lib\<board>\psp
*        lib\<board>\bsp\dbg                                                                
*     Remove path 
*         lib\<board>\mqx
*         lib\<board>\mqx\dbg 
*         
*     Change debugger config files paths (W6.3, CW7.2, CW9.1 - CF Debugger Setting, CW10 - Debug Configurations..., )
*        lib\<board>\mqx\dbg\<board>.cfg -> lib\<board>\bsp\dbg\<board>.cfg
*        lib\<board>\mqx\dbg\<board>.mem -> lib\<board>\bsp\dbg\<board>.mem                
*
*/

   