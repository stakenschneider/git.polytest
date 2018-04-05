% function lab02()
%%

close all;
clc;
work_path = 'Y:\4_course\tau\labs\lab02\figs\';
fprintf('lab02 is going to be done\n');

% числитель и знаменатель передаточной функции 
num = [1];
den = [1 0 1];

left = -10;
right = 100;
w = left:0.01:right;
fprintf('Передаточная функция');
Ws = tf(num, den)
fprintf('Корни характеристического полинома(полюса):');
pole(Ws)


% представление системы в пространстве состояний   
sys = ss(Ws)
CNsys = canon(sys, 'companion')

% figure
% [y, t, X] = impulse(sys);
% subplot(3, 1, 1)
% plot(t, y)
% grid on
% subplot(3, 1, 2)
% plot(t, X(:, 1))
% grid on
% subplot(3, 1, 3)
% plot(t, X(: ,2))
% grid on
% 
% figure
% [y, t, X] = step(sys);
% subplot(3, 1, 1)
% plot(t, y)
% grid on
% subplot(3, 1, 2)
% plot(t, X(:, 1))
% grid on
% subplot(3, 1, 3)
% plot(t, X(: ,2))
% grid on


% считаем собственныые значения (проверяем
% правильность перехода к представлению в п. с.)
% если собственные числа совпадают с полюсами передаточной функции => OK
% eig(sys)


    
    
    %%
    % Ступенчатое воздействие
    stepFig = figure;
    title('h(t)');
    grid on;
    step(Ws,w);
    saveas(stepFig, [work_path 'step'], 'png');
 
    % Импульсное воздействие
    impFig = figure;
    title('w(t)');
    grid on;
    impulse(Ws,w);
    saveas(impFig, [work_path 'imp'], 'png');
    
    % Диаграмма Боде
    bodeFig = figure;
    grid on;
    bodeplot(Ws,0.01:0.01:200)
    saveas(bodeFig, [work_path 'bode'], 'png');
    
    % Диаграмма Нейквиста
    nyqFig = figure;
    grid on;
    nyquistplot(Ws,0.01:0.01:180)
    saveas(nyqFig, [work_path 'nyq'], 'png');
    
    
    
%     % u = l(t)
%     % решение ДУ на промежутке t > 0 
%     fprintf('решение ДУ на промежутке t > 0');
%     dsolve('D2x - x - 1 == 0 ', 't')
%     
%     % решение ДУ на промежутке t < 0
%     fprintf('решение ДУ на промежутке t < 0');
%     dsolve('D2x - x == 0 ', 't')
%   
%     % u = 0
%     fprintf('решение ДУ на промежутке t > 0');
%     dsolve('D2x - x == 0 ', 't')

% end
