@echo off

rem  Copy common header files

copy /Y ..\..\..\..\usb\host\source\classes\common\usb_classes.c .

copy /Y ..\..\..\..\usb\host\source\classes\phdc\usb_host_phdc.h .
copy /Y ..\..\..\..\usb\host\source\classes\audio\usb_host_audio.h .
copy /Y ..\..\..\..\usb\host\source\classes\cdc\usb_host_cdc.h .
copy /Y ..\..\..\..\usb\host\source\classes\hid\usb_host_hid.h .
copy /Y ..\..\..\..\usb\host\source\classes\hub\usb_host_hub.h .
copy /Y ..\..\..\..\usb\host\source\classes\hub\usb_host_hub_sm.h .
copy /Y ..\..\..\..\usb\host\source\classes\msd\mfs\usbmfs.h .
copy /Y ..\..\..\..\usb\host\source\classes\msd\mfs\usbmfspr.h .
copy /Y ..\..\..\..\usb\host\source\classes\msd\usb_host_msd_bo.h .
copy /Y ..\..\..\..\usb\host\source\classes\msd\usb_host_msd_ufi.h .
copy /Y ..\..\..\..\usb\host\source\classes\printer\usb_host_printer.h .
copy /Y ..\..\..\..\usb\host\source\include\host_close.h .
copy /Y ..\..\..\..\usb\host\source\include\host_cnfg.h .
copy /Y ..\..\..\..\usb\host\source\include\host_cnl.h .
copy /Y ..\..\..\..\usb\host\source\include\host_common.h .
copy /Y ..\..\..\..\usb\host\source\include\host_dev_list.h .
copy /Y ..\..\..\..\usb\host\source\include\host_ch9.h .
copy /Y ..\..\..\..\usb\host\source\include\host_main.h .
copy /Y ..\..\..\..\usb\host\source\include\host_rcv.h .
copy /Y ..\..\..\..\usb\host\source\include\host_shut.h .
copy /Y ..\..\..\..\usb\host\source\include\host_snd.h .
copy /Y ..\..\..\..\usb\host\source\include\hostapi.h .
copy /Y ..\..\..\..\usb\host\source\include\types.h .
copy /Y ..\..\..\..\usb\host\source\include\usbprv.h .
copy /Y ..\..\..\..\usb\host\source\rtos\mqx\mqx_host.h .
copy /Y ..\..\..\..\usb\host\source\rtos\mqx\mqx_dll.h .
copy /Y ..\..\..\..\usb\host\source\rtos\mqx\usb_mqx.h .

copy /Y ..\..\..\..\usb\common\include\usb_types.h .\..
copy /Y ..\..\..\..\usb\common\include\usb.h .\..
copy /Y ..\..\..\..\usb\common\include\usb_debug.h .\..
copy /Y ..\..\..\..\usb\common\include\usb_desc.h .\..
copy /Y ..\..\..\..\usb\common\include\usb_error.h .\..
copy /Y ..\..\..\..\usb\common\include\usb_misc.h .\..
copy /Y ..\..\..\..\usb\common\include\usb_misc_prv.h .\..
copy /Y ..\..\..\..\usb\common\include\usb_prv.h .\..
