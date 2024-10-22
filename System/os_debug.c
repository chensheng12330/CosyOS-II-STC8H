/**************************************************************************//**
 * @item     CosyOS-II Kernel
 * @file     os_debug.c
 * @brief    DEBUG接口（串口发送和接收解析）
 * @author   迟凯峰
 * @version  V3.0.0
 * @date     2024.06.25
 ******************************************************************************/

#include "os_link.h"
#if SYSCFG_DEBUGGING == __ENABLED__

#if MCUCFG_ISA != __ARM__
static int _atoi_(const char* str)
{
	if(str == OS_NULL || !*str){
		return 0;
	}
	else{
		int a = 0;
		int k = 1;
		s_u8_t i = s_strlen(str);
		while(i--){
			if(str[i] >= '0' && str[i] <= '9'){
				a += (str[i] - '0') * k;
				k *= 10;
			}
			else if(str[i] == '+'){
				return a;
			}
			else if(str[i] == '-'){
				return -a;
			}
			else{
				return 0;
			}
		}
		return a;
	}
}
#else
#define _atoi_ atoi
#endif

static void _debug_recv_(void)
{
	#define	hptr s_debug_recvbuff
	s_u16_t temp;
	char strtmp[8];
	char _XDATA_MEM_ *mptr = hptr;
	char _XDATA_MEM_ *tptr = hptr + s_strlen(hptr) - 2;
	if(!s_strcmp(tptr, "\r\n")){
		if(!s_strcmp(hptr, "taskmgr\r\n")){
			s_sign_taskmgr = true;
		}
		else if(!s_strcmp(hptr, "exit\r\n")){
			s_sign_taskmgr = false;
		}
		else if(!s_memcmp(hptr, "taskmgr /s=", 11)){
			mptr += 11;
			s_memcpy(strtmp, mptr, tptr - mptr);
			strtmp[tptr - mptr] = '\0';
			temp = _atoi_(strtmp);
			if(temp >= 50 && temp <= 5000){
				mSysIRQ_Disable;
				s_timqry_reload[OS_TMID_TASKMGR] = (1000UL * temp) / SYSCFG_SYSTICKCYCLE;
				mSysIRQ_Enable;
			}
		}
	}
	#undef  hptr
}

uCreateTask_TimInt(OS_TMID_DEBUGGER, false, Debugger, SYSCFG_TASKPRIORITY - 1, MCUCFG_STACKSIZE_DEBUGGER, 0, 0)
{
	s_debug_recvptr[0] = '\0';
	_debug_recv_();
	s_debug_recvptr = s_debug_recvbuff;
	uSuspendTasking;
	uEndTasking;
}

uCreateHook_TimQry(OS_TMID_DEBUGHOOK, s_debug_sendtype, false, debug_hook)
{
	if(s_debug_sendtype & OS_DEBUG_SEND_TASKMGR){
		s_debug_sendtype &= ~OS_DEBUG_SEND_TASKMGR;
		s_debug_sendptr = s_taskmgr_sendbuff;
	}
	else if(s_debug_sendtype & OS_DEBUG_SEND_CMDLINE){
		s_debug_sendtype &= ~OS_DEBUG_SEND_CMDLINE;
		s_debug_sendptr = s_cmdline_sendbuff;
	}
	SYSCFG_DEBUGSEND;
}

#endif
