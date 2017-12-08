  org 8300h
timerInit:
  ; timer`s initialization (mode: 16-bits counter)
  anl TMOD, #11110000b
  orl TMOD, #00000001b

  ; counter`s initialization (mode: "tik" = 5 mcs)
  mov TH0, #ECh
  mov TL0, #77h

  setb ea   ; enable of ALL interruptions
  setb et0  ; enable of T/C0 interruption
  ;setb tr0  ; enable of count
  mov r7, #0
  ret

timerTim0:
  mov TH0, #ECh
  mov TL0, #77h
  cpl p1.0
  inc r7
  cjne r7, #200, TC0_reti

  ; остновка таймера
  clr tr0
  lcall clear
  ;mov number, #40
  setb ri

  ;'E'
  mov dptr, #str1
  mov a, #45h
  movx @dptr, a
  ;'r'
  inc dptr
  mov a, #72h
  movx @dptr, a
  ;'r'
  inc dptr
  movx @dptr, a
  ;'o'
  inc dptr
  mov a, #6Fh
  movx @dptr, a
  ;'r'
  inc dptr
  mov a, #72h
  movx @dptr, a
  ;'!'
  inc dptr
  mov a, #21h
  movx @dptr, a
TC0_reti:
  reti


; Interrupt handler
  org 800bh
  ljmp timerTim0