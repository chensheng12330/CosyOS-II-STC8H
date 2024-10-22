/**************************************************************************//**
 * @item     CosyOS-II Hook
 * @file     idle_hook.c
 * @brief    空闲钩子
 * @detail   在系统空闲任务中被调用，用户可添加自己的代码做一些没有实时性要求的工作。
 * @author   迟凯峰
 * @version  V3.0.0
 * @date     2024.06.25
 ******************************************************************************/
 
#include "..\System\os_link.h"
#if SYSCFG_IDLEHOOK == __ENABLED__

void idle_hook(void)
{

}

#endif
