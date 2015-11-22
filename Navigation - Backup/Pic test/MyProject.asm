
_main:
	MOV	#2048, W15
	MOV	#6142, W0
	MOV	WREG, 32
	MOV	#1, W0
	MOV	WREG, 52
	MOV	#4, W0
	IOR	68
	LNK	#12

;MyProject.mbas,6 :: 		main:
;MyProject.mbas,9 :: 		TRISC.0 = 1
	PUSH	W10
	PUSH	W11
	PUSH	W12
	BSET	TRISC, #0
;MyProject.mbas,10 :: 		TRISC.1 = 1
	BSET	TRISC, #1
;MyProject.mbas,11 :: 		TRISC.2 = 1
	BSET	TRISC, #2
;MyProject.mbas,12 :: 		TRISA.1 = 1
	BSET	TRISA, #1
;MyProject.mbas,13 :: 		TRISA.0 = 1
	BSET	TRISA, #0
;MyProject.mbas,14 :: 		TRISB.4 = 0
	BCLR	TRISB, #4
;MyProject.mbas,15 :: 		TRISA.4 = 0
	BCLR	TRISA, #4
;MyProject.mbas,16 :: 		TRISB.8 = 0
	BCLR	TRISB, #8
;MyProject.mbas,17 :: 		TRISA.9 = 0
	BCLR	TRISA, #9
;MyProject.mbas,18 :: 		TRISC.3 = 0
	BCLR	TRISC, #3
;MyProject.mbas,19 :: 		TRISC.4 = 0
	BCLR	TRISC, #4
;MyProject.mbas,20 :: 		TRISC.5 = 0
	BCLR	TRISC, #5
;MyProject.mbas,21 :: 		TRISB.9 = 0
	BCLR	TRISB, #9
;MyProject.mbas,22 :: 		TRISB.12 = 0
	BCLR	TRISB, #12
;MyProject.mbas,23 :: 		TRISA.10 = 0
	BCLR	TRISA, #10
;MyProject.mbas,24 :: 		TRISA.7 = 0
	BCLR	TRISA, #7
;MyProject.mbas,25 :: 		TRISA.9=0
	BCLR	TRISA, #9
;MyProject.mbas,26 :: 		LATB.4 = 1
	BSET	LATB, #4
;MyProject.mbas,27 :: 		LATA.4 = 1
	BSET	LATA, #4
;MyProject.mbas,28 :: 		LATB.8 = 1
	BSET	LATB, #8
;MyProject.mbas,29 :: 		LATA.9 = 1
	BSET	LATA, #9
;MyProject.mbas,30 :: 		LATC.3 = 1
	BSET	LATC, #3
;MyProject.mbas,31 :: 		LATC.4 = 1
	BSET	LATC, #4
;MyProject.mbas,32 :: 		LATC.5 = 1
	BSET	LATC, #5
;MyProject.mbas,33 :: 		LATB.9 = 1
	BSET	LATB, #9
;MyProject.mbas,34 :: 		LATB.12 = 1
	BSET	LATB, #12
;MyProject.mbas,35 :: 		LATA.10 = 1
	BSET	LATA, #10
;MyProject.mbas,36 :: 		LATA.7 = 1
	BSET	LATA, #7
;MyProject.mbas,37 :: 		LATA.9=1
	BSET	LATA, #9
;MyProject.mbas,39 :: 		Unlock_IOLOCK()
	CALL	_Unlock_IOLOCK
;MyProject.mbas,41 :: 		PPS_Mapping_NoLock(23, _INPUT, _U1RX)   ' Sets pin 2 of PIC as USB RX input
	MOV.B	#11, W12
	MOV.B	#1, W11
	MOV.B	#23, W10
	CALL	_PPS_Mapping_NoLock
;MyProject.mbas,42 :: 		PPS_Mapping_NoLock(22, _OUTPUT, _U1TX)  ' Sets pin 3 of PIC as USB TX output
	MOV.B	#3, W12
	CLR	W11
	MOV.B	#22, W10
	CALL	_PPS_Mapping_NoLock
;MyProject.mbas,43 :: 		PPS_Mapping_NoLock(19, _INPUT, _U2RX)   ' Sets pin 4 of PIC as RS485 RX input
	MOV.B	#13, W12
	MOV.B	#1, W11
	MOV.B	#19, W10
	CALL	_PPS_Mapping_NoLock
;MyProject.mbas,44 :: 		PPS_Mapping_NoLock(20, _OUTPUT, _U2TX)  ' Sets pin 5 of PIC as RS485 TX output
	MOV.B	#5, W12
	CLR	W11
	MOV.B	#20, W10
	CALL	_PPS_Mapping_NoLock
;MyProject.mbas,45 :: 		Lock_IOLOCK()
	CALL	_Lock_IOLOCK
;MyProject.mbas,49 :: 		ADC1_Init()          ' Enable ADC module
	CALL	_ADC1_Init
;MyProject.mbas,50 :: 		UART1_Init(9600)     ' Initialize UART communication
	MOV	#9600, W10
	MOV	#0, W11
	CALL	_UART1_Init
;MyProject.mbas,52 :: 		while TRUE
L__main2:
;MyProject.mbas,53 :: 		ADCresult = ADC1_Get_Sample(1)   ' Acquire ADC sample
	MOV	#1, W10
	CALL	_ADC1_Get_Sample
	MOV	W0, _ADCresult
;MyProject.mbas,54 :: 		WordToStr(ADCresult, txt)        ' convert its value to string
	MOV	#lo_addr(_txt), W11
	MOV	W0, W10
	CALL	_WordToStr
;MyProject.mbas,55 :: 		UART1_Write_Text(" ADC 1 = ")
	ADD	W14, #0, W1
	MOV.B	#32, W0
	MOV.B	W0, [W1++]
	MOV.B	#65, W0
	MOV.B	W0, [W1++]
	MOV.B	#68, W0
	MOV.B	W0, [W1++]
	MOV.B	#67, W0
	MOV.B	W0, [W1++]
	MOV.B	#32, W0
	MOV.B	W0, [W1++]
	MOV.B	#49, W0
	MOV.B	W0, [W1++]
	MOV.B	#32, W0
	MOV.B	W0, [W1++]
	MOV.B	#61, W0
	MOV.B	W0, [W1++]
	MOV.B	#32, W0
	MOV.B	W0, [W1++]
	CLR	W0
	MOV.B	W0, [W1++]
	ADD	W14, #0, W0
	MOV	W0, W10
	CALL	_UART1_Write_Text
;MyProject.mbas,56 :: 		UART1_Write_Text(txt)            ' and send it to UART terminal
	MOV	#lo_addr(_txt), W10
	CALL	_UART1_Write_Text
;MyProject.mbas,57 :: 		UART1_Write_Text("   ")
	ADD	W14, #0, W1
	MOV.B	#32, W0
	MOV.B	W0, [W1++]
	MOV.B	#32, W0
	MOV.B	W0, [W1++]
	MOV.B	#32, W0
	MOV.B	W0, [W1++]
	CLR	W0
	MOV.B	W0, [W1++]
	ADD	W14, #0, W0
	MOV	W0, W10
	CALL	_UART1_Write_Text
;MyProject.mbas,58 :: 		Delay_ms(50)
	MOV	#4, W8
	MOV	#11725, W7
L__main6:
	DEC	W7
	BRA NZ	L__main6
	DEC	W8
	BRA NZ	L__main6
	NOP
;MyProject.mbas,59 :: 		ADCresult = ADC1_Get_Sample(2)   ' Acquire ADC sample
	MOV	#2, W10
	CALL	_ADC1_Get_Sample
	MOV	W0, _ADCresult
;MyProject.mbas,60 :: 		WordToStr(ADCresult, txt)        ' convert its value to string
	MOV	#lo_addr(_txt), W11
	MOV	W0, W10
	CALL	_WordToStr
;MyProject.mbas,61 :: 		UART1_Write_Text(" ADC 2 = ")
	ADD	W14, #0, W1
	MOV.B	#32, W0
	MOV.B	W0, [W1++]
	MOV.B	#65, W0
	MOV.B	W0, [W1++]
	MOV.B	#68, W0
	MOV.B	W0, [W1++]
	MOV.B	#67, W0
	MOV.B	W0, [W1++]
	MOV.B	#32, W0
	MOV.B	W0, [W1++]
	MOV.B	#50, W0
	MOV.B	W0, [W1++]
	MOV.B	#32, W0
	MOV.B	W0, [W1++]
	MOV.B	#61, W0
	MOV.B	W0, [W1++]
	MOV.B	#32, W0
	MOV.B	W0, [W1++]
	CLR	W0
	MOV.B	W0, [W1++]
	ADD	W14, #0, W0
	MOV	W0, W10
	CALL	_UART1_Write_Text
;MyProject.mbas,62 :: 		UART1_Write_Text(txt)            ' and send it to UART terminal
	MOV	#lo_addr(_txt), W10
	CALL	_UART1_Write_Text
;MyProject.mbas,63 :: 		UART1_Write_Text("   ")
	ADD	W14, #0, W1
	MOV.B	#32, W0
	MOV.B	W0, [W1++]
	MOV.B	#32, W0
	MOV.B	W0, [W1++]
	MOV.B	#32, W0
	MOV.B	W0, [W1++]
	CLR	W0
	MOV.B	W0, [W1++]
	ADD	W14, #0, W0
	MOV	W0, W10
	CALL	_UART1_Write_Text
;MyProject.mbas,64 :: 		Delay_ms(50)
	MOV	#4, W8
	MOV	#11725, W7
L__main8:
	DEC	W7
	BRA NZ	L__main8
	DEC	W8
	BRA NZ	L__main8
	NOP
;MyProject.mbas,65 :: 		ADCresult = ADC1_Get_Sample(3)   ' Acquire ADC sample
	MOV	#3, W10
	CALL	_ADC1_Get_Sample
	MOV	W0, _ADCresult
;MyProject.mbas,66 :: 		WordToStr(ADCresult, txt)        ' convert its value to string
	MOV	#lo_addr(_txt), W11
	MOV	W0, W10
	CALL	_WordToStr
;MyProject.mbas,67 :: 		UART1_Write_Text(" ADC 3 = ")
	ADD	W14, #0, W1
	MOV.B	#32, W0
	MOV.B	W0, [W1++]
	MOV.B	#65, W0
	MOV.B	W0, [W1++]
	MOV.B	#68, W0
	MOV.B	W0, [W1++]
	MOV.B	#67, W0
	MOV.B	W0, [W1++]
	MOV.B	#32, W0
	MOV.B	W0, [W1++]
	MOV.B	#51, W0
	MOV.B	W0, [W1++]
	MOV.B	#32, W0
	MOV.B	W0, [W1++]
	MOV.B	#61, W0
	MOV.B	W0, [W1++]
	MOV.B	#32, W0
	MOV.B	W0, [W1++]
	CLR	W0
	MOV.B	W0, [W1++]
	ADD	W14, #0, W0
	MOV	W0, W10
	CALL	_UART1_Write_Text
;MyProject.mbas,68 :: 		UART1_Write_Text(txt)            ' and send it to UART terminal
	MOV	#lo_addr(_txt), W10
	CALL	_UART1_Write_Text
;MyProject.mbas,69 :: 		UART1_Write_Text("   ")
	ADD	W14, #0, W1
	MOV.B	#32, W0
	MOV.B	W0, [W1++]
	MOV.B	#32, W0
	MOV.B	W0, [W1++]
	MOV.B	#32, W0
	MOV.B	W0, [W1++]
	CLR	W0
	MOV.B	W0, [W1++]
	ADD	W14, #0, W0
	MOV	W0, W10
	CALL	_UART1_Write_Text
;MyProject.mbas,70 :: 		Delay_ms(50)
	MOV	#4, W8
	MOV	#11725, W7
L__main10:
	DEC	W7
	BRA NZ	L__main10
	DEC	W8
	BRA NZ	L__main10
	NOP
;MyProject.mbas,71 :: 		ADCresult = ADC1_Get_Sample(4)   ' Acquire ADC sample
	MOV	#4, W10
	CALL	_ADC1_Get_Sample
	MOV	W0, _ADCresult
;MyProject.mbas,72 :: 		WordToStr(ADCresult, txt)        ' convert its value to string
	MOV	#lo_addr(_txt), W11
	MOV	W0, W10
	CALL	_WordToStr
;MyProject.mbas,73 :: 		UART1_Write_Text(" ADC 4 = ")
	ADD	W14, #0, W1
	MOV.B	#32, W0
	MOV.B	W0, [W1++]
	MOV.B	#65, W0
	MOV.B	W0, [W1++]
	MOV.B	#68, W0
	MOV.B	W0, [W1++]
	MOV.B	#67, W0
	MOV.B	W0, [W1++]
	MOV.B	#32, W0
	MOV.B	W0, [W1++]
	MOV.B	#52, W0
	MOV.B	W0, [W1++]
	MOV.B	#32, W0
	MOV.B	W0, [W1++]
	MOV.B	#61, W0
	MOV.B	W0, [W1++]
	MOV.B	#32, W0
	MOV.B	W0, [W1++]
	CLR	W0
	MOV.B	W0, [W1++]
	ADD	W14, #0, W0
	MOV	W0, W10
	CALL	_UART1_Write_Text
;MyProject.mbas,74 :: 		UART1_Write_Text(txt)            ' and send it to UART terminal
	MOV	#lo_addr(_txt), W10
	CALL	_UART1_Write_Text
;MyProject.mbas,75 :: 		UART1_Write_Text("   ")
	ADD	W14, #0, W1
	MOV.B	#32, W0
	MOV.B	W0, [W1++]
	MOV.B	#32, W0
	MOV.B	W0, [W1++]
	MOV.B	#32, W0
	MOV.B	W0, [W1++]
	CLR	W0
	MOV.B	W0, [W1++]
	ADD	W14, #0, W0
	MOV	W0, W10
	CALL	_UART1_Write_Text
;MyProject.mbas,76 :: 		Delay_ms(50)
	MOV	#4, W8
	MOV	#11725, W7
L__main12:
	DEC	W7
	BRA NZ	L__main12
	DEC	W8
	BRA NZ	L__main12
	NOP
;MyProject.mbas,77 :: 		ADCresult = ADC1_Get_Sample(5)   ' Acquire ADC sample
	MOV	#5, W10
	CALL	_ADC1_Get_Sample
	MOV	W0, _ADCresult
;MyProject.mbas,78 :: 		WordToStr(ADCresult, txt)        ' convert its value to string
	MOV	#lo_addr(_txt), W11
	MOV	W0, W10
	CALL	_WordToStr
;MyProject.mbas,79 :: 		UART1_Write_Text(" ADC 5 = ")
	ADD	W14, #0, W1
	MOV.B	#32, W0
	MOV.B	W0, [W1++]
	MOV.B	#65, W0
	MOV.B	W0, [W1++]
	MOV.B	#68, W0
	MOV.B	W0, [W1++]
	MOV.B	#67, W0
	MOV.B	W0, [W1++]
	MOV.B	#32, W0
	MOV.B	W0, [W1++]
	MOV.B	#53, W0
	MOV.B	W0, [W1++]
	MOV.B	#32, W0
	MOV.B	W0, [W1++]
	MOV.B	#61, W0
	MOV.B	W0, [W1++]
	MOV.B	#32, W0
	MOV.B	W0, [W1++]
	CLR	W0
	MOV.B	W0, [W1++]
	ADD	W14, #0, W0
	MOV	W0, W10
	CALL	_UART1_Write_Text
;MyProject.mbas,80 :: 		UART1_Write_Text(txt)            ' and send it to UART terminal
	MOV	#lo_addr(_txt), W10
	CALL	_UART1_Write_Text
;MyProject.mbas,81 :: 		UART1_Write_Text("   ")
	ADD	W14, #0, W1
	MOV.B	#32, W0
	MOV.B	W0, [W1++]
	MOV.B	#32, W0
	MOV.B	W0, [W1++]
	MOV.B	#32, W0
	MOV.B	W0, [W1++]
	CLR	W0
	MOV.B	W0, [W1++]
	ADD	W14, #0, W0
	MOV	W0, W10
	CALL	_UART1_Write_Text
;MyProject.mbas,82 :: 		UART1_Write_Text("   ")
	ADD	W14, #0, W1
	MOV.B	#32, W0
	MOV.B	W0, [W1++]
	MOV.B	#32, W0
	MOV.B	W0, [W1++]
	MOV.B	#32, W0
	MOV.B	W0, [W1++]
	CLR	W0
	MOV.B	W0, [W1++]
	ADD	W14, #0, W0
	MOV	W0, W10
	CALL	_UART1_Write_Text
;MyProject.mbas,83 :: 		Delay_ms(50)
	MOV	#4, W8
	MOV	#11725, W7
L__main14:
	DEC	W7
	BRA NZ	L__main14
	DEC	W8
	BRA NZ	L__main14
	NOP
;MyProject.mbas,84 :: 		ADCresult = ADC1_Get_Sample(6)   ' Acquire ADC sample
	MOV	#6, W10
	CALL	_ADC1_Get_Sample
	MOV	W0, _ADCresult
;MyProject.mbas,85 :: 		WordToStr(ADCresult, txt)        ' convert its value to string
	MOV	#lo_addr(_txt), W11
	MOV	W0, W10
	CALL	_WordToStr
;MyProject.mbas,86 :: 		UART1_Write_Text(" ADC 6 = ")
	ADD	W14, #0, W1
	MOV.B	#32, W0
	MOV.B	W0, [W1++]
	MOV.B	#65, W0
	MOV.B	W0, [W1++]
	MOV.B	#68, W0
	MOV.B	W0, [W1++]
	MOV.B	#67, W0
	MOV.B	W0, [W1++]
	MOV.B	#32, W0
	MOV.B	W0, [W1++]
	MOV.B	#54, W0
	MOV.B	W0, [W1++]
	MOV.B	#32, W0
	MOV.B	W0, [W1++]
	MOV.B	#61, W0
	MOV.B	W0, [W1++]
	MOV.B	#32, W0
	MOV.B	W0, [W1++]
	CLR	W0
	MOV.B	W0, [W1++]
	ADD	W14, #0, W0
	MOV	W0, W10
	CALL	_UART1_Write_Text
;MyProject.mbas,87 :: 		UART1_Write_Text(txt)            ' and send it to UART terminal
	MOV	#lo_addr(_txt), W10
	CALL	_UART1_Write_Text
;MyProject.mbas,88 :: 		UART1_Write_Text("   ")
	ADD	W14, #0, W1
	MOV.B	#32, W0
	MOV.B	W0, [W1++]
	MOV.B	#32, W0
	MOV.B	W0, [W1++]
	MOV.B	#32, W0
	MOV.B	W0, [W1++]
	CLR	W0
	MOV.B	W0, [W1++]
	ADD	W14, #0, W0
	MOV	W0, W10
	CALL	_UART1_Write_Text
;MyProject.mbas,89 :: 		Delay_ms(50)
	MOV	#4, W8
	MOV	#11725, W7
L__main16:
	DEC	W7
	BRA NZ	L__main16
	DEC	W8
	BRA NZ	L__main16
	NOP
;MyProject.mbas,90 :: 		ADCresult = ADC1_Get_Sample(7)   ' Acquire ADC sample
	MOV	#7, W10
	CALL	_ADC1_Get_Sample
	MOV	W0, _ADCresult
;MyProject.mbas,91 :: 		WordToStr(ADCresult, txt)        ' convert its value to string
	MOV	#lo_addr(_txt), W11
	MOV	W0, W10
	CALL	_WordToStr
;MyProject.mbas,92 :: 		UART1_Write_Text(" ADC 7 = ")
	ADD	W14, #0, W1
	MOV.B	#32, W0
	MOV.B	W0, [W1++]
	MOV.B	#65, W0
	MOV.B	W0, [W1++]
	MOV.B	#68, W0
	MOV.B	W0, [W1++]
	MOV.B	#67, W0
	MOV.B	W0, [W1++]
	MOV.B	#32, W0
	MOV.B	W0, [W1++]
	MOV.B	#55, W0
	MOV.B	W0, [W1++]
	MOV.B	#32, W0
	MOV.B	W0, [W1++]
	MOV.B	#61, W0
	MOV.B	W0, [W1++]
	MOV.B	#32, W0
	MOV.B	W0, [W1++]
	CLR	W0
	MOV.B	W0, [W1++]
	ADD	W14, #0, W0
	MOV	W0, W10
	CALL	_UART1_Write_Text
;MyProject.mbas,93 :: 		UART1_Write_Text(txt)            ' and send it to UART terminal
	MOV	#lo_addr(_txt), W10
	CALL	_UART1_Write_Text
;MyProject.mbas,94 :: 		UART1_Write_Text("   ")
	ADD	W14, #0, W1
	MOV.B	#32, W0
	MOV.B	W0, [W1++]
	MOV.B	#32, W0
	MOV.B	W0, [W1++]
	MOV.B	#32, W0
	MOV.B	W0, [W1++]
	CLR	W0
	MOV.B	W0, [W1++]
	ADD	W14, #0, W0
	MOV	W0, W10
	CALL	_UART1_Write_Text
;MyProject.mbas,95 :: 		Delay_ms(50)
	MOV	#4, W8
	MOV	#11725, W7
L__main18:
	DEC	W7
	BRA NZ	L__main18
	DEC	W8
	BRA NZ	L__main18
	NOP
;MyProject.mbas,96 :: 		ADCresult = ADC1_Get_Sample(8)   ' Acquire ADC sample
	MOV	#8, W10
	CALL	_ADC1_Get_Sample
	MOV	W0, _ADCresult
;MyProject.mbas,97 :: 		WordToStr(ADCresult, txt)        ' convert its value to string
	MOV	#lo_addr(_txt), W11
	MOV	W0, W10
	CALL	_WordToStr
;MyProject.mbas,98 :: 		UART1_Write_Text(" ADC 8 = ")
	ADD	W14, #0, W1
	MOV.B	#32, W0
	MOV.B	W0, [W1++]
	MOV.B	#65, W0
	MOV.B	W0, [W1++]
	MOV.B	#68, W0
	MOV.B	W0, [W1++]
	MOV.B	#67, W0
	MOV.B	W0, [W1++]
	MOV.B	#32, W0
	MOV.B	W0, [W1++]
	MOV.B	#56, W0
	MOV.B	W0, [W1++]
	MOV.B	#32, W0
	MOV.B	W0, [W1++]
	MOV.B	#61, W0
	MOV.B	W0, [W1++]
	MOV.B	#32, W0
	MOV.B	W0, [W1++]
	CLR	W0
	MOV.B	W0, [W1++]
	ADD	W14, #0, W0
	MOV	W0, W10
	CALL	_UART1_Write_Text
;MyProject.mbas,99 :: 		UART1_Write_Text(txt)            ' and send it to UART terminal
	MOV	#lo_addr(_txt), W10
	CALL	_UART1_Write_Text
;MyProject.mbas,100 :: 		UART1_Write_Text("   ")
	ADD	W14, #0, W1
	MOV.B	#32, W0
	MOV.B	W0, [W1++]
	MOV.B	#32, W0
	MOV.B	W0, [W1++]
	MOV.B	#32, W0
	MOV.B	W0, [W1++]
	CLR	W0
	MOV.B	W0, [W1++]
	ADD	W14, #0, W0
	MOV	W0, W10
	CALL	_UART1_Write_Text
;MyProject.mbas,101 :: 		Delay_ms(50)
	MOV	#4, W8
	MOV	#11725, W7
L__main20:
	DEC	W7
	BRA NZ	L__main20
	DEC	W8
	BRA NZ	L__main20
	NOP
;MyProject.mbas,102 :: 		ADCresult = ADC1_Get_Sample(9)   ' Acquire ADC sample
	MOV	#9, W10
	CALL	_ADC1_Get_Sample
	MOV	W0, _ADCresult
;MyProject.mbas,103 :: 		WordToStr(ADCresult, txt)        ' convert its value to string
	MOV	#lo_addr(_txt), W11
	MOV	W0, W10
	CALL	_WordToStr
;MyProject.mbas,104 :: 		UART1_Write_Text(" ADC 9 = ")
	ADD	W14, #0, W1
	MOV.B	#32, W0
	MOV.B	W0, [W1++]
	MOV.B	#65, W0
	MOV.B	W0, [W1++]
	MOV.B	#68, W0
	MOV.B	W0, [W1++]
	MOV.B	#67, W0
	MOV.B	W0, [W1++]
	MOV.B	#32, W0
	MOV.B	W0, [W1++]
	MOV.B	#57, W0
	MOV.B	W0, [W1++]
	MOV.B	#32, W0
	MOV.B	W0, [W1++]
	MOV.B	#61, W0
	MOV.B	W0, [W1++]
	MOV.B	#32, W0
	MOV.B	W0, [W1++]
	CLR	W0
	MOV.B	W0, [W1++]
	ADD	W14, #0, W0
	MOV	W0, W10
	CALL	_UART1_Write_Text
;MyProject.mbas,105 :: 		UART1_Write_Text(txt)            ' and send it to UART terminal
	MOV	#lo_addr(_txt), W10
	CALL	_UART1_Write_Text
;MyProject.mbas,106 :: 		UART1_Write_Text("   ")
	ADD	W14, #0, W1
	MOV.B	#32, W0
	MOV.B	W0, [W1++]
	MOV.B	#32, W0
	MOV.B	W0, [W1++]
	MOV.B	#32, W0
	MOV.B	W0, [W1++]
	CLR	W0
	MOV.B	W0, [W1++]
	ADD	W14, #0, W0
	MOV	W0, W10
	CALL	_UART1_Write_Text
;MyProject.mbas,107 :: 		Delay_ms(50)
	MOV	#4, W8
	MOV	#11725, W7
L__main22:
	DEC	W7
	BRA NZ	L__main22
	DEC	W8
	BRA NZ	L__main22
	NOP
;MyProject.mbas,108 :: 		ADCresult = ADC1_Get_Sample(10)   ' Acquire ADC sample
	MOV	#10, W10
	CALL	_ADC1_Get_Sample
	MOV	W0, _ADCresult
;MyProject.mbas,109 :: 		WordToStr(ADCresult, txt)        ' convert its value to string
	MOV	#lo_addr(_txt), W11
	MOV	W0, W10
	CALL	_WordToStr
;MyProject.mbas,110 :: 		UART1_Write_Text(" ADC 10 = ")
	ADD	W14, #0, W1
	MOV.B	#32, W0
	MOV.B	W0, [W1++]
	MOV.B	#65, W0
	MOV.B	W0, [W1++]
	MOV.B	#68, W0
	MOV.B	W0, [W1++]
	MOV.B	#67, W0
	MOV.B	W0, [W1++]
	MOV.B	#32, W0
	MOV.B	W0, [W1++]
	MOV.B	#49, W0
	MOV.B	W0, [W1++]
	MOV.B	#48, W0
	MOV.B	W0, [W1++]
	MOV.B	#32, W0
	MOV.B	W0, [W1++]
	MOV.B	#61, W0
	MOV.B	W0, [W1++]
	MOV.B	#32, W0
	MOV.B	W0, [W1++]
	CLR	W0
	MOV.B	W0, [W1++]
	ADD	W14, #0, W0
	MOV	W0, W10
	CALL	_UART1_Write_Text
;MyProject.mbas,111 :: 		UART1_Write_Text(txt)            ' and send it to UART terminal
	MOV	#lo_addr(_txt), W10
	CALL	_UART1_Write_Text
;MyProject.mbas,112 :: 		UART1_Write_Text("   ")
	ADD	W14, #0, W1
	MOV.B	#32, W0
	MOV.B	W0, [W1++]
	MOV.B	#32, W0
	MOV.B	W0, [W1++]
	MOV.B	#32, W0
	MOV.B	W0, [W1++]
	CLR	W0
	MOV.B	W0, [W1++]
	ADD	W14, #0, W0
	MOV	W0, W10
	CALL	_UART1_Write_Text
;MyProject.mbas,113 :: 		Delay_ms(50)
	MOV	#4, W8
	MOV	#11725, W7
L__main24:
	DEC	W7
	BRA NZ	L__main24
	DEC	W8
	BRA NZ	L__main24
	NOP
;MyProject.mbas,114 :: 		ADCresult = ADC1_Get_Sample(11)   ' Acquire ADC sample
	MOV	#11, W10
	CALL	_ADC1_Get_Sample
	MOV	W0, _ADCresult
;MyProject.mbas,115 :: 		WordToStr(ADCresult, txt)        ' convert its value to string
	MOV	#lo_addr(_txt), W11
	MOV	W0, W10
	CALL	_WordToStr
;MyProject.mbas,116 :: 		UART1_Write_Text(" ADC 11 = ")
	ADD	W14, #0, W1
	MOV.B	#32, W0
	MOV.B	W0, [W1++]
	MOV.B	#65, W0
	MOV.B	W0, [W1++]
	MOV.B	#68, W0
	MOV.B	W0, [W1++]
	MOV.B	#67, W0
	MOV.B	W0, [W1++]
	MOV.B	#32, W0
	MOV.B	W0, [W1++]
	MOV.B	#49, W0
	MOV.B	W0, [W1++]
	MOV.B	#49, W0
	MOV.B	W0, [W1++]
	MOV.B	#32, W0
	MOV.B	W0, [W1++]
	MOV.B	#61, W0
	MOV.B	W0, [W1++]
	MOV.B	#32, W0
	MOV.B	W0, [W1++]
	CLR	W0
	MOV.B	W0, [W1++]
	ADD	W14, #0, W0
	MOV	W0, W10
	CALL	_UART1_Write_Text
;MyProject.mbas,117 :: 		UART1_Write_Text(txt)            ' and send it to UART terminal
	MOV	#lo_addr(_txt), W10
	CALL	_UART1_Write_Text
;MyProject.mbas,118 :: 		UART1_Write_Text("   ")
	ADD	W14, #0, W1
	MOV.B	#32, W0
	MOV.B	W0, [W1++]
	MOV.B	#32, W0
	MOV.B	W0, [W1++]
	MOV.B	#32, W0
	MOV.B	W0, [W1++]
	CLR	W0
	MOV.B	W0, [W1++]
	ADD	W14, #0, W0
	MOV	W0, W10
	CALL	_UART1_Write_Text
;MyProject.mbas,119 :: 		Delay_ms(50)
	MOV	#4, W8
	MOV	#11725, W7
L__main26:
	DEC	W7
	BRA NZ	L__main26
	DEC	W8
	BRA NZ	L__main26
	NOP
;MyProject.mbas,120 :: 		ADCresult = ADC1_Get_Sample(12)   ' Acquire ADC sample
	MOV	#12, W10
	CALL	_ADC1_Get_Sample
	MOV	W0, _ADCresult
;MyProject.mbas,121 :: 		WordToStr(ADCresult, txt)        ' convert its value to string
	MOV	#lo_addr(_txt), W11
	MOV	W0, W10
	CALL	_WordToStr
;MyProject.mbas,122 :: 		UART1_Write_Text(" ADC 12 = ")
	ADD	W14, #0, W1
	MOV.B	#32, W0
	MOV.B	W0, [W1++]
	MOV.B	#65, W0
	MOV.B	W0, [W1++]
	MOV.B	#68, W0
	MOV.B	W0, [W1++]
	MOV.B	#67, W0
	MOV.B	W0, [W1++]
	MOV.B	#32, W0
	MOV.B	W0, [W1++]
	MOV.B	#49, W0
	MOV.B	W0, [W1++]
	MOV.B	#50, W0
	MOV.B	W0, [W1++]
	MOV.B	#32, W0
	MOV.B	W0, [W1++]
	MOV.B	#61, W0
	MOV.B	W0, [W1++]
	MOV.B	#32, W0
	MOV.B	W0, [W1++]
	CLR	W0
	MOV.B	W0, [W1++]
	ADD	W14, #0, W0
	MOV	W0, W10
	CALL	_UART1_Write_Text
;MyProject.mbas,123 :: 		UART1_Write_Text(txt)            ' and send it to UART terminal
	MOV	#lo_addr(_txt), W10
	CALL	_UART1_Write_Text
;MyProject.mbas,124 :: 		UART1_Write_Text("   ")
	ADD	W14, #0, W1
	MOV.B	#32, W0
	MOV.B	W0, [W1++]
	MOV.B	#32, W0
	MOV.B	W0, [W1++]
	MOV.B	#32, W0
	MOV.B	W0, [W1++]
	CLR	W0
	MOV.B	W0, [W1++]
	ADD	W14, #0, W0
	MOV	W0, W10
	CALL	_UART1_Write_Text
;MyProject.mbas,125 :: 		Delay_ms(50)
	MOV	#4, W8
	MOV	#11725, W7
L__main28:
	DEC	W7
	BRA NZ	L__main28
	DEC	W8
	BRA NZ	L__main28
	NOP
;MyProject.mbas,126 :: 		wend
	GOTO	L__main2
L_end_main:
	POP	W12
	POP	W11
	POP	W10
	ULNK
L__main_end_loop:
	BRA	L__main_end_loop
; end of _main
