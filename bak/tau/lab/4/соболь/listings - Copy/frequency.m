syms p
syms w real
clc;
syms k_p real
syms k_i real

B = 0.75*(k_p*p + k_i)
C = p*(p^2+2*p+0.75)
D = expand(B + C)

W_3 = simplify(B / D, 100)

assumeAlso(w >= 0)
assumeAlso(k_p >= 0)
assumeAlso(k_i >= 0)

W_3_jw = simplify(subs(W_3, p, 1i*w), 100)

re = simplify(real(W_3_jw), 10)
im = simplify(imag(W_3_jw), 10)

A = simplify(sqrt(re^2 + im^2), 100)
L = simplify(20*log10(A), 100)

A_0 = simplify(subs(A, w, 0), 100)

res = simplify(solve(D == 0, p), 100)
simplify(res(1) * res(2) * res(3), 100)

Wmain = simplify(B /(p*D), 100)
Wnoice = simplify(1 /(p*D), 100)