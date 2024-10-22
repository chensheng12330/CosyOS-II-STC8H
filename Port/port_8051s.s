;*******************************************************************************
;* @item     CosyOS-II Port
;* @file     port_8051s.s
;* @brief    8051 Core Port File
;* @author   迟凯峰
;* @version  V3.0.1
;* @date     2024.07.07
;*******************************************************************************
;
$NOMOD51
$INCLUDE (..\Config\syscfg.h)
IF SYSCFG_MCUCORE == 8051
$INCLUDE (..\Config\mcucfg_8051.h)
$INCLUDE (..\Config\mcucfg_8051.inc)
;
;///////////////////////////////////////////////////////////////////////////////
;
; 用户定义
;
; 包含头文件 或 直接定义寄存器，具体包括：
; 1、mcucfg_8051.h -> PendSV中断配置 -> 中断清零，涉及的寄存器；
; 2、用户自定义任务切换现场保护，汇编语言保护方案，涉及的寄存器；
;
; 通常，直接包含标准头文件（h、inc）即可。
; 如果包含h文件导致编译报错，说明h文件中存在汇编器不能识别的C语言语法，
; 您可包含inc文件或重写新的h文件，也可在此处直接定义寄存器。
;
$INCLUDE (STC8H.H)
;
;///////////////////////////////////////////////////////////////////////////////

				NAME	PORT_8051S

;///////////////////////////////////////////////////////////////////////////////

; OS_PendSV_Handler
; PendSV中断服务函数

?PR?OS_PendSV_Handler?PORT_8051S	SEGMENT	CODE
?STACK			SEGMENT	IDATA
				EXTRN	DATA (?C_XBP)
				EXTRN	CODE (sPendSV_Handler)
				EXTRN	DATA (s_task_current)
				EXTRN	DATA (s_task_news)
				EXTRN	DATA (m_bsp_add)
				IF SYSCFG_DEBUGGING == __ENABLED__
				EXTRN	BIT  (s_sign_taskmgr)
				EXTRN	DATA (s_taskstacklen)
				ENDIF
				IF SYSCFG_TASKPC_MONITOR == __ENABLED__
				EXTRN	DATA (s_pc)
				ENDIF

				CSEG	AT	MCUCFG_PENDSV_VECTORADDR
				LJMP	OS_PendSV_Handler

				RSEG	?PR?OS_PendSV_Handler?PORT_8051S
				USING	MCUCFG_TASK_REGBANK
OS_PendSV_Handler:

				PUSH	ACC
				PUSH	B
				PUSH	DPH
				PUSH	DPL
				PUSH	PSW
IF MCUCFG_TASK_REGBANK <> MCUCFG_SYSINT_REGBANK
				MOV		PSW, #MCUCFG_SYSINT_REGBANK*8
ELSE
				PUSH	AR0
				PUSH	AR1
				PUSH	AR2
				PUSH	AR3
				PUSH	AR4
				PUSH	AR5
				PUSH	AR6
				PUSH	AR7
ENDIF
				mPendSV_Clear
; s_taskstacklen
IF SYSCFG_DEBUGGING == __ENABLED__
				CLR		C
				MOV		A, SP
				SUBB	A, #?STACK-1
	IF MCUCFG_TASK_REGBANK <> MCUCFG_SYSINT_REGBANK
		IF MCUCFG_XBPSTACK == __ENABLED__
				ADD		A, #MCUCFG_USERREGSIZE+8+2
		ELSE
				ADD		A, #MCUCFG_USERREGSIZE+8
		ENDIF
	ELSE
		IF MCUCFG_XBPSTACK == __ENABLED__
				ADD		A, #MCUCFG_USERREGSIZE+2
		ELSEIF MCUCFG_USERREGSIZE
				ADD		A, #MCUCFG_USERREGSIZE
		ENDIF
	ENDIF
				MOV		s_taskstacklen, A
ENDIF
				LCALL	sPendSV_Handler

				CLR		C
				MOV		A, R7
				JNZ		?PROTECTING
				LJMP	RETURN
?PROTECTING:	DEC		A
				JZ		RESTORE

; 任务PC监控
IF SYSCFG_TASKPC_MONITOR == __ENABLED__
				JNB		s_sign_taskmgr, PROTECTING
				MOV		A, SP
	IF MCUCFG_TASK_REGBANK == MCUCFG_SYSINT_REGBANK
				SUBB	A, #15-1
	ELSE
				SUBB	A, #7-1
	ENDIF
				MOV		R0, A
				MOV		s_pc+1, @R0
				INC		R0
				MOV		s_pc, @R0
ENDIF

; 保护现场
PROTECTING:
IF MCUCFG_USERREGCONFIG == 1					; SAVE USERREG: 保护方案一
				mUserReg_PUSH
ENDIF

				MOV		DPH, s_task_current		; Save the STACKLEN to s_task_current->stacklen.
				MOV		DPL, s_task_current+1
				MOV		A, SP
				SUBB	A, #?STACK-1
				MOV		R1, A
				MOVX	@DPTR, A

				MOV		A, s_task_current+1		; Get the address of s_task_current->bsp.
				ADD		A, m_bsp_add
				MOV		DPL, A
				JNC		GETCURBSP
				INC		DPH
				CLR		C
GETCURBSP:		MOVX	A, @DPTR				; Get the s_task_current->bsp.
				MOV		R0, A
				INC		DPTR
				MOVX	A, @DPTR
				MOV		DPH, R0
				MOV		DPL, A

				MOV		R0, #?STACK				; Get the start address of STACK.

SAVESTACK:		MOV		A, @R0					; SAVE STACK
				MOVX	@DPTR, A
				INC		R0
				INC		DPTR
				DJNZ	R1, SAVESTACK

IF MCUCFG_TASK_REGBANK <> MCUCFG_SYSINT_REGBANK	; SAVE TASK-REGBANK
				MOV		A, AR0
				MOVX	@DPTR, A
				INC		DPTR
				MOV		A, AR1
				MOVX	@DPTR, A
				INC		DPTR
				MOV		A, AR2
				MOVX	@DPTR, A
				INC		DPTR
				MOV		A, AR3
				MOVX	@DPTR, A
				INC		DPTR
				MOV		A, AR4
				MOVX	@DPTR, A
				INC		DPTR
				MOV		A, AR5
				MOVX	@DPTR, A
				INC		DPTR
				MOV		A, AR6
				MOVX	@DPTR, A
				INC		DPTR
				MOV		A, AR7
				MOVX	@DPTR, A
	IF MCUCFG_USERREGCONFIG == 2 || MCUCFG_XBPSTACK == __ENABLED__
				INC		DPTR
	ENDIF
ENDIF

IF MCUCFG_USERREGCONFIG == 2					; SAVE USERREG: 保护方案二
				mUserReg_Save
	IF MCUCFG_XBPSTACK == __ENABLED__
				INC		DPTR
	ENDIF
ENDIF

IF MCUCFG_XBPSTACK == __ENABLED__				; SAVE ?C_XBP
				MOV		A, ?C_XBP
				MOVX	@DPTR, A
				INC		DPTR
				MOV		A, ?C_XBP+1
				MOVX	@DPTR, A
ENDIF

; 恢复现场
RESTORE:		MOV		s_task_current, s_task_news
				MOV		s_task_current+1, s_task_news+1

				MOV		DPH, s_task_current		; Get the STACKLEN from s_task_current->stacklen.
				MOV		DPL, s_task_current+1
				MOVX	A, @DPTR
				MOV		R1, A

				MOV		A, s_task_current+1		; Get the address of s_task_current->bsp.
				ADD		A, m_bsp_add
				MOV		DPL, A
				JNC		GETNEWBSP
				INC		DPH
				CLR		C
GETNEWBSP:		MOVX	A, @DPTR				; Get the s_task_current->bsp.
				MOV		R0, A
				INC		DPTR
				MOVX	A, @DPTR
				MOV		DPH, R0
				MOV		DPL, A

				MOV		SP, #?STACK-1			; Get the start address of STACK, minus one more.

RESTORESTACK:	MOVX	A, @DPTR				; RESTORE STACK
				PUSH	ACC
				INC		DPTR
				DJNZ	R1, RESTORESTACK

IF MCUCFG_USERREGCONFIG == 1					; RESTORE USERREG: 保护方案一
				mUserReg_POP
ENDIF

IF MCUCFG_TASK_REGBANK <> MCUCFG_SYSINT_REGBANK	; RESTORE TASK-REGBANK
				MOVX	A, @DPTR
				MOV		AR0, A
				INC		DPTR
				MOVX	A, @DPTR
				MOV		AR1, A
				INC		DPTR
				MOVX	A, @DPTR
				MOV		AR2, A
				INC		DPTR
				MOVX	A, @DPTR
				MOV		AR3, A
				INC		DPTR
				MOVX	A, @DPTR
				MOV		AR4, A
				INC		DPTR
				MOVX	A, @DPTR
				MOV		AR5, A
				INC		DPTR
				MOVX	A, @DPTR
				MOV		AR6, A
				INC		DPTR
				MOVX	A, @DPTR
				MOV		AR7, A
	IF MCUCFG_USERREGCONFIG == 2 || MCUCFG_XBPSTACK == __ENABLED__
				INC		DPTR
	ENDIF
ENDIF

IF MCUCFG_USERREGCONFIG == 2					; RESTORE USERREG: 保护方案二
				mUserReg_Res
	IF MCUCFG_XBPSTACK == __ENABLED__
				INC		DPTR
	ENDIF
ENDIF

IF MCUCFG_XBPSTACK == __ENABLED__				; RESTORE ?C_XBP
				MOVX	A, @DPTR
				MOV		R0, A
				INC		DPTR
				MOVX	A, @DPTR
				JBC		EA, __EA
				MOV		?C_XBP, R0
				MOV		?C_XBP+1, A
				SJMP	RETURN
__EA:			MOV		?C_XBP, R0
				MOV		?C_XBP+1, A
				SETB	EA
ENDIF

RETURN:
IF MCUCFG_TASK_REGBANK == MCUCFG_SYSINT_REGBANK
				POP		AR7
				POP		AR6
				POP		AR5
				POP		AR4
				POP		AR3
				POP		AR2
				POP		AR1
				POP		AR0
ENDIF
				POP		PSW
				POP		DPL
				POP		DPH
				POP		B
				POP		ACC

				RETI

;///////////////////////////////////////////////////////////////////////////////

; 中断挂起服务FIFO队列

IF MCUCFG_PENDSVFIFO_DEPTH > 0

; mPendSV_FIFOPreempt
; 抢占队列项

?PR?mPendSV_FIFOPreempt?PORT_8051S	SEGMENT	CODE
				EXTRN	BIT  (m_sign_fifo)
				EXTRN	BIT  (m_sign_fifo_0_0)
				EXTRN	BIT  (m_sign_fifo_0_1)
				EXTRN	BIT  (m_sign_fifo_0_2)
				EXTRN	BIT  (m_sign_fifo_0_3)
				EXTRN	BIT  (m_sign_fifo_0_4)
				EXTRN	BIT  (m_sign_fifo_0_5)
				EXTRN	BIT  (m_sign_fifo_0_6)
				EXTRN	BIT  (m_sign_fifo_0_7)
				EXTRN	BIT  (m_sign_fifo_1_0)
				EXTRN	BIT  (m_sign_fifo_1_1)
				EXTRN	BIT  (m_sign_fifo_1_2)
				EXTRN	BIT  (m_sign_fifo_1_3)
				EXTRN	BIT  (m_sign_fifo_1_4)
				EXTRN	BIT  (m_sign_fifo_1_5)
				EXTRN	BIT  (m_sign_fifo_1_6)
				EXTRN	BIT  (m_sign_fifo_1_7)
IF MCUCFG_PENDSVFIFO_DEPTH > 8
				EXTRN	BIT  (m_sign_fifo_0_8)
				EXTRN	BIT  (m_sign_fifo_0_9)
				EXTRN	BIT  (m_sign_fifo_0_10)
				EXTRN	BIT  (m_sign_fifo_0_11)
				EXTRN	BIT  (m_sign_fifo_0_12)
				EXTRN	BIT  (m_sign_fifo_0_13)
				EXTRN	BIT  (m_sign_fifo_0_14)
				EXTRN	BIT  (m_sign_fifo_0_15)
				EXTRN	BIT  (m_sign_fifo_1_8)
				EXTRN	BIT  (m_sign_fifo_1_9)
				EXTRN	BIT  (m_sign_fifo_1_10)
				EXTRN	BIT  (m_sign_fifo_1_11)
				EXTRN	BIT  (m_sign_fifo_1_12)
				EXTRN	BIT  (m_sign_fifo_1_13)
				EXTRN	BIT  (m_sign_fifo_1_14)
				EXTRN	BIT  (m_sign_fifo_1_15)
IF MCUCFG_PENDSVFIFO_DEPTH > 16
				EXTRN	BIT  (m_sign_fifo_0_16)
				EXTRN	BIT  (m_sign_fifo_0_17)
				EXTRN	BIT  (m_sign_fifo_0_18)
				EXTRN	BIT  (m_sign_fifo_0_19)
				EXTRN	BIT  (m_sign_fifo_0_20)
				EXTRN	BIT  (m_sign_fifo_0_21)
				EXTRN	BIT  (m_sign_fifo_0_22)
				EXTRN	BIT  (m_sign_fifo_0_23)
				EXTRN	BIT  (m_sign_fifo_1_16)
				EXTRN	BIT  (m_sign_fifo_1_17)
				EXTRN	BIT  (m_sign_fifo_1_18)
				EXTRN	BIT  (m_sign_fifo_1_19)
				EXTRN	BIT  (m_sign_fifo_1_20)
				EXTRN	BIT  (m_sign_fifo_1_21)
				EXTRN	BIT  (m_sign_fifo_1_22)
				EXTRN	BIT  (m_sign_fifo_1_23)
IF MCUCFG_PENDSVFIFO_DEPTH > 24
				EXTRN	BIT  (m_sign_fifo_0_24)
				EXTRN	BIT  (m_sign_fifo_0_25)
				EXTRN	BIT  (m_sign_fifo_0_26)
				EXTRN	BIT  (m_sign_fifo_0_27)
				EXTRN	BIT  (m_sign_fifo_0_28)
				EXTRN	BIT  (m_sign_fifo_0_29)
				EXTRN	BIT  (m_sign_fifo_0_30)
				EXTRN	BIT  (m_sign_fifo_0_31)
				EXTRN	BIT  (m_sign_fifo_1_24)
				EXTRN	BIT  (m_sign_fifo_1_25)
				EXTRN	BIT  (m_sign_fifo_1_26)
				EXTRN	BIT  (m_sign_fifo_1_27)
				EXTRN	BIT  (m_sign_fifo_1_28)
				EXTRN	BIT  (m_sign_fifo_1_29)
				EXTRN	BIT  (m_sign_fifo_1_30)
				EXTRN	BIT  (m_sign_fifo_1_31)
IF MCUCFG_PENDSVFIFO_DEPTH > 32
				EXTRN	BIT  (m_sign_fifo_0_32)
				EXTRN	BIT  (m_sign_fifo_0_33)
				EXTRN	BIT  (m_sign_fifo_0_34)
				EXTRN	BIT  (m_sign_fifo_0_35)
				EXTRN	BIT  (m_sign_fifo_0_36)
				EXTRN	BIT  (m_sign_fifo_0_37)
				EXTRN	BIT  (m_sign_fifo_0_38)
				EXTRN	BIT  (m_sign_fifo_0_39)
				EXTRN	BIT  (m_sign_fifo_1_32)
				EXTRN	BIT  (m_sign_fifo_1_33)
				EXTRN	BIT  (m_sign_fifo_1_34)
				EXTRN	BIT  (m_sign_fifo_1_35)
				EXTRN	BIT  (m_sign_fifo_1_36)
				EXTRN	BIT  (m_sign_fifo_1_37)
				EXTRN	BIT  (m_sign_fifo_1_38)
				EXTRN	BIT  (m_sign_fifo_1_39)
ENDIF
ENDIF
ENDIF
ENDIF
				RSEG	?PR?mPendSV_FIFOPreempt?PORT_8051S
mPendSV_FIFOPreempt:

				CLR		A
				MOV		DPH, A
				JB		m_sign_fifo, FIFO_0
				INC		DPH
				LJMP	FIFO_1

FIFO_0:			JBC		m_sign_fifo_0_0,  FIFO_X_0
				JBC		m_sign_fifo_0_1,  FIFO_X_1
				JBC		m_sign_fifo_0_2,  FIFO_X_2
				JBC		m_sign_fifo_0_3,  FIFO_X_3
				JBC		m_sign_fifo_0_4,  FIFO_X_4
				JBC		m_sign_fifo_0_5,  FIFO_X_5
				JBC		m_sign_fifo_0_6,  FIFO_X_6
				JBC		m_sign_fifo_0_7,  FIFO_X_7
IF MCUCFG_PENDSVFIFO_DEPTH > 8
				JBC		m_sign_fifo_0_8,  FIFO_X_8
				JBC		m_sign_fifo_0_9,  FIFO_X_9
				JBC		m_sign_fifo_0_10, FIFO_X_10
				JBC		m_sign_fifo_0_11, FIFO_X_11
				JBC		m_sign_fifo_0_12, FIFO_X_12
				JBC		m_sign_fifo_0_13, FIFO_X_13
				JBC		m_sign_fifo_0_14, FIFO_X_14
				JBC		m_sign_fifo_0_15, FIFO_X_15
IF MCUCFG_PENDSVFIFO_DEPTH > 16
				JBC		m_sign_fifo_0_16, FIFO_X_16
				JBC		m_sign_fifo_0_17, FIFO_X_17
				JBC		m_sign_fifo_0_18, FIFO_X_18
				JBC		m_sign_fifo_0_19, FIFO_X_19
				JBC		m_sign_fifo_0_20, FIFO_X_20
				JBC		m_sign_fifo_0_21, FIFO_X_21
				JBC		m_sign_fifo_0_22, FIFO_X_22
				JBC		m_sign_fifo_0_23, FIFO_X_23
IF MCUCFG_PENDSVFIFO_DEPTH > 24
				JBC		m_sign_fifo_0_24, FIFO_X_24
				JBC		m_sign_fifo_0_25, FIFO_X_25
				JBC		m_sign_fifo_0_26, FIFO_X_26
				JBC		m_sign_fifo_0_27, FIFO_X_27
				JBC		m_sign_fifo_0_28, FIFO_X_28
				JBC		m_sign_fifo_0_29, FIFO_X_29
				JBC		m_sign_fifo_0_30, FIFO_X_30
				JBC		m_sign_fifo_0_31, FIFO_X_31
IF MCUCFG_PENDSVFIFO_DEPTH > 32
				JBC		m_sign_fifo_0_32, FIFO_X_32
				JBC		m_sign_fifo_0_33, FIFO_X_33
				JBC		m_sign_fifo_0_34, FIFO_X_34
				JBC		m_sign_fifo_0_35, FIFO_X_35
				JBC		m_sign_fifo_0_36, FIFO_X_36
				JBC		m_sign_fifo_0_37, FIFO_X_37
				JBC		m_sign_fifo_0_38, FIFO_X_38
				JBC		m_sign_fifo_0_39, FIFO_X_39
ENDIF
ENDIF
ENDIF
ENDIF
				INC		A
				RET

FIFO_X_0:		MOV		R0, #0*2
				RET
FIFO_X_1:		MOV		R0, #1*2
				RET
FIFO_X_2:		MOV		R0, #2*2
				RET
FIFO_X_3:		MOV		R0, #3*2
				RET
FIFO_X_4:		MOV		R0, #4*2
				RET
FIFO_X_5:		MOV		R0, #5*2
				RET
FIFO_X_6:		MOV		R0, #6*2
				RET
FIFO_X_7:		MOV		R0, #7*2
				RET
IF MCUCFG_PENDSVFIFO_DEPTH > 8
FIFO_X_8:		MOV		R0, #8*2
				RET
FIFO_X_9:		MOV		R0, #9*2
				RET
FIFO_X_10:		MOV		R0, #10*2
				RET
FIFO_X_11:		MOV		R0, #11*2
				RET
FIFO_X_12:		MOV		R0, #12*2
				RET
FIFO_X_13:		MOV		R0, #13*2
				RET
FIFO_X_14:		MOV		R0, #14*2
				RET
FIFO_X_15:		MOV		R0, #15*2
				RET
IF MCUCFG_PENDSVFIFO_DEPTH > 16
FIFO_X_16:		MOV		R0, #16*2
				RET
FIFO_X_17:		MOV		R0, #17*2
				RET
FIFO_X_18:		MOV		R0, #18*2
				RET
FIFO_X_19:		MOV		R0, #19*2
				RET
FIFO_X_20:		MOV		R0, #20*2
				RET
FIFO_X_21:		MOV		R0, #21*2
				RET
FIFO_X_22:		MOV		R0, #22*2
				RET
FIFO_X_23:		MOV		R0, #23*2
				RET
IF MCUCFG_PENDSVFIFO_DEPTH > 24
FIFO_X_24:		MOV		R0, #24*2
				RET
FIFO_X_25:		MOV		R0, #25*2
				RET
FIFO_X_26:		MOV		R0, #26*2
				RET
FIFO_X_27:		MOV		R0, #27*2
				RET
FIFO_X_28:		MOV		R0, #28*2
				RET
FIFO_X_29:		MOV		R0, #29*2
				RET
FIFO_X_30:		MOV		R0, #30*2
				RET
FIFO_X_31:		MOV		R0, #31*2
				RET
IF MCUCFG_PENDSVFIFO_DEPTH > 32
FIFO_X_32:		MOV		R0, #32*2
				RET
FIFO_X_33:		MOV		R0, #33*2
				RET
FIFO_X_34:		MOV		R0, #34*2
				RET
FIFO_X_35:		MOV		R0, #35*2
				RET
FIFO_X_36:		MOV		R0, #36*2
				RET
FIFO_X_37:		MOV		R0, #37*2
				RET
FIFO_X_38:		MOV		R0, #38*2
				RET
FIFO_X_39:		MOV		R0, #39*2
				RET
ENDIF
ENDIF
ENDIF
ENDIF

FIFO_1:			JBC		m_sign_fifo_1_0,  FIFO_X_0
				JBC		m_sign_fifo_1_1,  FIFO_X_1
				JBC		m_sign_fifo_1_2,  FIFO_X_2
				JBC		m_sign_fifo_1_3,  FIFO_X_3
				JBC		m_sign_fifo_1_4,  FIFO_X_4
				JBC		m_sign_fifo_1_5,  FIFO_X_5
				JBC		m_sign_fifo_1_6,  FIFO_X_6
				JBC		m_sign_fifo_1_7,  FIFO_X_7
IF MCUCFG_PENDSVFIFO_DEPTH > 8
				JBC		m_sign_fifo_1_8,  FIFO_X_8
				JBC		m_sign_fifo_1_9,  FIFO_X_9
				JBC		m_sign_fifo_1_10, FIFO_X_10
				JBC		m_sign_fifo_1_11, FIFO_X_11
				JBC		m_sign_fifo_1_12, FIFO_X_12
				JBC		m_sign_fifo_1_13, FIFO_X_13
				JBC		m_sign_fifo_1_14, FIFO_X_14
				JBC		m_sign_fifo_1_15, FIFO_X_15
IF MCUCFG_PENDSVFIFO_DEPTH > 16
				JBC		m_sign_fifo_1_16, FIFO_X_16
				JBC		m_sign_fifo_1_17, FIFO_X_17
				JBC		m_sign_fifo_1_18, FIFO_X_18
				JBC		m_sign_fifo_1_19, FIFO_X_19
				JBC		m_sign_fifo_1_20, FIFO_X_20
				JBC		m_sign_fifo_1_21, FIFO_X_21
				JBC		m_sign_fifo_1_22, FIFO_X_22
				JBC		m_sign_fifo_1_23, FIFO_X_23
IF MCUCFG_PENDSVFIFO_DEPTH > 24
				JBC		m_sign_fifo_1_24, FIFO_X_24
				JBC		m_sign_fifo_1_25, FIFO_X_25
				JBC		m_sign_fifo_1_26, FIFO_X_26
				JBC		m_sign_fifo_1_27, FIFO_X_27
				JBC		m_sign_fifo_1_28, FIFO_X_28
				JBC		m_sign_fifo_1_29, FIFO_X_29
				JBC		m_sign_fifo_1_30, FIFO_X_30
				JBC		m_sign_fifo_1_31, FIFO_X_31
IF MCUCFG_PENDSVFIFO_DEPTH > 32
				JBC		m_sign_fifo_1_32, FIFO_X_32
				JBC		m_sign_fifo_1_33, FIFO_X_33
				JBC		m_sign_fifo_1_34, FIFO_X_34
				JBC		m_sign_fifo_1_35, FIFO_X_35
				JBC		m_sign_fifo_1_36, FIFO_X_36
				JBC		m_sign_fifo_1_37, FIFO_X_37
				JBC		m_sign_fifo_1_38, FIFO_X_38
				JBC		m_sign_fifo_1_39, FIFO_X_39
ENDIF
ENDIF
ENDIF
ENDIF
				INC		A
				RET

; mPendSV_FIFOLoader
; 装载中断挂起服务的结构体指针

?PR?_mPendSV_FIFOLoader?PORT_8051S	SEGMENT	CODE
				PUBLIC	_mPendSV_FIFOLoader
				RSEG	?PR?_mPendSV_FIFOLoader?PORT_8051S
_mPendSV_FIFOLoader:

				LCALL	mPendSV_FIFOPreempt
				JNZ		FAILED
				MOV		DPL, R0
				MOV		A, R6
				MOVX	@DPTR, A
				INC		DPTR
				MOV		A, R7
				MOVX	@DPTR, A
				MOV		R7, #0
				RET
FAILED:			MOV		R7, A
				RET

ENDIF

;///////////////////////////////////////////////////////////////////////////////

ENDIF
				END
