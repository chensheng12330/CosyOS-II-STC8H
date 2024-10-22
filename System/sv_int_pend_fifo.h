/**************************************************************************//**
 * @item     CosyOS-II Kernel
 * @file     sv_int_pend_fifo.h
 * @brief    中断挂起服务 with FIFO - 调用宏（仅在用户中断中调用）
 * @author   迟凯峰
 * @version  V3.0.0
 * @date     2024.06.25
 ******************************************************************************/

#ifndef __SV_INT_PEND_FIFO_H
#define __SV_INT_PEND_FIFO_H



/*
 * 任务
 */

/* 恢复任务 */
#define	sPSV_ResumeTask(load) \
do{ \
	static sp_task_ts u_psv = {OS_SVID_RESUMETASK, OS_NULL}; \
	u_psv.node = load; \
	mPendSV_FIFOLoad; \
}while(false)

/* 挂起任务 */
#define	sPSV_SuspendTask(load) \
do{ \
	static sp_task_ts u_psv = {OS_SVID_SUSPENDTASK, OS_NULL}; \
	u_psv.node = load; \
	mPendSV_FIFOLoad; \
}while(false)

/* 删除任务 */
#define	sPSV_DeleteTask(load) \
do{ \
	static sp_task_ts u_psv = {OS_SVID_DELETETASK, OS_NULL}; \
	u_psv.node = load; \
	mPendSV_FIFOLoad; \
}while(false)

/* 设置任务优先级 */
#define	sPSV_SetTaskPri(load, pri) \
do{ \
	static sp_taskpri_ts u_psv = {OS_SVID_SETTASKPRI, OS_NULL, pri}; \
	u_psv.node = load; \
	mPendSV_FIFOLoad; \
}while(false)

/* 设置阻塞（时间）*/
#define	sPSV_SetBlock(load, tc) \
do{ \
	static sp_blocktime_ts u_psv = {OS_SVID_SETBLOCK, OS_NULL, tc}; \
	u_psv.node = load; \
	mPendSV_FIFOLoad; \
}while(false)

/* 清除阻塞（状态）*/
#define	sPSV_ClearBlock(load) \
do{ \
	static sp_task_ts u_psv = {OS_SVID_CELARBLOCK, OS_NULL}; \
	u_psv.node = load; \
	mPendSV_FIFOLoad; \
}while(false)



/*
 * 定时
 */

/* 定时中断 */
#define	sPSV_TimInt(tmid, tc) \
do{ \
	static sp_timint_ts u_psv = {OS_SVID_TIMINT, tmid, 0}; \
	u_psv.tick = tc; \
	mPendSV_FIFOLoad; \
}while(false)

/* 定时查询 */
#define	sPSV_TimQry(tmid, tc) \
do{ \
	static sp_timqry_ts u_psv = {OS_SVID_TIMQRY, tmid, 0}; \
	u_psv.tick = tc; \
	mPendSV_FIFOLoad; \
}while(false)



/*
 * 二值信号量
 */

/* 上锁/给予 */
#define sPSV_WriteBin(bin, val) \
do{ \
	static sp_binary_ts u_psv = {OS_SVID_BINARY, &bin, val}; \
	mPendSV_FIFOLoad; \
}while(false)



/*
 * 计数信号量
 */

/* 给予 */
#define sPSV_GiveSem(sem) \
do{ \
	static sp_semaph_ts u_psv = {OS_SVID_GIVESEM, &sem}; \
	mPendSV_FIFOLoad; \
}while(false)



/*
 * 飞信
 */

/* 发送飞信 */
#define sPSV_SendFetion(tbox, tion) \
do{ \
	static sp_fetion_ts u_psv = {OS_SVID_SENDFETION, &tbox, 0}; \
	u_psv.fetion = tion; \
	mPendSV_FIFOLoad; \
}while(false)



/*
 * 消息邮箱
 */

/* 发送邮件 */
#define sPSV_SendMail(mbox, _mail) \
do{ \
	static sp_mailbox_ts u_psv = {OS_SVID_SENDMAIL, &mbox, OS_NULL}; \
	u_psv.mail = _mail; \
	mPendSV_FIFOLoad; \
}while(false)



/*
 * 消息队列
 */

/* 发送消息 */
#define	sPSV_SendMsg(que, _msg) \
do{ \
	static sp_msg_ts u_psv = {OS_SVID_SENDMSG, que, OS_NULL}; \
	u_psv.msg = _msg; \
	mPendSV_FIFOLoad; \
}while(false)



/*
 * 事件标志组
 */

/* 清除标志组 */
#define sPSV_ClearFlagGroup(group) \
do{ \
	static sp_group_ts u_psv = {OS_SVID_GROUP, (void *)&group, -sizeof(group), ~0}; \
	mPendSV_FIFOLoad; \
}while(false)

/* 写多标志位 */
#define sPSV_WriteFlagBits(group, sign, nbit) \
do{ \
	static sp_group_ts u_psv = {OS_SVID_GROUP, (void *)&group, sign##sizeof(group), 0}; \
	u_##group##_ts lv; \
	miWriteFlagBits; \
	sizeof(lv) == 1 ? *(s_u8_t  *)&lv = false : MCUCFG_TERNARYMASK \
	sizeof(lv) == 2 ? *(s_u16_t *)&lv = false : MCUCFG_TERNARYMASK \
	sizeof(lv) == 4 ? *(s_u32_t *)&lv = false : false; \
	siWriteFlagBits_##nbit(



/*
 * 全局变量
 */

/* 全局变量写访问 */
#define sPSV_WriteGVar(gp, lp, size) \
do{ \
	static sp_gvar_ts u_psv = {OS_SVID_GVARWRITE, gp, lp, size}; \
	mPendSV_FIFOLoad; \
}while(false)

/* 挂起服务调用 */
#define sPSV_PendSVC(fp) \
do{ \
	static sp_pendsvc_ts u_psv = {OS_SVID_PENDSVC, fp}; \
	mPendSV_FIFOLoad; \
}while(false)

/* 发送信号 */
#define sPSV_SendSign(sign) \
do{ \
	static sp_sign_ts u_psv = {OS_SVID_SENDSIGN, (s_bool_t *)&sign}; \
	mPendSV_FIFOLoad; \
}while(false)



/*
 * 软件RTC
 */

/* 设置时间 */
#define sPSV_SetTime(t) \
do{ \
	iWriteGAry(s_rtc[0], t, sizeof(s_rtc[0])); \
	iSendSign(s_sign_rtc); \
}while(false)



#endif
