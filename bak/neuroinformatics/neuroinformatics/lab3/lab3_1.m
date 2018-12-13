% 1 Изучение функций инициализации
%% 1
clear all;
n = 4;
m = 2;
net = network(n, 1, [1], [1 1 1 1], [0], [1]);
for i = 1:4
    net.inputs{i}.size = 1;
end
net.layers{1}.size = m;
net.initFcn = 'initlay';
%% 2
net.layers{1}.initFcn = 'initnw';
net = init(net);

net.iw{:}
net.b{:}

%% 3
net.layers{1}.initFcn = 'initwb';

ifcn_wb = {'initzero', 'rands', 'randsmall'};
ifcn_w  = {'midpoint', 'randnc', 'randnr'  };
ifcn_b  = 'initcon';

'результаты инициализации различными функциями:'
'для весов и смещений:'
for i = 1:3
    ifcn_wb{i}
    net.inputWeights{1}.initFcn = ifcn_wb{i};
    net.biases{1}.initFcn = ifcn_wb{i};
    net = init(net);
    
    'матрица весов:'
    net.iw{:}
    'матрица смещений:'
    net.b{:}
end

'для весов:'
for i = 1:3
    ifcn_w{i}
    net.inputWeights{1}.initFcn = ifcn_w{i};
    net = init(net);
    
    'матрица весов:'
    net.iw{:}
end

'для смещений:'
net.biases{1}.initFcn = ifcn_b;
net = init(net);

'матрица смещений:'
ifcn_b
net.b{:}

% 2
%% 1

pfcn = {'fixunknowns', 'mapminmax', 'mapstd', 'processpca', ...
    'removeconstantrows', 'removerows'};
data{1} = [rand(1, 1), nan, rand(1, 1), nan];
pfcn_res{1} = fixunknowns(data{1});
data{2} = 10*rand(1, 4);
pfcn_res{2} = mapminmax(data{2});
data{3} = [1 2 3 4];
pfcn_res{3} = mapstd(data{3});
data{4} = rand(4);
pfcn_res{4} = processpca(data{4});
data{5} = [rand(1, 4); ones(1, 4); rand(1, 4); 2*ones(1, 4)];
pfcn_res{5} = removeconstantrows(data{5});
data{6} = rand(4);
pfcn_res{6} = removerows(data{6}, 2); %% не работает(

for i = 1:6
    pfcn{i}
    % входные данные:
    data{i}
    % результат предобработки:
    pfcn_res{i}
end

% 3
%% 1

E = rand(4, 100)
mae(E)
mse(E)
sae(E)
sse(E)

% 4
%% 1
Q = 100;
trainRatio = 0.7;
valRatio = 0.15;
testRatio = 0.15;
[trainInd,valInd,testInd] = divideblock(Q,trainRatio,valRatio,testRatio)
[trainInd,valInd,testInd] = divideind(Q,1:70,71:85,86:100)
[trainInd,valInd,testInd] = divideint(Q,trainRatio,valRatio,testRatio)
[trainInd,valInd,testInd] = dividerand(Q,trainRatio,valRatio,testRatio)
[trainInd,valInd,testInd] = dividetrain(Q)

trainRatio = 0.34;
valRatio = 0.33;
testRatio = 0.33;
[trainInd,valInd,testInd] = divideblock(Q,trainRatio,valRatio,testRatio)
[trainInd,valInd,testInd] = divideind(Q,1:34,35:67,68:100)
[trainInd,valInd,testInd] = divideint(Q,trainRatio,valRatio,testRatio)
[trainInd,valInd,testInd] = dividerand(Q,trainRatio,valRatio,testRatio)
[trainInd,valInd,testInd] = dividetrain(Q)

% 5
%% 1 формирование выборок
clear all;
y = @ (x) 0.1*sin(2*x) + 0.3*sin(8*x) + 0.5*sin(32*x)+cos(11*x);

% обучающая выборка
P = 0.2:0.01:1;
T = y(P);

% тестовая выборка
P_VAL = P - 0.005;
P_VAL(1) = [];
T_VAL = y(P_VAL);


net = newff(P, T, [20]);

%% 2.1
% обучение НС
net.trainFcn = 'traingd';
net = train(net, P, T);

%% 2.2
net = newff(P, T, [20]);
net.trainFcn = 'traingd';

% задание критериев останова
net.trainParam.goal = 0.03;
net.trainParam.min_grad = 0.01;
net.trainParam.epochs = 2000;
net.trainParam.time = 3000;
net.trainParam.max_fail = 2;

net = train(net, P, T);

%% 3
net = newff(P, T, [20]);
net.trainFcn = 'traingd';
net.trainParam.goal = 0;
net.trainParam.min_grad = 0;
net.trainParam.max_fail = 1;

net = train(net, P, T);

%% 4
clear err avg_err;
lr = [0.01 0.1 1 10];
for i = 1:length(lr)
    % усреднение по различным начальным весам и смещениям
    for j = 1:4
        net = newff(P, T, [15]);
        net.trainFcn = 'traingd';
        net.trainParam.epochs = 10000;
        net.trainParam.goal = 0;
        net.trainParam.min_grad = 0;
        net.trainParam.max_fail = 6;
        net.trainParam.lr = lr(i);
        
        [net, tr] = train(net, P, T);
        err(j) = tr.best_vperf; % ошибка на контрольном множестве
    end
    avg_err(i) = mean(err);
end

figure;
plot(lr, avg_err);

%% 5
clear err avg_err;
num_neurons = [1 5 10 15 20 25 30 35];
for i = 1:length(num_neurons)
    for j = 1:1
        net = newff(P, T, [num_neurons(i)]);
        net.trainFcn = 'traingd';
        net.trainParam.epochs = 10000;
        net.trainParam.goal = 0;
        net.trainParam.min_grad = 0;
        net.trainParam.max_fail = 6;
        net.trainParam.lr = 0.4;
        
        [net, tr] = train(net, P, T);
        err(i) = tr.best_vperf; % ошибка на контрольном множестве
    end
    avg_err(i) = mean(err);
end

figure;
plot(num_neurons, avg_err);

%% 6.1
net = newff(P, T, [20]);
net.trainFcn = 'trains';
net.inputWeights{1}.learnFcn = 'learngd';
net.biases{1}.learnFcn = 'learngd';
net.inputWeights{1}.learnParam.lr = 0.4;
net.biases{1}.learnParam.lr = 0.4;
net.trainParam.max_fail = 6;

[net, tr] = train(net, P, T);

%% 6.2
net = newff(P, T, [20]);
net.trainFcn = 'trainb';
net.inputWeights{1}.learnFcn = 'learngd';
net.biases{1}.learnFcn = 'learngd';
net.inputWeights{1}.learnParam.lr = 0.4;
net.biases{1}.learnParam.lr = 0.4;
net.trainParam.max_fail = 30;

[net, tr] = train(net, P, T);

% Пакетные алгоритмы обучения:
%% 7.1
clear err avg_err;
num_neurons = [1 20 30 40 60 80];
for i = 1:length(num_neurons)
    for j = 1:1
        net = newff(P, T, [num_neurons(i)]);
        net.trainFcn = 'traingda';
        net.trainParam.epochs = 5000;
        net.trainParam.goal = 0;
        net.trainParam.min_grad = 0;
        net.trainParam.max_fail = 50;
        net.trainParam.lr = 0.01;
        
        [net, tr] = train(net, P, T);
        err(i) = tr.best_vperf; % ошибка на контрольном множестве
    end
    avg_err(i) = mean(err);
end

figure;
plot(num_neurons, avg_err);

%% вывод графиков аппроксимации

trainfcn = 'traingda';
best = 40;
avg = 10;
worst = 1;
appPlot(trainfcn, best, avg, worst, P, T, P_VAL, T_VAL);

%% 7.2
clear err avg_err;
num_neurons = [1 20 30 40 60 80];
for i = 1:length(num_neurons)
    for j = 1:1
        net = newff(P, T, [num_neurons(i)]);
        net.trainFcn = 'traingdm';
        net.trainParam.epochs = 5000;
        net.trainParam.goal = 0;
        net.trainParam.min_grad = 0;
        net.trainParam.max_fail = 50;
        net.trainParam.lr = 0.01;
        
        [net, tr] = train(net, P, T);
        err(i) = tr.best_vperf; % ошибка на контрольном множестве
    end
    avg_err(i) = mean(err);
end

figure;
plot(num_neurons, avg_err);

%% вывод графиков аппроксимации

trainfcn = 'traingdm';
best = 40;
avg = 10;
worst = 1;
appPlot(trainfcn, best, avg, worst, P, T, P_VAL, T_VAL);

%% 7.3
clear err avg_err;
num_neurons = [1 20 30 40 60 80];
for i = 1:length(num_neurons)
    for j = 1:1
        net = newff(P, T, [num_neurons(i)]);
        net.trainFcn = 'traingdx';
        net.trainParam.epochs = 5000;
        net.trainParam.goal = 0;
        net.trainParam.min_grad = 0;
        net.trainParam.max_fail = 50;
        net.trainParam.lr = 0.01;
        
        [net, tr] = train(net, P, T);
        err(i) = tr.best_vperf; % ошибка на контрольном множестве
    end
    avg_err(i) = mean(err);
end

figure;
plot(num_neurons, avg_err);

%% вывод графиков аппроксимации

trainfcn = 'traingdx';
best = 30;
avg = 10;
worst = 1;
appPlot(trainfcn, best, avg, worst, P, T, P_VAL, T_VAL);

%% 7.4
clear err avg_err;
num_neurons = [1 20 30 40 60 80];
for i = 1:length(num_neurons)
    for j = 1:1
        net = newff(P, T, [num_neurons(i)]);
        net.trainFcn = 'trainbfg';
        net.trainParam.epochs = 5000;
        net.trainParam.goal = 0;
        net.trainParam.min_grad = 0;
        net.trainParam.max_fail = 6;
        net.trainParam.lr = 0.01;
        
        [net, tr] = train(net, P, T);
        err(i) = tr.best_vperf; % ошибка на контрольном множестве
    end
    avg_err(i) = mean(err);
end

figure;
plot(num_neurons, avg_err);

%% вывод графиков аппроксимации

trainfcn = 'trainbfg';
best = 40;
avg = 10;
worst = 80;
appPlot(trainfcn, best, avg, worst, P, T, P_VAL, T_VAL);

%% 7.5
clear err avg_err;
num_neurons = [1 20 30 40 60 80];
for i = 1:length(num_neurons)
    for j = 1:1
        net = newff(P, T, [num_neurons(i)]);
        net.trainFcn = 'trainlm';
        net.trainParam.epochs = 5000;
        net.trainParam.goal = 0;
        net.trainParam.min_grad = 0;
        net.trainParam.max_fail = 6;
        net.trainParam.lr = 0.01;
        
        [net, tr] = train(net, P, T);
        err(i) = tr.best_vperf; % ошибка на контрольном множестве
    end
    avg_err(i) = mean(err);
end

figure;
plot(num_neurons, avg_err);

%% вывод графиков аппроксимации

trainfcn = 'trainlm';
best = 30;
avg = 10;
worst = 1;
appPlot(trainfcn, best, avg, worst, P, T, P_VAL, T_VAL);

%% 7.6
clear err avg_err;
num_neurons = [1 20 30 40 60 80];
for i = 1:length(num_neurons)
    for j = 1:1
        net = newff(P, T, [num_neurons(i)]);
        net.trainFcn = 'trainoss';
        net.trainParam.epochs = 5000;
        net.trainParam.goal = 0;
        net.trainParam.min_grad = 0;
        net.trainParam.max_fail = 50;
        net.trainParam.lr = 0.01;
        
        [net, tr] = train(net, P, T);
        err(i) = tr.best_vperf; % ошибка на контрольном множестве
    end
    avg_err(i) = mean(err);
end

figure;
plot(num_neurons, avg_err);

%% вывод графиков аппроксимации

trainfcn = 'trainoss';
best = 30;
avg = 10;
worst = 1;
appPlot(trainfcn, best, avg, worst, P, T, P_VAL, T_VAL);

%% 7.7
clear err avg_err;
num_neurons = [1 20 30 40 60 80];
for i = 1:length(num_neurons)
    for j = 1:1
        net = newff(P, T, [num_neurons(i)]);
        net.trainFcn = 'traincgf';
        net.trainParam.epochs = 5000;
        net.trainParam.goal = 0;
        net.trainParam.min_grad = 0;
        net.trainParam.max_fail = 6;
        net.trainParam.lr = 0.01;
        
        [net, tr] = train(net, P, T);
        err(i) = tr.best_vperf; % ошибка на контрольном множестве
    end
    avg_err(i) = mean(err);
end

figure;
plot(num_neurons, avg_err);

%% вывод графиков аппроксимации

trainfcn = 'traincgf';
best = 40;
avg = 10;
worst = 1;
appPlot(trainfcn, best, avg, worst, P, T, P_VAL, T_VAL);

%% 8 Двухслойная НС прямого распространения

figure; % вывод графика исходной функции
hold on;
plot(P_VAL, T_VAL, 'LineWidth', 2, 'linestyle', '--', 'color', 'k');

net = newff(P, T, [10, 10]);
net.trainFcn = 'trainlm';
net.trainParam.epochs = 1000;
net.trainParam.goal = 0;
net.trainParam.min_grad = 0;
net.trainParam.max_fail = 50;
net.trainParam.lr = 0.01;

[net, tr] = train(net, P, T);
plot(P_VAL, sim(net, P_VAL), 'color', 'b', 'LineWidth', 2);


%% 6  Классификация с помощью НС ПР
%  использование 4-слойного персептрона из работы 1 для
%  формирования обучающей выборки
clear all;
net=network(1,3,[1 1 1]',[1 0 0]',[[0 1 0]' [0 0 1]' [0 0 0]'],[0 0 1]);
net.inputs{1}.size=2;

net.layers{1}.size=16;
net.layers{2}.size=4;
net.layers{3}.size=1;

net.layers{1}.transferFcn='hardlims';
net.layers{2}.transferFcn='hardlim';
net.layers{3}.transferFcn='hardlim';


net.IW{1,1} = [ [1 0];[-1 0]; [0 1];[ 0 -1];
                [ 1 -0];[ -1 0];[ 0 -1];[ 0 1];
                [ 1 0];[ -1 0];[ 0 -1];[ 0 1];
                [ 1 0];[ -1 0];[ 0 -1];[ 0 1] ];

net.b{1}= [ -0.2 1   0    0.2   
            -0.2 0.4 0.8 -0.2  
            -0.4 0.8 0.8 -0.6   
            -0.6 0.8 0.6 -0.4 ]';

net.LW{2,1} = [[ 1 1 1 1 0 0 0 0 0 0 0 0 0 0 0 0];
               [ 0 0 0 0 1 1 1 1 0 0 0 0 0 0 0 0];
               [ 0 0 0 0 0 0 0 0 1 1 1 1 0 0 0 0];
               [ 0 0 0 0 0 0 0 0 0 0 0 0 1 1 1 1];];

net.b{2}= -[3.5 3.5 3.5 3.5]';

net.LW{3,2} = [ 1 1 1 1 ];
net.b{3}= -0.5;

P = rand(2, 1000);
P_TEST = rand(2, 1000);
T = sim(net, P);
T_TEST = sim(net, P_TEST);

% обучение нс класификации на сфомированной выборке

net = newff(P, T, [10, 10]);
net.trainFcn = 'trainlm';
net.trainParam.epochs = 1000;
net.trainParam.goal = 0;
net.trainParam.min_grad = 0;
net.trainParam.max_fail = 6;
net.trainParam.lr = 0.01;

[net, tr] = train(net, P, T);

% граф
figure;
hold on;
plot([0.2 0.2 0.8 0.8 0.6 0.6 0.4 0.4 1], ...
    [0 0.8 0.8 0.4 0.4 0.6 0.6 0.2 0.2] ,  'LineWidth', 2, 'Color', 'k');

for i = 1:1000
    P1{i} = rand(2, 1);
    T1{i} = sim(net, P1{i});
    if round(T1{i})
        color = [.9 .1 .1];
    else
        color = [.1 .1 .9];
    end
    plot(P1{i}(1), P1{i}(2), 'marker', 'x', 'markersize', 15, ...
        'color', color);
end
axis([0 1 0 1]);

% определение оптимального числа нейронов
clear err avg_err;
num_neurons = [100 120 130 140 150];
for i = 1:length(num_neurons)
    for j = 1:1
        net = newff(P, T, [num_neurons(i)]);
        net.trainFcn = 'traincgf';
        net.trainParam.epochs = 1000;
        net.trainParam.goal = 0;
        net.trainParam.min_grad = 0;
        net.trainParam.max_fail = 6;
        net.trainParam.lr = 0.01;
        
        [net, tr] = train(net, P, T);
        err(i) = sum(abs(sim(net, P_TEST) - T_TEST))/length(P_TEST); 
    end
    avg_err(i) = mean(err);
end

figure;
plot(num_neurons, avg_err);
