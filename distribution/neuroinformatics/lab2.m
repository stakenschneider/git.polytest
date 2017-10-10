%% 1

P = [1.0 -1.2]; % Обучающая выборка ? входной сигнал
T = [0.5 1.0]; % Обучающая выборка ? выходной сигнал
net = newlind(P,T); % Создание и обучение НС
Y = sim(net, P) % Выход НС
E = T - Y % Ошибка
SSE = sumsqr(E) % Суммарная ошибка
net.IW{1,1} % Весовой коэффициент
net.b{1} % Смещение
w_range = -1:0.1:1; % диапазон весов;
b_range = -1:0.1:1; % диапазон смещений;
ES = errsurf(P, T, w_range, b_range, 'purelin'); % Вычисление поверхности
plotes(w_range,b_range,ES); % Построение поверхности
hold on
plotep(net.IW{1,1}, net.b{1}, SSE); % Построение точки [W b] после обучения сети на поверхности ошибок
hold off % ? знаком 'x' отмечены оптимальные значения веса и % смещения.

%% 2.1

P = [1 -1.2];
T = [0.5 1];
maxlr = 0.40*maxlinlr(P,'bias');

net = newlin([-2 2], 1, [0], maxlr);
gensim(net)

w_range = -1:0.2:1; b_range = -1:0.2:1;
ES = errsurf(P,T,w_range,b_range,'purelin');

surfc(w_range, b_range, ES);

X = zeros(1, 50); Y = zeros(1, 50);
net.IW{1,1} = 1; net.b{1} = -1 % ? начальные значения;
X(1) = net.IW{1}; Y(1) = net.b{1};
net.trainParam.goal = 0.001;
net.trainParam.epochs = 1;

for I=2:50,
[net, tr] = train(net, P, T);
X(I) = net.IW{1, 1};
Y(I) = net.b{1}; 
end


clc; % очистка экрана
contour(w_range, b_range, ES, 20) % 20 линий
hold on
plot(X,Y,'-*') % ? построение траектории
hold off

net.IW{1, 1} = 1; net.b{1} = -1;
net.trainParam.epochs = 50;
net.trainParam.goal = 0.001; % ? первое значение;
[net, tr] = train(net, P, T);
net.IW{1, 1}, net.b{1} %
net.trainParam.goal = 0.00001; % ? второе значение
[net, tr] = train(net, P, T);
net.IW{1, 1}, net.b{1} %

%% 3

clear all;
P = [ 1.0 2.0 3.0; ...
4.0 5.0 6.0];
T = [0.5 1.0 -1.0];
maxlr = 0.40*maxlinlr(P,'bias');
net = newlin([1 3; 4 6], 1, [0], maxlr);
[net, tr] = train(net, P, T);
net.IW{1, 1}, net.b{1}
Y = sim(net, P)

Error= T - Y             
SSE = sumsqr(E)      
net.IW{1,1}          
net.b{1}            
w_range = -1:0.1:1; 
b_range = -1:0.1:1; 
ES = errsurf(P, T, w_range, b_range, 'purelin');
plotes(w_range,b_range,ES);      
hold onplotep(net.IW{1,1}, net.b{1}, SSE);
hold off

%% 4.1 формирование обучающей и тестовой выборки
clear all;
y = @ (x) cos(x) + 0.3*sin(20*x) + 0.03*sin(15*x) ;

% обучающая выборка
P = 0.2:0.02:1;
T = y(P);

% тестовая выборка

P_TEST = P - 0.3;
T_TEST = y(P_TEST);

% 4.2 создание и обучение нс

maxlr = 0.40*maxlinlr(P,'bias');
net = newlin([0 1], 1, [0], maxlr);
net.trainParam.epochs = 1;
% вывод графиков зависимостей, реализуемых нс через каждые step эпох.
step = 1;
for i = 1:20
    [net, tr] = train(net, P, T);
    w = net.IW{1, 1};
    b = net.b{1};
    if (rem(i, step)) == 0
        figure;
        hold on;
        ezplot(y);
        axis([0 1 0 3]);
        plot([0, 1], [b, w + b]);
    end
end
axis([0 1 0 3]);

% оценка ошибки аппроксимации
'оценка ошибки при задании сети функцией newlin'
% абсолютная ошибка
%  на обучающей выборке
abs_err = mean(abs(T - sim(net, P)))
%  на тестовой выборке
abs_err_test = mean(abs(T_TEST - sim(net, P_TEST)))

% относительная ошибка
%  на обучающей выборке
rel_err = mean(abs(T - sim(net, P))./T)
%  на тестовой выборке
rel_err_test = mean(abs((T_TEST - sim(net, P_TEST))./T_TEST))

% 4.3 создание и обучение нс с помощью функции newlind
% вывод графиков зависимостей, реализуемых нс через каждые step эпох.
net = newlind(P, T);
w = net.IW{1, 1};
b = net.b{1};
figure;
hold on;
ezplot(y);
axis([0 1 0 3]);
plot([0, 1], [b, w + b]);
axis([0 1 0 3]);

% оценка ошибки аппроксимации
'оценка ошибки при задании сети функцией newlind'
% абсолютная ошибка
%  на обучающей выборке
abs_err = mean(abs(T - sim(net, P)))
%  на тестовой выборке
abs_err_test = mean(abs(T_TEST - sim(net, P_TEST)))

% относительная ошибка
%  на обучающей выборке
rel_err = mean(abs(T - sim(net, P))./T)
%  на тестовой выборке
rel_err_test = mean(abs((T_TEST - sim(net, P_TEST))./T_TEST))

%% 5 классификация с помощью линейной сети
% реакция сети <  0.5 => принадлежность классу '0'
%              >= 0.5 => принадлежность классу '1'
clear all;
P = [2 1 -2 -1; 2 -2 2 1];
T = [0 1 0 1];
net = newlin([-2 2; -2 2], 1);
net.trainParam.goal = 0.1;
[net, tr] = train(net, P, T);
net.IW{1, 1}, net.b{1}
A = sim(net, P)

%% 6.0
%  использование 4-слойного персептрона из работы 1 для
%  формирования обучающей выборки
net = network(2, 4, [1; 1; 1; 1], [1 1; 0 0; 0 0; 0 0], ...
              [0 0 0 0; 1 0 0 0; 0 1 0 0; 0 1 1 0], [0 0 0 1]);

for i = 1:2
    net.inputs{i}.size = 1;
end
layer_sizes = [11; 3; 1; 1];
for i = 1:4
    net.layers{i}.size = layer_sizes(i);
end

net.b{1} = [-0.1; 0.9; 0.6; -0.4;  -0.6; 0.8; 0.4; 2.4;  -0.5; 0.5; 0.4];
net.b{2} = [-3.5; -3.5; -2.5];
net.b{3} = -0.5;
net.b{4} = -0.5;

net.iw{1, 1} = [1; -1; 0; 0; 0; 0; 2; -2; 1; -1; 0];
net.iw{1, 2} = [0; 0; -1; 1; 1; -1; -1; -1; 1; 1; -1];
net.lw{2, 1} = [[ 1 1 1 1 0 0 0 0 0 0 0  ]; 
                [ 0 0 0 0 1 1 1 1 0 0 0  ]; 
                [ 0 0 0 0 0 0 0 0 1 1 1  ]; ];
net.lw{3, 2} = [ 0  1  1 ];
net.lw{4, 2} = [ 1  0  0 ];
net.lw{4, 3} = 1;

net.layers{1:4}.transferFcn = 'hardlim';

P = rand(2, 100);
P_TEST = rand(2, 100);
T = sim(net, P);
T_TEST = sim(net, P_TEST);

% 6.0.2 проверка

figure;

hold on;
for i = 1:1:25
    for j = 1:1:25
        if sim(net,[i/25;j/25]) == 1
            plot( i/25, j/25 ,'x', 'Color', [.1 .9 .1]);
        else
            plot( i/25, j/25 ,'x', 'Color', [.9 .9 .9]);
        end
    end
end


axis([0 1 0 1]);

%
% 6.1 обучение линейной нс классификации сложных образов
net = newlind(P, T);

% оценка ошибок классификации
err_1 = 0; % число ошибок 1 рода (A(i) == 0, T(i) ~= 0)
err_2 = 0; % число ошибок 2 рода (A(i) == 1, T(i) ~= 1)
A = sim(net, P);
for i = 1:length(A)
    if (A(i) < 0.5) && (T(i) ~= 0)
        err_1 = err_1 + 1;
    end
    if (A(i) >= 0.5) && (T(i) ~= 1)
        err_2 = err_2 + 1;
    end
end

err_1_test = 0; % число ошибок 1 рода (A(i) == 0, T(i) ~= 0)
err_2_test = 0; % число ошибок 2 рода (A(i) == 1, T(i) ~= 1)
A_TEST = sim(net, P_TEST);
for i = 1:length(A)
    if (A(i) < 0.5) && (T_TEST(i) ~= 0)
        err_1_test = err_1_test + 1;
    end
    if (A(i) >= 0.5) && (T_TEST(i) ~= 1)
        err_2_test = err_2_test + 1;
    end
end

err_1
err_2
err_1_test
err_2_test

% 6.1.2 проверка

figure;
hold on;

for i = 1:1:25
    for j = 1:1:25
        if sim(net,[i/25;j/25]) > 0.5
            plot( i/25, j/25 ,'x', 'Color', [.1 .9 .1]);
        else
            plot( i/25, j/25 ,'x', 'Color', [.9 .9 .9]);
        end
    end
end

axis([0 1 0 1]);
%% 7 Неопределенность при обучении
clear all;

P = [0.5];
T = 0.3;


net = newlin([0 1], 1, [0], 0.4*maxlinlr(P,'bias'));

net.trainParam.epochs = 20;
net = train(net, P, T);

figure;
hold on;
for i = 1:10
    P1{i} = 0:0.1:1;

    plot(P1{i}, sim(net, P1{i}), '.-');
end
axis([0 1 0 1]);


%% 8 Выбор скорости обучения
% формирование обучающей и тестовой выборки
clear all;
y = @ (x) sin(x) + 0.5*sin(4*x) + 0.25*sin(16*x) + 0.125*sin(64*x);

% обучающая выборка
P = 0.2:0.02:1;
T = y(P);

maxlr = maxlinlr(P,'bias');
net = newlin([0 1], 1, [0], maxlr);
net.trainParam.epochs = 100;
[net, tr] = train(net, P, T);

% определение максимальной скорости обучения
% при скорости, большей 2*maxlinlr, происходит рост ошибки
% после каждого цикла обучения
maxlr = 2.0002*maxlinlr(P,'bias')
net = newlin([0 1], 1, [0], maxlr);
net.trainParam.epochs = 100;
[net, tr] = train(net, P, T);

% скорость обучения:
% а) больше maxlr

maxlr = 2.1*maxlinlr(P,'bias')
net = newlin([0 1], 1, [0], maxlr);

w_range = -1:0.2:1; b_range = -1:0.2:1;
ES = errsurf(P,T,w_range,b_range,'purelin');
surfc(w_range, b_range, ES);

X = zeros(1, 50); Y = zeros(1, 50);
net.IW{1,1} = 0; net.b{1} = 0.5; % ? начальные значения;
X(1) = net.IW{1}; Y(1) = net.b{1};
net.trainParam.goal = 0.001;
net.trainParam.epochs = 1;

for I=2:50,
[net, tr] = train(net, P, T);
X(I) = net.IW{1, 1};
Y(I) = net.b{1}; 
end

clc; % очистка экрана
contour(w_range, b_range, ES, 20) % 20 линий
hold on
plot(X,Y,'-*') % ? построение траектории
hold off


% б) равна maxlr

maxlr = 2*maxlinlr(P,'bias')
net = newlin([0 1], 1, [0], maxlr);

w_range = -0.25:0.05:0.25; b_range = 0.5:0.05:1;
ES = errsurf(P,T,w_range,b_range,'purelin');
surfc(w_range, b_range, ES);

X = zeros(1, 50); Y = zeros(1, 50);
net.IW{1,1} = 0; net.b{1} = 0.5; % ? начальные значения;
X(1) = net.IW{1}; Y(1) = net.b{1};
net.trainParam.goal = 0.001;
net.trainParam.epochs = 1;

for I=2:50,
[net, tr] = train(net, P, T);
X(I) = net.IW{1, 1};
Y(I) = net.b{1}; 
end

clc; % очистка экрана
contour(w_range, b_range, ES, 20) % 20 линий
hold on
plot(X,Y,'-*') % ? построение траектории
hold off

% в) меньше maxlr
% 1)
maxlr = maxlinlr(P,'bias')
net = newlin([0 1], 1, [0], maxlr);

w_range = -0.25:0.05:0.25; b_range = 0.4:0.05:0.9;
ES = errsurf(P,T,w_range,b_range,'purelin');
surfc(w_range, b_range, ES);

X = zeros(1, 50); Y = zeros(1, 50);
net.IW{1,1} = 0; net.b{1} = 0.5; % ? начальные значения;
X(1) = net.IW{1}; Y(1) = net.b{1};
net.trainParam.goal = 0.001;
net.trainParam.epochs = 1;

for I=2:50,
[net, tr] = train(net, P, T);
X(I) = net.IW{1, 1};
Y(I) = net.b{1}; 
end

clc; % очистка экрана
contour(w_range, b_range, ES, 20) % 20 линий
hold on
plot(X,Y,'-*') % ? построение траектории
hold off

% 2)
maxlr = 0.1*maxlinlr(P,'bias')
net = newlin([0 1], 1, [0], maxlr);

w_range = -1:0.2:1; b_range = 0.4:0.05:0.9;
ES = errsurf(P,T,w_range,b_range,'purelin');
surfc(w_range, b_range, ES);

X = zeros(1, 100); Y = zeros(1, 100);
net.IW{1,1} = 0; net.b{1} = 0.5; % ? начальные значения;
X(1) = net.IW{1}; Y(1) = net.b{1};
net.trainParam.goal = 0.001;
net.trainParam.epochs = 1;

for I=2:100,
[net, tr] = train(net, P, T);
X(I) = net.IW{1, 1};
Y(I) = net.b{1}; 
end

clc; % очистка экрана
contour(w_range, b_range, ES, 20) % 20 линий
hold on
plot(X,Y,'-*') % ? построение траектории
hold off

% 3)
maxlr = 0.01*maxlinlr(P,'bias')
net = newlin([0 1], 1, [0], maxlr);

w_range = -1:0.2:1; b_range = 0.4:0.05:0.9;
ES = errsurf(P,T,w_range,b_range,'purelin');
surfc(w_range, b_range, ES);

X = zeros(1, 200); Y = zeros(1, 200);
net.IW{1,1} = 0; net.b{1} = 0.5; % ? начальные значения;
X(1) = net.IW{1}; Y(1) = net.b{1};
net.trainParam.goal = 0.001;
net.trainParam.epochs = 1;

for I=2:200,
[net, tr] = train(net, P, T);
X(I) = net.IW{1, 1};
Y(I) = net.b{1}; 
end

clc; % очистка экрана
contour(w_range, b_range, ES, 20) % 20 линий
hold on
plot(X,Y,'-*') % ? построение траектории
hold off

% быстрее всего обучение происходит при скорости 1*maxlinlr
