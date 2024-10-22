/**************************************************************************//**
 * @item     CosyOS-II Kernel
 * @file     sv_task.h
 * @brief    任务服务（仅在任务中调用）
 * @author   迟凯峰
 * @version  V3.0.0
 * @date     2024.06.25
 ******************************************************************************/

#ifndef __SV_TASK_H
#define __SV_TASK_H

void       su_enter_critical    (void);
void       su_exit_critical     (void);
void      *su_return_voidptr    (void *p);
void       su_enter_critical_one(void);
void       su_exit_critical_one (void);
void       su_exit_critical_psv (void);
void       su_task_schedule     (void);
s_ecode_t  su_startup_task      (s_taskhand_tsp hand, s_voidvoid_tfp entry, s_u8_t status);
s_ecode_t  su_suspend_task      (s_tasknode_tsp node);
s_ecode_t  su_resume_task       (s_tasknode_tsp node);
s_ecode_t  su_resume_suspend    (s_tasknode_tsp node);
s_ecode_t  su_delete_task       (s_tasknode_tsp node);
s_ecode_t  su_set_taskpri       (s_tasknode_tsp node, s_u8_t pri);
s_ecode_t  su_set_block         (s_tasknode_tsp node, s_delay_t tick);
s_ecode_t  su_clear_block       (s_tasknode_tsp node);
void       su_delay             (s_delay_t tick);
s_bool_t   su_take_mutex        (s_mutex_tsp mut, s_delay_t tick);
void       su_back_mutex        (s_mutex_tsp mut);
s_bool_t   su_wait_binary       (s_binary_tsp bin, s_delay_t tick);
s_bool_t   su_take_binary       (s_binary_tsp bin, s_delay_t tick);
void       su_give_binary       (s_binary_tsp bin);
s_bool_t   su_take_semaphore    (s_semaph_tsp sem, s_delay_t tick);
void       su_give_semaphore    (s_semaph_tsp sem);
s_bool_t   su_recv_taskmsg      (m_taskmsg_t _STATIC_MEM_ *flag, s_delay_t tick);
m_fetion_t su_recv_fetion       (s_fetion_tsp fet, s_delay_t tick);
void      *su_recv_mail         (s_mailbox_tsp mbox, s_delay_t tick);
void       su_send_mail         (s_mailbox_tsp mbox, void *mail);
void      *su_recv_msg          (s_queue_tsp queue, s_delay_t tick);
s_ecode_t  su_send_msg          (s_queue_tsp queue, void *msg);
s_bool_t   su_query_group       (void _STATIC_MEM_ *p, s_u8_t size);
s_bool_t   su_wait_group        (void _STATIC_MEM_ *p, s_u8_t size, s_delay_t tick);

s_bool_t   su_init_mempool      (s_thrmem_tsp p, size_t size);
void _MALLOC_MEM_ *su_talloc    (s_thrmem_tsp p, size_t size);
void _MALLOC_MEM_ *su_xalloc    (void _MALLOC_MEM_ *p);



/*
 * 任务
 */

/* 启动任务 */
#define sUSV_StartTask(hand, entry, status) \
( \
	(m_boolvoid_tf(su_enter_critical_one))() || true ? su_startup_task((s_taskhand_tsp)hand, entry, !status ? OS_STATUS_READY : OS_STATUS_SUSPENDED) : false \
)

/* 挂起任务 */
#define	sUSV_SuspendTask(node) \
( \
	(m_boolvoid_tf(su_enter_critical))() || true ? su_suspend_task(node) : false \
)

/* 恢复任务 */
#define	sUSV_ResumeTask(node) \
( \
	(m_boolvoid_tf(su_enter_critical))() || true ? su_resume_task(node) : false \
)

/* 恢复指定任务并挂起自身任务 */
#define	sUSV_ResumeSuspend(node) \
( \
	(m_boolvoid_tf(su_enter_critical))() || true ? su_resume_suspend(node) : false \
)

/* 删除任务 */
#define	sUSV_DeleteTask(node) \
( \
	(m_boolvoid_tf(su_enter_critical))() || true ? su_delete_task(node) : false \
)

/* 设置任务优先级 */
#define	sUSV_SetTaskPri(node, pri) \
( \
	(m_boolvoid_tf(su_enter_critical))() || true ? su_set_taskpri(node, pri) : false \
)

/* 设置阻塞（时间）*/
#define sUSV_SetBlock(node, tc) \
( \
	(m_boolvoid_tf(su_enter_critical))() || true ? su_set_block(node, tc) : false \
)

/* 清除阻塞（状态）*/
#define sUSV_ClearBlock(node) \
( \
	(m_boolvoid_tf(su_enter_critical))() || true ? su_clear_block(node) : false \
)

/* 自身任务延时 */
#define	sUSV_Delay(tc) \
do{ \
	uEnterCritical; \
	su_delay(tc); \
}while(false)



/*
 * 定时
 */

/* 定时中断 */
#define	sUSV_TimInt(tmid, tc) \
do{ \
	uEnterCritical; \
	s_timint_loader[tmid] = s_timint_reload[tmid] = tc; \
	uExitCritical; \
}while(false)

/* 定时查询 */
#define	sUSV_TimQry(tmid, tc) \
do{ \
	uEnterCritical; \
	s_timqry_loader[tmid] = s_timqry_reload[tmid] = tc; \
	uExitCritical; \
}while(false)



/*
 * 互斥信号量
 */

/* 获取 */
#define	sUSV_TakeMut(mut, tc) \
( \
	(m_boolvoid_tf(su_enter_critical_one))() || true ? su_take_mutex(&mut, tc) : false \
)

/* 归还 */
#define sUSV_BackMut(mut) \
do{ \
	uEnterCritical; \
	su_back_mutex(&mut); \
}while(false)



/*
 * 二值信号量
 */

/* 等待 */
#define	sUSV_WaitBin(bin, tc) \
( \
	(m_boolvoid_tf(su_enter_critical_one))() || true ? su_wait_binary(&bin, tc) : false \
)

/* 获取 */
#define	sUSV_TakeBin(bin, tc) \
( \
	(m_boolvoid_tf(su_enter_critical_one))() || true ? su_take_binary(&bin, tc) : false \
)

/* 给予/归还 */
#define sUSV_GiveBin(bin) \
do{ \
	uEnterCritical; \
	su_give_binary(&bin); \
}while(false)



/*
 * 计数信号量
 */

/* 获取 */
#define sUSV_TakeSem(sem, tc) \
( \
	(m_boolvoid_tf(su_enter_critical_one))() || true ? su_take_semaphore(&sem, tc) : false \
)

/* 给予/归还 */
#define sUSV_GiveSem(sem) \
do{ \
	uEnterCritical; \
	su_give_semaphore(&sem); \
}while(false)



/*
 * 私信
 */

/* 接收私信 */
#define	sUSV_RecvTaskMsg(tc) \
( \
	(m_boolvoid_tf(su_enter_critical_one))() || true \
	? su_recv_taskmsg(&m0_, tc) \
		? s_memcpy(&m0 + 1, &m0_ + 1, MCUCFG_TASKMSG_SIZE) || true \
			? (m_boolvoid_tf(su_exit_critical_one))() || true \
			: (m_boolvoid_tf(su_exit_critical_one))() || true \
		: false \
	: false \
)

/* 发送私信 */
#define	sUSV_SendTaskMsg(task) \
	uEnterCritical; \
	s_sign_send_taskmsg = true; \
	MCUCFG_TASKMSG_PSP; \
	s_schedule_taskmsg(u_taskhandle_##task); \
	task(MCUCFG_TASKMSG_VAL, 



/*
 * 飞信
 */

/* 接收飞信 */
#define sUSV_RecvFetion(tbox, tc) \
( \
	(m_boolvoid_tf(su_enter_critical_one))() || true ? su_recv_fetion(&tbox, tc) : false \
)

/* 发送飞信 */
#define sUSV_SendFetion(tbox, tion) \
do{ \
	uEnterCritical; \
	tbox.fetion = tion; \
	s_schedule_notnull(tbox.node); \
	uExitCritical; \
}while(false)



/*
 * 消息邮箱
 */

/* 接收邮件 */
#define sUSV_RecvMail(mbox, tc) \
( \
	(m_boolvoid_tf(su_enter_critical_one))() || true ? su_recv_mail(&mbox, tc) : OS_NULL \
)

/* 发送邮件 */
#define sUSV_SendMail(mbox, mail) \
do{ \
	uEnterCritical; \
	su_send_mail(&mbox, mail); \
}while(false)



/*
 * 消息队列
 */

/* 接收消息 */
#define	sUSV_RecvMsg(que, tc) \
( \
	(m_boolvoid_tf(su_enter_critical_one))() || true ? su_recv_msg(que, tc) : OS_NULL \
)

/* 发送消息 */
#define	sUSV_SendMsg(que, msg) \
( \
	(m_boolvoid_tf(su_enter_critical))() || true ? su_send_msg(que, msg) : false \
)



/*
 * 事件标志组
 */

/* 查询标志组 */
#define	sUSV_QueryFlagGroup(group) \
( \
	(m_boolvoid_tf(su_enter_critical))() || true ? su_query_group((void *)&group, sizeof(group)) : false \
)

/* 等待标志组 */
#define	sUSV_WaitFlagGroup(group, tc) \
( \
	(m_boolvoid_tf(su_enter_critical_one))() || true ? su_wait_group((void *)&group, sizeof(group), tc) : false \
)

/* 清除标志组 */
#define sUSV_ClearFlagGroup(group) \
do{ \
	uEnterCritical; \
	sizeof(group) == 1 ? *(s_u8_t  *)&group = false : MCUCFG_TERNARYMASK \
	sizeof(group) == 2 ? *(s_u16_t *)&group = false : MCUCFG_TERNARYMASK \
	sizeof(group) == 4 ? *(s_u32_t *)&group = false : false; \
	uExitCritical; \
}while(false)

/* 写标志位 */
#define sUSV_WriteFlagBit(group, bit, value) \
do{ \
	uEnterCritical; \
	group.bit = value; \
	uExitCritical; \
}while(false)



/*
 * 软件RTC
 */

/* 获取时间 */
#define sUSV_GetTime(t) \
( \
	(m_boolvoid_tf(su_enter_critical))() || true ? su_return_voidptr(s_memcpy(t, s_rtc[0], sizeof(s_rtc[0]))) : OS_NULL \
)

/* 设置时间 */
#define sUSV_SetTime(t) \
do{ \
	uEnterCritical; \
	s_memcpy(s_rtc[0], t, sizeof(s_rtc[0])); \
	uUpdateCopy(s_memcpy(s_rtc[1], s_rtc[0], sizeof(s_rtc[0]))); \
	uExitCritical; \
}while(false)



/*
 * 进程内存
 */

/* malloc */
#define	sUSV_Malloc(size) \
( \
	(m_boolvoid_tf(su_enter_critical))() || true ? su_xalloc((void _MALLOC_MEM_ *)s_malloc(size)) : OS_NULL \
)

/* calloc */
#define	sUSV_Calloc(nmemb, size) \
( \
	(m_boolvoid_tf(su_enter_critical))() || true ? su_xalloc((void _MALLOC_MEM_ *)s_calloc(nmemb, size)) : OS_NULL \
)

/* realloc */
#define	sUSV_Realloc(p, size) \
( \
	(m_boolvoid_tf(su_enter_critical))() || true ? su_xalloc((void _MALLOC_MEM_ *)s_realloc(p, size)) : OS_NULL \
)

/* free */
#define	sUSV_Free(p) \
do{ \
	uEnterCritical; \
	s_free(p); \
	uExitCritical; \
}while(false)



/*
 * 线程内存
 */

/* 初始化线程内存池 */
#define	sUSV_InitMempool(size) \
( \
	(m_boolvoid_tf(su_enter_critical))() || true ? su_init_mempool(&u_thrmem, size) : false \
)

/* 线程内存分配 */
#define	sUSV_Talloc(size) \
( \
	(m_boolvoid_tf(su_enter_critical))() || true ? su_xalloc(su_talloc(&u_thrmem, size)) : OS_NULL \
)

/* 释放线程内存池 */
#define	sUSV_FreeMempool \
do{ \
	sUSV_Free(u_thrmem.head); \
	u_thrmem.head = u_thrmem.move = OS_NULL; \
}while(false)



/*
 * 任务调度
 */

#define sUSV_TaskSchedule  su_task_schedule()



#endif
