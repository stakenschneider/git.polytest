a dd ?
b db ?
c dd 12 dup ?
d db 13 dup ?
;temporary memory cells - end
;temporary memory cells - end
;temporary memory cells - end
	MOV a, b
	MOV b, c
	MOV c, d[1]
	MOV d, e[12]
:compiled_lable0
	;cycle body
	MOV a, b
	MOV b, c
	MOV c, d[1]
	MOV d, e[12]
:compiled_lable1
	;cycle body
	MOV a, b
	MOV b, c
	MOV c, d[1]
	MOV d, e[12]
	;cycle body - end
	MOV eax, b
	TEST eax
	JNZ compiled_lable1
	;condition end
	;cycle body - end
	MOV eax, b
	TEST eax
	JNZ compiled_lable0
	;condition end
	MOV a, b
	MOV b, c
	MOV c, d[1]
	MOV d, e[12]
:compiled_lable2
	;cycle body
	MOV a, b
	MOV b, c
	MOV c, d[1]
	MOV d, e[12]
	;cycle body - end
	MOV eax, c
	TEST eax
	JA compiled_lable2
	;condition end
