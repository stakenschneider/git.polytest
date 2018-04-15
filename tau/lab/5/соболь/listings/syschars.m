clear all;
close all;
clc;
format compact;
% warning('off', 'all')

count = 1000;

coeff_range = zeros(1, count);
for i=1:1:count
    coeff_range(i) = i;
end

static_coeff=1;

Ks_by_a=zeros(3, count);
Ks_by_b=zeros(3, count);
Ks_by_ab=zeros(3, count);

Q=[1 0; 0 static_coeff];
for i=1:1:count
    Q(1, 1)=coeff_range(i);
    Ks_by_a(:, i)=coeffs(Q);
end

Q=[static_coeff 0; 0 1];
for i=1:1:count
    Q(2, 2)=coeff_range(i);
    Ks_by_b(:, i)=coeffs(Q);
end

Q=[1 0; 0 1];
for i=1:1:count
    Q(1, 1)=coeff_range(i);
    Q(2, 2)=coeff_range(i);
    Ks_by_ab(:, i)=coeffs(Q);
end

figure;
hold on;
    subplot(3,3,1); plot(coeff_range, Ks_by_a(1, :))
    ylabel('k0');
    subplot(3,3,4); plot(coeff_range, Ks_by_a(2, :))
    ylabel('k1');
    subplot(3,3,7); plot(coeff_range, Ks_by_a(3, :))
    ylabel('g');
    xlabel('a');
    
    subplot(3,3,2); plot(coeff_range, Ks_by_b(1, :))
    axis([0 count 0.9 1.1]);
    subplot(3,3,5); plot(coeff_range, Ks_by_b(2, :))
    subplot(3,3,8); plot(coeff_range, Ks_by_b(3, :))
    axis([0 count 0 0.1]);
    xlabel('b');
    
    subplot(3,3,3); plot(coeff_range, Ks_by_ab(1, :))
    subplot(3,3,6); plot(coeff_range, Ks_by_ab(2, :))
    subplot(3,3,9); plot(coeff_range, Ks_by_ab(3, :))
    xlabel('a & b');
hold off;

for i=1:1:count
    Ks_by_a(:, i)=quality(Ks_by_a(1, i), Ks_by_a(2, i));
end

for i=1:1:count
    Ks_by_b(:, i)=quality(Ks_by_b(1, i), Ks_by_b(2, i));
end

for i=1:1:count
    Ks_by_ab(:, i)=quality(Ks_by_ab(1, i), Ks_by_ab(2, i));
end

figure;
hold on;
    subplot(3,3,1); plot(coeff_range, Ks_by_a(1, :))
    ylabel('Max abs imagine');
    subplot(3,3,4); plot(coeff_range, Ks_by_a(2, :))
    ylabel('Omega');
    subplot(3,3,7); plot(coeff_range, Ks_by_a(3, :))
    ylabel('Min abs real');
    xlabel('a');
    
    subplot(3,3,2); plot(coeff_range, Ks_by_b(1, :))
    subplot(3,3,5); plot(coeff_range, Ks_by_b(2, :))
    axis([0 count 0.9 1.1]);
    subplot(3,3,8); plot(coeff_range, Ks_by_b(3, :))
    xlabel('b');
    
    subplot(3,3,3); plot(coeff_range, Ks_by_ab(1, :))
    subplot(3,3,6); plot(coeff_range, Ks_by_ab(2, :))
    subplot(3,3,9); plot(coeff_range, Ks_by_ab(3, :))
    xlabel('a & b');
hold off;

%% kek
clc;
close all;

Q=[1000 0; 0 1];

val = coeffs(Q);

k0 = val(1)
k1 = val(2)
g = val(3)

numerator = [1*g];
denominator = [1 (k1) (1+k0)];
Wp = tf(numerator, denominator, 'OutputName', 'torque', 'Variable','p')

figure;
hold on;
    bode(Wp);
    title('Диаграмма Боде max(a & b)');
    xlabel('Частота');
    grid on;
hold off;

figure;
hold on;
    step(Wp);
    title('Переходная характеристика max(a & b)');
    xlabel('Время');
    ylabel('Амплитуда');
    grid on;
hold off;    

figure;
hold on;
    impulse(Wp);
    title('Весовая характеристика max(a & b)');
    xlabel('Время');
    ylabel('Амплитуда');
    grid on;
hold off;
    
   