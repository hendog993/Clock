
	; Microchip MPLAB XC8 C Compiler V2.36
	; Copyright (C) 2022 Microchip Technology Inc.

	; Auto-generated runtime startup code for final link stage.

	;
	; Compiler options:
	;
	; -q --opt=none --chip=16f18326 \
	; -Mdist/default/production/MaverickClock.X.production.map \
	; -DXPRJ_default=default -L--defsym=__MPLAB_BUILD=1 \
	; --dfp=C:/Users/henry/.mchp_packs/Microchip/PIC16F1xxxx_DFP/1.6.143/xc8 \
	; --double=32 --float=32 --opt=+1 --opt=+asmfile --addrqual=ignore -P \
	; --warn=-2 --asmlist --summary=-psect,-class,+mem,-hex,-file \
	; --output=+inhx32 --runtime=+clear --runtime=+init --runtime=+keep \
	; --runtime=-osccal --runtime=+resetbits --runtime=+resetbits \
	; --runtime=-download --runtime=-stackcall --runtime=+config --std=c99 \
	; --output=+elf:multilocs --stack=compiled:auto:auto --summary=+xml \
	; --summarydir=dist/default/production/memoryfile.xml \
	; -oMaverickClock.X.production.elf --objdir=dist/default/production \
	; --outdir=dist/default/production \
	; build/default/production/mcc_generated_files/pin_manager.p1 \
	; build/default/production/mcc_generated_files/device_config.p1 \
	; build/default/production/mcc_generated_files/tmr1.p1 \
	; build/default/production/mcc_generated_files/mcc.p1 \
	; build/default/production/mcc_generated_files/interrupt_manager.p1 \
	; build/default/production/mcc_generated_files/tmr3.p1 \
	; build/default/production/mcc_generated_files/tmr5.p1 \
	; build/default/production/mcc_generated_files/memory.p1 \
	; build/default/production/patternMode.p1 \
	; build/default/production/changeColorMode.p1 \
	; build/default/production/timeCalculation.p1 \
	; build/default/production/main.p1 build/default/production/ws2812b.p1 \
	; build/default/production/rotaryEncoder.p1 \
	; build/default/production/app.p1 build/default/production/clockLEDs.p1 \
	; build/default/production/CRC16bit.p1 \
	; build/default/production/nvmMirror.p1 --callgraph=none \
	; --errformat=%f:%l:%c: error: (%n) %s \
	; --warnformat=%f:%l:%c: warning: (%n) %s \
	; --msgformat=%f:%l:%c: advisory: (%n) %s
	;


	processor	16F18326

	global	_main,start,reset_vec
	fnroot	_main
	psect	code,class=CODE,delta=2
	psect	powerup,class=CODE,delta=2
	psect	reset_vec,class=CODE,delta=2
	psect	maintext,class=CODE,delta=2
	C	set	0
	Z	set	2
	PCL	set	2
	INDF	set	0

	STATUS	equ	3
	BSR	equ	8
	PCLATH	equ	0Ah
	psect	eeprom_data,class=EEDATA,delta=2,space=3,noexec
	psect	strings,class=CODE,delta=2,reloc=256
	psect	intentry,class=CODE,delta=2
	psect	functab,class=ENTRY,delta=2
	global	intlevel0,intlevel1,intlevel2, intlevel3, intlevel4, intlevel5
intlevel0:
intlevel1:
intlevel2:
intlevel3:
intlevel4:
intlevel5:
	psect	init,class=CODE,delta=2
	psect	cinit,class=CODE,delta=2
	psect	text,class=CODE,delta=2
	psect	end_init,class=CODE,delta=2
	psect	clrtext,class=CODE,delta=2
	INDF0	set	0
	INDF1	set	1
	PCL	set	2
	STATUS	set	3
	FSR0L	set	4
	FSR0H	set	5
	FSR1L	set	6
	FSR1H	set	7
	BSR	set	8
	WREG	set	9
	PCLATH	set	10
	INTCON	set	11

	psect	reset_vec
reset_vec:
	; No powerup routine
	global start

; jump to start
	ljmp	start


	psect	init
start:

	; Save the TO, PD bit states before they are cleared by clrwdt instruction
	global	___resetbits
	global	___powerdown
	global	___timeout
	___resetbits	equ	32
	___powerdown	equ	(___resetbits*8)+3
	___timeout	equ	(___resetbits*8)+4
	movlb	0		;select bank 0
	movf STATUS,w
	movwf ___resetbits

;Initialize the stack pointer (FSR1)
	fsr1l	equ	6
	fsr1h	equ	7
	global ___stacklo, ___stackhi
	___stacklo	equ	0
	___stackhi	equ	0


	psect	stack,class=STACK,space=2,noexec
	global ___sp,___int_sp
	___sp:
	___int_sp:

; No heap to be allocated
	psect	heap,class=HEAP,space=7,noexec
	global ___heap_lo
	___heap_lo	equ	0
	global ___heap_hi
	___heap_hi	equ	0

	psect	end_init
	global start_initialization
	ljmp start_initialization	;jump to C runtime clear & initialization

psect common,class=COMMON,space=1
psect bank0,class=BANK0,space=1
psect bank1,class=BANK1,space=1
psect bank2,class=BANK2,space=1
psect bank3,class=BANK3,space=1
psect bank4,class=BANK4,space=1
psect bank5,class=BANK5,space=1
psect bank6,class=BANK6,space=1
psect bank7,class=BANK7,space=1
psect bank8,class=BANK8,space=1
psect bank9,class=BANK9,space=1
psect bank10,class=BANK10,space=1
psect bank11,class=BANK11,space=1
psect bank12,class=BANK12,space=1
psect bank13,class=BANK13,space=1
psect bank14,class=BANK14,space=1
psect bank15,class=BANK15,space=1
psect bank16,class=BANK16,space=1
psect bank17,class=BANK17,space=1
psect bank18,class=BANK18,space=1
psect bank19,class=BANK19,space=1
psect bank20,class=BANK20,space=1
psect bank21,class=BANK21,space=1
psect bank22,class=BANK22,space=1
psect bank23,class=BANK23,space=1
psect bank24,class=BANK24,space=1
psect bank25,class=BANK25,space=1
psect bank26,class=BANK26,space=1
psect bank27,class=BANK27,space=1
psect bank28,class=BANK28,space=1
psect bank29,class=BANK29,space=1
psect bank30,class=BANK30,space=1
psect bank31,class=BANK31,space=1
psect bigram,class=BIGRAM,space=1
psect ram,class=RAM,space=1
psect abs1,class=ABS1,space=1
psect sfr0,class=SFR0,space=1
psect sfr1,class=SFR1,space=1
psect sfr2,class=SFR2,space=1
psect sfr3,class=SFR3,space=1
psect sfr4,class=SFR4,space=1
psect sfr5,class=SFR5,space=1
psect sfr6,class=SFR6,space=1
psect sfr7,class=SFR7,space=1
psect sfr8,class=SFR8,space=1
psect sfr9,class=SFR9,space=1
psect sfr10,class=SFR10,space=1
psect sfr11,class=SFR11,space=1
psect sfr12,class=SFR12,space=1
psect sfr13,class=SFR13,space=1
psect sfr14,class=SFR14,space=1
psect sfr15,class=SFR15,space=1
psect sfr16,class=SFR16,space=1
psect sfr17,class=SFR17,space=1
psect sfr18,class=SFR18,space=1
psect sfr19,class=SFR19,space=1
psect sfr20,class=SFR20,space=1
psect sfr21,class=SFR21,space=1
psect sfr22,class=SFR22,space=1
psect sfr23,class=SFR23,space=1
psect sfr24,class=SFR24,space=1
psect sfr25,class=SFR25,space=1
psect sfr26,class=SFR26,space=1
psect sfr27,class=SFR27,space=1
psect sfr28,class=SFR28,space=1
psect sfr29,class=SFR29,space=1
psect sfr30,class=SFR30,space=1
psect sfr31,class=SFR31,space=1


	end	start
