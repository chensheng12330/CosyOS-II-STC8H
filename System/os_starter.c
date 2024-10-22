/**************************************************************************//**
 * @item     CosyOS-II Kernel
 * @file     os_starter.c
 * @brief    启动任务（Starter）
 * @author   迟凯峰
 * @version  V3.0.0
 * @date     2024.06.25
 ******************************************************************************/

#include "os_link.h"

dCreateTask(Starter, SYSCFG_TASKPRIORITY - 2, MCUCFG_STACKSIZE_STARTER, 0, 0)
{
	#if OS_TIMINTTOTAL || OS_TIMQRYTOTAL
	s_u8_t i;
	#if OS_TIMINTTOTAL
	i = OS_TIMINTTOTAL;
	while(i--){
		if(s_timint_hookortask[i]){
			sStartTask_TimInt(i);
		}
	}
	#endif
	#if OS_TIMQRYTOTAL
	i = OS_TIMQRYTOTAL - 1;
	while(i--){
		if(s_timqry_hookortask[i]){
			sStartTask_TimQry(i);
		}
	}
	#endif
	#endif
	start_hook();
	uDeleteTasking;
	uEndTasking;
}
