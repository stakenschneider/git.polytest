function [ output_args ] = appPlot( trainfcn, best, avg, worst, P, T, ...
    P_VAL, T_VAL )
% вывод графиков аппроксимации
figure;
hold on;
plot(P_VAL, T_VAL, 'LineWidth', 2, 'linestyle', '--', 'color', 'k');

net = newff(P, T, [best]);
net.trainFcn = trainfcn;
net.trainParam.epochs = 5000;
net.trainParam.goal = 0;
net.trainParam.min_grad = 0;
net.trainParam.max_fail = 50;
net.trainParam.lr = 0.01;
[net, tr] = train(net, P, T);
plot(P_VAL, sim(net, P_VAL), 'color', 'b', 'LineWidth', 2);

net = newff(P, T, [avg]);
net.trainFcn = trainfcn;
net.trainParam.epochs = 5000;
net.trainParam.goal = 0;
net.trainParam.min_grad = 0;
net.trainParam.max_fail = 50;
net.trainParam.lr = 0.01;
[net, tr] = train(net, P, T);
plot(P_VAL, sim(net, P_VAL), 'color', 'm', 'LineWidth', 2);

net = newff(P, T, [worst]);
net.trainFcn = trainfcn;
net.trainParam.epochs = 5000;
net.trainParam.goal = 0;
net.trainParam.min_grad = 0;
net.trainParam.max_fail = 50;
net.trainParam.lr = 0.01;
[net, tr] = train(net, P, T);
plot(P_VAL, sim(net, P_VAL), 'color', 'r', 'LineWidth', 2);
end
