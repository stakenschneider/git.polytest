	org 8700h

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

memklav:
	mov 20h, #0h ; 0 for clear C
	mov R1, #33h 	; ����� ������ ������ ������ ��� ���������
	mov R3, #3h	; �������(�� ������� � ��������)
	mov 35h, #0h 	; ������� ������� ������
	mov 37h, #0h 	; ��� �������
	mov 38h, #0h	; ����� ������
	mov 39h, #0h	; ����� �������
	lcall klav
	
	; ������� - �������� �� ���� (������ �� ������)
zero_chk:
	mov C, 0h		; clear C
	mov A, @R1 	; ������ ������ �� ������
	;mov 56h, R1
	subb A, #f0h	; �������� 0Fh - ���� ����� ����, �� ������ �� ������.
			; ����� �������, ��� ���� �����-������ �������.	
	jz skip_cntr	; A==0 - ���������� ������� �������
	inc 35h		; �� ���� - ��������� �������� �������
	mov A,@R1
	mov 37h,A		; ��������� ��� ������� �������.
	mov 38h,R3	; ��������� ����� ������ ������� �������

skip_cntr:	
	dec R1		; ���� ��������� ������� �� ������
			; ���� �� �������� ����� ������� ��� �������� -
	dec R3		; ����������� ����� ������
	mov C, 0h		; clear C
	cjne R1, #2Fh, zero_chk ; - ���������� ����
	; ����� �� ����� �������� ���������� �������
		

	mov A, 35h	; ������ � � ������� �������
	jz wr_0		; 0 ������� - ����� ����
	mov C, 0h		; clear C
	cjne A, #01h, wr_FF 	; ������ 1 ������� - ����� FF
	
	mov dptr, #cdMask 	; ������ ������� �����
	mov R3,#0h;	; �������� �������
	
find_column:
	inc R3;		; ������� ������ �������
	mov 39h,R3	; ��������� ����� �������
	mov A,R3;
	mov C, 0h		; clear C
	subb A,#5h
	jz wr_FF ; �.�. ������� ����� ������(��� ���������)
		; �� ��� ����������� ������ ������� � �������
		; ����� - ���� ������ ��������� ������, � ��� �� ������
	movx A, @dptr	; �������� ������� 
	inc dptr		; ����� inc ������ � �������
	mov C, 0h		; clear C
	cjne A, 37h, find_column ; ���� ����� �� ����� ����������,
			; ��������� �����
get_num:
	; ����� ������*4+����� �������
	mov A, 38h
	mov C, 0h		; clear C	
	rl A
	rl A			; ��� ������ ����� =*4
	;add A, 39h		; �������� �����
	add A, #5h
	subb A, 39h
	mov 34h, A	; ������ ����� 
	sjmp ext
wr_0:	mov 34h, #0h
	sjmp ext
wr_FF:	mov 34h, #FFh
	sjmp ext			

	; ������������ ���� ������ - ���������� ��� �������.
cdMask: db E0h, D0h, B0h, 70h
	
ext:	ret

p5:	equ f8h

klav:	mov r0, #30h	; �����������������������
	orl p5, #f0h	; ���������������������
	mov a, #7fh	; ������������������������

mb:	mov r2, a

	rlc a
	mov p1.7, c
	rlc a
	mov p1.6, c
	rlc a
	mov p1.5, c
	rlc a
	mov p1.4, c
	
	mov a, p5	; ��������������������������
	anl a,#f0h
	mov @r0, a	; �������������
	inc r0		; ��������������������������
	mov a, r2
	rr a		; �����������������
	cjne a, #f7h, mb; �������������
	ret

;���������� ���������� ����� � ����������� �������
decim:	mov a,34h
	cjne a,#ffh, wrff
	mov a,#46h
	mov dptr, #str2 + 17
	movx @dptr,a
	inc dptr
	movx @dptr,a
	ret

wrff:	mov dptr, #str2 + 17
	mov a, 34h
	mov b, #10	
	div ab 
	add a, #30h
	movx @dptr, a
	inc dptr
	mov a, b 
	add a, #30h
	movx @dptr, a
	ret

; �����������
		org FFD0h
str1:	db	20h, 20h, 20h, 3Ah, 20h, 20h, 20h, 3Ah, 20h, 20h, 20h, 2eh, 20h, 20h, 30h, 30h, 20h, 20h, 20h, 20h	
str2:	db	'BUTTON NUMBER:      '