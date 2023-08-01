; Test #01
; -> Conditional Branches
; ECED 3403
; 21 June 2023
;
num1	equ 	#1738		; def 'num1' to #1738
num2	equ 	#2023		; def 'num1' to #1738
;
	org	#1100		; Set the assembler origin to #1100
true				; true = address #2000
;
	movl	num2,R0		; Set R0 to #2023
	movh	num2,R0		;
	bra	Compare		; Return
;
;
false
	movl	num1,R0		; Set R0 to #1738
	movh	num1,R0		;
	bra	Compare		; Return
;
;
	org	#1000		; Sets the assembler origin to #1000
Start				
	movl	num1,R0		; R0.low <- #38
	movh	num1,R0		; R0.high <- #17
	mov	R0,R1		; Copy #1738 into R1
Compare
	cmp	R0,R1		; Compare R1 and R0
	beq	true		; Branch to true if equal
	bne	false		; Branch to false if false
;
	end	Start		; End of program, set PC to address of Start
