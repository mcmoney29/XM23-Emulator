; Test #11
; -> 
; ECED 3403
; 1 August 2023
;
	org	#1000
;
Start
;
	setcc	z
	cex	ne,$1,$1
	add 	$1,R1
	add 	$1,R0
;
	setcc	z
	clrcc	z
	cex	ne,$1,$1
	add 	$1,R1
	add 	$1,R0		
;
	setcc	c
	cex	cc,$1,$1
	add 	$1,R1
	add 	$1,R0
;
	setcc	c
	clrcc	c
	cex	cc,$1,$1
	add 	$1,R1
	add 	$1,R0
;
	setcc	n
	cex	pl,$1,$1
	add 	$1,R1
	add 	$1,R0
;
	setcc	n
	clrcc	n
	cex	pl,$1,$1
	add 	$1,R1
	add 	$1,R0
;
	setcc	v
	cex	vc,$1,$1
	add 	$1,R1
	add 	$1,R0
;
	setcc	v
	clrcc	v
	cex	vc,$1,$1
	add 	$1,R1
	add 	$1,R0
;
	end	Start
