	org 8100h
	lcall init
work:	lcall receive
	cpl a
	lcall send
	sjmp work

init:	clr tr1
	mov scon, #11010010b
	anl tmod, #0Fh
	orl tmod, #00100000b
	mov th1, #e6h
	anl D8h, #7Fh
	anl 87h, #7Fh
	setb tr1
	ret
receive:	jnb ri, receive
	mov a, sbuf
	clr ri
	ret

send:	jnb ti, send
	mov sbuf, a
	clr ti
	ret