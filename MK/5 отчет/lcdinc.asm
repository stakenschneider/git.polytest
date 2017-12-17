	indic_w0:	equ 20h
	indic_w1:	equ 21h
	cnt:	equ 22h
indic:
indic_init:	mov indic_w1,#0
	mov indic_w0,#38h
	lcall indic_wr
	mov indic_w0,#0Ch
	lcall indic_wr
	mov indic_w0,#80h
	lcall indic_wr
	mov indic_w1,#1
	mov dptr,#data	
	mov  cnt,#0
indic_data_wr_1:
	movx a,@dptr
	mov indic_w0,a
	lcall indic_wr
	inc dptr
	inc cnt
	mov a,cnt
	cjne a,#20,indic_data_wr_1
	mov indic_w1,#0
	mov indic_w0,#0C0h
	lcall indic_wr
	mov indic_w1,#1

indic_data_wr_2
	movx a,@dptr
	mov indic_w0,a
	lcall indic_wr
	inc dptr
	inc cnt
	mov a,cnt
	cjne a,#40,indic_data_wr_2

indic_exit:	ret	
indic_wr:	mov 0E8h,indic_w0
	setb p1.7
	clr p1.4
	mov a,indic_w1
	mov c,acc.0
	mov p1.6,c
	lcall indic_delay
	clr p1.7
	lcall indic_delay
	setb p1.7
	ret
indic_delay:
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	ret
 
data:	db	'                                        '                    