/**HEADER********************************************************************
* 
* Copyright (c) 2008 Freescale Semiconductor;	  
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
* $FileName: linker_symbols.s$
* $Version : 3.8.1.0$
* $Date    : Aug-30-2011$
*
* Comments:
*
*   This file contains the symbols used to set addresses based on their
* location in the linker file.  For example, we put the KERNEL_DATA_START
* section after everything else in ram, and then its address tells us 
* where the balance of the free RAM starts for the kernel data.
*
*END************************************************************************/

  AREA KERNEL_DATA_START, DATA, ALIGN=4, READWRITE
  EXPORT __KERNEL_DATA_START
__KERNEL_DATA_START
  DCD 0

  AREA KERNEL_DATA_END, DATA, ALIGN=4, READWRITE
  EXPORT __KERNEL_DATA_END
__KERNEL_DATA_END
  DCD 0

  AREA BOOT_STACK, DATA, ALIGN=4, READWRITE
  EXPORT __BOOT_STACK_ADDRESS
__BOOT_STACK_ADDRESS
  DCD 0

  END

; EOF
