timerinit:	
	anl TMOD,#11110000b ; Инициализация таймера
	orl TMOD,#00000001b
	
	mov TH0, #FEh	; Инициализация счетчика 500 мкс
	mov TL0, #0Bh 

	setb ea		; разрешение прерываний
	setb et0	; разрешение внутр. прер. T/C0
	setb tr0
	ret

	
 timer_ir:	
	mov TH0, #FEh	; Инициализация счетчика 500 мкс
	mov TL0, #0Bh	
	inc timertics	; Инкрементируем счетчик прерываний
	reti
	
	org 800bh
	ljmp timer_ir

