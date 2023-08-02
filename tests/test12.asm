; Test #12
; -> 
; ECED 3403
; 1 August 2023
;
	org	#1000
;
Start
;
	setcc	c
	clrcc	z
	cex	hi,$1,$1
	add 	$1,R1
	add 	$1,R0
;
	setcc	z
	clrcc	c
	cex	ls,$1,$1
	add 	$1,R1
	add 	$1,R0			
;
	setcc	nv
	cex	ge,$1,$1
	add 	$1,R1
	add 	$1,R0
;
	setcc	n
	clrcc	v
	cex	lt,$1,$1
	add 	$1,R1
	add 	$1,R0
;
	setcc	nv
	clrcc	z
	cex	gt,$1,$1
	add 	$1,R1
	add 	$1,R0
;
	setcc	n,z
	clrcc	v
	cex	le,$1,$1
	add 	$1,R1
	add 	$1,R0
;
	end	Start
