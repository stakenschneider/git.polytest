function sys=get_sys(K)
syms p
A = [0 1; -0.75 -2];
B = [0; 1];
C = [0.75 0];
Afull=A-B*K;

unscaled_sys=tf(ss(Afull, B, C, 0));
unscaled_num=poly2sym(unscaled_sys.num{1, 1}, p);
looped_den=poly2sym(unscaled_sys.den{1, 1}, p);

unlooped_den=looped_den-unscaled_num;
M=subs(unscaled_num/unlooped_den, p, 0);
g=(M+1)/M;

sys=ss(Afull, B*double(g), C, 0);
end