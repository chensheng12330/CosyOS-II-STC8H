/**************************************************************************//**
 * @item     CosyOS-II Port
 * @file     port_8051.h
 * @brief    8051 Core Port File
 * @author   迟凯峰
 * @version  V3.0.1
 * @date     2024.07.07
 ******************************************************************************/

#ifndef __PORT_8051_H
#define __PORT_8051_H

/* Header */
#include <string.h>
#include <intrins.h>
#include "..\System\os_base.h"
#include "..\Config\mcucfg_8051.h"

/* Memory */
#define _SYS_MEM_    data
#define _CODE_MEM_   code
#define _CONST_MEM_  code
#define _STACK_MEM_  idata
#define _XDATA_MEM_  xdata
#define _STATIC_MEM_ xdata
#ifndef _MALLOC_MEM_
#define _MALLOC_MEM_ xdata
#endif
#define _OBJ_MEM_    xdata

/* Register */
#define _SYS_REG_    _SYS_MEM_

/* Typedef */
#define m_boolvoid_tf (s_boolvoid_tf)
typedef bit      m_bit_t;
typedef s_u8_t   m_sp_t;
typedef s_u8_t   m_stacksize_t;
typedef s_u16_t  m_tick_t;
typedef s_u16_t  m_pc_t;
typedef s_u8_t   m_taskmsg_t;
typedef s_u8_t   m_fetion_t;
typedef s_u32_t  m_group_t;

/* Extern */
extern  bit      m_sign_fifo_0_0;
extern  s_u8_t   mPendSV_FIFO_DepthMAX;
extern  void     mx_disable_irq(void);
extern  void     mx_resume_irq (void);
extern  s_bool_t mPendSV_FIFOLoader (s_u16_t sv);
extern  void     mPendSV_FIFOHandler(void);

/* CONST & ATTRIBUTE */
#define MCUCFG_ISA                __MCS_51__
#define MCUCFG_NOP                _nop_()
#define MCUCFG_PCLEN              2
#if MCUCFG_TASK_REGBANK != MCUCFG_SYSINT_REGBANK
#define MCUCFG_C51USING           using MCUCFG_SYSINT_REGBANK
#else
#define MCUCFG_C51USING
#endif
#define MCUCFG_SYSTICK_ATTRIBUTE  interrupt 1 MCUCFG_C51USING
#define MCUCFG_TERNARYMASK        false;

#define MCUCFG_TASKMSG_TYPE       0
#define MCUCFG_TASKMSG_PSP
#define MCUCFG_TASKMSG_SIZE       (s_u8_t)(&mx - &m0 - 1)
#if SYSCFG_TASKMSGMODE == __PERFORMANCE__
#define MCUCFG_TASKMSG_VAR        m_taskmsg_t m0_
#define MCUCFG_TASKMSG_VAL        1
#elif SYSCFG_TASKMSGMODE == __INTELLIGENT__
#define MCUCFG_TASKMSG_VAR        m_taskmsg_t R3_, m_taskmsg_t R5_, m_taskmsg_t R7_, m_taskmsg_t m0_
#define MCUCFG_TASKMSG_VAL        0, 0, 0, 1
#endif

/** !=2: \PUSH {PC,A,B,DPH,DPL,PSW}, \SAVE {TASK-REGBANK:R0-R7,[USERREG(ASM)],[?C_XBP]} */
/** !=1: \PUSH {PC,A,B,DPH,DPL,PSW,[USERREG(ASM)]}, \SAVE {TASK-REGBANK:R0-R7,[?C_XBP]} */
/** ==2: \PUSH {PC,A,B,DPH,DPL,PSW,TASK-REGBANK:R0-R7}, \SAVE {[USERREG(ASM)],[?C_XBP]} */
/** ==1: \PUSH {PC,A,B,DPH,DPL,PSW,TASK-REGBANK:R0-R7,[USERREG(ASM)]}, \SAVE {[?C_XBP]} */
#define MCUCFG_BASICSTACKSIZE     (15 + MCUCFG_USERREGSIZE + (MCUCFG_XBPSTACK == __ENABLED__ ? 2 : 0))
#define MCUCFG_STACK_ALIGN
#define MCUCFG_TASKSTACK_REALLOC  __ENABLED__
#define MCUCFG_STACKSIZE_TASKMGR  (MCUCFG_BASICSTACKSIZE + 16)
#define MCUCFG_STACKSIZE_DEBUGGER (MCUCFG_BASICSTACKSIZE + 16)

/*
 * MCUAPI
 */

/* TaskNode */
#define mTaskNode_Head_           m_stacksize_t stacklen;
#define mTaskNode_Tail_           mUserReg_C_

/* SysTick */
#define mSysTick_CLKMOD           (SYSCFG_SYSCLK / (1000000UL / SYSCFG_SYSTICKCYCLE) <= 65536 ? 1 : 12)
#define mSysTick_Cycle            (SYSCFG_SYSCLK / (1000000UL / SYSCFG_SYSTICKCYCLE) / mSysTick_CLKMOD)
#if mSysTick_Cycle > 65536
#error 系统滴答定时器溢出，必须减小系统时钟或系统滴答周期。
#elif 1000000UL % SYSCFG_SYSTICKCYCLE
#warning 每秒钟的系统滴答周期数不为整数，建议重新调整系统滴答周期。
#elif SYSCFG_SYSCLK % (1000000UL / SYSCFG_SYSTICKCYCLE)
#warning 每秒钟的系统滴答周期数不为整数，建议重新调整系统时钟或系统滴答周期。
#elif SYSCFG_SYSCLK / (1000000UL / SYSCFG_SYSTICKCYCLE) % mSysTick_CLKMOD
#warning 每秒钟的系统滴答周期数不为整数，建议重新调整系统时钟或系统滴答周期。
#endif
#define mSysTick_InitValue        (65536 - mSysTick_Cycle)
#define mSysTick_Counter          ((TH0 << 8) | TL0)
#define mSysTick_Disable          ET0 = 0
#define mSysTick_Enable           ET0 = 1
#define mSysTick_Clear

#define mxDisableIRQ              mx_disable_irq()
#define mxResumeIRQ               mx_resume_irq()

#define mSysIRQ_Disable \
do{ \
	mPendSV_Disable; \
	mSysTick_Disable; \
}while(false)

#define mSysIRQ_Enable \
do{ \
	mSysTick_Enable; \
	mPendSV_Enable; \
}while(false)

#define mSys_Idle \
do{ \
	PCON |= 0x01; \
	OS_NOPx4; \
}while(false)

#define mSys_INIT \
do{ \
	s_init_mempool((void _MALLOC_MEM_ *)MCUCFG_MALLOCMEMBPTR, MCUCFG_MALLOCMEMSIZE); \
	OS_NOPx1; \
	AUXR = mSysTick_CLKMOD == 1 ? AUXR | 0x80 : AUXR &~0x80; \
	TMOD &= 0xF0; \
	TL0 = (s_u8_t)(mSysTick_InitValue); \
	TH0 = (s_u8_t)(mSysTick_InitValue >> 8); \
	TR0 = 1; \
	mSysIRQ_Enable; \
	EA = 1; \
}while(false)

#define mSysTick_Counting \
do{ \
	if(mSysTick_Counter <= tick_temp) break; \
	s_tick_counter1 += mSysTick_Counter - tick_temp; \
	s_tick_counter2++; \
}while(false)

#define mUsedTime_END \
do{ \
	if(usedtime[0]){ \
		usedtime[0]--; \
		usedtime[1] = 65536 - usedtime[1] + counter - mSysTick_InitValue; \
	} \
	else{ \
		if(counter >= usedtime[1]){ \
			usedtime[1] = counter - usedtime[1]; \
		} \
		else{ \
			usedtime[1] = 65536 - usedtime[1] + counter - mSysTick_InitValue; \
		} \
	} \
	s_task_current->usedtime[0] += usedtime[0]; \
	s_task_current->usedtime[0] += (s_task_current->usedtime[1] + usedtime[1]) / mSysTick_Cycle; \
	s_task_current->usedtime[1]  = (s_task_current->usedtime[1] + usedtime[1]) % mSysTick_Cycle; \
}while(false)

#define mUsedTime_INIT \
do{ \
	usedtime[0] = 0; \
	usedtime[1] = counter; \
}while(false)

#if SYSCFG_PENDSVFIFO_MONITOR == __ENABLED__
#define mPendSV_FIFOLoad \
do{ \
	if(!mPendSV_FIFOLoader((s_u16_t)&u_psv)) \
		mPendSV_Set; \
	else s_fault.overflow_pendsvfifo = true; \
}while(false)
#else
#define mPendSV_FIFOLoad \
do{ \
	if(!mPendSV_FIFOLoader((s_u16_t)&u_psv)) \
		mPendSV_Set; \
}while(false)
#endif

#define mPendSV_FIFOHandle \
	if(!m_sign_fifo_0_0) mPendSV_FIFOHandler()

#define miWriteFlagBits \
	static bit u_f = false; \
	if(!u_f){ \
		u_f = true

#if MCUCFG_XBPSTACK == __ENABLED__
#define mXBP_INIT \
	*(s_u16_t *)(node_news->bsp + 15 + MCUCFG_USERREGSIZE) \
	= node_news->bsp + node_news->stacksize
#else
#define mXBP_INIT
#endif

#define mTaskStack_INIT \
do{ \
	node_news->stacklen = ( \
		(MCUCFG_TASK_REGBANK == MCUCFG_SYSINT_REGBANK ? 15 : 7) + \
		(MCUCFG_USERREGCONFIG == 1 ? MCUCFG_USERREGSIZE : 0) \
	); \
	*(s_u16_t *)node_news->bsp = ( \
		((s_u16_t)s_task_starter->entry << 8) | \
		((s_u16_t)s_task_starter->entry >> 8) \
	); \
	*(s_u8_t *)(node_news->bsp + 7 - 1) = MCUCFG_TASK_REGBANK * 8; \
	mUserReg_INIT; \
	mXBP_INIT; \
	mUserReg_CINIT; \
}while(false)



#endif
