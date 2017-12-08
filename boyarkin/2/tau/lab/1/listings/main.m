clear all;
close all; 
clc;
format compact;

path = '../images/';

% x'' + a1 * x' + a0 * x = b1 * u' + b0 * u 
% x(0) = 0, x'(0) = 0

% u = 1(t) => u(t < 0) = 0, u(t = 0) = inf, u(t > 0) = 1, u'(t = 0) = inf, u'(t != 0) = 0

% t > 0 => x'' + a1 * x' + a0 * x = b0
% t = 0 => x(0) = 0, x'(0) = 0, x''(0) = inf
% t < 0 => x'' + a1 * x' + a0 * x = 0

a0 = 0;
a1 = 25;
b0 = 25;
b1 = 5;

syms x(t) dx(t)
% t <= 0
tNegative = dsolve(diff(x, t, 2) + a1 * diff(x, t) + a0 * x == 0)
% t > 0
tPositive = dsolve(diff(x, t, 2) + a1 * diff(x, t) + a0 * x == b0)

% Wp = (b1 p + b0) / (p^2 + a1 p + a0)
numerator = [b1 b0];
denominator = [1 a1 a0];
Wp = tf(numerator, denominator, 'OutputName', 'torque', 'Variable','p')

figure;
hold on;
    nyquist(Wp)
    axis([-2 4 -3 3])
    
    title('Диаграмма Найквеста');
    xlabel('Действительная часть');
    ylabel('Мнимая часть');
    grid on;
    print('-dpng', strcat(path, 'nyquist'));
hold off;

figure;
hold on;
    bode(Wp);
    
    title('Диаграмма Боде');
    xlabel('Частота');
    grid on;
    print('-dpng', strcat(path, 'bode'));
hold off;

figure;
hold on;
    step(Wp);
    
    title('Переходная характеристика');
    xlabel('Время');
    ylabel('Амплитуда');
    grid on;
    print('-dpng', strcat(path, 'step'));
hold off;

figure;
hold on;
    impulse(Wp);
    
    title('Весовая характеристика');
    xlabel('Время');
    ylabel('Амплитуда');
    grid on;
    print('-dpng', strcat(path, 'impulse'));
hold off;


figure;
hold on;
    for j = -10:10
        % t <= 0
        for i = -10:10
            [t, y] = ode45(@diffSysNegative, [-5 0], [j i]);
            plot(y(:,1), y(:,2));
        end

        % t > 0
         for i = -10:10
             [t, y] = ode45(@diffSysPositive, [0 5], [j i]);
             plot(y(:,1), y(:,2));
         end
    end

    title('Фазовый портрет системы');
    xlabel('X');
    ylabel('dX/dt');
    grid on;
    print('-dpng', strcat(path, 'ode'));
hold off;
