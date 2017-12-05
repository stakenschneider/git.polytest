	org 8100h

p5:	equ	f8h
p4:	equ	e8h
number:	equ	40h	; хранит кол-во символов

	
start:	lcall init		; иниц. обмен с МК
	; инициализируем, НО НЕ ЗАПУСКАЕМ T/C0
	lcall timerInit
	lcall clear		; очистка индикации
	mov number, #0h

rec_str:	jnb ri, rec_str	; ждем передачу
	setb tr0  		; запуск таймер после 1ой передачи
	mov a, sbuf	; принятый байт
	clr ri		; сброс флага
	
	jmp handler

work:	lcall receive
	jmp handler
	sjmp work

 ; проверка, что символов <= 40
handler:	mov r7, a	; сохраняем КОД символа
	inc number	; кол-во принятых байт
	mov dptr, #str1	; запоминаем начало строки
	mov a, number	;
	cjne a, #41, handler2	; если !=41 => идем дальше
	jmp breakingBad	; если 41 => выход

 ; сдвиг dptr в нужное место
handler2:	dec a		; хранит НОМЕР символа
	jz handler3	; если 0 => нужная позиция в dptr
	inc dptr
	jmp handler2

 ; запись в нужное место в строке
handler3:	mov a, r7
	movx @dptr, a
	cjne   a,   #2Eh,   work	; проверка на точку

breakingBad: clr tr0	; остановка таймера
	lcall indic	; индикация

	jmp start
	ret

 ; подпрограмма для очистки индикации
clear:	mov dptr, #str1
	mov r7, #40
	mov a, #20h	; код пробела
cl_m1:	movx @dptr, a
	inc dptr
	dec r7
	cjne r7, #0, cl_m1
	ret
	
	

 ; ****** FUNCTIONS *****
 ; программа send_rec (обмен с контроллером)
init:	clr tr1		   ; запретили счет
	mov scon, #11010010b
	anl tmod, #0Fh
	orl tmod, #00100000b
	mov th1, #e6h	;  1200 бит/с
	anl D8h, #7Fh
	anl 87h, #7Fh
	setb tr1		; разрешение TC1
	ret


 ; прием последовательных данных из МК
receive:	jnb ri, receive
	mov a, sbuf
	clr ri
	ret
	
 	include ASMS\43501_3\bk\4\p43\indic.asm
	include ASMS\43501_3\bk\4\p43\timer.asm

	org FFD0h
str1:   	db  '                    '
str2:   	db  '                    '
	
 	


