function lab01()
 
    close all;
    clc;
    work_path = 'Y:\4_course\tau\labs\lab01\figs\';
    fprintf('lab01');
    
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
 
    % u = l(t)
    % решение ДУ на промежутке t > 0 
    fprintf('решение ДУ на промежутке t > 0');
    dsolve('D2x - x - 1 == 0 ', 't')
    
    % решение ДУ на промежутке t < 0
    fprintf('решение ДУ на промежутке t < 0');
    dsolve('D2x - x == 0 ', 't')
  
    % u = 0
    fprintf('решение ДУ на промежутке t > 0');
    dsolve('D2x - x == 0 ', 't')
end
