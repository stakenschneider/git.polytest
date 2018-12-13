% 1. Сформируйте желаемую входную и выходную последовательности 
P = {1  0 0 1 1  0 1  0 0 0 0 1 1  0 0 1};
T = {1 -1 0 1 0 -1 1 -1 0 0 0 1 0 -1 0 1};
T1 = cell2mat(T);
 
%% PART 1
% 2. Создайте НС с задержками с 5 скрытыми и задержками [0 1]: 
net = newfftd( P, T, [0 1], 5 ); 
 
% 3. Вычислите выходной сигнала в ответ на входную последовательность 
Y1 = net( P );
Y1 = cell2mat( Y1 );
% Сравните результат с желаемым. 
E1 = T1 - Y1;
SSE(1) = sumsqr( E1 );
 
% 4. Обучите НС на 50 эпохах 
net.trainParam.epochs = 50; 
net = train( net, P, T ); 
 
% 5. Снова вычислите выходной сигнала в ответ на входную последовательность 
Y2 = net( P );
Y2 = cell2mat( Y2 );
% Сравните результат с желаемым. 
E2 = T1 - Y2;
SSE(2) = sumsqr( E2 );

%% PART 2
% 6. Попробуйте обучить НС без задержек и с задержками [0 1 2], [0 2]. 
% Как изменяется результат? Почему? 
% НС без задержек 
net = newfftd( P, T, 5 ); 
net.trainParam.epochs = 50; 
net = train( net, P, T ); 
Y3 = net( P );
Y3 = cell2mat( Y3 );
% Сравните результат с желаемым. 
E3 = T1 - Y3;
SSE(3) = sumsqr( E3 );
 
% НС с задержками [0 1 2]
net = newfftd( P, T, [0 1 2], 5 ); 
net.trainParam.epochs = 50; 
net = train( net, P, T ); 
Y4 = net( P );
Y4 = cell2mat( Y4 );
% Сравните результат с желаемым. 
E4 = T1 - Y4;
SSE(4) = sumsqr( E4 );
 
% НС с задержками [0 2]
net = newfftd( P, T, [0 2], 5 ); 
net.trainParam.epochs = 50; 
net = train( net, P, T ); 
Y5 = net( P );
Y5 = cell2mat( Y5 );
% Сравните результат с желаемым. 
E5 = T1 - Y5;
SSE(5) = sumsqr( E5 );
% 7. Проделайте то же самое для распределенной НС с задержками 
net = newdtdnn( P, T, 5, {[0 1] [0 1]} ); 
net.trainParam.epochs = 50; 
net = train( net, P, T ); 
Y6 = net( P );
Y6 = cell2mat( Y6 );
% Сравните результат с желаемым. 
E6 = T1 - Y6;
SSE(6) = sumsqr( E6 );
nntraintool( 'close' );
SSE

%% 2 
% Считайте данные из файла laser_dataset.mat и постройте их на графике.
y = laser_dataset;
y = y(1:600); 
plot(cell2mat(y)); 
grid on;

% Создайте фокусированную НС с задержками с 10 скрытыми нейронами и задержками от 1 до 8 с шагом 1.
ftdnn_net = newfftd(y,y,[1:8],10);
% Задайте 1000 эпох обучения
ftdnn_net.trainParam.epochs = 1000;
% Отмените деление выборки на части
ftdnn_net.divideFcn = '';
% Сформируйте обучающий набор (p,t) и начальные условия Pi
p = y( 9:end );
t = y( 9:end );
Pi=y( 1:8 );
% Обучите НС
ftdnn_net = train( ftdnn_net, p, t, Pi );
%  Определите реакцию НС на обучающие примеры p с начальными условиями Pi
yp = sim( ftdnn_net, p, Pi );
% Вычислите среднюю ошибку
e = gsubtract( yp, t );
rmse = sqrt( mse( e ) )
% Визуализируйте результаты
figure;
subplot(3,1,1);
plot(cell2mat(p)); 
grid on; 
title('Данные'); 
subplot(3,1,2); 
plot(cell2mat(yp)); 
grid on;
title('Выход НС'); 
subplot(3,1,3); 
plot(cell2mat(e)); 
grid on; 
title('Ошибка');

% Сделайте все то же самое для линейной НС
lin_net = newlin(y, 1, [1:8]); 
lin_net.trainFcn='trainlm'; 
[lin_net,tr] = train(lin_net,p,t,Pi); 
lin_yp = sim(lin_net, p,Pi);
lin_e = gsubtract(lin_yp,t); 
lin_rmse = sqrt(mse(lin_e))
figure;
subplot(3,1,1); 
plot(cell2mat(p)); 
grid on; 
title('Данные');
subplot(3,1,2); 
plot(cell2mat(lin_yp)); 
grid on; 
title('Выход НС'); 
subplot(3,1,3); 
plot(cell2mat(lin_e)); 
grid on; 
title('Ошибка');

%% 3
% Сформируйте сигналы y1 и y2 разной частоты
time = 0:99;
y1 = sin(2*pi*time/10); 
y2 = sin(2*pi*time/5);

% Сформируйте последовательность y как две пары последовательностей y1 и y2.
y = [y1 y2 y1 y2];

% Сформируйте желаемый выходной сигнал как 1 в ответ на y1 и -1 в ответ на y2
t1 = ones(1,100); 
t2 = -ones(1,100); 
t = [t1 t2 t1 t2];

% Задайте задержки для 1 и 2 слоя.
d1 = 0:4; 
d2 = 0:3;

% Cформируйте обучающую выборку.
p = con2seq(y); 
t = con2seq(t);

% Создайте распределенную НС с задержками, функцией обучения trainbr, 100 эпохами обучения и без деления выборки.
dtdnn_net = newdtdnn(p,t, 5, {d1,d2}); 
dtdnn_net.trainFcn = 'trainbr';
dtdnn_net.divideFcn = ''; 
dtdnn_net.trainParam.epochs = 100;

% Обучите НС. Можете нажать Stop при малых значениях ошибки, чтобы не ждать.
dtdnn_net = train(dtdnn_net,p,t);

%  Вычислите реакцию НС в ответ на входной сигнал p.
yp = sim(dtdnn_net,p);

% Визуализируйте результат с помощью функции plotresponse
plotresponse(t,yp);

% Попробуйте решить эту же задачу с помощью фокусированной НС с
% задержками...
netff = newfftd( p, t, [0 1], 5 );
yp = sim( netff, p );
figure(3)
plotresponse( t, yp );

%... и с помощью линейного слоя. 
net = newlind( p, t );
yp = sim( net, p );
figure(2)
plotresponse( t, yp );
