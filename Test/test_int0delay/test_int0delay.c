/**************************************************************************//**
 * @file     test_int0delay.c
 * @brief    零中断延迟测试
 * @detail   非最低优先级中断，零中断延迟测试。
 * @author   迟凯峰
 * @version  V1.1.0
 * @date     2024.06.25
 ******************************************************************************/

#include "..\Test\test.h"
#if TEST_OBJECT == TEST_INT0DELAY

/*
 * 用户定义
 */

// 测试类型
// 0：无服务调用
// 1：有服务调用并恢复task_3 - 中断挂起服务FIFO队列
// 2：有服务调用并恢复task_3 - 中断挂起服务FLAG队列
#define TEST_TYPE     0

////////////////////////////////////////////////////////////////////////////////

#if TEST_TYPE == 0
	#if MCUCFG_ISA == __MCS_51__
	#define TEST_TIMB_FREQ  1000000UL // 1us
	#elif MCUCFG_ISA == __MCS_251__
	#define TEST_TIMB_FREQ  1000000UL // 1us
	#elif MCUCFG_ISA == __ARM__
	#define TEST_TIMB_FREQ  4000000UL // 0.25us
	#endif
#else
	#if MCUCFG_ISA == __MCS_51__
	#define TEST_TIMB_FREQ   200000UL // 5us
	#elif MCUCFG_ISA == __MCS_251__
	#define TEST_TIMB_FREQ   500000UL // 2us
	#elif MCUCFG_ISA == __ARM__
	#define TEST_TIMB_FREQ  2000000UL // 0.5us
	#endif
#endif

m_bit_t u_resume_task_3 = false; // 中断挂起服务FLAG队列 - 标志位

/* 创建二值信号量 */
uCreateBin(bin_1, 1);
uCreateBin(bin_2, 0);

/*
 * task_1
 */
uCreateTask(task_1, 1, TEST_TASKSTACKSIZE, 0, 0)
{
	if(uTakeBin(bin_1, ~0)){
		OS_NOPx2;
		uGiveBin(bin_2);
	}
	uEndTasking;
}

/*
 * task_2
 */
uCreateTask(task_2, 2, TEST_TASKSTACKSIZE, 0, 0)
{
	if(uTakeBin(bin_2, ~0)){
		OS_NOPx2;
		uGiveBin(bin_1);
	}
	uEndTasking;
}

/*
 * task_3
 */
uCreateTask(task_3, 3, TEST_TASKSTACKSIZE, 0, 0)
{
	TEST_GPIO_P03_RESET;
	printf("OK\r\n");
	uSuspendTasking;
	uEndTasking;
}

/*
 * 定时器B中断
 */
TEST_ISR_TIMB
{
	#if TEST_TYPE == 1
	static u32 _SYS_MEM_ i = 0;
	#endif
	TEST_ISR_TIMB_CLR;
	TEST_GPIO_P00_TOGGLE;
	#if TEST_TYPE == 1
	/**
	 * \brief    中断挂起服务FIFO队列
	 * \detail   支持有限的并发调用，只要中断挂起服务FIFO队列不溢出就不会出问题；
	             如果是永不停息的并发调用，将导致系统瘫痪。
	*/
	if(++i == TEST_TIMB_FREQ / 1000){
		i = 0;
		TEST_GPIO_P03_SET;
		iResumeTask(task_3);
	}
	#elif TEST_TYPE == 2
	/**
	 * \brief    中断挂起服务FLAG队列
	 * \detail   即使是永不停息的并发调用，任务仍可运行。
	*/
	if(true){
		TEST_GPIO_P03_SET;
		u_resume_task_3 = true;
		mPendSV_Set; // 触发PendSV，并在 pendsv_hook() 中执行。
	}
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
	#if TEST_TYPE
	TEST_GPIO_P03_RESET;
	uStartTask(task_3, 1); /*!< 启动 task_3，初始状态为挂起 */
	#endif
	/* 打印标题 */
	printf("\xC1\xE3\xD6\xD0\xB6\xCF\xD1\xD3\xB3\xD9\xB2\xE2\xCA\xD4\r\n");
	printf("\xB7\xC7\xD7\xEE\xB5\xCD\xD3\xC5\xCF\xC8\xBC\xB6\xD6\xD0\xB6\xCF\xA3\xAC\xC1\xE3\xD6\xD0\xB6\xCF\xD1\xD3\xB3\xD9\xB2\xE2\xCA\xD4\r\n");
	/* 初始化定时器B */
	TEST_TIMB_INIT;
}

#endif
