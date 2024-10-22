/**************************************************************************//**
 * @item     CosyOS-II Kernel
 * @file     ur_api.h
 * @brief    用户API（创建API、任务API、滴答API、中断API、PSVAPI、公共API）
 * @author   迟凯峰
 * @version  V3.0.0
 * @date     2024.06.25
 ******************************************************************************/

#ifndef __UR_API_H
#define __UR_API_H



/*
 * 声明任务
 */

/* 无私信 */
#define uExternTask(task) \
	sCSV_ExternTask(task)

#define dExternTask(task) \
	dCSV_ExternTask(task)

/* 有私信 */
#define uExternTask_Msg(task) \
	sCSV_ExternTask_Msg(task) /**< \taskmsg\params ...) >*/

#define dExternTask_Msg(task) \
	dCSV_ExternTask_Msg(task) /**< \taskmsg\params ...) >*/



/*
 * 创建任务
 */

/* 创建一般任务 */
#define uCreateTask(task, pri, tss, srt, ntm) \
	sCSV_CreateTask(task, pri, tss, srt, ntm, OS_NULL) /**[ \taskmsg\params (...) ]*//**< \taskcode {...} >*/

#define dCreateTask(task, pri, tss, srt, ntm) \
	dCSV_CreateTask(task, pri, tss, srt, ntm, OS_NULL) /**[ \taskmsg\params (...) ]*//**< \taskcode {...} >*/

/* 创建定时中断任务 */
#define uCreateTask_TimInt(tmid, arl, task, pri, tss, srt, ntm) \
	sCSV_CreateTask_TimInt(tmid, arl, task, pri, tss, srt, ntm) /**[ \taskmsg\params (...) ]*//**< \taskcode {...} >*/

#define dCreateTask_TimInt(tmid, arl, task, pri, tss, srt, ntm) \
	dCSV_CreateTask_TimInt(tmid, arl, task, pri, tss, srt, ntm) /**[ \taskmsg\params (...) ]*//**< \taskcode {...} >*/

/* 创建定时查询任务 */
#define uCreateTask_TimQry(tmid, event, arl, task, pri, tss, srt, ntm) \
	sCSV_CreateTask_TimQry(tmid, event, arl, task, pri, tss, srt, ntm) /**[ \taskmsg\params (...) ]*//**< \taskcode {...} >*/

#define dCreateTask_TimQry(tmid, event, arl, task, pri, tss, srt, ntm) \
	dCSV_CreateTask_TimQry(tmid, event, arl, task, pri, tss, srt, ntm) /**[ \taskmsg\params (...) ]*//**< \taskcode {...} >*/



/*
 * 创建钩子
 */

/* 创建定时中断钩子 */
#define uCreateHook_TimInt(tmid, arl, hook) \
	sCSV_CreateHook_TimInt(tmid, arl, hook) /**< \hookcode {...} >*/

/* 创建定时查询钩子 */
#define uCreateHook_TimQry(tmid, event, arl, hook) \
	sCSV_CreateHook_TimQry(tmid, event, arl, hook) /**< \hookcode {...} >*/



/*
 * 启动任务
 */

/* 启动任务 */
#define uStartTask(task, status) sUSV_StartTask(&u_taskhand_##task, (s_voidvoid_tfp)task, status) /** \return ecode */

/* 启动任务并就绪 */
#define uStartTask_Ready(task)   uStartTask(task, OS_STATUS_READY) /** \return ecode */

/* 启动任务并挂起 */
#define uStartTask_Suspend(task) uStartTask(task, OS_STATUS_SUSPENDED) /** \return ecode */



/*
 * 操作任务
 */

/* 恢复任务 */
#define uResumeTask(task)        sUSV_ResumeTask(u_taskhandle_##task) /** \return ecode */
#define tResumeTask(task)        sTSV_ResumeTask(u_taskhandle_##task) /** \return ecode */
#define iResumeTask(task)        sPSV_ResumeTask(u_taskhandle_##task)
#define pResumeTask(task)        sPS2_ResumeTask(u_taskhandle_##task)

/* 挂起任务 */
#define uSuspendTask(task)       sUSV_SuspendTask(u_taskhandle_##task) /** \return ecode */
#define tSuspendTask(task)       sTSV_SuspendTask(u_taskhandle_##task) /** \return ecode */
#define iSuspendTask(task)       sPSV_SuspendTask(u_taskhandle_##task)
#define pSuspendTask(task)       sPS2_SuspendTask(u_taskhandle_##task)

/* 删除任务 */
#define uDeleteTask(task)        sUSV_DeleteTask(u_taskhandle_##task) /** \return ecode */
#define tDeleteTask(task)        sTSV_DeleteTask(u_taskhandle_##task) /** \return ecode */
#define iDeleteTask(task)        sPSV_DeleteTask(u_taskhandle_##task)
#define pDeleteTask(task)        sPS2_DeleteTask(u_taskhandle_##task)

/* 设置任务优先级 */
#define uSetTaskPri(task, pri)   sUSV_SetTaskPri(u_taskhandle_##task, pri) /** \return ecode */
#define tSetTaskPri(task, pri)   sTSV_SetTaskPri(u_taskhandle_##task, pri) /** \return ecode */
#define iSetTaskPri(task, pri)   sPSV_SetTaskPri(u_taskhandle_##task, pri)
#define pSetTaskPri(task, pri)   sPS2_SetTaskPri(u_taskhandle_##task, pri)



/*
 * 操作自身任务
 */

/* 设置自身任务优先级 */
#define uSetTaskingPri(pri)      sUSV_SetTaskPri(s_task_current, pri) /** \return ecode */

/* 挂起自身任务 */
#define uSuspendTasking          sUSV_SuspendTask(s_task_current) /** \return ecode */

/* 删除自身任务 */
#define uDeleteTasking           sUSV_DeleteTask(s_task_current) /** \return ecode */

/* 恢复指定任务并挂起自身任务 */
#define uResumeSuspend(task)     sUSV_ResumeSuspend(u_taskhandle_##task) /** \return ecode */

/* 结束自身任务 */
#define uEndTasking              }while(true)



/*
 * 设置阻塞（时间）
 */

/* 滴答周期 */
#define uSetBlock_tc(task, tc)   sUSV_SetBlock(u_taskhandle_##task, tc) /** \return ecode */
#define tSetBlock_tc(task, tc)   sTSV_SetBlock(u_taskhandle_##task, tc) /** \return ecode */
#define iSetBlock_tc(task, tc)   sPSV_SetBlock(u_taskhandle_##task, tc)
#define pSetBlock_tc(task, tc)   sPS2_SetBlock(u_taskhandle_##task, tc)

/* 毫秒 */
#define uSetBlock_ms(task, ms)   uSetBlock_tc(task, (1000UL * (ms)) / SYSCFG_SYSTICKCYCLE) /** \return ecode */
#define tSetBlock_ms(task, ms)   tSetBlock_tc(task, (1000UL * (ms)) / SYSCFG_SYSTICKCYCLE) /** \return ecode */
#define iSetBlock_ms(task, ms)   iSetBlock_tc(task, (1000UL * (ms)) / SYSCFG_SYSTICKCYCLE)
#define pSetBlock_ms(task, ms)   pSetBlock_tc(task, (1000UL * (ms)) / SYSCFG_SYSTICKCYCLE)

/* 秒钟 */
#define uSetBlock_s(task, s)     uSetBlock_ms(task, 1000UL * (s)) /** \return ecode */
#define tSetBlock_s(task, s)     tSetBlock_ms(task, 1000UL * (s)) /** \return ecode */
#define iSetBlock_s(task, s)     iSetBlock_ms(task, 1000UL * (s))
#define pSetBlock_s(task, s)     pSetBlock_ms(task, 1000UL * (s))

/* 分钟 */
#define uSetBlock_m(task, m)     uSetBlock_s(task, 60UL * (m)) /** \return ecode */
#define tSetBlock_m(task, m)     tSetBlock_s(task, 60UL * (m)) /** \return ecode */
#define iSetBlock_m(task, m)     iSetBlock_s(task, 60UL * (m))
#define pSetBlock_m(task, m)     pSetBlock_s(task, 60UL * (m))

/* 小时 */
#define uSetBlock_h(task, h)     uSetBlock_m(task, 60UL * (h)) /** \return ecode */
#define tSetBlock_h(task, h)     tSetBlock_m(task, 60UL * (h)) /** \return ecode */
#define iSetBlock_h(task, h)     iSetBlock_m(task, 60UL * (h))
#define pSetBlock_h(task, h)     pSetBlock_m(task, 60UL * (h))



/*
 * 清除阻塞（状态）
 */

#define uClearBlock(task)        sUSV_ClearBlock(u_taskhandle_##task) /** \return ecode */
#define tClearBlock(task)        sTSV_ClearBlock(u_taskhandle_##task) /** \return ecode */
#define iClearBlock(task)        sPSV_ClearBlock(u_taskhandle_##task)
#define pClearBlock(task)        sPS2_ClearBlock(u_taskhandle_##task)



/*
 * 自身任务延时
 */

/* 延时滴答周期 */
#define uDelay_tc(tc) sUSV_Delay(tc)

/* 延时毫秒 */
#define uDelay_ms(ms) uDelay_tc((1000UL * (ms)) / SYSCFG_SYSTICKCYCLE)

/* 延时秒钟 */
#define uDelay_s(s)   uDelay_ms(1000UL * (s))

/* 延时分钟 */
#define uDelay_m(m)   uDelay_s(60UL * (m))

/* 延时小时 */
#define uDelay_h(h)   uDelay_m(60UL * (h))



/*
 * 定时中断
 */

/* 定时中断滴答周期 */
#define uTimInt_tc(tmid, tc) sUSV_TimInt(tmid, tc)
#define tTimInt_tc(tmid, tc) sTSV_TimInt(tmid, tc)
#define iTimInt_tc(tmid, tc) sPSV_TimInt(tmid, tc)

/* 定时中断毫秒 */
#define uTimInt_ms(tmid, ms) uTimInt_tc(tmid, (1000UL * (ms)) / SYSCFG_SYSTICKCYCLE)
#define tTimInt_ms(tmid, ms) tTimInt_tc(tmid, (1000UL * (ms)) / SYSCFG_SYSTICKCYCLE)
#define iTimInt_ms(tmid, ms) iTimInt_tc(tmid, (1000UL * (ms)) / SYSCFG_SYSTICKCYCLE)

/* 定时中断秒钟 */
#define uTimInt_s(tmid, s)   uTimInt_ms(tmid, 1000UL * (s))
#define tTimInt_s(tmid, s)   tTimInt_ms(tmid, 1000UL * (s))
#define iTimInt_s(tmid, s)   iTimInt_ms(tmid, 1000UL * (s))

/* 定时中断分钟 */
#define uTimInt_m(tmid, m)   uTimInt_s(tmid, 60UL * (m))
#define tTimInt_m(tmid, m)   tTimInt_s(tmid, 60UL * (m))
#define iTimInt_m(tmid, m)   iTimInt_s(tmid, 60UL * (m))

/* 定时中断小时 */
#define uTimInt_h(tmid, h)   uTimInt_m(tmid, 60UL * (h))
#define tTimInt_h(tmid, h)   tTimInt_m(tmid, 60UL * (h))
#define iTimInt_h(tmid, h)   iTimInt_m(tmid, 60UL * (h))

/* 终止定时中断 */
#define uTimInt_Cancel(tmid) uTimInt_tc(tmid, 0)
#define tTimInt_Cancel(tmid) tTimInt_tc(tmid, 0)
#define iTimInt_Cancel(tmid) iTimInt_tc(tmid, 0)

/* 定时中断定时器自动重装载 */
#define xTimInt_AutoReload(tmid, arl) s_timint_autoreload[tmid] = arl



/*
 * 定时查询
 */

/* 定时查询滴答周期 */
#define uTimQry_tc(tmid, tc) sUSV_TimQry(tmid, tc)
#define tTimQry_tc(tmid, tc) sTSV_TimQry(tmid, tc)
#define iTimQry_tc(tmid, tc) sPSV_TimQry(tmid, tc)

/* 定时查询毫秒 */
#define uTimQry_ms(tmid, ms) uTimQry_tc(tmid, (1000UL * (ms)) / SYSCFG_SYSTICKCYCLE)
#define tTimQry_ms(tmid, ms) tTimQry_tc(tmid, (1000UL * (ms)) / SYSCFG_SYSTICKCYCLE)
#define iTimQry_ms(tmid, ms) iTimQry_tc(tmid, (1000UL * (ms)) / SYSCFG_SYSTICKCYCLE)

/* 定时查询秒钟 */
#define uTimQry_s(tmid, s)   uTimQry_ms(tmid, 1000UL * (s))
#define tTimQry_s(tmid, s)   tTimQry_ms(tmid, 1000UL * (s))
#define iTimQry_s(tmid, s)   iTimQry_ms(tmid, 1000UL * (s))

/* 定时查询分钟 */
#define uTimQry_m(tmid, m)   uTimQry_s(tmid, 60UL * (m))
#define tTimQry_m(tmid, m)   tTimQry_s(tmid, 60UL * (m))
#define iTimQry_m(tmid, m)   iTimQry_s(tmid, 60UL * (m))

/* 定时查询小时 */
#define uTimQry_h(tmid, h)   uTimQry_m(tmid, 60UL * (h))
#define tTimQry_h(tmid, h)   tTimQry_m(tmid, 60UL * (h))
#define iTimQry_h(tmid, h)   iTimQry_m(tmid, 60UL * (h))

/* 终止定时查询 */
#define uTimQry_Cancel(tmid) uTimQry_tc(tmid, ~0)
#define tTimQry_Cancel(tmid) tTimQry_tc(tmid, ~0)
#define iTimQry_Cancel(tmid) iTimQry_tc(tmid, ~0)

/* 定时查询定时器自动重装载 */
#define xTimQry_AutoReload(tmid, arl) s_timqry_autoreload[tmid] = arl



/*
 * 互斥信号量
 */

/* 声明 */
#define uExternMut(mut) \
	sCSV_ExternMut(mut)

/* 创建 */
#define uCreateMut(mut) \
	sCSV_CreateMut(mut)

/* 获取 */
#define uTakeMut(mut, tc) sUSV_TakeMut(mut, tc) /** \return result */

/* 归还 */
#define uBackMut(mut)     sUSV_BackMut(mut)



/*
 * 二值信号量
 */

/* 声明 */
#define uExternBin(bin) \
	sCSV_ExternBin(bin)

/* 创建 */
#define uCreateBin(bin, init) \
	sCSV_CreateBin(bin, init)

/* 上锁 */
#define uLockBin(bin)     bin.binary = false
#define tLockBin(bin)     bin.binary = false
#define iLockBin(bin)     sPSV_WriteBin(bin, false)
#define pLockBin(bin)     sPS2_WriteBin(bin, false)

/* 等待 */
#define uWaitBin(bin, tc) sUSV_WaitBin(bin, tc) /** \return result */

/* 给予 */
#define uGiveBin(bin)     sUSV_GiveBin(bin)
#define tGiveBin(bin)     bin.binary = true
#define iGiveBin(bin)     sPSV_WriteBin(bin, true)
#define pGiveBin(bin)     sPS2_WriteBin(bin, true)

/* 获取 */
#define uTakeBin(bin, tc) sUSV_TakeBin(bin, tc) /** \return result */
#define tTakeBin(bin)     sTSV_TakeBin(bin)     /** \return result */
#define iTakeBin(bin)     sISV_TakeBin(bin)     /** \return result */

/* 归还 */
#define uBackBin(bin)     uGiveBin(bin)
#define tBackBin(bin)     bin.binary = true
#define iBackBin(bin) \
do{ \
	bin.binary = true; \
	iGiveBin(bin); \
}while(false)



/*
 * 计数信号量
 */

/* 声明 */
#define uExternSem(sem) \
	sCSV_ExternSem(sem)

/* 创建 */
#define uCreateSem(sem, init, max) \
	sCSV_CreateSem(sem, init, max)

/* 给予 */
#define uGiveSem(sem)     sUSV_GiveSem(sem)
#define tGiveSem(sem)     sTSV_GiveSem(sem)
#define iGiveSem(sem)     sPSV_GiveSem(sem)

/* 获取 */
#define uTakeSem(sem, tc) sUSV_TakeSem(sem, tc) /** \return result */
#define tTakeSem(sem)     sTSV_TakeSem(sem)     /** \return result */

/* 归还 */
#define uBackSem(sem)     uGiveSem(sem)
#define tBackSem(sem)     tGiveSem(sem)



/*
 * 私信
 */

/* 接收私信 */
#define uRecvTaskMsg(tc)   sUSV_RecvTaskMsg(tc) /** \return result */

/* 发送私信 */
#define uSendTaskMsg(task) sUSV_SendTaskMsg(task) /**< \taskmsg\params ...) >*/
#define tSendTaskMsg(task) sTSV_SendTaskMsg(task) /**< \taskmsg\params ...) >*/

/* API简化 */
#define uRecvTM uRecvTaskMsg
#define uSendTM uSendTaskMsg
#define tSendTM tSendTaskMsg



/*
 * 飞信
 */

/* 声明信箱 */
#define uExternFetion(tbox) \
	sCSV_ExternFetion(tbox)

/* 创建信箱 */
#define uCreateFetion(tbox) \
	sCSV_CreateFetion(tbox)

/* 接收飞信 */
#define uRecvFetion(tbox, tc)   sUSV_RecvFetion(tbox, tc) /** \return fetion */
#define tRecvFetion(tbox)       sTSV_RecvFetion(tbox)     /** \return fetion */
#define iRecvFetion(tbox)       sISV_RecvFetion(tbox)     /** \return fetion */

/* 发送飞信 */
#define uSendFetion(tbox, tion) sUSV_SendFetion(tbox, tion)
#define tSendFetion(tbox, tion) tbox.fetion = tion
#define iSendFetion(tbox, tion) sPSV_SendFetion(tbox, tion)

/* API简化 */
#define uRecvFet uRecvFetion
#define tRecvFet tRecvFetion
#define iRecvFet iRecvFetion
#define uSendFet uSendFetion
#define tSendFet tSendFetion
#define iSendFet iSendFetion



/*
 * 消息邮箱
 */

/* 声明邮箱 */
#define uExternMailbox(mbox) \
	sCSV_ExternMailbox(mbox)

/* 创建邮箱 */
#define uCreateMailbox(mbox) \
	sCSV_CreateMailbox(mbox)

/* 接收邮件 */
#define uRecvMail(mbox, tc)   sUSV_RecvMail(mbox, tc) /** \return mail-ptr */
#define tRecvMail(mbox)       sTSV_RecvMail(mbox)     /** \return mail-ptr */
#define iRecvMail(mbox)       sISV_RecvMail(mbox)     /** \return mail-ptr */

/* 发送邮件 */
#define uSendMail(mbox, mail) sUSV_SendMail(mbox, mail)
#define tSendMail(mbox, mail) sTSV_SendMail(mbox, mail)
#define iSendMail(mbox, mail) sPSV_SendMail(mbox, mail)



/*
 * 消息队列
 */

/* 声明静态消息队列 */
#define uExternQueue(que) \
	sCSV_ExternQueue(que)

/* 声明动态消息队列 */
#define dExternQueue(que) \
	dCSV_ExternQueue(que)

/* 创建静态消息队列 */
#define uCreateQueue(que, mode, len) \
	sCSV_CreateQueue(que, mode, len)

/* 创建动态消息队列 */
#define dCreateQueue(que, mode, len) \
	dCSV_CreateQueue(que, mode, len)

/* 接收消息 */
#define uRecvMsg(que, tc)  sUSV_RecvMsg((s_queue_tsp)&que, tc) /** \return msg-ptr */
#define tRecvMsg(que)      sTSV_RecvMsg((s_queue_tsp)&que)     /** \return msg-ptr */
#define iRecvMsg(que)      sISV_RecvMsg((s_queue_tsp)&que)     /** \return msg-ptr */

/* 发送消息 */
#define uSendMsg(que, msg) sUSV_SendMsg((s_queue_tsp)&que, msg) /** \return ecode */
#define tSendMsg(que, msg) sTSV_SendMsg((s_queue_tsp)&que, msg) /** \return ecode */
#define iSendMsg(que, msg) sPSV_SendMsg((s_queue_tsp)&que, msg)



/*
 * 事件标志组
 */

/* 声明标志组 */
#define uExternFlagGroup(group, bits) \
	sCSV_ExternFlagGroup(group, bits)

/* 定义标志位 */
#define uDefFlagBit(bit) sDefBitField(bit)

/* 定义空位 */
#define uDefVoidBits(nvb) sDefVoidBits(nvb)

/* 创建标志组 */
#define uCreateFlagGroup(group) \
	sCSV_CreateFlagGroup(group) /**[ \init = {...} ]*/

/* 等待标志组 */
#define uWaitFlagGroup(group, tc)   sUSV_WaitFlagGroup(group, tc) /** \return result */

/* 查询标志组 */
#define uQueryFlagGroup(group)      sUSV_QueryFlagGroup(group) /** \return result */
#define tQueryFlagGroup(group)      sTSV_QueryFlagGroup(group) /** \return result */
#define iQueryFlagGroup(group)      sISV_QueryFlagGroup(group) /** \return result */

/* 清除标志组 */
#define uClearFlagGroup(group)      sUSV_ClearFlagGroup(group)
#define tClearFlagGroup(group)      sTSV_ClearFlagGroup(group)
#define iClearFlagGroup(group)      sPSV_ClearFlagGroup(group)

/* 查询标志位（任务、滴答、中断中，均直接查询）*/

/* 设置标志位（滴答中直接设置，任务中也可在任务临界区中设置）*/
#define uSetFlagBit(group, bit)     sUSV_WriteFlagBit(group, bit, true)
#define iSetFlagBit(group, bit)     iSetFlagBits(group, 1) bit)

/* 清除标志位（滴答中直接清除，任务中也可在任务临界区中清除）*/
#define uClearFlagBit(group, bit)   sUSV_WriteFlagBit(group, bit, false)
#define iClearFlagBit(group, bit)   iClearFlagBits(group, 1) bit)

/* 设置多标志位（滴答中直接设置，任务中在任务临界区中设置）*/
#define iSetFlagBits(group, nbit)   sPSV_WriteFlagBits(group, +, nbit) /**< \bits ...) >*/

/* 清除多标志位（滴答中直接清除，任务中在任务临界区中清除）*/
#define iClearFlagBits(group, nbit) sPSV_WriteFlagBits(group, -, nbit) /**< \bits ...) >*/



/*
 * 全局变量访问
 */

/* 更新全局变量副本 */
#define uUpdateCopy(code)        sUpdateCopy(code)
#define tUpdateCopy(code)        sUpdateCopy(code)

/* 读哪一个全局变量？（返回0读正本，返回1读副本）*/
#define iWhichGVarToRead         !s_sign_updatecopy /** \return bool */

/* 写全局变量 */
#define iWriteGVar(gv, lv)       sPSV_WriteGVar((void *)&gv, (void *)&lv, sizeof(gv))

/* 写全局数组 */
#define iWriteGAry(gp, lp, size) sPSV_WriteGVar(gp, lp, size)

/* 写全局字符串 */
#define iWriteGStr(gs, ls)       sPSV_WriteGVar(gs, ls, 0)

/* 挂起服务调用 */
#define iPendSVC(fp)             sPSV_PendSVC(fp)

/* 发送信号 */
#define iSendSign(sign)          sPSV_SendSign(sign)



/*
 * 软件RTC
 */

/* 获取时间 */
#define uGetTime(t) sUSV_GetTime(t) /** \return t */
#define tGetTime(t) sTSV_GetTime(t) /** \return t */
#define iGetTime(t) sISV_GetTime(t) /** \return t */

/* 设置时间 */
#define uSetTime(t) sUSV_SetTime(t)
#define tSetTime(t) sTSV_SetTime(t)
#define iSetTime(t) sPSV_SetTime(t)



/*
 * 进程内存
 */

/* malloc */
#define uMalloc(size)        sUSV_Malloc(size) /** \return ptr */

/* calloc */
#define uCalloc(nmemb, size) sUSV_Calloc(nmemb, size) /** \return ptr */

/* realloc */
#define uRealloc(p, size)    sUSV_Realloc(p, size) /** \return ptr */

/* free */
#define uFree(p)             sUSV_Free(p)



/*
 * 线程内存
 */

/* 创建线程内存池 */
#define uCreateMempool     sCSV_CreateMempool

/* 初始化线程内存池 */
#define uInitMempool(size) sUSV_InitMempool(size) /** \return result */

/* 线程内存分配 */
#define uTalloc(size)      sUSV_Talloc(size) /** \return ptr */

/* 释放线程内存池 */
#define uFreeMempool       sUSV_FreeMempool



/*
 * DEBUG
 */

#define iDebugSend s_sign_debug_send = true
#define iDebugRecv s_sign_debug_recv = true

#if false
/* 应用示例 - 8051/80251 UART1 */
void UART1_Isr(void) interrupt 4
{
	if(TI){
		TI = 0;
		if(*s_debug_sendptr){
			SBUF = *s_debug_sendptr++;
		}
		else{
			iDebugSend;
		}
	}
	if(RI){
		RI = 0;
		if(s_debug_recvptr - s_debug_recvbuff < OS_DEBUGRECVBUFFSIZE - 1){
			*s_debug_recvptr++ = SBUF;
		}
		iDebugRecv;
	}
}

/* 应用示例 - STM32 USART1 */
void USART1_IRQHandler(void)
{
	if(USART_GetITStatus(USART1, USART_IT_TXE) != RESET){
		if(*s_debug_sendptr){
			USART_SendData(USART1, *s_debug_sendptr++);
		}
		else{
			USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
			iDebugSend;
		}
	}
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET){
		if(s_debug_recvptr - s_debug_recvbuff < OS_DEBUGRECVBUFFSIZE - 1){
			*s_debug_recvptr++ = USART_ReceiveData(USART1);
		}
		iDebugRecv;
	}
}

#endif



/*
 * 任务调度
 * 由用户手动触发的临时性的任务调度
 */

#define uTaskSchedule  sUSV_TaskSchedule
#define iTaskSchedule  sISV_TaskSchedule



/*
 * 任务临界区
 * 任务级的临界区保护，仅关闭SysTick、PendSV，仅支持在任务中调用。
 */

/* 进入临界区 */
#define uEnterCritical su_enter_critical()

/* 退出临界区 */
#define uExitCritical  su_exit_critical()



/*
 * 全局临界区
 * 系统级的临界区保护，通常会关闭总中断，支持在任意处调用。
 * CosyOS内核中从来不会进入全局临界区，提供此项服务只是为了便于用户对全局公共资源和程序过程的保护。
 */

/* [ARM] */#if MCUCFG_ISA == __ARM__

/* 方法一：操作PRIMASK */
#define xDisableIRQ         mxDisableIRQ         /* 进入全局临界区 *//** \return oldirq */
#define xResumeIRQ(oldirq)  mxResumeIRQ(oldirq)  /* 退出全局临界区 */

/* 方法二：操作FAULTMASK */
#define xDisableFIQ         mxDisableFIQ         /* 进入全局临界区 *//** \return oldfiq */
#define xResumeFIQ(oldfiq)  mxResumeFIQ(oldfiq)  /* 退出全局临界区 */

/* 方法三：操作BASEPRI */
#define xMaskingPRI(newpri) mxMaskingPRI(newpri) /* 进入全局临界区 *//** \return oldpri */
#define xResumePRI(oldpri)  mxResumePRI(oldpri)  /* 退出全局临界区 */

/* [!ARM] */#else

/* 操作EA */
#define xDisableIRQ         mxDisableIRQ         /* 进入全局临界区 */
#define xResumeIRQ          mxResumeIRQ          /* 退出全局临界区 */

#endif



#endif
