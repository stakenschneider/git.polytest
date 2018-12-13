REAl function integral (x)
real step
COMMON /cm1/step
integral =  exp(step*x)*sin(x)
end function

program lagrange
external integral
 REAl integral, step, X(11),Y(11),B(11),C(11),D(11),SEVAl, U, ABSERR,RELERR, RESULT,ERREST,FLAG,S
 INTEGER NOFUN,N,i
 COMMON /cm1/step
 RELERR = 1.E-07
 ABSERR = 0
 step =0
 A = 0
 B = 1
 N = 11
 X(1) = step
 CALL QUANC8(integral,A,B,ABSERR,RELERR,RESULT,ERREST,NOFUN,FLAG)
 Y(1) = RESULT
 PRINT *, Y(1)
 do i = 2,11,1
 X(i)= X(i-1)+0.2
 step = X(i)
 CALL QUANC8(integral,A,B,ABSERR,RELERR,RESULT,ERREST,NOFUN,FLAG)
 Y(i) = RESULT
 PRINT *, Y(i)
 end do
 do i=0,10,1
 U = (i-0.5)*0.2
 CALL SPLINE (N,X,Y,B,C,D)
 S=SEVAL(N,U,X,Y,B,C,D)
 PRINT *, U
 end do
 end
