/**************************************************************************//**
 * @item     CosyOS-II Kernel
 * @file     os_def.h
 * @brief    初始宏定义
 * @author   迟凯峰
 * @version  V3.0.0
 * @date     2024.06.25
 ******************************************************************************/

#ifndef __OS_DEF_H
#define __OS_DEF_H

#define OS_NULL 0
#define false   0
#define true    1

/*
 * CONFIG VALUE
 */

#define __DISABLED__     0
#define __ENABLED__      1
#define __STATIC__       0
#define __DYNAMIC__      2
#define __MSP__          0
#define __PSP__          1
#define	__STCLK__        0
#define	__FCLK__         1
#define __STANDARD__     0
#define __INTELLIGENT__  0
#define	__PERFORMANCE__  1
#define __MSGQUE_FIFO__  0
#define __MSGQUE_LIFO__  1
#define __MCS_51__       0
#define __MCS_251__      1
#define __ARM__          2



/*
 * 任务相关
 */

#define OS_TASKQUEUELEN_MAX      255 // 任务队列最大长度

/* 任务状态 */
#define	OS_STATUS_READY         0x00 // 就绪
#define	OS_STATUS_FLOATING      0x01 // 浮动
#define	OS_STATUS_BLOCKED       0x02 // 阻塞
#define OS_STATUS_OVERTIME      0x04 // 超时
#define	OS_STATUS_SUSPENDED     0x08 // 挂起
#define	OS_STATUS_STOPPED       0x10 // 停止
#define	OS_STATUS_STOPPED_TSOF  0x30 // 任务栈溢出停止
#define OS_STATUS_STOPPED_TSRF  0x50 // 任务栈重分配失败停止
#define	OS_STATUS_DELETED       0x80 // 已删除/未启动

/* 阻塞事件类型 */
#define	OS_BLOCKED_DELAY        0x00 // 延时阻塞
#define	OS_BLOCKED_BINARY       0x10 // 二值信号量阻塞（获取/等待）
#define	OS_BLOCKED_MUTEX        0x20 // 互斥信号量阻塞（获取）
#define	OS_BLOCKED_SEMAPHORE    0x30 // 计数信号量阻塞（获取）
#define OS_BLOCKED_FLAGGROUP    0x40 // 等待标志组阻塞
#define	OS_BLOCKED_TASKMSG      0x50 // 接收私信阻塞
#define OS_BLOCKED_FETION       0x60 // 接收飞信阻塞
#define	OS_BLOCKED_MAIL         0x70 // 接收邮件阻塞
#define	OS_BLOCKED_MSG          0x80 // 接收消息阻塞



/*
 * DEBUG SEND TYPE
 */

#define OS_DEBUG_SEND_CMDLINE   0x01
#define OS_DEBUG_SEND_TASKMGR   0x02



/*
 * TASKMGR LEN
 */

#define	OS_TASKMGR_LEN_PRI   6
#define OS_TASKMGR_LEN_STA   6
#define	OS_TASKMGR_LEN_CPU   9



/*
 * SVID（服务ID，中断挂起服务系统专用ID）
 */

#define OS_SVID_RESUMETASK   0x00
#define OS_SVID_SUSPENDTASK  0x01
#define OS_SVID_DELETETASK   0x02
#define OS_SVID_SETTASKPRI   0x03
#define OS_SVID_SETBLOCK     0x04
#define OS_SVID_CELARBLOCK   0x05
#define OS_SVID_TIMINT       0x06
#define OS_SVID_TIMQRY       0x07
#define OS_SVID_BINARY       0x08
#define OS_SVID_GIVESEM      0x09
#define OS_SVID_SENDFETION   0x0A
#define OS_SVID_SENDMAIL     0x0B
#define OS_SVID_SENDMSG      0x0C
#define OS_SVID_GROUP        0x0D
#define OS_SVID_GVARWRITE    0x0E
#define OS_SVID_PENDSVC      0x0F
#define OS_SVID_SENDSIGN     0x10
#define OS_SVID_END          0x11



/*
 * ECODE（错误码）
 */

#define OS_ECODE_NOERROR               0 // 无错误
#define OS_ECODE_OVERFLOW_MSGQUEUE     1 // 消息队列溢出
#define OS_ECODE_OVERFLOW_TASKQUEUE    2 // 任务队列溢出
#define OS_ECODE_OVERFLOW_TASKSTACK    3 // 任务栈溢出
#define OS_ECODE_MALLOCFAIL_MSGNODE    4 // 消息节点内存分配失败
#define OS_ECODE_MALLOCFAIL_TASKNODE   5 // 任务节点内存分配失败
#define OS_ECODE_MALLOCFAIL_TASKSTACK  6 // 任务栈内存分配失败
#define OS_ECODE_TASKSUSPENDED         7 // 任务已挂起
#define OS_ECODE_TASKSTOPPED           8 // 任务已停止
#define OS_ECODE_TASKNOTSTARTED        9 // 任务未启动/已删除
#define OS_ECODE_TASKNOTREADY         10 // 任务未就绪
#define OS_ECODE_TASKNOTBLOCKED       11 // 任务未阻塞
#define OS_ECODE_TASKNOTSUSPENDED     12 // 任务未挂起
#define OS_ECODE_TASKPRIUNCHANGED     13 // 任务优先级未改变
#define OS_ECODE_DONOTKNOW           255 // 未知错误



#endif
