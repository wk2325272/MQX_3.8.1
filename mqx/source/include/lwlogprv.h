#ifndef __lwlogprv_h__
#define __lwlogprv_h__ 1
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
* $FileName: lwlogprv.h$
* $Version : 3.0.3.0$
* $Date    : Nov-21-2008$
*
* Comments:
*
*   This include file is used to define constants and data types for the
*   Log component.
*
*END************************************************************************/

/*--------------------------------------------------------------------------*/
/*                        CONSTANT DEFINITIONS                              */

#define LWLOG_VALID          ((_mqx_uint)(0x6C776C67))   /* "lwlg" */

/* Control bits in the control flags */
#define LWLOG_ENABLED        (0x1000)

/* Types of logs */
#define LWLOG_STATIC         (1)
#define LWLOG_DYNAMIC        (2)

/*--------------------------------------------------------------------------*/
/*                      DATA STRUCTURE DEFINITIONS                          */

/* 
** LOG HEADER STRUCT
**
** This structure is stored at the front of each log to provide
** information about the current state of the log
*/
typedef struct lwlog_header_struct
{

   /* Control flags for the log */
   _mqx_uint                FLAGS;     

   /* The sequence number for next write */
   _mqx_uint                NUMBER;    

   /* The number of entries currently in use */
   _mqx_uint                CURRENT_ENTRIES;

   /* The maximum number of log entries */
   _mqx_uint                MAX_ENTRIES;

   /* How many read nexts in row have been done */
   _mqx_uint                READS;

   /* The type of log */
   _mqx_uint                TYPE;

   LWLOG_ENTRY_STRUCT_PTR   WRITE_PTR;
   LWLOG_ENTRY_STRUCT_PTR   READ_PTR;
   LWLOG_ENTRY_STRUCT_PTR   OLDEST_PTR;

   LWLOG_ENTRY_STRUCT       FIRST_ENTRY;

} LWLOG_HEADER_STRUCT, _PTR_ LWLOG_HEADER_STRUCT_PTR;



/* 
** LW LOG COMPONENT STRUCT
**
** This structure is used to store information 
** required for log retrieval. Its address is stored in the kernel component 
** field of the kernel data structure
*/
typedef struct lwlog_component_struct
{
   /* A validation stamp to verify structure correctness */
   _mqx_uint               VALID;

   /* The address of the log headers */
   LWLOG_HEADER_STRUCT_PTR LOGS[LOG_MAXIMUM_NUMBER];
   
} LWLOG_COMPONENT_STRUCT, _PTR_ LWLOG_COMPONENT_STRUCT_PTR;

/*--------------------------------------------------------------------------*/
/*                           EXTERNAL DECLARATIONS                          */

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __TAD_COMPILE__
extern _mqx_uint _lwlog_create_internal(_mqx_uint, _mqx_uint, _mqx_uint, 
   LWLOG_HEADER_STRUCT_PTR);
extern _mqx_uint _lwlog_write_internal(_mqx_uint, _mqx_max_type, _mqx_max_type,
    _mqx_max_type, _mqx_max_type, _mqx_max_type, _mqx_max_type, _mqx_max_type);
#endif

#ifdef __cplusplus
}
#endif

#endif
/* EOF */
