	; conver.asm

; DEF, FRMSYS, TOSYS, VAL
def:	equ 60h
frmsys:	equ 61h
tosys:	equ 62h
value:	equ 63h

current:	equ 64h

; Состояния S1 S2 S3
sfrm:	equ 65h
sto:	equ 66h
swait:	equ 67h

; CNT, MAX
vcount:	equ 68h
mcount:	equ 69h


initsys:
	; Установка состояния S1
	mov sfrm, #1h
	mov sto, #0h
	mov swait, #0h

	mov vcount, #0h
	mov mcount, #0h
	ret

gloop:	
	mov a, 34h
	add a, #0h
	jz exgloop
	clr c
	subb a, #FFh
	jz exgloop

	; Если состояние S1
	mov a, sfrm
	clr c
	subb a, #1h
	jnz nextto
	lcall hfrm
	jmp exgloop

nextto:
	; Если состояние S2
	mov a, sto
	clr c
	subb a, #1h
	jnz nextwait
	lcall hto
	jmp exgloop

nextwait:
	; Если состояние S3
	mov a, swait
	clr c
	subb a, #1h
	jnz lastsys
	lcall hwait
	jmp exgloop
	

lastsys:
	; Перевод в систему счисления и вывод на индикацию
	lcall tosyshdr
	lcall initsys

exgloop:	
	ret

;;;;;;;;;;;;;;;;;;;;;;;;;;

; Обработчик состояния S1

hfrm:
	lcall clrall

	mov a, 34h
	clr c
	subb a, #2
	jnz frmnext8
	mov frmsys, #2
	mov mcount, #8
	jmp chfrm
	
frmnext8:
	mov a, 34h
	clr c
	subb a, #8
	jnz frmnext10
	mov frmsys, #8
	mov mcount, #3
	jmp chfrm

frmnext10:
	mov a, 34h
	clr c
	subb a, #10
	jnz frmnext16
	mov frmsys, #10
	mov mcount, #3
	jmp chfrm

frmnext16:
	mov a, 34h
	clr c
	subb a, #16
	jnz exfrm
	mov frmsys, #16
	mov mcount, #2
	jmp chfrm

chfrm:
	mov sfrm, #0h
	mov sto, #1h
	mov swait, #0h
	lcall drwfrm;
exfrm:
	ret

;;;;;;;;;;;;;;;;;;;;;;;;;;

; Обработчик состояния S2

hto:
	mov a, 34h
	clr c
	subb a, #2
	jnz tonext8
	mov tosys, #2
	mov def, #128
	jmp chto
	
tonext8:
	mov a, 34h
	clr c
	subb a, #8
	jnz tonext10
	mov tosys, #8
	mov def, #64
	jmp chto

tonext10:
	mov a, 34h
	clr c
	subb a, #10
	jnz tonext16
	mov tosys, #10
	mov def, #100
	jmp chto

tonext16:
	mov a, 34h
	clr c
	subb a, #16
	jnz exto
	mov tosys, #16
	mov def, #16
	jmp chto

chto:
	mov sfrm, #0h
	mov sto, #0h
	mov swait, #1h
	lcall drwto;
exto:
	ret

;;;;;;;;;;;;;;;;;;;;;;;;;;

; Обработчик состояния S3
; Алгоритм перевода в двоичное представление

hwait:
	mov a, 34h
	dec a
	clr c
	subb a, frmsys
	jc suwait
	jmp badres

suwait:
	mov a, value
	mov b, frmsys
	mul ab
	mov value, a
	mov a, b
	add a, #0h
	jnz badres
	mov a, value
	dec a
	add a, 34h
	mov value, a
	inc vcount
	lcall drwwait
	clr c
	mov a, vcount
	subb a, mcount
	jnz exwait
	mov sfrm, #0h
	mov sto, #0h
	mov swait, #0h
	jmp exwait
	
badres:
	mov value, #0h
	mov vcount, #0h
	lcall clrval
	
exwait:
	ret

;;;;;;;;;;;;;;;;;;;;;;;;;;

; Индикация состояния S1

drwfrm:
	mov dptr, #str1 + 14

	mov a, frmsys
	clr c
	subb a, #10
	jc seccrf
	mov a, #31h
	movx @dptr, a
	mov a, frmsys
	clr c
	add a, #26h
	jmp secsmf
	
seccrf:
	mov a, frmsys
	add a, #30h

secsmf:
	inc dptr
	movx @dptr, a
	ret;

;;;;;;;;;;;;;;;;;;;;;;;;;;

; Индикация состояния S2

drwto:
	mov dptr, #str1 + 18

	mov a, tosys
	clr c
	subb a, #10
	jc seccrt
	mov a, #31h
	movx @dptr, a
	mov a, tosys
	clr c
	add a, #26h
	jmp secsmt
	
seccrt:
	mov a, tosys
	add a, #30h

secsmt:
	inc dptr
	movx @dptr, a
	ret


;;;;;;;;;;;;;;;;;;;;;;;;;;

; Индикация состояния S3

drwwait:
	mov dptr, #str2
	
	mov r0, #0h
drwwloop:
	inc r0
	inc dptr
	mov a, r0
	cjne a, vcount, drwwloop
	
	mov a, 34h
	dec a

	mov r0, a
	
	clr c
	subb a, #Ah
	mov a, r0
	jc drwwnum
	add a, #7h
drwwnum:
	add a, #30h

	movx @dptr, a

	ret

;;;;;;;;;;;;;;;;;;;;;;;;;;

; Очистка индикации переводимого значения

clrval:
	mov dptr, #str2 + 1
	mov r0, #8h
clrvsl:
	dec r0
	mov a, #20h
	movx @dptr, a
	inc dptr
	mov a, r0
	cjne a, #0h, clrvsl
	ret


;;;;;;;;;;;;;;;;;;;;;;;;;;

; Очистка всей индикации

clrall:	
	mov dptr, #str1 + 14
	mov a, #20h
	movx @dptr, a
	inc dptr
	mov a, #20h
	movx @dptr, a

	mov dptr, #str1 + 18
	mov a, #20h
	movx @dptr, a
	inc dptr
	mov a, #20h
	movx @dptr, a

	mov dptr, #str2 + 1
	mov r0, #8h
clrv:
	dec r0
	mov a, #20h
	movx @dptr, a
	inc dptr
	mov a, r0
	cjne a, #0h, clrv

	mov dptr, #str2 + 12
	mov r0, #8h
clrr:
	dec r0
	mov a, #20h
	movx @dptr, a
	inc dptr
	mov a, r0
	cjne a, #0h, clrr

	ret