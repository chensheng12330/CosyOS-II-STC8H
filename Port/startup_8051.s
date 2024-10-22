;*******************************************************************************
;* @item     CosyOS-II Port
;* @file     startup_8051.s
;* @brief    8051 Core Startup File
;* @author   �ٿ���
;* @version  V3.0.1
;* @date     2024.07.07
;*******************************************************************************
;
$NOMOD51
$INCLUDE (..\Config\syscfg.h)
IF SYSCFG_MCUCORE == 8051
;
;------------------------------------------------------------------------------
;  ���� STARTUP.A51 ���޸ģ�������һЩ�����õĻ��ò����Ĺ��ܣ�
;  ����Ӹ�Ϊ�ḻ������ע�͡�
;
;  *** <<< Use Configuration Wizard in Context Menu >>> ***
;------------------------------------------------------------------------------
;
; <h> �ڴ�����
;
; <o> IDATA�ڴ��С <0-256>
; <i> IDATA�ڴ�ľ��Կ�ʼ��ַ����0���������á�
; <i> IDATA�ڴ渲��������DATA��BIT����
IDATALEN        EQU     100H
;
; <o> XDATA�ڴ濪ʼ��ַ <0-0xFFFF>
; <i> XDATA�ڴ�ľ��Կ�ʼ��ַ��
XDATASTART      EQU     0
;
; <o> XDATA�ڴ��С <0-0xFFFF>
; <i> XDATA�ڴ��С��
XDATALEN        EQU     8192
;
; </h>
;------------------------------------------------------------------------------
;
; <h> ������ջ��ʼ��
; <i> ����������������֮ǰ�����õ�������ջ��
; <i> ���ڳ�ʼ�����ӻ�ĳ�ж��е���һ�������뺯�������ʼ��������ջ��
;
; <q> XBPSTACK
; <i> �Ƿ��ʼ����ģ�Ϳ�����ջ��XBPSTACK����
XBPSTACK        EQU     1
;
; <o> XBPSTACKTOP <0x0-0xFFFF>
; <i> ����XBPSTACK��ջ��ָ�롣
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
