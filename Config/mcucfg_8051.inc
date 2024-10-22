;*******************************************************************************
;* @item     CosyOS-II Config
;* @file     mcucfg_8051.inc
;* @brief    8051 Core Config File
;* @detail   自定义任务切换现场保护，汇编语言保护方案。
;* @author   迟凯峰
;* @version  V3.0.0
;* @date     2024.06.25
;*******************************************************************************
;
; 请在 mcucfg_8051.h 文件中，选择您中意的保护方案，而后在下面重新调整代码。
; 下方各定义项已经直接给出示例（保护/恢复：DPH1、DPL1、DPS），直接修改即可。
;
;///////////////////////////////////////////////////////////////////////////////

; 保护方案一
; 采用PUSH/POP指令入栈/出栈，操作简单、代码简洁、但性能偏低。

IF MCUCFG_USERREGCONFIG == 1

mUserReg_PUSH	MACRO	; 定义保护现场代码
		PUSH	DPH1
		PUSH	DPL1
		PUSH	DPS
				ENDM

mUserReg_POP	MACRO	; 定义恢复现场代码
		POP		DPS
		POP		DPL1
		POP		DPH1
				ENDM

ENDIF

;///////////////////////////////////////////////////////////////////////////////

; 保护方案二
; 在用户寄存器与任务栈之间直接搬移数据，操作相对繁琐，代码量稍大，但性能高效。

IF MCUCFG_USERREGCONFIG == 2

mUserReg_Save	MACRO	; 定义保护现场代码
		MOV		A, DPH1
		MOVX	@DPTR, A
		INC		DPTR
		MOV		A, DPL1
		MOVX	@DPTR, A
		INC		DPTR
		MOV		A, DPS
		MOVX	@DPTR, A
				ENDM

mUserReg_Res	MACRO	; 定义恢复现场代码
		MOVX	A, @DPTR
		MOV		DPH1, A
		INC		DPTR
		MOVX	A, @DPTR
		MOV		DPL1, A
		INC		DPTR
		MOVX	A, @DPTR
		MOV		DPS, A
				ENDM

ENDIF
