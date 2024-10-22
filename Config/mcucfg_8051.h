/**************************************************************************//**
 * @item     CosyOS-II Config
 * @file     mcucfg_8051.h
 * @brief    8051 Core Config File
 * @author   �ٿ���
 * @version  V3.0.1
 * @date     2024.07.07
 ******************************************************************************/

#ifndef __MCUCFG_8051_H
#define __MCUCFG_8051_H

///////////////////////////////////////////////////////////////////////////////

          //*** <<< Use Configuration Wizard in Context Menu >>> ***//

///////////////////////////////////////////////////////////////////////////////
// <h> ����ջ����
// <i> ����ջ����

// <o> ϵͳ�������������ջ��С��Bytes��
// <i> ϵͳ�������������ջ��С����λΪ�ֽ���
#define MCUCFG_STACKSIZE_STARTER        32

// <o> ϵͳ�������������ջ��С��Bytes��
// <i> ϵͳ�������������ջ��С����λΪ�ֽ���
#define MCUCFG_STACKSIZE_SYSIDLE        32

// <o> ����ջ�ط�������
// <0=> 0 <4=> 4 <8=> 8 <16=> 16 <32=> 32
// <i> ������Ϊ��̬����ʱ��CosyOS���Զ���������ջ�ط�����ƣ��Ե�������ջ����ķ��ա�
// <i> ͨ�����������һ���ط�������������Ч�����ط���Ĵ����������ڴ���Ƭ��
// <i> ��ֵ��ѡȡ�ʵ��ͺã������������С��
#define MCUCFG_TASKSTACKREALLOC_INC     8

// </h>
///////////////////////////////////////////////////////////////////////////////
// <h> REGBANK����
// <i> REGBANK����

// <o> REGBANK-����
// <0=> bank0 <1=> bank1 <2=> bank2 <3=> bank3
// <i> ��֪CosyOS������ʹ����һ��REGBANK��
// <i> ��Ĭ������£������δ�����ر�ĵ������������񶼽�ʹ��bank0��
#define MCUCFG_TASK_REGBANK             0

// <o> REGBANK-ϵͳ�ж�
// <0=> bank0 <1=> bank1 <2=> bank2 <3=> bank3
// <i> ����ϵͳ�ж���ʹ�õ�REGBANK��
// <i> ���ϵͳ�жϱ�����Ϊʹ�ö�����REGBANK��������ʹ�ò�ͬ��REGBANK���������л����ܽ�����������
#define MCUCFG_SYSINT_REGBANK           0

// </h>
///////////////////////////////////////////////////////////////////////////////
// <h> PendSV�ж�����
// <i> ����ѡ��һ��δʹ�õ�Ӳ���жϣ���ΪPendSV���жϣ�����ִ��ϵͳ�Ĺ������
// <i> �����ڳ�ʼ�����������ø��жϣ�ȷ�������ᱻӲ���������ж����ȼ�����Ϊ��ͼ���

// <o> �ж�������ַ
// <i> �ж�������ַ
#define MCUCFG_PENDSV_VECTORADDR        0003H

// <o> �жϿ���
// <i> ������Ӧ���ı��༭�����ж��塣
// <i> ʾ����EX0 = 1
#define mPendSV_Enable                  EX0 = 1

// <o> �жϹر�
// <i> ������Ӧ���ı��༭�����ж��塣
// <i> ʾ����EX0 = 0
#define mPendSV_Disable                 EX0 = 0

// <o> �жϴ��������жϱ�־λ��
// <i> ������Ӧ���ı��༭�����ж��塣
// <i> ʾ����IE0 = 1
#define mPendSV_Set                     IE0 = 1

// <o> �ж����㣨���жϱ�־λ��
// <i> ������Ӧ���ı��༭�����ж��壬����ʹ�û����롣
// <i> ��ʹ�ñ�־λ�ܹ����жϷ��������Ӳ���Զ����㣬�Խ����û���Ҫʡ�ԣ���ȷ����ɿ����㡣
// <i> ʾ����CLR IE0
#define mPendSV_Clear                   CLR IE0

// </h>
///////////////////////////////////////////////////////////////////////////////
// <o> PendSV_FIFO���
// <0=> 0����������PendSV_FIFO��<8=> 8�� <16=> 16�� <24=> 24�� <32=> 32�� <40=> 40��
// <i> �������ȡ���������ж��е��ù��������������жϵ�Ƶ�ʡ�
// <i> ����8051��˵��PendSV_FIFO��������Ϊ40����Ӧ���ƴ����ж��е��õĹ�����񣬱��ⲻ��Ҫ�ĵ��á�
// <i> �ɿ���PendSV_FIFO��ع��ܣ������ʷ�ϵ����ֵ�����ʵ�������ȷ���䲻�������
#define MCUCFG_PENDSVFIFO_DEPTH         16

///////////////////////////////////////////////////////////////////////////////
// <h> ��̬�ڴ�����
// <i> CosyOS��ʹ�����в����Զ���ʼ���ڴ�ء�

// <o> �ڴ��ָ��
// <i> �ڴ�ص���ʼ�ڴ��ַ
#define MCUCFG_MALLOCMEMBPTR            4096

// <o> �ڴ�ش�С��Bytes��
// <i> �ڴ�ش�С����λΪ�ֽ���
#define MCUCFG_MALLOCMEMSIZE            3584

// </h>
///////////////////////////////////////////////////////////////////////////////
// <h> �����л��ֳ�����
// <i> �����л��ֳ�����

// <q> XBPSTACK
// <i> �Ƿ����ô�ģ�Ϳ�����ջ�������л��ֳ�������
#define MCUCFG_XBPSTACK                 0

// <h> �û��Զ���
// <i> CosyOS�������л�ʱ��Ĭ���Զ�����ļĴ�����{PC,A,B,DPH,DPL,PSW,TASK-REGBANK:R0-R7,[?C_XBP]}��
// <i> ������Ҫ�������Ӷ������Ĵ������ֳ������������������ѡ�
// <i> ����ѡ����C���Ի���������д�ֳ������Ĵ��룬Ӧ�÷ǳ���

// <q> C���Ա�������
// <i> �Ƿ�����C���Ա���������
// <i> �·����������Ѿ�ֱ�Ӹ���ʾ��������/�ָ���DPH1��DPL1��DPS����ֱ�����ı��༭�������޸ļ��ɡ�
#define __MCUCFG_USERREGSAVE_C          0

#if __MCUCFG_USERREGSAVE_C == 1

// ���������ֳ�����������
// ����һ��"unsigned char"���͵����飬��β�������������";"��
#define mUserReg_C_ \
	unsigned char user_reg[3];

// ���屣���ֳ�����
// ��c���Դ��룬����Ӧ�Ĵ��������ݿ������Զ���������С�
#define mUserReg_CSave \
do{ \
	s_task_current->user_reg[0] = DPH1; \
	s_task_current->user_reg[1] = DPL1; \
	s_task_current->user_reg[2] = DPS; \
}while(false)

// ����ָ��ֳ�����
// ��c���Դ��룬���Զ��������е����ݻָ�����Ӧ�ļĴ�����
#define mUserReg_CRes \
do{ \
	DPH1  = node_news->user_reg[0]; \
	DPL1  = node_news->user_reg[1]; \
	DPS   = node_news->user_reg[2]; \
}while(false)

// �Ĵ������ݳ�ʼ��
// ��c���Դ��룬��ʼ���Զ��������еļĴ������ݡ�
#define mUserReg_CINIT \
do{ \
	node_news->user_reg[0] = DPH1; \
	node_news->user_reg[1] = DPL1; \
	node_news->user_reg[2] = DPS; \
}while(false)

#endif

// <e> ������Ա�������
// <i> �Ƿ����û�����Ա���������
// <i> �������������ѡ������� mcucfg_8051.inc �ļ���д���롣
#define __MCUCFG_USERREGSAVE_ASM        0

#if __MCUCFG_USERREGSAVE_ASM == 1

// <o> �û��Ĵ���SIZE
// <i> �����ֳ��������û��Ĵ��������ֽ�����
#define MCUCFG_USERREGSIZE              3

// <o> ��������ѡ��
// <1=> ��������һ <2=> ����������
// <i> ��������һ������PUSH/POPָ����ջ/��ջ�������򵥡������ࡢ������ƫ�͡�
// <i> ���������������û��Ĵ���������ջ֮��ֱ�Ӱ������ݣ�������Է������������Դ󣬵����ܸ�Ч��
#define MCUCFG_USERREGCONFIG            2

// �Ĵ������ݳ�ʼ��
// ��c���Դ��룬��ʼ������ջ�еļĴ������ݡ�
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
#error �жϹ������FIFO������ȣ����ô���
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
