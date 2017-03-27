
_config_timer:

;SenoDSPIC.c,47 :: 		void config_timer(){
;SenoDSPIC.c,48 :: 		T1CON	 = 0x8000;
	MOV	#32768, W0
	MOV	WREG, T1CON
;SenoDSPIC.c,49 :: 		T1IE_bit	 = 1;
	BSET	T1IE_bit, BitPos(T1IE_bit+0)
;SenoDSPIC.c,50 :: 		T1IF_bit	 = 0;
	BCLR	T1IF_bit, BitPos(T1IF_bit+0)
;SenoDSPIC.c,51 :: 		IPC0		 = IPC0 | 0x1000;
	MOV	#4096, W1
	MOV	#lo_addr(IPC0), W0
	IOR	W1, [W0], [W0]
;SenoDSPIC.c,52 :: 		PR1		 = 400;
	MOV	#400, W0
	MOV	WREG, PR1
;SenoDSPIC.c,53 :: 		}
L_end_config_timer:
	RETURN
; end of _config_timer

_Timer1Interrupt:
	PUSH	52
	PUSH	RCOUNT
	PUSH	W0
	MOV	#2, W0
	REPEAT	#12
	PUSH	[W0++]

;SenoDSPIC.c,55 :: 		void Timer1Interrupt() iv IVT_ADDR_T1INTERRUPT{
;SenoDSPIC.c,56 :: 		T1IF_bit	 = 0;
	BCLR	T1IF_bit, BitPos(T1IF_bit+0)
;SenoDSPIC.c,58 :: 		bandera = 1;
	MOV	#lo_addr(_bandera), W1
	MOV.B	#1, W0
	MOV.B	W0, [W1]
;SenoDSPIC.c,59 :: 		}
L_end_Timer1Interrupt:
	MOV	#26, W0
	REPEAT	#12
	POP	[W0--]
	POP	W0
	POP	RCOUNT
	POP	52
	RETFIE
; end of _Timer1Interrupt

_main:
	MOV	#2048, W15
	MOV	#6142, W0
	MOV	WREG, 32
	MOV	#1, W0
	MOV	WREG, 52
	MOV	#4, W0
	IOR	68

;SenoDSPIC.c,68 :: 		void main() {
;SenoDSPIC.c,73 :: 		TRISA.F0 = 1;     //pin RA0 como entrada
	PUSH	W10
	PUSH	W11
	PUSH	W12
	PUSH	W13
	BSET	TRISA, #0
;SenoDSPIC.c,74 :: 		TRISB.F15 = 1;
	BSET	TRISB, #15
;SenoDSPIC.c,76 :: 		TRISB.F14 = 0;   //RB14 como salida
	BCLR	TRISB, #14
;SenoDSPIC.c,80 :: 		RPOR5 = 0x0708;
	MOV	#1800, W0
	MOV	WREG, RPOR5
;SenoDSPIC.c,82 :: 		loadPinDir = 0;                           // LOAD pin
	BCLR	TRISB13_bit, BitPos(TRISB13_bit+0)
;SenoDSPIC.c,83 :: 		csPinDir   = 0;                           // CS pin
	BCLR	TRISB12_bit, BitPos(TRISB12_bit+0)
;SenoDSPIC.c,84 :: 		csPin    = 1;                             // Set CS to inactive
	BSET	LATB12_bit, BitPos(LATB12_bit+0)
;SenoDSPIC.c,85 :: 		loadPin  = 1;                             // Set LOAD to inactive
	BSET	LATB13_bit, BitPos(LATB13_bit+0)
;SenoDSPIC.c,88 :: 		SPI1_Init_Advanced(_SPI_MASTER, _SPI_16_BIT,_SPI_PRESCALE_SEC_1, _SPI_PRESCALE_PRI_1,
	MOV	#3, W13
	MOV	#28, W12
	MOV	#1024, W11
	MOV	#32, W10
;SenoDSPIC.c,90 :: 		_SPI_ACTIVE_2_IDLE);
	CLR	W0
	PUSH	W0
;SenoDSPIC.c,89 :: 		_SPI_SS_DISABLE, _SPI_DATA_SAMPLE_MIDDLE, _SPI_CLK_IDLE_HIGH,
	MOV	#64, W0
	PUSH	W0
	CLR	W0
	PUSH	W0
	CLR	W0
	PUSH	W0
;SenoDSPIC.c,90 :: 		_SPI_ACTIVE_2_IDLE);
	CALL	_SPI1_Init_Advanced
	SUB	#8, W15
;SenoDSPIC.c,92 :: 		config_timer();
	CALL	_config_timer
;SenoDSPIC.c,94 :: 		TRISB.F15 = 0;
	BCLR	TRISB, #15
;SenoDSPIC.c,95 :: 		LATB.F15 = 0;
	BCLR	LATB, #15
;SenoDSPIC.c,96 :: 		LATB.F14 =1;
	BSET	LATB, #14
;SenoDSPIC.c,99 :: 		frecuenciadigital=frecuencia/frecuenciamuestreo;
	MOV	_frecuenciamuestreo, W2
	MOV	_frecuenciamuestreo+2, W3
	MOV	_frecuencia, W0
	MOV	_frecuencia+2, W1
	CALL	__Div_FP
	MOV	W0, _frecuenciadigital
	MOV	W1, _frecuenciadigital+2
;SenoDSPIC.c,102 :: 		for(;;){
L_main0:
;SenoDSPIC.c,103 :: 		if( bandera == 1){
	MOV	#lo_addr(_bandera), W0
	MOV.B	[W0], W0
	CP.B	W0, #1
	BRA Z	L__main10
	GOTO	L_main3
L__main10:
;SenoDSPIC.c,105 :: 		bandera = 0;
	MOV	#lo_addr(_bandera), W1
	CLR	W0
	MOV.B	W0, [W1]
;SenoDSPIC.c,107 :: 		y=floor((1*sin(2*3.1416*muestra*frecuenciadigital))*2048 + 2048);  // calculo seno digital de 12 bits  // calculo sino digital de 12 bits;  // calculo sino digital de 12 bits
	MOV	_muestra, W0
	ASR	W0, #15, W1
	SETM	W2
	CALL	__Long2Float
	MOV	#4089, W2
	MOV	#16585, W3
	CALL	__Mul_FP
	MOV	_frecuenciadigital, W2
	MOV	_frecuenciadigital+2, W3
	CALL	__Mul_FP
	MOV.D	W0, W10
	CALL	_sin
	MOV	#0, W2
	MOV	#17664, W3
	CALL	__Mul_FP
	MOV	#0, W2
	MOV	#17664, W3
	CALL	__AddSub_FP
	MOV.D	W0, W10
	CALL	_floor
	CALL	__Float2Longint
	MOV	W0, _y
;SenoDSPIC.c,109 :: 		muestra++;
	MOV	#1, W1
	MOV	#lo_addr(_muestra), W0
	ADD	W1, [W0], [W0]
;SenoDSPIC.c,111 :: 		if( indice == 69)    {
	MOV	#lo_addr(_indice), W0
	MOV.B	[W0], W1
	MOV.B	#69, W0
	CP.B	W1, W0
	BRA Z	L__main11
	GOTO	L_main4
L__main11:
;SenoDSPIC.c,113 :: 		indice = 0;
	MOV	#lo_addr(_indice), W1
	CLR	W0
	MOV.B	W0, [W1]
;SenoDSPIC.c,114 :: 		}
L_main4:
;SenoDSPIC.c,119 :: 		csPin = 0;                                // CS enable for DAC
	BCLR	LATB12_bit, BitPos(LATB12_bit+0)
;SenoDSPIC.c,120 :: 		SPI1_Write(0x3000 | y);
	MOV	#12288, W1
	MOV	#lo_addr(_y), W0
	IOR	W1, [W0], W0
	MOV	W0, W10
	CALL	_SPI1_Write
;SenoDSPIC.c,121 :: 		loadPin  = 0;                             // Load data in DAC
	BCLR	LATB13_bit, BitPos(LATB13_bit+0)
;SenoDSPIC.c,122 :: 		delay_us(2);
	MOV	#3, W7
L_main5:
	DEC	W7
	BRA NZ	L_main5
	NOP
;SenoDSPIC.c,123 :: 		loadPin  = 1;
	BSET	LATB13_bit, BitPos(LATB13_bit+0)
;SenoDSPIC.c,124 :: 		csPin    = 1;                             // CS disable for DAC
	BSET	LATB12_bit, BitPos(LATB12_bit+0)
;SenoDSPIC.c,127 :: 		}
L_main3:
;SenoDSPIC.c,129 :: 		}
	GOTO	L_main0
;SenoDSPIC.c,130 :: 		}
L_end_main:
	POP	W13
	POP	W12
	POP	W11
	POP	W10
L__main_end_loop:
	BRA	L__main_end_loop
; end of _main
