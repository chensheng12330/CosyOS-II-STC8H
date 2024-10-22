/**************************************************************************//**
 * @item     CosyOS-II Kernel
 * @file     os_sysidle.c
 * @brief    系统空闲任务（Sysidle）
 * @author   迟凯峰
 * @version  V3.0.0
 * @date     2024.06.25
 ******************************************************************************/

#include "os_link.h"

uCreateTask(Sysidle, 0, MCUCFG_STACKSIZE_SYSIDLE, 0, 0)
{
	#if SYSCFG_SAFERUNTIME == __ENABLED__
	if(true){
		s_tid_t i;
		for(i = 1; i < OS_TASKTOTAL + 1; i++){
			s_sign_timeout[i] = 0;
		}
	}
	#endif
	#if SYSCFG_SOFTRTC == __ENABLED__
	if(true){
		static s_u8_t year = 0xFF;
		if(year != s_rtc[0][0]){
			year = s_rtc[0][0];
			s_month2day = year ? ((year & 3) ? 28 : 29) : ((sDefCentury & 3) ? 28 : 29);
		}
	}
	#endif
	#if SYSCFG_IDLEHOOK == __ENABLED__
	idle_hook();
	#endif
	#if SYSCFG_LOWPOWERMODE == __ENABLED__
	mSys_Idle;
	#endif
	OS_NOPx2;
	uEndTasking;
}
