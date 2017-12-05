	org 8400h
T2CON: 	equ 0C8h

pwm:
	lcall U1_read
	lcall calc
	lcall init
	;lcall 128h
	ljmp pwm
	ret

calc:
	mov a, U1
	mov b, #03h        ; целая часть
	mul ab
	mov r2, b        ; старший байт
	mov r3, a        ; младший байт
	
	mov a, U1
	mov b, #ECh      ; дробная часть
	mul ab
	mov a, b         ; помещаем в АКК ст. байт
	clr c            ; сброс переноса
	add a, r3        ; добавляем к 1му произведению
	mov r3, a        ;  ст. байт 2го произведения
	mov a, r2
	addc a, #0h      ; учет переноса
	mov r2, a

	mov a, r2        ; инвертирование константы
	cpl a
	mov r2, a
	mov a, r3
	cpl a
	mov r3, a

	ret

init:
	orl p1, #00001000b ; настройка порта P1.3 для вывода ШИМ
	mov T2CON, #0 ; остановка таймера T/C2

	; Задаем частоту
	mov CBh, #FCh ; Старший байт
	mov CAh, #17h ; Младший байт

	mov C1h, #00001000b ; разрешение работы 1-го канала
	mov C3h, r2
	mov C2h, r3
	mov T2CON, #00010001b ; задание режима генератора
	ret

	;include ASMS\43501_3\bk\3\p32\myadc.asm