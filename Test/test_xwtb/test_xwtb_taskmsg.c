/**************************************************************************//**
 * @item     CosyOS-II 行为同步测试
 * @file     test_xwtb_taskmsg.c
 * @brief    私信测试
 * @detail   通过私信的接收/发送，实现任务对任务的行为同步（消息同步）。
 * @service  uRecvTaskMsg(tick); 任务中接收私信
             uSendTaskMsg(task); 任务中发送私信
 * @author   迟凯峰
 * @version  V1.1.0
 * @date     2024.06.25
 ******************************************************************************/

#include "..\Test\test.h"
#if TEST_OBJECT == TEST_XWTB_TASKMSG

/* 创建计数信号量，初值为0，最大值为65535 */
uCreateSem(sem_2, 0, 65535);

/**
 * \brief    task_1
 * \detail   处理更为耗时的事务。
 */
#if MCUCFG_ISA != __ARM__ && SYSCFG_TASKMSGMODE == __PERFORMANCE__
#pragma NOREGPARMS
#elif MCUCFG_ISA == __ARM__
#pragma push
#pragma O0
#endif
uCreateTask(task_1, 1, TEST_TASKSTACKSIZE, 0, 3)(const char *p, uint a, float b)
{
	if(uRecvTaskMsg(~0)){
		TEST_GPIO_P01_RESET; // 任务被同步运行后，需立即关闭测量信号
		printf("%s %u, %f\r\n", p, a, b); // 打印 task_2 发送至 task_1 的私信
		led_task_1();
	}
	uEndTasking;
}
#if MCUCFG_ISA != __ARM__ && SYSCFG_TASKMSGMODE == __PERFORMANCE__
#pragma REGPARMS
#elif MCUCFG_ISA == __ARM__
#pragma pop
#endif

/**
 * \brief    task_2
 * \detail   先对事件做必要的紧急处理，再同步至 task_1 做后续处理。
 */
uCreateTask(task_2, 2, TEST_TASKSTACKSIZE, 0, 0)
{
	static uint a = 0;
	static float b = 0;
	if(uTakeSem(sem_2, ~0)){
		TEST_GPIO_P02_RESET; // 任务被同步运行后，需立即关闭测量信号
		printf("%s %u, %f\r\n", "TIME_A -> task_2:", ++a, ++b); // 打印 TIME_A 同步至 task_2 的次数
		led_task_2();
		
		TEST_GPIO_P01_SET; // 先开启测量信号，再同步
		uSendTaskMsg(task_1) "task_2 -> task_1:", a, b); /*!< 同步至 task_1 */
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
	iGiveSem(sem_2); /*!< 同步至 task_2 */
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
	printf("\xCB\xBD\xD0\xC5\xB2\xE2\xCA\xD4\r\n");
	/* 初始化定时器A */
	TEST_TIMA_INIT;
}

#endif
