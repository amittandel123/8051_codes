;------------------------------------------------------------------------------
;  This file is part of the 'RTX-51 tiny' Real-Time Operating System Package
;  Copyright KEIL ELEKTRONIK GmbH and Keil Software, Inc.  1991 - 2002
;------------------------------------------------------------------------------
;
;  OS_SEND_USER.A51:  This module contains the OS_SEND_USER function
;
;  RTX51 TINY VERSION 2
;
;------------------------------------------------------------------------------

NAME	?RTX51_TINY_OS_SEND

$include (os_defines.inc)

PUBLIC  _os_send_user
PUBLIC  _isr_send_user
EXTRN CODE (?RTX?SET_ISR)

?RTX?CODE	SEGMENT	CODE
		RSEG	?RTX?CODE
		USING	0		; Registerbank 0 for following code


_os_send_user:
_isr_send_user:
;  os_send_user (uchar taskid)  {
;    uchar data *p;
;---- Variable 'taskid' assigned to Register 'R7' ----
;---- Variable 'p' assigned to Register 'R0' ----
;    if (no > MAXTASKN)  return (0xff);
		MOV     A,R7
		SETB    C
		SUBB    A,#?RTX_MAXTASKN
		JC      ??C0010
		MOV     R7,#0FFH
		RET     
??C0010:

;    p = &STATE[taskid].st;
		MOV     A,R7
		RL	A
		ADD	A,#?RTX?TASKSTATE?S+1
		MOV	R0,A
		CLR	EA
		MOV	A,@R0
;      *p |= (K_READY | K_USER);
		ORL	A,#K_READY OR K_USER
		XCH	A,@R0
		SETB	EA
		MOV	R7,#0
		JNB	ACC.B_USR,NO_USR_PENDING
		INC	R7
NO_USR_PENDING:	JMP     ?RTX?SET_ISR

		END
