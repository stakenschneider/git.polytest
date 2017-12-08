clear all;
close all; 
clc;
format compact;

%% Finding best k and T parameters

minReal = intmax('int64');
minSumm = intmax('int64');

resultK = 0;
resultT = 0;

% Лучше изменить интервалы k и T для ускорения работы
for k = 0.01:0.01:1
    for T = 0.1:0.1:100
        results = roots([1 5*(k*T+5) 5*k*(5*T+1) 25*k]);
        
        % Мнимые части корней должны быть нулевыми
        if imag(results(1))~=0 || imag(results(2))~=0 || imag(results(3))~=0
            continue
        end
        
        % Хотя бы одна реальная часть должна быть минимальной по модулю
        if minReal < abs(real(results(1))) && minReal < abs(real(results(2))) && minReal < abs(real(results(3)))
            continue
        end
        
        minReal = min([abs(real(results(1))) abs(real(results(2))) abs(real(results(3)))]);
        
        numerator = [5*k*T (5*T+1) 5];
        denominator = [1 5*(k*T+5) 5*k*(5*T+1) 25*k];
        Wz = tf(numerator, denominator, 'OutputName', 'torque', 'Variable','p');
        
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
        
        resultK = k;
        resultT = T;
    end
end

%% Result values

k = resultK
T = resultT

%% Roots

results = roots([1 5*(k*T+5) 5*k*(5*T+1) 25*k]);

% Мнимые части корней (должны быть 0)
im = [imag(results(1)) imag(results(2)) imag(results(3))]

% Реальные части корней (среди них должен быть хотя бы один минимальный по модулю)
re = [double(real(results(1))) double(real(results(2))) double(real(results(3)))]

%% Frequency response

syms f res(f)
assume(f, 'real')
assumeAlso(f >= 0)

assume(res(f), 'real')
assumeAlso(res(f) >= 0)

zn = 25*(5*k-(k*T+5)*f^2)^2+(5*k*(5*T+1)*f-f^3)^2;
re = (5*k*((5*k*T^2+20*T-1)*f^4+(125*k*T+5*k-125)*f^2+125*k)) / zn;
im = - (5*k*(T*f^5+(125*T+20)*f^3)) / zn;
res(f) = simplify(sqrt(re^2 + im^2));

disp(sprintf('Bandwidth [0 - %0.10f]', double(abs(solve(res(f)==1/sqrt(2), f)))))

% АЧХ
figure;
hold on
    ezplot(res, (0:0.0001:1000)')
    axis([0 1000 0 1.5])
hold off

numerator = [5*k*T (5*T+1) 5];
denominator = [1 5*(k*T+5) 5*k*(5*T+1) 25*k];
Wz = tf(numerator, denominator, 'OutputName', 'torque', 'Variable', 'p');

% ЛАЧХ, ЛФЧХ
figure;
hold on
    bode(Wz)
hold off

%% Step function with noice.

syms Wmain Wnoice p t

max = 2;
min = 0;
interval = 0.002;
count = (max - min) / interval + 1;

% Wz/p = B(p)/(p*D(p))
Wmain = (5*k*T*p^2+(5*T+1)*p+5)/(p*(p^3+5*(k*T+5)*p^2+5*k*(5*T+1)*p+25*k));
lapMain = vpa(ilaplace(Wmain, p, t), 20);
arrayMain = subs(lapMain, t, min:interval:max);

% 1/(p*D(p))
Wnoice = 1/(p*(p^3+5*(k*T+5)*p^2+5*k*(5*T+1)*p+25*k));
lapNoice = vpa(ilaplace(Wnoice, p, t), 20);
arrayNoice = subs(lapNoice, t, min:interval:max);

disp(sprintf('int (h(t) - 1(t))^2 dt = %0.10f', real(integral(matlabFunction((lapMain-1)^2), 0, inf))))

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