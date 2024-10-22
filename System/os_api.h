/**************************************************************************//**
 * @item     CosyOS-II Kernel
 * @file     os_api.h
 * @brief    系统内核专用API
 * @author   迟凯峰
 * @version  V3.0.0
 * @date     2024.06.25
 ******************************************************************************/

#ifndef __OS_API_H
#define __OS_API_H

void s_schedule_taskpri(s_tasknode_tsp node);
void s_schedule_taskmsg(s_tasknode_tsp node);
void s_schedule_notnull(s_tasknode_tsp node);
void s_schedule_ready  (s_tasknode_tsp node);
void s_set_taskpri     (s_tasknode_tsp node);



/*
 * 标准库
 */

#define s_init_mempool  init_mempool
#define s_malloc  malloc
#define s_calloc  calloc
#define s_realloc realloc
#define s_free    free
#define s_memcpy  memcpy
#define s_memcmp  memcmp
#define s_strcpy  strcpy
#define s_strcmp  strcmp
#define s_strlen  strlen



/*
 * 任务
 */

/* 启动定时中断任务 */
#define	sStartTask_TimInt(tmid) \
	sUSV_StartTask((s_taskhand_tsp)s_timint_handle[tmid]->hookorhand, OS_NULL, OS_STATUS_SUSPENDED)

/* 启动定时查询任务 */
#define	sStartTask_TimQry(tmid) \
	sUSV_StartTask((s_taskhand_tsp)s_timqry_handle[tmid]->hookorhand, OS_NULL, OS_STATUS_SUSPENDED)

/* 恢复定时中断任务 */
#define	sResumeTask_TimInt(tmid) \
	sTSV_ResumeTask(*((s_taskhand_tsp)s_timint_handle[tmid]->hookorhand)->handle)

/* 恢复定时查询任务 */
#define	sResumeTask_TimQry(tmid) \
	sTSV_ResumeTask(*((s_taskhand_tsp)s_timqry_handle[tmid]->hookorhand)->handle)



/*
 * 更新全局变量副本
 */

#define sUpdateCopy(code) \
do{ \
	s_sign_updatecopy = true; \
	code; \
	s_sign_updatecopy = false; \
}while(false)



/*
 * 私信 - 创建信箱
 */

#define m0__  MCUCFG_TASKMSG_VAR

#if MCUCFG_TASKMSG_TYPE == 0

#define __dm  static

#define sTaskMsg_Init \
if(s_sign_send_taskmsg){ \
	s_sign_send_taskmsg = false; \
	if(s_taskcri_counter){ \
		uExitCritical; \
	} \
	return; \
} \
else{ \
	m0_ = false; \
}

#define sCreateTaskMsg_0 \
(void) \
{ \
	do

#define sCreateTaskMsg_1(m1) \
(m0__, m1##_) \
{ \
	__dm m_taskmsg_t m0; \
	__dm m1; \
	__dm m_taskmsg_t mx; \
	sTaskMsg_Init; \
	do

#define sCreateTaskMsg_2(m1, m2) \
(m0__, m1##_, m2##_) \
{ \
	__dm m_taskmsg_t m0; \
	__dm m1; __dm m2; \
	__dm m_taskmsg_t mx; \
	sTaskMsg_Init; \
	do

#define sCreateTaskMsg_3(m1, m2, m3) \
(m0__, m1##_, m2##_, m3##_) \
{ \
	__dm m_taskmsg_t m0; \
	__dm m1; __dm m2; __dm m3; \
	__dm m_taskmsg_t mx; \
	sTaskMsg_Init; \
	do

#define sCreateTaskMsg_4(m1, m2, m3, m4) \
(m0__, m1##_, m2##_, m3##_, m4##_) \
{ \
	__dm m_taskmsg_t m0; \
	__dm m1; __dm m2; __dm m3; __dm m4; \
	__dm m_taskmsg_t mx; \
	sTaskMsg_Init; \
	do

#define sCreateTaskMsg_5(m1, m2, m3, m4, m5) \
(m0__, m1##_, m2##_, m3##_, m4##_, m5##_) \
{ \
	__dm m_taskmsg_t m0; \
	__dm m1; __dm m2; __dm m3; __dm m4; __dm m5; \
	__dm m_taskmsg_t mx; \
	sTaskMsg_Init; \
	do

#define sCreateTaskMsg_6(m1, m2, m3, m4, m5, m6) \
(m0__, m1##_, m2##_, m3##_, m4##_, m5##_, m6##_) \
{ \
	__dm m_taskmsg_t m0; \
	__dm m1; __dm m2; __dm m3; __dm m4; __dm m5; __dm m6; \
	__dm m_taskmsg_t mx; \
	sTaskMsg_Init; \
	do

#define sCreateTaskMsg_7(m1, m2, m3, m4, m5, m6, m7) \
(m0__, m1##_, m2##_, m3##_, m4##_, m5##_, m6##_, m7##_) \
{ \
	__dm m_taskmsg_t m0; \
	__dm m1; __dm m2; __dm m3; __dm m4; __dm m5; __dm m6; __dm m7; \
	__dm m_taskmsg_t mx; \
	sTaskMsg_Init; \
	do

#define sCreateTaskMsg_8(m1, m2, m3, m4, m5, m6, m7, m8) \
(m0__, m1##_, m2##_, m3##_, m4##_, m5##_, m6##_, m7##_, m8##_) \
{ \
	__dm m_taskmsg_t m0; \
	__dm m1; __dm m2; __dm m3; __dm m4; __dm m5; __dm m6; __dm m7; __dm m8; \
	__dm m_taskmsg_t mx; \
	sTaskMsg_Init; \
	do

#elif MCUCFG_TASKMSG_TYPE == 1

#define __dm  __align(4) static

#define sTaskMsg_Init \
if(s_sign_send_taskmsg){ \
	register s_u32_t *p; \
	register s_u8_t size; \
	m0_ = true; \
	p = &m0_ + 1; \
	size = &m0 - &m0_ - 1; \
	do{ \
		*p++ = *m_taskmsg_psp++; \
	}while(--size); \
	s_sign_send_taskmsg = false; \
	if(s_taskcri_counter){ \
		uExitCritical; \
	} \
	return; \
}

#define sCreateTaskMsg_0 \
(void) \
{ \
	do

#define sCreateTaskMsg_1(m1) \
(m0__, m1##__) \
{ \
	__dm m_taskmsg_t m0_ = false; \
	__dm m1##_; \
	__dm m_taskmsg_t m0; \
	__dm m1; \
	sTaskMsg_Init; \
	do

#define sCreateTaskMsg_2(m1, m2) \
(m0__, m1##__, m2##__) \
{ \
	__dm m_taskmsg_t m0_ = false; \
	__dm m1##_; __dm m2##_; \
	__dm m_taskmsg_t m0; \
	__dm m1; __dm m2; \
	sTaskMsg_Init; \
	do

#define sCreateTaskMsg_3(m1, m2, m3) \
(m0__, m1##__, m2##__, m3##__) \
{ \
	__dm m_taskmsg_t m0_ = false; \
	__dm m1##_; __dm m2##_; __dm m3##_; \
	__dm m_taskmsg_t m0; \
	__dm m1; __dm m2; __dm m3; \
	sTaskMsg_Init; \
	do

#define sCreateTaskMsg_4(m1, m2, m3, m4) \
(m0__, m1##__, m2##__, m3##__, m4##__) \
{ \
	__dm m_taskmsg_t m0_ = false; \
	__dm m1##_; __dm m2##_; __dm m3##_; __dm m4##_; \
	__dm m_taskmsg_t m0; \
	__dm m1; __dm m2; __dm m3; __dm m4; \
	sTaskMsg_Init; \
	do

#define sCreateTaskMsg_5(m1, m2, m3, m4, m5) \
(m0__, m1##__, m2##__, m3##__, m4##__, m5##__) \
{ \
	__dm m_taskmsg_t m0_ = false; \
	__dm m1##_; __dm m2##_; __dm m3##_; __dm m4##_; __dm m5##_; \
	__dm m_taskmsg_t m0; \
	__dm m1; __dm m2; __dm m3; __dm m4; __dm m5; \
	sTaskMsg_Init; \
	do

#define sCreateTaskMsg_6(m1, m2, m3, m4, m5, m6) \
(m0__, m1##__, m2##__, m3##__, m4##__, m5##__, m6##__) \
{ \
	__dm m_taskmsg_t m0_ = false; \
	__dm m1##_; __dm m2##_; __dm m3##_; __dm m4##_; __dm m5##_; __dm m6##_; \
	__dm m_taskmsg_t m0; \
	__dm m1; __dm m2; __dm m3; __dm m4; __dm m5; __dm m6; \
	sTaskMsg_Init; \
	do

#define sCreateTaskMsg_7(m1, m2, m3, m4, m5, m6, m7) \
(m0__, m1##__, m2##__, m3##__, m4##__, m5##__, m6##__, m7##__) \
{ \
	__dm m_taskmsg_t m0_ = false; \
	__dm m1##_;	__dm m2##_; __dm m3##_; __dm m4##_; __dm m5##_; __dm m6##_; __dm m7##_; \
	__dm m_taskmsg_t m0; \
	__dm m1; __dm m2; __dm m3; __dm m4; __dm m5; __dm m6; __dm m7; \
	sTaskMsg_Init; \
	do

#define sCreateTaskMsg_8(m1, m2, m3, m4, m5, m6, m7, m8) \
(m0__, m1##__, m2##__, m3##__, m4##__, m5##__, m6##__, m7##__, m8##__) \
{ \
	__dm m_taskmsg_t m0_ = false; \
	__dm m1##_;	__dm m2##_; __dm m3##_; __dm m4##_; __dm m5##_; __dm m6##_; __dm m7##_; __dm m8##_; \
	__dm m_taskmsg_t m0; \
	__dm m1; __dm m2; __dm m3; __dm m4; __dm m5; __dm m6; __dm m7; __dm m8; \
	sTaskMsg_Init; \
	do

#endif



/*
 * 事件标志组 - 写标志位
 */

#define siWriteFlagBits \
		u_psv.value = ( \
			  sizeof(lv) == 1 ? *(s_u8_t  *)&lv \
			: sizeof(lv) == 2 ? *(s_u16_t *)&lv \
			: sizeof(lv) == 4 ? *(s_u32_t *)&lv \
			: 0 \
		); \
	} \
	mPendSV_FIFOLoad; \
}while(false)

#define siWriteFlagBits_1(bit1) \
	lv.bit1 = true; \
	siWriteFlagBits

#define siWriteFlagBits_2(bit1, bit2) \
	lv.bit1 = lv.bit2 = true; \
	siWriteFlagBits

#define siWriteFlagBits_3(bit1, bit2, bit3) \
	lv.bit1 = lv.bit2 = lv.bit3 = true; \
	siWriteFlagBits

#define siWriteFlagBits_4(bit1, bit2, bit3, bit4) \
	lv.bit1 = lv.bit2 = lv.bit3 = lv.bit4 = true; \
	siWriteFlagBits

#define siWriteFlagBits_5(bit1, bit2, bit3, bit4, bit5) \
	lv.bit1 = lv.bit2 = lv.bit3 = lv.bit4 = lv.bit5 = true; \
	siWriteFlagBits

#define siWriteFlagBits_6(bit1, bit2, bit3, bit4, bit5, bit6) \
	lv.bit1 = lv.bit2 = lv.bit3 = lv.bit4 = lv.bit5 = lv.bit6 = true; \
	siWriteFlagBits

#define siWriteFlagBits_7(bit1, bit2, bit3, bit4, bit5, bit6, bit7) \
	lv.bit1 = lv.bit2 = lv.bit3 = lv.bit4 = lv.bit5 = lv.bit6 = lv.bit7 = true; \
	siWriteFlagBits

#define siWriteFlagBits_8(bit1, bit2, bit3, bit4, bit5, bit6, bit7, bit8) \
	lv.bit1 = lv.bit2 = lv.bit3 = lv.bit4 = lv.bit5 = lv.bit6 = lv.bit7 = lv.bit8 = true; \
	siWriteFlagBits

#define siWriteFlagBits_9(bit1, bit2, bit3, bit4, bit5, bit6, bit7, bit8, bit9) \
	lv.bit1 = lv.bit2 = lv.bit3 = lv.bit4 = lv.bit5 = lv.bit6 = lv.bit7 = lv.bit8 = true; \
	lv.bit9 = true; \
	siWriteFlagBits

#define siWriteFlagBits_10(bit1, bit2, bit3, bit4, bit5, bit6, bit7, bit8, bit9, bit10) \
	lv.bit1 = lv.bit2 = lv.bit3 = lv.bit4 = lv.bit5 = lv.bit6 = lv.bit7 = lv.bit8 = true; \
	lv.bit9 = lv.bit10 = true; \
	siWriteFlagBits

#define siWriteFlagBits_11(bit1, bit2, bit3, bit4, bit5, bit6, bit7, bit8, bit9, bit10, bit11) \
	lv.bit1 = lv.bit2 = lv.bit3 = lv.bit4 = lv.bit5 = lv.bit6 = lv.bit7 = lv.bit8 = true; \
	lv.bit9 = lv.bit10 = lv.bit11 = true; \
	siWriteFlagBits

#define siWriteFlagBits_12(bit1, bit2, bit3, bit4, bit5, bit6, bit7, bit8, bit9, bit10, bit11, bit12) \
	lv.bit1 = lv.bit2 = lv.bit3 = lv.bit4 = lv.bit5 = lv.bit6 = lv.bit7 = lv.bit8 = true; \
	lv.bit9 = lv.bit10 = lv.bit11 = lv.bit12 = true; \
	siWriteFlagBits

#define siWriteFlagBits_13(bit1, bit2, bit3, bit4, bit5, bit6, bit7, bit8, bit9, bit10, bit11, bit12, bit13) \
	lv.bit1 = lv.bit2 = lv.bit3 = lv.bit4 = lv.bit5 = lv.bit6 = lv.bit7 = lv.bit8 = true; \
	lv.bit9 = lv.bit10 = lv.bit11 = lv.bit12 = lv.bit13 = true; \
	siWriteFlagBits

#define siWriteFlagBits_14(bit1, bit2, bit3, bit4, bit5, bit6, bit7, bit8, bit9, bit10, bit11, bit12, bit13, bit14) \
	lv.bit1 = lv.bit2 = lv.bit3 = lv.bit4 = lv.bit5 = lv.bit6 = lv.bit7 = lv.bit8 = true; \
	lv.bit9 = lv.bit10 = lv.bit11 = lv.bit12 = lv.bit13 = lv.bit14 = true; \
	siWriteFlagBits

#define siWriteFlagBits_15(bit1, bit2, bit3, bit4, bit5, bit6, bit7, bit8, bit9, bit10, bit11, bit12, bit13, bit14, bit15) \
	lv.bit1 = lv.bit2 = lv.bit3 = lv.bit4 = lv.bit5 = lv.bit6 = lv.bit7 = lv.bit8 = true; \
	lv.bit9 = lv.bit10 = lv.bit11 = lv.bit12 = lv.bit13 = lv.bit14 = lv.bit15 = true; \
	siWriteFlagBits

#define siWriteFlagBits_16(bit1, bit2, bit3, bit4, bit5, bit6, bit7, bit8, bit9, bit10, bit11, bit12, bit13, bit14, bit15, bit16) \
	lv.bit1 = lv.bit2 = lv.bit3 = lv.bit4 = lv.bit5 = lv.bit6 = lv.bit7 = lv.bit8 = true; \
	lv.bit9 = lv.bit10 = lv.bit11 = lv.bit12 = lv.bit13 = lv.bit14 = lv.bit15 = lv.bit16 = true; \
	siWriteFlagBits



/*
 * 消息队列
 */

/* 动态队列 - 发送消息 */
#define sSendMsg_Dynamic(p) \
do{ \
	node->msg = p; \
	node->next = OS_NULL; \
	node->last = ((sd_queue_tsp)queue)->tail; \
	if(((sd_queue_tsp)queue)->tail != OS_NULL){ \
		((sd_queue_tsp)queue)->tail->next = node; \
	} \
	((sd_queue_tsp)queue)->tail = node; \
	if(((sd_queue_tsp)queue)->head == OS_NULL){ \
		((sd_queue_tsp)queue)->head = node; \
	} \
	queue->counter++; \
}while(false)

/* 动态队列 - 接收消息 */
#define sRecvMsg_Dynamic(p) \
do{ \
	if(queue->mode == __MSGQUE_LIFO__){ \
		node = ((sd_queue_tsp)queue)->tail; \
		p = node->msg; \
		node = node->last; \
		s_free(((sd_queue_tsp)queue)->tail); \
		((sd_queue_tsp)queue)->tail = node; \
		if(node == OS_NULL){ \
			((sd_queue_tsp)queue)->head = OS_NULL; \
		} \
		else{ \
			node->next = OS_NULL; \
		} \
	} \
	else{ \
		node = ((sd_queue_tsp)queue)->head; \
		p = node->msg; \
		node = node->next; \
		s_free(((sd_queue_tsp)queue)->head); \
		((sd_queue_tsp)queue)->head = node; \
		if(node == OS_NULL){ \
			((sd_queue_tsp)queue)->tail = OS_NULL; \
		} \
		else{ \
			node->last = OS_NULL; \
		} \
	} \
	queue->counter--; \
}while(false)

/* 静态队列 - 发送消息 */
#define sSendMsg_Static(p) \
do{ \
	if(((ss_queue_tsp)queue)->tail < ((ss_queue_tsp)queue)->base + queue->len - 1){ \
		((ss_queue_tsp)queue)->tail++; \
	} \
	else{ \
		((ss_queue_tsp)queue)->tail = ((ss_queue_tsp)queue)->base; \
	} \
	*((ss_queue_tsp)queue)->tail = p; \
	queue->counter++; \
}while(false)

/* 静态队列 - 接收消息 */
#define sRecvMsg_Static(p) \
do{ \
	if(queue->mode == __MSGQUE_LIFO__){ \
		p = *((ss_queue_tsp)queue)->tail; \
		if(((ss_queue_tsp)queue)->tail > ((ss_queue_tsp)queue)->base){ \
			((ss_queue_tsp)queue)->tail--; \
		} \
		else{ \
			((ss_queue_tsp)queue)->tail = ((ss_queue_tsp)queue)->base + queue->len - 1; \
		} \
	} \
	else{ \
		p = *((ss_queue_tsp)queue)->head; \
		if(((ss_queue_tsp)queue)->head < ((ss_queue_tsp)queue)->base + queue->len - 1){ \
			((ss_queue_tsp)queue)->head++; \
		} \
		else{ \
			((ss_queue_tsp)queue)->head = ((ss_queue_tsp)queue)->base; \
		} \
	} \
	queue->counter--; \
}while(false)



#endif
