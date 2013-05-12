/**HEADER********************************************************************
* 
* Copyright (c) 2008 Freescale Semiconductor;
* All Rights Reserved
*
* Copyright (c) 2004-2008 Embedded Access Inc.;
* All Rights Reserved
*
*************************************************************************** 
*
* THIS SOFTWARE IS PROVIDED BY FREESCALE "AS IS" AND ANY EXPRESSED OR 
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES 
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  
* IN NO EVENT SHALL FREESCALE OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR 
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
* IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF 
* THE POSSIBILITY OF SUCH DAMAGE.
*
**************************************************************************
*
* $FileName: Mail.c$
* $Version : 3.8.6.1$
* $Date    : Feb-13-2012$
*
* Comments:
*
*   
*
*END************************************************************************/


#include <mqx.h>
#include <bsp.h>
#include <rtcs.h>
#include <ipcfg.h>
#include <string.h>
#include "security_private.h"
#include "security_public.h"
#include "security.h"


/*
** Translation Table as described in RFC1113
*/
static const char cb64[]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";


static uint_32 SMTP_Send_command(uint_32 sock, char * cmd, char * in_param, char * out_param, uint_32 param_len) 
{
   char     temp[150];
   uint_32  len;
   
   //Place command string into 'temp' variable
   if (in_param==NULL) {
      sprintf(temp, "%s\r\n", cmd);      
   } else {
      sprintf(temp,"%s %s\r\n",cmd,in_param);      
   }
   send(sock,temp,strlen(temp),0);          //Send to server
   printf("Sent:\n %s\n",temp);             //Print to console what was just sent
   
   //Receive server's response
   len = recv(sock, out_param, param_len - 1, 0);         
   out_param[len] = 0;
   
   //Print to console what server response was
   if (len && (len!=IO_ERROR)) {
      printf("Received:\n %s\n",out_param);         
   }
   return len;
}

static void SMTP_Send_string(uint_32 sock, char * data) 
{
    send(sock, data, strlen(data), 0);          //Send to server
    printf(data);             //Print to console what was just sent
}

static void SMTP_Receive_string(uint_32 sock, char * data, uint_32 len) 
{
   uint_32 rx_len;
   
   rx_len=recv(sock,data,len-1,0);       
   data[rx_len]=0;
   printf(data);             //Print to console what was just sent
   printf("\n");
   
}

const char Email_header[] =    
   "From: \"TWR-MCF51CN-KIT \" <" EMAIL_FROM ">\r\n"
   "To:  \"Developer\" <" EMAIL_TO ">\r\n"
   "Subject: Freescale Tower System: Intrusion Alert!\r\n" ;


const char_ptr wday[7] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
const char_ptr months[] =  
{
   "Jan", "Feb", "Mar", "Apr", "May", "Jun", 
   "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" 
};

char                 last[150] = {0};

void SEC_EmailAlert()
{
   MQX_TICK_STRUCT      ticks;
   MQX_XDATE_STRUCT     xdate;
   sockaddr_in          addr;
   _ip_address          ipaddr;
   uint_32              sock;
   uint_32              error;
   uint_32              option;
   char_ptr             event_ptr;
   char                 temp[150];
   char                 domain[30];
   char                 user[30];
    
#if DEMOCFG_AUTH_REQUIRED    
   char                 temp_encoded[40];
#endif
    

  
   if (!RTCS_resolve_ip_address(EMAIL_SERVER,&ipaddr,NULL,0)) {
      printf("Error resolving IP address for %s using DNS Server at %d.%d.%d.%d\n",EMAIL_SERVER,IPBYTES(LWDNS_server_ipaddr));
      return;
   }
  
    /* Create socket */        
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == RTCS_SOCKET_ERROR) 
    {
        printf("Error creating socket\n");
        return;
    } 

   /* Reduce buffer size of socket to save memory */
   option = SMTP_BUFFER_SIZE;   
   error = setsockopt(sock, SOL_TCP, OPT_TBSIZE, &option, sizeof(option));
   option = SMTP_BUFFER_SIZE;   
   error = setsockopt(sock, SOL_TCP, OPT_RBSIZE, &option, sizeof(option));
   option = 1000;   
   error = setsockopt(sock, SOL_TCP, OPT_TIMEWAIT_TIMEOUT, &option, sizeof(option));


    /* Allow binding to any address */
    addr.sin_family      = AF_INET;
    addr.sin_port        = 0;
    addr.sin_addr.s_addr = INADDR_ANY;
    error = bind(sock, &addr, sizeof(addr));
    if (error != RTCS_OK) {
        printf("Error in binding socket %08x\n",error);
        shutdown(sock, FLAG_ABORT_CONNECTION);
        return;
    } 

    /* Connect to SMTP server */
    addr.sin_port        = IPPORT_SMTP;
    addr.sin_addr.s_addr = ipaddr;
    error = connect(sock, &addr, sizeof(addr));
    if (error != RTCS_OK) {
        printf("Error in connection %08x\n",error);
        shutdown(sock, FLAG_ABORT_CONNECTION);
        return;
    }
  
    printf("\nConnecting to SMTP server %s...\n",EMAIL_SERVER);
    printf("SMTP Server Response:\n");
    SMTP_Receive_string(sock, temp, sizeof(temp));


    /* Follow SMTP protocol to send email */   

    /* Parse domain name from the FROM address, 
     for use with the EHLO or HELO commands */
    sscanf(EMAIL_FROM,"%30s@%30s",user,domain);
  
#if DEMOCFG_AUTH_REQUIRED 
  
    /* If Authentification is required, use EHLO command */
    SMTP_Send_command(sock, "EHLO", domain, temp, sizeof(temp));
    SMTP_Send_command(sock,"AUTH LOGIN",NULL,temp,sizeof(temp));

    /* Find base64 econding of login name and send to SMTP server */
    sprintf(temp,"%s\r\n",base64_encode(AUTH_USERNAME,temp_encoded));
    SMTP_Send_string(sock,temp);
    SMTP_Receive_string(sock,temp,sizeof(temp)-1);

    /* Find base64 econding of password and send to SMTP server */
    sprintf(temp,"%s\r\n",base64_encode(AUTH_PASSWORD,temp_encoded));
    SMTP_Send_string(sock,temp);
    SMTP_Receive_string(sock,temp, sizeof(temp)-1);
#else
    /* If Authentification is not required, use HELO command as not all servers recognize EHLO */
    SMTP_Send_command(sock, "HELO", domain, temp, sizeof(temp));
#endif


   /* Transfer sender email address to server */     
   SMTP_Send_command(sock,"MAIL FROM:","<" EMAIL_FROM ">" ,temp,sizeof(temp));
   /* Transfer recipient email address to server */            
   SMTP_Send_command(sock,"RCPT TO:","<" EMAIL_TO ">" ,temp,sizeof(temp));

   /* Start DATA section */
   SMTP_Send_command(sock,"DATA",NULL,temp,sizeof(temp));

   printf("Sent:\n");
   /* Send Subject, To, and From fields */
   SMTP_Send_string(sock,(pointer)Email_header);


   /* Determine what to send as body of message */
   _time_get_ticks(&ticks);
   _time_ticks_to_xdate(&ticks,&xdate);    

   sprintf(temp, "Date: %s, %d %s %d %02d:%02d:%02d -0000\r\n\r\n",
      wday[xdate.WDAY],xdate.MDAY, months[xdate.MONTH-1],xdate.YEAR, xdate.HOUR, xdate.MIN, xdate.SEC  );
   SMTP_Send_string(sock,temp);


   /* If button pushed was the "Door"... */
   if(SEC_Params.Status==SEC_DOOR_OPEN_STATUS)
   {
      event_ptr = "Door was opened ";
   } else 
   /* If button pushed was the "Window"... */
   if(SEC_Params.Status==SEC_WINDOW_OPEN_STATUS)
   {
      event_ptr = "Window was opened ";
   }
   /* If not sure what caused stop mode to exit... */  
   else 
   {
      event_ptr = "Unknown event occurred ";
   }    
   SMTP_Send_string(sock,event_ptr);

   /* If SNTP enabled, then send exact date and time button press happened */
   #if DEMOCFG_ENABLE_SNTP
      sprintf(temp,"on %d/%d/%d at %02d:%02d:%02d GMT. \r\n",xdate.MONTH,xdate.MDAY,xdate.YEAR,xdate.HOUR,xdate.MIN,xdate.SEC);
   /* If SNTP not enabled, then give elapsed time instead */
   #else
      sprintf(temp,"%02d:%02d:%02d after system was started. \r\n",xdate.HOUR,xdate.MIN,xdate.SEC);
   #endif
   SMTP_Send_string(sock,temp);  

   if (last[0]) {
      /* Send out how long ago the last event was */
      SMTP_Send_string(sock,"Previous event was ");
      SMTP_Send_string(sock,last);
   }
   strcpy(last,temp); 
   
   /* Send Freescale signature and disconnect from server with QUIT command */
   SMTP_Send_string(sock,"\r\n-Freescale Semiconductor\r\n.\r\n");
   
   SMTP_Send_command(sock,"QUIT",NULL,temp,sizeof(temp));

  /* Close socket */
    shutdown(sock, FLAG_CLOSE_TX);  
}




/*
** base64_encodeblock
**
** encode 3 8-bit binary bytes as 4 '6-bit' characters
*/
void base64_encodeblock(unsigned char in[3], unsigned char out[4], int len )
{
  out[0] = cb64[in[0]>>2];
  out[1] = cb64[((in[0] & 0x03) << 4) | ((in[1] & 0xf0) >> 4) ];
  out[2] = (unsigned char) (len > 1 ? cb64[ ((in[1] & 0x0f) << 2) | ((in[2] & 0xc0) >> 6) ] : '=');
  out[3] = (unsigned char) (len > 2 ? cb64[ in[2] & 0x3f ] : '=');
}


char * base64_encode(char *source, char *destination)
{
  unsigned char in[3], out[4];
  int i, n, p,len;
  i=0;
  n=0;
  p=0;
  len=0;

  *destination=0;
  
  while( source[n]!=0 ) 
  {
    len = 0;
    for( i = 0; i < 3; i++ ) 
    {
      in[i] = source[n];
      if( source[n]!=0  ) 
      {
        len++;
        n++;
      }
      else 
      {
        in[i] = 0;
      }
    }
    
    if( len ) 
    {
      base64_encodeblock( in, out, len );
      for( i = 0; i < 4; i++ ) 
      {
        destination[p]= out[i];
        p++;
      }
    }
  }
  
  destination[p]=0;
  return destination;
}
