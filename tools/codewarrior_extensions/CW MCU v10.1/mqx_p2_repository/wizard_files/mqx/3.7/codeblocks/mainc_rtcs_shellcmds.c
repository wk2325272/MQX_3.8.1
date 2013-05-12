
   /* RTCS commands */ 
   { "arpadd",    Shell_arpadd },
   { "arpdel",    Shell_arpdel },
   { "arpdisp",   Shell_arpdisp },
   { "exit",      Shell_exit },      
   { "gate",      Shell_gate },
   { "gethbn",    Shell_get_host_by_name }, 
   { "getrt",     Shell_getroute },
   { "help",      Shell_help }, 
   { "ipconfig",  Shell_ipconfig },      
   { "netstat",   Shell_netstat },   
#if RTCSCFG_ENABLE_ICMP      
   { "ping",      Shell_ping },      
#endif
   { "telnet",    Shell_Telnet_client },
   { "telnetd",   Shell_Telnetd },
   { "walkrt",    Shell_walkroute },
   { "?",         Shell_command_list },
