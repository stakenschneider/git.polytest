	org 8400h
P4: 	equ E8h
P5:	equ F8h

indic:	clr P5.0
	mov r4,#38h		; ��������� 8-������� ������ ������ � ��� � ������� ����� �����
	lcall ind_wr
	mov r4,#0Ch		; ����������� ������ ��� ��������
	lcall ind_wr

	mov r4,#80h		; �������� � ������� �� ������ ������� ������ 1-� ������ ������ ���
	lcall ind_wr		; ���������� � ����� ������ (R/S=1)
	
	mov dptr, #str1+20
	mov E4h, dpl
	mov dptr, #str1+40
	mov F8h, dpl	

	mov dptr, #FFD0h		;����� ������� ������, ��� �������� ��������� ������
	setb P5.0	

wr_str1:	movx a,@dptr		
	mov r4,a
	lcall ind_wr	; ������ ������ � ���
	inc dptr
	mov a,dpl		
	cjne a,#E4h,wr_str1	; �������� ��������� 1-� ������
	
	clr P5.0
	mov r4,#C0h	; ������� ��� ��������� 1-� ������ ������ ������
	lcall ind_wr	
	setb P5.0

wr_str2:	movx a,@dptr
	mov r4,a
	lcall ind_wr
	inc dptr
	mov a,dpl
	cjne a,#0F8h,wr_str2
	ret

ind_wr:	mov P4, r4		; �������� � ���� P5 ������������ � ��� ����������
	setb P5.3			; ��������� ������� E
	clr P5.2			; R/W=0 (������)
	lcall delay
	clr P5.3			; ����� ������� E
	lcall delay
	setb P5.3			; ��������� ������� E
	ret

delay:	mov r3, #7
m1:	djnz r3, m1	
	ret
	
	org FFD0h

str1:	db 'Microcontrollers2016'
str2:	db 20h, 20h, 20h 20h, A0h, 4Fh, B1h, 50h, 4Bh, A5h, 48h, 20h, 20h, 4Bh, 41h, 48h, 20h, 20h, 20h, 20h