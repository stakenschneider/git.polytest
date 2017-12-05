P4: 	equ E8h
P5:	equ F8h

indic:	clr P5.0		; ���������� � ����� ������: RS = 0
	mov r4, #38h	; 8-������� ����� ������ � ������� ����� �����
	lcall ind_wr	; ������ ������� � ���
	mov r4, #0Ch	; ����������� ���� ��������� ������� ��� �������
	lcall ind_wr
	mov r4, #80h	; ����� ������� ������ 1-�� ������
	lcall ind_wr
			 
	mov dptr, #FFD0h
	setb P5.0 	; ���������� � ����� ������: RS = 1
	
; ������� 1-�� ������
wr_str1:	movx a, @dptr ; ������ ������ �� ������� ������
	mov r4, a
	lcall ind_wr  ; ������ ������ � ���
	inc dptr	  ; ��������� ��. ����� �����������
	mov a, dpl	; ��. ����� dptr
	cjne a, #E4h, wr_str1; �������� ��������� ������ �������� 1 ������
			
	clr P5.0
	mov r4, #C0h
	lcall ind_wr
	setb P5.0
			
; ������� 2-�� ������
wr_str2:	movx a, @dptr ; ������ ������ �� ������� ������
	mov r4, a	  ; ������ ������ � ���
	lcall ind_wr  ; ��������� ��. ����� �����������
	inc dptr 
	mov a, dpl 
	cjne a, #0F8h, wr_str2; �������� ��������� ������ �������� 2 ������
	ret
			
ind_wr: 	mov P4, r4 ; ������ � ���� �4 ������������ �������
	setb p5.3	; ��������� ������� �
	clr p5.2	; ������ R/W=0 (������)
	lcall delay
	clr p5.3	; ����� ������� �
	lcall delay
	setb p5.3	
	ret

delay:	mov r3, #7
m2:	djnz r3, m2
	ret
	