/**************************************************************************//**
 * @item     CosyOS-II Kernel
 * @file     sv_tick.c
 * @brief    滴答服务（仅在滴答钩子、定时中断钩子、定时查询钩子中调用）
 * @author   迟凯峰
 * @version  V3.0.0
 * @date     2024.06.25
 ******************************************************************************/

#include "os_var.h"
#include "os_api.h"



/*
 * 任务
 */

/* 挂起任务 */
s_ecode_t st_suspend_task(s_tasknode_tsp node) MCUCFG_C51USING
{
	if(node == OS_NULL){
		return OS_ECODE_TASKNOTSTARTED;
	}
	if(node->status < OS_STATUS_SUSPENDED){
		node->status |= OS_STATUS_SUSPENDED;
		return OS_ECODE_NOERROR;
	}
	if(node->status & OS_STATUS_SUSPENDED){
		return OS_ECODE_TASKSUSPENDED;
	}
	if(node->status & OS_STATUS_STOPPED){
		return OS_ECODE_TASKSTOPPED;
	}
	if(node->status == OS_STATUS_DELETED){
		return OS_ECODE_TASKNOTSTARTED;
	}
	else{
		return OS_ECODE_DONOTKNOW;
	}
}

/* 恢复任务 */
s_ecode_t st_resume_task(s_tasknode_tsp node) MCUCFG_C51USING
{
	if(node == OS_NULL){
		return OS_ECODE_TASKNOTSTARTED;
	}
	if(node->status & OS_STATUS_SUSPENDED){
		node->status &= (~OS_STATUS_SUSPENDED & 0xFF);
		return OS_ECODE_NOERROR;
	}
	if(node->status < OS_STATUS_STOPPED){
		return OS_ECODE_TASKNOTSUSPENDED;
	}
	if(node->status & OS_STATUS_STOPPED){
		return OS_ECODE_TASKSTOPPED;
	}
	if(node->status == OS_STATUS_DELETED){
		return OS_ECODE_TASKNOTSTARTED;
	}
	else{
		return OS_ECODE_DONOTKNOW;
	}
}

/* 删除任务 */
s_ecode_t st_delete_task(s_tasknode_tsp node) MCUCFG_C51USING
{
	if(node == OS_NULL){
		return OS_ECODE_TASKNOTSTARTED;
	}
	if(node->status < OS_STATUS_DELETED){
		node->status = OS_STATUS_DELETED;
		return OS_ECODE_NOERROR;
	}
	if(node->status == OS_STATUS_DELETED){
		return OS_ECODE_TASKNOTSTARTED;
	}
	else{
		return OS_ECODE_DONOTKNOW;
	}
}

/* 设置任务优先级 */
s_ecode_t st_set_taskpri(s_tasknode_tsp node, s_u8_t pri) MCUCFG_C51USING
{
	if(node == OS_NULL){
		return OS_ECODE_TASKNOTSTARTED;
	}
	if(node->status < OS_STATUS_STOPPED){
		if(node->pri != pri){
			node->pri = pri;
			s_set_taskpri(node);
			return OS_ECODE_NOERROR;
		}
		else{
			return OS_ECODE_TASKPRIUNCHANGED;
		}
	}
	if(node->status & OS_STATUS_STOPPED){
		return OS_ECODE_TASKSTOPPED;
	}
	if(node->status == OS_STATUS_DELETED){
		return OS_ECODE_TASKNOTSTARTED;
	}
	else{
		return OS_ECODE_DONOTKNOW;
	}
}

/* 设置阻塞（时间）*/
s_ecode_t st_set_block(s_tasknode_tsp node, s_delay_t tick) MCUCFG_C51USING
{
	if(node == OS_NULL){
		return OS_ECODE_TASKNOTSTARTED;
	}
	if(node->status == OS_STATUS_BLOCKED || node->status == OS_STATUS_FLOATING){
		if(!tick){
			node->status = OS_STATUS_READY;
		}
		node->timer = tick;
		return OS_ECODE_NOERROR;
	}
	return OS_ECODE_TASKNOTBLOCKED;
}

/* 清除阻塞（状态）*/
s_ecode_t st_clear_block(s_tasknode_tsp node) MCUCFG_C51USING
{
	if(node == OS_NULL){
		return OS_ECODE_TASKNOTSTARTED;
	}
	if(node->status == OS_STATUS_BLOCKED || node->status == OS_STATUS_FLOATING){
		node->status = OS_STATUS_READY;
		node->timer = 0;
		return OS_ECODE_NOERROR;
	}
	return OS_ECODE_TASKNOTBLOCKED;
}



/*
 * 二值信号量
 */
#if SYSCFG_BINARY == __ENABLED__
/* 获取 */
s_bool_t st_take_binary(s_binary_tsp bin) MCUCFG_C51USING
{
	if(bin->binary){
		bin->binary = false;
		return true;
	}
	return false;
}

#endif



/*
 * 计数信号量
 */
#if SYSCFG_SEMAPHORE == __ENABLED__
/* 获取 */
s_bool_t st_take_semaphore(s_semaph_tsp sem) MCUCFG_C51USING
{
	if(sem->counter){
		(sem->counter)--;
		return true;
	}
	return false;
}

/* 给予/归还 */
void st_give_semaphore(s_semaph_tsp sem) MCUCFG_C51USING
{
	if(sem->counter < sem->maximum){
		sem->counter++;
	}
}

#endif



/*
 * 飞信
 */
#if SYSCFG_FETION == __ENABLED__
/* 接收飞信 */
m_fetion_t st_recv_fetion(s_fetion_tsp fet) MCUCFG_C51USING
{
	m_fetion_t fetion = fet->fetion;
	fet->fetion = false;
	return fetion;
}

#endif



/*
 * 消息邮箱
 */
#if SYSCFG_MAILBOX == __ENABLED__
/* 接收邮件 */
void *st_recv_mail(s_mailbox_tsp mbox) MCUCFG_C51USING
{
	if(mbox->flag){
		mbox->flag = false;
		return mbox->mail;
	}
	return OS_NULL;
}

/* 发送邮件 */
void st_send_mail(s_mailbox_tsp mbox, void *mail) MCUCFG_C51USING
{
	mbox->flag = false;
	mbox->mail = mail;
	mbox->flag = true;
}

#endif



/*
 * 消息队列
 */
#if SYSCFG_MSGQUEUE == __ENABLED__
/* 接收消息 */
void *st_recv_msg(s_queue_tsp queue) MCUCFG_C51USING
{
	void *msg = OS_NULL;
	s_msgnode_tsp node;
	queue->mutex = false;
	
	if(!queue->counter);
	else if(queue->type == __DYNAMIC__)
		sRecvMsg_Dynamic(msg);
	else
		sRecvMsg_Static(msg);
	
	queue->mutex = true;
	return msg;
}

/* 发送消息 */
s_ecode_t st_send_msg(s_queue_tsp queue, void *msg) MCUCFG_C51USING
{
	queue->mutex = false;
	
	if(queue->counter == queue->len){
		#if SYSCFG_DEBUGGING == __ENABLED__
		s_alarm.overflow_msgqueue = true;
		#endif
		queue->mutex = true;
		return OS_ECODE_OVERFLOW_MSGQUEUE;
	}
	if(queue->type == __DYNAMIC__){
		s_msgnode_tsp node = OS_NULL;
		node = (s_msgnode_tsp)s_malloc(sizeof(s_msgnode_ts));
		if(node == OS_NULL){
			#if SYSCFG_DEBUGGING == __ENABLED__
			s_fault.mallocfailed_msgnode = true;
			#endif
			queue->mutex = true;
			return OS_ECODE_MALLOCFAIL_MSGNODE;
		}
		sSendMsg_Dynamic(msg);
	}
	else
		sSendMsg_Static(msg);
	
	queue->mutex = true;
	return OS_ECODE_NOERROR;
}

#endif
