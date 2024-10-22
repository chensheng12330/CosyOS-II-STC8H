/**************************************************************************//**
 * @item     CosyOS-II Kernel
 * @file     sv_int_loc.h
 * @brief    中断本地服务（仅在用户中断中调用，并在本地执行）
 * @author   迟凯峰
 * @version  V3.0.0
 * @date     2024.06.25
 ******************************************************************************/

#ifndef __SV_INT_LOC_H
#define __SV_INT_LOC_H

void       si_task_schedule   (void);
s_bool_t   si_take_binary     (s_binary_tsp bin);
m_fetion_t si_recv_fetion     (s_fetion_tsp fet);
void      *si_recv_mail       (s_mailbox_tsp mbox);
void      *si_recv_msg        (s_queue_tsp queue);
s_bool_t   si_can_access_queue(s_queue_tsp queue);



/*
 * 二值信号量获取
 */

#define sISV_TakeBin(bin) \
	si_take_binary(&bin)



/*
 * 接收飞信
 */

#define sISV_RecvFetion(tbox) \
	si_recv_fetion(&tbox)



/*
 * 接收邮件
 */

#define sISV_RecvMail(mbox) \
	si_recv_mail(&mbox)



/*
 * 接收消息
 */
#if SYSCFG_MCUCORE == 8051
#define	sISV_RecvMsg(que) \
( \
	si_can_access_queue(que) ? si_recv_msg(que) : OS_NULL \
)
#else
#define	sISV_RecvMsg(que) \
	si_recv_msg(que)
#endif



/*
 * 查询标志组
 */

#define	sISV_QueryFlagGroup(group) \
( \
	  sizeof(group) == 1 ? *(s_u8_t  *)&group ? true : false \
	: sizeof(group) == 2 ? *(s_u16_t *)&group ? true : false \
	: sizeof(group) == 4 ? *(s_u32_t *)&group ? true : false \
	: false \
)



/*
 * 软件RTC - 获取时间
 */

#define sISV_GetTime(t) \
	s_memcpy(t, !iWhichGVar_Read ? s_rtc[0] : s_rtc[1], sizeof(s_rtc[0]))



/*
 * 任务调度
 */

#define sISV_TaskSchedule \
	si_task_schedule()



#endif
