Freescale Semiconductor MQX(TM) CodeSize script

The script analyzes the generated xMAP file produced by CodeWarrior or
IAR Embedded Workbench build tools and creates codesize report in an HTML 
format.

The script PERL source code is located in the current folder, the script 
executable is located in /tools directory. 

The reports created with this script for different build tools are available 
in the "results" folder. The MQX kernel configurations used for these results
are available in the "config" directory.   

==============================================================================
 
Supported tools:
    CodeWarrior for MCU/ColdFire 6.3 (use -c cwcf6)
    CodeWarrior for ColdFire 7.2 (use -c cwcf7)
    CodeWarrior for mobileGT 9.2 (use -c cwmpc9)
    CodeWarrior version 10 for ColdFire platforms (use -c cwcf10)
    CodeWarrior version 10 for Kinetis platforms  (use -c cwarm10)
    IAR Embedded Workbench for ARM v6.10 or later (use -c iararm6)
    ARM-MDK Keil uVision v4.23 or later (use -c uv4)

Options:
    -M ... print detailed MAP file analysis report
    -t ... dump text output to console
    -W ... enable warning messages
    -o <FILE>  ... specify report output file name
    -n <NAME>  ... alternative MAP file name displayed in the report
    -c <FORMAT> .. MAP file format specifier (use before the MAP file name)
       supported formats: cwcf7, cwmcu6, cwmpc9, cwcf10, cwarm10, iararm6, uv4

Report Options:
    +PSP  or -PSP  .. force or suppress PSP details (default +)
    +BSP  or -BSP  .. force or suppress BSP details (default +)
    +MFS  or -MFS  .. force or suppress MFS File System details (default -)
    +RTCS or -RTCS .. force or suppress RTCS TCP/IP stack details (default -)
    +USBH or -USBH .. force or suppress USB Host stack details (default -)
    +USBL or -USBL .. force or suppress USB Dev stack details  (default -)
    +SUMM or -SUMM .. force or suppress summary report (default +)

Usage examples:
    codesize.exe -c cwcf7 <MAP_file>
    codesize.exe -M -c cwcf10 <MAP_file>
    codesize.exe +MFS -PSP -BSP -c cwcf10 <MAP_file>
    codesize.exe -c cwcf10 <MAP_file1> -c cwmpc9 <MAP_file2> ....
