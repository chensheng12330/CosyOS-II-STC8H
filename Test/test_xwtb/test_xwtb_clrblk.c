/**************************************************************************//**
 * @item     CosyOS-II 行为同步测试
 * @file     test_xwtb_clrblk.c
 * @brief    清除阻塞测试
 * @detail   通过阻塞/清除阻塞，实现中断对任务、任务对任务的行为同步（事件同步）。
 * @service  uDelay_x(tick);    任务中阻塞延时
             uClearBlock(task); 任务中清除阻塞
             iClearBlock(task); 中断中清除阻塞 - 中断挂起服务FIFO队列
			 pClearBlock(task); 中断中清除阻塞 - 中断挂起服务FLAG队列
 * @author   迟凯峰
 * @version  V1.1.0
 * @date     2024.06.25
 ******************************************************************************/

#include "..\Test\test.h"
#if TEST_OBJECT == TEST_XWTB_CLRBLK

/*
 * 用户定义
 */

// 测试类型（0：中断挂起服务FIFO队列，1：中断挂起服务FLAG队列）
#define TEST_TYPE     0

////////////////////////////////////////////////////////////////////////////////

m_bit_t u_clrblk_task_2 = false; // 中断挂起服务FLAG队列 - 标志位

/**
 * \brief    task_1
 * \detail   处理更为耗时的事务。
 */
uCreateTask(task_1, 1, TEST_TASKSTACKSIZE, 0, 0)
{
	static uint i = 0;
	uDelay_tc(~0);
	if(1){
		TEST_GPIO_P01_RESET; // 任务被同步运行后，需立即关闭测量信号
		printf("task_2 -> task_1: %u\r\n", ++i); // 打印 task_2 同步至 task_1 的次数
		led_task_1();
	}
	uEndTasking;
}

/**
 * \brief    task_2
 * \detail   先对事件做必要的紧急处理，再同步至 task_1 做后续处理。
 */
uCreateTask(task_2, 2, TEST_TASKSTACKSIZE, 0, 0)
{
	static uint i = 0;
	uDelay_tc(~0);
	if(1){
		TEST_GPIO_P02_RESET; // 任务被同步运行后，需立即关闭测量信号
		printf("TIME_A -> task_2: %u\r\n", ++i); // 打印 TIME_A 同步至 task_2 的次数
		led_task_2();
		
		TEST_GPIO_P01_SET; // 先开启测量信号，再同步
		uClearBlock(task_1); /*!< 同步至 task_1 */
	}
	uEndTasking;
}

/**
 * \brief    定时器A中断
 * \detail   先对事件做必要的紧急处理，再同步至 task_2 做后续处理。
 */
TEST_ISR_TIMA
{
	TEST_ISR_TIMA_CLR;
	TEST_GPIO_P02_SET; // 先开启测量信号，再同步
	/*!< 同步至 task_2 */
	#if TEST_TYPE == 0
	iClearBlock(task_2); // 中断挂起服务FIFO队列
	#else
	u_clrblk_task_2 = true; // 中断挂起服务FLAG队列
	mPendSV_Set; // 触发PendSV，并在 pendsv_hook() 中执行。
	#endif
}

/**
 * \brief    启动钩子
 * \detail   在启动任务中被调用，可在此启动用户任务。
 */
void start_hook(void)
{
	TEST_GPIO_P01_RESET;   /*!< task_2 同步至 task_1，任务切换时间测量 */
	TEST_GPIO_P02_RESET;   /*!< TIME_A 同步至 task_2，中断响应时间测量 */
	uStartTask(task_1, 0); /*!< 启动 task_1，初始状态为就绪 */
	uStartTask(task_2, 0); /*!< 启动 task_2，初始状态为就绪 */
	/* 打印标题 */
	printf("\x43\x6F\x73\x79\x4F\x53\x2D\x49\x49\x20\xD0\xD0\xCE\xAA\xCD\xAC\xB2\xBD\xB2\xE2\xCA\xD4\r\n");
	printf("\xC7\xE5\xB3\xFD\xD7\xE8\xC8\xFB\xB2\xE2\xCA\xD4\r\n");
	/* 初始化定时器A */
	TEST_TIMA_INIT;
}

#endif
