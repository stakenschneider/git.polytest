%% 1
% 1. Задайте входные данные
p = [.1 .8 .1 .9 ; .2 .9 .1 .8];
% 2. Создайте соревновательную НС
net = newc([0 1; 0 1],2);
% 3. Проверьте как инициализируются веса и смещения
wts = net.IW{1,1} 
biases = net.b{1}
% 4. Обучите НС на 500 эпохах.
net.trainParam.epochs = 500;
net = train(net,p);
% 5. Проверьте работу НС на входных примерах
a = sim(net,p)
ac = vec2ind(a)
% 6. Посмотрите, как изменились весовые коэффициенты и смещения 7. Постройте исходные данные и поверх них центры кластеров
wts = net.IW{1,1}
biases = net.b{1}
% 7. Постройте исходные данные и поверх них центры кластеров
plot(p(1,:), p(2,:),'marker','x', 'markersize', 15, 'linewidth', 2, 'linestyle', 'none')
axis([0 1 0 1.2]); hold on; grid on;
plot(wts(:,1), wts(:,2), 'marker','o', 'markersize', 10, 'linewidth', 2, 'linestyle', 'none', 'color','r') 
legend('Input Data', 'Cluster centers');

%% 2
% 1. Сгенерируйте множество кластеров в 2-мерном пространстве
bounds = [0 1; 0 1]; % Область расположение кластеров
clusters = 8;        % Количество кластеров
points = 10;         % Количество точек в каждом кластере
std_dev = 0.05;      % СКО точек от кластера
x = nngenc(bounds,clusters,points,std_dev);
% 2. Постройте точки всех кластеров
plot(x(1,:),x(2,:),'+r');
title('Input Vector');
xlabel('x(1)');
ylabel('x(2)');
% 3. Создайте соревновательную НС и постройте центры ее кластеров до обучения
net = newc(x, clusters,.1, 0); % Количество нейронов как и количество кластеров 
w = net.IW{1};
plot(x(1,:),x(2,:),'+r');
hold on;
circles = plot(w(:,1),w(:,2),'ob');
% 4. Обучите НС
net.trainParam.epochs = 20; 
net = train(net,x);
w = net.IW{1};
% 5. Перестройте центры кластеров
delete(circles);
plot(w(:,1),w(:,2),'ob');
% 6. Определите реакцию НС на новую точку x1
x1 = [0; 0.2];
y = net(x1)
% 7. Постройте разбиение пространства на кластеры
delta = .1; 
k_delta = 2;
minX1 = round(min(x(1,:))/delta )*delta - k_delta*delta; 
minX2 = round(min(x(2,:))/delta )*delta - k_delta*delta; 
maxX1 = round(max(x(1,:))/delta )*delta + k_delta*delta; 
maxX2 = round(max(x(2,:))/delta )*delta + k_delta*delta; 
delta2 = .01;
x1 = minX1:delta2:maxX1;
x2 = minX2:delta2:maxX2;
[X1,X2] = meshgrid(x1,x2);
xx = [ X1(:) X2(:)]';
yy = net(xx);
yy = full(yy);
colors = { [0 0.5 1], [0 1.0 0.5], [0.5 0 1], 'r', 'c', 'k', 'm', 'y' }; 
figure;

for i = 1:size(yy,1)
m = mesh(X1,X2,reshape(yy(i,:),length(x2),length(x1))); 
set(m,'facecolor',colors{i},'linestyle','none');
if i == 1
hold on 
end;
end;

plot3(x(1,:),x(2,:), ones(1, size(x,2))+0.1,'.','markersize',30) 
plot3(w(:,1),w(:,2),1.1*ones(size(w(:,1))),'.','markersize',30,'color', [ 1 1 1 ]) 
hold off
view(2)
xlabel('x1')
ylabel('x2')
axis([minX1 maxX1 minX2 maxX2]);

