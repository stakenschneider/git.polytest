function obs_sys=get_observer(L, K)
syms p
A = [0 1; -0.75 -2];
B = [0; 1];
C = [0.75 0];

Afull=[A-B*K, B*K; zeros(2), A-L*C];
Bfull=[B; zeros(2, 1)];
Cfull=[C, zeros(1, 2)];

unscaled_sys=tf(ss(Afull, Bfull, Cfull, 0));
unscaled_num=poly2sym(unscaled_sys.num{1, 1}, p);
looped_den=poly2sym(unscaled_sys.den{1, 1}, p);

unlooped_den=looped_den-unscaled_num;
M=subs(unscaled_num/unlooped_den, p, 0);
g=(M+1)/M;

obs_sys=ss(Afull, Bfull*double(g), Cfull, 0);
end