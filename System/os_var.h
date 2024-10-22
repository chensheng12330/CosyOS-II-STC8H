/**************************************************************************//**
 * @item     CosyOS-II Kernel
 * @file     os_var.h
 * @brief    系统全局变量声明
 * @author   迟凯峰
 * @version  V3.0.0
 * @date     2024.06.25
 ******************************************************************************/

#ifndef __OS_VAR_H
#define __OS_VAR_H
#include "os_redef.h"

/* The Soft-RTC */
#if SYSCFG_SOFTRTC == __ENABLED__
extern  volatile  m_bit_t            /*_SYS_MEM_*/ s_sign_rtc;
extern  volatile  s_every_ts           _SYS_MEM_   s_sign_every;
extern            s_u8_t               _SYS_MEM_   s_rtc[2][7];
extern            s_u8_t               _SYS_MEM_   s_month2day;
extern  const     s_u8_t             _CONST_MEM_   s_month0day[13];
#endif
/* The Task */
extern            m_bit_t            /*_SYS_MEM_*/ s_sign_return;
extern  volatile  m_bit_t            /*_SYS_MEM_*/ s_sign_schedule;
extern  volatile  m_bit_t            /*_SYS_MEM_*/ s_sign_schedule_all;
extern            s_u8_t               _SYS_MEM_   s_taskcri_counter;
extern            s_sharing_t          _SYS_MEM_   s_tick_counter;
extern            s_taskhand_tsp       _SYS_MEM_   s_task_starter;
extern            s_tasknode_tsp       _SYS_MEM_   s_task_current;
extern            s_tasknode_tsp       _SYS_MEM_   s_task_switch;
extern            s_tasknode_tsp       _SYS_MEM_   s_taskpri_tail;
extern            s_ecode_t            _SYS_MEM_   s_startup_code;
extern            s_u8_t               _SYS_MEM_   s_task_status0;
#if SYSCFG_SAFERUNTIME == __ENABLED__
extern            s_bool_t                         s_sign_timeout[OS_TASKTOTAL + 1];
#endif
#if SYSCFG_TIMESHARINGMODE == 2
extern  const     s_sharing_t        _CONST_MEM_   s_time_sharing[SYSCFG_TASKPRIORITY];
#endif
/* The Timing Interrupt-Task/Hook */
#if OS_TIMINTTOTAL
extern            s_bool_t                         s_timint_hookortask[OS_TIMINTTOTAL];
extern            s_bool_t                         s_timint_autoreload[OS_TIMINTTOTAL];
extern            s_timint_t                       s_timint_reload[OS_TIMINTTOTAL];
extern            s_timint_t                       s_timint_loader[OS_TIMINTTOTAL];
extern            s_timinthand_tsp                 s_timint_handle[OS_TIMINTTOTAL];
#endif
/* The Timing Query-Task/Hook */
#if OS_TIMQRYTOTAL
extern            s_bool_t                         s_timqry_hookortask[OS_TIMQRYTOTAL];
extern            s_bool_t                         s_timqry_autoreload[OS_TIMQRYTOTAL];
extern            s_timqry_t                       s_timqry_reload[OS_TIMQRYTOTAL];
extern            s_timqry_t                       s_timqry_loader[OS_TIMQRYTOTAL];
extern            s_timqryhand_tsp                 s_timqry_handle[OS_TIMQRYTOTAL];
#endif
/* The Global Variable Accessor */
extern  volatile  m_bit_t            /*_SYS_MEM_*/ s_sign_updatecopy;
/* The Task Message */
#if SYSCFG_TASKMSG == __ENABLED__
extern  volatile  m_bit_t            /*_SYS_MEM_*/ s_sign_send_taskmsg;
#endif
/* The Debug Interface */
#if SYSCFG_DEBUGGING == __ENABLED__
extern  volatile  m_bit_t            /*_SYS_MEM_*/ s_sign_taskmgr;
extern  volatile  m_bit_t            /*_SYS_MEM_*/ s_sign_debug_send;
extern  volatile  m_bit_t            /*_SYS_MEM_*/ s_sign_debug_recv;
extern  volatile  s_u8_t               _SYS_MEM_   s_debug_sendtype;
extern            char               _XDATA_MEM_   s_debug_recvbuff[OS_DEBUGRECVBUFFSIZE];
extern            char               _XDATA_MEM_   s_cmdline_sendbuff[OS_CMDLINESENDBUFFSIZE];
extern            char               _XDATA_MEM_   s_taskmgr_sendbuff[OS_TASKMGRSENDBUFFSIZE];
extern            char _XDATA_MEM_ *   _SYS_MEM_   s_debug_recvptr;
extern            char _XDATA_MEM_ *   _SYS_MEM_   s_debug_sendptr;
extern            s_alarm_ts                       s_alarm;
extern            s_fault_ts                       s_fault;
#if SYSCFG_SYSTICKTIME_COUNT == __ENABLED__
extern            s_u32_t                          s_tick_counter1;
extern            s_u32_t                          s_tick_counter2;
#endif
#if SYSCFG_TASKPC_MONITOR == __ENABLED__
extern            m_pc_t               _SYS_MEM_   s_pc;
#endif
#endif

extern void (_CODE_MEM_ * const _CONST_MEM_ sPendSV_FIFOHandler[OS_SVID_END])(void _STATIC_MEM_ *sv);

#endif
