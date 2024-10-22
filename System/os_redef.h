/**************************************************************************//**
 * @item     CosyOS-II Kernel
 * @file     os_redef.h
 * @brief    系统重定义（包括宏定义和类型定义）
 * @detail   基于os_def、os_base、syscfg、mcucfg再次进行定义及复合数据类型定义。
 * @author   迟凯峰
 * @version  V3.0.0
 * @date     2024.06.25
 ******************************************************************************/

#ifndef __OS_REDEF_H
#define __OS_REDEF_H

/*------------------------------------------------------------------------------
 * SYSAPI */

#define	sDefStr(s)              sDefStr_0(s)
#define	sDefStr_0(s)            #s

#define	sCat2Str(s1, s2)        sCat2Str_0(s1, s2)
#define	sCat2Str_0(s1, s2)      s1##s2

#define	sCat3Str(s1, s2, s3)    sCat3Str_0(s1, s2, s3)
#define	sCat3Str_0(s1, s2, s3)  s1##s2##s3

#define sDefBitsType(b)         sDefBitsType_0(b)
#define sDefBitsType_0(b)       s_u##b##_t

#define	sDefBitField(v)         sDefBitField_0(v)
#define	sDefBitField_0(v)       volatile s_u8_t v:1

#define sDefVoidBits(n)         sDefVoidBits_0(n)
#define	sDefVoidBits_0(n)       volatile s_u8_t  :n

#define sInitRealTime                         sInitRealTime_0(SYSCFG_MANUFACTUREDATE)
#define sInitRealTime_0(d)                    sInitRealTime_1(d)
#define sInitRealTime_1(y, m, d, H, M, s, w)  {y % 100, m, d, H, M, s, w}

#define sDefCentury                           sDefCentury_0(SYSCFG_MANUFACTUREDATE)
#define sDefCentury_0(d)                      sDefCentury_1(d)
#define sDefCentury_1(y, m, d, H, M, s, w)    (y % 100 ? y / 100 + 1 : y / 100)

/*------------------------------------------------------------------------------
 * INCLUDE */

#include <stdlib.h>
#include "..\Config\syscfg.h"
#include SYSCFG_STANDARDHEAD
#include sDefStr(sCat2Str(..\Port\port_, SYSCFG_MCUCORE.h))

/*------------------------------------------------------------------------------
 * NOPxX */

#define OS_NOPx1  MCUCFG_NOP
#define OS_NOPx2  OS_NOPx1; OS_NOPx1
#define OS_NOPx3  OS_NOPx2; OS_NOPx1
#define OS_NOPx4  OS_NOPx3; OS_NOPx1
#define OS_NOPx5  OS_NOPx4; OS_NOPx1
#define OS_NOPx6  OS_NOPx5; OS_NOPx1
#define OS_NOPx7  OS_NOPx6; OS_NOPx1
#define OS_NOPx8  OS_NOPx7; OS_NOPx1

/*------------------------------------------------------------------------------
 * TOTAL */

#if SYSCFG_DEBUGGING == 0
#define OS_TASKTOTAL      (SYSCFG_USERTASKTOTAL + 2)
#define OS_TIMINTTOTAL     SYSCFG_USERTIMINTTOTAL
#define OS_TIMQRYTOTAL     SYSCFG_USERTIMQRYTOTAL
#else
#define OS_TASKTOTAL      (SYSCFG_USERTASKTOTAL + 4)
#define OS_TIMINTTOTAL    (SYSCFG_USERTIMINTTOTAL + 1)
#define OS_TIMQRYTOTAL    (SYSCFG_USERTIMQRYTOTAL + 2)

/*------------------------------------------------------------------------------
 * TMID */

#define OS_TMID_DEBUGGER   SYSCFG_USERTIMINTTOTAL
#define OS_TMID_DEBUGHOOK  SYSCFG_USERTIMQRYTOTAL
#if OS_TIMQRYTOTAL == 2
#define OS_TMID_TASKMGR    1
#elif OS_TIMQRYTOTAL == 3
#define OS_TMID_TASKMGR    2
#elif OS_TIMQRYTOTAL == 4
#define OS_TMID_TASKMGR    3
#elif OS_TIMQRYTOTAL == 5
#define OS_TMID_TASKMGR    4
#elif OS_TIMQRYTOTAL == 6
#define OS_TMID_TASKMGR    5
#elif OS_TIMQRYTOTAL == 7
#define OS_TMID_TASKMGR    6
#elif OS_TIMQRYTOTAL == 8
#define OS_TMID_TASKMGR    7
#elif OS_TIMQRYTOTAL == 9
#define OS_TMID_TASKMGR    8
#elif OS_TIMQRYTOTAL == 10
#define OS_TMID_TASKMGR    9
#elif OS_TIMQRYTOTAL == 11
#define OS_TMID_TASKMGR   10
#elif OS_TIMQRYTOTAL == 12
#define OS_TMID_TASKMGR   11
#elif OS_TIMQRYTOTAL == 13
#define OS_TMID_TASKMGR   12
#elif OS_TIMQRYTOTAL == 14
#define OS_TMID_TASKMGR   13
#elif OS_TIMQRYTOTAL == 15
#define OS_TMID_TASKMGR   14
#elif OS_TIMQRYTOTAL == 16
#define OS_TMID_TASKMGR   15
#elif OS_TIMQRYTOTAL == 17
#define OS_TMID_TASKMGR   16
#elif OS_TIMQRYTOTAL == 18
#define OS_TMID_TASKMGR   17
#elif OS_TIMQRYTOTAL == 19
#define OS_TMID_TASKMGR   18
#elif OS_TIMQRYTOTAL == 20
#define OS_TMID_TASKMGR   19
#elif OS_TIMQRYTOTAL == 21
#define OS_TMID_TASKMGR   20
#elif OS_TIMQRYTOTAL == 22
#define OS_TMID_TASKMGR   21
#elif OS_TIMQRYTOTAL == 23
#define OS_TMID_TASKMGR   22
#elif OS_TIMQRYTOTAL == 24
#define OS_TMID_TASKMGR   23
#elif OS_TIMQRYTOTAL == 25
#define OS_TMID_TASKMGR   24
#elif OS_TIMQRYTOTAL == 26
#define OS_TMID_TASKMGR   25
#elif OS_TIMQRYTOTAL == 27
#define OS_TMID_TASKMGR   26
#elif OS_TIMQRYTOTAL == 28
#define OS_TMID_TASKMGR   27
#elif OS_TIMQRYTOTAL == 29
#define OS_TMID_TASKMGR   28
#elif OS_TIMQRYTOTAL == 30
#define OS_TMID_TASKMGR   29
#elif OS_TIMQRYTOTAL == 31
#define OS_TMID_TASKMGR   30
#elif OS_TIMQRYTOTAL == 32
#define OS_TMID_TASKMGR   31
#elif OS_TIMQRYTOTAL == 33
#define OS_TMID_TASKMGR   32
#elif OS_TIMQRYTOTAL == 34
#define OS_TMID_TASKMGR   33
#elif OS_TIMQRYTOTAL == 35
#define OS_TMID_TASKMGR   34
#elif OS_TIMQRYTOTAL == 36
#define OS_TMID_TASKMGR   35
#elif OS_TIMQRYTOTAL == 37
#define OS_TMID_TASKMGR   36
#elif OS_TIMQRYTOTAL == 38
#define OS_TMID_TASKMGR   37
#elif OS_TIMQRYTOTAL == 39
#define OS_TMID_TASKMGR   38
#elif OS_TIMQRYTOTAL == 40
#define OS_TMID_TASKMGR   39
#elif OS_TIMQRYTOTAL == 41
#define OS_TMID_TASKMGR   40
#elif OS_TIMQRYTOTAL == 42
#define OS_TMID_TASKMGR   41
#elif OS_TIMQRYTOTAL == 43
#define OS_TMID_TASKMGR   42
#elif OS_TIMQRYTOTAL == 44
#define OS_TMID_TASKMGR   43
#elif OS_TIMQRYTOTAL == 45
#define OS_TMID_TASKMGR   44
#elif OS_TIMQRYTOTAL == 46
#define OS_TMID_TASKMGR   45
#elif OS_TIMQRYTOTAL == 47
#define OS_TMID_TASKMGR   46
#elif OS_TIMQRYTOTAL == 48
#define OS_TMID_TASKMGR   47
#elif OS_TIMQRYTOTAL == 49
#define OS_TMID_TASKMGR   48
#elif OS_TIMQRYTOTAL == 50
#define OS_TMID_TASKMGR   49
#elif OS_TIMQRYTOTAL == 51
#define OS_TMID_TASKMGR   50
#elif OS_TIMQRYTOTAL == 52
#define OS_TMID_TASKMGR   51
#elif OS_TIMQRYTOTAL == 53
#define OS_TMID_TASKMGR   52
#elif OS_TIMQRYTOTAL == 54
#define OS_TMID_TASKMGR   53
#elif OS_TIMQRYTOTAL == 55
#define OS_TMID_TASKMGR   54
#elif OS_TIMQRYTOTAL == 56
#define OS_TMID_TASKMGR   55
#elif OS_TIMQRYTOTAL == 57
#define OS_TMID_TASKMGR   56
#elif OS_TIMQRYTOTAL == 58
#define OS_TMID_TASKMGR   57
#elif OS_TIMQRYTOTAL == 59
#define OS_TMID_TASKMGR   58
#elif OS_TIMQRYTOTAL == 60
#define OS_TMID_TASKMGR   59
#elif OS_TIMQRYTOTAL == 61
#define OS_TMID_TASKMGR   60
#elif OS_TIMQRYTOTAL == 62
#define OS_TMID_TASKMGR   61
#elif OS_TIMQRYTOTAL == 63
#define OS_TMID_TASKMGR   62
#elif OS_TIMQRYTOTAL == 64
#define OS_TMID_TASKMGR   63
#elif OS_TIMQRYTOTAL == 65
#define OS_TMID_TASKMGR   64
#elif OS_TIMQRYTOTAL == 66
#define OS_TMID_TASKMGR   65
#endif
#endif

/*------------------------------------------------------------------------------
 * Time Sharing */

#if SYSCFG_TIMESHARINGMODE == 0
#define OS_TIMESHARING      SYSCFG_GLOBALTIMESHARING
#define OS_TIMESHARING_MAX  SYSCFG_GLOBALTIMESHARING
#define OS_TIMESHARING_MIN  SYSCFG_GLOBALTIMESHARING
#elif SYSCFG_TIMESHARINGMODE == 1
#define OS_TIMESHARING      SYSCFG_ALGORITHMTIMESHARING(s_task_current->pri)
#define OS_TIMESHARING_MAX  SYSCFG_ALGORITHMTIMESHARING(0)
#define OS_TIMESHARING_MIN  SYSCFG_ALGORITHMTIMESHARING(SYSCFG_TASKPRIORITY - 1)
#elif SYSCFG_TIMESHARINGMODE == 2
#define OS_TIMESHARING      s_time_sharing[s_task_current->pri]
#define OS_TIMESHARING_MAX  SYSCFG_TIMESHARING_MAX
#define OS_TIMESHARING_MIN  1
#endif
#if OS_TIMESHARING_MIN == 0 || OS_TIMESHARING_MAX > 65535
#error 时间片定义非法！
#endif

/*------------------------------------------------------------------------------
 * Taskmgr - LEN */

#if OS_TASKTOTAL <= 999
#define OS_TASKMGR_LEN_TID  6
#elif OS_TASKTOTAL <= 9999
#define OS_TASKMGR_LEN_TID  7
#elif OS_TASKTOTAL <= 99999
#define OS_TASKMGR_LEN_TID  8
#elif OS_TASKTOTAL <= 999999
#define OS_TASKMGR_LEN_TID  9
#endif

#if MCUCFG_ISA == __ARM__
#define	OS_TASKMGR_LEN_RAM  15
#else
#define	OS_TASKMGR_LEN_RAM  13
#endif

#if SYSCFG_PENDSVFIFO_MONITOR == __ENABLED__
#define OS_TASKMGR_LEN_PSVFIFO  (9 + 3 + (3 * 2 + 1)) /* (Head + Tail + Len) */
#else
#define OS_TASKMGR_LEN_PSVFIFO   0
#endif

#if SYSCFG_TASKPC_MONITOR == __ENABLED__
#define OS_TASKMGR_LEN_TASK_PC  (9 + 3 + 8) /* (Head + Tail + Len) */
#else
#define OS_TASKMGR_LEN_TASK_PC   0
#endif

#if SYSCFG_SYSTICKTIME_COUNT == __ENABLED__
#define	OS_TASKMGR_LEN_SYSTICK  (9 + 5 + 7) /* (Head + Tail + Len) */
#else
#define	OS_TASKMGR_LEN_SYSTICK   0
#endif

#define OS_TASKMGR_LEN_ALARM    (7 + 3 - 2 + 5 * 6) /* (Head + Tail - Back + Len * Line) */
#define	OS_TASKMGR_LEN_FAULT    (7 + 3 - 2 + 5 * 8) /* (Head + Tail - Back + Len * Line) */

/*------------------------------------------------------------------------------
 * DEBUGBUFF - SIZE */

#define OS_DEBUGRECVBUFFSIZE    (SYSCFG_TASKNAMELEN_MAX + 64)
#define OS_CMDLINESENDBUFFSIZE  (SYSCFG_TASKNAMELEN_MAX + 64)
#define OS_TASKMGRSENDBUFFSIZE \
( \
	( \
		  SYSCFG_TASKNAMELEN_MAX \
		+ OS_TASKMGR_LEN_TID \
		+ OS_TASKMGR_LEN_PRI \
		+ OS_TASKMGR_LEN_STA \
		+ OS_TASKMGR_LEN_CPU \
		+ OS_TASKMGR_LEN_RAM \
		+ 2 \
	) * (OS_TASKTOTAL + 2) \
	+ OS_TASKMGR_LEN_PSVFIFO \
	+ OS_TASKMGR_LEN_TASK_PC \
	+ OS_TASKMGR_LEN_SYSTICK \
	+ OS_TASKMGR_LEN_ALARM \
	+ OS_TASKMGR_LEN_FAULT \
	+ (2 * 3) \
	+ 1 \
)

/*------------------------------------------------------------------------------
 * typedef base */

typedef s_u8_t s_ecode_t;

#if OS_TASKTOTAL + 1 < 256
typedef s_u8_t  s_tid_t;
#elif OS_TASKTOTAL + 1 < 65536
typedef s_u16_t s_tid_t;
#elif OS_TASKTOTAL + 1 < 4294967296
typedef s_u32_t s_tid_t;
#else
typedef s_u64_t s_tid_t;
#endif

#if OS_TIMESHARING_MAX < 256
typedef s_u8_t  s_sharing_t;
#elif OS_TIMESHARING_MAX < 65536
typedef s_u16_t s_sharing_t;
#else
#error 时间片溢出！
#endif

typedef sDefBitsType(SYSCFG_DELAYBITS)     s_delay_t;
typedef sDefBitsType(SYSCFG_TIMINTBITS)    s_timint_t;
typedef sDefBitsType(SYSCFG_TIMQRYBITS)    s_timqry_t;
typedef sDefBitsType(SYSCFG_SEMAPHOREBITS) s_semsize_t;

/*------------------------------------------------------------------------------
 * typedef function and function pointer */

typedef s_bool_t s_boolvoid_tf(void);
typedef s_bool_t (_CODE_MEM_ *s_boolvoid_tfp)(void);
typedef void     (_CODE_MEM_ *s_voidvoid_tfp)(void);

/*------------------------------------------------------------------------------
 * typedef struct */

typedef struct
{
	sDefBitField(year);
	sDefBitField(month);
	sDefBitField(day);
	sDefBitField(hour);
	sDefBitField(minute);
	sDefBitField(second);
	sDefVoidBits(2);
}s_every_ts;

typedef struct
{
	sDefBitField(overflow_msgqueue);
	sDefBitField(overflow_taskqueue);
	sDefBitField(timedout_saferuntime);
	sDefBitField(outrange_taskpriority);
	sDefBitField(realloc_taskstack);
	sDefBitField(overflow_taskstack_future);
	sDefVoidBits(2);
}s_alarm_ts;

typedef struct
{
	sDefBitField(mallocfailed_msgnode);
	sDefBitField(mallocfailed_tasknode);
	sDefBitField(mallocfailed_taskstack);
	sDefBitField(reallocfailed_taskstack);
	sDefBitField(overflow_taskstack);
	sDefBitField(failed_startuptask);
	sDefBitField(error_recvmsg_int);
	sDefBitField(overflow_pendsvfifo);
}s_fault_ts;

typedef struct
{
	void _MALLOC_MEM_ *head;
	void _MALLOC_MEM_ *move;
	size_t size;
}s_thrmem_ts;
typedef s_thrmem_ts _STATIC_MEM_ *s_thrmem_tsp;

typedef struct s_tasknode_ts
{
	mTaskNode_Head_
	s_tid_t tid;
	s_u8_t oldpri;
	m_stacksize_t stacksize;
	struct s_tasknode_ts _OBJ_MEM_ * _STATIC_MEM_ *handle;
	s_voidvoid_tfp entry;
	#if SYSCFG_DEBUGGING == __ENABLED__
	const char _CONST_MEM_ *name;
	#endif
	#if SYSCFG_SAFERUNTIME == __ENABLED__
	s_u16_t saferuntime;
	#endif
	s_u8_t _OBJ_MEM_ *bsp;
	s_u8_t create;
	s_u8_t pri;
	s_u8_t status;
	s_u8_t blocktype;
	s_delay_t timer;
	void _STATIC_MEM_ *ptr;
	#if SYSCFG_DEBUGGING == __ENABLED__
	s_u32_t usedtime[2];
	m_stacksize_t stacklen_max;
	#endif
	#if SYSCFG_SAFERUNTIME == __ENABLED__
	s_u32_t counter;
	#endif
	struct s_tasknode_ts _OBJ_MEM_ *last;
	struct s_tasknode_ts _OBJ_MEM_ *next;
	mTaskNode_Tail_
}s_tasknode_ts;
typedef s_tasknode_ts _OBJ_MEM_ *s_tasknode_tsp;

typedef	struct
{
	const mTaskNode_Head_
	s_tid_t tid;
	const s_u8_t oldpri;
	const m_stacksize_t stacksize;
	s_tasknode_ts _OBJ_MEM_ * _STATIC_MEM_ * const handle;
	s_voidvoid_tfp entry;
	#if SYSCFG_DEBUGGING == __ENABLED__
	const char _CONST_MEM_ * const name;
	#endif
	#if SYSCFG_SAFERUNTIME == __ENABLED__
	const s_u16_t saferuntime;
	#endif
}s_taskhand_ts;
typedef	s_taskhand_ts _STATIC_MEM_ *s_taskhand_tsp;

typedef	struct
{
	const s_u8_t hookortask_autoreload;
	void _CODE_MEM_ * const hookorhand;
}s_timinthand_ts;
typedef	const s_timinthand_ts _CONST_MEM_ *s_timinthand_tsp;

typedef	struct
{
	const s_u8_t hookortask_autoreload;
	void _CODE_MEM_ * const hookorhand;
	s_boolvoid_tfp const event;
}s_timqryhand_ts;
typedef	const s_timqryhand_ts _CONST_MEM_ *s_timqryhand_tsp;

typedef struct /* 互斥信号量 */
{
	volatile s_u8_t mutex;
	s_tasknode_tsp node;
	s_u8_t oldpri;
}s_mutex_ts;
typedef s_mutex_ts _STATIC_MEM_ *s_mutex_tsp;

typedef struct /* 二值信号量 */
{
	volatile s_bool_t binary;
	s_tasknode_tsp node;
}s_binary_ts;
typedef s_binary_ts _STATIC_MEM_ *s_binary_tsp;

typedef struct /* 计数信号量 */
{
	volatile s_semsize_t counter;
	const    s_semsize_t maximum;
	s_tasknode_tsp node;
}s_semaph_ts;
typedef s_semaph_ts _STATIC_MEM_ *s_semaph_tsp;

typedef struct /* 飞信 */
{
	volatile m_fetion_t fetion;
	s_tasknode_tsp node;
}s_fetion_ts;
typedef s_fetion_ts _STATIC_MEM_ *s_fetion_tsp;

typedef struct /* 消息邮箱 */
{
	volatile s_bool_t flag;
	void *mail;
	s_tasknode_tsp node;
}s_mailbox_ts;
typedef s_mailbox_ts _STATIC_MEM_ *s_mailbox_tsp;

typedef	struct s_msgnode_ts
{
	void *msg;
	struct s_msgnode_ts _MALLOC_MEM_ *last;
	struct s_msgnode_ts _MALLOC_MEM_ *next;
}s_msgnode_ts;
typedef	s_msgnode_ts _MALLOC_MEM_ *s_msgnode_tsp;

typedef	struct
{
	size_t counter;
	const s_u8_t type;
	const s_u8_t mode;
	const size_t len;
	s_tasknode_tsp node;
	volatile s_bool_t mutex;
}s_queue_ts;
typedef	s_queue_ts _STATIC_MEM_ *s_queue_tsp;

typedef	struct
{
	size_t counter;
	const s_u8_t type;
	const s_u8_t mode;
	const size_t len;
	s_tasknode_tsp node;
	volatile s_bool_t mutex;
	void * _STATIC_MEM_ * head;
	void * _STATIC_MEM_ * tail;
	void * _STATIC_MEM_ * const base;
}ss_queue_ts;
typedef	ss_queue_ts _STATIC_MEM_ *ss_queue_tsp;

typedef	struct
{
	size_t counter;
	const s_u8_t type;
	const s_u8_t mode;
	const size_t len;
	s_tasknode_tsp node;
	volatile s_bool_t mutex;
	s_msgnode_tsp head;
	s_msgnode_tsp tail;
}sd_queue_ts;
typedef	sd_queue_ts _STATIC_MEM_ *sd_queue_tsp;

/* PendSV struct */

typedef struct
{
	const s_u8_t svid;
	s_tasknode_tsp node;
}sp_task_ts;
typedef sp_task_ts _STATIC_MEM_ *sp_task_tsp;

typedef struct
{
	const s_u8_t svid;
	s_tasknode_tsp node;
	const s_u8_t pri;
}sp_taskpri_ts;
typedef sp_taskpri_ts _STATIC_MEM_ *sp_taskpri_tsp;

typedef struct
{
	const s_u8_t svid;
	s_tasknode_tsp node;
	const s_delay_t tick;
}sp_blocktime_ts;
typedef sp_blocktime_ts _STATIC_MEM_ *sp_blocktime_tsp;

typedef struct
{
	const s_u8_t svid;
	const s_u8_t tmid;
	s_timint_t tick;
}sp_timint_ts;
typedef sp_timint_ts _STATIC_MEM_ *sp_timint_tsp;

typedef struct
{
	const s_u8_t svid;
	const s_u8_t tmid;
	s_timqry_t tick;
}sp_timqry_ts;
typedef sp_timqry_ts _STATIC_MEM_ *sp_timqry_tsp;

typedef struct
{
	const s_u8_t svid;
	s_binary_tsp const bin;
	const s_bool_t value;
}sp_binary_ts;
typedef sp_binary_ts _STATIC_MEM_ *sp_binary_tsp;

typedef struct
{
	const s_u8_t svid;
	s_semaph_tsp const sem;
}sp_semaph_ts;
typedef sp_semaph_ts _STATIC_MEM_ *sp_semaph_tsp;

typedef struct
{
	const s_u8_t svid;
	s_fetion_tsp const tbox;
	m_fetion_t fetion;
}sp_fetion_ts;
typedef sp_fetion_ts _STATIC_MEM_ *sp_fetion_tsp;

typedef struct
{
	const s_u8_t svid;
	s_mailbox_tsp const mbox;
	void *mail;
}sp_mailbox_ts;
typedef sp_mailbox_ts _STATIC_MEM_ *sp_mailbox_tsp;

typedef struct
{
	const s_u8_t svid;
	s_queue_tsp const queue;
	void *msg;
}sp_msg_ts;
typedef sp_msg_ts _STATIC_MEM_ *sp_msg_tsp;

typedef struct
{
	const s_u8_t svid;
	void _STATIC_MEM_ * const group;
	const char size;
	m_group_t value;
}sp_group_ts;
typedef sp_group_ts _STATIC_MEM_ *sp_group_tsp;

typedef struct
{
	const s_u8_t svid;
	void * const gp;
	void * const lp;
	const size_t size;
}sp_gvar_ts;
typedef sp_gvar_ts _STATIC_MEM_ *sp_gvar_tsp;

typedef struct
{
	const s_u8_t svid;
	s_voidvoid_tfp const fp;
}sp_pendsvc_ts;
typedef sp_pendsvc_ts _STATIC_MEM_ *sp_pendsvc_tsp;

typedef struct
{
	const s_u8_t svid;
	s_bool_t * const sign;
}sp_sign_ts;
typedef sp_sign_ts _STATIC_MEM_ *sp_sign_tsp;



#endif
