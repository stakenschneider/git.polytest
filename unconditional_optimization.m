function [] = Main ()

clc;
clear all;                     
close all;

initialX = [-7; 10]; % Начальная точка  
initialX2 = [0; 0]; % Начальная точка  
initialX3= [4; 3]; % Начальная точка  
initialX4 = [6; 5]; % Начальная точка  
 
index = [-7,-13,8,10,80]; % Значения аргументов
e = 0.1;                    
H = [index(1)*2, index(3); 
     index(3), index(2)*2]; 

% Открытие файла вывода для записи результатов
fileID = fopen('results.txt', 'wt');

% Функция построение графика метода
function [] = PlotGraph (v,v2,v3,v4)
    
% Область построения
x_1=-7:.1:6;
x_2=0:.1:10;

[x_1,x_2]=meshgrid(x_1,x_2);
w=(index(1)*x_1.^2 +index(2)*x_2.^2 + index(3)*x_1.*x_2 + index(4)*x_1 + index(5)*x_2 );

figure;
hold on;
contour(x_1,x_2,w,30);
plot(x, y, x2,y2,x3,y3,x4,y4);
contour(x_1,x_2,w,v,w,v2,w,v3,w,v4);
xlabel('x1');
ylabel('x2');
hold off;
end


% Вычисление функции и значение её производной
function [fX, dfX] = derivative(X) 
% Вычисление значения функции от Х
fX = index(1) * X(1)^2 + index(2) * X(2)^2 + index(3) * X(1) * X(2) + index(4) * X(1) + index(5) * X(2);
% Вычисление частных производных по Х1 и Х2 соответственно
dfX = [index(1)*2 * X(1) + index(3) * X(2) + index(4); index(2)*2 * X(2) + index(3) * X(1) + index(5)];
end

%% Метод релаксационный
X=initialX;  
X2 = initialX2;
X3 = initialX3;
X4 = initialX4;

[fX, dfX] = derivative(X);
i = 1;
j = 1;
% clear x y;
x(i) = X(1);
y(i) = X(2);
v(1,1) = fX;
K=[dfX(1);0];
t=-(dfX'*K)/(K'*H*K);
while (norm(dfX) > e)
    X = X+t*K;
    [fX, dfX] = derivative(X);
    i = i+1;
    x(i) = X(1);
    y(i) = X(2);
    v(1,i) = fX;
    K=dfX;
    t=-(dfX'*K)/(K'*H*K);
end
% PlotGraph(v);





[fX, dfX] = derivative(X2);
i = 1;
j = 1;
% clear x y;
x2(i) = X2(1);
y2(i) = X2(2);
v2(1,1) = fX;
K=[dfX(1);0];
t=-(dfX'*K)/(K'*H*K);
while (norm(dfX) > e)
    X2 = X2+t*K;
    [fX, dfX] = derivative(X2);
    i = i+1;
    x2(i) = X2(1);
    y2(i) = X2(2);
    v2(1,i) = fX;
    K=dfX;
    t=-(dfX'*K)/(K'*H*K);
end




[fX, dfX] = derivative(X3);
i = 1;
j = 1;
x3(i) = X3(1);
y3(i) = X3(2);
v3(1,1) = fX;
K=[dfX(1);0];
t=-(dfX'*K)/(K'*H*K);
while (norm(dfX) > e)
    X3 = X3+t*K;
    [fX, dfX] = derivative(X3);
    i = i+1;
    x3(i) = X3(1);
    y3(i) = X3(2);
    v(1,i) = fX;
    K=dfX;
    t=-(dfX'*K)/(K'*H*K);
end


[fX, dfX] = derivative(X4);
i = 1;
j = 1;

x4(i) = X4(1);
y4(i) = X4(2);
v4(1,1) = fX;
K=[dfX(1);0];
t=-(dfX'*K)/(K'*H*K);
while (norm(dfX) > e)
    X4 = X4+t*K;
    [fX, dfX] = derivative(X4);
    i = i+1;
    x4(i) = X4(1);
    y4(i) = X4(2);
    v4(1,i) = fX;
    K=dfX;
    t=-(dfX'*K)/(K'*H*K);
end
PlotGraph(v,v2,v3,v4);
legend ('Линии равного уровня','Методы релаксации','Метод релаксации 2','Метод релаксации 3','Метод релаксации 4');







%%  Метод наискорейшего подъема
X=initialX;  
X2 = initialX2;
X3 = initialX3;
X4 = initialX4;


[fX, dfX] = derivative(X);
i = 1;
j = 1;
x(i) = X(1);
y(i) = X(2);
v(1,1) = fX;
t=-(dfX'*dfX)/(dfX'*H*dfX);
while (norm(dfX) > e)
    X = X+t*dfX;
    [fX, dfX] = derivative(X); % Считаем значение функции и производной
    i = i+1;
    x(i) = X(1);
    y(i) = X(2);
    v(1,i) = fX;
    t=-(dfX'*dfX)/(dfX'*H*dfX);
end

[fX, dfX] = derivative(X2);
i = 1;
j = 1;
x2(i) = X2(1);
y2(i) = X2(2);
v2(1,1) = fX;
t=-(dfX'*dfX)/(dfX'*H*dfX);
while (norm(dfX) > e)
    X2 = X2+t*dfX;
    [fX, dfX] = derivative(X2); % Считаем значение функции и производной
    i = i+1;
    x2(i) = X2(1);
    y2(i) = X2(2);
    v2(1,i) = fX;
    t=-(dfX'*dfX)/(dfX'*H*dfX);
end

[fX, dfX] = derivative(X3);
i = 1;
j = 1;
x3(i) = X3(1);
y3(i) = X3(2);
v3(1,1) = fX;
t=-(dfX'*dfX)/(dfX'*H*dfX);
while (norm(dfX) > e)
    X3 = X3+t*dfX;
    [fX, dfX] = derivative(X3); % Считаем значение функции и производной
    i = i+1;
    x3(i) = X3(1);
    y3(i) = X3(2);
    v3(1,i) = fX;
    t=-(dfX'*dfX)/(dfX'*H*dfX);
end

[fX, dfX] = derivative(X4);
i = 1;
j = 1;
x4(i) = X4(1);
y4(i) = X4(2);
v4(1,1) = fX;
t=-(dfX'*dfX)/(dfX'*H*dfX);
while (norm(dfX) > e)
    X4 = X4+t*dfX;
    [fX, dfX] = derivative(X4); % Считаем значение функции и производной
    i = i+1;
    x4(i) = X4(1);
    y4(i) = X4(2);
    v4(1,i) = fX;
    t=-(dfX'*dfX)/(dfX'*H*dfX);
end
PlotGraph(v,v2,v3,v4);
legend ('Линии равного уровня','Методы наискорейшего подъема');

%%  Метод Ньютона
X=initialX;  
X2 = initialX2;
X3 = initialX3;
X4 = initialX4;

[fX, dfX] = derivative(X);
i = 1;
x(i) = X(1);
y(i) = X(2);
v(1,1) = fX;
while (norm(dfX) > e)
    X = X - H^(-1) * dfX;
    [fX, dfX] = derivative(X);
    i = i + 1;
    x(i) = X(1);
    y(i) = X(2);
    v(1,i) = fX;

end

[fX, dfX] = derivative(X2);
i = 1;
x2(i) = X2(1);
y2(i) = X2(2);
v2(1,1) = fX;
while (norm(dfX) > e)
    X2 = X2 - H^(-1) * dfX;
    [fX, dfX] = derivative(X2);
    i = i + 1;
    x2(i) = X2(1);
    y2(i) = X2(2);
    v2(1,i) = fX;

end

[fX, dfX] = derivative(X3);
i = 1;
x3(i) = X3(1);
y3(i) = X3(2);
v3(1,1) = fX;
while (norm(dfX) > e)
    X3 = X3 - H^(-1) * dfX;
    [fX, dfX] = derivative(X3);
    i = i + 1;
    x3(i) = X(1);
    y3(i) = X3(2);
    v3(1,i) = fX;

end

[fX, dfX] = derivative(X4);
i = 1;
x4(i) = X4(1);
y4(i) = X4(2);
v4(1,1) = fX;
while (norm(dfX) > e)
    X4 = X4 - H^(-1) * dfX;
    [fX, dfX] = derivative(X4);
    i = i + 1;
    x4(i) = X4(1);
    y4(i) = X4(2);
    v4(1,i) = fX;

end


PlotGraph(v,v2,v3,v4);
legend ('Линии равного уровня','Методы Ньютона');
fprintf(fileID, '\n\n');

%%  Метод сопряженных градиентов
X=initialX;  
X2 = initialX2;
X3 = initialX3;
X4 = initialX4;


[fX, dfX] = derivative(X);
K = dfX;
i = 1;
x(i) = X(1);
y(i) = X(2);
v(1,1) = fX;
t = -(dfX' * K) / ((H * K)' * K);
while (norm(dfX) > e)
     X = X + t * K;
    [fX, dfX] = derivative(X);
    K=dfX+(norm(dfX))^2 / (norm(K))^2 *K;
    i = i + 1;
    x(i) = X(1);
    y(i) = X(2);
    v(1,i) = fX;
    t = -(dfX' * K) / ((H * K)' * K);
end


[fX, dfX] = derivative(X2);
K = dfX;
i = 1;
x2(i) = X2(1);
y2(i) = X2(2);
v2(1,1) = fX;
t = -(dfX' * K) / ((H * K)' * K);
while (norm(dfX) > e)
     X2 = X2 + t * K;
    [fX, dfX] = derivative(X2);
    K=dfX+(norm(dfX))^2 / (norm(K))^2 *K;
    i = i + 1;
    x2(i) = X2(1);
    y2(i) = X2(2);
    v2(1,i) = fX;
    t = -(dfX' * K) / ((H * K)' * K);
end

[fX, dfX] = derivative(X3);
K = dfX;
i = 1;
x3(i) = X3(1);
y3(i) = X3(2);
v(1,1) = fX;
t = -(dfX' * K) / ((H * K)' * K);
while (norm(dfX) > e)
     X3 = X3 + t * K;
    [fX, dfX] = derivative(X3);
    K=dfX+(norm(dfX))^2 / (norm(K))^2 *K;
    i = i + 1;
    x3(i) = X3(1);
    y3(i) = X3(2);
    v3(1,i) = fX;
    t = -(dfX' * K) / ((H * K)' * K);
end

[fX, dfX] = derivative(X4);
K = dfX;
i = 1;
x4(i) = X4(1);
y4(i) = X4(2);
v4(1,1) = fX;
t = -(dfX' * K) / ((H * K)' * K);
while (norm(dfX) > e)
     X4 = X4 + t * K;
    [fX, dfX] = derivative(X4);
    K=dfX+(norm(dfX))^2 / (norm(K))^2 *K;
    i = i + 1;
    x4(i) = X4(1);
    y4(i) = X4(2);
    v4(1,i) = fX;
    t = -(dfX' * K) / ((H * K)' * K);
end


PlotGraph(v,v2,v3,v4);
legend ('Линии равного уровня','Метод сопряженных градиентов');
fprintf(fileID, '\n\n');

%% Метод Бройдена
X=initialX;  
X2 = initialX2;
X3 = initialX3;
X4 = initialX4;

[fX,dfX] = derivative(X);
i=1;
x(i) = X(1);
y(i) = X(2);
v(1,1) = fX;
n = -eye(2);
K = dfX;
t = - dfX'*K/(K'*H*K)
bX=X;
[fbX, dfbX] = derivative(bX);
X = X + t * K;
[fX, dfX] = derivative(X); % i-тая
i=2;
x(i) = X(1);
y(i) = X(2);
v(1,i) = fX;
dg=dfX - dfbX
dx=X-bX
z=dx-n*dg
dn=(z*z')/(z'*dg)
n = n +dn
K=-n*dfX
t= -(dfX'*K)/(K'*H*K)
while (norm(dfX) > e)
    bX=X;
    X=X+t*K;
    i = i + 1;
    x(i) = X(1);
    y(i) = X(2);
    v(1,i) = fX;
    [fbX, dfbX] = derivative(bX);
    [fX, dfX] = derivative(X); % i-тая
    dg=dfX - dfbX;
    dx=X-bX;
    z=dx-n*dg;
    dn=(z*z')/(z'*dg);
    n = n +dn;
    K=-n*dfX;
    t= -(dfX'*K)/(K'*H*K);
end 



[fX,dfX] = derivative(X2);
i=1;
x2(i) = X2(1);
y2(i) = X2(2);
v2(1,1) = fX;
n = -eye(2);
K = dfX;
t = - dfX'*K/(K'*H*K)
bX=X2;
[fbX, dfbX] = derivative(bX);
X2 = X2 + t * K;
[fX, dfX] = derivative(X2); % i-тая
i=2;
x2(i) = X2(1);
y2(i) = X2(2);
v2(1,i) = fX;
dg=dfX - dfbX
dx=X2-bX
z=dx-n*dg
dn=(z*z')/(z'*dg)
n = n +dn
K=-n*dfX
t= -(dfX'*K)/(K'*H*K)
while (norm(dfX) > e)
    bX=X2;
    X2=X2+t*K;
    i = i + 1;
    x2(i) = X2(1);
    y2(i) = X2(2);
    v2(1,i) = fX;
    [fbX, dfbX] = derivative(bX);
    [fX, dfX] = derivative(X2); % i-тая
    dg=dfX - dfbX;
    dx=X2-bX;
    z=dx-n*dg;
    dn=(z*z')/(z'*dg);
    n = n +dn;
    K=-n*dfX;
    t= -(dfX'*K)/(K'*H*K);
end 



[fX,dfX] = derivative(X3);
i=1;
x3(i) = X3(1);
y3(i) = X3(2);
v3(1,1) = fX;
n = -eye(2);
K = dfX;
t = - dfX'*K/(K'*H*K)
bX=X3;
[fbX, dfbX] = derivative(bX);
X3 = X3 + t * K;
[fX, dfX] = derivative(X3); % i-тая
i=2;
x3(i) = X3(1);
y3(i) = X3(2);
v3(1,i) = fX;
dg=dfX - dfbX
dx=X3-bX
z=dx-n*dg
dn=(z*z')/(z'*dg)
n = n +dn
K=-n*dfX
t= -(dfX'*K)/(K'*H*K)
while (norm(dfX) > e)
    bX=X3;
    X3=X3+t*K;
    i = i + 1;
    x3(i) = X3(1);
    y3(i) = X3(2);
    v3(1,i) = fX;
    [fbX, dfbX] = derivative(bX);
    [fX, dfX] = derivative(X3); % i-тая
    dg=dfX - dfbX;
    dx=X3-bX;
    z=dx-n*dg;
    dn=(z*z')/(z'*dg);
    n = n +dn;
    K=-n*dfX;
    t= -(dfX'*K)/(K'*H*K);
end 



[fX,dfX] = derivative(X4);
i=1;
x4(i) = X4(1);
y4(i) = X4(2);
v4(1,1) = fX;
n = -eye(2);
K = dfX;
t = - dfX'*K/(K'*H*K)
bX=X4;
[fbX, dfbX] = derivative(bX);
X4 = X4 + t * K;
[fX, dfX] = derivative(X4); % i-тая
i=2;
x4(i) = X4(1);
y4(i) = X4(2);
v4(1,i) = fX;
dg=dfX - dfbX
dx=X4-bX
z=dx-n*dg
dn=(z*z')/(z'*dg)
n = n +dn
K=-n*dfX
t= -(dfX'*K)/(K'*H*K)
while (norm(dfX) > e)
    bX=X4;
    X4=X4+t*K;
    i = i + 1;
    x4(i) = X4(1);
    y4(i) = X4(2);
    v4(1,i) = fX;
    [fbX, dfbX] = derivative(bX);
    [fX, dfX] = derivative(X4); % i-тая
    dg=dfX - dfbX;
    dx=X4-bX;
    z=dx-n*dg;
    dn=(z*z')/(z'*dg);
    n = n +dn;
    K=-n*dfX;
    t= -(dfX'*K)/(K'*H*K);
end 


PlotGraph(v,v2,v3,v4);
legend ('Линии равного уровня','Метод Бройдена');
fprintf(fileID, '\n\n');

%  Метод переменной метрики ДФП
X=initialX;  
X2 = initialX2;
X3 = initialX3;
X4 = initialX4;
[fX, dfX] = derivative(X);
i = 1;

x(i) = X(1);
y(i) = X(2);
v(1,1) = fX;
n = -eye(2);
K = -n * dfX;
t = -(dfX' * K) / ((H * K)' * K)
while (norm(dfX) > e)
    dfbXinitial = dfX;
    t = -(dfX' * K) / ((H * K)' * K)
    deltaX = t * K;
    X = X + deltaX;
    [fX, dfX] = derivative(X);
    deltaG = dfX - dfbXinitial
    A = (deltaX * deltaX') / (deltaX' * deltaG)
    B = (n * deltaG * deltaG' * n') / (deltaG' * n' * deltaG)
    delta_n = A - B
    n = n + delta_n;
    K = -n * dfX;
    i = i + 1;
    x(i) = X(1);
    y(i) = X(2);
    v(1,i) = fX;
end

[fX, dfX] = derivative(X2);
i = 1;
x2(i) = X2(1);
y2(i) = X2(2);
v2(1,1) = fX;
n = -eye(2);
K = -n * dfX;
t = -(dfX' * K) / ((H * K)' * K)
while (norm(dfX) > e)
    dfbXinitial = dfX;
    t = -(dfX' * K) / ((H * K)' * K)
    deltaX = t * K;
    X2 = X2 + deltaX;
    [fX, dfX] = derivative(X2);
    deltaG = dfX - dfbXinitial
    A = (deltaX * deltaX') / (deltaX' * deltaG)
    B = (n * deltaG * deltaG' * n') / (deltaG' * n' * deltaG)
    delta_n = A - B
    n = n + delta_n;
    K = -n * dfX;
    i = i + 1;
    x2(i) = X2(1);
    y2(i) = X2(2);
    v2(1,i) = fX;
end

[fX, dfX] = derivative(X3);
i = 1;

x3(i) = X3(1);
y3(i) = X3(2);
v3(1,1) = fX;
n = -eye(2);
K = -n * dfX;
t = -(dfX' * K) / ((H * K)' * K)
while (norm(dfX) > e)
    dfbXinitial = dfX;
    t = -(dfX' * K) / ((H * K)' * K)
    deltaX = t * K;
    X3 = X3 + deltaX;
    [fX, dfX] = derivative(X3);
    deltaG = dfX - dfbXinitial
    A = (deltaX * deltaX') / (deltaX' * deltaG)
    B = (n * deltaG * deltaG' * n') / (deltaG' * n' * deltaG)
    delta_n = A - B
    n = n + delta_n;
    K = -n * dfX;
    i = i + 1;
    x3(i) = X3(1);
    y3(i) = X3(2);
    v3(1,i) = fX;
end
[fX, dfX] = derivative(X4);
i = 1;

x4(i) = X4(1);
y4(i) = X4(2);
v4(1,1) = fX;
n = -eye(2);
K = -n * dfX;
t = -(dfX' * K) / ((H * K)' * K)
while (norm(dfX) > e)
    dfbXinitial = dfX;
    t = -(dfX' * K) / ((H * K)' * K)
    deltaX = t * K;
    X4 = X4 + deltaX;
    [fX, dfX] = derivative(X4);
    deltaG = dfX - dfbXinitial
    A = (deltaX * deltaX') / (deltaX' * deltaG)
    B = (n * deltaG * deltaG' * n') / (deltaG' * n' * deltaG)
    delta_n = A - B
    n = n + delta_n;
    K = -n * dfX;
    i = i + 1;
    x4(i) = X4(1);
    y4(i) = X4(2);
    v4(1,i) = fX;
end





PlotGraph(v,v2,v3,v4);
legend('Линии равного уровня', 'Метод переменной метрики ДФП');

end
