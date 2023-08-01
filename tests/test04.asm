; Test #04
; -> Bit-Wise Operations
; ECED 3403
; 2 July 2023
;
num1	equ 	#CCCC		; def 'num1' to #CCCC
num2	equ 	#3333		; def 'num1' to #3333
base	equ	#FFF0		; def 'base' to #FFF0 (high bit should be ignored by movlz)
;
	org	#1000		; Sets the assembler origin to #1000
Start				
	movl	num1,R0		; R0.low <- #33
	movh	num1,R0		; R0.high <- #33
	movl	num2,R1		; R1.low <- #CC
	movh	num2,R1		; R1.high <- #CC
	movlz	base,R2		; R2 = #00F0
	st	R2,R0		; mem[#00F0] = #CCCC
;
	xor	R1,R0		; XOR
	ld	R2,R0		; reset R0 to #CCCC
;
	and 	R1,R0		; AND
	ld	R2,R0		; reset R0 to #CCCC
;
	or	R1,R0;		; OR
	ld	R2,R0;		; reset R0 to #CCCC
;
	bit	R1,R0;		; BIT
	ld	R2,R0;		; reset R0 to #CCCC
;
	bic	R1,R0;		; BIC
	ld	R2,R0;		; reset R0 to #CCCC
;
	bis	R1,R0;		; BIS
	ld 	R2,R0;		; reset R0 to #CCCC
;
	swap	R1,R0;		; swap R0 and R1
	swap 	R0,R1;		; swap back
;
	sra	R0;		; Shift Right
	ld	R2,R0;		; reset R0 to #CCCC
;
	add 	R1,R0;		; R0 = R0 + R1 (=#FFFF)
	add	R1,R0;		; add again to set carry bit
	rrc	R1;		; Rotate right through carry
;
	end	Start		; End of program, set PC to address of Start
