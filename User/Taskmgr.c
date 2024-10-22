/**************************************************************************//**
 * @item     CosyOS-II User
 * @file     Taskmgr.c
 * @brief    CosyOS-II 任务管理器
 * @detail   当前配置为使用UART2，用户可自行调整。
             正常情况下，下方代码中，您仅需调整：UART2_Isr、中断向量号、S2CON、S2TI、S2RI、S2BUF。
             您需自行初始化串口、串口中断优先级、波特率等相关配置。
 * @author   迟凯峰
 * @version  V1.0.2
 * @date     2024.05.11
 ******************************************************************************/

#include "..\System\os_link.h"
#if SYSCFG_DEBUGGING == __ENABLED__

void UART2_Isr(void) interrupt 8// using 3
{
	if(S2CON & S2TI){
		S2CON &= ~S2TI;
		if(*s_debug_sendptr){
			S2BUF = *s_debug_sendptr++;
		}
		else{
			iDebugSend;
		}
	}
	if(S2CON & S2RI){
		S2CON &= ~S2RI;
		if(s_debug_recvptr - s_debug_recvbuff < OS_DEBUGRECVBUFFSIZE - 1){
			*s_debug_recvptr++ = S2BUF;
		}
		iDebugRecv;
	}
}

#endif
