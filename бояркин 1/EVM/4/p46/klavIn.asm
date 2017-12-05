klavIn:

	clr TB8
 ; SLAVE не имеет права ничего нажимать
	mov a, status
	clr C
	subb a, #53h	; S-Slave
	jz s_end	; ј!=0 => следующа€ проверка




 ; ****** button15 #0fh=15 (установка режима "Master/Slave")
	mov a, button
	clr C
	subb a, #0Fh	
	jnz s_m1; ј!=0 => следующа€ проверка

	; если была нажата 15
	mov a, #4Dh	; #4Dh="M"(master)
	mov status, a	; status=Master
	mov dptr, #str1
	movx @dptr, a	; устанавливаем букву M
	jmp s_next	



 ; ќбычный режим - никак особых действий
s_m1:	mov a, status
	clr C
	subb a, #20h	;_-обычный режим
	jz s_next	; к стд. части




 ; ****** button14 #0Eh=14 (передача адреса)
	mov a, button
	clr C
	subb a, #0Eh	
	jnz s_m2	; ј!=0 => следующа€ проверка

	; если нажата 14
	setb TB8	; установили бит=>передаем адрес

	mov a, #31h	; #31h="1"
	mov dptr, #str2+6	; на место 1го ’
	movx @dptr, a	; помещаем "1" в str2

	mov a, #34h	; #34h="4"
	inc dptr
	movx @dptr, a	; помещаем "4" в str2
	jmp s_next


  ; ****** button13 #0Dh=13 (передача адреса)
s_m2:	mov a, button
	clr C
	subb a, #0Dh	
	jnz s_m3	; ј!=0 => следующа€ проверка

	; если нажата 13
	setb TB8	; установили бит=>передаем адрес
	mov a, #31h	; #31h="1"
	mov dptr, #str2+6	; на место 1го ’
	movx @dptr, a	; помещаем "1" в str2

	mov a, #33h	; #33h="3"
	inc dptr
	movx @dptr, a	; помещаем "3" в str2

	jmp s_next

  ; ****** button12 #0Ch=12 (завершение посылки)
s_m3:	mov a, button
	clr C
	subb a, #0Ch	
	jnz s_next	; ј!=0 => никаких действий,
			; просто передача

	; если нажата 12 (конец посылки)
	setb TB8	; вдруг адрес передан не был
	clr SM2		; сбросили биты
	mov status, #20h	; сбросили статус
	mov a, #78h	; #78h="x"
	mov dptr, #str1
	movx @dptr, a	; сброс режима

	mov dptr, #str2+6
	movx @dptr, a	; помещаем "x" в str2
	inc dptr
	movx @dptr, a	; помещаем "x" в str2
	jmp s_next

s_next:
 ; вывод номера нажатой кнопки на индикатор
	mov r0, #34h  ; address of inside memmory
	mov a, @r0  ; number of button 1-16
	mov b, #0Ah ; b = 10
	div ab  ; a/b  => a=0-1,  b=0-6
	mov dptr, #str1+17  ; address "N" in str1
	add a, #30h  ; translate to ASCII-code
	movx @dptr, a ; "N" <=> 0-1
	mov a, b  ; b=0-6
	add a, #30h  ; ASCII-code
	inc dptr  ; address "x" in str2
	movx @dptr, a ; "N" <=> 0-1
s_end:
	ret





klavIn2:
 ; MASTER ничего не принимает
	mov a, status
	clr C
	subb a, #4Dh	; M-Master
	jz r_end	; ј!=0 => следующа€ проверка

 ; определение посылки

 ; ****** button15 #0fh=15 (установка режима "Master/Slave")
	mov a, button2
	clr C
	subb a, #0Fh	
	jnz r_m1	; ј!=0 => следующа€ проверка

	; если была нажата 15
	setb SM2	; особый режим прослушивани€
	mov a, #53h	; #53h="S"(Slave)
	mov status, a	; status=Slave
	mov dptr, #str1
	movx @dptr, a	; устанавливаем букву S
	jmp r_next



 ; ќбычный режим - никак особых действий
r_m1:	mov a, status
	clr C
	subb a, #20h	;_-обычный режим
	jz r_next	; к стд. части


  ; ****** button13 #0Dh=13 (передача адреса)
	mov a, button2
	clr C
	subb a, #0Dh	
	jnz r_m2	; ј!=0 => следующа€ проверка

	; если нажата 14
	clr SM2		; ждем данные

	mov a, #31h	; #31h="1"
	mov dptr, #str2+6	; на место 1го ’
	movx @dptr, a	; помещаем "1" в str2

	mov a, #33h	; #34h="3"
	inc dptr
	movx @dptr, a	; помещаем "3" в str2
	jmp r_next


  ; ****** button14 #0Eh=14 (передача адреса)
r_m2:	mov a, button2
	clr C
	subb a, #0Dh	
	jz r_end	; если нажата 14


  ; ****** button12 #0Ch=12 (завершение посылки)
	mov a, button2
	clr C
	subb a, #0Ch	
	jnz r_next	; ј!=0 => никаких действий,
			; просто передача

	; если нажата 12 (конец посылки)
	clr SM2		; сбросили биты
	mov status, #20h	; сбросили статус

	mov a, #78h	; #78h="x"
	mov dptr, #str1
	movx @dptr, a	; сброс режима
	mov dptr, #str2+6
	movx @dptr, a	; помещаем "x" в str2
	inc dptr
	movx @dptr, a	; помещаем "x" в str2
	jmp r_next

r_next:
 ; вывод номера нажатой кнопки на индикатор
	mov r0, #35h  ; address of inside memmory
	mov a, @r0  ; number of button 1-16
	mov b, #0Ah ; b = 10
	div ab  ; a/b  => a=0-1,  b=0-6
	mov dptr, #str2+17  ; address "N" in str1
	add a, #30h  ; translate to ASCII-code
	movx @dptr, a ; "N" <=> 0-1
	mov a, b  ; b=0-6
	add a, #30h  ; ASCII-code
	inc dptr  ; address "x" in str2
	movx @dptr, a ; "N" <=> 0-1
r_end:
	ret