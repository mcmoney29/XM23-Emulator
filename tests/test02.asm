; Test #02
; -> Conditional Branches (2)
; ECED 3403
; 30 June 2023
;
num1	equ 	#EF00		; def 'num1' to #FF17
num2	equ 	#1000		; def 'num2' to #1000
num3	equ	#0200		; def 'num3' to #0200
;
	org 	#1100		; Sets the assembler orgin to #1100
Less	
	movh	num1,R0		; R0.high <- #EF
	movh	num3,R1		; R1.high <- #02
	add	R1,R0		; R0 <- R0 + R1
	blt	Finish
;
Negative
	sub	R1,R0
	bge	Negative
	bn	Less
;
	org	#1000		; Sets the assembler origin to #1000
Start				
	movls	num1,R0		; R0.high <- #FF
	movh	num2,R1		; R1.high <- #10
Carry
	add	R1,R0		; Add R1 into R0
	bc	Carry		; Branch to true if equal
	bnc	Negative		; Branch to false if false
;
Finish
	end	Start		; End of program, set PC to address of Start
