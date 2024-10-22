/**************************************************************************//**
 * @item     CosyOS-II Hook
 * @file     pendsv_hook.c
 * @brief    挂起服务钩子
 * @detail   用于处理标志队列中的中断挂起服务。
 * @author   迟凯峰
 * @version  V3.0.0
 * @date     2024.06.25
 ******************************************************************************/

#include "..\System\os_link.h"
#if SYSCFG_PENDSVHOOK == __ENABLED__
#include "..\Test\test.h"
extern m_bit_t u_resume_task_2;
extern m_bit_t u_clrblk_task_2;
extern m_bit_t u_give_bin_2;
extern m_bit_t u_resume_task_3;
void pendsv_hook(void) MCUCFG_C51USING
{
	#if TEST_OBJECT == TEST_XWTB_RESSUS
	if(u_resume_task_2){
		u_resume_task_2 = false;
		pResumeTask(task_2);
	}
	#elif TEST_OBJECT == TEST_XWTB_CLRBLK
	if(u_clrblk_task_2){
		u_clrblk_task_2 = false;
		pClearBlock(task_2);
	}
	#elif TEST_OBJECT == TEST_XWTB_BIN
	if(u_give_bin_2){
		u_give_bin_2 = false;
		pGiveBin(bin_2);
	}
	#elif TEST_OBJECT == TEST_INT0DELAY
	if(u_resume_task_3){
		u_resume_task_3 = false;
		pResumeTask(task_3);
	}
	#endif
}

#endif
