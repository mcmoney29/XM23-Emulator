; Test #06
; -> Byte Instructions
; ECED 3403
; 2 July 2023

num1	equ 	#1721		; def 'num1' to #1721
num2	equ 	#6532		; def 'num2' to #6532
;
	org	#1000		; Sets the assembler origin to #1000
Start				
	movl	num1,R0		; R0.low <- #21
	movh	num1,R0		; R0.high <- #17
	movl	num2,R1		; R1.low <- #32
	movh	num2,R1		; R1.high <- #65
;
	dadd.b	R1,R0
	xor.b 	R1,R0
	and.b	R1,R0
	add.b	R1,R0
	or.b	R1,R0
	bit.b	R1,R0
	bic.b	R1,R0
	mov.b	R1,R0
	sra.b	R1
	rrc.b	R1
;
	end 	Start
	
