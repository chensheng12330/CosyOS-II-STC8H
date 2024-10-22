/**************************************************************************//**
 * @item     CosyOS-II Kernel
 * @file     os_var.c
 * @brief    系统全局变量定义
 * @author   迟凯峰
 * @version  V3.0.0
 * @date     2024.06.25
 ******************************************************************************/

#include "os_redef.h"

/* The Soft-RTC */
#if SYSCFG_SOFTRTC == __ENABLED__
volatile  m_bit_t            /*_SYS_MEM_*/ s_sign_rtc = false;
volatile  s_every_ts           _SYS_MEM_   s_sign_every = {0, 0, 0, 0, 0, 0};
          s_u8_t               _SYS_MEM_   s_rtc[2][7] = {sInitRealTime, sInitRealTime};
          s_u8_t               _SYS_MEM_   s_month2day = 28;
const     s_u8_t             _CONST_MEM_   s_month0day[13] = {31, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
#endif
/* The Task */
          m_bit_t            /*_SYS_MEM_*/ s_sign_return = true;
volatile  m_bit_t            /*_SYS_MEM_*/ s_sign_schedule = false;
volatile  m_bit_t            /*_SYS_MEM_*/ s_sign_schedule_all = false;
          s_u8_t               _SYS_MEM_   s_taskcri_counter = 0;
          s_sharing_t          _SYS_MEM_   s_tick_counter = 0;
          s_taskhand_tsp       _SYS_MEM_   s_task_starter = OS_NULL;
          s_tasknode_tsp       _SYS_MEM_   s_task_current = OS_NULL;
          s_tasknode_tsp       _SYS_MEM_   s_task_switch  = OS_NULL;
          s_tasknode_tsp       _SYS_MEM_   s_taskpri_tail = OS_NULL;
          s_ecode_t            _SYS_MEM_   s_startup_code = OS_ECODE_NOERROR;
          s_u8_t               _SYS_MEM_   s_task_status0 = OS_STATUS_DELETED;
#if SYSCFG_SAFERUNTIME == __ENABLED__
          s_bool_t                         s_sign_timeout[OS_TASKTOTAL + 1];
#endif
#if SYSCFG_TIMESHARINGMODE == 2
const     s_sharing_t        _CONST_MEM_   s_time_sharing[SYSCFG_TASKPRIORITY] = {SYSCFG_CUSTOMTIMESHARING};
#endif
/* The Timing Interrupt-Task/Hook */
#if OS_TIMINTTOTAL
          s_bool_t                         s_timint_hookortask[OS_TIMINTTOTAL];
          s_bool_t                         s_timint_autoreload[OS_TIMINTTOTAL];
          s_timint_t                       s_timint_reload[OS_TIMINTTOTAL];
          s_timint_t                       s_timint_loader[OS_TIMINTTOTAL];
          s_timinthand_tsp                 s_timint_handle[OS_TIMINTTOTAL];
#endif
/* The Timing Query-Task/Hook */
#if OS_TIMQRYTOTAL
          s_bool_t                         s_timqry_hookortask[OS_TIMQRYTOTAL];
          s_bool_t                         s_timqry_autoreload[OS_TIMQRYTOTAL];
          s_timqry_t                       s_timqry_reload[OS_TIMQRYTOTAL];
          s_timqry_t                       s_timqry_loader[OS_TIMQRYTOTAL];
          s_timqryhand_tsp                 s_timqry_handle[OS_TIMQRYTOTAL];
#endif
/* The Global Variable Accessor */
volatile  m_bit_t            /*_SYS_MEM_*/ s_sign_updatecopy = false;
/* The Task Message */
#if SYSCFG_TASKMSG == __ENABLED__
volatile  m_bit_t            /*_SYS_MEM_*/ s_sign_send_taskmsg = false;
#endif
/* The Debug Interface */
#if SYSCFG_DEBUGGING == __ENABLED__
volatile  m_bit_t            /*_SYS_MEM_*/ s_sign_taskmgr = SYSCFG_TASKMGRAUTOSTART;
volatile  m_bit_t            /*_SYS_MEM_*/ s_sign_debug_send = false;
volatile  m_bit_t            /*_SYS_MEM_*/ s_sign_debug_recv = false;
volatile  s_u8_t               _SYS_MEM_   s_debug_sendtype = 0;
          char               _XDATA_MEM_   s_debug_recvbuff[OS_DEBUGRECVBUFFSIZE];
          char               _XDATA_MEM_   s_cmdline_sendbuff[OS_CMDLINESENDBUFFSIZE];
          char               _XDATA_MEM_   s_taskmgr_sendbuff[OS_TASKMGRSENDBUFFSIZE];
          char _XDATA_MEM_ *   _SYS_MEM_   s_debug_recvptr = s_debug_recvbuff;
          char _XDATA_MEM_ *   _SYS_MEM_   s_debug_sendptr;
          s_alarm_ts                       s_alarm = {0, 0, 0, 0};
          s_fault_ts                       s_fault = {0, 0, 0, 0, 0, 0, 0, 0};
#if SYSCFG_SYSTICKTIME_COUNT == __ENABLED__
          s_u32_t                          s_tick_counter1 = 0;
          s_u32_t                          s_tick_counter2 = 0;
#endif
#if SYSCFG_TASKPC_MONITOR == __ENABLED__
          m_pc_t               _SYS_MEM_   s_pc = 0;
#endif
#endif
