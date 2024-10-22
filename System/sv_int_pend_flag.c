/**************************************************************************//**
 * @item     CosyOS-II Kernel
 * @file     sv_int_pend_flag.c
 * @brief    中断挂起服务 with FLAG（仅在PendSV中调用并执行）
 * @author   迟凯峰
 * @version  V3.0.0
 * @date     2024.06.25
 ******************************************************************************/

#include "os_link.h"

/* 恢复任务 */
void sp_resume_task(s_tasknode_tsp node) MCUCFG_C51USING
{
	if(node == OS_NULL) return;
	if(node->status & OS_STATUS_SUSPENDED){
		node->status &= (~OS_STATUS_SUSPENDED & 0xFF);
		s_schedule_ready(node);
	}
}

/* 挂起任务 */
void sp_suspend_task(s_tasknode_tsp node) MCUCFG_C51USING
{
	if(node == OS_NULL) return;
	if(node->status < OS_STATUS_SUSPENDED){
		node->status |= OS_STATUS_SUSPENDED;
		if(node == s_task_current){
			s_sign_schedule = true;
		}
	}
}

/* 删除任务 */
void sp_delete_task(s_tasknode_tsp node) MCUCFG_C51USING
{
	if(node == OS_NULL) return;
	if(node->status < OS_STATUS_DELETED){
		node->status = OS_STATUS_DELETED;
		if(node == s_task_current){
			s_sign_schedule = true;
		}
	}
}

/* 设置任务优先级 */
void sp_set_taskpri(s_tasknode_tsp node, s_u8_t pri) MCUCFG_C51USING
{
	if(node == OS_NULL) return;
	if(node->status < OS_STATUS_STOPPED){
		if(node->pri != pri){
			node->pri = pri;
			s_set_taskpri(node);
			s_sign_schedule = true;
		}
	}
}

/* 设置阻塞（时间）*/
void sp_set_block(s_tasknode_tsp node, s_delay_t tick) MCUCFG_C51USING
{
	if(node == OS_NULL) return;
	if(node->status == OS_STATUS_BLOCKED || node->status == OS_STATUS_FLOATING){
		if(!tick){
			node->status = OS_STATUS_READY;
			s_schedule_taskpri(node);
		}
		node->timer = tick;
	}
}

/* 清除阻塞（状态）*/
void sp_clear_block(s_tasknode_tsp node) MCUCFG_C51USING
{
	if(node == OS_NULL) return;
	if(node->status == OS_STATUS_BLOCKED || node->status == OS_STATUS_FLOATING){
		node->status = OS_STATUS_READY;
		node->timer = 0;
		s_schedule_taskpri(node);
	}
}

/* 上锁/给予二值信号量 */
#if SYSCFG_BINARY == __ENABLED__
void sp_write_binary(s_binary_tsp bin, s_bool_t value) MCUCFG_C51USING
{
	bin->binary = value;
	if(!value) return;
	s_schedule_notnull(bin->node);
}
#endif
