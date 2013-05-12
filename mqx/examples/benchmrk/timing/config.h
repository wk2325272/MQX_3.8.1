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
* $FileName: config.h$
* $Version : 3.8.18.4$
* $Date    : Feb-27-2012$
*
* Comments:
*
*   This include file is used to define the configuration for the 
*   timing tests
*
*END************************************************************************/

#ifndef __config_h__
    #define __config_h__

#define TEST_ARRAY_DIV              1
#define LOOP_OVERHEAD_AVG_SIZE      3

extern _mqx_uint ints;

/*=========================================================================*/
/* Define the Compiler being used */

#if defined(__CODEWARRIOR__)
    #if defined(PSP_COLDFIRE)
        #define tCOMPILER "Freescale CodeWarrior for ColdFire"
    # elif defined(PSP_POWERPC)
        #define tCOMPILER "Freescale CodeWarrior for PowerPC"
    #else
        #error undefined Codewarrior target
    #endif
#elif defined(__IAR_SYSTEMS_ICC__)
    #if defined(__ICCARM__)
        #define tCOMPILER       "IAR Embedded Workbench for ARM"
    #elif defined(__ICCCF__)
        #define tCOMPILER       "IAR Embedded Workbench for ColdFire"    
    #endif
    #define tCOMPILER_VERION    __VER__
#else
    #error "tCOMPILER has not been defined at the start of config.h"
#endif

/*=========================================================================*/
/* Define the BSP being used*/
#if BSP_M52259EVB
    /* M52259 Evaluation board */
    #define TEST_ARRAY_SIZE        20
    #define TASK_ARRAY_SIZE        20
    
    #define tBOARD                  "Freescale M52259EVB"
    #define tCPU                    "MCF52259" 
    #define tMEMORY                 "64K Internal RAM"
    
    #define INTERRUPT_VECTOR        MCF5225_INT_I2C0
    #define MORE_STACK              200
    
    vuint_32_ptr int_request;
    
    void setup_interrupt(void);
    void generate_interrupt (void);
    void interrupt_ack(pointer isr_value);
    void setup_interrupt(void) 
    {
        int_request = &(PSP_GET_ICTRL0_BASE()->ICTRL.INTFRCL);
        _mcf5225_int_init (INTERRUPT_VECTOR, 1, 1, TRUE);
    }
    
    void generate_interrupt (void) 
    {
        *int_request = (1 << 17);
    }
    
    void interrupt_ack(pointer isr_value) 
    {
        ++ints;
        *int_request = 0;
    }
#endif /* BSP_M52259EVB */
    
/* Define the BSP being used*/
#if BSP_TWRMCF52259
    /* TWRMCF52259 board */
    #define TEST_ARRAY_SIZE        20
    #define TASK_ARRAY_SIZE        20

    #define tBOARD                  "Freescale TWRMCF52259"
    #define tCPU                    "MCF52259"
    #define tMEMORY                 "64K Internal RAM"

    #define INTERRUPT_VECTOR        MCF5225_INT_I2C0
    #define MORE_STACK              200

    vuint_32_ptr int_request;

    void setup_interrupt(void);
    void generate_interrupt (void);
    void interrupt_ack(pointer isr_value);
    void setup_interrupt(void)
    {
        int_request = &(PSP_GET_ICTRL0_BASE()->ICTRL.INTFRCL);
        _mcf5225_int_init (INTERRUPT_VECTOR, 1, 1, TRUE);
    }
    
    void generate_interrupt (void)
    {
        *int_request = (1 << 17);
    }

    void interrupt_ack(pointer isr_value)
    {
        ++ints;
        *int_request = 0;
    }
#endif /* BSP_TWRMCF52259 */

#if BSP_M54455EVB
    /* M54455EVB Evaluation board */
    
    #define TEST_ARRAY_SIZE        50
    #define TASK_ARRAY_SIZE        50
    
    #define tBOARD                  "Freescale M54455EVB"
    #define tCPU                    "MCF54455" 
    #define tMEMORY                 "32K Internal RAM"
    
    #define INTERRUPT_VECTOR        MCF5445_INT_I2C
    #define MORE_STACK              200  
    
    vuint_32_ptr int_request;
    
    void setup_interrupt(void);
    void generate_interrupt (void);
    void interrupt_ack(pointer isr_value);
    
    void setup_interrupt(void) 
    {
        int_request = &(PSP_GET_ICTRL0_BASE()->ICTRL.INTFRCL);
        _mcf5445_int_init (INTERRUPT_VECTOR, 1, TRUE);
    }
    
    void generate_interrupt (void) 
    {
        *int_request = (1 << 30);
    }
    
    void interrupt_ack(pointer isr_value) 
    {
        ++ints;
        *int_request = 0;
    }
#endif /* BSP_M54455EVB */

#if BSP_TWRMCF54418
    /* BSP_TWRMCF54418 board */

    #define TEST_ARRAY_SIZE        50
    #define TASK_ARRAY_SIZE        50

    #define tBOARD                  "Freescale BSP_TWRMCF54418"
    #define tCPU                    "MCF54418"
    #define tMEMORY                 "128 MB DDR2 SDRAM"

    #define INTERRUPT_VECTOR        MCF5441_INT_I2C1
    #define MORE_STACK              200

    vuint_32_ptr int_request;

    void setup_interrupt(void);
    void generate_interrupt (void);
    void interrupt_ack(pointer isr_value);

    void setup_interrupt(void)
    {
        int_request = &(PSP_GET_ICTRL1_BASE()->ICTRL.INTFRCH);
        _mcf5441_int_init(INTERRUPT_VECTOR, 1, TRUE);
    }

    void generate_interrupt (void)
    {
        *int_request = (1 << 25);
    }
    
    void interrupt_ack(pointer isr_value)
    {
        ++ints;
        *int_request = 0;
    }
#endif /* BSP_TWRMCF54418 */

#if BSP_TWRMCF51CN
    /* TWRMCF51CN board */
    
    #define TEST_ARRAY_SIZE         8
    #define TASK_ARRAY_SIZE         8
    
    #define tBOARD                  "Freescale TWRMCF51CN"
    #define tCPU                    "MCF51CN128" 
    #define tMEMORY                 "24K Internal RAM"
    
    #define INTERRUPT_VECTOR        MCF51CN_INT_Vtrap0
    #define MORE_STACK              200

    vuint_32_ptr int_request;
    
    void setup_interrupt(void);
    void generate_interrupt (void);
    void interrupt_ack(pointer isr_value);

    void setup_interrupt(void) 
    {
    }
    
    void generate_interrupt (void) 
    {
        asm 
        {
           trap #0
        }
    }
    
    void interrupt_ack(pointer isr_value) 
    {
        ++ints;
    }
#endif  /* BSP_TWRMCF51CN */
    
#if PSP_MQX_CPU_IS_KINETIS

    #if defined(__IAR_SYSTEMS_ICC__)
        #ifndef NDEBUG  /* NDEBUG is automatically defined in Release version */  
            #define _DEBUG                  1    
        #endif
    
        #if FLASH_TARGET
            #define _TARGET                 "Int Flash"
        #else
            #define _TARGET                 "Int RAM"
        #endif
    #endif
    
    /* Only Register ABI is used */
    #define _ABI                    "RegABI"    

    /* K60N512 board is used for testing */
    #if BSP_TWR_K60N512 == 1
        #define tBOARD              "Freescale TWRK60N512"
        #if     MQX_CPU == PSP_CPU_MK60DN512Z
            /* MK60N512 device */
            #define tCPU            "MK60DN512Z" 
            #define tMEMORY         "128K Internal RAM"
        #elif   MQX_CPU == PSP_CPU_MK60DN256Z
            /* MK60N512 device */
            #define tCPU            "MK60DN256Z" 
            #define tMEMORY         "64K Internal RAM"
        #endif
    /* K40X256 board is used for testing */
    #elif BSP_TWR_K40X256 == 1   
        #define tBOARD              "Freescale TWRK40X256"

        #if (MQX_CPU == PSP_CPU_MK40DX256Z)
            /* For MK40X256 device */
            #define tCPU            "MK40DX256Z" 
            #define tMEMORY         "64K Internal RAM"
        #endif
    /* MK70F120M board is used for testing */
    #elif BSP_TWR_K70F120M == 1
        #define tBOARD              "Freescale TWRK70F120M"
        #define tCPU                "MK70F120M"

        #if DDR_DATA_TARGET
            #define tMEMORY          "256MB External DDR2 RAM"
        #else
            #define tMEMORY          "128K Internal SRAM"
        #endif
    #endif

    #define TEST_ARRAY_SIZE         50
    #define TASK_ARRAY_SIZE         50
    
    #define INTERRUPT_VECTOR        79
    #define MORE_STACK              200

    #define SYSTEM_CLOCK           (BSP_SYSTEM_CLOCK / 1000000)       /* system bus frequency in MHz */
    
    
    vuint_32_ptr int_request;
    
    void setup_interrupt(void);
    void generate_interrupt (void);
    void interrupt_ack(pointer isr_value);
    
    void setup_interrupt(void) 
    {
#if 0
        /* 100MHz / 50kHz = 2000. */
        FTM1_MOD = 0x03C0;                                  
        FTM1_C0V = 0x0300;
        FTM1_CNT = 0x0000;
        
        /* PWM, true high pulses  */
        FTM1_C0SC |= FTM_CnSC_MSB_MASK | FTM_CnSC_ELSB_MASK;  
        /* System Clock = 48MHz, divide-by-1 */
        FTM1_SC |= FTM_SC_CLKS(0x01) | FTM_SC_PS(0x00);
        
        /* FTM1 is vector 79 and external interrupt 63. So it is enabled in NVICISER1, bit 31 */
        NVICICPR1 |= (1 << 31);     /* Clear any pending interrupts */
        NVICISER1 |= (1 << 31);     /* Enable interrupt for FTM1.   */
        NVICIP79 = 0x00;            /* Set to highest priority.     */
        
        SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK;
        PORTA_PCR8  |= 0x300;       /* FTM1_CH0    PortA8   A34     */
#endif        
        _bsp_int_init(INT_FTM1, 2, 0, TRUE);
    }
    
    void generate_interrupt (void) 
    {
         /* Enable interrupts in the FTM1 module */
        //FTM1_SC |= FTM_SC_TOIE_MASK;
        NVICSTIR = NVIC_STIR_INTID(INTERRUPT_VECTOR - 16);
    }
    
    void interrupt_ack(pointer isr_value) 
    {
        /* Acknowledge Interrupt */
        //FTM1_SC &= ~FTM_SC_TOF_MASK;
        //FTM1_SC &= ~FTM_SC_TOIE_MASK;
        ++ints;
    }
#endif /* PSP_MQX_CPU_IS_KINETIS */

#if BSP_TWRMPC5125
    /* BSP_TWRMPC5125 */
    
    #define TEST_ARRAY_SIZE        50
    #define TASK_ARRAY_SIZE        50
    
    #define tBOARD                  "Freescale TWR-MPC5125"
    #define tCPU                    "MPC5125" 
    #define tMEMORY                 "256MB DRAM"
    
    #define INTERRUPT_VECTOR        MPC5125_GPT0_VECTOR
    #define MORE_STACK              200  
    
    vuint_32_ptr int_request;
    
    void setup_interrupt(void);
    void generate_interrupt (void);
    void interrupt_ack(pointer isr_value);

    void setup_interrupt(void) {
   		//MPC5125_GPT_CHANNEL_STRUCT_PTR   chan_ptr;
   		//chan_ptr = (MPC5125_GPT_CHANNEL_STRUCT_PTR)_mpc512x_gpt_channel_address(0);
   		
   		/* Set Prescaler and Count values */
   		//chan_ptr->CIOR = 0x00010001;
   
   		/* Enable clock and Interrupt */
   		//chan_ptr->EMSR = MPC5125_GPT_EMSR_CE | MPC5125_GPT_EMSR_INTEN | MPC5125_GPT_EMSR_TIMER_MS_GPIO;   
                      
   		/* Enable Timer Interrupt at IPIC level  */
   		_mpc5125_enable_interrupt(INTERRUPT_VECTOR);
    }
    
    void generate_interrupt(void) {
		MPC5125_IPIC_STRUCT_PTR ipic_ptr = MPC5125_IPIC_ADDR();
		ipic_ptr->SIFCRH |= 2;
    }
    
    void interrupt_ack(pointer parameter) {
    	MPC5125_IPIC_STRUCT_PTR ipic_ptr = MPC5125_IPIC_ADDR();
		ipic_ptr->SIFCRH &= ~2;
		
        ++ints;
        //*int_request = 0;
    }
    
    #define SYSTEM_CLOCK           (BSP_SYSTEM_CLOCK/1000000)       /* system bus frequency in MHz */
    
#elif PSP_POWERPC 
   #if BSP_MPC8308RDB == 1
      #define tBOARD    "Freescale MPC8308RDB"
      #define tCPU      "MPC8308" 
      #define tMEMORY   "256MB DDR SDRAM"
   #elif BSP_MPC8306SOM == 1
      #define tBOARD    "Freescale MPC8306SOM"
      #define tCPU      "MPC8306" 
      #define tMEMORY   "256MB DDR SDRAM"
   #elif BSP_MPC8309SOM == 1
      #define tBOARD    "Freescale MPC8309SOM"
      #define tCPU      "MPC8309" 
      #define tMEMORY   "256MB DDR SDRAM"
   #elif BSP_TWRMPC5125 == 1
      #define tBOARD    "Freescale TWRMPC5125"
      #define tCPU      "MPC5125" 
      #define tMEMORY   "256MB DDR SDRAM"
   #elif BSP_TWRPXS20 == 1
      #define _TARGET   "Flash"
   #elif BSP_TWRPXS30 == 1
      #define _TARGET   "Flash"
   #endif
   
   // No choice of ABI on PowerPC
   #define _ABI   ""
   
   // We execute from DDR on PowerPC, unless defined otherwise
   #ifndef _TARGET
   #define _TARGET "Ram"
   #endif
   
   #define TEST_ARRAY_SIZE        50
   #define TASK_ARRAY_SIZE        50
   #define INTERRUPT_VECTOR  PSP_EXCPT_SYSTEM_CALL
   #define MORE_STACK 200

   vuint_32_ptr int_request;

   void setup_interrupt(void);
   void generate_interrupt (void);
   void interrupt_ack(pointer);
   void setup_interrupt(void) 
   {
   }

   void generate_interrupt (void) 
   {
     asm {
       sc
       blr
     }
   }

   void interrupt_ack(pointer) 
   {
       ++ints;
   }
#endif

#ifndef tBOARD
    #error "tBOARD has not been defined at the start of config.h"
#endif

#ifndef MORE_STACK
    #error "missing psp and compiler specification"
#endif

    
/*=========================================================================*/
/* Target settings */

#ifdef _TARGET
    #define tTARGET   _TARGET
#else
    #define tTARGET   "Unknown"
#endif

#ifdef _DEBUG
    #define tOPTIM    "Debug"
#else
    #define tOPTIM    "Release"
#endif

#ifdef _ABI
    #define tABI      _ABI
#else
    #define tABI      "Unknown"
#endif    
    
    
/*=========================================================================*/

/* Define the PSP being used */

#if MQX_USE_LWMEM_ALLOCATOR
    #define MEMBLOCK_STRUCT LWMEM_BLOCK_STRUCT
#else
    #define MEMBLOCK_STRUCT STOREBLOCK_STRUCT
#endif

#define tKERNEL_RAM       sizeof(KERNEL_DATA_STRUCT)
#define tISR              sizeof(INTERRUPT_TABLE_STRUCT)
#define tTASK             sizeof(TD_STRUCT)
#define tMEMORY_BLOCK     sizeof(MEMBLOCK_STRUCT)
#define tPARTITION        sizeof(INTERNAL_PARTITION_BLOCK_STRUCT)
#define tTASKQ            sizeof(TASK_QUEUE_STRUCT)
#define tMESSAGE          sizeof(INTERNAL_MESSAGE_STRUCT)
#define tMSGQ             sizeof(MSGQ_STRUCT)
#define tMUTEX            sizeof(MUTEX_STRUCT)
#define tSEM              sizeof(SEM_STRUCT)+sizeof(NAME_STRUCT)
#define tEVENT            sizeof(EVENT_STRUCT)+sizeof(NAME_STRUCT)
#define tTIMER            sizeof(TIMER_ENTRY_STRUCT)

#define tLWEVENT          sizeof(LWEVENT_STRUCT)
#define tLWSEM            sizeof(LWSEM_STRUCT)
#define tLWMEM            sizeof(LWMEM_BLOCK_STRUCT)
#define tLWTIMER          sizeof(LWTIMER_STRUCT)
    
/*=========================================================================*/

#ifndef ALIGN
    #define ALIGN()
#endif

#endif /* __config_h__ */
/* EOF */
