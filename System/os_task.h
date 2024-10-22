/**************************************************************************//**
 * @item     CosyOS-II Kernel
 * @file     os_task.h
 * @brief    系统任务声明
 * @author   迟凯峰
 * @version  V3.0.0
 * @date     2024.06.25
 ******************************************************************************/

#ifndef __OS_TASK_H
#define __OS_TASK_H

/* 任务管理器 */
uExternTask(Taskmgr);

/* 调试任务 */
uExternTask(Debugger);

/* 启动任务 */
dExternTask(Starter);

/* 系统空闲任务 */
uExternTask(Sysidle);



#endif
