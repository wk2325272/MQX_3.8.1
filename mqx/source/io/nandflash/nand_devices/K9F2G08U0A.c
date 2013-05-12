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
* $FileName: K9F2G08U0A.c$
* $Version : 3.8.2.1$
* $Date    : Feb-13-2012$
*
* Comments:
*
*   The file contains the definitions for the K9F2G08U0A flash device.
*
*END************************************************************************/

#include "mqx.h" 
#include "bsp.h"
#include "nandflash.h"


/* K9F2G08U0A organization */
#define K9F2G08U0A_PHYSICAL_PAGE_SIZE      2048
#define K9F2G08U0A_SPARE_AREA_SIZE         64
#define K9F2G08U0A_BLOCK_SIZE              131072 /* 128 KB */
#define K9F2G08U0A_NUM_BLOCKS              2048
#define K9F2G08U0A_WIDTH                   8


NANDFLASH_INFO_STRUCT _K9F2G08U0A_organization_8bit[] =  {
  K9F2G08U0A_PHYSICAL_PAGE_SIZE,
  K9F2G08U0A_SPARE_AREA_SIZE, 
  K9F2G08U0A_BLOCK_SIZE,
  K9F2G08U0A_NUM_BLOCKS,
  K9F2G08U0A_WIDTH
};

/* EOF*/
