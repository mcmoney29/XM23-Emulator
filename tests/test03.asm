; Test #05
; -> Add bytes with carry
; ECED 3403
; 24 June 2023
;
num1	equ 	#4280		; def 'num1' to #4280
num2	equ 	#17A0		; def 'num2' to #17A0
;
	org	#1000		; Sets the assembler origin to #1000
Start
	movl	num1,R0		; Set R0 = #4280
	movh	num1,R0		; 
	movl	num2,R1		; Set R1 = #17A0
	movh	num2,R1		;
Loop
	add.b	R1,R0		; Add #A0 to #80 (= #120)
				; R0.low <= #20
				; PSW.c is set
	swpb	R0		; Swap bytes in R0
	swpb	R1		; Swap bytes in R1
	addc.b	R1,R0		; Add bytes with carry
	swpb	R0		; Swap bytes in R0 back
	swpb	R1		; Swap bytes in R1 back
;
	end	Start		; End of program, set PC to address of Start
