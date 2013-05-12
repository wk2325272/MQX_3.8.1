Freescale Semiconductor MQX(TM) Timing script

This script adds HTML tags to make human readable 
HTML form of the timing benchmark output.
Please copy the timing benchmark output from console,
store it as regular text file and invoke this script.
The input file is supposed to have lines formatted as
"name,value,units,loops,note", delimited by comma.
The result will be placed into another file named as
concatenation of input file name and ".html".

Usage:

timing.exe <timing_benchmark_console_output_filename>

Example:

ccperl timing.pl test.log
File test.log.html was created.
