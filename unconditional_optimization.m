function [] = Main ()

clc;
clear all;                     
close all;

initialX = [-7; 10]; % Начальная точка  
  
index = [-7,-13,8,10,80]; % Значения аргументов
e = 0.1;                    
H = [index(1)*2, index(3); 
     index(3), index(2)*2]; 

% Открытие файла вывода для записи результатов
fileID = fopen('results.txt', 'wt');

% Функция построение графика метода
function [] = PlotGraph (v)
    
% Область построения
x_1=-7:.1:6.9;
x_2=1.7:.1:10;

[x_1,x_2]=meshgrid(x_1,x_2);
w=(index(1)*x_1.^2 +index(2)*x_2.^2 + index(3)*x_1.*x_2 + index(4)*x_1 + index(5)*x_2 );

figure;
hold on;
contour(x_1,x_2,w,30);
plot(x, y, '.-k');
contour(x_1,x_2,w,v);
xlabel('x1');
ylabel('x2');
hold off;
end

% Функция построения графика сравнения кол-ва итераций
function [] = PlotIterCountGraph ()
figure;
surf(from:1:2*to,from:1:2*to,N);
xlabel('x1');
ylabel('x2');
zlabel('Кол-во итераций');
colorbar

figure;
contourf(from:1:2*to,from:1:2*to,N)
xlabel('x1');
ylabel('x2');
c = colorbar;
c.Label.String = 'Кол-во итераций';
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
[fX, dfX] = derivative(X);
i = 1;
j = 1;
clear x y;
x(i) = X(1);
y(i) = X(2);
v(1,1) = fX;
K=[dfX(1);0];
t=-(dfX'*K)/(K'*H*K);
fprintf(fileID, 'Релаксационный метод\n\n');
fprintf(fileID, 'i    x1         x2           gradf(X)1   gradf(X)2   K1         K2          t           fX        ||df(X)||\n');
fprintf(fileID, '%-4d %-10.4f %-10.4f   %-10.4f  %-10.4f  %-10.4f %-10.4f  %-10.4f  %-10.4f  %-10.4f\n',i, X, dfX, K, t, fX, norm(dfX));
while (norm(dfX) > e)
    X = X+t*K;
    [fX, dfX] = derivative(X);
    i = i+1;
    x(i) = X(1);
    y(i) = X(2);
    v(1,i) = fX;
    K=dfX;
    t=-(dfX'*K)/(K'*H*K);
fprintf(fileID, '%-4d %-10.4f %-10.4f   %-10.4f  %-10.4f  %-10.4f %-10.4f  %-10.4f  %-10.4f  %-10.4f\n',i, X, dfX, K, t, fX, norm(dfX));
end
PlotGraph(v);
legend ('Линии равного уровня', 'Релаксационный метод');
fprintf(fileID, '\n\n');
clear v;

%%  Метод наискорейшего подъема
X = initialX;
[fX, dfX] = derivative(X);
i = 1;
j = 1;
x(i) = X(1);
y(i) = X(2);
v(1,1) = fX;
t=-(dfX'*dfX)/(dfX'*H*dfX);
fprintf(fileID, 'Метод наискорейшего подъема\n\n');
fprintf(fileID, 'i    x1         x2           gradf(X)1   gradf(X)2   t           fX        ||df(X)||\n');
fprintf(fileID, '%-4d %-10.4f %-10.4f   %-10.4f  %-10.4f  %-10.4f  %-10.4f  %-10.4f\n',i, X, dfX, t, fX, norm(dfX));
while (norm(dfX) > e)
    X = X+t*dfX;
    [fX, dfX] = derivative(X); % Считаем значение функции и производной
    i = i+1;
    x(i) = X(1);
    y(i) = X(2);
    v(1,i) = fX;
    t=-(dfX'*dfX)/(dfX'*H*dfX);
    fprintf(fileID, '%-4d %-10.4f %-10.4f   %-10.4f  %-10.4f  %-10.4f  %-10.4f  %-10.4f\n',i, X, dfX, t, fX, norm(dfX));
end
PlotGraph(v);
legend ('Линии равного уровня','Метод наискорейшего подъема');
fprintf(fileID, '\n\n');
clear v;
%%  Метод Ньютона
X = initialX;
[fX, dfX] = derivative(X);
i = 1;
clear x y;
x(i) = X(1);
y(i) = X(2);
v(1,1) = fX;
fprintf(fileID, 'Метод Ньютона\n\n');
fprintf(fileID, 'i    x1         x2         f(X)       ||df(X)||\n');
fprintf(fileID, '%-4d %-10.4f %-10.4f %-10.4f %-10.4f\n', i, X, fX, norm(dfX));
while (norm(dfX) > e)
    X = X - H^(-1) * dfX;
    [fX, dfX] = derivative(X);
    i = i + 1;
    x(i) = X(1);
    y(i) = X(2);
    v(1,i) = fX;
    fprintf(fileID, '%-4d %-10.4f %-10.4f %-10.4f %-10.4f\n', i, X, fX, norm(dfX));
end
PlotGraph(v);
legend ('Линии равного уровня','Метод Ньютона');
fprintf(fileID, '\n\n');
clear v;
%%  Метод сопряженных градиентов
X = initialX;
[fX, dfX] = derivative(X);
K = dfX;
i = 1;
clear x y;
x(i) = X(1);
y(i) = X(2);
v(1,1) = fX;
t = -(dfX' * K) / ((H * K)' * K);
fprintf(fileID, 'Метод сопряженных градиентов\n\n');
fprintf(fileID, 'i    x1         x2           gradf(X)1   gradf(X)2   K1         K2          t           fX        ||df(X)||\n');
fprintf(fileID, '%-4d %-10.4f %-10.4f   %-10.4f  %-10.4f  %-10.4f %-10.4f  %-10.4f  %-10.4f  %-10.4f\n',i, X, dfX, K, t, fX, norm(dfX));
while (norm(dfX) > e)
     X = X + t * K;
    [fX, dfX] = derivative(X);
    K=dfX+(norm(dfX))^2 / (norm(K))^2 *K;
    i = i + 1;
    x(i) = X(1);
    y(i) = X(2);
    v(1,i) = fX;
    t = -(dfX' * K) / ((H * K)' * K);
    fprintf(fileID, '%-4d %-10.4f %-10.4f   %-10.4f  %-10.4f  %-10.4f %-10.4f  %-10.4f  %-10.4f  %-10.4f\n',i, X, dfX, K, t, fX, norm(dfX));
end
PlotGraph(v);
legend ('Линии равного уровня','Метод сопряженных градиентов');
fprintf(fileID, '\n\n');
clear v;
%% Метод Бройдена
X=initialX;
[fX,dfX] = derivative(X);
i=1;
clear x y;
x(i) = X(1);
y(i) = X(2);
v(1,1) = fX;
n = -eye(2);
K = dfX;
t = - dfX'*K/(K'*H*K)
fprintf(fileID, 'метод Бройдена\n\n');
fprintf(fileID, 'i    x           gradf(X)   K          t           fX       n                      ||df(X)||\n');
fprintf(fileID, '%-4d %-10.4f  %-10.4f %-10.4f %-10.4f %-10.4f %-10.4f %-10.4f %-10.4f\n',i, X(1), dfX(1), K(1), t, fX, n(1,1), n(1,2),norm(dfX));
fprintf(fileID, '     %-10.4f  %-10.4f %-10.4f                       %-10.4f %-10.4f     \n',X(2), dfX(2), K(2), n(2,1), n(2,2));
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
fprintf(fileID, '%-4d %-10.4f  %-10.4f %-10.4f %-10.4f %-10.4f %-10.4f %-10.4f %-10.4f\n',i, X(1), dfX(1), K(1), t, fX, n(1,1), n(1,2),norm(dfX));
fprintf(fileID, '     %-10.4f  %-10.4f %-10.4f                       %-10.4f %-10.4f     \n',X(2), dfX(2), K(2), n(2,1), n(2,2));
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
    fprintf(fileID, '%-4d %-10.4f  %-10.4f %-10.4f %-10.4f %-10.4f %-10.4f %-10.4f %-10.4f\n',i, X(1), dfX(1), K(1), t, fX, n(1,1), n(1,2),norm(dfX));
    fprintf(fileID, '     %-10.4f  %-10.4f %-10.4f                       %-10.4f %-10.4f     \n',X(2), dfX(2), K(2), n(2,1), n(2,2));
end 
PlotGraph(v);
legend ('Линии равного уровня','Метод Бройдена');
fprintf(fileID, '\n\n');
clear v;
%%  Метод переменной метрики ДФП
X = initialX;
[fX, dfX] = derivative(X);
i = 1;
clear x y;
x(i) = X(1);
y(i) = X(2);
v(1,1) = fX;
n = -eye(2);
K = -n * dfX;
t = -(dfX' * K) / ((H * K)' * K)
fprintf(fileID, 'Метод переменной метрики ДФП\n\n');
fprintf(fileID, 'i    x           gradf(X)   K          t           fX        ||df(X)||\n');
fprintf(fileID, '%-4d %-10.4f  %-10.4f %-10.4f %-10.4f %-10.4f %-10.4f\n',i, X(1), dfX(1), K(1), t, fX,norm(dfX));
fprintf(fileID, '     %-10.4f  %-10.4f %-10.4f                         \n',X(2), dfX(2), K(2));
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
    fprintf(fileID, '%-4d %-10.4f  %-10.4f %-10.4f %-10.4f %-10.4f %-10.4f\n',i, X(1), dfX(1), K(1), t, fX,norm(dfX));
    fprintf(fileID, '     %-10.4f  %-10.4f %-10.4f                         \n',X(2), dfX(2), K(2));
end
PlotGraph(v);
legend('Линии равного уровня', 'Метод переменной метрики ДФП');
fclose(fileID);
clear v;
end
