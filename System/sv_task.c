/**************************************************************************//**
 * @item     CosyOS-II Kernel
 * @file     sv_task.c
 * @brief    任务服务（仅在任务中调用）
 * @author   迟凯峰
 * @version  V3.0.0
 * @date     2024.06.25
 ******************************************************************************/

#include "os_var.h"
#include "os_api.h"
#include "ur_api.h"

/* 进入嵌套任务临界区 */
void su_enter_critical(void)
{
	mSysIRQ_Disable;
	s_taskcri_counter++;
}

/* 退出嵌套任务临界区 */
void su_exit_critical(void)
{
	s_taskcri_counter--;
	if(!s_taskcri_counter){
		if(s_sign_schedule){
			mPendSV_Set;
			mSysIRQ_Enable;
			#if MCUCFG_ISA == __ARM__
			OS_NOPx1;
			#endif
			while(s_sign_schedule);
		}
		else{
			mSysIRQ_Enable;
		}
	}
}

/* 退出嵌套任务临界区并返回(void *) */
void *su_return_voidptr(void *p)
{
	uExitCritical;
	return p;
}

/* 进入非嵌套任务临界区 */
void su_enter_critical_one(void)
{
	mSysIRQ_Disable;
}

/* 退出非嵌套任务临界区 */
void su_exit_critical_one(void)
{
	mSysIRQ_Enable;
}

/* 退出非嵌套任务临界区并触发任务调度 */
void su_exit_critical_psv(void)
{
	s_sign_schedule = true;
	mPendSV_Set;
	mSysIRQ_Enable;
	#if MCUCFG_ISA == __ARM__
	OS_NOPx1;
	#endif
	while(s_sign_schedule);
}

/* 任务调度 */
void su_task_schedule(void)
{
	uEnterCritical;
	s_sign_schedule_all = true;
	s_sign_schedule = true;
	uExitCritical;
}

/* 启动任务 */
s_ecode_t su_startup_task(s_taskhand_tsp hand, s_voidvoid_tfp entry, s_u8_t status)
{
	static s_tid_t _SYS_MEM_ tid = 0;
	if(!hand->tid){
		tid++;
		hand->tid = tid;
	}
	if(*hand->handle != OS_NULL){
		((s_tasknode_tsp)hand)->status = status;
	}
	else{
		s_task_status0 = status;
	}
	if(hand->entry == OS_NULL){
		hand->entry = entry;
	}
	s_task_starter = hand;
	if(!s_sign_return){
		s_sign_schedule_all = true;
	}
	su_exit_critical_psv();
	return s_startup_code;
}

/* 挂起任务 */
s_ecode_t su_suspend_task(s_tasknode_tsp node)
{
	if(node == OS_NULL){
		uExitCritical;
		return OS_ECODE_TASKNOTSTARTED;
	}
	if(node->status < OS_STATUS_SUSPENDED){
		node->status |= OS_STATUS_SUSPENDED;
		if(node == s_task_current){
			s_sign_schedule = true;
		}
		uExitCritical;
		return OS_ECODE_NOERROR;
	}
	if(node->status & OS_STATUS_SUSPENDED){
		uExitCritical;
		return OS_ECODE_TASKSUSPENDED;
	}
	if(node->status & OS_STATUS_STOPPED){
		uExitCritical;
		return OS_ECODE_TASKSTOPPED;
	}
	if(node->status == OS_STATUS_DELETED){
		uExitCritical;
		return OS_ECODE_TASKNOTSTARTED;
	}
	else{
		uExitCritical;
		return OS_ECODE_DONOTKNOW;
	}
}

/* 恢复任务 */
s_ecode_t su_resume_task(s_tasknode_tsp node)
{
	if(node == OS_NULL){
		uExitCritical;
		return OS_ECODE_TASKNOTSTARTED;
	}
	if(node->status & OS_STATUS_SUSPENDED){
		node->status &= (~OS_STATUS_SUSPENDED & 0xFF);
		s_schedule_ready(node);
		uExitCritical;
		return OS_ECODE_NOERROR;
	}
	if(node->status < OS_STATUS_STOPPED){
		uExitCritical;
		return OS_ECODE_TASKNOTSUSPENDED;
	}
	if(node->status & OS_STATUS_STOPPED){
		uExitCritical;
		return OS_ECODE_TASKSTOPPED;
	}
	if(node->status == OS_STATUS_DELETED){
		uExitCritical;
		return OS_ECODE_TASKNOTSTARTED;
	}
	else{
		uExitCritical;
		return OS_ECODE_DONOTKNOW;
	}
}

/* 恢复指定任务并挂起自身任务 */
s_ecode_t su_resume_suspend(s_tasknode_tsp node)
{
	if(node == OS_NULL){
		uExitCritical;
		return OS_ECODE_TASKNOTSTARTED;
	}
	if(node->status & OS_STATUS_SUSPENDED){
		node->status &= (~OS_STATUS_SUSPENDED & 0xFF);
	}
	if(node->status == OS_STATUS_READY){
		s_task_current->status |= OS_STATUS_SUSPENDED;
		s_schedule_taskpri(node);
		s_sign_schedule = true;
		uExitCritical;
		return OS_ECODE_NOERROR;
	}
	if(node->status < OS_STATUS_STOPPED){
		uExitCritical;
		return OS_ECODE_TASKNOTREADY;
	}
	if(node->status & OS_STATUS_STOPPED){
		uExitCritical;
		return OS_ECODE_TASKSTOPPED;
	}
	if(node->status == OS_STATUS_DELETED){
		uExitCritical;
		return OS_ECODE_TASKNOTSTARTED;
	}
	else{
		uExitCritical;
		return OS_ECODE_DONOTKNOW;
	}
}

/* 删除任务 */
s_ecode_t su_delete_task(s_tasknode_tsp node)
{
	if(node == OS_NULL){
		uExitCritical;
		return OS_ECODE_TASKNOTSTARTED;
	}
	if(node->status < OS_STATUS_DELETED){
		node->status = OS_STATUS_DELETED;
		if(node == s_task_current){
			s_sign_schedule = true;
		}
		uExitCritical;
		return OS_ECODE_NOERROR;
	}
	if(node->status == OS_STATUS_DELETED){
		uExitCritical;
		return OS_ECODE_TASKNOTSTARTED;
	}
	else{
		uExitCritical;
		return OS_ECODE_DONOTKNOW;
	}
}

/* 设置任务优先级 */
s_ecode_t su_set_taskpri(s_tasknode_tsp node, s_u8_t pri)
{
	if(node == OS_NULL){
		uExitCritical;
		return OS_ECODE_TASKNOTSTARTED;
	}
	if(node->status < OS_STATUS_STOPPED){
		if(node->pri != pri){
			node->pri = pri;
			s_set_taskpri(node);
			s_sign_schedule = true;
			uExitCritical;
			return OS_ECODE_NOERROR;
		}
		else{
			uExitCritical;
			return OS_ECODE_TASKPRIUNCHANGED;
		}
	}
	if(node->status & OS_STATUS_STOPPED){
		uExitCritical;
		return OS_ECODE_TASKSTOPPED;
	}
	if(node->status == OS_STATUS_DELETED){
		uExitCritical;
		return OS_ECODE_TASKNOTSTARTED;
	}
	else{
		uExitCritical;
		return OS_ECODE_DONOTKNOW;
	}
}

/* 设置阻塞（时间）*/
s_ecode_t su_set_block(s_tasknode_tsp node, s_delay_t tick)
{
	if(node == OS_NULL){
		uExitCritical;
		return OS_ECODE_TASKNOTSTARTED;
	}
	if(node->status == OS_STATUS_BLOCKED || node->status == OS_STATUS_FLOATING){
		if(!tick){
			node->status = OS_STATUS_READY;
			s_schedule_taskpri(node);
		}
		node->timer = tick;
		uExitCritical;
		return OS_ECODE_NOERROR;
	}
	else{
		uExitCritical;
		return OS_ECODE_TASKNOTBLOCKED;
	}
}

/* 清除阻塞（状态）*/
s_ecode_t su_clear_block(s_tasknode_tsp node)
{
	if(node == OS_NULL){
		uExitCritical;
		return OS_ECODE_TASKNOTSTARTED;
	}
	if(node->status == OS_STATUS_BLOCKED || node->status == OS_STATUS_FLOATING){
		node->status = OS_STATUS_READY;
		node->timer = 0;
		s_schedule_taskpri(node);
		uExitCritical;
		return OS_ECODE_NOERROR;
	}
	else{
		uExitCritical;
		return OS_ECODE_TASKNOTBLOCKED;
	}
}

/* 自身任务延时 */
void su_delay(s_delay_t tick)
{
	if(s_task_current->status == OS_STATUS_READY){
		s_task_current->status = OS_STATUS_BLOCKED;
		s_task_current->timer = tick;
		s_sign_schedule = true;
	}
	else if(s_task_current->status == OS_STATUS_BLOCKED){
		s_task_current->timer += tick;
	}
	uExitCritical;
}



/*
 * 互斥信号量
 */
#if SYSCFG_MUTEX == __ENABLED__
/* 获取 */
s_bool_t su_take_mutex(s_mutex_tsp mut, s_delay_t tick)
{
	if(!mut->mutex) goto __RET_TRUE1;
	if(s_task_current == mut->node) goto __RET_TRUE2;
	if(!tick) goto __RET_FALSE;
	
	s_task_current->status = OS_STATUS_BLOCKED;
	s_task_current->blocktype = OS_BLOCKED_MUTEX | 0x0F;
	s_task_current->timer = tick;
	s_task_current->ptr = mut;
	if(s_task_current->pri > mut->node->pri){
		mut->node->pri = s_task_current->pri;
		s_set_taskpri(mut->node);
	}
	su_exit_critical_psv();
	s_task_current->blocktype = 0;
	#if SYSCFG_MCUCORE == 8051
	mut = (s_mutex_tsp)s_task_current->ptr;
	#endif
	if(!mut->mutex) goto __RET_TRUE1;
	
__RET_FALSE:
	mSysIRQ_Enable;
	return false;
	
__RET_TRUE1:
	mut->node = s_task_current;
	mut->oldpri = s_task_current->pri;
	
__RET_TRUE2:
	mut->mutex++;
	mSysIRQ_Enable;
	return true;
}

/* 归还 */
void su_back_mutex(s_mutex_tsp mut)
{
	if(!mut->mutex);
	else if(s_task_current == mut->node){
		mut->mutex--;
		if(!mut->mutex){
			if(s_task_current->pri > mut->oldpri){
				s_task_current->pri = mut->oldpri;
				s_set_taskpri(s_task_current);
				s_sign_schedule = true;
			}
		}
	}
	uExitCritical;
}

#endif



/*
 * 二值信号量
 */
#if SYSCFG_BINARY == __ENABLED__
/* 等待 */
s_bool_t su_wait_binary(s_binary_tsp bin, s_delay_t tick)
{
	if(bin->binary) goto __RET_TRUE;
	if(!tick) goto __RET_FALSE;
	
	bin->node = s_task_current;
	s_task_current->status = OS_STATUS_BLOCKED;
	s_task_current->blocktype = OS_BLOCKED_BINARY | sizeof(bin->binary);
	s_task_current->timer = tick;
	s_task_current->ptr = bin;
	su_exit_critical_psv();
	s_task_current->blocktype = 0;
	#if SYSCFG_MCUCORE == 8051
	bin = (s_binary_tsp)s_task_current->ptr;
	#endif
	bin->node = OS_NULL;
	if(bin->binary) goto __RET_TRUE;
	
__RET_FALSE:
	mSysIRQ_Enable;
	return false;
	
__RET_TRUE:
	mSysIRQ_Enable;
	return true;
}

/* 获取 */
s_bool_t su_take_binary(s_binary_tsp bin, s_delay_t tick)
{
	if(bin->binary) goto __RET_TRUE;
	if(!tick) goto __RET_FALSE;
	
	if(bin->node == OS_NULL || s_task_current->pri > bin->node->pri){
		bin->node = s_task_current;
	}
	s_task_current->status = OS_STATUS_BLOCKED;
	s_task_current->blocktype = OS_BLOCKED_BINARY | sizeof(bin->binary);
	s_task_current->timer = tick;
	s_task_current->ptr = bin;
	su_exit_critical_psv();
	s_task_current->blocktype = 0;
	#if SYSCFG_MCUCORE == 8051
	bin = (s_binary_tsp)s_task_current->ptr;
	#endif
	bin->node = OS_NULL;
	if(bin->binary) goto __RET_TRUE;
	
__RET_FALSE:
	mSysIRQ_Enable;
	return false;
	
__RET_TRUE:
	bin->binary = false;
	mSysIRQ_Enable;
	return true;
}

/* 给予/归还 */
void su_give_binary(s_binary_tsp bin)
{
	bin->binary = true;
	s_schedule_notnull(bin->node);
	uExitCritical;
}

#endif



/*
 * 计数信号量
 */
#if SYSCFG_SEMAPHORE == __ENABLED__
/* 获取 */
s_bool_t su_take_semaphore(s_semaph_tsp sem, s_delay_t tick)
{
	if(sem->counter) goto __RET_TRUE;
	if(!tick) goto __RET_FALSE;
	
	if(sem->node == OS_NULL || s_task_current->pri > sem->node->pri){
		sem->node = s_task_current;
	}
	s_task_current->status = OS_STATUS_BLOCKED;
	s_task_current->blocktype = OS_BLOCKED_SEMAPHORE | sizeof(sem->counter);
	s_task_current->timer = tick;
	s_task_current->ptr = sem;
	su_exit_critical_psv();
	s_task_current->blocktype = 0;
	#if SYSCFG_MCUCORE == 8051
	sem = (s_semaph_tsp)s_task_current->ptr;
	#endif
	sem->node = OS_NULL;
	if(sem->counter) goto __RET_TRUE;
	
__RET_FALSE:
	mSysIRQ_Enable;
	return false;
	
__RET_TRUE:
	(sem->counter)--;
	mSysIRQ_Enable;
	return true;
}

/* 给予/归还 */
void su_give_semaphore(s_semaph_tsp sem)
{
	if(sem->counter < sem->maximum){
		sem->counter++;
	}
	s_schedule_notnull(sem->node);
	uExitCritical;
}

#endif



/*
 * 私信
 */
#if SYSCFG_TASKMSG == __ENABLED__
/* 接收私信 */
s_bool_t su_recv_taskmsg(m_taskmsg_t _STATIC_MEM_ *flag, s_delay_t tick)
{
	if(*flag) goto __RET_TRUE;
	if(!tick) goto __RET_FALSE;
	
	s_task_current->status = OS_STATUS_BLOCKED;
	s_task_current->blocktype = OS_BLOCKED_TASKMSG | sizeof(*flag);
	s_task_current->timer = tick;
	s_task_current->ptr = flag;
	su_exit_critical_psv();
	s_task_current->blocktype = 0;
	#if SYSCFG_MCUCORE == 8051
	flag = (m_taskmsg_t *)s_task_current->ptr;
	#endif
	if(*flag) goto __RET_TRUE;
	
__RET_FALSE:
	mSysIRQ_Enable;
	return false;
	
__RET_TRUE:
	*flag = false;
	return true;
}

#endif



/*
 * 飞信
 */
#if SYSCFG_FETION == __ENABLED__
/* 接收飞信 */
static m_fetion_t _recv_fetion_(s_fetion_tsp fet)
{
	m_fetion_t fetion = fet->fetion;
	fet->fetion = false;
	mSysIRQ_Enable;
	return fetion;
}

m_fetion_t su_recv_fetion(s_fetion_tsp fet, s_delay_t tick)
{
	if(fet->fetion) goto __RET_TION;
	if(!tick) goto __RET_FALSE;
	
	fet->node = s_task_current;
	s_task_current->status = OS_STATUS_BLOCKED;
	s_task_current->blocktype = OS_BLOCKED_FETION | sizeof(fet->fetion);
	s_task_current->timer = tick;
	s_task_current->ptr = fet;
	su_exit_critical_psv();
	s_task_current->blocktype = 0;
	#if SYSCFG_MCUCORE == 8051
	fet = (s_fetion_tsp)s_task_current->ptr;
	#endif
	fet->node = OS_NULL;
	
__RET_TION:
	return _recv_fetion_(fet);
	
__RET_FALSE:
	mSysIRQ_Enable;
	return false;
}

#endif



/* 退出非嵌套任务临界区并返回(void *)，邮箱、队列专用 */
#if SYSCFG_MAILBOX == __ENABLED__ || SYSCFG_MSGQUEUE == __ENABLED__
static void *_ret_vptr_(void *p)
{
	mSysIRQ_Enable;
	return p;
}
#endif



/*
 * 消息邮箱
 */
#if SYSCFG_MAILBOX == __ENABLED__
/* 接收邮件 */
void *su_recv_mail(s_mailbox_tsp mbox, s_delay_t tick)
{
	if(mbox->flag) goto __RET_MAIL;
	if(!tick) goto __RET_NULL;
	
	mbox->node = s_task_current;
	s_task_current->status = OS_STATUS_BLOCKED;
	s_task_current->blocktype = OS_BLOCKED_MAIL | sizeof(mbox->flag);
	s_task_current->timer = tick;
	s_task_current->ptr = mbox;
	su_exit_critical_psv();
	s_task_current->blocktype = 0;
	#if SYSCFG_MCUCORE == 8051
	mbox = (s_mailbox_tsp)s_task_current->ptr;
	#endif
	mbox->node = OS_NULL;
	if(mbox->flag) goto __RET_MAIL;
	
__RET_NULL:
	mSysIRQ_Enable;
	return OS_NULL;
	
__RET_MAIL:
	mbox->flag = false;
	return _ret_vptr_(mbox->mail);
}

/* 发送邮件 */
void su_send_mail(s_mailbox_tsp mbox, void *mail)
{
	mbox->flag = false;
	mbox->mail = mail;
	mbox->flag = true;
	s_schedule_notnull(mbox->node);
	uExitCritical;
}

#endif



/*
 * 消息队列
 */
#if SYSCFG_MSGQUEUE == __ENABLED__
/* 接收消息 */
void *su_recv_msg(s_queue_tsp queue, s_delay_t tick)
{
	void *msg;
	s_msgnode_tsp node;
	queue->mutex = false;
	
	if(queue->counter) goto __RECV_MSG;
	if(!tick) goto __RET_NULL;
	
	queue->mutex = true;
	queue->node = s_task_current;
	s_task_current->status = OS_STATUS_BLOCKED;
	s_task_current->blocktype = OS_BLOCKED_MSG | sizeof(queue->counter);
	s_task_current->timer = tick;
	s_task_current->ptr = queue;
	su_exit_critical_psv();
	s_task_current->blocktype = 0;
	#if SYSCFG_MCUCORE == 8051
	queue = (s_queue_tsp)s_task_current->ptr;
	#endif
	queue->node = OS_NULL;
	queue->mutex = false;
	if(!queue->counter) goto __RET_NULL;
	
__RECV_MSG:
	if(queue->type == __DYNAMIC__)
		sRecvMsg_Dynamic(msg);
	else
		sRecvMsg_Static(msg);
	queue->mutex = true;
	return _ret_vptr_(msg);
	
__RET_NULL:
	queue->mutex = true;
	mSysIRQ_Enable;
	return OS_NULL;
}

/* 发送消息 */
s_ecode_t su_send_msg(s_queue_tsp queue, void *msg)
{
	queue->mutex = false;
	
	if(queue->counter == queue->len){
		#if SYSCFG_DEBUGGING == __ENABLED__
		s_alarm.overflow_msgqueue = true;
		#endif
		queue->mutex = true;
		uExitCritical;
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
			uExitCritical;
			return OS_ECODE_MALLOCFAIL_MSGNODE;
		}
		sSendMsg_Dynamic(msg);
	}
	else
		sSendMsg_Static(msg);
	
	s_schedule_notnull(queue->node);
	queue->mutex = true;
	uExitCritical;
	return OS_ECODE_NOERROR;
}

#endif



/*
 * 事件标志组
 */
#if SYSCFG_FLAGGROUP == __ENABLED__
/* 查询标志组 */
s_bool_t su_query_group(void _STATIC_MEM_ *p, s_u8_t size)
{
	switch(size){
	case 1: if(*(s_u8_t  *)p) goto __RET_TRUE; break;
	case 2: if(*(s_u16_t *)p) goto __RET_TRUE; break;
	case 4: if(*(s_u32_t *)p) goto __RET_TRUE; break;
	}
	uExitCritical;
	return false;
	
__RET_TRUE:
	uExitCritical;
	return true;
}

/* 等待标志组 */
s_bool_t su_wait_group(void _STATIC_MEM_ *p, s_u8_t size, s_delay_t tick)
{
	switch(size){
	case 1: if(*(s_u8_t  *)p) goto __RET_TRUE; break;
	case 2: if(*(s_u16_t *)p) goto __RET_TRUE; break;
	case 4: if(*(s_u32_t *)p) goto __RET_TRUE; break;
	}
	if(!tick) goto __RET_FALSE;
	
	s_task_current->status = OS_STATUS_BLOCKED;
	s_task_current->blocktype = OS_BLOCKED_FLAGGROUP | size;
	s_task_current->timer = tick;
	s_task_current->ptr = p;
	su_exit_critical_psv();
	#if SYSCFG_MCUCORE == 8051 || SYSCFG_MCUCORE == 80251
	size = s_task_current->blocktype & 0x0F;
	#endif
	s_task_current->blocktype = 0;
	#if SYSCFG_MCUCORE == 8051
	p = s_task_current->ptr;
	#endif
	switch(size){
	case 1: if(*(s_u8_t  *)p) goto __RET_TRUE; break;
	case 2: if(*(s_u16_t *)p) goto __RET_TRUE; break;
	case 4: if(*(s_u32_t *)p) goto __RET_TRUE; break;
	}
__RET_FALSE:
	mSysIRQ_Enable;
	return false;
	
__RET_TRUE:
	mSysIRQ_Enable;
	return true;
}

#endif



/*
 * 动态内存
 */
#if SYSCFG_DYNMEMMGE == __ENABLED__
/* uxalloc */
void _MALLOC_MEM_ *su_xalloc(void _MALLOC_MEM_ *p)
{
	uExitCritical;
	return p;
}

/* 初始化线程内存池 */
s_bool_t su_init_mempool(s_thrmem_tsp p, size_t size)
{
	p->head = p->move = (void _MALLOC_MEM_ *)s_malloc(size);
	if(p->head == OS_NULL){
		uExitCritical;
		return false;
	}
	else{
		p->size = size;
		uExitCritical;
		return true;
	}
}

/* 线程内存分配 */
void _MALLOC_MEM_ *su_talloc(s_thrmem_tsp p, size_t size)
{
	if(p->head == OS_NULL || (size_t)p->move - (size_t)p->head + size > p->size){
		return OS_NULL;
	}
	else{
		void _MALLOC_MEM_ *r = p->move;
		p->move = (s_u8_t _MALLOC_MEM_ *)p->move + size;
		return r;
	}
}

#endif
