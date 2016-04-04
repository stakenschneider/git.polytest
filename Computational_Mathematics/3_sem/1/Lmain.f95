REAl function integral (x)
real step
COMMON /cm1/step
integral =  exp(step*x)*sin(x)
end function

program lagrange
external integral
 REAl F,RELERR,ABSERR,RESULT,ERREST,FLAG, A, B,integral, step, num, denom
 REAl, dimension (0:10, 1:2)::arr
 INTEGER NOFUN, i, j, z
 COMMON /cm1/step
 RELERR = 1.E-07
 ABSERR = 0
 step =0
 A = 0
 B = 1
 arr(0,1) = step
 CALL QUANC8(integral,A,B,ABSERR,RELERR,RESULT,ERREST,NOFUN,FLAG)
 arr(0,2) = RESULT
 PRINT *, arr(0,2)
 do i = 1,10,1
 arr(i,1)= arr(i-1,1)+0.2
 step = arr(i,1)
 CALL QUANC8(integral,A,B,ABSERR,RELERR,RESULT,ERREST,NOFUN,FLAG)
 arr(i,2) = RESULT
 PRINT *, arr(i,2)
 end do
 do j = 0, 10, 1
 F = 0
    do z = 0, 10 , 1
    num = 1
    denom = 1
    do i = 0,10,1
    if ( i /= z ) then
    num=((j-0.5)*0.2-arr(i,1))*num
    denom=(arr(z,1)-arr(i,1))*denom
    endif
    enddo
    F = arr(z,2)*num/denom + F
 enddo
  PRINT *, F
 end do

 end
