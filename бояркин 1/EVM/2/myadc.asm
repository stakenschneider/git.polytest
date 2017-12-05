	org 8100h

ADCON:   	equ D8h
ADDAT:   	equ D9h
DAPR:	 equ Dah
U1: 	 equ 40h
U2: 	 equ 41h

U1_read: 	mov A,#08h ;����� �������� ������
	anl ADCON, #E0h ;������������� ���
	orl ADCON, A
	;mov DAPR, #D4h ;������ ��������
	mov DAPR, #00h ;������ ��������
	jb D8h.4,$

	mov r7, #15 ;����� �����
m1: 	djnz r7, m1 ;�����
	mov U1, ADDAT
			
U2_read: 	mov A,#0fh ; ����� �������� ������
	anl ADCON, #E0h ; ������������� ���
	orl ADCON, A
	;mov DAPR, #D4h ; ����� ��������
	mov DAPR, #62h ;������ ��������
	jb D8h.4, $

	mov r7, #15 ; ����� �����
m2: 	djnz r7, m2 ; �����
	mov U2, ADDAT

	lcall 128h ;!!!!!!!!!!!!!!
	ljmp U1_read ;!!!!!!!!!!!!!!

	ret