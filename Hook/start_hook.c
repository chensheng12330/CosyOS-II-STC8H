/**************************************************************************//**
 * @item     CosyOS-II Hook
 * @file     start_hook.c
 * @brief    启动钩子
 * @detail   在启动任务中被调用，可在此启动用户任务。
 * @service  uStartTask(task, status); 启动任务
             uStartTask_Ready(task);   启动任务并就绪
             uStartTask_Suspend(task); 启动任务并挂起
 * @author   迟凯峰
 * @version  V3.0.0
 * @date     2024.06.25
 ******************************************************************************/

#include "..\System\os_link.h"
#if 0
void start_hook(void)
{

}
#endif
