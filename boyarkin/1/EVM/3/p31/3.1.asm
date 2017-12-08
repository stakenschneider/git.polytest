	org 8100h
	mov  r5, #ECh  		; TH
	mov  r6, #77h  		; TL

	lcall   init		; ����. ������� � ������� ����������
			
loop: 					; ������� ����
	lcall memklav		
	mov  dptr, #8300h   ; ���� ��� TH TL
	mov  a, 34h			; ������ ����� ������� �������
	clr  c				; ������� ��� �
	rlc  a				; ����� ����� � (�� ����� A[0] ��� 0 - ��� �)
	push a				; ��������� � � �����
	movc  a, @a+dptr	; � � ��� �������� �� ������ [� + DPTR]
	mov  r5, a			; ����� � � ��
	pop a				; ��������������� � �� �����
	inc  a				; �������������� ��� �������� �� TL
	movc  a, @a+dptr	; � � ��� �������� �� ������ [� + DPTR]
	mov  r6, a			; ����� � � �L
	sjmp  loop
	
init:
	anl  TMOD,#11110000b; ����. ������� ��� ������
	orl  TMOD,#00000001b; � ������ 16-������� ��������
	mov  TH0, r5		; ����. �������� �/�0 ���
	mov  TL0, r6		; ������������ ���� 5 �� (��-���������)
	setb  ea			; ���������� ���� ����������
	setb  et0			; ����������  ���������� �/�0
	setb  tr0			; ���������� �����
	ret
tim0: 
	mov  TH0, r5		; ����. �������� �/�0 ���
	mov  TL0, r6		; ������������ ���� 5 �� (��-���������)
	cpl  P1.3			; ������������ �������
	reti
	
	org  800Bh			; ���������� ���������� �/�0
	ljmp  tim0
	
	include ASMS\43501_3\bk\3\sklav.asm
	
	org  8300h 			; ������ ����� ��� TH TL
cod db ECh,A8h, ECh,A8h, F6h,47h, F9h,8Ch, FBh,29h, FCh,24h, FCh,C9h, FDh,41h, FDh,9Ah, FDh,E0h, FEh,16h, FEh,44h, FEh,6Ah, FEh,8Ah, FEh,A4h, FEh,BCh, FEh, D2h	
;cod db ECh,77h, ECh,77h, F6h,3Bh, F9h,7Ch, FBh,1Dh, FCh,17h, FCh,BEh, FDh,34h, FDh,8Eh, FDh,D3h, FEh,0Bh, FEh,38h, FEh,5Eh, FEh,7Eh, FEh,9Ah, FEh,B2h, FEh,C6h	
	