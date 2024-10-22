/**************************************************************************//**
 * @item     CosyOS-II Hook
 * @file     gvar_hook.c
 * @brief    全局变量钩子（用于更新全局变量的副本）
 * @detail   当一个非原子访问的全局变量需要在中断中读取时，可在全局变量钩子中，
             用全局变量的正本来更新副本，可采用赋值、内存拷贝、字符串拷贝等方式。
 * @author   迟凯峰
 * @version  V3.0.0
 * @date     2024.06.25
 ******************************************************************************/

#include "..\System\os_link.h"
#if SYSCFG_GVARHOOK == __ENABLED__

void gvar_hook(void) MCUCFG_C51USING
{
	
}

#endif
