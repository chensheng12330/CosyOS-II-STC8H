/**************************************************************************//**
 * @item     CosyOS-II Kernel
 * @file     sv_create.h
 * @brief    创建服务（用于创建各种内核对象）
 * @author   迟凯峰
 * @version  V3.0.1
 * @date     2024.07.07
 ******************************************************************************/

#ifndef __SV_CREATE_H
#define __SV_CREATE_H



/*
 * 声明任务
 */

/* 无私信 */
#define sCSV_ExternTask(name) \
	extern s_tasknode_tsp u_taskhandle_##name; \
	extern s_tasknode_ts u_taskhand_##name; \
	void name(void)

#define dCSV_ExternTask(name) \
	extern s_tasknode_tsp u_taskhandle_##name; \
	extern s_taskhand_ts u_taskhand_##name; \
	void name(void)

/* 有私信 */
#define sCSV_ExternTask_Msg(name) \
	extern s_tasknode_tsp u_taskhandle_##name; \
	extern s_tasknode_ts u_taskhand_##name; \
	void name(MCUCFG_TASKMSG_VAR, 

#define dCSV_ExternTask_Msg(name) \
	extern s_tasknode_tsp u_taskhandle_##name; \
	extern s_taskhand_ts u_taskhand_##name; \
	void name(MCUCFG_TASKMSG_VAR, 



/*
 * 创建任务
 */

/* 创建一般任务（静态）*/
#if SYSCFG_DEBUGGING == __ENABLED__
	#if SYSCFG_SAFERUNTIME == __ENABLED__
		#define sCSV_CreateTask(name, pri, tss, srt, ntm, ent) \
			s_tasknode_ts u_taskhand_##name; \
			s_tasknode_tsp u_taskhandle_##name = &u_taskhand_##name; \
			MCUCFG_STACK_ALIGN static s_u8_t u_taskstack_##name[tss]; \
			s_tasknode_ts u_taskhand_##name = {0, 0, pri, tss, &u_taskhandle_##name, ent, #name, srt, u_taskstack_##name, 0}; \
			void name \
			sCreateTaskMsg_##ntm
	#else
		#define sCSV_CreateTask(name, pri, tss, srt, ntm, ent) \
			s_tasknode_ts u_taskhand_##name; \
			s_tasknode_tsp u_taskhandle_##name = &u_taskhand_##name; \
			MCUCFG_STACK_ALIGN static s_u8_t u_taskstack_##name[tss]; \
			s_tasknode_ts u_taskhand_##name = {0, 0, pri, tss, &u_taskhandle_##name, ent, #name, /**/ u_taskstack_##name, 0}; \
			void name \
			sCreateTaskMsg_##ntm
	#endif
#else
	#if SYSCFG_SAFERUNTIME == __ENABLED__
		#define sCSV_CreateTask(name, pri, tss, srt, ntm, ent) \
			s_tasknode_ts u_taskhand_##name; \
			s_tasknode_tsp u_taskhandle_##name = &u_taskhand_##name; \
			MCUCFG_STACK_ALIGN static s_u8_t u_taskstack_##name[tss]; \
			s_tasknode_ts u_taskhand_##name = {0, 0, pri, tss, &u_taskhandle_##name, ent, /****/ srt, u_taskstack_##name, 0}; \
			void name \
			sCreateTaskMsg_##ntm
	#else
		#define sCSV_CreateTask(name, pri, tss, srt, ntm, ent) \
			s_tasknode_ts u_taskhand_##name; \
			s_tasknode_tsp u_taskhandle_##name = &u_taskhand_##name; \
			MCUCFG_STACK_ALIGN static s_u8_t u_taskstack_##name[tss]; \
			s_tasknode_ts u_taskhand_##name = {0, 0, pri, tss, &u_taskhandle_##name, ent, /****/ /**/ u_taskstack_##name, 0}; \
			void name \
			sCreateTaskMsg_##ntm
	#endif
#endif
/* 创建一般任务（动态）*/
#if SYSCFG_DEBUGGING == __ENABLED__
	#if SYSCFG_SAFERUNTIME == __ENABLED__
		#define dCSV_CreateTask(name, pri, tss, srt, ntm, ent) \
			s_tasknode_tsp u_taskhandle_##name = OS_NULL; \
			s_taskhand_ts u_taskhand_##name = {0, 0, pri, tss, &u_taskhandle_##name, ent, #name, srt}; \
			void name \
			sCreateTaskMsg_##ntm
	#else
		#define dCSV_CreateTask(name, pri, tss, srt, ntm, ent) \
			s_tasknode_tsp u_taskhandle_##name = OS_NULL; \
			s_taskhand_ts u_taskhand_##name = {0, 0, pri, tss, &u_taskhandle_##name, ent, #name}; \
			void name \
			sCreateTaskMsg_##ntm
	#endif
#else
	#if SYSCFG_SAFERUNTIME == __ENABLED__
		#define dCSV_CreateTask(name, pri, tss, srt, ntm, ent) \
			s_tasknode_tsp u_taskhandle_##name = OS_NULL; \
			s_taskhand_ts u_taskhand_##name = {0, 0, pri, tss, &u_taskhandle_##name, ent, srt}; \
			void name \
			sCreateTaskMsg_##ntm
	#else
		#define dCSV_CreateTask(name, pri, tss, srt, ntm, ent) \
			s_tasknode_tsp u_taskhandle_##name = OS_NULL; \
			s_taskhand_ts u_taskhand_##name = {0, 0, pri, tss, &u_taskhandle_##name, ent}; \
			void name \
			sCreateTaskMsg_##ntm
	#endif
#endif
/* 创建定时中断任务 */
#define sCSV_CreateTask_TimInt(tmid, arl, name, pri, tss, srt, ntm) \
	void name(void); \
	s_tasknode_ts u_taskhand_##name; \
	const s_timinthand_ts _CONST_MEM_ u_timinthand_##tmid = {0x80 | arl, &u_taskhand_##name}; \
	sCSV_CreateTask(name, pri, tss, srt, ntm, name)

#define dCSV_CreateTask_TimInt(tmid, arl, name, pri, tss, srt, ntm) \
	void name(void); \
	s_taskhand_ts u_taskhand_##name; \
	const s_timinthand_ts _CONST_MEM_ u_timinthand_##tmid = {0x80 | arl, &u_taskhand_##name}; \
	dCSV_CreateTask(name, pri, tss, srt, ntm, name)

/* 创建定时查询任务 */
#define sCSV_CreateTask_TimQry(tmid, event, arl, name, pri, tss, srt, ntm) \
	void name(void); \
	s_bool_t u_timqryevent_##tmid(void) MCUCFG_C51USING {return (event ? true : false);} \
	s_tasknode_ts u_taskhand_##name; \
	const s_timqryhand_ts _CONST_MEM_ u_timqryhand_##tmid = {0x80 | arl, &u_taskhand_##name, u_timqryevent_##tmid}; \
	sCSV_CreateTask(name, pri, tss, srt, ntm, name)

#define dCSV_CreateTask_TimQry(tmid, event, arl, name, pri, tss, srt, ntm) \
	void name(void); \
	s_bool_t u_timqryevent_##tmid(void) MCUCFG_C51USING {return (event ? true : false);} \
	s_taskhand_ts u_taskhand_##name; \
	const s_timqryhand_ts _CONST_MEM_ u_timqryhand_##tmid = {0x80 | arl, &u_taskhand_##name, u_timqryevent_##tmid}; \
	dCSV_CreateTask(name, pri, tss, srt, ntm, name)



/*
 * 创建钩子
 */

/* 创建定时中断钩子 */
#define sCSV_CreateHook_TimInt(tmid, arl, name) \
	void name(void); \
	const s_timinthand_ts _CONST_MEM_ u_timinthand_##tmid = {0x00 | arl, name}; \
	void name(void) MCUCFG_C51USING

/* 创建定时查询钩子 */
#define sCSV_CreateHook_TimQry(tmid, event, arl, name) \
	s_bool_t u_timqryevent_##tmid(void) MCUCFG_C51USING {return (event ? true : false);} \
	void name(void); \
	const s_timqryhand_ts _CONST_MEM_ u_timqryhand_##tmid = {0x00 | arl, name, u_timqryevent_##tmid}; \
	void name(void) MCUCFG_C51USING



/*
 * 互斥信号量
 */

/* 声明 */
#define	sCSV_ExternMut(name) \
	extern s_mutex_ts name

/* 创建 */
#define	sCSV_CreateMut(name) \
	s_mutex_ts name = {false, OS_NULL, 0}



/*
 * 二值信号量
 */

/* 声明 */
#define	sCSV_ExternBin(name) \
	extern s_binary_ts name

/* 创建 */
#define	sCSV_CreateBin(name, init) \
	s_binary_ts name = {init, OS_NULL}



/*
 * 计数信号量
 */

/* 声明 */
#define	sCSV_ExternSem(name) \
	extern s_semaph_ts name

/* 创建 */
#define	sCSV_CreateSem(name, init, max) \
	s_semaph_ts name = {init, max, OS_NULL}



/*
 * 飞信
 */

/* 声明信箱 */
#define sCSV_ExternFetion(name) \
	extern s_fetion_ts name

/* 创建信箱 */
#define sCSV_CreateFetion(name) \
	s_fetion_ts name = {false, OS_NULL}



/*
 * 消息邮箱
 */

/* 声明邮箱 */
#define sCSV_ExternMailbox(name) \
	extern s_mailbox_ts name

/* 创建邮箱 */
#define sCSV_CreateMailbox(name) \
	s_mailbox_ts name = {false, OS_NULL, OS_NULL}



/*
 * 消息队列
 */

/* 声明静态消息队列 */
#define sCSV_ExternQueue(name) \
	extern ss_queue_ts name

/* 声明动态消息队列 */
#define dCSV_ExternQueue(name) \
	extern sd_queue_ts name

/* 创建静态消息队列 */
#define	sCSV_CreateQueue(name, mode, len) \
	void *u_queue_##name[len]; \
	ss_queue_ts name = {0, __STATIC__, mode, len, OS_NULL, true, u_queue_##name, u_queue_##name + len - 1, u_queue_##name}

/* 创建动态消息队列 */
#define	dCSV_CreateQueue(name, mode, len) \
	sd_queue_ts name = {0, __DYNAMIC__, mode, len, OS_NULL, true, OS_NULL, OS_NULL}



/*
 * 事件标志组（必须声明）
 */

/* 声明 */
#define sCSV_ExternFlagGroup(name, bits) \
	typedef struct{bits}u_##name##_ts; \
	extern volatile u_##name##_ts name

/* 创建 */
#define sCSV_CreateFlagGroup(name) \
	volatile u_##name##_ts name



/*
 * 线程内存
 */

/* 创建线程内存池 */
#define	sCSV_CreateMempool \
	static s_thrmem_ts u_thrmem = {OS_NULL, OS_NULL, 0}



#endif
