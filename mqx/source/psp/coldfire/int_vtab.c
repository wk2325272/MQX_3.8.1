/**HEADER********************************************************************
* 
* Copyright (c) 2008 Freescale Semiconductor;
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
* $FileName: int_vtab.c$
* $Version : 3.5.3.0$
* $Date    : Dec-8-2009$
*
* Comments:
*
*   This file contains the functions used to access the vector
*   table locations.
*
*END************************************************************************/

#include "mqx_inc.h"

#if MQX_USE_INTERRUPTS

/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _int_set_vector_table
* Returned Value   : pointer old_vector table address
* Comments         :
*    This function changes the VBR to point to the new address
*  and returns the old interuupt vector table pointer (VBR) value.
*
************************************************************************/

_mqx_max_type _int_set_vector_table
   (
      /* [IN] the new address for the vector table */
      _mqx_max_type new
   )
{ /* Body */
   _mqx_max_type old;

   _PSP_GET_VBR(old);
   _PSP_SET_VBR(new);

   return (old);

} /* Endbody */


/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _int_get_vector_table
* Returned Value   : pointer address of vector table
* Comments         :
*    This function returns the current vector table pointer
*
************************************************************************/

_mqx_max_type _int_get_vector_table
   (
      void
   )
{ /* Body */
   _mqx_max_type old;

   _PSP_GET_VBR(old);

   return(old);
   
} /* Endbody */

#endif
/* EOF */
