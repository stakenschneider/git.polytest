klavIn:
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

  ret


klavIn2:
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

  ret