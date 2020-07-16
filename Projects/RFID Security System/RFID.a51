
RB0     EQU     00H    ; Select Register Bank 0
RB1     EQU     08H    ; Select Register Bank 1  ...poke to PSW to use
RB2     EQU     10H    ; Select Register Bank 1  ...poke to PSW to use


;%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
;		PORT DECLERATION
;%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% 
SDA 	EQU 	P1.2  	;SDA=PIN5
SCL 	EQU 	P1.1	;SCL=PIN6
;CLK	EQU	P3.2

DS1307W   EQU    0D0H     ; SLAVE ADDRESS 1101 000 + 0 TO WRITE
DS1307R   EQU    0D1H     ; SLAVE ADDRESS 1101 000 + 1 TO READ

	KB_LSHIFT	EQU      12H
	KB_RSHIFT	EQU      59H
    KB_CTRL		EQU      14H
    KB_ALT		EQU      11H
    KB_CAPS		EQU      58H
    KB_SCROLL	EQU      7EH
    KB_NUML		EQU      77H
    KB_TAB		EQU      0DH
    KB_REL		EQU      0F0H
    KB_EXT		EQU      0E0H
    KB_PAUSE	EQU      0E1H
    
    CAPS		EQU          01H
    NUML		EQU          02H
    SCROLL	EQU          04H
    SHIFT	EQU          08H
    ACK		EQU          10H
    CTRL		EQU          20H
    ALT		EQU          40H
    
    RELEASE	EQU        	 80H
    PAUSED	EQU         40H
    EXTENDED EQU       80H
    
SDA1 	EQU 	P2.3  	;SDA=PIN5
SCL1 	EQU 	P2.2		;SCL=PIN6
WTCMD EQU   10100000B       ;WRITE DATA COMMAND Note 3
RDCMD EQU   10100001B       ;READ DATA COMMAND Note 3  

UP		EQU	P1.4
DOWN	EQU	P1.6
ENTER EQU	P1.7
CANCEL EQU	P1.5

	
; ***LCD CONTROL***
LCD_RS    EQU    P0.0     ;LCD REGISTER SELECT LINE
LCD_E     EQU    P0.1    ;LCD ENABLE LINE
LCD_DB4   EQU    P0.2     ;PORT 1 IS USED FOR DATA
LCD_DB5   EQU    P0.3     ;USED FOR DATA
LCD_DB6   EQU    P0.4    ;FOR DATA
LCD_DB7   EQU    P0.5     ;FOR DATA

; ***CURSOR CONTROL INSTRUCTIONS***

OFFCUR    EQU    0CH
BLINKCUR  EQU    0DH

; ***DISPLAY CONTROL INSTRUCTIONS***

CLRDSP    EQU    01H
ONDSP     EQU    0CH

; ***SYSTEM INSTRUCTIONS***

CONFIG    EQU    28H      ; 4-BIT DATA,2 LINES,5X7 MATRIX LCD
ENTRYMODE EQU    6        ; INCREMENT CURSOR DON'T SHIFT DISPLAY 

KEYB_CLOCK	EQU	P0.7
KEYB_DATA	EQU	P0.6


    
DSEG            ; This is internal data memory
ORG     20H     ; Bit adressable memory
FLAGS1:	DS		1
RECEIVED	BIT FLAGS1.0
AM			BIT	FLAGS1.1
MAS		BIT	FLAGS1.2
KEYBRD	BIT	FLAGS1.3
AMS		BIT	FLAGS1.4
COUNTER:	DS		1
FLAGS:	DS		1
LASTREAD  BIT    FLAGS.0
SQW 	    BIT    FLAGS.4
ACKS       BIT    FLAGS.5
BUS_FLT   BIT    FLAGS.6
_2W_BUSY  BIT    FLAGS.7


CAPPS_ON:	DS		1
CAAPS			BIT	CAPPS_ON.0

BITCNT:	DS		1
BYTECNT:	DS		1
BYTE:		DS		10
BYTES:	DS		10
SECS:		DS		1      ;   '   SECONDS STORAGE RAM
MINS:		DS		1       ;   '   MINUTES   '     '
HRS:		DS		1       ;   '   HOURS     '     '
DAY:		DS		1       ;   '   DAY       '     '
DATE1:	DS		1      ;   '   DATE      '     '
MONTH:	DS		1      ;   '   MONTH     '     '
YEAR:		DS		1       ;   '   YEAR      '     '
TOTAL:	DS		1			;TOTAL COUNT
TEMP:		DS		1
TEMPS:	DS		1

KB_OK:		DS		1
KB_DATA:		DS		1
KB_STATS:		DS		1
KB_SCAN:		DS		1
KB_TEMP:		DS		1  ; For the da*n pause key
KB_COUNT:	DS		1
NAMES:		DS		16
STACK:		DS		1
; ***MACRO'S***

SCL_HIGH  MACRO
          SETB   SCL      ; SET SCL HIGH
          JNB    SCL,$    ; LOOP UNTIL STRONG 1 ON SCL
          ENDM
;%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CSEG            ; Code begins here
         
;  ---------==========----------==========---------=========---------
;  Main routine. Program execution starts here.
;  ---------==========----------==========---------=========---------
		ORG     00H    ; Reset
		AJMP MAIN
            
		ORG 23H
		JMP SERIAL 
;  ---------==========----------==========---------=========---------				
MAIN:		
         MOV    PSW,#RB0    		  ; Select register bank 0
        	MOV    SP,#STACK	
; **********************************************************	
;		INITILIZE RTC 
; **********************************************************	
	SETB SDA        	; ENSURE SDA HIGH
   SCL_HIGH               ; ENSURE SCL HIGH
   CLR ACKS      	       ; CLEAR STATUS FLAGS
   CLR BUS_FLT
   CLR _2W_BUSY
	CLR SQW
	CALL OSC_CONTROL		;Initilize the RTC
; **********************************************************
			MOV TMOD,#20H
			MOV TH1,#0FDH
			MOV SCON,#50H
			SETB ES
			SETB EA
			SETB TR1	
			
			MOV COUNTER,#01H 
        	CLR RECEIVED
        	CLR KEYBRD
			
			CALL RESETLCD4
			CALL INITLCD4
			CALL TITLES
			
;			MOV TOTAL,#00H
;			CALL WRITE_TOTAL	
			
			
; **********************************************************
;		CHECK FOR ENTER THE TIME 
; **********************************************************
	LCALL SEND_START 		; SEND 2WIRE START CONDITION
	MOV A,#DS1307W 			; SEND DS1307 WRITE COMMAND
	LCALL SEND_BYTE
	MOV A,#08H 			; SET POINTER TO REG 08H ON DS1307
	LCALL SEND_BYTE
	LCALL SEND_STOP 		; SEND STOP CONDITION
	LCALL SEND_START 		; SEND START CONDITION
	MOV A,#DS1307R 			; SEND DS1307 READ COMMAND
	LCALL SEND_BYTE
	LCALL READ_BYTE 		; READ A BYTE OF DATA
	MOV R1,A
	LCALL SEND_STOP 		; SEND 2WIRE STOP CONDITION

	CJNE A,#0AAH,SET_RTC
	AJMP TOP

;**********************************************************		
;		STORE RTC TIME
;**********************************************************	   
KBNotOKS:
			CALL DISP1
			JB ENTER,$
			NOP
			JNB ENTER,$
			AJMP FXC4
;************************************************************************   			
SET_RTC:			
			MOV TEMP,#00H
			JB KEYBRD,LAOPS
FXC4:		CALL InitKeyb				;Inizialize Keyboard 
      	MOV a, KB_OK
      	JZ KBNotOKS   
      	SETB KEYBRD 
LAOPS:	LCALL LOAPA
;************************************************************************
;************************************************************************
			MOV R1,#BYTE
			MOV R4,#10H				;MASTER ADDRESS
			CALL STORE_EEPROM		
;			MOV TOTAL,#00H
;			CALL WRITE_TOTAL			
;************************************************************************
TOP:		CALL RTCLOCK
			MOV R0,#04H
RX3: 		MOV R1,#0FFH
RX1:		MOV R2,#0FFH
RX2:		JB RECEIVED,TOPS
			DJNZ R2,RX2
			DJNZ R1,RX1  
			DJNZ R0,RX3  
			AJMP TOP		
;************************************************************************			
TOPS:		MOV COUNTER,#01H 
			CLR RECEIVED
			CLR ES
			
			MOV R1,#BYTES					;CHECK FOR MASTER CARD
			MOV R4,#10H						;MASTER ADDRESS
			MOV R6,#10						;NUMBER OF BYTES
			CALL READ_EEPROM	
			CLR MAS		
			CALL COMPARE
			JNB MAS,OTHER1
			CALL DISPLAY
			CALL DELAYS
			CALL DELAYS
			
REP1:		MOV TEMP,#01H
			CALL DISPLAY1			;Press enter to add tag
			
			SETB UP
			SETB DOWN
			SETB ENTER
			SETB CANCEL
UJI1:		JNB CANCEL, VBN1
			JNB UP,VBN2
			JNB DOWN,VBN3
			JNB ENTER,VBN4
			AJMP UJI1
			
OTHER1:	AJMP OTHER			
			
VBN1:		JNB CANCEL,$
			SETB ES
			CLR RECEIVED
			AJMP TOP		

VBN2:		JNB UP,$
			CALL DISPLAY1
			MOV TEMP,#01H
			AJMP UJI1
VBN3:		JNB DOWN,$
			CALL DISPLAY2
			MOV TEMP,#02H
			AJMP UJI1

;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@			
GHO1:		CJNE R5,#02H,GHO2				;Delete Tag
			JNB ENTER,$
			JMP DELETE_PGM
			
GHO2:		JNB ENTER,$
			AJMP UJI1			
KBNotOK:
			CALL DISP1
			JB ENTER,$
			NOP
			JNB ENTER,$
			AJMP FDC4			
			
REAP1:	CALL DELAY
			JNB CANCEL,$
			AJMP REP1			
;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@			
VBN4:		MOV R5,TEMP
			CJNE R5,#01H,GHO1				;Add Tag
VBC1:		CLR RECEIVED
			CALL DISPLAY3					;Show TAG
			SETB ES
			
FVG1:		JNB CANCEL,REAP1
			JNB RECEIVED,FVG1
			CLR ES
			
			JB KEYBRD,LOOP1
FDC4:		CALL InitKeyb				;Inizialize Keyboard 
      	MOV a, KB_OK
      	JZ KBNotOK   
      	SETB KEYBRD 
      		
LOOP1:   CALL DISPLAY4		;plz enter name
			MOV TEMP,#00H
			CALL CLEAR_RAM
EDC:		CALL LOOP
        	INC TEMP
        	MOV R2,TEMP
        	CJNE R2,#01H,EDC1
        	MOV R1,#NAMES
        	MOV @R1,KB_DATA
EDC1:		CJNE R2,#02H,EDC2
        	MOV R1,#NAMES+1			
        	MOV @R1,KB_DATA
EDC2:		CJNE R2,#03H,EDC3
        	MOV R1,#NAMES+2
        	MOV @R1,KB_DATA		
EDC3:		CJNE R2,#04H,EDC4
        	MOV R1,#NAMES+3
        	MOV @R1,KB_DATA		
EDC4:		CJNE R2,#05H,EDC5
        	MOV R1,#NAMES+4
        	MOV @R1,KB_DATA	
EDC5:		CJNE R2,#06H,EDC6
        	MOV R1,#NAMES+5
        	MOV @R1,KB_DATA		
EDC6:		CJNE R2,#07H,EDC7
        	MOV R1,#NAMES+6
        	MOV @R1,KB_DATA	
EDC7:		CJNE R2,#08H,EDC8
        	MOV R1,#NAMES+7
        	MOV @R1,KB_DATA	
EDC8:		CJNE R2,#09H,EDC9
        	MOV R1,#NAMES+8
        	MOV @R1,KB_DATA	
EDC9:		CJNE R2,#10,EDC10
        	MOV R1,#NAMES+9
        	MOV @R1,KB_DATA	
EDC10:	CJNE R2,#11,EDC11
        	MOV R1,#NAMES+10
        	MOV @R1,KB_DATA	
EDC11:	CJNE R2,#12,EDC12
        	MOV R1,#NAMES+11
        	MOV @R1,KB_DATA
EDC12:	CJNE R2,#13,EDC13
        	MOV R1,#NAMES+12
        	MOV @R1,KB_DATA
EDC13:	CJNE R2,#14,EDC14
        	MOV R1,#NAMES+13
        	MOV @R1,KB_DATA
EDC14:	CJNE R2,#15,EDC15
        	MOV R1,#NAMES+14
        	MOV @R1,KB_DATA
EDC15:	CJNE R2,#16,EDC16
        	MOV R1,#NAMES+15
        	MOV @R1,KB_DATA
EDC16:	CJNE R2,#17,EDC       	
        	CALL DELAYS
        	
        	CALL PRESS_ENTER
        	SETB ENTER
        	SETB CANCEL
HHJ:    	JNB CANCEL,LOOP1S
        	JB ENTER,HHJ    
        	
        	CALL READ_TOTAL
        	INC TOTAL 	
        	
        	MOV A,TOTAL
			CJNE A,#01H,JNM1
			MOV R1,#BYTE		;store tag ID
			MOV R4,#20H			;Starting Address
			MOV R6,#10			;STORE 10 BYTES
			CALL STORE_EEPROM		
			CALL MDELAY
			MOV R1,#NAMES			;store Name
			MOV R4,#30H			;Starting Address
			MOV R6,#16			;STORE 16 BYTES
			CALL STORE_EEPROM	
			AJMP JNM	
					
LOOP1S:	AJMP LOOP1			

JNM1:		CJNE A,#02H,JNM2
			MOV R1,#BYTE		;store tag ID
			MOV R4,#40H			;Starting Address
			MOV R6,#10			;STORE 10 BYTES
			CALL STORE_EEPROM		
			CALL MDELAY
			MOV R1,#NAMES			;store Name
			MOV R4,#50H			;Starting Address
			MOV R6,#16			;STORE 16 BYTES
			CALL STORE_EEPROM		
			AJMP JNM		

JNM2:		CJNE A,#03H,JNM3
			MOV R1,#BYTE		;store tag ID
			MOV R4,#60H			;Starting Address
			MOV R6,#10			;STORE 10 BYTES
			CALL STORE_EEPROM		
			CALL MDELAY
			MOV R1,#NAMES			;store Name
			MOV R4,#70H			;Starting Address
			MOV R6,#16			;STORE 16 BYTES
			CALL STORE_EEPROM	
			AJMP JNM			

JNM3:		CJNE A,#04H,JNM4
			MOV R1,#BYTE		;store tag ID
			MOV R4,#80H			;Starting Address
			MOV R6,#10			;STORE 10 BYTES
			CALL STORE_EEPROM		
			CALL MDELAY
			MOV R1,#NAMES			;store Name
			MOV R4,#90H			;Starting Address
			MOV R6,#16			;STORE 16 BYTES
			CALL STORE_EEPROM	
			AJMP JNM						

JNM4:		CJNE A,#05H,JNM5
			MOV R1,#BYTE		;store tag ID
			MOV R4,#0A0H			;Starting Address
			MOV R6,#10			;STORE 10 BYTES
			CALL STORE_EEPROM		
			CALL MDELAY
			MOV R1,#NAMES			;store Name
			MOV R4,#0B0H			;Starting Address
			MOV R6,#16			;STORE 16 BYTES
			CALL STORE_EEPROM	
			AJMP JNM						
						
JNM5:		CJNE A,#06H,JNM6
			MOV R1,#BYTE		;store tag ID
			MOV R4,#0C0H			;Starting Address
			MOV R6,#10			;STORE 10 BYTES
			CALL STORE_EEPROM		
			CALL MDELAY
			MOV R1,#NAMES			;store Name
			MOV R4,#0D0H			;Starting Address
			MOV R6,#16			;STORE 16 BYTES
			CALL STORE_EEPROM	
			AJMP JNM		
			
JNM6:		CJNE A,#07H,JNM
			MOV R1,#BYTE		;store tag ID
			MOV R4,#0E0H			;Starting Address
			MOV R6,#10			;STORE 10 BYTES
			CALL STORE_EEPROM		
			CALL MDELAY
			MOV R1,#NAMES			;store Name
			MOV R4,#0F0H			;Starting Address
			MOV R6,#16			;STORE 16 BYTES
			CALL STORE_EEPROM	
			AJMP JNM													
JNM:		CALL ID_SAVED			;Tag stored sucessfully	
			CALL WRITE_TOTAL		;WRITE TOTAL ID IN MEMORY	
			CALL DELAYS
			JNB ENTER,$
			AJMP REP1



        	
    		      	
        
Loop:    CALL Check_Keyb 
			JNB ENTER,GHBH
			JNB CANCEL,DCFV1
        	JNC Loop
        	MOV R4,KB_DATA
         CALL WRLCDDATA
         RET
GHBH:		JNB ENTER,$
			MOV TEMP,#16
			RET	    
			
DCFV1:	JNB CANCEL,$
			AJMP LOOP1  
			     
;********************************************					
OTHER:	
			CALL READ_TOTAL       		
			MOV R2,TOTAL
			CALL CLEAR_RAM
					
			;check first card  
			MOV R1,#BYTES					;GET DATA IN BYTES(RAM)
			MOV R4,#20H						;MASTER ADDRESS
			MOV R6,#10						;NUMBER OF BYTES
			CALL READ_EEPROM
			CLR MAS		
			CALL COMPARE
			JNB MAS,GAN1
			MOV R1,#NAMES					;GET DATA IN BYTES(RAM)
			MOV R4,#30H						;MASTER ADDRESS
			MOV R6,#16						;NUMBER OF BYTES
			CALL READ_EEPROM
			CALL NAME1
			CALL DISP_NAME		
			AJMP GAN2
GAN1:			
			;check 2 card  
			MOV R1,#BYTES					;GET DATA IN BYTES(RAM)
			MOV R4,#40H						;MASTER ADDRESS
			MOV R6,#10						;NUMBER OF BYTES
			CALL READ_EEPROM
			CLR MAS		
			CALL COMPARE
			JNB MAS,GAN3
			MOV R1,#NAMES					;GET DATA IN BYTES(RAM)
			MOV R4,#50H						;MASTER ADDRESS
			MOV R6,#16						;NUMBER OF BYTES
			CALL READ_EEPROM
			CALL NAME1
			CALL DISP_NAME		
			AJMP GAN2
GAN3:
			;check 3 card  
			MOV R1,#BYTES					;GET DATA IN BYTES(RAM)
			MOV R4,#60H						;MASTER ADDRESS
			MOV R6,#10						;NUMBER OF BYTES
			CALL READ_EEPROM
			CLR MAS		
			CALL COMPARE
			JNB MAS,GAN4
			MOV R1,#NAMES					;GET DATA IN BYTES(RAM)
			MOV R4,#70H						;MASTER ADDRESS
			MOV R6,#16						;NUMBER OF BYTES
			CALL READ_EEPROM
			CALL NAME1
			CALL DISP_NAME		
			AJMP GAN2
GAN4:
			;check 2 card  
			MOV R1,#BYTES					;GET DATA IN BYTES(RAM)
			MOV R4,#80H						;MASTER ADDRESS
			MOV R6,#10						;NUMBER OF BYTES
			CALL READ_EEPROM
			CLR MAS		
			CALL COMPARE
			JNB MAS,GAN5
			MOV R1,#NAMES					;GET DATA IN BYTES(RAM)
			MOV R4,#90H						;MASTER ADDRESS
			MOV R6,#16						;NUMBER OF BYTES
			CALL READ_EEPROM
			CALL NAME1
			CALL DISP_NAME		
			AJMP GAN2
GAN5:
			;check 2 card  
			MOV R1,#BYTES					;GET DATA IN BYTES(RAM)
			MOV R4,#0A0H						;MASTER ADDRESS
			MOV R6,#10						;NUMBER OF BYTES
			CALL READ_EEPROM
			CLR MAS		
			CALL COMPARE
			JNB MAS,GAN6
			MOV R1,#NAMES					;GET DATA IN BYTES(RAM)
			MOV R4,#0B0H						;MASTER ADDRESS
			MOV R6,#16						;NUMBER OF BYTES
			CALL READ_EEPROM
			CALL NAME1
			CALL DISP_NAME		
			AJMP GAN2

GAN6:			;check 2 card  
			MOV R1,#BYTES					;GET DATA IN BYTES(RAM)
			MOV R4,#0C0H						;MASTER ADDRESS
			MOV R6,#10						;NUMBER OF BYTES
			CALL READ_EEPROM
			CLR MAS		
			CALL COMPARE
			JNB MAS,GAN7
			MOV R1,#NAMES					;GET DATA IN BYTES(RAM)
			MOV R4,#0D0H						;MASTER ADDRESS
			MOV R6,#16						;NUMBER OF BYTES
			CALL READ_EEPROM
			CALL NAME1
			CALL DISP_NAME		
			AJMP GAN2

GAN7:			;check 2 card  
			MOV R1,#BYTES					;GET DATA IN BYTES(RAM)
			MOV R4,#0E0H						;MASTER ADDRESS
			MOV R6,#10						;NUMBER OF BYTES
			CALL READ_EEPROM
			CLR MAS		
			CALL COMPARE
			JNB MAS,GAN8
			MOV R1,#NAMES					;GET DATA IN BYTES(RAM)
			MOV R4,#0F0H						;MASTER ADDRESS
			MOV R6,#16						;NUMBER OF BYTES
			CALL READ_EEPROM
			CALL NAME1
			CALL DISP_NAME		
			AJMP GAN2

GAN8:		;check 2 card  
			MOV R1,#BYTES					;GET DATA IN BYTES(RAM)
			MOV R4,#30H						;MASTER ADDRESS
			MOV R6,#10						;NUMBER OF BYTES
			CALL READ_EEPROM
			CLR MAS		
			CALL COMPARE
			JNB MAS,GAN9
			MOV R1,#NAMES					;GET DATA IN BYTES(RAM)
			MOV R4,#40H						;MASTER ADDRESS
			MOV R6,#16						;NUMBER OF BYTES
			CALL READ_EEPROM
			CALL NAME1
			CALL DISP_NAME		
			AJMP GAN2
			
GAN9:
			CALL CARD_ERROR			
GAN2:		SETB ES 		
			CALL DELAYS
			CALL DELAYS
			AJMP TOP
			
;##########################################################
;		DISPLAY ROUTINES
;##########################################################
TITLES:
	MOV DPTR,#MSAG
	CALL LCD_MSG
	RET
MSAG:
	DB 1H,81H,'RFID Based',0C0H,'Attendance Sys',00H
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
TITLE1:
	MOV DPTR,#MSAG1
	CALL LCD_MSG
	RET
MSAG1:
	DB 1H,81H,'RFID Data:',0C0H,00H
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~	
DISPLAY:
	MOV DPTR,#MSAG2
	CALL LCD_MSG
	RET
MSAG2:
	DB 1H,80H,'Master Card',0C0H,'Detected...',00H
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
DISPLAY1:
	MOV DPTR,#MSAG3
	CALL LCD_MSG
	RET
MSAG3:
	DB 1H,80H,'Press Enter...',0C0H,'*** ADD TAG ***',00H
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
DISPLAY2:
	MOV DPTR,#MSAG4
	CALL LCD_MSG
	RET
MSAG4:
	DB 1H,80H,'Press Enter...',0C1H,'** EDIT TAG **',00H
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
DISPLAY3:
	MOV DPTR,#MSAG5
	CALL LCD_MSG
	RET
MSAG5:
	DB 1H,80H,'Please show the',0C3H,'** TAG **',00H
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
DISPLAY4:
	MOV DPTR,#MSAG6
	CALL LCD_MSG
	RET
MSAG6:
	DB 1H,0FH,80H,'Plz enter Name',0C0H,00H
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
DISPLAY5:
	MOV DPTR,#MSAG7
	CALL LCD_MSG
	RET
MSAG7:
	DB 1H,80H,'Press enter EDIT',0C0H,00H
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
DISP1:
	MOV DPTR,#MSG1
	CALL LCD_MSG
	RET
MSG1:
	DB 1H,84H,'Keyboard',0C3H,'* Not Ok *', 00H
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
DATE_DISP:
	MOV DPTR,#MSG2
	CALL LCD_MSG
	RET
MSG2:
	DB 1H,80H,'Enter Date',0C0H,'(01-31):',00H
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
MON_DISP:
	MOV DPTR,#MSG3
	CALL LCD_MSG
	RET
MSG3:
	DB 1H,80H,'Enter Month',0C0H,'(01-12):',00H
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
YR_DISP:
	MOV DPTR,#MSG4
	CALL LCD_MSG
	RET
MSG4:
	DB 1H,80H,'Enter Year',0C0H,'(00-99):',00H
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
HR_DISP:
	MOV DPTR,#MSG5
	CALL LCD_MSG
	RET
MSG5:
	DB 1H,80H,'Enter Hour',0C0H,'(01-12):',00H
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
MIN_DISP:
	MOV DPTR,#MSG6
	CALL LCD_MSG
	RET
MSG6:
	DB 1H,80H,'Enter Minutes',0C0H,'(00-59):',00H
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
DAY_DISP:
	MOV DPTR,#MSG7
	CALL LCD_MSG
	RET
MSG7:
	DB 1H,80H,'Enter Day',0C0H,'(0-7):',00H
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
AMS_DISP:
	MOV DPTR,#MSG8
	CALL LCD_MSG
	RET
MSG8:
	DB 1H,80H,'Enter AM/PM',0C0H,'AM=0,PM=1:',00H
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
AM_DISP:
	MOV DPTR,#MSG9
	CALL LCD_MSG
	RET
MSG9:
	DB 1H,80H,'Enter AM/PM',0C0H,'AM=0,PM=1: AM',00H
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
PM_DISP:
	MOV DPTR,#MSG10
	CALL LCD_MSG
	RET
MSG10:
	DB 1H,80H,'Enter AM/PM',0C0H,'AM=0,PM=1: PM',00H
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
PRESS_ENTER:
	MOV DPTR,#MSG11
	CALL LCD_MSG
	RET
MSG11:
	DB 1H,82H,'Press Enter',0C4H,'to Save',00H
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ID_SAVED:
	MOV DPTR,#MSG12
	CALL LCD_MSG
	RET
MSG12:
	DB 1H,83H,'TAG Saved',0C2H,'Sucessfully',00H
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
NAME1:
	MOV DPTR,#MSG13
	CALL LCD_MSG
	RET
MSG13:
	DB 1H,82H,'* Welcome *',0C0H,00H
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CARD_ERROR:
	MOV DPTR,#MSG14
	CALL LCD_MSG
	RET
MSG14:
	DB 1H,84H,'Card Not',0C3H,'Programmed',00H
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
NO_CARD:
	MOV DPTR,#MSG15
	CALL LCD_MSG
	RET
MSG15:
	DB 1H,84H,'No card',0C3H,'Programmed',00H
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
;&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
;			COMPARE WITH MASTER CARD
;&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
COMPARE:
			MOV A,BYTES
			CJNE A,BYTE,FGG
			MOV A,BYTES+1
			CJNE A,BYTE+1,FGG
			MOV A,BYTES+2
			CJNE A,BYTE+2,FGG
			MOV A,BYTES+3
			CJNE A,BYTE+3,FGG
			MOV A,BYTES+4
			CJNE A,BYTE+4,FGG
			MOV A,BYTES+5
			CJNE A,BYTE+5,FGG
			MOV A,BYTES+6
			CJNE A,BYTE+6,FGG
			MOV A,BYTES+7
			CJNE A,BYTE+7,FGG
			MOV A,BYTES+8
			CJNE A,BYTE+8,FGG
			MOV A,BYTES+9
			CJNE A,BYTE+9,FGG
			SETB MAS
FGG:		RET
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
DELAY:
		MOV R1,#0FFH
RE1:	MOV R2,#0FFH
RE:	NOP
		DJNZ R2,RE
		DJNZ R1,RE1
		RET
;**********************************************************		
DELAYS:                       ;One second delay routine
     	MOV R0,#05H
RS3: 	MOV R1,#0FFH
RA1:	MOV R2,#0FFH
RS2:	NOP
		DJNZ R2,RS2
		DJNZ R1,RA1  
		DJNZ R0,RS3  
  RET
;---------==========----------==========---------=========---------
SERIAL:		
		PUSH   PSW           ; save current registerset
	 	MOV    PSW,#RB1
      PUSH   ACC
		JB TI,TRANS
		MOV A,SBUF
		CJNE A,#0AH,DOWNW
		MOV COUNTER,#01H 
		AJMP DDWN
DOWNW:CJNE A,#0DH,DOWN2 
		MOV COUNTER,#01H 
		AJMP DDWN		
DOWN2:
		MOV R1,COUNTER
		CJNE R1,#01H,YH1
		MOV BYTE,A
		AJMP DOWN1
YH1:	CJNE R1,#02H,YH2
		MOV BYTE+1,A
		AJMP DOWN1
YH2:	CJNE R1,#03H,YH3
		MOV BYTE+2,A
		AJMP DOWN1
YH3:	CJNE R1,#04H,YH4
		MOV BYTE+3,A
		AJMP DOWN1
YH4:	CJNE R1,#05H,YH5
		MOV BYTE+4,A
		AJMP DOWN1
YH5:	CJNE R1,#06H,YH6
		MOV BYTE+5,A
		AJMP DOWN1
YH6:	CJNE R1,#07H,YH7
		MOV BYTE+6,A
		AJMP DOWN1
YH7:	CJNE R1,#08H,YH8
		MOV BYTE+7,A
		AJMP DOWN1
YH8:	CJNE R1,#09H,YH9
		MOV BYTE+8,A
		AJMP DOWN1
YH9:	CJNE R1,#0AH,DOWN1
		MOV BYTE+9,A
		SETB RECEIVED
		AJMP DOWN1
DOWN1:INC COUNTER

DDWN:	CLR RI
		POP    ACC
   	POP    PSW 
		RETI
		
TRANS: CLR TI
		POP    ACC
   	POP    PSW 
		RETI
;**********************************************************
; INITIALIZE THE LCD 4-BIT MODE                                                                                   
;**********************************************************
INITLCD4:
         CLR         LCD_RS     ; LCD REGISTER SELECT LINE
         CLR         LCD_E      ; ENABLE LINE
         MOV         R4, #CONFIG; FUNCTION SET - DATA BITS,
                                ; LINES, FONTS
         CALL       WRLCDCOM4
         MOV         R4, #ONDSP ; DISPLAY ON
         CALL       WRLCDCOM4
         MOV         R4, #ENTRYMODE ; SET ENTRY MODE
         CALL       WRLCDCOM4  ; INCREMENT CURSOR RIGHT, NO SHIFT
         MOV         R4, #CLRDSP; CLEAR DISPLAY, HOME CURSOR
         CALL       WRLCDCOM4
         RET


; **********************************************************
; SOFTWARE VERSION OF THE POWER ON RESET
; **********************************************************
RESETLCD4:
         CLR         LCD_RS     ; LCD REGISTER SELECT LINE
         CLR         LCD_E      ; ENABLE LINE
         CLR         LCD_DB7    ; SET BIT PATTERN FOR...
         CLR         LCD_DB6    ; ... POWER-ON-RESET
         SETB        LCD_DB5
         SETB        LCD_DB4
         SETB        LCD_E      ; START ENABLE PULSE
         CLR         LCD_E      ; END ENABLE PULSE
         MOV         A, #4      ; DELAY 4 MILLISECONDS
         CALL       MDELAY
         SETB        LCD_E      ; START ENABLE PULSE
         CLR         LCD_E      ; END ENABLE PULSE
         MOV         A, #1      ; DELAY 1 MILLISECOND
         CALL       MDELAY
         SETB        LCD_E      ; START ENABLE PULSE
         CLR         LCD_E      ; END ENABLE PULSE
         MOV         A, #1      ; DELAY 1 MILLISECOND
         CALL       MDELAY
         CLR         LCD_DB4    ; SPECIFY 4-BIT OPERATION
         SETB        LCD_E      ; START ENABLE PULSE
         CLR         LCD_E      ; END ENABLE PULSE
         MOV         A, #1      ; DELAY 1 MILLISECOND
         CALL       MDELAY
         MOV         R4, #CONFIG; FUNCTION SET
         CALL       WRLCDCOM4
         MOV         R4, #08H   ; DISPLAY OFF
         CALL       WRLCDCOM4
         MOV         R4, #1     ; CLEAR DISPLAY, HOME CURSOR
         CALL       WRLCDCOM4
         MOV         R4,#ENTRYMODE  ; SET ENTRY MODE
         ACALL       WRLCDCOM4
 	   JMP INITLCD4

; **********************************************************
; SUB WRITES A COMMAND WORD TO THE LCD
; COMMAND MUST BE PLACED IN R4 BY CALLING PROGRAM
; **********************************************************
WRLCDCOM4:
         CLR         LCD_E
         CLR         LCD_RS     ; SELECT SEND COMMAND
         PUSH        ACC        ; SAVE ACCUMULATOR
         MOV         A, R4      ; PUT DATA BYTE IN ACC
         MOV         C, ACC.4   ; LOAD HIGH NIBBLE ON DATA BUS
         MOV         LCD_DB4, C ; ONE BIT AT A TIME USING...
         MOV         C, ACC.5   ; BIT MOVE OPERATOINS
         MOV         LCD_DB5, C
         MOV         C, ACC.6
         MOV         LCD_DB6, C
         MOV         C, ACC.7
         MOV         LCD_DB7, C
         SETB        LCD_E      ; PULSE THE ENABLE LINE
         CLR         LCD_E
         MOV         C, ACC.0   ; SIMILARLY, LOAD LOW NIBBLE
         MOV         LCD_DB4, C
         MOV         C, ACC.1
         MOV         LCD_DB5, C
         MOV         C, ACC.2
         MOV         LCD_DB6, C
         MOV         C, ACC.3
         MOV         LCD_DB7, C
         CLR         LCD_E
         SETB        LCD_E      ; PULSE THE ENABLE LINE
         CLR         LCD_E
         CALL MADELAY
         POP         ACC
         RET
; **********************************************************
; SUB TO WRITE A DATA WORD TO THE LCD
; DATA MUST BE PLACED IN R4 BY CALLING PROGRAM
; **********************************************************
WRLCDDATA:
         CLR         LCD_E
         SETB        LCD_RS     ; SELECT SEND DATA
             PUSH        ACC        ; SAVE ACCUMULATOR
         MOV         A, R4      ; PUT DATA BYTE IN ACC
         MOV         C, ACC.4   ; LOAD HIGH NIBBLE ON DATA BUS
         MOV         LCD_DB4, C ; ONE BIT AT A TIME USING...
         MOV         C, ACC.5   ; BIT MOVE OPERATOINS
         MOV         LCD_DB5, C
         MOV         C, ACC.6
         MOV         LCD_DB6, C
         MOV         C, ACC.7
         MOV         LCD_DB7, C
         SETB        LCD_E      ; PULSE THE ENABLE LINE
         CLR         LCD_E
         MOV         C, ACC.0   ; SIMILARLY, LOAD LOW NIBBLE
         MOV         LCD_DB4, C
         MOV         C, ACC.1
         MOV         LCD_DB5, C
         MOV         C, ACC.2
         MOV         LCD_DB6, C
         MOV         C, ACC.3
         MOV         LCD_DB7, C
         CLR         LCD_E
         SETB        LCD_E      ; PULSE THE ENABLE LINE
         CLR         LCD_E
         NOP
         NOP
         POP         ACC
         RET

; **********************************************************
; SUB TAKES THE STRING IMMEDIATELY FOLLOWING THE CALL AND
; DISPLAYS ON THE LCD. STRING MUST BE TERMINATED WITH A
; NULL (0).
; **********************************************************
LCD_MSG:
	CLR A			; Clear Index 
	MOVC A,@A+DPTR		; Get byte pointed by Dptr 
	INC DPTR			; Point to the next byte 
	JZ LCD_Msg9		; Return if found the zero (end of stringz) 
        CJNE A,#001H,Lcd_Msg1	; Check if is a Clear Command 
	MOV R4,A
	CALL WRLCDCOM4       	;If yes, write it as command to LCD 
	JMP   LCD_MSG          	;Go get next byte from stringz 
                              
Lcd_Msg1: CJNE A,#0FFH,FLL	;Check for displaying full character
	MOV R4,A
	CALL WRLCDDATA
	JMP LCD_MSG
 FLL:	CJNE  A,#080h,$+3      	; Data or Address?  If => 80h then is address. 
	JC    Lcd_Msg_Data     	; Carry will be set if A < 80h (Data) 
	MOV R4,A
	CALL  WRLCDCOM4         ; Carry not set if A=>80, it is address 
	JMP Lcd_Msg          	; Go get next byte from stringz 
                             
Lcd_Msg_Data:                   ; 
	MOV R4,A
	CALL WRLCDDATA        	; It was data, write it to Lcd 
	JMP  Lcd_Msg          		; Go get next byte from stringz 
Lcd_Msg9: 
	
	RET                    	; Return to Caller 



; **********************************************************
; 1 MILLISECOND DELAY ROUTINE
; **********************************************************

MDELAY:
         PUSH        ACC
         MOV         A,#0A6H
MD_OLP:
         INC         A
         NOP
         NOP
         NOP
         NOP
         NOP
         NOP
         NOP
         NOP
         JNZ         MD_OLP
         NOP
         POP         ACC
         RET
MADELAY:
         PUSH        ACC
         MOV         A,#036H
MAD_OLP:
         INC         A
         NOP
         NOP
         NOP
         NOP
         NOP
         NOP
         NOP
         NOP
         JNZ         MAD_OLP
         NOP
         POP         ACC
         RET
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
; **********************************************************
; SUB SETS THE DS1307 OSCILLATOR
; **********************************************************

OSC_CONTROL:
         ACALL       SEND_START ; GENERATE START CONDITION
         MOV         A,#DS1307W ; 1101 0000 ADDRESS + WRITE-BIT
         ACALL       SEND_BYTE  ; SEND BYTE TO 1307
         MOV         A,#00H     ; ADDRESS BYTE TO REGISTER 00H
         ACALL       SEND_BYTE  ; SECONDS REGISTER, ALWAYS LEAVE
         SETB        LASTREAD   ; REG 00H-BIT #7 = 0 (LOW)
         ACALL       SEND_STOP  ; IF REG 00H-BIT #7 = 1 CLOCK
         ACALL       SEND_START ; OSCILLATOR IS OFF.
         MOV         A,#DS1307R ; 1101 0001 ADDRESS + READ-BIT
         ACALL       SEND_BYTE  ;
         ACALL       READ_BYTE  ; READ A BYTE FROM THE 1307
         CLR         ACC.7      ; CLEAR REG 00H-BIT #7 TO ENABLE
OSC_SET:                        ; OSCILLATOR.
         PUSH        ACC        ; SAVE ON STACK
         ACALL       SEND_STOP  ;
         ACALL       SEND_START ;
         MOV         A,#DS1307W ; SETUP TO WRITE
         ACALL       SEND_BYTE  ;
         MOV         A,#00H     ; REGISTER 00H ADDRESS
         ACALL       SEND_BYTE  ;
         POP         ACC        ; GET DATA TO START OSCILLATOR
         ACALL       SEND_BYTE  ; SEND IT
         ACALL       SEND_STOP
        RET 
; **********************************************************	
; THIS SUB CONTROLS THE SQW OUTPUT 1HZ
; **********************************************************	
SQW_CONTROL_1HZ:
	LCALL SEND_START 	; SEND START CONDITION
	MOV A,#DS1307W	 	; SET POINTER TO REG 07H ON
				; DS1307
	LCALL SEND_BYTE
	MOV A,#07H
	LCALL SEND_BYTE
	MOV A,#90H 		; SQW/OUT ON AT 1HZ
	JNB SQW,SQW_SET		 ; JUMP IF SQW BIT IS ACTIVE
	MOV A,#80H 		; TURN SQW/OUT OFF – OFF HIGH
SQW_SET:
	LCALL SEND_BYTE
	LCALL SEND_STOP
	RET
	; **********************************************************	
; THIS SUB READS ONE BYTE OF DATA FROM THE DS1307
; **********************************************************

READ_BYTE:
         MOV         BITCNT,#08H; SET COUNTER FOR 8-BITS DATA
         MOV         A,#00H
         SETB        SDA        ; SET SDA HIGH TO ENSURE LINE
                                ; FREE
READ_BITS:
         SCL_HIGH               ; TRANSITION SCL LOW-TO-HIGH
         MOV         C,SDA      ; MOVE DATA BIT INTO CARRY
         RLC         A          ; ROTATE CARRY-BIT INTO ACC.0
         CLR         SCL        ; TRANSITION SCL HIGH-TO-LOW
         DJNZ        BITCNT,READ_BITS
                                ; LOOP FOR 8-BITS
         JB          LASTREAD,ACKN
                                ; CHECK TO SEE IF THIS IS
                                ; THE LAST READ
         CLR         SDA        ; IF NOT LAST READ SEND ACK-BIT

ACKN:
         SCL_HIGH               ; PULSE SCL TO TRANSMIT ACKNOWLEDGE
         CLR         SCL        ; OR NOT ACKNOWLEDGE BIT
         RET

; **********************************************************
; SUB SENDS START CONDITION
; **********************************************************

SEND_START:
         SETB        _2W_BUSY   ; INDICATE THAT 2-WIRE
         CLR         ACKS        ; OPERATION IS IN PROGRESS
         CLR         BUS_FLT    ; CLEAR STATUS FLAGS
         JNB         SCL,FAULT
         JNB         SDA,FAULT
         SETB        SDA        ; BEGIN START CODITION
         SCL_HIGH
         CLR         SDA
         ACALL       DEELAY
         CLR         SCL
         RET
FAULT:
         SETB        BUS_FLT
         RET

; **********************************************************
; SUB SENDS STOP CONDITION
; **********************************************************
SEND_STOP:
         CLR         SDA
         SCL_HIGH
         SETB        SDA
         CLR         _2W_BUSY
         RET
; **********************************************************
; SUB DELAYS THE BUS
; **********************************************************
DEELAY:
         NOP                    ; DELAY FOR BUS TIMING
         RET
; **********************************************************
; THIS SUB SENDS 1 BYTE OF DATA TO THE DS1307
; CALL THIS FOR EACH REGISTER SECONDS TO YEAR
; ACC MUST CONTAIN DATA TO BE SENT TO CLOCK
; **********************************************************
SEND_BYTE:
         MOV         BITCNT,#08H; SET COUNTER FOR 8-BITS
SB_LOOP:
         JNB         ACC.7,NOTONE; CHECK TO SEE IF BIT-7 OF
         SETB        SDA        ; ACC IS A 1, AND SET SDA HIGH
         JMP         ONE
NOTONE:
         CLR         SDA        ; CLR SDA LOW
ONE:
         SCL_HIGH               ; TRANSITION SCL LOW-TO-HIGH
         RL          A          ; ROTATE ACC LEFT 1-BIT
         CLR         SCL        ; TRANSITION SCL LOW-TO-HIGH
         DJNZ        BITCNT,SB_LOOP; LOOP FOR 8-BITS
         SETB        SDA        ; SET SDA HIGH TO LOOK FOR
         SCL_HIGH               ; ACKNOWLEDGE PULSE
         CLR         ACKS
         JNB         SDA,SB_EX  ; CHECK FOR ACK OR NOT ACK
         SETB        ACKS        ; SET ACKNOWLEDGE FLAG FOR
                                ; NOT ACK
SB_EX:
         CALL       DEELAY      ; DELAY FOR AN OPERATION
         CLR         SCL        ; TRANSITION SCL HIGH-TO-LOW
         CALL       DEELAY      ; DELAY FOR AN OPERATION
         RET
; **********************************************************
; SUB READS THE CLOCK AND WRITES IT TO THE SCRATCHPAD MEMORY
; ON RETURN FROM HERE DATE & TIME DATA WILL BE STORED IN THE
; DATE & TIME REGISTERS FROM 24H (SECS) TO 2AH (YEAR)
; ALARM SETTINGS IN REGISTERS 2CH(HRS) AND 2DH(MINUTES).
; **********************************************************
READ_CLOCK:
         MOV         R1,#SECS   ; SECONDS STORAGE LOCATION
         MOV         BYTECNT,#00H
         CLR         LASTREAD
         CALL       SEND_START
         MOV         A,#DS1307W
         CALL       SEND_BYTE
         MOV         A,#00H
         CALL       SEND_BYTE
         CALL       SEND_STOP
         CALL       SEND_START
         MOV         A,#DS1307R
         CALL       SEND_BYTE

READ_LOOP:
         MOV         A,BYTECNT
         CJNE        A,#09H,NOT_LAST
         SETB        LASTREAD

NOT_LAST:
         CALL       READ_BYTE
         MOV         @R1,A
         MOV         A,BYTECNT
         CJNE        A,#00H,NOT_FIRST
         MOV         A,@R1
         CLR         ACC.7      ; ENSURE OSC BIT=0 (ENABLED)
         MOV         @R1,A
NOT_FIRST:
         INC         R1
         INC         BYTECNT
         MOV         A,BYTECNT
         CJNE        A,#0AH,READ_LOOP
         CALL       SEND_STOP
         RET
;&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&         
;				DISPLAY CLOCK ON LCD
;&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
RTCLOCK:
		  CALL READ_CLOCK
        MOV R4,#01H
        CALL WRLCDCOM4
        CALL MDELAY
        MOV R4,#81H
        CALL WRLCDCOM4
        CALL MDELAY
        
        MOV R3,DATE1
        CALL SPLIT_DATA
        MOV R4,#'-'
        CALL WRLCDDATA
        CALL MDELAY
        MOV R3,MONTH
        CALL SPLIT_DATA
        MOV R4,#'-'
        CALL WRLCDDATA
        CALL MDELAY
        MOV R4,#'2'
        CALL WRLCDDATA
        CALL MDELAY
        MOV R4,#'0'
        CALL WRLCDDATA
        CALL MDELAY
        MOV R3,YEAR
        CALL SPLIT_DATA
        
        MOV A,DAY
        CJNE A,#01H,TGY1
        CALL SUN
        JMP TGY
TGY1:	  CJNE A,#02H,TGY2
        CALL MON
        JMP TGY
TGY2:	  CJNE A,#03H,TGY3
        CALL TUE
        JMP TGY        
TGY3:	  CJNE A,#04H,TGY4
        CALL WED
        JMP TGY
TGY4:	  CJNE A,#05H,TGY5
        CALL THU
        JMP TGY
TGY5:	  CJNE A,#06H,TGY6
        CALL FRI
        JMP TGY
TGY6:	  CJNE A,#07H,TGY
        CALL SAT
        JMP TGY        
TGY:    MOV R4,#0C3H
        CALL WRLCDCOM4
        CALL MDELAY
        MOV A,HRS
        JNB ACC.5,DCF3
        CLR AM
        AJMP DCF2
DCF3:	  SETB AM       
DCF2:	  ANL A,#00011111B
		  MOV R3,A
        CALL SPLIT_DATA
        MOV R4,#':'
        CALL WRLCDDATA
        CALL MDELAY
        MOV R3,MINS
        CALL SPLIT_DATA
        MOV R4,#':'
        CALL WRLCDDATA
        CALL MDELAY
        MOV R3,SECS
        CALL SPLIT_DATA
        MOV R4,#' '
        CALL WRLCDDATA
        CALL MDELAY
        
        JNB AM,DCF0
        MOV R4,#'A'
        CALL WRLCDDATA
        CALL MDELAY
        JMP DCF4
DCF0:	  MOV R4,#'P'
        CALL WRLCDDATA
        CALL MDELAY
                
DCF4:   MOV R4,#'M'
        CALL WRLCDDATA
        CALL MDELAY
        RET
;&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&        
SPLIT_DATA:
			MOV A,R3
			ANL A,#0F0H
			SWAP A
			ADD A,#30H
			MOV R4,A
			CALL WRLCDDATA
			CALL MDELAY
			MOV A,R3
			ANL A,#0FH
			ADD A,#30H
			MOV R4,A
			CALL WRLCDDATA
			CALL MDELAY
			RET        
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~	
SUN:
	MOV DPTR,#MAG1
	CALL LCD_MSG
	RET
MAG1:
	DB ' SUN',00H
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~	        
MON:
	MOV DPTR,#MAG2
	CALL LCD_MSG
	RET
MAG2:
	DB ' MON',00H
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~	        
TUE:
	MOV DPTR,#MAG3
	CALL LCD_MSG
	RET
MAG3:
	DB ' TUE',00H
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~	        
WED:
	MOV DPTR,#MAG4
	CALL LCD_MSG
	RET
MAG4:
	DB ' WED',00H
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~	        
THU:
	MOV DPTR,#MAG5
	CALL LCD_MSG
	RET
MAG5:
	DB ' THU',00H
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~	        
FRI:
	MOV DPTR,#MAG6
	CALL LCD_MSG
	RET
MAG6:
	DB ' FRI',00H
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~	        
SAT:
	MOV DPTR,#MAG7
	CALL LCD_MSG
	RET
MAG7:
	DB ' SAT',00H
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~	        
 ;***********************************************************************
; THIS ROUTINE SENDS OUT CONTENTS OF THE ACCUMULATOR
; to the EEPROM and includes START condition.  Refer to the data sheets
; for discussion of START and STOP conditions.
;***********************************************************************

OUTS:   MOV     R2,#8           ;LOOP COUNT -- EQUAL TO BIT COUNT
        SETB   SDA1            ;INSURE DATA IS HI               
        SETB    SCL1            ;INSURE CLOCK IS HI
        NOP                     ;NOTE 1
        NOP                     
        NOP
        CLR     SDA1            ;START CONDITION -- DATA = 0
        NOP                     ;NOTE 1
        NOP      
        NOP
        CLR     SCL1            ;CLOCK = 0
OTSLP:  RLC     A               ;SHIFT BIT
        JNC     BITLS
        SETB    SDA1            ;DATA = 1
        JMP     OTSL1           ;CONTINUE
BITLS:  CLR     SDA1            ;DATA = 0
OTSL1:  SETB    SCL1            ;CLOCK HI
        NOP                     ;NOTE 1
        NOP
        NOP
        
        CLR     SCL1            ;CLOCK LOW
        DJNZ    R2,OTSLP        ;DECREMENT COUNTER
        SETB    SDA1            ;TURN PIN INTO INPUT
        NOP                     ;NOTE 1
                             
        SETB    SCL1            ;CLOCK ACK
        NOP                     ;NOTE 1
        NOP
        NOP
      
        CLR     SCL1
        RET

;**********************************************************************
; THIS ROUTINE SENDS OUT CONTENTS OF ACCUMLATOR TO EEPROM
; without sending a START condition.
;**********************************************************************

OUT:    MOV     R2,#8           ;LOOP COUNT -- EQUAL TO BIT COUNT
OTLP:   RLC     A               ;SHIFT BIT
        JNC     BITL            
        SETB    SDA1            ;DATA = 1
        JMP     OTL1            ;CONTINUE
BITL:   CLR     SDA1            ;DATA = 0
OTL1:   SETB    SCL1            ;CLOCK HI
        NOP                     ;NOTE 1
        NOP
        NOP
     
        CLR     SCL1            ;CLOCK LOW
        DJNZ    R2,OTLP         ;DECREMENT COUNTER
        SETB    SDA1            ;TURN PIN INTO INPUT
        NOP                     ;NOTE 1
     
        SETB    SCL1            ;CLOCK ACK
        NOP                     ;NOTE 1
        NOP
        NOP
      
        CLR     SCL1
        RET


STOP:   CLR     SDA1            ;STOP CONDITION SET DATA LOW
        NOP                     ;NOTE 1
        NOP
        NOP
      
        SETB    SCL1            ;SET CLOCK HI
        NOP                     ;NOTE 1
        NOP
        NOP
      
        SETB    SDA1            ;SET DATA HIGH
        RET
;*******************************************************************
; THIS ROUTINE READS A BYTE OF DATA FROM EEPROM
; From EEPROM current address pointer.
; Returns the data byte in R1
;*******************************************************************
CREAD:  MOV     A,#RDCMD        ;LOAD READ COMMAND
        CALL    OUTS            ;SEND IT
        CALL    IN              ;READ DATA
        MOV     R1,A            ;STORE DATA
        CALL    STOP            ;SEND STOP CONDITION
        RET

;**********************************************************************
; THIS ROUTINE READS IN A BYTE FROM THE EEPROM
; and stores it in the accumulator
;**********************************************************************

IN:     MOV     R2,#8           ;LOOP COUNT
        SETB    SDA1            ;SET DATA BIT HIGH FOR INPUT
INLP:   CLR     SCL1            ;CLOCK LOW
        NOP                     ;NOTE 1
        NOP
        NOP
        NOP
     
        SETB    SCL1            ;CLOCK HIGH
        CLR     C               ;CLEAR CARRY
        JNB     SDA1,INL1       ;JUMP IF DATA = 0
        CPL     C               ;SET CARRY IF DATA = 1
INL1:   RLC     A               ;ROTATE DATA INTO ACCUMULATOR
        DJNZ    R2,INLP         ;DECREMENT COUNTER
        CLR     SCL1            ;CLOCK LOW
        RET

;*********************************************************************
; This routine test for WRITE DONE condition 
; by testing for an ACK.
; This routine can be run as soon as a STOP condition
; has been generated after the last data byte has been sent
; to the EEPROM. The routine loops until an ACK is received from 
; the EEPROM. No ACK will be received until the EEPROM is done with
; the write operation.
;*********************************************************************
ACKTST: MOV     A,#WTCMD        ;LOAD WRITE COMMAND TO SEND ADDRESS
        MOV     R2,#8           ;LOOP COUNT -- EQUAL TO BIT COUNT
        CLR     SDA1            ;START CONDITION -- DATA = 0
        NOP                     ;NOTE 1
        NOP
        NOP
           
        CLR     SCL1            ;CLOCK = 0
AKTLP:  RLC     A               ;SHIFT BIT
        JNC     AKTLS
        SETB    SDA1            ;DATA = 1
        JMP     AKTL1           ;CONTINUE
AKTLS:  CLR     SDA1            ;DATA = 0
AKTL1:  SETB    SCL1            ;CLOCK HI
        NOP                     ;NOTE 1
        NOP
        NOP
      
        CLR     SCL1            ;CLOCK LOW
        DJNZ    R2,AKTLP        ;DECREMENT COUNTER
        SETB    SDA1            ;TURN PIN INTO INPUT
        NOP                     ;NOTE 1
      
        SETB    SCL1            ;CLOCK ACK
        NOP                     ;NOTE 1
        NOP
        NOP
       
        JNB     SDA1,EXIT       ;EXIT IF ACK (WRITE DONE)
        JMP     ACKTST          ;START OVER
EXIT:   CLR     SCL1            ;CLOCK LOW
        CLR     SDA1            ;DATA LOW
        NOP                     ;NOTE 1
        NOP
        NOP
      
        SETB    SCL1            ;CLOCK HIGH
        NOP
        NOP
        SETB    SDA1            ;STOP CONDITION
        RET
;*********************************************************************	
;%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
;				STORE ID IN EEPROM
;%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%   					
STORE_EEPROM:
			MOV A,#WTCMD ;LOAD WRITE COMMAND
			CALL OUTS ;SEND IT
			MOV A,R4 ;GET LOW BYTE ADDRESS
			CALL OUT ;SEND IT
BTLP: 	MOV A,@R1 ;GET DATA
			CALL OUT ;SEND IT
			INC R1 ;INCREMENT DATA POINTER
			DJNZ R6,BTLP ;LOOP TILL DONE
			CALL STOP ;SEND STOP CONDITION
			RET
;%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
;			READ DATAS FROM EEPROM
;%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%   		
READ_EEPROM:
		 	MOV A,#WTCMD ;LOAD WRITE COMMAND TO SEND ADDRESS
			CALL OUTS ;SEND IT
			MOV A,R4 ;GET LOW BYTE ADDRESS
			CALL OUT ;SEND IT
			MOV A,#RDCMD ;LOAD READ COMMAND
			CALL OUTS ;SEND IT
BRDLP: 	CALL IN ;READ DATA
			MOV @R1,a ;STORE DATA
			INC R1 ;INCREMENT DATA POINTER
			DJNZ R6,AKLP ;DECREMENT LOOP COUNTER
			CALL STOP ;IF DONE, ISSUE STOP CONDITION
			RET ;DONE, EXIT ROUTINE

AKLP: 	CLR SDA1 ;NOT DONE, ISSUE ACK
			SETB SCL1
			NOP ;NOTE 1
			NOP
			NOP
			NOP ;NOTE 2
			NOP
			CLR SCL1
			JMP BRDLP ;CONTINUE WITH READS
;%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
;				STORE TOTAL COUNT IN EEPROM
;%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%   					
WRITE_TOTAL:
			MOV     A,#WTCMD        ;LOAD WRITE COMMAND
      	CALL    OUTS            ;SEND IT
     	   MOV     A,#05H            ;GET BYTE ADDRESS
    	   CALL    OUT             ;SEND IT
    	   MOV     A,TOTAL            ;GET DATA
    	   CALL    OUT             ;SEND IT
    	   CALL    STOP            ;SEND STOP CONDITION
    	   CALL MDELAY		
    	   RET
;%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
;			READ TOTAL COUNT FROM EEPROM
;%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%   		
READ_TOTAL:
			MOV     A,#WTCMD        ;LOAD WRITE COMMAND TO SEND ADDRESS
        	CALL    OUTS            ;SEND IT
       	MOV     A,#05H            ;GET LOW BYTE ADDRESS
       	CALL    OUT             ;SEND IT
       	CALL    CREAD           ;GET DATA BYTE
       	MOV TOTAL,R1  
       	CALL MDELAY  	   
    	   RET
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
;---------------------
; Keyboard Routines:
;---------------------
;*************************************
; WaitKB: Wait for keypress
;*************************************
WaitKB:
        call   Check_Keyb
        jnc     WaitKB
        ret
;*************************************
; InitKeyb: c=1 if ACK OK
;*************************************
CheckACK:
        mov     a, KB_STATS
        mov     c, acc.4
        clr     acc.4
        mov     KB_STATS, a
        ret

;*************************************
; InitKeyb:
;*************************************
InitKeyb:
        mov     KB_TEMP, #0
        mov     KB_OK, #0
        mov     r1, #0FFH
        call   Write_Keyb
        call   Check_Keyb
        call   CheckACK
        jnc     InitKeyb

        mov     r1, #0F4H       ; Enable
       call   Write_Keyb
        call   Check_Keyb
        call   CheckACK
        jnc     KeybErr

        mov     r1, #0F3H       ; Set Typematic
        call   Write_Keyb
        call   Check_Keyb
        call   CheckACK
        jnc     KeybErr
        mov     r1, #00H      ; Typematic = 250 ms / 30 cps
        call   Write_Keyb
        call   Check_Keyb
        call   CheckACK
        jnc     KeybErr

        mov     KB_OK, #1
        mov     KB_STATS, #2    ; Num Lock ON

;*************************************
; Keyb_Leds: Set KB_STATS as leds
;*************************************
Keyb_Leds:
        mov     r1, #0EDH       ; Set Leds
        call   Write_Keyb
        call   Check_Keyb
        call   CheckACK
        jnc     KeybErr
        mov     r1, KB_STATS
        call   Write_Keyb
        call   Check_Keyb
        call   CheckACK
KeybErr:
        ret
;*************************************
; Zero2One: Wait for 0 to 1 on kb
; clock line, read the kb data line
; and shift right the bit to acc.7
;*************************************
Zero2One:
        jnb     KEYB_CLOCK, $
        jb      KEYB_CLOCK, $
        mov     c, KEYB_DATA
        rrc     a
        ret

;*************************************
; Check_Keyb: Check to see if any key
; are pressed or release, returns
; ASCII codes on KB_DATA, or 1 for
; special keys, 2 for same special
; with shift. Return also the scan
; code on KB_SCAN.
; Special Keys are basicaly all non
; printable keys. See the table below
; all 1 and 2 returned are special keys
;*************************************
Check_Keyb:
        setb    KEYB_DATA
        setb    KEYB_CLOCK              ; CLOCK &amp; DATA high = Idle Pos
        mov     r0, #50                 
CheckAgain:
        jnb     KEYB_CLOCK, KeyHit
        djnz    r0, CheckAgain          ; check r0 times
        sjmp    KeyEnd
KeyHit:
        jnb     KEYB_DATA, KeyHit2      ; Start bit must be 0
KeyEnd:
        clr     KEYB_CLOCK              ; disable keyb
        clr     c                       ; c=0 = no keypress
        ret
KeyHit2:
        mov     r0, #8          ; 8 bits
        clr     a
KeyHit3:
        call   Zero2One
        djnz    r0, KeyHit3
        mov     r1, a

        clr     a
        call   Zero2One        ; Parity bit
        call   Zero2One        ; Stop bit
        ; acc.7 = stop, acc.6 = parity
        clr     KEYB_CLOCK

        mov     a, KB_TEMP
        jz      NoIgnore        
        dec     KB_TEMP         ; Igonre pause scans
        sjmp    ChkKbEndNC

NoIgnore:
        mov     KB_SCAN, r1

        cjne    r1, #0FAH, NoKbACK
        orl     KB_STATS, #ACK
        sjmp    ChkKbEndNC

NoKbACK:
        cjne    r1, #KB_PAUSE, NoKbPause
        mov     KB_TEMP, #7             ; Ignore next 7 scans
        mov     a, KB_OK
        cpl     acc.6
        mov     KB_OK, a
        sjmp    ChkKbEndNC

NoKbPause:
        cjne    r1, #KB_EXT, NoKbExt
        orl     KB_OK, #EXTENDED
        sjmp    ChkKbEndNC

NoKbExt:
        cjne    r1, #KB_REL, NoRelease
        orl     KB_STATS, #RELEASE
        sjmp    ChkKbEndNC

NoRelease:
; Test Num lock, if pressed toggle led
        cjne    r1, #KB_NUML, NoNumLock
        mov     a, KB_STATS
        jnb     acc.7, ChkKbEndNC
        cpl     acc.1
        clr     acc.7
        mov     KB_STATS, a
        call   Keyb_Leds
        sjmp    ChkKbEndNC

NoNumLock:
; Test Caps lock, if pressed toggle led
        cjne    r1, #KB_CAPS, NoCapsLock
        mov     a, KB_STATS
        jnb     acc.7, ChkKbEndNC
        cpl     acc.2
        clr     acc.7
        mov     KB_STATS, a
        call   Keyb_Leds
        sjmp    ChkKbEndNC

NoCapsLock:
; Test Scroll lock, if pressed toggle led
        cjne    r1, #KB_SCROLL, NoScrollLock
        mov     a, KB_STATS
        jnb     acc.7, ChkKbEndNC
        cpl     acc.0
        clr     acc.7
        mov     KB_STATS, a
        call   Keyb_Leds
ChkKbEndNC:
        clr     c
        ret

NoScrollLock:
; Test L &amp; R shifts, set bit if pressed, clear on release
        cjne    r1, #KB_LSHIFT, NoShift1
ShiftOK:
        mov     a, KB_STATS
        jbc     acc.7, ShiftRel
        setb    acc.3           ; not releasing, so Set SHIFT bit
        sjmp    ShiftEnd
ShiftRel:
        clr     acc.3           ; releasing, so Clear SHIFT bit
ShiftEnd:
        mov     KB_STATS, a
        sjmp    ChkKbEndNC      
NoShift1:
        cjne    r1, #KB_RSHIFT, NoShift
        sjmp    ShiftOK

NoShift:
        cjne    r1, #KB_CTRL, NoCtrl
        mov     a, KB_STATS
        jbc     acc.7, CtrlRel
        setb    acc.5           ; not releasing, so Set CTRL bit
        sjmp    CtrlEnd
CtrlRel:
        clr     acc.5           ; releasing, so Clear SHIFT bit
CtrlEnd:
        mov     KB_STATS, a
        sjmp    ChkKbEndNC      

NoCtrl:
        cjne    r1, #KB_ALT, NoAlt
        mov     a, KB_STATS
        jbc     acc.7, AltRel
        setb    acc.6           ; not releasing, so Set ALT bit
        sjmp    AltEnd
AltRel:
        clr     acc.6           ; releasing, so Clear ALT bit
AltEnd:
        mov     KB_STATS, a
        sjmp    ChkKbEndNC      

NoAlt:
        mov     a, KB_STATS             ; Releasing key test
        jnb     acc.7, NoRel2           
        clr     acc.7                   ; if releasing &gt; clear
        mov     KB_STATS, a             ; rel bit on KB_STATS
        clr     c                      ; and do nothing 
        ret
NoRel2:
        mov     a, KB_OK                ; Extended key test
        jnb     acc.7, KbChars           
        clr     acc.7                   ; if Extended &gt; clear
        mov     KB_OK, a                ; EXT bit on KB_OK
        clr     c                    ; and do nothing 
        ret
KbChars:mov     dptr, #KbScanCodes
        mov     a, KB_STATS
        jnb     acc.2, TestShift
        jb      acc.3, KbChkOK
        mov     a, r1
        movc    a, @a+dptr
        mov     r0, a
        subb    a, #97
        jc      KbChkOK
        mov     a, r0
        subb    a, #123
        jnc     KbChkOK
        mov     dptr, #KbScanCodes2     ; if (a to z) &amp; Caps &gt; table 2
        sjmp    KbChkOK
TestShift:
        jnb     acc.3, KbChkOK
        mov     dptr, #KbScanCodes2     ; with shift table 2
KbChkOK:
        mov     a, r1
        movc    a, @a+dptr
        mov     KB_DATA, a
        setb    c
        ret

;*************************************
; Zero2One2: Wait for high to low in
; kb clock line
;*************************************
Zero2One2:
        jnb     KEYB_CLOCK, $
        jb      KEYB_CLOCK, $
        ret
;*************************************
; Write_Keyb: Send r1 to the kb
;*************************************
Write_Keyb:
        mov     r0, #8          ; 8 bits to receive
        clr     KEYB_CLOCK      ; break the Keyboard
        mov	r7, #00H       ; some delay (safety reasons)
_WKwait:djnz	r7, _WKwait
        clr     KEYB_DATA       ; request to send
        setb    KEYB_CLOCK      ; enable the Keyboard
        acall   Zero2One2       ; Start Bit
        mov     a, r1           ; Data Bits
TxData:
        rrc     a
        mov     KEYB_DATA, c
        call   Zero2One2
        djnz    r0, TxData

        mov     a, r1           ; calculate parity bit
        mov     c, psw.0        ; this is Even parity
        cpl     c               ; and Keyboard needs Odd parity
        mov     KEYB_DATA, c    ; send parity bit
        call   Zero2One2

        setb    KEYB_DATA       ; send stop bit
        call   Zero2One2

        call   Zero2One2
        mov     c, KEYB_DATA    ; get ACK bit
        clr     KEYB_CLOCK      ; stop the keyboard     
        ret
;######################################################################       
LAOP:		
LOAPA:	INC TEMP
			MOV R1,TEMP
			CJNE R1,#01H,IOP1					;GET DATE
		   CALL DATE_DISP
		   CALL LOOPS
		   MOV A,KB_DATA
		   CLR C
		   SUBB A,#30H
		   SWAP A
		   MOV DATE1,A
         CALL LOOPS
		   MOV A,KB_DATA
		   CLR C
		   SUBB A,#30H
		   ORL A,DATE1
		   MOV DATE1,A
         CALL DELAY
		   AJMP LAOP	
IOP1:		CJNE R1,#02H,IOP2					;GET MONTH
		   CALL MON_DISP
		   CALL LOOPS
		   MOV A,KB_DATA
		   CLR C
		   SUBB A,#30H
		   SWAP A
		   MOV MONTH,A
         CALL LOOPS
		   MOV A,KB_DATA
		   CLR C
		   SUBB A,#30H
		   ORL A,MONTH
		   MOV MONTH,A
		   CALL DELAY
		   AJMP LAOP		   
IOP2:		CJNE R1,#03H,IOP3					;GET YEAR
		   CALL YR_DISP
		   CALL LOOPS
		   MOV A,KB_DATA
		   CLR C
		   SUBB A,#30H
		   SWAP A
		   MOV YEAR,A
         CALL LOOPS
		   MOV A,KB_DATA
		   CLR C
		   SUBB A,#30H
		   ORL A,YEAR
		   MOV YEAR,A
		   CALL DELAY
		   AJMP LAOP		     
IOP3:		CJNE R1,#04H,IOP4					;GET DAY
		   CALL DAY_DISP
		   CALL LOOPS
		   MOV A,KB_DATA
		   CLR C
		   SUBB A,#30H
		   MOV DAY,A
		   CALL DELAY
		   AJMP LAOP		
IOP4:		CJNE R1,#05H,IOP5					;GET HOUR
		   CALL HR_DISP
		   CALL LOOPS
		   MOV A,KB_DATA
		   CLR C
		   SUBB A,#30H
		   SWAP A
		   MOV HRS,A
         CALL LOOPS
		   MOV A,KB_DATA
		   CLR C
		   SUBB A,#30H
		   ORL A,HRS
		   MOV HRS,A
		   CALL DELAY
		   AJMP LAOP	
IOP5:		CJNE R1,#06H,IOP6					;GET MIN
		   CALL MIN_DISP
		   CALL LOOPS
		   MOV A,KB_DATA
		   CLR C
		   SUBB A,#30H
		   SWAP A
		   MOV MINS,A
		   
		   MOV R4,KB_DATA
         CALL WRLCDDATA
         CALL LOOPS
		   MOV A,KB_DATA
		   CLR C
		   SUBB A,#30H
		   ORL A,MINS
		   MOV MINS,A
		   CALL DELAY
		   AJMP LAOP	  
IOP6:		CJNE R1,#07H,STORE_RTC					;GET DAY
		   CALL AMS_DISP
		   CALL LOOPS
		   MOV A,KB_DATA
		   CLR C
		   SUBB A,#30H
		   CJNE A,#00H,GBBN1
		   SETB AMS
		   CALL AM_DISP
		   AJMP GBBN2
GBBN1:	CLR AMS
			CALL PM_DISP
GBBN2:   CALL DELAY
			AJMP LAOP						   	          		   
LOOPS:   CALL Check_Keyb 
        	JNC LOOPS
        	MOV R4,KB_DATA
         CALL WRLCDDATA
        	RET          	
;(((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((
;		STORE THE TIME TO RTC CHIP
;(((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((	
STORE_RTC:
	LCALL SEND_START 		; SEND 2WIRE START CONDITION
	MOV A,#DS1307W 			; LOAD DS1307 WRITE COMMAND
	LCALL SEND_BYTE 		; SEND WRITE COMMAND
	MOV A,#08H 			; SET DS1307 DATA POINTER TO BEGINNING
	LCALL SEND_BYTE 		; OF USER RAM 08H
	MOV A,#0AAH 			; WRITE BYTE TO ENTIRE RAM SPACE
	LCALL SEND_BYTE 
	LCALL SEND_STOP 		; SEND 2WIRE STOP CONTION

	LCALL SEND_START 		; SEND 2WIRE START CONDITION
	MOV A,#DS1307W 			; LOAD DS1307 WRITE COMMAND
	LCALL SEND_BYTE 		; SEND WRITE COMMAND
	MOV A,#01H 			; SET DS1307 DATA POINTER TO BEGINNING
	LCALL SEND_BYTE 		; OF 00H
	MOV A,MINS			; Send min
	LCALL SEND_BYTE 		
	MOV A,HRS			;send hr
	SETB ACC.6        ;12 HR MODE
	JNB AMS,YUH
	CLR ACC.5			;AM/PM		1=PM,0=AM
	AJMP YUH1
YUH: SETB ACC.5	
YUH1:	LCALL SEND_BYTE 
	MOV A,DAY			; Send Day
	LCALL SEND_BYTE 		
	MOV A,DATE1			; Send date
	LCALL SEND_BYTE			
	MOV A,MONTH			; Send month
	LCALL SEND_BYTE
	MOV A,YEAR			; Send yr
	LCALL SEND_BYTE
	LCALL SEND_STOP 		; SEND 2WIRE STOP CONTION
	RET
;$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$	
DISP_NAME:
			
			MOV R4,NAMES
			CJNE R4,#0FFH,FV1
			RET
FV1:		CALL WRLCDDATA
			CALL MDELAY
			MOV R4,NAMES+1
			CJNE R4,#0FFH,FV2
			RET
FV2:		CALL WRLCDDATA
			CALL MDELAY
			MOV R4,NAMES+2
			CJNE R4,#0FFH,FV3
			RET
FV3:		CALL WRLCDDATA
			CALL MDELAY
			MOV R4,NAMES+3
			CJNE R4,#0FFH,FV4
			RET
FV4:		CALL WRLCDDATA
			CALL MDELAY
			MOV R4,NAMES+4
			CJNE R4,#0FFH,FV5
			RET
FV5:		CALL WRLCDDATA
			CALL MDELAY
			MOV R4,NAMES+5
			CJNE R4,#0FFH,FV6
			RET
FV6:		CALL WRLCDDATA
			CALL MDELAY
			MOV R4,NAMES+6
			CJNE R4,#0FFH,FV7
			RET
FV7:		CALL WRLCDDATA
			CALL MDELAY
			MOV R4,NAMES+7
			CJNE R4,#0FFH,FV8
			RET
FV8:		CALL WRLCDDATA
			CALL MDELAY
			MOV R4,NAMES+8
			CJNE R4,#0FFH,FV9
			RET
FV9:		CALL WRLCDDATA
			CALL MDELAY
			MOV R4,NAMES+9
			CJNE R4,#0FFH,FV10
			RET
FV10:		CALL WRLCDDATA
			CALL MDELAY
			MOV R4,NAMES+10
			CJNE R4,#0FFH,FV11
			RET
FV11:		CALL WRLCDDATA
			CALL MDELAY
			MOV R4,NAMES+11
			CJNE R4,#0FFH,FV12
			RET
FV12:		CALL WRLCDDATA
			CALL MDELAY
			MOV R4,NAMES+12
			CJNE R4,#0FFH,FV13
			RET
FV13:		CALL WRLCDDATA
			CALL MDELAY
			MOV R4,NAMES+13
			CJNE R4,#0FFH,FV14
			RET
FV14:		CALL WRLCDDATA
			CALL MDELAY
			MOV R4,NAMES+14
			CJNE R4,#0FFH,FV15
			RET
FV15:		CALL WRLCDDATA
			CALL MDELAY
			MOV R4,NAMES+15
			CJNE R4,#0FFH,FV16
			RET
FV16:		
			CALL WRLCDDATA
			CALL MDELAY
			RET
;$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$		
;
;$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$		
CLEAR_RAM:
		MOV NAMES,#0FFH
		MOV NAMES+1,#0FFH
		MOV NAMES+2,#0FFH
		MOV NAMES+3,#0FFH
		MOV NAMES+4,#0FFH
		MOV NAMES+5,#0FFH
		MOV NAMES+6,#0FFH
		MOV NAMES+7,#0FFH
		MOV NAMES+8,#0FFH
		MOV NAMES+9,#0FFH
		MOV NAMES+10,#0FFH
		MOV NAMES+11,#0FFH
		MOV NAMES+12,#0FFH
		MOV NAMES+13,#0FFH
		MOV NAMES+14,#0FFH
		MOV NAMES+15,#0FFH
		RET
;$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$		
DELETE_PGM:
			CALL READ_TOTAL
			MOV TEMPS,TOTAL		
			SETB UP 
			SETB DOWN
			SETB CANCEL
			SETB ENTER	
			MOV R5,TEMPS
			CJNE R5,#00H,EDT0
			CALL NO_CARD
			CALL DELAYS
			JMP REP1		
EDT0:		MOV R5,TEMPS	
			CJNE R5,#01H,EDT1
			MOV R1,#NAMES					;GET DATA IN BYTES(RAM)
			MOV R4,#30H						;MASTER ADDRESS
			MOV R6,#16						;NUMBER OF BYTES
			AJMP EDIT
EDT1:		CJNE R5,#02H,EDT2
			MOV R1,#NAMES					;GET DATA IN BYTES(RAM)
			MOV R4,#50H						;MASTER ADDRESS
			MOV R6,#16						;NUMBER OF BYTES
			AJMP EDIT
EDT2:		CJNE R5,#03H,EDT3
			MOV R1,#NAMES					;GET DATA IN BYTES(RAM)
			MOV R4,#70H						;MASTER ADDRESS
			MOV R6,#16						;NUMBER OF BYTES
			AJMP EDIT
EDT3:		CJNE R5,#04H,EDT4
			MOV R1,#NAMES					;GET DATA IN BYTES(RAM)
			MOV R4,#90H						;MASTER ADDRESS
			MOV R6,#16						;NUMBER OF BYTES
			AJMP EDIT
EDT4:		CJNE R5,#05H,EDT5
			MOV R1,#NAMES					;GET DATA IN BYTES(RAM)
			MOV R4,#0B0H						;MASTER ADDRESS
			MOV R6,#16						;NUMBER OF BYTES
			AJMP EDIT
EDT5:		CJNE R5,#06H,EDT6
			MOV R1,#NAMES					;GET DATA IN BYTES(RAM)
			MOV R4,#0D0H						;MASTER ADDRESS
			MOV R6,#16						;NUMBER OF BYTES
			AJMP EDIT
EDT6:		CJNE R5,#07H,EDIT
			MOV R1,#NAMES					;GET DATA IN BYTES(RAM)
			MOV R4,#0F0H						;MASTER ADDRESS
			MOV R6,#16						;NUMBER OF BYTES
			AJMP EDIT

			
EDIT:		CALL READ_EEPROM
			CALL DISPLAY5
			CALL DISP_NAME
EDIT7:	JNB UP,GBN2
			JNB CANCEL,VFR3
			JNB DOWN,GBN1
			JNB ENTER,EDIT_NAM
			AJMP EDIT7
			
VFR3:		CALL DELAY
			JNB CANCEL,$
			JMP REP1	
	
GBN2:		JNB UP,$
			MOV A,TEMPS
			CJNE A,#01H,VFR2
			MOV TEMPS,TOTAL
			JMP EDT0
VFR2:		DEC TEMPS
			JMP EDT0			
					
GBN1:		JNB DOWN,$
			MOV A,TEMPS
			XRL A,TOTAL
			JNZ VFR1
			MOV TEMPS,#01H
			JMP EDT0
VFR1:		INC TEMPS
			JMP EDT0		
			
KBNotOKAS2: JMP KBNotOKAS		
;%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%		
EDIT_NAM:
			JB KEYBRD,LAOP1
FDAC4:	CALL InitKeyb				;Inizialize Keyboard 
      	MOV a, KB_OK
      	JZ KBNotOKAS2   
      	SETB KEYBRD 
      		
LAOP1:   CALL DISPLAY4		;plz enter NEW name
			MOV TEMP,#00H
			CALL CLEAR_RAM
EADC:		CALL LOOPZ
        	INC TEMP
        	MOV R2,TEMP
        	CJNE R2,#01H,EADC1
        	MOV R1,#NAMES
        	MOV @R1,KB_DATA
EADC1:		CJNE R2,#02H,EADC2
        	MOV R1,#NAMES+1			
        	MOV @R1,KB_DATA
EADC2:		CJNE R2,#03H,EADC3
        	MOV R1,#NAMES+2
        	MOV @R1,KB_DATA		
EADC3:		CJNE R2,#04H,EADC4
        	MOV R1,#NAMES+3
        	MOV @R1,KB_DATA		
EADC4:		CJNE R2,#05H,EADC5
        	MOV R1,#NAMES+4
        	MOV @R1,KB_DATA	
EADC5:		CJNE R2,#06H,EADC6
        	MOV R1,#NAMES+5
        	MOV @R1,KB_DATA		
EADC6:		CJNE R2,#07H,EADC7
        	MOV R1,#NAMES+6
        	MOV @R1,KB_DATA	
EADC7:		CJNE R2,#08H,EADC8
        	MOV R1,#NAMES+7
        	MOV @R1,KB_DATA	
EADC8:		CJNE R2,#09H,EADC9
        	MOV R1,#NAMES+8
        	MOV @R1,KB_DATA	
EADC9:		CJNE R2,#10,EADC10
        	MOV R1,#NAMES+9
        	MOV @R1,KB_DATA	
EADC10:	CJNE R2,#11,EADC11
        	MOV R1,#NAMES+10
        	MOV @R1,KB_DATA	
EADC11:	CJNE R2,#12,EADC12
        	MOV R1,#NAMES+11
        	MOV @R1,KB_DATA
EADC12:	CJNE R2,#13,EADC13
        	MOV R1,#NAMES+12
        	MOV @R1,KB_DATA
EADC13:	CJNE R2,#14,EADC14
        	MOV R1,#NAMES+13
        	MOV @R1,KB_DATA
EADC14:	CJNE R2,#15,EADC15
        	MOV R1,#NAMES+14
        	MOV @R1,KB_DATA
EADC15:	CJNE R2,#16,EADC16
        	MOV R1,#NAMES+15
        	MOV @R1,KB_DATA
EADC16:	CJNE R2,#17,EADC       	
        	CALL DELAYS
        	
        	CALL PRESS_ENTER
        	SETB ENTER
        	SETB CANCEL
HHJS:    JNB CANCEL,LAOPZ1
        	JB ENTER,HHJS    
        	
        	
        	MOV A,TEMPS
			CJNE A,#01H,JANM1
			MOV R1,#NAMES			;store Name
			MOV R4,#30H			;Starting Address
			MOV R6,#16			;STORE 16 BYTES
			CALL STORE_EEPROM	
			AJMP JANM	
			
LAOPZ1:	JMP LAOP1
					
JANM1:		CJNE A,#02H,JANM2
			MOV R1,#NAMES			;store Name
			MOV R4,#50H			;Starting Address
			MOV R6,#16			;STORE 16 BYTES
			CALL STORE_EEPROM		
			AJMP JANM		

JANM2:		CJNE A,#03H,JANM3
			MOV R1,#NAMES			;store Name
			MOV R4,#70H			;Starting Address
			MOV R6,#16			;STORE 16 BYTES
			CALL STORE_EEPROM	
			AJMP JANM			

JANM3:		CJNE A,#04H,JANM4
			MOV R1,#NAMES			;store Name
			MOV R4,#90H			;Starting Address
			MOV R6,#16			;STORE 16 BYTES
			CALL STORE_EEPROM	
			AJMP JANM						

JANM4:		CJNE A,#05H,JANM5
			MOV R1,#NAMES			;store Name
			MOV R4,#0B0H			;Starting Address
			MOV R6,#16			;STORE 16 BYTES
			CALL STORE_EEPROM	
			AJMP JANM						
						
JANM5:		CJNE A,#06H,JANM6
			MOV R1,#NAMES			;store Name
			MOV R4,#0D0H			;Starting Address
			MOV R6,#16			;STORE 16 BYTES
			CALL STORE_EEPROM	
			AJMP JANM		
			
JANM6:		CJNE A,#07H,JANM
			MOV R1,#NAMES			;store Name
			MOV R4,#0F0H			;Starting Address
			MOV R6,#16			;STORE 16 BYTES
			CALL STORE_EEPROM	
			AJMP JANM													
JANM:		CALL ID_SAVED			;Tag stored sucessfully	
			CALL DELAYS
			JNB ENTER,$
			JMP DELETE_PGM



        	
    		      	
        
LoopZ:    CALL Check_Keyb 
			JNB ENTER,GAHBH
			JNB CANCEL,DCFAV1
        	JNC LoopZ
        	MOV R4,KB_DATA
         CALL WRLCDDATA
         RET
GAHBH:		JNB ENTER,$
			MOV TEMP,#16
			RET	    
			
DCFAV1:	JNB CANCEL,$
			JMP DELETE_PGM  
			
KBNotOKAS:
			CALL DISP1
			JB ENTER,$
			NOP
			JNB ENTER,$
			AJMP FDAC4			
;$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$		
; last 262 addr of code mem with scan codes tables
org    1E00H
KbScanCodes:
; Keyboard Scancodes
;       ?,  F9, ?,  F5,  F4,  F1,  F2, F12, ?, F10,  F8,  F6,  F4, TAB,  ~
db     0,  1 , 0,  1 ,  1 ,  1 ,  1 ,  1 , 0,  1 ,  1 ,  1 ,  1 ,  1 , '~'
;       ?, ?,Lalt,Lshf, ?,Lctr,  Q ,  ! , ?, ?, ?,  Z ,  S ,  A ,  W ,  @
db     0, 0,  0 ,  0 , 0,  0 , 'q', '1', 0, 0, 0, 'z', 's', 'a', 'w', '2'
;       ?, ?,  C ,  X ,  D ,  E ,  $ ,  # , ?, ?, " ",  V ,  F ,  T ,  R
db     0, 0, 'c', 'x', 'd', 'e', '4', '3', 0, 0, ' ', 'v', 'f', 't', 'r'
;       % , ?, ?,  N ,  B ,  H ,  G ,  Y ,  ^ , ?, ?, ?,  M ,  J ,  U ,  &
db    '5', 0, 0, 'n', 'b', 'h', 'g', 'y', '6', 0, 0, 0, 'm', 'j', 'u', '7'
;       * , ?, ?,  < ,  K ,  I ,  O ,  ) ,  ( , ?, ?,  > ,  ? ,  L ,  : ,  P
db    '8', 0, 0, ',', 'k', 'i', 'o', '0', '9', 0, 0, '.', '/', 'l', ';', 'p'
;       _ , ?, ?, ?,  " , ?,  { ,  + , ?, ?,Caps,Rshf,Entr,  } , ?,  | 
db    '-', 0, 0, 0, 39 ,  0,  '[',  '=', 0, 0,  0 ,  0 ,  1 , ']', 0,  92
;       ?, ?, ?, ?, ?, ?, ?, ?,BkSp, ?, ?,  1 , ?,  4 ,  7 , ?, ?, ?,  0
db     0, 0, 0, 0, 0, 0, 0, 0,  1 , 0, 0, '1', 0, '4', '7', 0, 0, 0, '0'
;       . ,  2 ,  5 ,  6 ,  8 , ESC,Numl, F11,  + ,  3 ,  - ,  * ,  9 ,Scrl
db    '.', '2', '5', '6', '8',  1 ,  0 ,  1 , '+', '3', '-', '*', '9',  0 
;       ?, ?, ?, ?,  F7
db     0, 0, 0, 0,  1

KbScanCodes2:
; Keyboard Scancodes with shift
;       ?,  F9, ?,  F5,  F4,  F1,  F2, F12, ?, F10,  F8,  F6,  F4, TAB,  ~
db     0,  2 , 0,  2 ,  2 ,  2 ,  2 ,  2 , 0,  2 ,  2 ,  2 ,  2 ,  2 , '`'
;       ?, ?,Lalt,Lshf, ?,Lctr,  Q ,  ! , ?, ?, ?,  Z ,  S ,  A ,  W ,  @
db     0, 0,  0 ,  0 , 0,  0 , 'Q', '!', 0, 0, 0, 'Z', 'S', 'A', 'W', '@'
;       ?, ?,  C ,  X ,  D ,  E ,  $ ,  # , ?, ?, " ",  V ,  F ,  T ,  R
db     0, 0, 'C', 'X', 'D', 'E', '$', '#', 0, 0, ' ', 'V', 'F', 'T', 'R'
;       % , ?, ?,  N ,  B ,  H ,  G ,  Y ,  ^ , ?, ?, ?,  M ,  J ,  U ,  &
db    '%', 0, 0, 'N', 'B', 'H', 'G', 'Y', '^', 0, 0, 0, 'M', 'J', 'U', '&'
;       * , ?, ?,  < ,  K ,  I ,  O ,  ) ,  ( , ?, ?,  > ,  ? ,  L ,  : ,  P
db    '*', 0, 0, '<', 'K', 'I', 'O', ')', '(', 0, 0, '>', '?', 'L', ':', 'P'
;       _ , ?, ?, ?,  " , ?,  { ,  + , ?, ?,Caps,Rshf,Entr,  } , ?,  | 
db    '_', 0, 0, 0, '"', 0, '{', '+', 0, 0,  0 ,  0 ,  2 , '}', 0, '|'
;       ?, ?, ?, ?, ?, ?, ?, ?,BkSp, ?, ?,  1 , ?,  4 ,  7 , ?, ?, ?,  0
db     0, 0, 0, 0, 0, 0, 0, 0,  2 , 0, 0, '1', 0, '4', '7', 0, 0, 0, '0'
;       . ,  2 ,  5 ,  6 ,  8 , ESC,Numl, F11,  + ,  3 ,  - ,  * ,  9 ,Scrl
db    '.', '2', '5', '6', '8',  2 ,  0 ,  2 , '+', '3', '-', '*', '9',  0
;       ?, ?, ?, ?,  F7
db     0, 0, 0, 0,  2    	   	
END
  

