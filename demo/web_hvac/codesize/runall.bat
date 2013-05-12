..\..\..\tools\codesize.exe -o results\twrmcf52259_report.html +BSP +PSP +MFS +RTCS +USBH -USBD -c cwcf7 -n "twrmcf52259 (Release)"     ..\cwcf72\twrmcf52259\intflash.elf.xMAP    -c cwcf10 -n "twrmcf52259 (Release)"    "..\cw10\web_hvac_twrmcf52259\Int Flash Release\intflash.elf.xMAP"

..\..\..\tools\codesize.exe -o results\twrmcf54418_report.html +BSP +PSP +MFS +RTCS +USBH -USBD -c cwcf7 -n "twrmcf54418 (Release)"     ..\cwcf72\twrmcf54418\extflash.elf.xMAP    -c cwcf10 -n "twrmcf54418 (Release)"    "..\cw10\web_hvac_twrmcf54418\Ext Flash Release\extflash.elf.xMAP"

..\..\..\tools\codesize.exe -o results\twrk60n512_report.html +BSP +PSP +MFS +RTCS +USBH -USBD -c iararm6 -n "twrk60n512 (Release)"    "..\iar\twrk60n512\Int Flash Release\List\twrk60n512.map"    -c cwarm10 -n "twrk60n512 (Release)"    "..\cw10\web_hvac_twrk60n512\Int Flash Release\intflash.afx.xMAP"   

..\..\..\tools\codesize.exe -o results\combined_report.html +BSP +PSP +MFS +RTCS +USBH -USBD -c cwcf7 -n "twrmcf52259 (Release)"     ..\cwcf72\twrmcf52259\intflash.elf.xMAP    -c cwcf10 -n "twrmcf52259 (Release)"    "..\cw10\web_hvac_twrmcf52259\Int Flash Release\intflash.elf.xMAP"    -c cwcf7 -n "twrmcf54418 (Release)"     ..\cwcf72\twrmcf54418\extflash.elf.xMAP    -c cwcf10 -n "twrmcf54418 (Release)"    "..\cw10\web_hvac_twrmcf54418\Ext Flash Release\extflash.elf.xMAP" -c iararm6 -n "twrk60n512 (Release)"    "..\iar\twrk60n512\Int Flash Release\List\twrk60n512.map"    -c cwarm10 -n "twrk60n512 (Release)"    "..\cw10\web_hvac_twrk60n512\Int Flash Release\intflash.afx.xMAP"   

