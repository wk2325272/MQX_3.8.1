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
* $Version : 3.8.4.1$
* $Date    : Feb-13-2012$
*
* Comments:
*
*   This file contains the functions used to access the vector
*   table locations.
*
*END************************************************************************/

#include "mqx_inc.h"


/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _int_set_vector_table
* Returned Value   : pointer old_vector table address
* Comments         :
*   This function does nothing on the PowerPC.
*
************************************************************************/

_mqx_max_type _int_set_vector_table
   (
      /* [IN] the new address for the vector table */
      _mqx_max_type new
   )
{ /* Body */

   #if defined(PSP_PPC401) || defined(PSP_PPC403) || defined(PSP_PPC405) || \
       defined(PSP_IOP480)

      return (_mqx_max_type)_PSP_SET_VECTOR_TABLE((uint_32)new);

   #else
      return 0;
   #endif

} /* Endbody */


/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _int_get_vector_table
* Returned Value   : _mqx_max_type address of vector table
* Comments         :
*    This function returns the current vector table location
*
************************************************************************/

_mqx_max_type _int_get_vector_table
   (
      void
   )
{ /* Body */

   #if defined(PSP_PPC401) || defined(PSP_PPC403) || defined(PSP_PPC405) || \
       defined(PSP_IOP480)

      return (_mqx_max_type)_PSP_GET_VECTOR_TABLE();

   #else
      uint_32 tmp;
      
      _PSP_MSR_GET(tmp);
      if (tmp & PSP_MSR_IP) {
         return 0xFFF00000;
      } else {
         return 0;
      }/* Endif */
   #endif
   
} /* Endbody */

/* EOF */
