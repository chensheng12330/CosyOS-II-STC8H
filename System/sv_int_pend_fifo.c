/**************************************************************************//**
 * @item     CosyOS-II Kernel
 * @file     sv_int_pend_fifo.c
 * @brief    中断挂起服务 with FIFO - 执行函数（仅在PendSV中由系统调用并执行）
 * @author   迟凯峰
 * @version  V3.0.0
 * @date     2024.06.25
 ******************************************************************************/

#include "os_link.h"

/* 恢复任务 */
static void _resume_task_(void _STATIC_MEM_ *sv) MCUCFG_C51USING
{
	s_tasknode_tsp node = ((sp_task_tsp)sv)->node;
	if(node == OS_NULL) return;
	if(node->status & OS_STATUS_SUSPENDED){
		node->status &= (~OS_STATUS_SUSPENDED & 0xFF);
		s_schedule_ready(node);
	}
}

/* 挂起任务 */
static void _suspend_task_(void _STATIC_MEM_ *sv) MCUCFG_C51USING
{
	s_tasknode_tsp node = ((sp_task_tsp)sv)->node;
	if(node == OS_NULL) return;
	if(node->status < OS_STATUS_SUSPENDED){
		node->status |= OS_STATUS_SUSPENDED;
		if(node == s_task_current){
			s_sign_schedule = true;
		}
	}
}

/* 删除任务 */
static void _delete_task_(void _STATIC_MEM_ *sv) MCUCFG_C51USING
{
	s_tasknode_tsp node = ((sp_task_tsp)sv)->node;
	if(node == OS_NULL) return;
	if(node->status < OS_STATUS_DELETED){
		node->status = OS_STATUS_DELETED;
		if(node == s_task_current){
			s_sign_schedule = true;
		}
	}
}

/* 设置任务优先级 */
static void _set_taskpri_(void _STATIC_MEM_ *sv) MCUCFG_C51USING
{
	s_tasknode_tsp node = ((sp_taskpri_tsp)sv)->node;
	if(node == OS_NULL) return;
	if(node->status < OS_STATUS_STOPPED){
		s_u8_t pri = ((sp_taskpri_tsp)sv)->pri;
		if(node->pri != pri){
			node->pri = pri;
			s_set_taskpri(node);
			s_sign_schedule = true;
		}
	}
}

/* 设置阻塞（时间）*/
static void _set_block_(void _STATIC_MEM_ *sv) MCUCFG_C51USING
{
	s_tasknode_tsp node = ((sp_blocktime_tsp)sv)->node;
	if(node == OS_NULL) return;
	if(node->status == OS_STATUS_BLOCKED || node->status == OS_STATUS_FLOATING){
		if(!((sp_blocktime_tsp)sv)->tick){
			node->status = OS_STATUS_READY;
			s_schedule_taskpri(node);
		}
		node->timer = ((sp_blocktime_tsp)sv)->tick;
	}
}

/* 清除阻塞（状态）*/
static void _clear_block_(void _STATIC_MEM_ *sv) MCUCFG_C51USING
{
	s_tasknode_tsp node = ((sp_task_tsp)sv)->node;
	if(node == OS_NULL) return;
	if(node->status == OS_STATUS_BLOCKED || node->status == OS_STATUS_FLOATING){
		node->status = OS_STATUS_READY;
		node->timer = 0;
		s_schedule_taskpri(node);
	}
}

/* 定时中断 */
#if OS_TIMINTTOTAL
static void _timint_(void _STATIC_MEM_ *sv) MCUCFG_C51USING
{
	  s_timint_loader[((sp_timint_tsp)sv)->tmid]
	= s_timint_reload[((sp_timint_tsp)sv)->tmid]
	= ((sp_timint_tsp)sv)->tick;
}
#else
#define _timint_ (s_voidvptr_tfp)0
#endif

/* 定时查询 */
#if OS_TIMQRYTOTAL
static void _timqry_(void _STATIC_MEM_ *sv) MCUCFG_C51USING
{
	  s_timqry_loader[((sp_timqry_tsp)sv)->tmid]
	= s_timqry_reload[((sp_timqry_tsp)sv)->tmid]
	= ((sp_timqry_tsp)sv)->tick;
}
#else
#define _timqry_ (s_voidvptr_tfp)0
#endif

/* 上锁/给予二值信号量 */
#if SYSCFG_BINARY == __ENABLED__
static void _binary_(void _STATIC_MEM_ *sv) MCUCFG_C51USING
{
	s_binary_tsp bin = ((sp_binary_tsp)sv)->bin;
	bin->binary = ((sp_binary_tsp)sv)->value;
	if(!bin->binary) return;
	s_schedule_notnull(bin->node);
}
#else
#define _binary_ (s_voidvptr_tfp)0
#endif

/* 给予计数信号量 */
#if SYSCFG_SEMAPHORE == __ENABLED__
static void _give_sem_(void _STATIC_MEM_ *sv) MCUCFG_C51USING
{
	s_semaph_tsp sem = ((sp_semaph_tsp)sv)->sem;
	if(sem->counter < sem->maximum){
		sem->counter++;
	}
	s_schedule_notnull(sem->node);
}
#else
#define _give_sem_ (s_voidvptr_tfp)0
#endif

/* 发送飞信 */
#if SYSCFG_FETION == __ENABLED__
static void _send_fetion_(void _STATIC_MEM_ *sv) MCUCFG_C51USING
{
	s_fetion_tsp tbox = ((sp_fetion_tsp)sv)->tbox;
	tbox->fetion = ((sp_fetion_tsp)sv)->fetion;
	s_schedule_notnull(tbox->node);
}
#else
#define _send_fetion_ (s_voidvptr_tfp)0
#endif

/* 发送邮件 */
#if SYSCFG_MAILBOX == __ENABLED__
static void _send_mail_(void _STATIC_MEM_ *sv) MCUCFG_C51USING
{
	s_mailbox_tsp mbox = ((sp_mailbox_tsp)sv)->mbox;
	mbox->flag = false;
	mbox->mail = ((sp_mailbox_tsp)sv)->mail;
	mbox->flag = true;
	s_schedule_notnull(mbox->node);
}
#else
#define _send_mail_ (s_voidvptr_tfp)0
#endif

/* 发送消息 */
#if SYSCFG_MSGQUEUE == __ENABLED__
static void _send_msg_(void _STATIC_MEM_ *sv) MCUCFG_C51USING
{
	s_queue_tsp queue = ((sp_msg_tsp)sv)->queue;
	queue->mutex = false;
	
	if(queue->counter == queue->len){
		#if SYSCFG_DEBUGGING == __ENABLED__
		s_alarm.overflow_msgqueue = true;
		#endif
		queue->mutex = true;
		return;
	}
	if(queue->type == __DYNAMIC__){
		s_msgnode_tsp node = OS_NULL;
		node = (s_msgnode_tsp)s_malloc(sizeof(s_msgnode_ts));
		if(node == OS_NULL){
			#if SYSCFG_DEBUGGING == __ENABLED__
			s_fault.mallocfailed_msgnode = true;
			#endif
			queue->mutex = true;
			return;
		}
		sSendMsg_Dynamic(((sp_msg_tsp)sv)->msg);
	}
	else
		sSendMsg_Static(((sp_msg_tsp)sv)->msg);
	
	s_schedule_notnull(queue->node);
	queue->mutex = true;
}
#else
#define _send_msg_ (s_voidvptr_tfp)0
#endif

/* 写标志组 */
#if SYSCFG_FLAGGROUP == __ENABLED__
static void _group_(void _STATIC_MEM_ *sv) MCUCFG_C51USING
{
	switch(((sp_group_tsp)sv)->size){
	case +1: *(s_u8_t  *)((sp_group_tsp)sv)->group |= ((sp_group_tsp)sv)->value; break;
	case +2: *(s_u16_t *)((sp_group_tsp)sv)->group |= ((sp_group_tsp)sv)->value; break;
	case +4: *(s_u32_t *)((sp_group_tsp)sv)->group |= ((sp_group_tsp)sv)->value; break;
	case -1: *(s_u8_t  *)((sp_group_tsp)sv)->group &=~((sp_group_tsp)sv)->value; break;
	case -2: *(s_u16_t *)((sp_group_tsp)sv)->group &=~((sp_group_tsp)sv)->value; break;
	case -4: *(s_u32_t *)((sp_group_tsp)sv)->group &=~((sp_group_tsp)sv)->value; break;
	}
}
#else
#define _group_ (s_voidvptr_tfp)0
#endif

/* 全局变量写访问 */
static void _gvar_write_(void _STATIC_MEM_ *sv) MCUCFG_C51USING
{
	((sp_gvar_tsp)sv)->size
	? s_memcpy(((sp_gvar_tsp)sv)->gp, ((sp_gvar_tsp)sv)->lp, ((sp_gvar_tsp)sv)->size)
	: s_strcpy((char *)((sp_gvar_tsp)sv)->gp, (char *)((sp_gvar_tsp)sv)->lp);
}

/* 挂起服务调用 */
static void _pendsvc_(void _STATIC_MEM_ *sv) MCUCFG_C51USING
{
	(*((sp_pendsvc_tsp)sv)->fp)();
}

/* 发送信号 */
static void _send_sign_(void _STATIC_MEM_ *sv) MCUCFG_C51USING
{
	*((sp_sign_tsp)sv)->sign = true;
}

typedef void (_CODE_MEM_ *s_voidvptr_tfp)(void _STATIC_MEM_ *sv);
void (_CODE_MEM_ * const _CONST_MEM_ sPendSV_FIFOHandler[OS_SVID_END])(void _STATIC_MEM_ *sv) =
{
	_resume_task_,
	_suspend_task_,
	_delete_task_,
	_set_taskpri_,
	_set_block_,
	_clear_block_,
	_timint_,
	_timqry_,
	_binary_,
	_give_sem_,
	_send_fetion_,
	_send_mail_,
	_send_msg_,
	_group_,
	_gvar_write_,
	_pendsvc_,
	_send_sign_
};
