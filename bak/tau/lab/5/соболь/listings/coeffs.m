function Cs=coeffs(Q)
%     A=[0 1; -0.75 -2];
%     B=[0; 1];
%     C=[0.75 0];
%     R=1;
%     
    A = [0 1; -1 0]
    B = [0; 1]
    C=[0 1];
    R=1;
    
    sys=ss(A,B,C,[0]);
    K=lqr(sys, Q, R);

    M=1/(K(1));
    g=(M+1)/M;

    Cs(1,1)=K(1);
    Cs(2,1)=K(2);
    Cs(3,1)=g;
end