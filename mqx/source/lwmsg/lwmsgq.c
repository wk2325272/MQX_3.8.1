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
* $FileName: lwmsgq.c$
* $Version : 3.8.5.0$
* $Date    : Aug-30-2011$
*
* Comments:
*
*   This file provides a simple light weight message queue.
*   A message in the message pool has a fixed size, a multiple of 32 bits.
*   Blocking reads, and Blocking writes are provided.
*
*END************************************************************************/

#include "mqx_inc.h"
#if MQX_USE_LWMSGQ
#include "lwmsgq.h"
#include "lwmsgqpr.h"

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : _lwmsgq_init
* Returned Value   : error code
* Comments         :
*   This function initializes the light weight message queue
*
*END*----------------------------------------------------------------------*/

_mqx_uint _lwmsgq_init_internal
   (
      /* The starting location of the message pool */
      pointer   location,

      /* Number of messages created after the header */
      _mqx_uint num_messages,

      /* The size of the messages in _mqx_max_type's */
      _mqx_uint msg_size,
      
      /* usermode functionality */
      boolean user
   )
{
    /* Start CR 1944 */
    KERNEL_DATA_STRUCT_PTR kernel_data;
    /* End CR 1944 */
    LWMSGQ_STRUCT_PTR q_ptr = (LWMSGQ_STRUCT_PTR)location;   
    /* START CR 1896 */
    LWMSGQ_STRUCT_PTR lwmsg_chk_ptr;
    /* END CR 1896 */

#if MQX_ENABLE_USER_MODE
    if (user && !_psp_mem_check_access_mask((uint_32)q_ptr, sizeof(LWMSGQ_STRUCT), MPU_UM_R, MPU_UM_RW)) {
        return MQX_EINVAL;
    }
#endif

    /* Start CR 1944 */
    _GET_KERNEL_DATA(kernel_data);
    _KLOGE4(KLOG_lwmsgq_init, location, num_messages, msg_size);
    /* End CR 1944 */
    
    _QUEUE_INIT(&q_ptr->WAITING_WRITERS,0);
    _QUEUE_INIT(&q_ptr->WAITING_READERS,0);
    q_ptr->MSG_SIZE      = msg_size;
    q_ptr->MAX_SIZE      = num_messages;
    q_ptr->CURRENT_SIZE  = 0;
    q_ptr->MSG_START_LOC = (_mqx_max_type_ptr)((uchar_ptr)q_ptr + sizeof(LWMSGQ_STRUCT));
    q_ptr->MSG_END_LOC   = q_ptr->MSG_START_LOC + msg_size * num_messages;
    q_ptr->MSG_WRITE_LOC = q_ptr->MSG_START_LOC;
    q_ptr->MSG_READ_LOC  = q_ptr->MSG_START_LOC;

#if MQX_ENABLE_USER_MODE
    if (user) {
        if (kernel_data->USR_LWMSGQS.NEXT == NULL) {
            /* Initialize the light weight message queue */
            _QUEUE_INIT(&kernel_data->USR_LWMSGQS, 0);
        }
    }
    else 
#endif
    {
        if (kernel_data->LWMSGQS.NEXT == NULL) {
            /* Initialize the light weight message queue */
            _QUEUE_INIT(&kernel_data->LWMSGQS, 0);
        }
    }

    q_ptr->VALID         = LWMSGQ_VALID;

#if MQX_CHECK_ERRORS
    /* Check if lwmsgq is already initialized */
#if MQX_ENABLE_USER_MODE
    if (user) {
        lwmsg_chk_ptr = (LWMSGQ_STRUCT_PTR)((pointer)kernel_data->USR_LWMSGQS.NEXT);
        while (lwmsg_chk_ptr != (LWMSGQ_STRUCT_PTR)((pointer)&kernel_data->USR_LWMSGQS)) {
            if (lwmsg_chk_ptr == q_ptr) {
                _KLOGX2(KLOG_lwmsgq_init, MQX_EINVAL);
                return(MQX_EINVAL);
            }
            lwmsg_chk_ptr = (LWMSGQ_STRUCT_PTR)((pointer)lwmsg_chk_ptr->LINK.NEXT);
        }
    }
    else
#endif // MQX_ENABLE_USER_MODE
    {
        lwmsg_chk_ptr = (LWMSGQ_STRUCT_PTR)((pointer)kernel_data->LWMSGQS.NEXT);
        while (lwmsg_chk_ptr != (LWMSGQ_STRUCT_PTR)((pointer)&kernel_data->LWMSGQS)) {
            if (lwmsg_chk_ptr == q_ptr) {
                _KLOGX2(KLOG_lwmsgq_init, MQX_EINVAL);
                return(MQX_EINVAL);
            }
            lwmsg_chk_ptr = (LWMSGQ_STRUCT_PTR)((pointer)lwmsg_chk_ptr->LINK.NEXT);
        }
    }
#endif

#if MQX_ENABLE_USER_MODE
    if (user) {
        _QUEUE_ENQUEUE(&kernel_data->USR_LWMSGQS, &q_ptr->LINK);
    }
    else 
#endif // MQX_ENABLE_USER_MODE
    {
        _QUEUE_ENQUEUE(&kernel_data->LWMSGQS, &q_ptr->LINK);
    }
   
    _KLOGX2(KLOG_lwmsgq_init, MQX_OK);
   
    return MQX_OK;
}

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : _lwmsgq_init
* Returned Value   : error code
* Comments         :
*   This function initializes the light weight message queue
*
*END*----------------------------------------------------------------------*/

_mqx_uint _lwmsgq_init
    (
        /* The starting location of the message pool */
        pointer location,

        /* Number of messages created after the header */
        _mqx_uint num_messages,

        /* The size of the messages in _mqx_max_type's */
        _mqx_uint msg_size
    )
{
#if MQX_ENABLE_USER_MODE && MQX_ENABLE_USER_STDAPI
    if (MQX_RUN_IN_USER_MODE) {
        return _usr_lwmsgq_init(location, num_messages, msg_size);
    }
#endif
    
    return _lwmsgq_init_internal(location, num_messages, msg_size, FALSE);
}

#if MQX_ENABLE_USER_MODE

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : _usr_lwmsgq_init
* Returned Value   : error code
* Comments         :
*   This function initializes the light weight message queue
*
*END*----------------------------------------------------------------------*/

_mqx_uint _usr_lwmsgq_init
    (
        /* The starting location of the message pool */
        pointer location,

        /* Number of messages created after the header */
        _mqx_uint num_messages,

        /* The size of the messages in _mqx_max_type's */
        _mqx_uint msg_size
    )
{
    MQX_API_CALL_PARAMS params = {(uint_32)location, (uint_32)num_messages, (uint_32)msg_size, 0, 0};
    return _mqx_api_call(MQX_API_LWMSGQ_INIT, &params);
}

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : _lwmsgq_usr_check
* Returned Value   : 
* Comments         :
*
*END*----------------------------------------------------------------------*/

_mqx_uint _lwmsgq_usr_check(pointer tst_lwmsgq_ptr)
{

    KERNEL_DATA_STRUCT_PTR kernel_data;
    LWMSGQ_STRUCT_PTR lwmsgq_ptr;
    _mqx_uint result = MQX_EINVAL;
    _mqx_uint queue_size;

    _GET_KERNEL_DATA(kernel_data);                                         

    lwmsgq_ptr = (LWMSGQ_STRUCT_PTR)((pointer)kernel_data->USR_LWMSGQS.NEXT);
    queue_size = _QUEUE_GET_SIZE(&kernel_data->USR_LWMSGQS);
    
    while (queue_size--) {
        if (lwmsgq_ptr->VALID != LWMSGQ_VALID) {
            break;
        }
        
        if (tst_lwmsgq_ptr == lwmsgq_ptr) {
            result = MQX_OK;
            break;
        }
        
        lwmsgq_ptr = (LWMSGQ_STRUCT_PTR)(pointer)lwmsgq_ptr->LINK.NEXT;
    }
    
    return result;
}

#endif // MQX_ENABLE_USER_MODE



#endif /* MQX_USE_LWMSGQ */
