/**************************************************************************//**
 * @item     CosyOS-II 行为同步测试
 * @file     test_xwtb_mailbox.c
 * @brief    邮箱测试
 * @detail   通过邮件的接收/发送，实现中断对任务、任务对任务的行为同步（消息同步）。
 * @service  uRecvMail(mbox, tick); 任务中接收邮件
             uSendMail(mbox, mail); 任务中发送邮件
             iSendMail(mbox, mail); 中断中发送邮件
 * @author   迟凯峰
 * @version  V1.1.0
 * @date     2024.06.25
 ******************************************************************************/

#include "..\Test\test.h"
#if TEST_OBJECT == TEST_XWTB_MAILBOX

/*
 * 用户定义
 */

// 测试类型（0：传输指针，1：传输数据）
#define TEST_TYPE     0

////////////////////////////////////////////////////////////////////////////////

/* 创建邮箱 */
uCreateMailbox(mbox_1);
uCreateMailbox(mbox_2);

/**
 * \brief    task_1
 * \detail   处理更为耗时的事务。
 */
uCreateTask(task_1, 1, TEST_TASKSTACKSIZE, 0, 0)
{
	#if TEST_TYPE == 0
	uint *m = (uint *)uRecvMail(mbox_1, ~0);
	if(m){
		TEST_GPIO_P01_RESET; // 任务被同步运行后，需立即关闭测量信号
		printf("task_2 -> task_1: %u\r\n", *m); // 打印 task_2 发送至 task_1 的邮件
		led_task_1();
	}
	#else
	uint m = (uint)uRecvMail(mbox_1, ~0);
	if(m){
		TEST_GPIO_P01_RESET;
		printf("task_2 -> task_1: %u\r\n", m);
		led_task_1();
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
	uint *m = (uint *)uRecvMail(mbox_2, ~0);
	if(m){
		TEST_GPIO_P02_RESET; // 任务被同步运行后，需立即关闭测量信号
		printf("TIME_A -> task_2: %u\r\n", *m); // 打印 TIME_A 发送至 task_2 的邮件
		led_task_2();
		
		TEST_GPIO_P01_SET; // 先开启测量信号，再同步
		uSendMail(mbox_1, m); /*!< 同步至 task_1 */
	}
	#else
	uint m = (uint)uRecvMail(mbox_2, ~0);
	if(m){
		TEST_GPIO_P02_RESET;
		printf("TIME_A -> task_2: %u\r\n", m);
		led_task_2();
		
		TEST_GPIO_P01_SET;
		uSendMail(mbox_1, (void *)m);
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
	static uint m = 0;
	TEST_ISR_TIMA_CLR;
	
	#if TEST_TYPE == 0
	m++;
	TEST_GPIO_P02_SET; // 先开启测量信号，再同步
	iSendMail(mbox_2, &m); /*!< 同步至 task_2 */
	#else
	if(!++m) m++;
	TEST_GPIO_P02_SET;
	iSendMail(mbox_2, (void *)m);
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
	printf("\xD3\xCA\xCF\xE4\xB2\xE2\xCA\xD4\r\n");
	/* 初始化定时器A */
	TEST_TIMA_INIT;
}

#endif
