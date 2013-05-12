const SHELL_COMMAND_STRUCT Telnetd_shell_commands[] = {
   /* RTCS commands */ 
   { "exit",      Shell_exit },      
   { "gethbn",    Shell_get_host_by_name }, 
   { "getrt",     Shell_getroute },
   { "help",      Shell_help }, 
   { "ipconfig",  Shell_ipconfig },      
   { "netstat",   Shell_netstat },
   { "pause",     Shell_pause },
#if RTCSCFG_ENABLE_ICMP      
   { "ping",      Shell_ping },
#endif
   { "telnet",    Shell_Telnet_client },
   { "walkrt",    Shell_walkroute },
   { "?",         Shell_command_list },     
   { NULL,        NULL } 
};
