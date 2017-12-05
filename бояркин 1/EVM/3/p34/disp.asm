lcd:	
	P4: equ E8h
	P5: equ F8h
indic:
	clr P5.0
	mov r4,#38h	; ��������� 8-������� ������ ������ � ��� � ������� ����� �����
	lcall ind_wr
	mov r4,#0Ch	; ����������� ������ ��� ��������
	lcall ind_wr
	mov r4,#80h	; �������� � ������� �� ������ ������� ������ 1-� ������ ������ ���
	lcall ind_wr	; ���������� � ����� ������ (R/S=1)

str1_set:	mov dptr,#strdata 	;����� ������� ������, ��� �������� ��������� ������
	setb P5.0

wr_str1: 	movx a,@dptr 
	mov r4,a
	lcall ind_wr	; ������ ������ � ���
	inc dptr
	mov a,dpl
	cjne a,#14h,wr_str1	; �������� ��������� 1-� ������

str2_set:
	clr P5.0
	mov r4,#C0h	; ������� ��� ��������� 1-� ������ ������ ������
	lcall ind_wr
	setb P5.0

wr_str2: 	movx a,@dptr
	mov r4,a
	lcall ind_wr
	inc dptr
	mov a,dpl
	cjne a,#026h,wr_str2 

	ret

ind_wr: 	mov P4, r4	; �������� � ���� P4 ������������ � ��� ����������
	setb P5.3	; ��������� ������� E
	clr P5.2		; R/W=0 (������)
	lcall delay
	clr P5.3   ; ����� ������� E
	lcall delay
	setb P5.3	; ��������� ������� E
	ret

delay:	mov r3, #7
m1:	djnz r3, m1
	ret ; 
