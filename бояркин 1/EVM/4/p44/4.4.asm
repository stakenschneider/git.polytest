	org 8100h

p5:	equ	f8h
p4:	equ	e8h
cmd:	equ	40h	; ������ ��� �������
hresult:	equ	41h	; ������ ������� ����
lresult:	equ	42h	; ������ ������� ����
cnt:	equ	43h	; ������� �������� �����
flag:	equ	44h
number:	equ	45h
breakp:	equ	46h


start:	lcall init		; ����. ����� � ��
	lcall timerInit   	; ����. T/C0 (��� �������)
	lcall clear		; ������� ���������
	mov hresult, #0h
	mov lresult, #0h
	mov flag, #0h
	mov cnt, #0h
	mov number, #0h
	mov breakp, #0h

rec_str:	jnb ri, rec_str	; ���� �������
	setb tr0  		; ������ ������ ����� 1�� ��������
	mov a, sbuf	; �������� ����, ��� �������
	clr ri		; ����� �����
	mov cmd, a
	
	cjne a, #2Dh, work	; ���� ����� ��������
	mov lresult, #FFh	; �� ��������� Min=FF

 ; �������� ����� �������
work:	
	clr C
	mov a, breakp
	subb a, #1h
	jz bBad

	lcall receive
	jmp handler
	jmp work

handler:	mov r6, a	; ��������� ��� �������

	; ASCII ��� ' '=20h - ������, ����������� ����� �������
	clr C
	subb a, #20h
	jnz nextBreak 
	mov a, flag
	subb a, #1h
	mov flag, #0h
	jz nextDo
	jmp work

nextBreak:
	; ASCII ��� '>'=3Eh - ����� ��������
	mov a, r6
	clr C
	subb a, #3Eh
	jnz nextCheck 
	mov a, flag
	subb a, #1h
	mov flag, #0h
	mov breakp, #1h
	jz nextDo
	jmp work

nextCheck:
	clr C
	mov a, flag
	subb a, #1h
	mov flag, #0h
	jz nextDo
	mov flag, #1h
	lcall refreshNum
	jmp work

nextDo:
	lcall refreshNum

	; ASCII ��� '+'=2Bh - top/����� ���������
	mov a, cmd
	clr C
	subb a, #2Bh
	jz maxMas

	; ASCII ��� '-'=2Dh - bottom/����� ��������
	mov a, cmd
	clr C
	subb a, #2Dh
	jz minMas

	; ASCII ��� ��������� �������� - ��.����
	inc cnt		; ���-�� �������� ����
	mov a, number
	add a, lresult
	jnc aves
	inc hresult
aves:
	mov lresult, a
	mov number, #0h
	jmp work

maxMas:
	mov a, number
	clr C
	subb a, lresult
	jc work	;  ���� ����=> ����.�� �����
	; ���� ���� �� ��������� => ��������� ����.
	mov lresult, number
	mov number, #0h
	jmp work

minMas:
	clr C
	mov a, number
	subb a, lresult
	jnc work	;  ���� ����� ���=>�������.�� �����
	; ���� ���� ����=> ��������� �������
	mov lresult, number
	mov number, #0h
	jmp work	

bBad: 
	clr tr0	; ��������� �������

	; ���������� �������
	mov dptr, #str1
	mov a, cmd
	movx @dptr, a
	
	; ����������, ���� ��� ��.�����.
	; ASCII ��� '/'=2Fh - ��.�����
	mov a, cmd
	clr C
	subb a, #2Fh
	jnz bb_m1
	
	mov r6, #0h

	mov a, #FFh
	mov b, cnt
	div ab
	mov r6, a

	mov a, hresult
	mov b, r6
	mul ab
	mov r6, a

	mov a, lresult
	mov b, cnt
	div ab
	add a, r6
	mov r6, a

	mov a, hresult
	mov b, cnt
	div ab
	add a, r6
	mov r6, a

	mov hresult, #0h
	mov lresult, r6

bb_m1:	; ���������� ���������

	inc dptr
	inc dptr

	mov a, lresult
	anl a, #F0h
	rr a
	rr a
	rr a
	rr a
	mov r6, a
	clr C
	subb a, #10
	mov a, r6
	jc hout
	add a, #7h
hout:
	add a, #30h
	
	movx @dptr, a
	inc dptr

	mov a, lresult
	anl a, #0Fh
	mov r6, a
	clr C
	subb a, #10
	mov a, r6
	jc lout
	add a, #7h
lout:
	add a, #30h
	
	movx @dptr, a
	inc dptr


	mov r6, #0h
lloop:
	inc r6
	inc dptr
	mov a, lresult
	clr C
	rlc a
	mov lresult, a
	jc lone
	mov a, #30h
	movx @dptr, a
	mov a, r6
	cjne a, #8h, lloop
	ljmp lexit
lone:
	mov a, #31h
	movx @dptr, a
	mov a, r6
	cjne a, #8h, lloop
	
lexit:	
	lcall indic	; ���������
	jmp start
	ret

refreshNum:
	mov a, number
	rl a
	rl a
	rl a
	rl a
	mov number, a
	
	lcall cascii
	add a, number
	mov number, a

	ret

cascii:	
	mov a, r6
	clr C
	subb a, #40h
	mov a, r6
	jc dascii 
hascii:
	clr C
	subb a, #7h
dascii:	
	clr C
	subb a, #30h
	ret

 ; ****** FUNCTIONS *****
 ; ��������� send_rec (����� � ������������)
init:	clr tr1		   ; ��������� ����
	mov scon, #11010010b
	anl tmod, #0Fh
	orl tmod, #00100000b
	mov th1, #e6h	;  1200 ���/�
	anl D8h, #7Fh
	anl 87h, #7Fh
	setb tr1		; ���������� TC1
	ret


 ; ����� ���������������� ������ �� ��
receive:	jnb ri, receive
	mov a, sbuf
	
	clr ri
	ret

 ; ������������ ��� ������� ���������
clear:	mov dptr, #str1
	mov r7, #40
	mov a, #20h	; ��� �������
cl_m1:	movx @dptr, a
	inc dptr
	dec r7
	cjne r7, #0, cl_m1
	ret

	include ASMS\43501_3\bk\4\p44\tmp\indic.asm
	include ASMS\43501_3\bk\4\p44\tmp\timer.asm

	org FFD0h
str1:   	db  '                    '
str2:   	db  '                    '
