	org 8500h
	ljmp start_main
start_main:
	lcall U1_read
convert1:
	mov 35h, U1 ; ����������� �������� �� �������� �� � ���������� ��
	mov a, 35h ; ��� ����� ����� �� 10 ��� ����
	mov b, #10
	div ab
	mov 36h, b ; ������� ������� �� �������
	mov b, #10
	div ab
	mov 37h, b
	mov 38h, a
	mov r0, #36h
ascii1:
	mov a, @r0 ; ����������� ���������� �����
	add a, #30h ; � HEX-��� ��� ������ �� �����
	mov @r0, a
	inc r0
	cjne r0, #39h, ascii1 ; ������ ��� �� ������ � ������� 39h
	mov r0, #36h
	mov dptr, #str1+19
show1: ; ������������ ����� ����������� ����������� �����
	mov a, @r0
	movx @dptr, a
	dec dpl
	inc r0
	cjne r0, #39h, show1 ;����������� �������� �� �������� �� � ���������� ��
convert2:
	mov 39h, U2
	mov a, 39h
	mov b, #10
	div ab ; ��� ����� ����� �� 10 ��� ����
	mov 3Ah, b ; ������� ������� �� �������
	mov b, #10
	div ab
	mov 3Bh, b
	mov 3Ch, a
	mov r0, #3Ah
ascii2:
	mov a, @r0
	add a, #30h ; ����������� ���������� �����
	mov @r0, a ; � HEX-��� ��� ������ �� �����
	inc r0
	cjne r0, #3Dh, ascii2 ; ������ ��� �� ������ � ������� 3Dh
	mov r0, #3Ah
	mov dptr, #str2+19
show2:
	mov a, @r0 ; ������������ ����� ����������� ����������� �����
	movx @dptr, a
	dec dpl
	inc r0
	cjne r0, #3Dh, show2
	lcall indic
	inc r0
	ret

	include ASMS\43501_3\bk\2\myadc.asm; ��������� �������� � ���
	include ASMS\43501_3\bk\2\indic.asm; ���������� ��������� ���������

	;str1: db "potenciometr"
	;str2: db "integrator"