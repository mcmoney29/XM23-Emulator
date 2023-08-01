; Test #00
; -> Branch with link #50
; ECED 3403
; 21 June 2023
;
num	equ 	#11		; def 'num' to #11
;
	org	#2000		; Set the assembler origin to #2000
;
subr
;
	swpb	R0		; Swap register 0
	mov	R5,R7		; Return to address in LR
;
	org	#1000		; Sets the assembler origin to #1000
;
Start
; 
; Load num into R0
;
	movl	num,R0		; R0.LSByte <- #11
;
	bl	subr		; Branch to subr
;
	swpb 	R0		; Swap register 0 again
;
	end	Start		; End of program, set PC to address of Start
