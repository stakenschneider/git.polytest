clear all;
close all; 
clc;
format compact;

%% Finding best k and T parameters

minReal = intmax('int64');
minSumm = intmax('int64');


syms p
syms w real
syms k real
syms T real

resultK = 0;
resultT = 0;

koeffs = [1 5*(k*T+5) 5*k*(5*T+1) 25*k];
W_z_numerator = [5*k*T (5*T+1) 5];
W_z_denominator =  [1 5*(k*T+5) 5*k*(5*T+1) 25*k];


W_p = 0.75 / (p^2 + 2*p + 0.75)
W_pd = (k*p)/(T*p + 1)

W_3 = W_pd * W_p
assumeAlso(w >= 0)
assumeAlso(T > 0)


B = 0.75*k*p

D = T*p^3+(2*T+1)*p^2+(0.75*T+0.75*k+2)*p+0.75

% Лучше изменить интервалы k и T для ускорения работы
for kk = 0.1:0.1:10
    for TT = 0.1:1:1000
        kfs = eval(subs(koeffs, [k T], [kk, TT]));
        results = roots(kfs);
        
        % Мнимые части корней должны быть нулевыми
        if imag(results(1))~=0 || imag(results(2))~=0 || imag(results(3))~=0
            continue
        end
        
        % Хотя бы одна реальная часть должна быть минимальной по модулю
        if minReal < abs(real(results(1))) && minReal < abs(real(results(2))) && minReal < abs(real(results(3)))
            continue
        end
        
        minReal = min([abs(real(results(1))) abs(real(results(2))) abs(real(results(3)))]);
        
        numerator = eval(subs(W_z_numerator, [k T], [kk TT]));
        denominator = eval(subs(W_z_denominator, [k T], [kk TT]));
        Wz = tf(numerator, denominator,  'OutputName', 'torque', 'Variable','p');
        
        % Сумма отклонений h(t) от сигнала 1(t)
        % Сделано для ускорения расчетов вместо интеграла (интеграл считается в конце).
        [y, t] = step(Wz, (0:0.01:1000)');
        summ = sum(abs(y - 1));
        
        % Отклонение должно быть минимальным
        if minSumm < summ
            continue
        end
        
        minSumm = double(summ);
        
        % Все условия выполнены, записываем результат
        
        resultK = kk;
        resultT = TT;
    end
end

%% Result values

resultK
resultT

%% Roots
 kfs = subs(koeffs, k, resultK);
kfs = subs(kfs, T, resultT);
results = roots(kfs);

% Мнимые части корней (должны быть 0)
im = [imag(results(1)) imag(results(2)) imag(results(3))];

% Реальные части корней (среди них должен быть хотя бы один минимальный по модулю)
re = [double(real(results(1))) double(real(results(2))) double(real(results(3)))];

%% Frequency response


W_3_jw = simplify(subs(W_3, p, 1i*w), 100);
W_3_jw = subs(W_3_jw, [k T], [resultK resultT]);
res = simplify(abs(W_3_jw))

solution = solve( res == 1/sqrt(2), w)

volution_value = eval(abs(solution))
fprintf('Bandwidth [0 - %0.10f]\n', volution_value)

plot_values = eval(subs(res, w, 0:0.1:1000));

% АЧХ
figure;
hold on
    plot(0:0.1:1000, plot_values)
    axis([0 1000 0 1.5])
hold off

numerator = eval(subs(W_z_numerator, [k, T], [resultK resultT]));
denominator = eval(subs(W_z_denominator, [k T], [resultK resultT]));
Wz = tf(numerator, denominator, 'OutputName', 'torque', 'Variable', 'p');

% ЛАЧХ, ЛФЧХ
figure;
hold on
    bode(Wz)
hold off

%% Step function with noice.

syms Wmain Wnoice t

max = 2;
min = 0;
interval = 0.002;
count = (max - min) / interval + 1;

% Wz/p = B(p)/(p*D(p))
Wmain = subs(B / (p*D), [k T], [resultK, resultT]);
lapMain = vpa(ilaplace(Wmain, p, t), 20);
arrayMain = subs(lapMain, t, min:interval:max);

% 1/(p*D(p))
Wnoice = subs(1/(p*D), [k T], [resultK, resultT]);
lapNoice = vpa(ilaplace(Wnoice, p, t), 20);
arrayNoice = subs(lapNoice, t, min:interval:max);

print_value = real(integral(matlabFunction((lapMain-1)^2), 0, inf));

fprintf('int (h(t) - 1(t))^2 dt = %0.10f\n', print_value )

noice = wgn(count, 1, 0);

% Шум
figure;
hold on
    plot(min:interval:max, noice);
    axis([min max -5 5])
hold off

% Без шума
figure;
hold on
    plot(min:interval:max, arrayMain);
    axis([min max 0 1.5])
hold off

% С шумом
figure;
hold on
    plot(min:interval:max, arrayMain + arrayNoice * diag(noice));
    axis([min max 0 1.5])
hold off

% Более гладкая переходная функция (если нужно)

% figure;
% hold on
%     step(Wz, (0:0.0001:10)')
% hold off