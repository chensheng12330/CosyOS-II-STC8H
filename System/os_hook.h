/**************************************************************************//**
 * @item     CosyOS-II Kernel
 * @file     os_hook.h
 * @brief    系统钩子函数声明
 * @author   迟凯峰
 * @version  V3.0.0
 * @date     2024.06.25
 ******************************************************************************/

#ifndef __OS_HOOK_H
#define __OS_HOOK_H

/* 初始化钩子 */
void init_hook(void);

/* 启动钩子 */
void start_hook(void);

/* 空闲钩子 */
void idle_hook(void);

/* 滴答钩子 */
void tick_hook(void);

/* 挂起服务钩子 */
void pendsv_hook(void);

/* 全局变量钩子 */
void gvar_hook(void);



#endif
