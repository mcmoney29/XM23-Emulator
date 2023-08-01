; Test #09
; -> Optimized Program for Direct Mapping
; ECED 3403
; 31 June 2023
;
	org	#0F0A
;
Recall	
;
	add	$1,R0
	cmp	$4,R0
	beq	Finish
	bra	Back
;	
	org	#1000		
;
Start				
	add	R1,R2	
;
Back
;	
	sub	R3,R4		
	xor	R2,R3
	swpb	R4
	bra	Recall	
Finish
;
	end	Start
