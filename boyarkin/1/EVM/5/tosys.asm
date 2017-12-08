	; tosys.asm
	; Алгоритм перевода в произвольную СС	
	
tosyshdr:
	; Инициализация DPTR, CUR
	; Остальные значения уже известны
	mov current, #0h
	mov dptr, #str2 + 12
	lcall clrmem
	mov dptr, #str2 + 12

start:
	mov a, value
	clr c
	subb a, def
	jnc incnum
	lcall toascii
	movx @dptr, a
	mov a, def
	clr c
	subb a, #1h
	jz exit
	mov current, #0h
	inc dptr
	mov a, def
	mov b, tosys
	div ab
	mov def, a
	jmp start
incnum:	
	mov value, a
	inc current
	jmp start
exit:
	ret

; Вывод цифры на экран 

toascii:	
	mov a, current
	clr c
	subb a, #Ah
	mov a, current
	jc decnum
	add a, #7h
decnum:
	add a, #30h
	ret		

; Очищение индикации

clrmem:
	mov r0, #8h
clrloop:
	dec r0
	mov a, #20h
	movx @dptr, a
	inc dptr
	mov a, r0
	cjne a, #0h, clrloop
	ret