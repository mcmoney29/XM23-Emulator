; Test #05
; -> DADD, CMP, SXT
; ECED 3403
; 2 July 2023

num1	equ 	#2498		; def 'num1' to #2498
num2	equ 	#0905		; def 'num2' to #0905
;
	org	#1000		; Sets the assembler origin to #1000
Start				
	movl	num1,R0		; R0.low <- #98
	movh	num1,R0		; R0.high <- #24
	movl	num2,R1		; R1.low <- #05
	movh	num2,R1		; R1.high <- #09
;
	dadd	R1,R0		; R0 = $2498 + $0905 = $3403
	sub.b	R1,R0		; R0 = #24(03 - 05 = FE)
	sxt	R0		; R0 = #FFFE
	comp 	R0		; R0 = ~R0 = #0001
	end	Start		; End of program, set PC to address of Start
