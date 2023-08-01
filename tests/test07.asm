; Test #07
; -> Cache Test
; ECED 3403
; 30 July 2023
;
loc	equ	#2010
;
	org	#1000		; Sets the assembler origin to #1000
Start				
;
	movh	loc,R0		; R0 <- #2000
	movl	loc,R2		; R2 <- #0010
;
Loop
;
	add	$1,R1		; Increament R1
	st	R1,R0		; Store value of R1 at mem[2000]
	cmp	R1,R2		; Compares R0 and R1
	bne 	Loop		; Loops if R1 != #10
;
	end 	Start
	
