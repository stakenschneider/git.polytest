syms a b c d
clc;
assumeAlso(a > 0)
assumeAlso(b > 0)
assumeAlso(c > 0)
assumeAlso(d > 0)
S = [a b; c d];
R = eye(2);
N = zeros(2);

Q = eye(2);

% A = [0 1; -0.75 -2]
% B = [0; 1]
% C=[0.75 0];
% r=1;

A = [0 1; -1 0]
B = [0; 1]
C=[0 1];
r=1;


[X, L, G] = care(A, B, Q)


size(S)
size(B)
% size((S*B + [0; 0]) * inv(R))

lhs = A'*S + S*A - (S*B)*(B'*S) + Q
% lhs = A'*S + S*A - (S*B + [0; 0])*inv(R)*(B'*S + [0 0]) + Q
result = solve(lhs == zeros(2));

aa = simplify(result.a, 100)
bb = simplify(result.b, 100)
cc = simplify(result.c, 100)
dd = simplify(result.d, 100)

SS = [aa bb; cc dd]
K = B'*SS
M = simplify(1/K(1),100)
g = simplify((M + 1)/M, 100)

syms p k_1 k_0
D = p^2+(k_1)*p+(1+k_0)
simplify(solve(D ==0, p), 100)