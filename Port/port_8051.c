/**************************************************************************//**
 * @item     CosyOS-II Port
 * @file     port_8051.c
 * @brief    8051 Core Port File
 * @author   迟凯峰
 * @version  V3.0.0
 * @date     2024.06.25
 ******************************************************************************/

#include "..\System\os_var.h"
#ifdef __PORT_8051_H

s_u8_t _SYS_MEM_ m_bsp_add = sizeof(s_taskhand_ts);



/*
 * 全局临界区
 */

static bit m_oldirq;
static volatile s_u8_t _SYS_MEM_ m_glocri_counter = 0;

void mx_disable_irq(void)
{
	if(_testbit_(EA)){
		m_oldirq = 1;
	}
	else if(!m_glocri_counter){
		m_oldirq = 0;
	}
	m_glocri_counter++;
}

void mx_resume_irq(void)
{
	m_glocri_counter--;
	if(!m_glocri_counter){
		EA = m_oldirq;
	}
}



/*
 * 中断挂起服务FIFO队列
 */

#if MCUCFG_PENDSVFIFO_DEPTH > 0

#if SYSCFG_PENDSVFIFO_MONITOR == __ENABLED__
s_u8_t mPendSV_FIFO_DepthMAX = 0;
#endif
static s_u16_t mPendSV_FIFO_0[MCUCFG_PENDSVFIFO_DEPTH] _at_ 0x0000;
static s_u16_t mPendSV_FIFO_1[MCUCFG_PENDSVFIFO_DEPTH] _at_ 0x0100;
bit m_sign_fifo = true;

#define mWriteCode(n) \
bit m_sign_fifo_0_##n = true; \
bit m_sign_fifo_1_##n = true

mWriteCode(0);
mWriteCode(1);
mWriteCode(2);
mWriteCode(3);
mWriteCode(4);
mWriteCode(5);
mWriteCode(6);
mWriteCode(7);
#if MCUCFG_PENDSVFIFO_DEPTH > 8
mWriteCode(8);
mWriteCode(9);
mWriteCode(10);
mWriteCode(11);
mWriteCode(12);
mWriteCode(13);
mWriteCode(14);
mWriteCode(15);
#if MCUCFG_PENDSVFIFO_DEPTH > 16
mWriteCode(16);
mWriteCode(17);
mWriteCode(18);
mWriteCode(19);
mWriteCode(20);
mWriteCode(21);
mWriteCode(22);
mWriteCode(23);
#if MCUCFG_PENDSVFIFO_DEPTH > 24
mWriteCode(24);
mWriteCode(25);
mWriteCode(26);
mWriteCode(27);
mWriteCode(28);
mWriteCode(29);
mWriteCode(30);
mWriteCode(31);
#if MCUCFG_PENDSVFIFO_DEPTH > 32
mWriteCode(32);
mWriteCode(33);
mWriteCode(34);
mWriteCode(35);
mWriteCode(36);
mWriteCode(37);
mWriteCode(38);
mWriteCode(39);
#endif
#endif
#endif
#endif

#undef mWriteCode

static void _fifo_0_(s_u8_t i) MCUCFG_C51USING
{
	void _STATIC_MEM_ *sv = (void _STATIC_MEM_ *)(mPendSV_FIFO_0[i]);
	(*sPendSV_FIFOHandler[*(const s_u8_t _STATIC_MEM_ *)sv])(sv);
}

static void _fifo_1_(s_u8_t i) MCUCFG_C51USING
{
	void _STATIC_MEM_ *sv = (void _STATIC_MEM_ *)(mPendSV_FIFO_1[i]);
	(*sPendSV_FIFOHandler[*(const s_u8_t _STATIC_MEM_ *)sv])(sv);
}

void mPendSV_FIFOHandler(void) MCUCFG_C51USING
{
	#if SYSCFG_PENDSVFIFO_MONITOR == __ENABLED__
	s_u8_t i;
	#endif
__LABLE:
	m_sign_fifo = false;
	
	m_sign_fifo_0_0 = true;
	      _fifo_0_(0);
	
	#if SYSCFG_PENDSVFIFO_MONITOR == __ENABLED__
	#define mWriteCode(n) \
	if(m_sign_fifo_0_##n){ i = n; goto __LABLE_0; } \
	   m_sign_fifo_0_##n = true; \
	         _fifo_0_ (n)
	#else
	#define mWriteCode(n) \
	if(m_sign_fifo_0_##n)  goto __LABLE_0; \
	   m_sign_fifo_0_##n = true; \
	         _fifo_0_ (n)
	#endif
	
	mWriteCode(1);
	mWriteCode(2);
	mWriteCode(3);
	mWriteCode(4);
	mWriteCode(5);
	mWriteCode(6);
	mWriteCode(7);
	#if MCUCFG_PENDSVFIFO_DEPTH > 8
	mWriteCode(8);
	mWriteCode(9);
	mWriteCode(10);
	mWriteCode(11);
	mWriteCode(12);
	mWriteCode(13);
	mWriteCode(14);
	mWriteCode(15);
	#if MCUCFG_PENDSVFIFO_DEPTH > 16
	mWriteCode(16);
	mWriteCode(17);
	mWriteCode(18);
	mWriteCode(19);
	mWriteCode(20);
	mWriteCode(21);
	mWriteCode(22);
	mWriteCode(23);
	#if MCUCFG_PENDSVFIFO_DEPTH > 24
	mWriteCode(24);
	mWriteCode(25);
	mWriteCode(26);
	mWriteCode(27);
	mWriteCode(28);
	mWriteCode(29);
	mWriteCode(30);
	mWriteCode(31);
	#if MCUCFG_PENDSVFIFO_DEPTH > 32
	mWriteCode(32);
	mWriteCode(33);
	mWriteCode(34);
	mWriteCode(35);
	mWriteCode(36);
	mWriteCode(37);
	mWriteCode(38);
	mWriteCode(39);
	#endif
	#endif
	#endif
	#endif
	
	#undef mWriteCode
	
	#if SYSCFG_PENDSVFIFO_MONITOR == __ENABLED__
	mPendSV_FIFO_DepthMAX = MCUCFG_PENDSVFIFO_DEPTH;
	#endif
__LABLE_0:
	m_sign_fifo = true;
	#if SYSCFG_PENDSVFIFO_MONITOR == __ENABLED__
	if(i > mPendSV_FIFO_DepthMAX) mPendSV_FIFO_DepthMAX = i;
	#endif
	
	if(m_sign_fifo_1_0) return;
	
	m_sign_fifo_1_0 = true;
	      _fifo_1_(0);
	
	#if SYSCFG_PENDSVFIFO_MONITOR == __ENABLED__
	#define mWriteCode(n) \
	if(m_sign_fifo_1_##n){ i = n; goto __LABLE_1; } \
	   m_sign_fifo_1_##n = true; \
	         _fifo_1_ (n)
	#else
	#define mWriteCode(n) \
	if(m_sign_fifo_1_##n)  goto __LABLE_1; \
	   m_sign_fifo_1_##n = true; \
	         _fifo_1_ (n)
	#endif
	
	mWriteCode(1);
	mWriteCode(2);
	mWriteCode(3);
	mWriteCode(4);
	mWriteCode(5);
	mWriteCode(6);
	mWriteCode(7);
	#if MCUCFG_PENDSVFIFO_DEPTH > 8
	mWriteCode(8);
	mWriteCode(9);
	mWriteCode(10);
	mWriteCode(11);
	mWriteCode(12);
	mWriteCode(13);
	mWriteCode(14);
	mWriteCode(15);
	#if MCUCFG_PENDSVFIFO_DEPTH > 16
	mWriteCode(16);
	mWriteCode(17);
	mWriteCode(18);
	mWriteCode(19);
	mWriteCode(20);
	mWriteCode(21);
	mWriteCode(22);
	mWriteCode(23);
	#if MCUCFG_PENDSVFIFO_DEPTH > 24
	mWriteCode(24);
	mWriteCode(25);
	mWriteCode(26);
	mWriteCode(27);
	mWriteCode(28);
	mWriteCode(29);
	mWriteCode(30);
	mWriteCode(31);
	#if MCUCFG_PENDSVFIFO_DEPTH > 32
	mWriteCode(32);
	mWriteCode(33);
	mWriteCode(34);
	mWriteCode(35);
	mWriteCode(36);
	mWriteCode(37);
	mWriteCode(38);
	mWriteCode(39);
	#endif
	#endif
	#endif
	#endif
	
	#undef mWriteCode
	
	#if SYSCFG_PENDSVFIFO_MONITOR == __ENABLED__
	mPendSV_FIFO_DepthMAX = MCUCFG_PENDSVFIFO_DEPTH;
	#endif
__LABLE_1:
	#if SYSCFG_PENDSVFIFO_MONITOR == __ENABLED__
	if(i > mPendSV_FIFO_DepthMAX) mPendSV_FIFO_DepthMAX = i;
	#endif
	
	if(!m_sign_fifo_0_0) goto __LABLE;
}

#endif



#endif
