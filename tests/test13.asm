; Test #13
; -> 
; ECED 3403
; 1 August 2023
;
	org	#1000
;
Start
;
	clrcc	c
	setcc	z
	cex	hi,$1,$1
	add 	$1,R1
	add 	$1,R0
;
	clrcc	z
	setcc	c
	cex	ls,$1,$1
	add 	$1,R1
	add 	$1,R0			
;
	clrcc	n
	setcc	v
	cex	ge,$1,$1
	add 	$1,R1
	add 	$1,R0
;
	setcc	n
	setcc	v
	cex	lt,$1,$1
	add 	$1,R1
	add 	$1,R0
;
	clrcc	nv
	setcc	z
	cex	gt,$1,$1
	add 	$1,R1
	add 	$1,R0
;
	clrcc	zvn
	cex	le,$1,$1
	add 	$1,R1
	add 	$1,R0
;
	end	Start
