/**************************************************************************//**
 * @item     CosyOS-II Hook
 * @file     init_hook.c
 * @brief    初始化钩子
 * @detail   在主函数中首先被调用，适用于初始化时钟、GPIO、寄存器等工作。
 * @author   迟凯峰
 * @version  V3.0.0
 * @date     2024.06.25
 ******************************************************************************/

#include "..\System\os_link.h"

void init_hook(void)
{
	#define BAUD  115200 // 串口波特率
	#define TM2   (65536 - SYSCFG_SYSCLK / 4 / BAUD)
	P0M0 = 0; P0M1 = 0;
	P1M0 = 0; P1M1 = 0;
	P2M0 = 0; P2M1 = 0;
	P3M0 = 0; P3M1 = 0;
	P4M0 = 0; P4M1 = 0;
	P5M0 = 0; P5M1 = 0;
	P6M0 = 0; P6M1 = 0;
	P7M0 = 0; P7M1 = 0;
	
	AUXR  =	0x00;  // 允许访问内部xdata
	P_SW2 =	EAXFR; // 允许访问扩展RAM区特殊功能寄存器
	
	TMOD = 0x00;   // 定时器0、1、2、3、4为16位自动重装载模式
	AUXR = 0x45;   // 定时器1、2为1T模式，  串口1用定时器2做波特率发生器
	SCON = 0x40;   // 串口1工作模式为模式1
	S2CON = 0x40;  // 串口2工作模式为模式0，串口2用定时器2做波特率发生器
	P_SW1 = 0xC0;  // 串口1切换至P4.3/P4.4
	TI = 1;        // 用于printf
	/* 定时器2 */
	T2L = (unsigned char)TM2;
	T2H = (unsigned char)(TM2 >> 8);
	AUXR|= T2R;
	/* UART2中断优先级为1级 */
	IP2H &=~PS2H;
	IP2 |= PS2;
	/* INT0中断优先级为0级 */
	IPH &=~PX0H;
	PX0 = 0;
	/* 任务管理器 */
	#if SYSCFG_DEBUGGING == 1
	S2CON |= S2REN;
	IE2 |= ES2;
	#endif
}
