	org 8100h

p5:	equ	f8h
p4:	equ	e8h
number:	equ	40h	; ������ ���-�� ��������

	
start:	lcall init		; ����. ����� � ��
	; ��������������, �� �� ��������� T/C0
	lcall timerInit
	lcall clear		; ������� ���������
	mov number, #0h

rec_str:	jnb ri, rec_str	; ���� ��������
	setb tr0  		; ������ ������ ����� 1�� ��������
	mov a, sbuf	; �������� ����
	clr ri		; ����� �����
	
	jmp handler

work:	lcall receive
	jmp handler
	sjmp work

 ; ��������, ��� �������� <= 40
handler:	mov r7, a	; ��������� ��� �������
	inc number	; ���-�� �������� ����
	mov dptr, #str1	; ���������� ������ ������
	mov a, number	;
	cjne a, #41, handler2	; ���� !=41 => ���� ������
	jmp breakingBad	; ���� 41 => �����

 ; ����� dptr � ������ �����
handler2:	dec a		; ������ ����� �������
	jz handler3	; ���� 0 => ������ ������� � dptr
	inc dptr
	jmp handler2

 ; ������ � ������ ����� � ������
handler3:	mov a, r7
	movx @dptr, a
	cjne   a,   #2Eh,   work	; �������� �� �����

breakingBad: clr tr0	; ��������� �������
	lcall indic	; ���������

	jmp start
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
	
 	include ASMS\43501_3\bk\4\p43\indic.asm
	include ASMS\43501_3\bk\4\p43\timer.asm

	org FFD0h
str1:   	db  '                    '
str2:   	db  '                    '
	
 	


