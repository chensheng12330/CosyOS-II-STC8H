/**************************************************************************//**
 * @item     CosyOS-II Kernel
 * @file     sv_tick.h
 * @brief    滴答服务（仅在滴答钩子、定时中断钩子、定时查询钩子中调用）
 * @author   迟凯峰
 * @version  V3.0.0
 * @date     2024.06.25
 ******************************************************************************/

#ifndef __SV_TICK_H
#define __SV_TICK_H

s_ecode_t  st_suspend_task   (s_tasknode_tsp node);
s_ecode_t  st_resume_task    (s_tasknode_tsp node);
s_ecode_t  st_delete_task    (s_tasknode_tsp node);
s_ecode_t  st_set_taskpri    (s_tasknode_tsp node, s_u8_t pri);
s_ecode_t  st_set_block      (s_tasknode_tsp node, s_delay_t tick);
s_ecode_t  st_clear_block    (s_tasknode_tsp node);
s_bool_t   st_take_binary    (s_binary_tsp bin);
s_bool_t   st_take_semaphore (s_semaph_tsp sem);
void       st_give_semaphore (s_semaph_tsp sem);
m_fetion_t st_recv_fetion    (s_fetion_tsp fet);
void      *st_recv_mail      (s_mailbox_tsp mbox);
void       st_send_mail      (s_mailbox_tsp mbox, void *mail);
void      *st_recv_msg       (s_queue_tsp queue);
s_ecode_t  st_send_msg       (s_queue_tsp queue, void *msg);



/*
 * 任务
 */

/* 挂起指定任务 */
#define	sTSV_SuspendTask(node) \
	st_suspend_task(node)

/* 恢复指定任务 */
#define	sTSV_ResumeTask(node) \
	st_resume_task(node)

/* 删除指定任务 */
#define	sTSV_DeleteTask(node) \
	st_delete_task(node)

/* 设置任务优先级 */
#define	sTSV_SetTaskPri(node, tpl) \
	st_set_taskpri(node, tpl)

/* 设置阻塞（时间）*/
#define	sTSV_SetBlock(node, tc) \
	st_set_block(node, tc)

/* 清除阻塞（状态）*/
#define	sTSV_ClearBlock(node) \
	st_clear_block(node)



/*
 * 定时
 */

/* 定时中断 */
#define	sTSV_TimInt(tmid, tc) \
	s_timint_loader[tmid] = s_timint_reload[tmid] = tc

/* 定时查询 */
#define	sTSV_TimQry(tmid, tc) \
	s_timqry_loader[tmid] = s_timqry_reload[tmid] = tc



/*
 * 二值信号量
 */

/* 获取 */
#define sTSV_TakeBin(bin) \
	st_take_binary(&bin)



/*
 * 计数信号量
 */

/* 获取 */
#define sTSV_TakeSem(sem) \
	st_take_semaphore(&sem)

/* 给予/归还 */
#define sTSV_GiveSem(sem) \
	st_give_semaphore(&sem)



/*
 * 私信
 */

/* 发送私信 */
#define	sTSV_SendTaskMsg(task) \
	s_sign_send_taskmsg = true; \
	MCUCFG_TASKMSG_PSP; \
	task(MCUCFG_TASKMSG_VAL, 



/*
 * 飞信
 */

/* 接收飞信 */
#define sTSV_RecvFetion(tbox) \
	st_recv_fetion(&tbox)



/*
 * 消息邮箱
 */

/* 接收邮件 */
#define sTSV_RecvMail(mbox) \
	st_recv_mail(&mbox)

/* 发送邮件 */
#define sTSV_SendMail(mbox, mail) \
	st_send_mail(&mbox, mail)



/*
 * 消息队列
 */

/* 接收消息 */
#define	sTSV_RecvMsg(que) \
	st_recv_msg(que)

/* 发送消息 */
#define	sTSV_SendMsg(que, msg) \
	st_send_msg(que, msg)



/*
 * 事件标志组
 */

/* 查询标志组 */
#define	sTSV_QueryFlagGroup(group) \
( \
	  sizeof(group) == 1 ? *(s_u8_t  *)&group ? true : false \
	: sizeof(group) == 2 ? *(s_u16_t *)&group ? true : false \
	: sizeof(group) == 4 ? *(s_u32_t *)&group ? true : false \
	: false \
)

/* 清除标志组 */
#define sTSV_ClearFlagGroup(group) \
do{ \
	sizeof(group) == 1 ? *(s_u8_t  *)&group = false : MCUCFG_TERNARYMASK \
	sizeof(group) == 2 ? *(s_u16_t *)&group = false : MCUCFG_TERNARYMASK \
	sizeof(group) == 4 ? *(s_u32_t *)&group = false : false; \
}while(false)



/*
 * 软件RTC
 */

/* 获取时间 */
#define sTSV_GetTime(t) \
	s_memcpy(t, s_rtc[0], sizeof(s_rtc[0]))

/* 设置时间 */
#define sTSV_SetTime(t) \
do{ \
	s_memcpy(s_rtc[0], t, sizeof(s_rtc[0])); \
	tUpdateCopy(s_memcpy(s_rtc[1], s_rtc[0], sizeof(s_rtc[0]))); \
}while(false)



#endif
