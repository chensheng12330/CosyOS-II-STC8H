/**************************************************************************//**
 * @item     CosyOS-II 行为同步测试
 * @file     test_xwtb_group.c
 * @brief    事件标志组测试
 * @detail   通过事件标志组的相关应用，实现中断对任务、任务对任务的行为同步（事件同步）。
 * @service  事件标志组相关服务、定时服务
 * @author   迟凯峰
 * @version  V1.1.0
 * @date     2024.06.25
 ******************************************************************************/

#include "..\Test\test.h"
#if TEST_OBJECT == TEST_XWTB_GROUP

/*
 * 用户定义
 */

// 测试类型（0：中断挂起服务FIFO队列，1：中断挂起服务FLAG队列）
#define TEST_TYPE     0

////////////////////////////////////////////////////////////////////////////////

m_bit_t u_timing_uart1_send = false; // 中断挂起服务FLAG队列 - 标志位
m_bit_t u_timing_uart1_recv = false; // 中断挂起服务FLAG队列 - 标志位

#define tmid_uart1_send 0
#define tmid_uart1_recv 0

struct
{
	char *send_ptr;
	char recv_temp[512];
	u16  recv_count;
}xdata u_uart1;

/* 声明并创建事件标志组 */
uExternFlagGroup
(
	group_uart1,
	uDefFlagBit(flag0);
	uDefFlagBit(flag1);
	uDefFlagBit(flag2);
	uDefFlagBit(flag3);
	uDefFlagBit(flag4);
	uDefFlagBit(flag5);
	uDefVoidBits(2);
);
uCreateFlagGroup(group_uart1);

/* UART1中断 */
void UART1_Isr(void) interrupt 4
{
	if(TI)
	{
		TI = 0;
		if(*u_uart1.send_ptr) SBUF = *u_uart1.send_ptr++;
		/* UART1发送分包间隔30ms */
		#if TEST_TYPE == 0
		else iTimQry_ms(tmid_uart1_send, 30); // 中断挂起服务FIFO队列
		#else
		else u_timing_uart1_send = true; // 中断挂起服务FLAG队列，定时服务仅时间关键，所以无需触发PendSV，而是在 tick_hook() 中执行。
		#endif
	}
	if(RI)
	{
		RI = 0;
		if(u_uart1.recv_count < sizeof(u_uart1.recv_temp)) u_uart1.recv_temp[u_uart1.recv_count++] = SBUF;
		/* 20ms分包提前解析，外部设备发送至串口的分包间隔要大于20ms+解析时间 */
		#if TEST_TYPE == 0
		iTimInt_ms(tmid_uart1_recv, 20); // 中断挂起服务FIFO队列
		#else
		u_timing_uart1_recv = true; // 中断挂起服务FLAG队列，定时服务仅时间关键，所以无需触发PendSV，而是在 tick_hook() 中执行。
		#endif
	}
}

/**
 * \brief    串口1接收任务
 * \detail   负责串口1接收数据的解析。
 */
uCreateTask_TimInt
(
	tmid_uart1_recv, false,
	task_uart1_recv, 6, TEST_TASKSTACKSIZE, 0, 0
)
{
	u_uart1.recv_temp[u_uart1.recv_count] = '\0';
	OS_NOPx1; // 接收解析
	u_uart1.recv_count = 0;
	uSuspendTasking;
	uEndTasking;
}

/**
 * \brief    串口1发送任务
 * \detail   按自定义的发送优先级依次查询各发送标志位，为真则清除标志位并发送相应数据。
 */
uCreateTask_TimQry
(
	tmid_uart1_send, tQueryFlagGroup(group_uart1), false,
	task_uart1_send, 5, TEST_TASKSTACKSIZE, 0, 0
)
{
	if(group_uart1.flag0){
		uClearFlagBit(group_uart1, flag0);
		u_uart1.send_ptr = "uart1 send data0\r\n";
		TI = 1;
	}
	else if(group_uart1.flag1){
		uClearFlagBit(group_uart1, flag1);
		u_uart1.send_ptr = "uart1 send data1\r\n";
		TI = 1;
	}
	else if(group_uart1.flag2){
		uClearFlagBit(group_uart1, flag2);
		u_uart1.send_ptr = "uart1 send data2\r\n";
		TI = 1;
	}
	else if(group_uart1.flag3){
		uClearFlagBit(group_uart1, flag3);
		u_uart1.send_ptr = "uart1 send data3\r\n";
		TI = 1;
	}
	else if(group_uart1.flag4){
		uClearFlagBit(group_uart1, flag4);
		u_uart1.send_ptr = "uart1 send data4\r\n";
		TI = 1;
	}
	else if(group_uart1.flag5){
		uClearFlagBit(group_uart1, flag5);
		u_uart1.send_ptr = "uart1 send data5\r\n";
		TI = 1;
	}
	uSuspendTasking;
	uEndTasking;
}

uCreateTask(task_1, 2, TEST_TASKSTACKSIZE, 0, 0)
{
	uSetFlagBit(group_uart1, flag3);
	uEnterCritical;
	group_uart1.flag4 = group_uart1.flag5 = 1;
	uExitCritical;
	uDelay_ms(1000);
	uEndTasking;
}

/**
 * \brief    定时器A中断
 * \detail   先对事件做必要的紧急处理，再同步至 task_2 做后续处理。
 */
TEST_ISR_TIMA
{
	TEST_ISR_TIMA_CLR;
	iSetFlagBit(group_uart1, flag0);
	iSetFlagBits(group_uart1, 2) flag1, flag2);
}

/**
 * \brief    启动钩子
 * \detail   在启动任务中被调用，可在此启动用户任务。
 */
void start_hook(void)
{
	uStartTask(task_1, 0);
	TEST_TIMA_INIT;
}

/* 在未启用任务管理器的前提下，如果用户仅创建一个定时查询（任务或钩子），
 * 系统会运行异常，所以又创建了如下的定时查询来补充。
 * 事件标志组测试时，用户另需在系统配置文件中重新定义用户定时中断和定时查询总数。
 */
#if !SYSCFG_DEBUGGING
#define tmid_uart2_send 1
uCreateTask_TimQry
(
	tmid_uart2_send, false, false,
	task_uart2_send, 5, TEST_TASKSTACKSIZE, 0, 0
)
{
	OS_NOPx1;
	uSuspendTasking;
	uEndTasking;
}
#endif

#endif
