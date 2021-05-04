	.arch msp430g2553
	.p2align 2 		; power of 2 alignment  
	.text

	.extern state		; state and freq variable are in statemachine.c
	.extern freq

	.text
	.global buzzer_advance

buzzer_advance:
	cmp #0, &state 		; lower freq if state = 0
	jz else			; jump if equal to zero

if:	add #225, &freq		; raise ffreq
	jmp exit
else:
	sub #450, &freq 	;lower

exit:	mov &freq, r12
	call #get_period

	call #buzzer_set_period
	pop r0			; return 
