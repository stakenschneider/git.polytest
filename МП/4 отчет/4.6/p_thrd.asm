          org 8400h

p5:	equ	f8h
p4:	equ	e8h
intPt:	equ	41h
retPt:	equ	42h
status:	equ	44h
tmp:	equ	45h
button:	equ	34h
button2:	equ	35h
IP0:     	equ  	A9h
IP1:     	equ 	B9h

nowTask:	equ	20h

	mov nowTask,#0h
	mov button, #0h
	
	; ћ-мастер, S-слейв, _-простой режим
	mov status, #20h	; M S _
  	; приоритеты

	;mov A9h, #04h
	;mov B9h, #04h

	;инициализаци€ дескриптора d_tsk1 второй задачи
	mov dptr, #task1  ; загрузка адреса 2ой задачи
	mov r0,dpl       ; запоминание в r0 и r1 
	mov r1,dph       ; (мл. и ст. байты адреса)
	mov dptr, #tsk1_d ; загрузка адреса дескриптора второй задачи
	;формирование адреса €чейки дексриптора
	mov a,dpl
	add a,#8h
	mov dpl,a
	;загрузка адреса второй задачи в дескрпитор
	mov a,r0
	movx @dptr,a
	inc dptr
	mov a,r1
	movx @dptr,a

	;инициализаци€ дескриптора d_tsk2 второй задачи
	mov dptr, #task2  ; загрузка адреса 2ой задачи
	mov r0,dpl       ; запоминание в r0 и r1 
	mov r1,dph       ; (мл. и ст. байты адреса)
	mov dptr, #tsk2_d ; загрузка адреса дескриптора второй задачи
	;формирование адреса €чейки дексриптора
	mov a,dpl
	add a,#8h
	mov dpl,a
	;загрузка адреса второй задачи в дескрпитор
	mov a,r0
	movx @dptr,a
	inc dptr
	mov a,r1
	movx @dptr,a

	;инициализаци€ дескриптора d_tsk3 третьей задачи
	mov dptr, #task3  ; загрузка адреса 3ий задачи
	mov r0,dpl       ; запоминание в r0 и r1 
	mov r1,dph       ; (мл. и ст. байты адреса)
	mov dptr, #tsk3_d ; загрузка адреса дескриптора третьей задачи
	;формирование адреса €чейки дексриптора
	mov a,dpl
	add a,#8h
	mov dpl,a
	;загрузка адреса третьей задачи в дескрпитор
	mov a,r0
	movx @dptr,a
	inc dptr
	mov a,r1
	movx @dptr,a

	setb ea ; enable of ALL interruptions
	lcall TC0Init
	lcall initSendRec

	; определ€ет номер кнопки от 1 до 16
task1:	lcall memklav
	sjmp task1

	; индикаици€, 
task2:	lcall indic
	mov a, button	; если ничего не нажато
	jz task2		; ничего не делаем

	lcall klavIn		; выводим на свой индикатор
			; +обработчик
	 ; SLAVE ничего не отправл€ет
	mov a, status
	clr C
	subb a, #53h	; S-Slave
	jz task2

	; если не SLAVE отправл€ем
	mov a, button	
	lcall send		; отправл€ем код кнопки
	sjmp task2

task3:	lcall receive
	mov button2, a
	lcall klavIn2	
	sjmp task3



delay31:
	mov r4, #04h
d_m11: 	mov r3, #FFh
d_m21: 	djnz r3, d_m21
	djnz r4, d_m11
	ret

delay32:
	mov r4, #01h
d_m12: 	mov r3, #FFh
d_m22: 	djnz r3, d_m22
	djnz r4, d_m12
	ret

delay33:
	mov r4, #01h
d_m13: 	mov r3, #FFh
d_m23: 	djnz r3, d_m23
	djnz r4, d_m13
	ret


	; ќбработчик диспетчера системы
time0: 
	cpl P1.0

	push dph
	push dpl
	push psw
	push b
	push a
	push 0
	push 1

	mov r0,sp
	inc r0
	lcall getTskDsc
	mov r1,#0h
saveM1:
	mov a,@r1
	movx @dptr,a
	inc r1
	inc dptr
	djnz r0,saveM1

	lcall nextTask
	lcall getTskDsc 

	; восстановление SP
	movx a,@dptr
	mov r0,a
	dec r0
	mov sp,r0

	mov r1,#0h
restM2:	movx a,@dptr
	mov @r1,a
	inc r1
	inc dptr
	djnz r0,restM2

	pop 1
	pop 0
	pop a
	pop b
	pop psw
	pop dpl
	pop dph

	mov TH1,#ech
	mov TL1,#77h
	reti

nextTask:
	inc nowTask
	mov a,nowTask
	cjne a,#3h,skip
	mov nowTask,#0h
skip:	ret

getTskDsc:
	mov a,nowTask
	cjne a, #0h,s1
	mov dptr, #tsk1_d
	ret
s1:	cjne a, #1h,s2
	mov dptr, #tsk2_d
	ret
s2:	mov dptr, #tsk3_d
	ret

;str1:   db 'x  Button number Nx' ; 17-18 bytes
;str2:   db '   Button number Nx'
          ;'0123456789_123456789'

tsk1_d: db 11h,1,0,0,0,0,0,0,00,00,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
tsk2_d: db 11h,1,0,0,0,0,0,0,00,00,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
tsk3_d: db 11h,1,0,0,0,0,0,0,00,00,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0


  include C:\SHELL51\ASMS\43501_3\bk\4\p46\indic.asm
  include C:\SHELL51\ASMS\43501_3\bk\4\p46\timer.asm
  include C:\SHELL51\ASMS\43501_3\bk\4\p46\sklav.asm
  include C:\SHELL51\ASMS\43501_3\bk\4\p46\klavIn.asm
  include C:\SHELL51\ASMS\43501_3\bk\4\p46\sendRec.asm


	org FFD0h

str1:   db 'X          send: xx ' ; 17-18 bytes
str2:   db 'addr: xx   recv: xx '