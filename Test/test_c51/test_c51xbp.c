/**************************************************************************//**
 * @file     test_c51xbp.c
 * @brief    C51-XBPSTACK测试
 * @author   迟凯峰
 * @version  V1.1.0
 * @date     2024.07.07
 ******************************************************************************/

#include "..\Test\test.h"
#if TEST_OBJECT == TEST_C51XBP

/* 延时时间不准确，仅是用来测试该函数是否实现可重入，
   以验证 CosyOS 对 XBPSTACK 的支持是否正常。*/
static void delay_ms(u32 ms) reentrant
{
	while(ms--){
		u32 us = 1000;
		while(us--){
			OS_NOPx8;
		}
	}
}

/* task_1 */
uCreateTask(task_1, 1, TEST_TASKSTACKSIZE, 0, 0)
{
	static uint i = 0;
	delay_ms(500);
	printf("task_1: %u\r\n", ++i);
	uEndTasking;
}

/* task_2 */
uCreateTask(task_2, 1, TEST_TASKSTACKSIZE, 0, 0)
{
	static uint i = 0;
	delay_ms(1000);
	printf("task_2: %u\r\n", ++i);
	uEndTasking;
}

/* 定时器A中断 */
TEST_ISR_TIMA
{
	TEST_ISR_TIMA_CLR;
	delay_ms(1);
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
	printf("\x43\x6F\x73\x79\x4F\x53\x2D\x49\x49\x20\x43\x35\x31\xB2\xE2\xCA\xD4\r\n");
	printf("\x58\x42\x50\x53\x54\x41\x43\x4B\xB2\xE2\xCA\xD4\r\n");
	/* 初始化定时器A */
	TEST_TIMA_INIT;
}

#endif
