/*
//*HEADER********************************************************************
//* 
//* Copyright (c) 2008 Freescale Semiconductor;
//* All Rights Reserved                       
//*
//* Copyright (c) 1989-2008 ARC International;
//* All Rights Reserved
//*
//*************************************************************************** 
//*
//* THIS SOFTWARE IS PROVIDED BY FREESCALE "AS IS" AND ANY EXPRESSED OR 
//* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES 
//* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  
//* IN NO EVENT SHALL FREESCALE OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
//* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
//* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR 
//* SERVICES// LOSS OF USE, DATA, OR PROFITS// OR BUSINESS INTERRUPTION) 
//* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
//* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
//* IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF 
//* THE POSSIBILITY OF SUCH DAMAGE.
//*
//**************************************************************************
//*
//* $FileName: asm_offsets.h$
//* $Version : 3.8.4.0$
//* $Date    : Aug-1-2011$
//*
//* Comments:
//*
//*   This file contains the assembler offsets to MQX data structures.
//*   These offsets are to be included in any assembler program that
//*   wishes to access kernel data structures.
//*   
//*
//*END***********************************************************************
*/

//
// 'PSP_INT_CONTEXT_STRUCT' size = 12 , 0xc
//
#define IC_STRUCT_SIZE                         12  //  0xc
#define IC_ERROR_CODE                           0  //  0x0
#define IC_ENABLE_SR                            4  //  0x4
#define IC_PREV_CONTEXT                         8  //  0x8


//
// 'INTERRUPT_TABLE_STRUCT' size = 12 , 0xc
//
#define IT_STRUCT_SIZE                         12  //  0xc
#define IT_APP_ISR                              0  //  0x0
#define IT_APP_ISR_EXCEPTION_HANDLER            4  //  0x4
#define IT_APP_ISR_DATA                         8  //  0x8

#define HASH_ISR_ITM_SIZE                       16
#define HASH_ISR_NUM                            0
#define HASH_ISR_ADDR                           4
#define HASH_ISR_DATA                           8
#define HASH_ISR_NEXT                           12

//
// 'READY_Q_STRUCT' size = 16 , 0x10
//
#define RQ_STRUCT_SIZE                         16  //  0x10
#define RQ_HEAD_READY_Q                         0  //  0x0
#define RQ_TAIL_READY_Q                         4  //  0x4
#define RQ_NEXT_Q                               8  //  0x8
#define RQ_ENABLE_SR                           12  //  0xc
#define RQ_PRIORITY                            14  //  0xe


//
// 'TD_STRUCT' size = 216 , 0xd8
//
#define TD_TD_NEXT                              0  //  0x0
#define TD_TD_PREV                              4  //  0x4
#define TD_STATE                                8  //  0x8
#define TD_TASK_ID                             12  //  0xc
#define TD_STACK_BASE                          16  //  0x10
#define TD_STACK_PTR                           20  //  0x14
#define TD_STACK_LIMIT                         24  //  0x18
#define TD_MY_QUEUE                            28  //  0x1c
#define TD_HOME_QUEUE                          32  //  
#define TD_TASK_SR                             36  //  uint_16
#define TD_RESERVED1                           38  //  uint_16
#define TD_FLAGS                               40  //  

//
// 'KERNEL_DATA_STRUCT' size = 1156 , 0x484
//
//KD_ADDRESSING_CAPABILITY                0  //  0x0
//KD_ENDIANESS                            4  //  0x4
//KD_CPU_TYPE                             8  //  0x8
//KD_PSP_CFG_MEMORY_ALIGNMENT            10  //  0xa
//KD_PSP_CFG_STACK_ALIGNMENT             12  //  0xc
//KD_PSP_CFG_MEM_STOREBLOCK_ALIGNMENT    14  //  0xe
//KD_CONFIG1                             16  //  0x10
#define KD_CONFIG2                             18  //  0x12
#define KD_FLAGS                               20  //  0x14
#define KD_DISABLE_SR                          22  //  0x16
#define KD_IN_ISR                              24  //  0x18
#define KD_ACTIVE_SR                           26  //  0x1a
#define KD_ACTIVE_PTR                          28  //  0x1c
#define KD_READY_Q_LIST                        32  //  0x20
#define KD_CURRENT_READY_Q                     36  //  0x24
#define KD_DEFAULT_ISR                         40  //  0x28
#define KD_FIRST_USER_ISR_VECTOR               44 //  0x2c
#define KD_LAST_USER_ISR_VECTOR                48 //  0x30
#define KD_INTERRUPT_CONTEXT_PTR               52 //  0x34
#define KD_INTERRUPT_TABLE_PTR                 56 //  0x38
#define KD_INTERRUPT_STACK_PTR                 60 //  0x3c
#define KD_LOG_CONTROL                           64
#define LOG_OLD_TD                              68
#define KD_FP_ACTIVE_PTR                        72
#define KD_DSP_ACTIVE_PTR                       76
#define KD_SYSTEM_TD                            80


// These size and offsets must agree with the psp_interrupt_frame_struct
#define IF_STACK_POINTER         0
#define IF_RETURN_ADDRESS        IF_STACK_POINTER+4
#define IF_INTERRUPT_CONTEXT     IF_RETURN_ADDRESS+4
#define IF_FRAME_POINTER         IF_INTERRUPT_CONTEXT+IC_STRUCT_SIZE
#define IF_FAKE_RETURN_ADDRESS   IF_FRAME_POINTER+4
#define IF_EXCEPTION_NUMBER      IF_FAKE_RETURN_ADDRESS+4
#define IF_STRUCT_SIZE           IF_EXCEPTION_NUMBER+4



//* EOF
