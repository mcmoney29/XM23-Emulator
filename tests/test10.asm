; Test #10
; -> 
; ECED 3403
; 1 August 2023
;
	org	#1000
;
Start
;
	setcc	z
	cex	eq,$1,$1
	add 	$1,R1
	add 	$1,R0
;
	setcc	z
	clrcc	z
	cex	eq,$1,$1
	add 	$1,R1
	add 	$1,R0		
;
	setcc	c
	cex	cs,$1,$1
	add 	$1,R1
	add 	$1,R0
;
	setcc	c
	clrcc	c
	cex	cs,$1,$1
	add 	$1,R1
	add 	$1,R0
;
	setcc	n
	cex	mi,$1,$1
	add 	$1,R1
	add 	$1,R0
;
	setcc	n
	clrcc	n
	cex	mi,$1,$1
	add 	$1,R1
	add 	$1,R0
;
	setcc	v
	cex	vs,$1,$1
	add 	$1,R1
	add 	$1,R0
;
	setcc	v
	clrcc	v
	cex	vs,$1,$1
	add 	$1,R1
	add 	$1,R0
;
	end	Start
