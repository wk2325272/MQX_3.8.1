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
* $FileName: int_sr.c$
* $Version : 3.0.2.0$
* $Date    : Aug-19-2008$
*
* Comments:
*
*   This file contains functions to manipulate the SR register
*
*END************************************************************************/

#include "mqx_inc.h"

/*FUNCTION*---------------------------------------------------------------------
*
* Function Name    : _psp_set_sr
* Returned Value   : old sr value
* Comments         :
*  sets the value of the SR register and returns the old value
*
*END*-------------------------------------------------------------------------*/

uint_32 _psp_set_sr
   (
      /* [IN] the new value for the status register */
      uint_32 sr_value
   )
{ /* Body */
   uint_32 old_sr_value;

   _PSP_GET_SR(old_sr_value);
   _PSP_SET_SR(sr_value);
   return(old_sr_value);

} /* Endbody */


/*FUNCTION*---------------------------------------------------------------------
*
* Function Name    : _psp_get_sr
* Returned Value   : sr value
* Comments         :
*  returns the current SR register value
*
*END*-------------------------------------------------------------------------*/

uint_32 _psp_get_sr
   (
      void
   )
{ /* Body */
   uint_32 old_sr_value;

   _PSP_GET_SR(old_sr_value);
   return(old_sr_value);

} /* Endbody */

/* EOF */
