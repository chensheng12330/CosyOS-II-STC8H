/**************************************************************************//**
 * @item     CosyOS-II Kernel
 * @file     os_handler.c
 * @brief    OS_SysTick_Handler and sPendSV_Handler
 * @author   迟凯峰
 * @version  V3.0.0
 * @date     2024.06.25
 ******************************************************************************/

#include "os_link.h"

#if SYSCFG_DEBUGGING == __ENABLED__
static s_u32_t usedtime[2];
#endif
static s_u8_t         _SYS_MEM_ task_queue_len = 0;
static s_tasknode_tsp _SYS_MEM_ task_node[3] = {OS_NULL, OS_NULL, OS_NULL};
#define	node_head               task_node[0]          /*!< 头节点 */
#define	node_midd               task_node[1]          /*!< 中间节点 */
#define	node_tail               task_node[2]          /*!< 尾节点 */
#define	node_last               node_curr->last       /*!< 上一节点 */
#define	node_next               node_curr->next       /*!< 下一节点 */
#define	move_forward            node_curr = node_last /*!< 节点前移 */
#define	move_backward           node_curr = node_next /*!< 节点后移 */

void OS_SysTick_Handler(void) MCUCFG_SYSTICK_ATTRIBUTE
{
	s_u8_t _SYS_REG_ i;
	s_tasknode_tsp _SYS_REG_ node_curr;
	#if SYSCFG_SYSTICKTIME_COUNT == __ENABLED__
	static m_tick_t tick_temp;
	#endif
	
	/* 清除中断标志位 */
	mSysTick_Clear;
	
	/* main()中完成启动系统任务之前，系统滴答不处理 */
	if(s_sign_return) return;
	
	/* 系统滴答时间统计开始 */
	#if SYSCFG_SYSTICKTIME_COUNT == __ENABLED__
	if(s_sign_taskmgr){
		tick_temp = mSysTick_Counter;
	}
	#endif
	
	/* 软件RTC */
	#if SYSCFG_SOFTRTC == __ENABLED__
	if(true){
		static
		#if SYSCFG_SYSTICKCYCLE > 1000000UL / 65536
		s_u16_t
		#else
		s_u32_t
		#endif
		_SYS_MEM_ counter = 0;
		if(counter < 1000000UL / SYSCFG_SYSTICKCYCLE - 1) counter++;
		else{
			counter = 0;
			s_sign_every.second = true;
			if(s_rtc[0][5] < 59) s_rtc[0][5]++;
			else{
				s_rtc[0][5] = 0;
				s_sign_every.minute = true;
				if(s_rtc[0][4] < 59) s_rtc[0][4]++;
				else{
					s_rtc[0][4] = 0;
					s_sign_every.hour = true;
					if(s_rtc[0][3] < 23) s_rtc[0][3]++;
					else{
						s_rtc[0][3] = 0;
						s_sign_every.day = true;
						if(s_rtc[0][6] < 7) s_rtc[0][6]++;
						else s_rtc[0][6] = 1;
						if(s_rtc[0][2] < (s_rtc[0][1] == 2 ? s_month2day : s_month0day[s_rtc[0][1]])) s_rtc[0][2]++;
						else{
							s_rtc[0][2] = 1;
							s_sign_every.month = true;
							if(s_rtc[0][1] < 12) s_rtc[0][1]++;
							else{
								s_rtc[0][1] = 1;
								s_sign_every.year = true;
								if(s_rtc[0][0] < 99) s_rtc[0][0]++;
								else s_rtc[0][0] = 0;
							}
						}
					}
				}
			}
		}
		tUpdateCopy(s_memcpy(s_rtc[1], s_rtc[0], sizeof(s_rtc[0])));
	}
	#endif
	
	#if SYSCFG_DEBUGGING == __ENABLED__
	if(s_sign_debug_send){
		s_sign_debug_send = false;
		tTimQry_ms(OS_TMID_DEBUGHOOK, 30);
	}
	if(s_sign_debug_recv){
		s_sign_debug_recv = false;
		tTimInt_ms(OS_TMID_DEBUGGER, 15);
	}
	#endif
	
	/* 滴答钩子 */
	#if SYSCFG_TICKHOOK == __ENABLED__
	tick_hook();
	#endif
	
	/* 定时中断 */
	#if OS_TIMINTTOTAL
	i = OS_TIMINTTOTAL;
	while(i--){
		if(s_timint_loader[i]){
			s_timint_loader[i]--;
			if(!s_timint_loader[i]){
				if(s_timint_autoreload[i]){
					s_timint_loader[i] = s_timint_reload[i];
				}
				if(s_timint_hookortask[i]){
					sResumeTask_TimInt(i);
				}
				else{
					(*(s_voidvoid_tfp)s_timint_handle[i]->hookorhand)();
				}
			}
		}
	}
	#endif
	
	/* 定时查询 */
	#if OS_TIMQRYTOTAL
	i = OS_TIMQRYTOTAL;
	while(i--){
		if(s_timqry_loader[i] && s_timqry_loader[i] < (s_timqry_t)~0){
			s_timqry_loader[i]--;
		}
		if(!s_timqry_loader[i]){
			if((*s_timqry_handle[i]->event)()){
				if(s_timqry_autoreload[i]){
					s_timqry_loader[i] = s_timqry_reload[i];
				}
				else{
					s_timqry_loader[i] = ~0;
				}
				if(s_timqry_hookortask[i]){
					sResumeTask_TimQry(i);
				}
				else{
					(*(s_voidvoid_tfp)s_timqry_handle[i]->hookorhand)();
				}
			}
		}
	}
	#endif
	
	/* 延时定时器 */
	i = task_queue_len - 1;
	node_curr = node_head;
	while(true){
		if(node_curr->status <= OS_STATUS_BLOCKED){
			if(node_curr->timer && node_curr->timer < (s_delay_t)~0){
				node_curr->timer--;
			}
		}
		if(!--i) break;
		move_backward;
	}
	
	/* 时间片滴答计数 */
	s_tick_counter++;
	
	/* 安全运行时 */
	#if SYSCFG_SAFERUNTIME == __ENABLED__
	node_curr = s_task_current;
	if(node_curr != OS_NULL && node_curr->pri && node_curr->saferuntime){
		node_curr->counter++;
		if(node_curr->counter > 1UL * node_curr->saferuntime * OS_TIMESHARING){
			node_curr->status = OS_STATUS_OVERTIME;
			s_sign_timeout[node_curr->tid] = true;
			#if SYSCFG_DEBUGGING == __ENABLED__
			s_alarm.timedout_saferuntime = true;
			#endif
		}
	}
	#endif
	
	/* 任务管理器相关 */
	#if SYSCFG_DEBUGGING == __ENABLED__
	if(s_sign_taskmgr){
		usedtime[0]++; /*!< CPU使用时间计数 */
		#if SYSCFG_SYSTICKTIME_COUNT == __ENABLED__
		mSysTick_Counting; /*!< 系统滴答时间统计 */
		#endif
	}
	#endif
	
	/* 触发定时调度 */
	if(true){
		s_sign_schedule = true;
		s_sign_schedule_all = true;
		mPendSV_Set;
	}
}

/* 移除节点（从任务队列中移除当前节点）*/
#define	node_remove \
do{ \
	node_last->next = node_next; \
	node_next->last = node_last; \
	if(node_curr == node_head){ \
		node_head = node_next; \
	} \
	task_queue_len--; \
}while(false)

/* 插入方向 */
#define forward   0x01 /*!< 向前 */
#define voidward  0x00 /*!< 空向 */
#define backward  0xFF /*!< 向后 */

/* 前插（新节点插入当前节点之前）*/
#define	insert_forward \
do{ \
	node_news->next = node_curr; \
	node_news->last = node_last; \
	node_last->next = node_news; \
	node_last = node_news; \
	if(node_curr == node_head){ \
		node_head = node_news; \
	} \
	switch(insert_direction){ \
	case forward: \
		insert_direction = voidward; \
		node_midd = node_midd->last; \
	break; \
	case voidward: \
		insert_direction = forward; \
	break; \
	case backward: \
		insert_direction = voidward; \
	break; \
	} \
}while(false)

/* 后插（新节点插入当前节点之后）*/
#define	insert_backward \
do{ \
	node_news->last = node_curr; \
	node_news->next = node_next; \
	node_next->last = node_news; \
	node_next = node_news; \
	if(node_curr == node_tail){ \
		node_tail = node_news; \
	} \
	switch(insert_direction){ \
	case forward: \
		insert_direction = voidward; \
	break; \
	case voidward: \
		insert_direction = backward; \
	break; \
	case backward: \
		insert_direction = voidward; \
		node_midd = node_midd->next; \
	break; \
	} \
}while(false)

/* 抢占式调度 or 时间片轮转调度 */
#define preemptive_or_rotate \
do{ \
	if(!sign_rotate){ \
		node_news = node_curr; \
		if(node_news->pri != s_task_current->pri){ \
			goto __TASKING_PREPROCESS; \
		} \
		sign_rotate = true; \
	} \
}while(false)

s_tasknode_tsp  _SYS_MEM_ s_task_news = OS_NULL;
#if SYSCFG_DEBUGGING == __ENABLED__
m_stacksize_t   _SYS_MEM_ s_taskstacklen;
#endif

#if SYSCFG_MCUCORE == 80251
#pragma functions(static)
#endif
s_u8_t sPendSV_Handler(void) MCUCFG_C51USING
{
	/* 中断挂起服务 with FIFO */
	mPendSV_FIFOHandle;
	
	/* 挂起服务钩子 */
	#if SYSCFG_PENDSVHOOK == __ENABLED__
	pendsv_hook();
	#endif
	
	/* 更新全局变量副本 */
	#if SYSCFG_SOFTRTC == __ENABLED__ \
	 || SYSCFG_GVARHOOK == __ENABLED__
	s_sign_updatecopy = true;
	#if SYSCFG_SOFTRTC == __ENABLED__
	if(s_sign_rtc){
		s_sign_rtc = false;
		s_memcpy(s_rtc[1], s_rtc[0], sizeof(s_rtc[0]));
	}
	#endif
	#if SYSCFG_GVARHOOK == __ENABLED__
	gvar_hook();
	#endif
	s_sign_updatecopy = false;
	#endif
	
	/* 任务调度 */
	if(s_sign_schedule){
		static s_u8_t  _SYS_MEM_ insert_direction = 0;
		s_tasknode_tsp _SYS_REG_ node_buff[3];
		#define	node_curr        node_buff[0]    /*!< 当前节点 */
		#define	node_news        node_buff[1]    /*!< 新加节点 */
		#define	node_temp        node_buff[2]    /*!< 缓存节点 */
		m_bit_t sign_x = false;
		#define sign_rotate      sign_x          /*!< 轮转信号 */
		#define sign_switch      sign_x          /*!< 切换信号 */
		#define sign_push        sign_x          /*!< 入栈信号 */
		#if SYSCFG_DEBUGGING == __ENABLED__
		m_tick_t _SYS_REG_ counter;
		#endif
		s_sign_schedule = false;
		
		/* 启动任务 */
		if(s_task_starter != OS_NULL){
			if(task_queue_len < OS_TASKQUEUELEN_MAX){
				/* 静态创建 */
				if(*s_task_starter->handle != OS_NULL){
					node_news = (s_tasknode_tsp)s_task_starter;
					if(node_news->stacksize < MCUCFG_BASICSTACKSIZE){
						#if SYSCFG_DEBUGGING == __ENABLED__
						s_fault.failed_startuptask = true;
						s_fault.overflow_taskstack = true;
						#endif
						s_startup_code = OS_ECODE_OVERFLOW_TASKSTACK;
						goto __STARTUP_END;
					}
					else{
						mTaskStack_INIT;
					}
				}
				/* 动态创建 */
				else{
					node_news = OS_NULL;
					node_news = (s_tasknode_tsp)s_malloc(sizeof(s_tasknode_ts));
					if(node_news != OS_NULL){
						node_news->stacksize = s_task_starter->stacksize;
						if(node_news->stacksize < MCUCFG_BASICSTACKSIZE){
							node_news->stacksize = MCUCFG_BASICSTACKSIZE;
							node_news->create = 2;
						}
						else{
							node_news->create = 1;
						}
						node_news->bsp = OS_NULL;
						node_news->bsp = (s_u8_t _MALLOC_MEM_ *)s_malloc(node_news->stacksize);
						if(node_news->bsp != OS_NULL){
							mTaskStack_INIT;
							node_news->tid = s_task_starter->tid;
							node_news->oldpri = s_task_starter->oldpri;
							node_news->handle = s_task_starter->handle;
							*node_news->handle = node_news;
							#if SYSCFG_DEBUGGING == __ENABLED__
							node_news->name = s_task_starter->name;
							#endif
							#if SYSCFG_SAFERUNTIME == __ENABLED__
							node_news->saferuntime = s_task_starter->saferuntime;
							#endif
							node_news->status = s_task_status0;
						}
						else{
							s_free(node_news);
							#if SYSCFG_DEBUGGING == __ENABLED__
							s_fault.failed_startuptask = true;
							s_fault.mallocfailed_taskstack = true;
							#endif
							s_startup_code = OS_ECODE_MALLOCFAIL_TASKSTACK;
							goto __STARTUP_END;
						}
					}
					else{
						#if SYSCFG_DEBUGGING == __ENABLED__
						s_fault.failed_startuptask = true;
						s_fault.mallocfailed_tasknode = true;
						#endif
						s_startup_code = OS_ECODE_MALLOCFAIL_TASKNODE;
						goto __STARTUP_END;
					}
				}
				/* 公共部分 */
				if(node_news->oldpri <= (s_u8_t)(SYSCFG_TASKPRIORITY - 1)){
					node_news->pri = node_news->oldpri;
				}
				else{
					node_news->pri = SYSCFG_TASKPRIORITY - 2;
					#if SYSCFG_DEBUGGING == __ENABLED__
					s_alarm.outrange_taskpriority = true;
					#endif
				}
				node_news->blocktype = 0;
				node_news->timer = 0;
				node_news->ptr = OS_NULL;
				#if SYSCFG_DEBUGGING == __ENABLED__
				node_news->usedtime[0] = 0;
				node_news->usedtime[1] = 0;
				node_news->stacklen_max = MCUCFG_BASICSTACKSIZE;
				#endif
				#if SYSCFG_SAFERUNTIME == __ENABLED__
				node_news->counter = 0;
				#endif
				/* 新任务加入任务队列 */
				if(!task_queue_len){
					node_head = node_midd = node_tail = node_news->last = node_news->next = node_news;
				}
				else{
					node_curr = node_midd;
					if(node_curr != node_tail && node_news->pri <= node_curr->pri){
						do{
							move_backward;
						}while(node_curr != node_tail && node_news->pri <= node_curr->pri);
					}
					else if(node_curr != node_head && node_news->pri > node_curr->pri){
						do{
							move_forward;
						}while(node_curr != node_head && node_news->pri > node_curr->pri);
					}
					if(node_news->pri > node_curr->pri){
						insert_forward;
					}
					else if(node_curr == node_tail && node_curr->pri == 0){
						insert_forward;
					}
					else{
						insert_backward;
					}
				}
				task_queue_len++;
				s_startup_code = OS_ECODE_NOERROR;
			}
			else{
				#if SYSCFG_DEBUGGING == __ENABLED__
				s_fault.failed_startuptask = true;
				s_alarm.overflow_taskqueue = true;
				#endif
				s_startup_code = OS_ECODE_OVERFLOW_TASKQUEUE;
			}
			__STARTUP_END:
			s_task_starter = OS_NULL;
			if(s_sign_return){
				if(node_news == u_taskhandle_Sysidle){
					s_sign_return = false;
					node_news = u_taskhandle_Starter;
					goto __NEWTASK_PREPROCESS; /*!< 首次任务调度 -> 新任务预处理 */
				}
				else{
					return 0;
				}
			}
		}
		
		/*
		 * 任务优先级调整
		 * 如果任务优先级发生改变，执行服务时已从任务队列中移除并加入到临时链表。
		 * 在此，从临时链表中依次取出各任务，并根据任务优先级重新加入到任务队列。
		 */
		if(s_taskpri_tail != OS_NULL){
			node_temp = s_taskpri_tail;
			do{
				node_news = node_temp;
				node_temp = node_temp->last;
				node_curr = node_midd;
				if(node_curr != node_tail && node_news->pri <= node_curr->pri){
					do{
						move_backward;
					}while(node_curr != node_tail && node_news->pri <= node_curr->pri);
				}
				else if(node_curr != node_head && node_news->pri > node_curr->pri){
					do{
						move_forward;
					}while(node_curr != node_head && node_news->pri > node_curr->pri);
				}
				if(node_news->pri > node_curr->pri){
					insert_forward;
				}
				else if(node_curr == node_tail && node_curr->pri == 0){
					insert_forward;
				}
				else{
					insert_backward;
				}
			}while(node_temp != OS_NULL);
			s_taskpri_tail = OS_NULL;
		}
		
		/* 调度类型：*/
		/* 1、立即切换 */
		if(!s_sign_schedule_all && s_task_switch != OS_NULL){
			sign_switch = true;
			node_news = s_task_switch;
			s_task_switch = OS_NULL;
			goto __TASKING_PREPROCESS;
		}
		/* 2、当前调度：从当前任务优先级组首节点开始查找就绪 */
		if(!s_sign_schedule_all){
			node_curr = s_task_current;
			while(node_last->pri == node_curr->pri){
				move_forward;
			}
		}
		/* 3、完整调度：从任务队列头节点开始查找就绪 */
		else{
			s_sign_schedule_all = false;
			s_task_switch = OS_NULL;
			node_curr = node_head;
		}
		
		/* 开始调度：更新任务状态并同步查找最高优先级的就绪任务 */
		while(true){
			switch(node_curr->status){
			case OS_STATUS_FLOATING:
				if(!node_curr->timer){
					node_curr->status = OS_STATUS_READY;
					preemptive_or_rotate;
				}
				else{
					switch(node_curr->blocktype & 0x0F){
					case 1:
						if(*(s_u8_t  *)node_curr->ptr){
							preemptive_or_rotate;
						}
						else{
							node_curr->status = OS_STATUS_BLOCKED;
						}
					break;
					case 2:
						if(*(s_u16_t *)node_curr->ptr){
							preemptive_or_rotate;
						}
						else{
							node_curr->status = OS_STATUS_BLOCKED;
						}
					break;
					case 4:
						if(*(s_u32_t *)node_curr->ptr){
							preemptive_or_rotate;
						}
						else{
							node_curr->status = OS_STATUS_BLOCKED;
						}
					break;
					case 0x0F:
						if(!*(s_u8_t *)node_curr->ptr){
							preemptive_or_rotate;
						}
						else{
							node_curr->status = OS_STATUS_BLOCKED;
						}
					break;
					}
				}
			break;
			case OS_STATUS_BLOCKED:
				if(!node_curr->timer){
					node_curr->status = OS_STATUS_READY;
					preemptive_or_rotate;
				}
				else if(node_curr->blocktype != OS_BLOCKED_DELAY){
					switch(node_curr->blocktype & 0x0F){
					case 1:
						if(*(s_u8_t  *)node_curr->ptr){
							node_curr->status = OS_STATUS_FLOATING;
							preemptive_or_rotate;
						}
					break;
					case 2:
						if(*(s_u16_t *)node_curr->ptr){
							node_curr->status = OS_STATUS_FLOATING;
							preemptive_or_rotate;
						}
					break;
					case 4:
						if(*(s_u32_t *)node_curr->ptr){
							node_curr->status = OS_STATUS_FLOATING;
							preemptive_or_rotate;
						}
					break;
					case 0x0F:
						if(!*(s_u8_t *)node_curr->ptr){
							node_curr->status = OS_STATUS_FLOATING;
							preemptive_or_rotate;
						}
					break;
					}
				}
			break;
			case OS_STATUS_READY:
				preemptive_or_rotate;
			break;
			case OS_STATUS_DELETED:
				if(node_curr != s_task_current){
					node_remove;
					if(node_curr->create){
						s_free(node_curr->bsp);
						*node_curr->handle = OS_NULL;
						s_free(node_curr);
					}
				}
			break;
			#if SYSCFG_SAFERUNTIME == __ENABLED__
			case OS_STATUS_OVERTIME:
				if(!s_sign_timeout[node_curr->tid]) {
					node_curr->status = OS_STATUS_READY;
					preemptive_or_rotate;
				}
			break;
			#endif
			}
			move_backward;
			/* 当前任务优先级组查询完毕 */
			if(sign_rotate && node_curr->pri != s_task_current->pri){
				sign_rotate = false;
				break;
			}
		}
		
		/* 当前任务预处理 */
		__TASKING_PREPROCESS:{
			/* 每调度监控（假定入栈）*/
			#if SYSCFG_DEBUGGING == __ENABLED__
			if(s_task_current->stacklen_max < s_taskstacklen){
				s_task_current->stacklen_max = s_taskstacklen;
				if(s_task_current->stacksize < s_taskstacklen){
					s_alarm.overflow_taskstack_future = true;
				}
			}
			#endif
		}
		
		/* 最终调度 */
		if(!sign_switch){
			/* 新任务与当前任务优先级不同 -> 切换至新任务 */
			if(node_news->pri != s_task_current->pri){
				goto __TASKING_POSTPROCESS;
			}
			/* 时间片轮转调度：*/
			/* 1、当前任务为就绪 && 时间片未到期 -> 返回 */
			if(s_tick_counter < OS_TIMESHARING && s_task_current->status <= OS_STATUS_FLOATING){
				s_task_current->status = OS_STATUS_READY;
				if(s_task_current->blocktype != OS_BLOCKED_DELAY){
					mSysIRQ_Disable;
				}
				return 0;
			}
			/* 2、当前任务未就绪 || 时间片已到期 -> 向后轮询查找就绪 */
			node_curr = s_task_current;
			move_backward;
			while(node_last != node_tail && node_last->pri == node_curr->pri){
				if(node_curr->status > OS_STATUS_FLOATING){
					move_backward;
				}
				else{
					node_news = node_curr;
					goto __TASKING_POSTPROCESS;
				}
			}
			/* 3、新任务仍为当前任务，说明时间片已到期 -> 返回 */
			if(node_news == s_task_current){
				s_tick_counter = 0;
				s_task_current->status = OS_STATUS_READY;
				if(s_task_current->blocktype != OS_BLOCKED_DELAY){
					mSysIRQ_Disable;
				}
				return 0;
			}
			/* 4、新任务不是当前任务，必位于当前任务之前 -> 切换至新任务 */
		}
		sign_switch = false;
		
		/* 当前任务后处理 */
		__TASKING_POSTPROCESS:{
			if(s_task_current->status < OS_STATUS_STOPPED){
				/* 入栈监控 */
				#if SYSCFG_DEBUGGING == __ENABLED__
				if(s_task_current->stacksize < s_taskstacklen){
					#if MCUCFG_TASKSTACK_REALLOC == __ENABLED__
					if(s_task_current->create){
						s_task_current->stacksize = s_taskstacklen + MCUCFG_TASKSTACKREALLOC_INC;
						s_free(s_task_current->bsp);
						s_task_current->bsp = OS_NULL;
						s_task_current->bsp = (s_u8_t _MALLOC_MEM_ *)s_malloc(s_task_current->stacksize);
						if(s_task_current->bsp != OS_NULL){
							sign_push = true;
							s_task_current->create = 2;
							s_alarm.realloc_taskstack = true;
						}
						else{
							s_task_current->status = OS_STATUS_STOPPED_TSRF;
							s_fault.reallocfailed_taskstack = true;
						}
					}
					else{
						s_task_current->status = OS_STATUS_STOPPED_TSOF;
						s_fault.overflow_taskstack = true;
					}
					#else
					s_task_current->status = OS_STATUS_STOPPED_TSOF;
					s_fault.overflow_taskstack = true;
					#endif
				}
				else{
					sign_push = true;
				}
				#else
				sign_push = true;
				#endif
			}
			else if(s_task_current->status == OS_STATUS_DELETED){
				node_curr = s_task_current;
				node_remove;
				if(node_curr->create){
					s_free(node_curr->bsp);
					*node_curr->handle = OS_NULL;
					s_free(node_curr);
				}
			}
		}
		
		/* 新任务预处理 */
		__NEWTASK_PREPROCESS:{
			s_tick_counter = 0;
			node_news->status = OS_STATUS_READY;
			if(node_news->blocktype != OS_BLOCKED_DELAY){
				mSysIRQ_Disable;
			}
			#if SYSCFG_DEBUGGING == __ENABLED__
			if(s_sign_taskmgr) counter = mSysTick_Counter;
			#endif
		}
		
		/* 当前任务入栈 */
		if(sign_push){
			#if SYSCFG_SAFERUNTIME == __ENABLED__
			if(s_task_current->pri > node_news->pri){
				s_task_current->counter = 0;
			}
			#endif
			#if SYSCFG_DEBUGGING == __ENABLED__
			if(s_sign_taskmgr) mUsedTime_END;
			#endif
			mUserReg_CSave;
		}
		
		/* 新任务出栈 */
		if(true){
			#if SYSCFG_DEBUGGING == __ENABLED__
			if(s_sign_taskmgr) mUsedTime_INIT;
			#endif
			mUserReg_CRes;
			s_task_news = node_news;
		}
		
		if(sign_push) return 2;
		else return 1;
	}
	else return 0;
}
