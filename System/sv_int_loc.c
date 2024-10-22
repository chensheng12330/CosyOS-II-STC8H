/**************************************************************************//**
 * @item     CosyOS-II Kernel
 * @file     sv_int_loc.c
 * @brief    中断本地服务（仅在用户中断中调用，并在本地执行）
 * @author   迟凯峰
 * @version  V3.0.0
 * @date     2024.06.25
 ******************************************************************************/

#include "os_var.h"
#include "os_api.h"

#if SYSCFG_MCUCORE == 8051
#pragma NOAREGS
#endif



/*
 * 任务调度
 */

void si_task_schedule(void)
{
	s_sign_schedule_all = true;
	s_sign_schedule = true;
	mPendSV_Set;
}



/*
 * 二值信号量获取
 */
#if SYSCFG_BINARY == __ENABLED__
s_bool_t si_take_binary(s_binary_tsp bin)
{
	if(bin->binary){
		bin->binary = false;
		return true;
	}
	return false;
}
#endif



/*
 * 接收飞信 
 */
#if SYSCFG_FETION == __ENABLED__
m_fetion_t si_recv_fetion(s_fetion_tsp fet)
{
	m_fetion_t fetion = fet->fetion;
	fet->fetion = false;
	return fetion;
}
#endif



/*
 * 接收邮件
 */
#if SYSCFG_MAILBOX == __ENABLED__
void *si_recv_mail(s_mailbox_tsp mbox)
{
	if(mbox->flag){
		mbox->flag = false;
		return mbox->mail;
	}
	return OS_NULL;
}
#endif



/*
 * 接收消息
 */
#if SYSCFG_MSGQUEUE == __ENABLED__

#if SYSCFG_MCUCORE == 8051

s_bool_t si_can_access_queue(s_queue_tsp queue)
{
	if(!queue->mutex) return false;
	queue->mutex = false;
	
	if(queue->type == __DYNAMIC__){
		#if SYSCFG_DEBUGGING == __ENABLED__
		s_fault.error_recvmsg_int = true;
		#endif
	}
	else if(!queue->counter);
	else return true;
	
	queue->mutex = true;
	return false;
}

static void *_recv_msg_(s_bool_t _OBJ_MEM_ *mut, void *msg)
{
	*mut = true;
	return msg;
}

void *si_recv_msg(s_queue_tsp queue)
{
	void *msg;
	sRecvMsg_Static(msg);
	return _recv_msg_((s_bool_t *)&queue->mutex, msg);
}

#else

void *si_recv_msg(s_queue_tsp queue)
{
	void *msg = OS_NULL;
	if(!queue->mutex) return OS_NULL;
	queue->mutex = false;
	
	if(queue->type == __DYNAMIC__){
		#if SYSCFG_DEBUGGING == __ENABLED__
		s_fault.error_recvmsg_int = true;
		#endif
	}
	else if(!queue->counter);
	else sRecvMsg_Static(msg);
	
	queue->mutex = true;
	return msg;
}

#endif

#endif
