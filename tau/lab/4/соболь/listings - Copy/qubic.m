clc;
syms T k p
D = T*p^3+(2*T+1)*p^2+(0.75*T+0.75*k+2)*p+0.75
B = 0.75*k*p
res = simplify(B / (D * p), 100)
res2 = simplify(1 /(D * p), 100)
% roots = solve(D == 0, p,'MaxDegree',3)
% omega = simplify(roots(1) * roots(2) * roots(3), 1000)
% reals = [simplify(real(roots(1)), 1000), simplify(real(roots(2)), 1000), simplify(real(roots(3)), 1000)]

% D = T*p^3+(3*k+2*T+1)*p^2+(2-3*T)*p-3
% roots = solve(D == 0, p,'MaxDegree',3)
% omega = simplify(roots(1) * roots(2) * roots(3), 1000)
