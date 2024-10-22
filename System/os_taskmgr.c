/**************************************************************************//**
 * @item     CosyOS-II Kernel
 * @file     os_taskmgr.c
 * @brief    任务管理器（Taskmgr）
 * @author   迟凯峰
 * @version  V3.0.0
 * @date     2024.06.25
 ******************************************************************************/

#include "os_link.h"
#if SYSCFG_DEBUGGING == __ENABLED__

#define _STR_MEM_   _OBJ_MEM_
static char _STR_MEM_ str[8];
static char _XDATA_MEM_ * _SYS_MEM_ mptr;

#define _chrcpy_(c) *mptr++ = c

static void _spccpy_(s_u8_t n)
{
	do{
		*mptr++ = ' ';
	}while(--n);
}

static void _strcpy_(const char *sub)
{
	while(*sub){
		*mptr++ = *sub++;
	}
}

static s_u8_t _strlen_(const char *src)
{
	s_u8_t i = 0;
	while(*src++){
		i++;
	}
	return i;
}

static void _u8str_(const s_u8_t var)
{
	char _STR_MEM_ *ptr = str;
	if(var >= 100){
		*ptr++ = var / 100 + '0';
		goto Lable_2;
	}
	if(var >= 10){
		*ptr++ = var / 10 + '0';
		goto Lable_1;
	}
	else{
		*ptr++ = var /**/+ '0';
		goto Lable_0;
	}
	Lable_2: *ptr++ = (var % 100) / 10 + '0';
	Lable_1: *ptr++ = (var % 10) /**/+ '0';
	Lable_0: *ptr = '\0';
}

static void _u16str_(const s_u16_t var)
{
	char _STR_MEM_ *ptr = str;
	if(var >= 10000){
		*ptr++ = var / 10000 + '0';
		goto Lable_4;
	}
	if(var >= 1000){
		*ptr++ = var / 1000 + '0';
		goto Lable_3;
	}
	if(var >= 100){
		*ptr++ = var / 100 + '0';
		goto Lable_2;
	}
	if(var >= 10){
		*ptr++ = var / 10 + '0';
		goto Lable_1;
	}
	else{
		*ptr++ = var /**/+ '0';
		goto Lable_0;
	}
	Lable_4: *ptr++ = (var % 10000) / 1000 + '0';
	Lable_3: *ptr++ = (var % 1000) / 100 + '0';
	Lable_2: *ptr++ = (var % 100) / 10 + '0';
	Lable_1: *ptr++ = (var % 10) /**/+ '0';
	Lable_0: *ptr = '\0';
}

static s_u8_t _u16len_(const s_u16_t var)
{
	if(var >= 10000) return 5;
	if(var >= 1000) return 4;
	if(var >= 100) return 3;
	if(var >= 10) return 2;
	return 1;
}

static void _used_ratio_(const s_u16_t permill)
{
	s_u8_t i;
	if(!permill){
		str[0] = '0';
		str[1] = '\0';
		i = 2;
	}
	else{
		_u16str_(permill);
		i = _strlen_(str);
		if(i > 2){
			str[i-0] = str[i-1];
			str[i-1] = str[i-2];
			str[i-2] = '.';
			str[i+1] = '\0';
			i += 2;
		}
		else{
			if(i == 1){
				str[2] = '0';
				str[3] = str[0];
			}
			else{
				str[2] = str[0];
				str[3] = str[1];
			}
			str[0] = '0';
			str[1] = '.';
			str[4] = '\0';
			i = 5;
		}
	}
	_spccpy_((s_u8_t)(OS_TASKMGR_LEN_CPU - i));
	_strcpy_(str);
	_chrcpy_('%');
}

#if SYSCFG_TASKPC_MONITOR == __ENABLED__
static void _u8str16_(const s_u8_t var)
{
	str[0] = (var >> 4) + '0';
	str[1] = (var & 0x0F) + '0';
	if(str[0] > '9') str[0] += 0x07;
	if(str[1] > '9') str[1] += 0x07;
	*mptr++ = str[0];
	*mptr++ = str[1];
}
#endif

uCreateTask_TimQry(OS_TMID_TASKMGR, s_sign_taskmgr, true, Taskmgr, SYSCFG_TASKPRIORITY - 1, MCUCFG_STACKSIZE_TASKMGR, 0, 0)
{
	static char _XDATA_MEM_ *p[2];
	mptr = s_taskmgr_sendbuff;
	/* Title1 */
	_strcpy_("\x43\x6F\x73\x79\x4F\x53\x2D\x49\x49\x20\xC8\xCE\xCE\xF1\xB9\xDC\xC0\xED\xC6\xF7");
	_spccpy_(
		  SYSCFG_TASKNAMELEN_MAX
		+ OS_TASKMGR_LEN_TID + OS_TASKMGR_LEN_PRI
		+ OS_TASKMGR_LEN_STA
		- 20 /* Title1 Len */
	);
	p[0] = mptr;
	_spccpy_(OS_TASKMGR_LEN_CPU + OS_TASKMGR_LEN_RAM);
	_strcpy_("\r\n");
	/* Title2 */
	_strcpy_("\xC3\xFB\xB3\xC6");
	_spccpy_(SYSCFG_TASKNAMELEN_MAX - 4);
	_spccpy_(OS_TASKMGR_LEN_TID - 3);
	_strcpy_("TID");
	_spccpy_(OS_TASKMGR_LEN_PRI - 3);
	_strcpy_("PRI");
	_spccpy_(OS_TASKMGR_LEN_STA - 4);
	_strcpy_("\xD7\xB4\xCC\xAC");
	_spccpy_(OS_TASKMGR_LEN_CPU - 3);
	_strcpy_("CPU");
	_spccpy_(OS_TASKMGR_LEN_RAM - 4);
	_strcpy_("\xC4\xDA\xB4\xE6");
	_strcpy_("\r\n");
	/* NULL Line */
	_strcpy_("\r\n");
	/* - */
	p[1] = mptr;
	while(true){
		static s_timqry_t upspeed = (1000UL * SYSCFG_TASKMGRUPSPEED) / SYSCFG_SYSTICKCYCLE;
		static s_tasknode_ts node_temp;
		s_tasknode_tsp    node_curr = u_taskhandle_Taskmgr;
		s_u16_t           permill1 = 10000;
		#if SYSCFG_TASKPC_MONITOR == __ENABLED__
		m_pc_t            pc = s_pc;
		#endif
		mptr = p[1];
		while(true){
			s_u16_t permill2;
			mSysIRQ_Disable;
			s_memcpy(&node_temp, node_curr, sizeof(s_tasknode_ts));
			node_curr->usedtime[0] = 0;
			node_curr->usedtime[1] = 0;
			mSysIRQ_Enable;
			/* Name */
			_strcpy_(node_temp.name);
			if(SYSCFG_TASKNAMELEN_MAX > _strlen_(node_temp.name)){
				_spccpy_((s_u8_t)(SYSCFG_TASKNAMELEN_MAX - _strlen_(node_temp.name)));
			}
			/* TID */
			_u8str_	(node_temp.tid);
			_spccpy_((s_u8_t)(OS_TASKMGR_LEN_TID - _strlen_(str)));
			_strcpy_(str);
			/* PRI */
			_u8str_	(node_temp.pri);
			_spccpy_((s_u8_t)(OS_TASKMGR_LEN_PRI - _strlen_(str)));
			_strcpy_(str);
			/* STA */
			_spccpy_(OS_TASKMGR_LEN_STA - 3);
			if(node_temp.status <= OS_STATUS_FLOATING){
				_strcpy_("RDY");
			}
			else if(node_temp.status == OS_STATUS_BLOCKED){
				node_temp.blocktype &= 0xF0;
				switch(node_temp.blocktype){
				case OS_BLOCKED_DELAY:     _strcpy_("DLY"); break;
				case OS_BLOCKED_BINARY:    _strcpy_("BIN"); break;
				case OS_BLOCKED_MUTEX:     _strcpy_("MUT"); break;
				case OS_BLOCKED_SEMAPHORE: _strcpy_("SEM"); break;
				case OS_BLOCKED_FLAGGROUP: _strcpy_("GRP"); break;
				case OS_BLOCKED_TASKMSG:   _strcpy_(" TM"); break;
				case OS_BLOCKED_FETION:    _strcpy_("FET"); break;
				case OS_BLOCKED_MAIL:      _strcpy_("MAL"); break;
				case OS_BLOCKED_MSG:       _strcpy_("MSG"); break;
				}
			}
			else if(node_temp.status == OS_STATUS_OVERTIME){
				_strcpy_("OSR");
			}
			else if(node_temp.status & OS_STATUS_SUSPENDED){
				_strcpy_("SPD");
			}
			else if(node_temp.status == OS_STATUS_STOPPED_TSOF){
				_strcpy_("!OF");
			}
			else if(node_temp.status == OS_STATUS_STOPPED_TSRF){
				_strcpy_("!RF");
			}
			else{
				_strcpy_("!XX");
			}
			/* CPU */
			permill2 = (
				(
				    (node_temp.usedtime[0] * 10000UL)
				  + (node_temp.usedtime[1] * 10000UL) / mSysTick_Cycle
				) / upspeed
			);
			_used_ratio_(permill2);
			if(!node_temp.pri){
				permill1 -= permill2;
			}
			/* RAM */
			_spccpy_(
				(s_u8_t)(
				    OS_TASKMGR_LEN_RAM - 4
				  - _u16len_((s_u16_t)node_temp.stacklen_max)
				  - _u16len_((s_u16_t)node_temp.stacksize)
				)
			);
			_u16str_(node_temp.stacklen_max);
			_strcpy_(str);
			_strcpy_("B/");
			switch(node_temp.create){
			case 0: _chrcpy_('s'); break;
			case 1: _chrcpy_('m'); break;
			case 2: _chrcpy_('r'); break;
			}
			_u16str_(node_temp.stacksize);
			_strcpy_(str);
			_chrcpy_('B');
			_strcpy_("\r\n");
			/* - */
			if(node_temp.next == u_taskhandle_Taskmgr){
				break;
			}
			node_curr = node_temp.next;
		}
		/* CPU-TOTAL */
		if(true){
			char _XDATA_MEM_ *p0 = mptr;
			mptr = p[0];
			_used_ratio_(permill1);
			mptr = p0;
		}
		/* NULL Line */
		_strcpy_("\r\n");
		/* PSVFIFO */
		#if SYSCFG_PENDSVFIFO_MONITOR == __ENABLED__
		if(mPendSV_FIFO_DepthMAX > MCUCFG_PENDSVFIFO_DEPTH){
			s_fault.overflow_pendsvfifo = true;
		}
		_strcpy_("PSVFIFO: ");
		_u16str_(mPendSV_FIFO_DepthMAX);
		_strcpy_(str);
		_strcpy_("/");
		_u16str_(MCUCFG_PENDSVFIFO_DEPTH);
		_strcpy_(str);
		_strcpy_(".\r\n");
		#endif
		/* Task-PC */
		#if SYSCFG_TASKPC_MONITOR == __ENABLED__
		_strcpy_("Task-PC: ");
		#if MCUCFG_PCLEN > 2
		_u8str16_((s_u8_t)(pc >> 24));
		_u8str16_((s_u8_t)(pc >> 16));
		#endif
		_u8str16_((s_u8_t)(pc >> 8));
		_u8str16_((s_u8_t)(pc));
		_strcpy_(".\r\n");
		#endif
		/* SysTick */
		#if SYSCFG_SYSTICKTIME_COUNT == __ENABLED__
		mSysIRQ_Disable;
		if(s_tick_counter2){
			s_u32_t counter1 = s_tick_counter1;
			s_u32_t counter2 = s_tick_counter2;
			s_tick_counter1 = 0;
			s_tick_counter2 = 0;
			mSysIRQ_Enable;
			counter2 = (counter1 * 100 * mSysTick_CLKMOD) / (SYSCFG_SYSCLK / 1000000UL) / counter2;
			_strcpy_("SysTick: ");
			_u16str_((s_u16_t)(counter2 / 100));
			_strcpy_(str);
			_chrcpy_('.');
			_u8str_((s_u8_t)((counter2 % 100) / 10));
			_strcpy_(str);
			_u8str_((s_u8_t)(counter2 % 10));
			_strcpy_(str);
			_strcpy_("us.\r\n");
		}
		else{
			mSysIRQ_Enable;
		}
		#endif
		/* Alarm */
		if(*(s_u8_t *)&s_alarm){
			_strcpy_("Alarm: ");
			if(s_alarm.overflow_msgqueue){
				_strcpy_("omq, ");
			}
			if(s_alarm.overflow_taskqueue){
				_strcpy_("otq, ");
			}
			if(s_alarm.timedout_saferuntime){
				_strcpy_("osr, ");
			}
			if(s_alarm.outrange_taskpriority){
				_strcpy_("otp, ");
			}
			if(s_alarm.realloc_taskstack){
				_strcpy_("rts, ");
			}
			if(s_alarm.overflow_taskstack_future){
				_strcpy_("ots, ");
			}
			mptr -= 2;
			_strcpy_(".\r\n");
		}
		/* Fault */
		if(*(s_u8_t *)&s_fault){
			_strcpy_("Fault: ");
			if(s_fault.mallocfailed_msgnode){
				_strcpy_("mmn, ");
			}
			if(s_fault.mallocfailed_tasknode){
				_strcpy_("mtn, ");
			}
			if(s_fault.mallocfailed_taskstack){
				_strcpy_("mts, ");
			}
			if(s_fault.reallocfailed_taskstack){
				_strcpy_("rts, ");
			}
			if(s_fault.overflow_taskstack){
				_strcpy_("ots, ");
			}
			if(s_fault.failed_startuptask){
				_strcpy_("fst, ");
			}
			if(s_fault.error_recvmsg_int){
				_strcpy_("erm, ");
			}
			#if SYSCFG_PENDSVFIFO_MONITOR == __ENABLED__
			if(s_fault.overflow_pendsvfifo){
				_strcpy_("opd, ");
			}
			#endif
			mptr -= 2;
			_strcpy_(".\r\n");
		}
		/* NULL Line */
		_strcpy_("\r\n");
		/* string tail */
		_chrcpy_('\0');
		/* - */
		mSysIRQ_Disable;
		upspeed = s_timqry_reload[OS_TMID_TASKMGR];
		s_debug_sendtype |= OS_DEBUG_SEND_TASKMGR;
		s_task_current->status |= OS_STATUS_SUSPENDED;
		su_exit_critical_psv();
	}
	uEndTasking;
}

#endif
