/**************************************************************************//**
 * @item     CosyOS-II Kernel
 * @file     sv_int_pend_flag.h
 * @brief    中断挂起服务 with FLAG（仅在PendSV中调用并执行）
 * @author   迟凯峰
 * @version  V3.0.0
 * @date     2024.06.25
 ******************************************************************************/

#ifndef __SV_INT_PEND_FLAG_H
#define __SV_INT_PEND_FLAG_H

void sp_resume_task (s_tasknode_tsp node);
void sp_suspend_task(s_tasknode_tsp node);
void sp_delete_task (s_tasknode_tsp node);
void sp_set_taskpri (s_tasknode_tsp node, s_u8_t pri);
void sp_set_block   (s_tasknode_tsp node, s_delay_t tick);
void sp_clear_block (s_tasknode_tsp node);
void sp_write_binary(s_binary_tsp bin, s_bool_t value);



/*
 * 任务
 */

/* 恢复任务 */
#define	sPS2_ResumeTask(load) \
	sp_resume_task(load)

/* 挂起任务 */
#define	sPS2_SuspendTask(load) \
	sp_suspend_task(load)

/* 删除任务 */
#define	sPS2_DeleteTask(load) \
	sp_delete_task(load)

/* 设置任务优先级 */
#define	sPS2_SetTaskPri(load, pri) \
	sp_set_taskpri(load, pri)

/* 设置阻塞（时间）*/
#define	sPS2_SetBlock(load, tc) \
	sp_set_block(load, tc)

/* 清除阻塞（状态）*/
#define	sPS2_ClearBlock(load) \
	sp_clear_block(load)



/*
 * 二值信号量
 */

/* 上锁/给予 */
#define sPS2_WriteBin(bin, val) \
	sp_write_binary(&bin, val)



#endif
