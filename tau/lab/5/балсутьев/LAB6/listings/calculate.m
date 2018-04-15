A = [0 1; -0.75 -2];
B = [0; 1];
C = [0.75 0];
eig(A)
syms a b c d
T = [a b; c d];
G = [-1 0; 0 -2];
F = [1; 1];
lhs = T*A - G*T
rhs = F*C
solution = solve(lhs == rhs);
aa = simplify(solution.a, 100)
bb = simplify(solution.b, 100)
cc = simplify(solution.c, 100)
dd = simplify(solution.d, 100)
TT = subs(T, [a b c d], [aa bb cc dd])
L = inv(TT)*F