/**************************************************************************//**
 * @item     CosyOS-II Config
 * @file     mcucfg_8051.h
 * @brief    8051 Core Config File
 * @author   迟凯峰
 * @version  V3.0.1
 * @date     2024.07.07
 ******************************************************************************/

#ifndef __MCUCFG_8051_H
#define __MCUCFG_8051_H

///////////////////////////////////////////////////////////////////////////////

          //*** <<< Use Configuration Wizard in Context Menu >>> ***//

///////////////////////////////////////////////////////////////////////////////
// <h> 任务栈配置
// <i> 任务栈配置

// <o> 系统启动任务的任务栈大小（Bytes）
// <i> 系统启动任务的任务栈大小，单位为字节数
#define MCUCFG_STACKSIZE_STARTER        32

// <o> 系统空闲任务的任务栈大小（Bytes）
// <i> 系统空闲任务的任务栈大小，单位为字节数
#define MCUCFG_STACKSIZE_SYSIDLE        32

// <o> 任务栈重分配增量
// <0=> 0 <4=> 4 <8=> 8 <16=> 16 <32=> 32
// <i> 当任务为动态创建时，CosyOS将自动启用任务栈重分配机制，以抵御任务栈溢出的风险。
// <i> 通过合理的配置一个重分配增量，可有效降低重分配的次数，减少内存碎片。
// <i> 此值的选取适当就好，不建议过大或过小。
#define MCUCFG_TASKSTACKREALLOC_INC     8

// </h>
///////////////////////////////////////////////////////////////////////////////
// <h> REGBANK配置
// <i> REGBANK配置

// <o> REGBANK-任务
// <0=> bank0 <1=> bank1 <2=> bank2 <3=> bank3
// <i> 告知CosyOS，任务使用哪一个REGBANK？
// <i> 在默认情况下，如果您未做出特别的调整，所有任务都将使用bank0。
#define MCUCFG_TASK_REGBANK             0

// <o> REGBANK-系统中断
// <0=> bank0 <1=> bank1 <2=> bank2 <3=> bank3
// <i> 配置系统中断所使用的REGBANK。
// <i> 如果系统中断被配置为使用独立的REGBANK（与任务使用不同的REGBANK），任务切换性能将显著提升。
#define MCUCFG_SYSINT_REGBANK           0

// </h>
///////////////////////////////////////////////////////////////////////////////
// <h> PendSV中断配置
// <i> 您可选择一个未使用的硬件中断，做为PendSV软中断，用于执行系统的挂起服务。
// <i> 还需在初始化钩子中配置该中断，确保它不会被硬件触发，中断优先级必须为最低级。

// <o> 中断向量地址
// <i> 中断向量地址
#define MCUCFG_PENDSV_VECTORADDR        0003H

// <o> 中断开启
// <i> 此项您应在文本编辑界面中定义。
// <i> 示例：EX0 = 1
#define mPendSV_Enable                  EX0 = 1

// <o> 中断关闭
// <i> 此项您应在文本编辑界面中定义。
// <i> 示例：EX0 = 0
#define mPendSV_Disable                 EX0 = 0

// <o> 中断触发（置中断标志位）
// <i> 此项您应在文本编辑界面中定义。
// <i> 示例：IE0 = 1
#define mPendSV_Set                     IE0 = 1

// <o> 中断清零（清中断标志位）
// <i> 此项您应在文本编辑界面中定义，必须使用汇编代码。
// <i> 即使该标志位能够在中断服务程序中硬件自动清零，仍建议用户不要省略，以确保其可靠清零。
// <i> 示例：CLR IE0
#define mPendSV_Clear                   CLR IE0

// </h>
///////////////////////////////////////////////////////////////////////////////
// <o> PendSV_FIFO深度
// <0=> 0级（不启用PendSV_FIFO）<8=> 8级 <16=> 16级 <24=> 24级 <32=> 32级 <40=> 40级
// <i> 此项参数取决于您在中断中调用挂起服务的总数及中断的频率。
// <i> 对于8051来说，PendSV_FIFO的最大深度为40，您应妥善处理中断中调用的挂起服务，避免不必要的调用。
// <i> 可开启PendSV_FIFO监控功能，监控历史上的最大值，再适当增大，以确保其不会溢出。
#define MCUCFG_PENDSVFIFO_DEPTH         16

///////////////////////////////////////////////////////////////////////////////
// <h> 动态内存设置
// <i> CosyOS会使用下列参数自动初始化内存池。

// <o> 内存池指针
// <i> 内存池的起始内存地址
#define MCUCFG_MALLOCMEMBPTR            4096

// <o> 内存池大小（Bytes）
// <i> 内存池大小，单位为字节数
#define MCUCFG_MALLOCMEMSIZE            3584

// </h>
///////////////////////////////////////////////////////////////////////////////
// <h> 任务切换现场保护
// <i> 任务切换现场保护

// <q> XBPSTACK
// <i> 是否启用大模型可重入栈的任务切换现场保护？
#define MCUCFG_XBPSTACK                 0

// <h> 用户自定义
// <i> CosyOS在任务切换时，默认自动保存的寄存器：{PC,A,B,DPH,DPL,PSW,TASK-REGBANK:R0-R7,[?C_XBP]}。
// <i> 如您需要额外增加对其它寄存器的现场保护，可配置下面的选项。
// <i> 您可选择用C语言或汇编语言来写现场保护的代码，应用非常灵活。

// <q> C语言保护方案
// <i> 是否启用C语言保护方案？
// <i> 下方各定义项已经直接给出示例（保护/恢复：DPH1、DPL1、DPS），直接在文本编辑界面中修改即可。
#define __MCUCFG_USERREGSAVE_C          0

#if __MCUCFG_USERREGSAVE_C == 1

// 定义用于现场保护的数组
// 定义一个"unsigned char"类型的数组，结尾必须加语句结束符";"。
#define mUserReg_C_ \
	unsigned char user_reg[3];

// 定义保护现场代码
// 用c语言代码，将相应寄存器的内容拷贝至自定义的数组中。
#define mUserReg_CSave \
do{ \
	s_task_current->user_reg[0] = DPH1; \
	s_task_current->user_reg[1] = DPL1; \
	s_task_current->user_reg[2] = DPS; \
}while(false)

// 定义恢复现场代码
// 用c语言代码，将自定义数组中的数据恢复至相应的寄存器。
#define mUserReg_CRes \
do{ \
	DPH1  = node_news->user_reg[0]; \
	DPL1  = node_news->user_reg[1]; \
	DPS   = node_news->user_reg[2]; \
}while(false)

// 寄存器数据初始化
// 用c语言代码，初始化自定义数组中的寄存器数据。
#define mUserReg_CINIT \
do{ \
	node_news->user_reg[0] = DPH1; \
	node_news->user_reg[1] = DPL1; \
	node_news->user_reg[2] = DPS; \
}while(false)

#endif

// <e> 汇编语言保护方案
// <i> 是否启用汇编语言保护方案？
// <i> 需先配置下面的选项，而后在 mcucfg_8051.inc 文件中写代码。
#define __MCUCFG_USERREGSAVE_ASM        0

#if __MCUCFG_USERREGSAVE_ASM == 1

// <o> 用户寄存器SIZE
// <i> 定义现场保护的用户寄存器的总字节数。
#define MCUCFG_USERREGSIZE              3

// <o> 保护方案选择
// <1=> 保护方案一 <2=> 保护方案二
// <i> 保护方案一：采用PUSH/POP指令入栈/出栈，操作简单、代码简洁、但性能偏低。
// <i> 保护方案二：在用户寄存器与任务栈之间直接搬移数据，操作相对繁琐，代码量稍大，但性能高效。
#define MCUCFG_USERREGCONFIG            2

// 寄存器数据初始化
// 用c语言代码，初始化任务栈中的寄存器数据。
#if MCUCFG_USERREGCONFIG == 1
#define mUserReg_INIT \
do{ \
	*(s_u8_t *)(node_news->bsp + (MCUCFG_TASK_REGBANK == MCUCFG_SYSINT_REGBANK ? 15 : 7) + 0) = DPH1; \
	*(s_u8_t *)(node_news->bsp + (MCUCFG_TASK_REGBANK == MCUCFG_SYSINT_REGBANK ? 15 : 7) + 1) = DPL1; \
	*(s_u8_t *)(node_news->bsp + (MCUCFG_TASK_REGBANK == MCUCFG_SYSINT_REGBANK ? 15 : 7) + 2) = DPS; \
}while(false)
#else
#define mUserReg_INIT \
do{ \
	*(s_u8_t *)(node_news->bsp + 15 + 0) = DPH1; \
	*(s_u8_t *)(node_news->bsp + 15 + 1) = DPL1; \
	*(s_u8_t *)(node_news->bsp + 15 + 2) = DPS; \
}while(false)
#endif

#endif
// </e>
// </h>
// </h>
///////////////////////////////////////////////////////////////////////////////

                //*** <<< end of configuration section >>> ***//

///////////////////////////////////////////////////////////////////////////////

#if MCUCFG_PENDSVFIFO_DEPTH != 0 \
 && MCUCFG_PENDSVFIFO_DEPTH != 8 \
 && MCUCFG_PENDSVFIFO_DEPTH != 16 \
 && MCUCFG_PENDSVFIFO_DEPTH != 24 \
 && MCUCFG_PENDSVFIFO_DEPTH != 32 \
 && MCUCFG_PENDSVFIFO_DEPTH != 40
#error 中断挂起服务FIFO队列深度，配置错误！
#endif

#if !__MCUCFG_USERREGSAVE_C
#define mUserReg_C_
#define mUserReg_CSave
#define mUserReg_CRes
#define mUserReg_CINIT
#endif

#if !__MCUCFG_USERREGSAVE_ASM
#define MCUCFG_USERREGSIZE              0
#define MCUCFG_USERREGCONFIG            0
#define mUserReg_INIT
#endif



#endif
