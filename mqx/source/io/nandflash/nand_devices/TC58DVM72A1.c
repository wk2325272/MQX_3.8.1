/**HEADER********************************************************************
* 
* Copyright (c) 2010 Freescale Semiconductor;
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
* $FileName: TC58DVM72A1.c$
* $Version : 3.8.3.1$
* $Date    : Feb-13-2012$
*
* Comments:
*
*   The file contains the definitions for the MT29F2G16 flash device.
*
*END************************************************************************/

#include "mqx.h" 
#include "bsp.h"
#include "nandflash.h"


/* MT29F2G16AABWP organization */
#define TC58DVM72A1_PHYSICAL_PAGE_SIZE      528
#define TC58DVM72A1_SPARE_AREA_SIZE         64
#define TC58DVM72A1_BLOCK_SIZE              (16*1024+512) /* 128kB */
#define TC58DVM72A1_NUM_BLOCKS              1024
#define TC58DVM72A1_WIDTH                   8


NANDFLASH_INFO_STRUCT _TC58DVM72A1_organization_16bit[] =  {
  TC58DVM72A1_PHYSICAL_PAGE_SIZE,
  TC58DVM72A1_SPARE_AREA_SIZE, 
  TC58DVM72A1_BLOCK_SIZE,
  TC58DVM72A1_NUM_BLOCKS,
  TC58DVM72A1_WIDTH
};

/* EOF*/
