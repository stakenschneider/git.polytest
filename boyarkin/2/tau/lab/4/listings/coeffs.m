function Cs=coeffs(Q)
    A=[0 1; 0 -25];
    B=[0; 1];
    C=[25 5];
    R=1;
    
    sys=ss(A,B,C,[0]);
    K=lqr(sys, Q, R);

    M=25/(K(1)-25);
    g=(M+1)/M;

    Cs(1,1)=K(1);
    Cs(2,1)=K(2);
    Cs(3,1)=g;
end