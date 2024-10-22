/**************************************************************************//**
 * @item     CosyOS-II Kernel
 * @file     os_api.c
 * @brief    系统内核专用API
 * @author   迟凯峰
 * @version  V3.0.0
 * @date     2024.06.25
 ******************************************************************************/

#include "os_var.h"

#if SYSCFG_MCUCORE == 8051
#pragma NOAREGS
#endif

void s_schedule_taskpri(s_tasknode_tsp node)
{
	if(node->pri > s_task_current->pri){
		if(s_task_switch == OS_NULL || node->pri > s_task_switch->pri){
			s_task_switch = node;
		}
		s_sign_schedule = true;
	}
}

void s_schedule_taskmsg(s_tasknode_tsp node)
{
	if(node->blocktype & OS_BLOCKED_TASKMSG){
		s_schedule_taskpri(node);
	}
}

void s_schedule_notnull(s_tasknode_tsp node)
{
	if(node != OS_NULL){
		s_schedule_taskpri(node);
	}
}

void s_schedule_ready(s_tasknode_tsp node)
{
	if(node->status == OS_STATUS_READY){
		s_schedule_taskpri(node);
	}
}

void s_set_taskpri(s_tasknode_tsp node)
{
	node->last->next = node->next;
	node->next->last = node->last;
	node->next = node->last = OS_NULL;
	if(s_taskpri_tail != OS_NULL){
		s_taskpri_tail->next = node;
		node->last = s_taskpri_tail;
	}
	s_taskpri_tail = node;
	s_sign_schedule_all = true;
}
