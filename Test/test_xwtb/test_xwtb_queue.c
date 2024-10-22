/**************************************************************************//**
 * @item     CosyOS-II 行为同步测试
 * @file     test_xwtb_queue.c
 * @brief    队列测试
 * @detail   通过消息的接收/发送，实现中断对任务、任务对任务的行为同步（消息同步）。
 * @service  uRecvMsg(que, tick); 任务中接收消息
             uSendMsg(que, msg);  任务中发送消息
             iSendMsg(que, msg);  中断中发送消息
 * @author   迟凯峰
 * @version  V1.1.0
 * @date     2024.06.25
 ******************************************************************************/

#include "..\Test\test.h"
#if TEST_OBJECT == TEST_XWTB_QUEUE
#if MCUCFG_ISA == __MCS_251__
#include "STC32G_UART.h"
#endif

/*
 * 用户定义
 */

// 测试类型（0：传输指针，1：传输数据）
#define TEST_TYPE     0

////////////////////////////////////////////////////////////////////////////////

/* 创建队列 */
uCreateQueue(queue_1, 0, 10); // queue_1：静态队列、FIFO、长度10
uCreateQueue(queue_2, 0, 10); // queue_2：静态队列、FIFO、长度10

/**
 * \brief    task_1
 * \detail   处理更为耗时的事务。
 */
uCreateTask(task_1, 1, TEST_TASKSTACKSIZE, 0, 0)
{
	#if TEST_TYPE == 0
	char *m = (char *)uRecvMsg(queue_1, ~0);
	if(m){
		printf("task_2 -> task_1: %s\r\n", m); // 打印 task_2 发送至 task_1 的常量字符串
		#if MCUCFG_ISA == __MCS_251__
		while(COM1.TX_send != COM1.TX_write);
		#endif
	}
	#else
	uint m = (uint)uRecvMsg(queue_1, ~0);
	if(m){
		printf("task_2 -> task_1: %u\r\n", m); // 打印 task_2 发送至 task_1 的数据
		#if MCUCFG_ISA == __MCS_251__
		while(COM1.TX_send != COM1.TX_write);
		#endif
	}
	#endif
	uEndTasking;
}

/**
 * \brief    task_2
 * \detail   先对事件做必要的紧急处理，再同步至 task_1 做后续处理。
 */
uCreateTask(task_2, 2, TEST_TASKSTACKSIZE, 0, 0)
{
	#if TEST_TYPE == 0
	char *m = (char *)uRecvMsg(queue_2, ~0);
	if(m){
		printf("TIME_A -> task_2: %s\r\n", m); // 打印 TIME_A 发送至 task_2 的常量字符串
		#if MCUCFG_ISA == __MCS_251__
		while(COM1.TX_send != COM1.TX_write);
		#endif
		uSendMsg(queue_1, m); /*!< 同步至 task_1 */
	}
	#else
	uint m = (uint)uRecvMsg(queue_2, ~0);
	if(m){
		printf("TIME_A -> task_2: %u\r\n", m); // 打印 TIME_A 发送至 task_2 的数据
		#if MCUCFG_ISA == __MCS_251__
		while(COM1.TX_send != COM1.TX_write);
		#endif
		uSendMsg(queue_1, (void *)m);
	}
	#endif
	uEndTasking;
}

/**
 * \brief    定时器A中断
 * \detail   先对事件做必要的紧急处理，再同步至 task_2 做后续处理。
 */
TEST_ISR_TIMA
{
	#if TEST_TYPE
	static uint m = 0;
	#endif
	TEST_ISR_TIMA_CLR;
	
	#if TEST_TYPE == 0
	iSendMsg(queue_2, "message: 01");
	iSendMsg(queue_2, "message: 02");
	iSendMsg(queue_2, "message: 03");
	iSendMsg(queue_2, "message: 04");
	iSendMsg(queue_2, "message: 05");
	iSendMsg(queue_2, "message: 06");
	iSendMsg(queue_2, "message: 07");
	iSendMsg(queue_2, "message: 08");
	iSendMsg(queue_2, "message: 09");
	iSendMsg(queue_2, "message: 10");
	#else
	iSendMsg(queue_2, (void *)(!++m ? ++m : m));
	iSendMsg(queue_2, (void *)(!++m ? ++m : m));
	iSendMsg(queue_2, (void *)(!++m ? ++m : m));
	iSendMsg(queue_2, (void *)(!++m ? ++m : m));
	iSendMsg(queue_2, (void *)(!++m ? ++m : m));
	iSendMsg(queue_2, (void *)(!++m ? ++m : m));
	iSendMsg(queue_2, (void *)(!++m ? ++m : m));
	iSendMsg(queue_2, (void *)(!++m ? ++m : m));
	iSendMsg(queue_2, (void *)(!++m ? ++m : m));
	iSendMsg(queue_2, (void *)(!++m ? ++m : m));
	#endif
}

/**
 * \brief    启动钩子
 * \detail   在启动任务中被调用，可在此启动用户任务。
 */
void start_hook(void)
{
	uStartTask(task_1, 0); /*!< 启动 task_1，初始状态为就绪 */
	uStartTask(task_2, 0); /*!< 启动 task_2，初始状态为就绪 */
	/* 打印标题 */
	printf("\x43\x6F\x73\x79\x4F\x53\x2D\x49\x49\x20\xD0\xD0\xCE\xAA\xCD\xAC\xB2\xBD\xB2\xE2\xCA\xD4\r\n");
	printf("\xB6\xD3\xC1\xD0\xB2\xE2\xCA\xD4\r\n");
	/* 初始化定时器A */
	TEST_TIMA_INIT;
}

#endif
