TC0Init:
  ;инициализация таймера
  anl TMOD,#11110000b
  orl TMOD,#00000001b
  mov TH0,#ech
  mov TL0,#77h
  setb ea
  setb et0
  setb tr0
  ret


  ; TC1 interruption
  org 800bh
  ljmp time0