/**HEADER********************************************************************
* 
* Copyright (c) 2008 Freescale Semiconductor;
* All Rights Reserved
*
* Copyright (c) 2004-2008 Embedded Access Inc.;
* All Rights Reserved
*
* Copyright (c) 1989-2008 ARC International;
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
* $FileName: clnterr.c$
* $Version : 3.0.6.0$
* $Date    : Jan-8-2009$
*
* Comments:
*
*   This file contains the implementation of the client
*   side RPC Library error control functions.
*
*END************************************************************************/

/*
** Sun RPC is a product of Sun Microsystems, Inc. and is provided for
** unrestricted use provided that this legend is included on all tape
** media and as a part of the software program in whole or part.  Users
** may copy or modify Sun RPC without charge, but are not authorized
** to license or distribute it to anyone else except as part of a product or
** program developed by the user.
**
** SUN RPC IS PROVIDED AS IS WITH NO WARRANTIES OF ANY KIND INCLUDING THE
** WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS FOR A PARTICULAR
** PURPOSE, OR ARISING FROM A COURSE OF DEALING, USAGE OR TRADE PRACTICE.
**
** Sun RPC is provided with no support and without any obligation on the
** part of Sun Microsystems, Inc. to assist in its use, correction,
** modification or enhancement.
**
** SUN MICROSYSTEMS, INC. SHALL HAVE NO LIABILITY WITH RESPECT TO THE
** INFRINGEMENT OF COPYRIGHTS, TRADE SECRETS OR ANY PATENTS BY SUN RPC
** OR ANY PART THEREOF.
**
** In no event will Sun Microsystems, Inc. be liable for any lost revenue
** or profits or other special, indirect and consequential damages, even if
** Sun has been advised of the possibility of such damages.
**
** Sun Microsystems, Inc.
** 2550 Garcia Avenue
** Mountain View, California  94043
*/


#include <string.h>
#include <rtcsrpc.h>

struct rpc_createerr rpc_createerr;
static char _rpc_createerrbuf[CLNTERRBUFSIZE];

static char_ptr auth_errmsg(enum auth_stat);

/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : auth_errmsg
* Returned Value   : string containing error message
* Comments  :  Returns a string describing an RPC auth_stat
*
*END*-----------------------------------------------------------------*/

struct auth_errtab {
   enum auth_stat status;
   char_ptr message;
};

static struct auth_errtab auth_errlist[] = {
   { AUTH_OK,              "Authentication OK"              },
   { AUTH_BADCRED,         "Invalid client credential"      },
   { AUTH_REJECTEDCRED,    "Server rejected credential"     },
   { AUTH_BADVERF,         "Invalid client verifier"        },
   { AUTH_REJECTEDVERF,    "Server rejected verifier"       },
   { AUTH_TOOWEAK,         "Client credential too weak"     },
   { AUTH_INVALIDRESP,     "Invalid server verifier"        },
   { AUTH_FAILED,          "Failed (unspecified error)"     },
};

static char_ptr auth_errmsg
   (
      enum auth_stat stat
            /* [IN] - error to describe */
   )
{ /* Body */
   uint_32 i;

   for (i = 0; i < sizeof(auth_errlist)/sizeof(struct auth_errtab); i++) {
      if (auth_errlist[i].status == stat) {
         return(auth_errlist[i].message);
      } /* Endif */
   } /* Endfor */
   return(NULL);
} /* Endbody */


/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : clnt_sperror
* Returned Value   : string containing error message
* Comments  :  Outputs reply error information to a string
*
*END*-----------------------------------------------------------------*/

char_ptr clnt_sperror
   (
      CLIENT_PTR  rpch,
            /* [IN] - client handle */
      char_ptr    s
            /* [IN] - prefix for output string */
   )
{ /* Body */
   RPC_ERROR e;
   char_ptr err;
   char_ptr str = rpch->cl_errbuf;

   if (str == NULL) {
      str = rpch->cl_errbuf = RTCS_mem_alloc_zero(CLNTERRBUFSIZE);
      if (str == NULL) {
         return (NULL);
      } /* Endif */
   } /* Endif */
   clnt_geterr(rpch, &e);

   sprintf(str, "%s: %s", s, clnt_sperrno(e.re_status));
   str += strlen(str);

   switch (e.re_status) {
   case RPC_SUCCESS:
   case RPC_CANTENCODEARGS:
   case RPC_CANTDECODERES:
   case RPC_TIMEDOUT:
   case RPC_PROGUNAVAIL:
   case RPC_PROCUNAVAIL:
   case RPC_CANTDECODEARGS:
   case RPC_SYSTEMERROR:
   case RPC_UNKNOWNHOST:
   case RPC_UNKNOWNPROTO:
   case RPC_PMAPFAILURE:
   case RPC_PROGNOTREGISTERED:
   case RPC_FAILED:
      break;

   case RPC_CANTSEND:
   case RPC_CANTRECV:
      sprintf(str, "; errno = 0x%lX", e.re_errno);
      str += strlen(str);
      break;

   case RPC_VERSMISMATCH:
      sprintf(str, "; low version = %lu, high version = %lu",
         e.re_vers.low, e.re_vers.high);
      str += strlen(str);
      break;

   case RPC_AUTHERROR:
      err = auth_errmsg(e.re_why);
      sprintf(str, "; why = ");
      str += strlen(str);
      if (err != NULL) {
         sprintf(str, "%s", err);
      } else {
         sprintf(str, "(unknown authentication error - %ld)",
            (int_32)e.re_why);
      } /* Endif */
      str += strlen(str);
      break;

   case RPC_PROGVERSMISMATCH:
      sprintf(str, "; low version = %lu, high version = %lu",
         e.re_vers.low, e.re_vers.high);
      str += strlen(str);
      break;

   default:   /* unknown */
      sprintf(str, "; s1 = %lu, s2 = %lu",
         e.re_lb.s1, e.re_lb.s2);
      str += strlen(str);
      break;
   } /* Endswitch */
   return(rpch->cl_errbuf);
} /* Endbody */

/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : clnt_perror
* Returned Value   : nothing
* Comments  :  Outputs reply error information to stderr
*
*END*-----------------------------------------------------------------*/

void clnt_perror
   (
      CLIENT_PTR  rpch,
            /* [IN] - client handle */
      char_ptr    s
            /* [IN] - prefix for output string */
   )
{ /* Body */
   fprintf(stderr, "%s\n", clnt_sperror(rpch, s));
} /* Endbody */


/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : clnt_sperrno
* Returned Value   : string containing error message
* Comments  :  Returns a string describing an RPC clnt_stat
*
*END*-----------------------------------------------------------------*/

struct rpc_errtab {
   enum clnt_stat status;
   char_ptr message;
};

static struct rpc_errtab rpc_errlist[] = {
   { RPC_SUCCESS,             "RPC: Success"                         },
   { RPC_CANTENCODEARGS,      "RPC: Can't encode arguments"          },
   { RPC_CANTDECODERES,       "RPC: Can't decode result"             },
   { RPC_CANTSEND,            "RPC: Unable to send"                  },
   { RPC_CANTRECV,            "RPC: Unable to receive"               },
   { RPC_TIMEDOUT,            "RPC: Timed out"                       },
   { RPC_VERSMISMATCH,        "RPC: Incompatible versions of RPC"    },
   { RPC_AUTHERROR,           "RPC: Authentication error"            },
   { RPC_PROGUNAVAIL,         "RPC: Program unavailable"             },
   { RPC_PROGVERSMISMATCH,    "RPC: Program/version mismatch"        },
   { RPC_PROCUNAVAIL,         "RPC: Procedure unavailable"           },
   { RPC_CANTDECODEARGS,      "RPC: Server can't decode arguments"   },
   { RPC_SYSTEMERROR,         "RPC: Remote system error"             },
   { RPC_UNKNOWNHOST,         "RPC: Unknown host"                    },
   { RPC_PMAPFAILURE,         "RPC: Port mapper failure"             },
   { RPC_PROGNOTREGISTERED,   "RPC: Program not registered"          },
   { RPC_FAILED,              "RPC: Failed (unspecified error)"      },
   { RPC_UNKNOWNPROTO,        "RPC: Unknown protocol"                },
   { RPC_SOCKETERROR,         "RPC: Could not create socket"         }
};

char_ptr clnt_sperrno
   (
      enum clnt_stat stat
            /* [IN] - error to describe */
   )
{ /* Body */
   uint_32 i;

   for (i = 0; i < sizeof(rpc_errlist)/sizeof(struct rpc_errtab); i++) {
      if (rpc_errlist[i].status == stat) {
         return (rpc_errlist[i].message);
      } /* Endif */
   } /* Endfor */
   return ("RPC: (unknown error code)");
} /* Endbody */

/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : clnt_perrno
* Returned Value   : nothing
* Comments  :  Outputs a string describing an RPC clnt_stat to stderr
*
*END*-----------------------------------------------------------------*/

void clnt_perrno
   (
      enum clnt_stat stat
            /* [IN] - error to describe */
   )
{ /* Body */
   fprintf(stderr, "%s\n", clnt_sperrno(stat));
} /* Endbody */


/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : clnt_spcreateerror
* Returned Value   : string containing error message
* Comments  :  Wrapper for clnt_sperrno
*
*END*-----------------------------------------------------------------*/

char_ptr clnt_spcreateerror
   (
      char_ptr s
            /* [IN] - prefix for output string */
   )
{ /* Body */
   char_ptr str = _rpc_createerrbuf;

   sprintf(str, "%s: ", s);
   strcat(str, clnt_sperrno(rpc_createerr.cf_stat));
   switch (rpc_createerr.cf_stat) {
   case RPC_PMAPFAILURE:
      strcat(str, " - ");
      strcat(str, clnt_sperrno(rpc_createerr.cf_error.re_status));
      break;

   case RPC_SYSTEMERROR:
   case RPC_SOCKETERROR:
      sprintf(&str[strlen(str)], " - Error 0x%lX",
         rpc_createerr.cf_error.re_errno);
      break;
   } /* Endswitch */
   return (str);
} /* Endbody */

/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : clnt_pcreateerror
* Returned Value   : nothing
* Comments  :  Wrapper for clnt_perrno
*
*END*-----------------------------------------------------------------*/

void clnt_pcreateerror
   (
      char_ptr s
            /* [IN] - prefix for output string */
   )
{ /* Body */
   fprintf(stderr, "%s\n", clnt_spcreateerror(s));
} /* Endbody */



/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : rpc_seterr_reply
* Returned Value   : void
* Comments  :  Fills in a client error structure given an RPC reply message
*
*END*-----------------------------------------------------------------*/

void rpc_seterr_reply
   (
      RPC_MESSAGE_PTR   msg,
      RPC_ERROR_PTR     error
   )
{ /* Body */

   switch (msg->rm_reply.rp_stat) {

   case MSG_ACCEPTED:
      switch (msg->acpted_rply.ar_stat) {

      case PROG_UNAVAIL:
         error->re_status = RPC_PROGUNAVAIL;
         break;
      case PROG_MISMATCH:
         error->re_status = RPC_PROGVERSMISMATCH;
         error->re_vers.low = msg->acpted_rply.ar_vers.low;
         error->re_vers.high = msg->acpted_rply.ar_vers.high;
         break;
      case PROC_UNAVAIL:
         error->re_status = RPC_PROCUNAVAIL;
         break;
      case GARBAGE_ARGS:
         error->re_status = RPC_CANTDECODEARGS;
         break;
      case SYSTEM_ERR:
         error->re_status = RPC_SYSTEMERROR;
         break;
      case SUCCESS:
         error->re_status = RPC_SUCCESS;
         break;
      default:    /* we should never get here */
         error->re_status = RPC_FAILED;
         error->re_lb.s1 = (int_32)MSG_ACCEPTED;
         error->re_lb.s2 = (int_32)msg->acpted_rply.ar_stat;
         break;
      } /* Endswitch */
      break;

   case MSG_DENIED:
      switch (msg->rjcted_rply.rj_stat) {

      case RPC_VERSMISMATCH:
         error->re_status = RPC_VERSMISMATCH;
         error->re_vers.low = msg->rjcted_rply.rj_vers.low;
         error->re_vers.high = msg->rjcted_rply.rj_vers.high;
         break;
      case AUTH_ERROR:
         error->re_status = RPC_AUTHERROR;
         error->re_why = msg->rjcted_rply.rj_why;
         break;
      default:    /* we should never get here */
         error->re_status = RPC_FAILED;
         error->re_lb.s1 = (int_32)MSG_DENIED;
         error->re_lb.s2 = (int_32)msg->rjcted_rply.rj_stat;
         break;
      } /* Endswitch */
      break;

   default:       /* we should never get here */
      error->re_status = RPC_FAILED;
      error->re_lb.s1 = (int_32)(msg->rm_reply.rp_stat);
      break;
   } /* Endswitch */

} /* Endbody */


/* EOF */
