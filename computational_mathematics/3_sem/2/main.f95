program main
 INTEGER NDIM,N,IPVT(12)
    REAL WORK(12),COND, CONDP1, TRANS(12,12), DELTA, MATRIXNORM
    REAL C(12,12),C1(12,12),C2(12,12), D(12),D1(12),D2(12),SUMD
    NDIM = 12
    N = 12
    OPEN(0, FILE = 'OUT.txt')
    DO i=1,N,1
      DO j=1, N, 1
        C(i,j) =1.0/(i+j-1.0)
        end do
        end do
    DO i =1, N, 1
        SUMD = 0
        DO j=1, N, 1
        SUMD = 1.0/(j+i-1.0)+SUMD
        end do
        D(i)=SUMD
        end do
      C1=C
      D1=D
      CALL DECOMP(NDIM,N,C1,COND,IPVT,WORK)
      PRINT 101,((C1(i,j),j=1,N),i=1,N)
      PRINT 102,COND
      CONDP1=COND+1.0
      IF(CONDP1.EQ.COND) PRINT 103
      IF(CONDP1.NE.COND) CALL SOLVE(NDIM,N,C1,D1,IPVT)
      IF(CONDP1.NE.COND) PRINT 104,(D1(I),I=1,N)
      D2=D
      C2=C
      TRANS = TRANSPOSE(C2)
      C2 = MATMUL(TRANS, C2)
      D2 = MATMUL(TRANS, D2)
      CALL DECOMP(NDIM,N,C2,COND,IPVT,WORK)
      PRINT 106,((C2(I,J),J=1,N),I=1,N)
      PRINT 102,COND
      CONDP1=COND+1.0
      IF(CONDP1.EQ.COND) PRINT 103
      IF(CONDP1.NE.COND) CALL SOLVE(NDIM,N,C2,D2,IPVT)
      IF(CONDP1.NE.COND) PRINT 104,(D2(I),I=1,N)
      IF(CONDP1.NE.COND) DELTA = MATRIXNORM(D1 - D2)/MATRIXNORM(D1)
      IF(CONDP1.NE.COND) PRINT 105,DELTA

  101 FORMAT(5X,'C',10(/5X,12F10.6))
  102 FORMAT(5X,'COND = ',E12.5)
  103 FORMAT(5X,'Matrix classified as degenerate')
  104 FORMAT(5X,'Solution X',3(/12X,F10.7))
  105 FORMAT(5X,'Delta = ', E12.5)
  106 FORMAT(5X,'C1',10(/5X,12F10.6))
      end

REAl function MATRIXNORM(D)
 REAL D(12)
 INTEGER I
 MATRIXNORM = 0.0
 DO I = 1, 12
  MATRIXNORM = MATRIXNORM + D(I)**2
 end do
 MATRIXNORM = MATRIXNORM**(0.5)
END function MATRIXNORM
