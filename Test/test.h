/**************************************************************************//**
 * @item     CosyOS-II 测试
 * @file     test.h
 * @brief    测试项目定义
 * @author   迟凯峰
 * @version  V1.1.1
 * @date     2024.07.07
 ******************************************************************************/

#ifndef __TEST_H
#define __TEST_H

///////////////////////////////////////////////////////////////////////////////

          //*** <<< Use Configuration Wizard in Context Menu >>> ***//

///////////////////////////////////////////////////////////////////////////////
// <o> 用户定义测试项目
// <0x01=> 行为同步 - 恢复挂起测试
// <0x02=> 行为同步 - 清除阻塞测试
// <0x03=> 行为同步 - 二值信号量测试
// <0x04=> 行为同步 - 计数信号量测试
// <0x05=> 行为同步 - 事件标志组测试
// <0x11=> 行为同步 - 飞信测试
// <0x12=> 行为同步 - 私信测试
// <0x13=> 行为同步 - 邮箱测试
// <0x14=> 行为同步 - 队列测试
// <0xF0=> 零中断延迟测试
// <0xF1=> 动态内存测试
// <0xF2=> C51-XBPSTACK测试
// <i> 用户定义测试项目
#define TEST_OBJECT          1

///////////////////////////////////////////////////////////////////////////////

                //*** <<< end of configuration section >>> ***//

///////////////////////////////////////////////////////////////////////////////



/////////////////////////////// 系统定义测试项目 //////////////////////////////

/* 一、行为同步测试 */
// 1、事件同步
#define TEST_XWTB_RESSUS     0x01 // 恢复挂起测试
#define TEST_XWTB_CLRBLK     0x02 // 清除阻塞测试
#define TEST_XWTB_BIN        0x03 // 二值信号量测试
#define TEST_XWTB_SEM        0x04 // 计数信号量测试
#define TEST_XWTB_GROUP      0x05 // 事件标志组测试
// 2、消息同步
#define TEST_XWTB_FETION     0x11 // 飞信测试
#define TEST_XWTB_TASKMSG    0x12 // 私信测试
#define TEST_XWTB_MAILBOX    0x13 // 邮箱测试
#define TEST_XWTB_QUEUE      0x14 // 队列测试

/* 二、零中断延迟测试 */
#define TEST_INT0DELAY       0xF0 // 零中断延迟测试

/* 三、动态内存测试 */
#define TEST_MALLOC          0xF1 // 动态内存测试

/* 四、C51测试 */
#define TEST_C51XBP          0xF2 // C51-XBPSTACK测试

///////////////////////////////////////////////////////////////////////////////

#include "..\System\os_link.h"
#include "..\User\typedef.h"
#include <stdio.h>
#if MCUCFG_ISA == __ARM__
#include "main.h"
#elif MCUCFG_ISA == __MCS_251__
#include "STC32G_Timer.h"
#include "STC32G_NVIC.h"
#endif

#if MCUCFG_ISA == __MCS_51__
#define TEST_TASKSTACKSIZE   64
#elif MCUCFG_ISA == __MCS_251__
#define TEST_TASKSTACKSIZE   192
#elif MCUCFG_ISA == __ARM__
#define TEST_TASKSTACKSIZE   1024
#endif

#if TEST_OBJECT == TEST_XWTB_TASKMSG
#if MCUCFG_ISA != __ARM__ && SYSCFG_TASKMSGMODE == __PERFORMANCE__
#pragma NOREGPARMS
#endif
uExternTask_Msg(task_1) const char *p, uint a, float b);
#if MCUCFG_ISA != __ARM__ && SYSCFG_TASKMSGMODE == __PERFORMANCE__
#pragma REGPARMS
#endif
#else
uExternTask(task_1);
#endif
uExternTask(task_2);
uExternTask(task_3);
uExternBin(bin_2);
void led_task_2(void);
void led_task_1(void);

#if MCUCFG_ISA != __ARM__
#define TEST_ISR_TIMA          void TIME_3(void) interrupt 19 MCUCFG_C51USING
#define TEST_ISR_TIMB          void TM1_Isr(void) interrupt 3
#define TEST_ISR_TIMA_CLR      AUXINTIF &= ~T3IF
#define TEST_ISR_TIMB_CLR      do{}while(0)
#define TEST_GPIO_P00_TOGGLE   P00 = ~P00
#define TEST_GPIO_P01_SET      P01 = 1
#define TEST_GPIO_P01_RESET    P01 = 0
#define TEST_GPIO_P02_SET      P02 = 1
#define TEST_GPIO_P02_RESET    P02 = 0
#define TEST_GPIO_P03_SET      P03 = 1
#define TEST_GPIO_P03_RESET    P03 = 0
#else
#define TEST_ISR_TIMA          void TIM3_IRQHandler(void)
#define TEST_ISR_TIMB          void TIM2_IRQHandler(void)
#define TEST_ISR_TIMA_CLR      TIM3->SR = 0
#define TEST_ISR_TIMB_CLR      TIM2->SR = 0
#define TEST_GPIO_P00_TOGGLE   HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_0)
#define TEST_GPIO_P01_SET      HAL_GPIO_WritePin(GPIOD, GPIO_PIN_1, GPIO_PIN_SET)
#define TEST_GPIO_P01_RESET    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_1, GPIO_PIN_RESET)
#define TEST_GPIO_P02_SET      HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET)
#define TEST_GPIO_P02_RESET    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET)
#define TEST_GPIO_P03_SET      HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, GPIO_PIN_SET)
#define TEST_GPIO_P03_RESET    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, GPIO_PIN_RESET)
#endif

#if MCUCFG_ISA == __MCS_51__
#define TEST_TIMA_INIT \
do{ \
	T4T3M &= 0xF0; \
	T3L = (unsigned char)((65536 - SYSCFG_SYSCLK / 12 / 40)); \
	T3H = (unsigned char)((65536 - SYSCFG_SYSCLK / 12 / 40) >> 8); \
	TM3PS = 99; \
	T4T3M |= 0x08; \
	IE2 |= ET3; \
}while(0)
#define TEST_TIMB_INIT \
do{ \
	TL1 = (unsigned char)((65536 - SYSCFG_SYSCLK / TEST_TIMB_FREQ)); \
	TH1 = (unsigned char)((65536 - SYSCFG_SYSCLK / TEST_TIMB_FREQ) >> 8); \
	IPH |= PT1H; \
	PT1 = 1; \
	TR1 = 1; \
	ET1 = 1; \
}while(0)
#elif MCUCFG_ISA == __MCS_251__
#define TEST_TIMA_INIT \
do{ \
	TIM_InitTypeDef TIM_InitStructure; \
	TIM_InitStructure.TIM_ClkSource = TIM_CLOCK_12T; \
	TIM_InitStructure.TIM_ClkOut    = DISABLE; \
	TIM_InitStructure.TIM_Value     = (u16)(65536UL - (SYSCFG_SYSCLK / (40 * 12))); \
	TIM_InitStructure.TIM_Run       = ENABLE; \
	TM3PS = 99; \
	Timer_Inilize(Timer3, &TIM_InitStructure); \
	NVIC_Timer3_Init(ENABLE, Priority_0); \
}while(0)
#define TEST_TIMB_INIT \
do{ \
	TIM_InitTypeDef TIM_InitStructure; \
	TIM_InitStructure.TIM_Mode      = TIM_16BitAutoReload; \
	TIM_InitStructure.TIM_ClkSource = TIM_CLOCK_1T; \
	TIM_InitStructure.TIM_ClkOut    = DISABLE; \
	TIM_InitStructure.TIM_Value     = (u16)(65536UL - SYSCFG_SYSCLK / TEST_TIMB_FREQ); \
	TIM_InitStructure.TIM_Run       = ENABLE; \
	Timer_Inilize(Timer1, &TIM_InitStructure); \
	NVIC_Timer1_Init(ENABLE, Priority_3); \
}while(0)
#elif MCUCFG_ISA == __ARM__
#define TEST_TIMA_INIT \
do{ \
	extern TIM_HandleTypeDef htim3; \
	TIM_ClockConfigTypeDef sClockSourceConfig = {0}; \
	TIM_MasterConfigTypeDef sMasterConfig = {0}; \
	htim3.Instance = TIM3; \
	htim3.Init.Prescaler = 8400-1; \
	htim3.Init.CounterMode = TIM_COUNTERMODE_UP; \
	htim3.Init.Period = 25000-1; \
	htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1; \
	htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE; \
	if (HAL_TIM_Base_Init(&htim3) != HAL_OK) \
	{ \
		Error_Handler(); \
	} \
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL; \
	if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK) \
	{ \
		Error_Handler(); \
	} \
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET; \
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE; \
	if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK) \
	{ \
		Error_Handler(); \
	} \
	HAL_TIM_Base_Start_IT(&htim3); \
}while(0)
#define TEST_TIMB_INIT \
do{ \
	extern TIM_HandleTypeDef htim2; \
	TIM_ClockConfigTypeDef sClockSourceConfig = {0}; \
	TIM_MasterConfigTypeDef sMasterConfig = {0}; \
	htim2.Instance = TIM2; \
	htim2.Init.Prescaler = 4000000UL / TEST_TIMB_FREQ - 1; \
	htim2.Init.CounterMode = TIM_COUNTERMODE_UP; \
	htim2.Init.Period = 21-1; \
	htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1; \
	htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE; \
	if (HAL_TIM_Base_Init(&htim2) != HAL_OK) \
	{ \
		Error_Handler(); \
	} \
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL; \
	if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK) \
	{ \
		Error_Handler(); \
	} \
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET; \
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE; \
	if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK) \
	{ \
		Error_Handler(); \
	} \
	HAL_TIM_Base_Start_IT(&htim2); \
}while(0)
#endif

///////////////////////////////////////////////////////////////////////////////

#endif
