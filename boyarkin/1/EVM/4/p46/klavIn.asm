klavIn:

	clr TB8
 ; SLAVE �� ����� ����� ������ ��������
	mov a, status
	clr C
	subb a, #53h	; S-Slave
	jz s_end	; �!=0 => ��������� ��������




 ; ****** button15 #0fh=15 (��������� ������ "Master/Slave")
	mov a, button
	clr C
	subb a, #0Fh	
	jnz s_m1; �!=0 => ��������� ��������

	; ���� ���� ������ 15
	mov a, #4Dh	; #4Dh="M"(master)
	mov status, a	; status=Master
	mov dptr, #str1
	movx @dptr, a	; ������������� ����� M
	jmp s_next	



 ; ������� ����� - ����� ������ ��������
s_m1:	mov a, status
	clr C
	subb a, #20h	;_-������� �����
	jz s_next	; � ���. �����




 ; ****** button14 #0Eh=14 (�������� ������)
	mov a, button
	clr C
	subb a, #0Eh	
	jnz s_m2	; �!=0 => ��������� ��������

	; ���� ������ 14
	setb TB8	; ���������� ���=>�������� �����

	mov a, #31h	; #31h="1"
	mov dptr, #str2+6	; �� ����� 1�� �
	movx @dptr, a	; �������� "1" � str2

	mov a, #34h	; #34h="4"
	inc dptr
	movx @dptr, a	; �������� "4" � str2
	jmp s_next


  ; ****** button13 #0Dh=13 (�������� ������)
s_m2:	mov a, button
	clr C
	subb a, #0Dh	
	jnz s_m3	; �!=0 => ��������� ��������

	; ���� ������ 13
	setb TB8	; ���������� ���=>�������� �����
	mov a, #31h	; #31h="1"
	mov dptr, #str2+6	; �� ����� 1�� �
	movx @dptr, a	; �������� "1" � str2

	mov a, #33h	; #33h="3"
	inc dptr
	movx @dptr, a	; �������� "3" � str2

	jmp s_next

  ; ****** button12 #0Ch=12 (���������� �������)
s_m3:	mov a, button
	clr C
	subb a, #0Ch	
	jnz s_next	; �!=0 => ������� ��������,
			; ������ ��������

	; ���� ������ 12 (����� �������)
	setb TB8	; ����� ����� ������� �� ���
	clr SM2		; �������� ����
	mov status, #20h	; �������� ������
	mov a, #78h	; #78h="x"
	mov dptr, #str1
	movx @dptr, a	; ����� ������

	mov dptr, #str2+6
	movx @dptr, a	; �������� "x" � str2
	inc dptr
	movx @dptr, a	; �������� "x" � str2
	jmp s_next

s_next:
 ; ����� ������ ������� ������ �� ���������
	mov r0, #34h  ; address of inside memmory
	mov a, @r0  ; number of button 1-16
	mov b, #0Ah ; b = 10
	div ab  ; a/b  => a=0-1,  b=0-6
	mov dptr, #str1+17  ; address "N" in str1
	add a, #30h  ; translate to ASCII-code
	movx @dptr, a ; "N" <=> 0-1
	mov a, b  ; b=0-6
	add a, #30h  ; ASCII-code
	inc dptr  ; address "x" in str2
	movx @dptr, a ; "N" <=> 0-1
s_end:
	ret





klavIn2:
 ; MASTER ������ �� ���������
	mov a, status
	clr C
	subb a, #4Dh	; M-Master
	jz r_end	; �!=0 => ��������� ��������

 ; ����������� �������

 ; ****** button15 #0fh=15 (��������� ������ "Master/Slave")
	mov a, button2
	clr C
	subb a, #0Fh	
	jnz r_m1	; �!=0 => ��������� ��������

	; ���� ���� ������ 15
	setb SM2	; ������ ����� �������������
	mov a, #53h	; #53h="S"(Slave)
	mov status, a	; status=Slave
	mov dptr, #str1
	movx @dptr, a	; ������������� ����� S
	jmp r_next



 ; ������� ����� - ����� ������ ��������
r_m1:	mov a, status
	clr C
	subb a, #20h	;_-������� �����
	jz r_next	; � ���. �����


  ; ****** button13 #0Dh=13 (�������� ������)
	mov a, button2
	clr C
	subb a, #0Dh	
	jnz r_m2	; �!=0 => ��������� ��������

	; ���� ������ 14
	clr SM2		; ���� ������

	mov a, #31h	; #31h="1"
	mov dptr, #str2+6	; �� ����� 1�� �
	movx @dptr, a	; �������� "1" � str2

	mov a, #33h	; #34h="3"
	inc dptr
	movx @dptr, a	; �������� "3" � str2
	jmp r_next


  ; ****** button14 #0Eh=14 (�������� ������)
r_m2:	mov a, button2
	clr C
	subb a, #0Dh	
	jz r_end	; ���� ������ 14


  ; ****** button12 #0Ch=12 (���������� �������)
	mov a, button2
	clr C
	subb a, #0Ch	
	jnz r_next	; �!=0 => ������� ��������,
			; ������ ��������

	; ���� ������ 12 (����� �������)
	clr SM2		; �������� ����
	mov status, #20h	; �������� ������

	mov a, #78h	; #78h="x"
	mov dptr, #str1
	movx @dptr, a	; ����� ������
	mov dptr, #str2+6
	movx @dptr, a	; �������� "x" � str2
	inc dptr
	movx @dptr, a	; �������� "x" � str2
	jmp r_next

r_next:
 ; ����� ������ ������� ������ �� ���������
	mov r0, #35h  ; address of inside memmory
	mov a, @r0  ; number of button 1-16
	mov b, #0Ah ; b = 10
	div ab  ; a/b  => a=0-1,  b=0-6
	mov dptr, #str2+17  ; address "N" in str1
	add a, #30h  ; translate to ASCII-code
	movx @dptr, a ; "N" <=> 0-1
	mov a, b  ; b=0-6
	add a, #30h  ; ASCII-code
	inc dptr  ; address "x" in str2
	movx @dptr, a ; "N" <=> 0-1
r_end:
	ret