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
* $FileName: psp_supp.c$
* $Version : 3.7.5.0$
* $Date    : Feb-7-2011$
*
* Comments:
*
*   This file contains the support functions for byte/word and others 
*   manipulations.
*
*END************************************************************************/

#include "mqx_inc.h"

#ifdef __ICCCF__

uint_16 _psp_swap2byte(uint_16 n)
{
#if __STDABI__ == 1
    asm ("move.l 4(sp), d0");
#endif /* __STDABI__ */
    asm ("byterev.l d0");
    asm ("swap d0");
    asm ("rts");
}

uint_32 _psp_swap4byte(uint_32 n)
{
#if __STDABI__ == 1
    asm ("move.l 4(sp), d0");
#endif /* __STDABI__ */
    asm ("byterev.l d0");
    asm ("rts");
}

#elif defined(__CODEWARRIOR__)

#if   __REGABI__
    __declspec(register_abi)
#elif __STDABI__
      __declspec(standard_abi)
#endif

asm uint_16 _psp_swap2byte(uint_16 val)
{
#if __STDABI__ == 1
    move.l 4(sp), d0
#endif /* __STDABI__ */
    byterev.l d0
    swap d0
    rts    
}

#if   __REGABI__
    __declspec(register_abi)
#elif __STDABI__
      __declspec(standard_abi)
#endif

asm uint_32 _psp_swap4byte(uint_32 val)
{
#if __STDABI__ == 1
    move.l 4(sp), d0
#endif /* __STDABI__ */
    byterev.l d0
    rts    
}

#else /* __ICCCF__, __CODEWARRIOR__ */

/*FUNCTION*-------------------------------------------------------------------
 * 
 * Function Name    : _psp_swap2byte
 * Returned Value   : uint_16
 * Comments         : Swap 2 bytes
 *   ticks
 *
 *END*----------------------------------------------------------------------*/
uint_16 _psp_swap2byte(uint_16 n)
{ /* Body */
    uchar temp[2];

    temp[1] = *((uchar *)&n);
    temp[0] = ((uchar *)&n)[1];
    return (*(uint_16 *)temp);
} /* Endbody */

/*FUNCTION*-------------------------------------------------------------------
 * 
 * Function Name    : _psp_swap4byte
 * Returned Value   : uint_32
 * Comments         : Swap 4 bytes
 *   ticks
 *
 *END*----------------------------------------------------------------------*/
uint_32 _psp_swap4byte(uint_32 n)
{ /* Body */
    uchar temp[4];

    temp[3] = *((uchar *)&n);
    temp[2] = ((uchar *)&n)[1];
    temp[1] = ((uchar *)&n)[2];
    temp[0] = ((uchar *)&n)[3];
    return (*(uint_32 *)temp);
} /* Endbody */

#endif /* __ICCCF__, __CODEWARRIOR__ */
