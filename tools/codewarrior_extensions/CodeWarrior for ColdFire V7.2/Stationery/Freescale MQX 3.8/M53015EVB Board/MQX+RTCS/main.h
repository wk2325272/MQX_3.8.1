#ifndef __main_h_
#define __main_h_


#ifdef BSP_DEFAULT_ENET_DEVICE

#define ENET_DEVICE BSP_DEFAULT_ENET_DEVICE

/*
** The target's IP address and netmask
*/
#ifndef ENET_IPADDR
     #define ENET_IPADDR IPADDR(169,254,3,3) 
#endif

#ifndef ENET_IPMASK
    #define ENET_IPMASK IPADDR(255,255,0,0) 
#endif

#else

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

#endif 


#define MAIN_TASK   1

extern void Main_task (uint_32);


#endif
