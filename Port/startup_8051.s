;*******************************************************************************
;* @item     CosyOS-II Port
;* @file     startup_8051.s
;* @brief    8051 Core Startup File
;* @author   迟凯峰
;* @version  V3.0.1
;* @date     2024.07.07
;*******************************************************************************
;
$NOMOD51
$INCLUDE (..\Config\syscfg.h)
IF SYSCFG_MCUCORE == 8051
;
;------------------------------------------------------------------------------
;  基于 STARTUP.A51 而修改，摒弃了一些不常用的或用不到的功能，
;  并添加更为丰富的中文注释。
;
;  *** <<< Use Configuration Wizard in Context Menu >>> ***
;------------------------------------------------------------------------------
;
; <h> 内存清零
;
; <o> IDATA内存大小 <0-256>
; <i> IDATA内存的绝对开始地址总是0，无需设置。
; <i> IDATA内存覆盖了物理DATA和BIT区域。
IDATALEN        EQU     100H
;
; <o> XDATA内存开始地址 <0-0xFFFF>
; <i> XDATA内存的绝对开始地址。
XDATASTART      EQU     0
;
; <o> XDATA内存大小 <0-0xFFFF>
; <i> XDATA内存大小。
XDATALEN        EQU     8192
;
; </h>
;------------------------------------------------------------------------------
;
; <h> 可重入栈初始化
; <i> 如果在任务调度运行之前，会用到可重入栈，
; <i> 如在初始化钩子或某中断中调用一个可重入函数，需初始化可重入栈。
;
; <q> XBPSTACK
; <i> 是否初始化大模型可重入栈（XBPSTACK）？
XBPSTACK        EQU     1
;
; <o> XBPSTACKTOP <0x0-0xFFFF>
; <i> 设置XBPSTACK的栈顶指针。
XBPSTACKTOP     EQU     0x1FFF +1
;
; </h>
;------------------------------------------------------------------------------

                NAME    ?C_STARTUP

?C_C51STARTUP   SEGMENT   CODE
?STACK          SEGMENT   IDATA
                RSEG    ?STACK
                DS      1

				EXTRN DATA (?C_XBP)
                EXTRN CODE (?C_START)
                PUBLIC  ?C_STARTUP

                CSEG    AT      0
?C_STARTUP:     LJMP    STARTUP1

                RSEG    ?C_C51STARTUP

STARTUP1:

IF IDATALEN <> 0
                MOV     R0,#IDATALEN - 1
                CLR     A
IDATALOOP:      MOV     @R0,A
                DJNZ    R0,IDATALOOP
ENDIF

IF XDATALEN <> 0
                MOV     DPTR,#XDATASTART
                MOV     R7,#LOW (XDATALEN)
  IF (LOW (XDATALEN)) <> 0
                MOV     R6,#(HIGH (XDATALEN)) +1
  ELSE
                MOV     R6,#HIGH (XDATALEN)
  ENDIF
                CLR     A
XDATALOOP:      MOVX    @DPTR,A
                INC     DPTR
                DJNZ    R7,XDATALOOP
                DJNZ    R6,XDATALOOP
ENDIF

IF XBPSTACK <> 0
                MOV     ?C_XBP,#HIGH XBPSTACKTOP
                MOV     ?C_XBP+1,#LOW XBPSTACKTOP
ENDIF

                MOV     SP,#?STACK-1

                LJMP    ?C_START

ENDIF
                END
