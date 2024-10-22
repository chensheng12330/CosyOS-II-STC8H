/**************************************************************************//**
 * @item     CosyOS-II Kernel
 * @file     os_main.c
 * @brief    主函数
 * @author   迟凯峰
 * @version  V3.0.0
 * @date     2024.06.25
 ******************************************************************************/

#include "os_link.h"

int main(void)
{
	void s_init_timing(void);
	#if SYSCFG_SAFERUNTIME == __ENABLED__
	s_tid_t i;
	for(i = 0; i < OS_TASKTOTAL + 1; i++){
		s_sign_timeout[i] = 0;
	}
	#endif
	#if OS_TIMINTTOTAL || OS_TIMQRYTOTAL
	s_init_timing();
	#endif
	init_hook();
	mSys_INIT;
	#if SYSCFG_DEBUGGING == __ENABLED__
	sStartTask_TimQry(OS_TMID_TASKMGR);
	#endif
	uStartTask(Starter, OS_STATUS_READY);
	uStartTask(Sysidle, OS_STATUS_READY);
	while(true);
}
