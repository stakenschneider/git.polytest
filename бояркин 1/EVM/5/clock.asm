	; clock.asm
	org 8300h

ms:	equ 3Fh	;����
sd10:	equ 40h	;100 ��
sec:	equ 41h	;�
min:	equ 42h	;�
hours:	equ 43h	;�

; �������������
initt:	
	;orl TMOD, #00010000b ;��� ������ � ������ 16-������� ��������

	anl TMOD, #1Fh
	mov TH1, #FEh 	;������������� �������� T/C1 ���
	mov TL1, #0Bh   ;������������ "����" 5 ��
	
	; ��������� ��������� ��� T/C1
        mov A9h, #08h
        mov B9h, #00h

	;setb ea	;���������� ���� ����������
	setb et1	;���������� ����������
	setb tr1	;���������� �����
	ret
clock:

tim1: 	mov TH1, #FEh	 ;2KHz = 500mks
	mov TL1, #17h    ; 17

	inc ms	  	 ;��������� �����
	mov r5, ms
	clr c
	mov a, r5
	subb a, #200
	jc end_tim	;���� ���������� ����� ����� 200
	mov ms, #0h
	lcall inc_dec_sec

end_tim:	reti

; �������������� ��, ��� � ���
inc_dec_sec:
	inc sd10	; ��������� 0,1 ���
	mov r5, sd10
	cjne r5, #64h, end	; �������� ��� == 0,1	
	inc sec		; ��������� ������
	mov r5, sec
	mov sd10, #0h
	cjne r5, #3Ch, end	; �������� ��� == 60
	inc min		; ��������� ������
	mov r5, min
	mov sec, #0h	
	cjne r5, #3Ch, end	; �������� ��� == 60
	inc hours		; ��������� ����
	mov min, #0h

end:	lcall to_int
	ret

	org 801bh
	ljmp tim1

	org 8500h
	
to_int:
	
; ��� ������� ����� ������
	mov a, 40h
	mov dptr, #FFDAh
	lcall overal

	dec dpl
	lcall overal
	
; ��� ������	
	mov a, 41h
	mov dptr, #FFD7h
	lcall overal
	
	dec dpl
	lcall overal
	
; ��� �����	
	mov a, 42h
	mov dptr, #FFD4h
	lcall overal
	
	dec dpl
	lcall overal

; ��� �����
	mov a, 43h
	mov dptr, #FFD1h
	lcall overal
	
	dec dpl
	lcall overal
	ret
	
overal:	mov b, #10d	;��������� ������� ���������
	div ab
	mov r1, a	
	mov a, b
	add a, #30h	;ASCII �������
	movx @dptr, a	;������
	mov a, r1
	ret

	