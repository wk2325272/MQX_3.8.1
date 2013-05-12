#ifndef __main_h_
#define __main_h_
#include <mqx.h>
#include <bsp.h>
$mainh_mfs_includes$
$mainh_usbhost_includes$
$mainh_usbdevice_includes$
$mainh_shell_includes$
$mainh_rtcs_includes$
$usbhost_usb_otg$

#define MAIN_TASK   1
$mfs_ram_disk_size$
$mfs_ramdisk_defines$
$rtcs_enet_device$
$rtcs_dhcp$

$rtcs_ip_address$
$rtcs_ip_mask$
$rtcs_gateway$
$rtcs_ppp$

$usbhost_max_frame_size$    
$usbhost_host_controller_number$

extern void Main_task (uint_32);

$mfs_ram_disk_start_function_prototype$
$rtcs_init_function_prototype$
$usbd_init_function_prototype$
$usbh_init_function_prototype$

$usbhost_hostevent_function_prototype$

/* PPP device must be set manually and 
** must be different from the default IO channel (BSP_DEFAULT_IO_CHANNEL) 
*/
#define PPP_DEVICE      "ittyb:" 

/*
** Define PPP_DEVICE_DUN only when using PPP to communicate
** to Win9x Dial-Up Networking over a null-modem
** This is ignored if PPP_DEVICE is not #define'd
*/
#define PPP_DEVICE_DUN  1

/*
** Define the local and remote IP addresses for the PPP link
** These are ignored if PPP_DEVICE is not #define'd
*/
#define PPP_LOCADDR     IPADDR(192,168,0,216)
#define PPP_PEERADDR    IPADDR(192,168,0,217)

/*
** Define a default gateway
*/
#define GATE_ADDR       IPADDR(192,168,0,1)

#endif /* __main_h_ */

