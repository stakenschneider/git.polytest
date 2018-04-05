clc;
close all;
A = [0 1; -5 -2]
B = [0; 1]
C = [5 5]
D = 0;
sys = ss(A, B, C, D)

figure
[y, t, X] = impulse(sys);
subplot(3, 1, 1)
plot(t, y)
grid on
subplot(3, 1, 2)
plot(t, X(:, 1))
grid on
subplot(3, 1, 3)
plot(t, X(: ,2))
grid on

figure
[y, t, X] = step(sys);
subplot(3, 1, 1)
plot(t, y)
grid on
subplot(3, 1, 2)
plot(t, X(:, 1))
grid on
subplot(3, 1, 3)
plot(t, X(: ,2))
grid on