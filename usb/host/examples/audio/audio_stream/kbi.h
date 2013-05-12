/******************************************************************************
 *
 * Freescale Semiconductor Inc.
 * (c) Copyright 2004-2010 Freescale Semiconductor, Inc.
 * ALL RIGHTS RESERVED.
 *
 ******************************************************************************
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
 **************************************************************************//*!
 *
 * @file kbi.h
 *
 * @author
 *
 * @version
 *
 * @date    
 *
 * @brief   
 *          
 *****************************************************************************/
#include"audio.h"
#ifndef _KBI_H_
#define _KBI_H_

#if (defined BSP_M52223EVB)
#define MCF5222_DTIM_DTMR_RST                    (0x1)
#define MCF5222_DTIM_DTMR_CLK(x)                 (((x)&0x3)<<0x1)
#define MCF5222_DTIM_DTMR_CLK_STOP               (0)
#define MCF5222_DTIM_DTMR_CLK_DIV1               (0x2)
#define MCF5222_DTIM_DTMR_CLK_DIV16              (0x4)
#define MCF5222_DTIM_DTMR_CLK_DTIN               (0x6)
#define MCF5222_DTIM_DTMR_FRR                    (0x8)
#define MCF5222_DTIM_DTMR_ORRI                   (0x10)
#define MCF5222_DTIM_DTMR_OM                     (0x20)
#define MCF5222_DTIM_DTMR_CE(x)                  (((x)&0x3)<<0x6)
#define MCF5222_DTIM_DTMR_CE_NONE                (0)
#define MCF5222_DTIM_DTMR_CE_RISE                (0x40)
#define MCF5222_DTIM_DTMR_CE_FALL                (0x80)
#define MCF5222_DTIM_DTMR_CE_ANY                 (0xC0)
#define MCF5222_DTIM_DTMR_PS(x)                  (((x)&0xFF)<<0x8)
#endif

void switch_init(void);
void polling_switch(void);

#endif
