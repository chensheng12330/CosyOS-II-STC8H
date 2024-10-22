/**************************************************************************//**
 * @item     CosyOS-II Hook
 * @file     tick_hook.c
 * @brief    滴答钩子
 * @detail   每个系统滴答周期，在系统滴答中断中都会被调用一次，
             适用于每滴答周期/秒/分/时/日/月/年/...做一次的工作。
 * @note  1. 如果您在滴答钩子中调用服务，应调用滴答API。
          2. 如果您在滴答钩子中调用自定义函数并且您的MCU内核为8051，自定义函数
             可能需要添加using属性、或声明为相对寄存器访问。
             demo1:
             void your_function(void) MCUCFG_C51USING {... ...}
             
             demo2:
             #pragma NOAREGS
             void your_function(void) {... ...}
             #pragma AREGS
 * @author   迟凯峰
 * @version  V3.0.0
 * @date     2024.06.25
 ******************************************************************************/

#include "..\System\os_link.h"
#if SYSCFG_TICKHOOK == __ENABLED__
#include "..\Test\test.h"
extern m_bit_t u_timing_uart1_send;
extern m_bit_t u_timing_uart1_recv;
void tick_hook(void) MCUCFG_C51USING
{
	if(true){ /* 每滴答周期执行一次 */
		#if TEST_OBJECT == TEST_XWTB_GROUP
		if(u_timing_uart1_send){
			u_timing_uart1_send = false;
			tTimQry_ms(0, 30);
		}
		if(u_timing_uart1_recv){
			u_timing_uart1_recv = false;
			tTimInt_ms(0, 20);
		}
		#endif
	}
	#if SYSCFG_SOFTRTC == __ENABLED__
	if(s_sign_every.second){
		s_sign_every.second = false;
		if(true){ /* 每秒钟执行一次（每秒，first tick）*/
			
		}
		if(s_sign_every.minute){
			s_sign_every.minute = false;
			if(true){ /* 每分钟执行一次（每分0秒，first tick）*/
				
			}
			if(s_sign_every.hour){
				s_sign_every.hour = false;
				if(true){ /* 每小时执行一次（每时0分0秒，first tick）*/
					
				}
				if(s_sign_every.day){
					s_sign_every.day = false;
					if(true){ /* 每天执行一次（每天0时0分0秒，first tick）*/
						
					}
					if(s_sign_every.month){
						s_sign_every.month = false;
						if(true){ /* 每月执行一次（每月1日0时0分0秒，first tick）*/
							
						}
						if(s_sign_every.year){
							s_sign_every.year = false;
							if(true){ /* 每年执行一次（每年1月1日0时0分0秒，first tick）*/
								
							}
						}
					}
				}
			}
		}
	}
	#endif
}

#endif
