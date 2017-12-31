clear; clc;
N=300;
% exe 1
% {
% X = rand(2, N)
% 
% fid = fopen('my_file.dat', 'wb');     % открытие файла на запись 
% if fid == -1                     % проверка корректности открытия 
%     error('File is not opened'); 
% end 
% 
% fwrite(fid, X, 'double');   % запись матрицы в файл (40 байт) 
% fclose(fid);                % закрытие файла 
% }
fid = fopen('my.txt', 'rb');     % открытие файла на чтение 
if fid == -1                     % проверка корректности открытия 
    error('File is not opened'); 
end 
 
X = zeros(N);
Y = zeros(N);
X = fread(fid, N, 'double');     % чтение 5 значений double 
Y = fread(fid, N, 'double');

fclose(fid);                     % закрытие файла
%plot(X,Y ,'.');




%****************
% ПУНКТ ""Б""
b= -0.0072;   % ровно половина точек
line_class1_X = zeros(1,N/2);  % поэтому N/2
line_class1_Y = zeros(1,N/2);
line_class2_X = zeros(1,N/2);
line_class2_Y = zeros(1,N/2);
i_cl1=1; i_cl2=1;
% Разделение точек одной прямой на два класса
for i=1:N
    if X(i)  <= Y(i)
        line_class1_X(i_cl1) = X(i);
        line_class1_Y(i_cl1) = Y(i);
        i_cl1 = i_cl1 + 1;
    end;
    
    if X(i) +b > Y(i)
        line_class2_X(i_cl2) = X(i);
        line_class2_Y(i_cl2) = Y(i);
        i_cl2 = i_cl2 + 1;
    end;   
end
i_cl1;
i_cl2;


% ЛИНИЯ-образец (делит)
line_X = zeros(1, N);
line_Y = zeros(1, N);
for i=1:1:N
    line_X(i) = i/N;
    line_Y(i) = line_X(i) + b;
    if line_Y(i) < 0
        line_Y(i) = 0;
    end
end
%вывод на экран
%{
hold on;
plot(line_class1_X, line_class1_Y , '.', 'Color', [.9 .1 .1]);
plot(line_class2_X, line_class2_Y , '.', 'Color', [.1 .9 .1]);
plot(line_X, line_Y , 'Color', [.0 .0 .0]);
%}



%****************
% ПУНКТ ""В""
b= 0.5181;   % ровно половина точек


cos_class1_X = zeros(1,N/2);  % поэтому N/2
cos_class1_Y = zeros(1,N/2);
cos_class2_X = zeros(1,N/2);
cos_class2_Y = zeros(1,N/2);
i_cl1=1; i_cl2=1;
% Разделение точек синусом на два класса
for i=1:1:N
    if  0.25*cos(10*X(i)) + b < Y(i)
       cos_class1_X(i_cl1) = X(i);
        cos_class1_Y(i_cl1) = Y(i);
        i_cl1 = i_cl1 + 1;
    end;
    
    if  0.25*cos(10*X(i)) +b > Y(i)
        cos_class2_X(i_cl2) = X(i);
        cos_class2_Y(i_cl2) = Y(i);
        i_cl2 = i_cl2 + 1;
    end;   
end
i_cl1 ;
i_cl2;

% cos -  КРИВАЯ-образец (делит) 
cos_X = zeros(1, N);
cos_Y = zeros(1, N);
for i=1:1:N
    cos_X(i) = i/N;
    cos_Y(i) = 0.25*cos(10*cos_X(i)) + b;
end
% вывод на экран
%{
hold on;
plot(cos_class1_X, cos_class1_Y , '.', 'Color', [.9 .1 .1]);
plot(cos_class2_X, cos_class2_Y , '.', 'Color', [.1 .9 .1]);
plot(cos_X, cos_Y , 'Color', [.0 .0 .0]);
%}


% Перемешивание данных
% для обучения перцептрона
P=zeros(2,N);
P(1,:)=X;
P(2,:)=Y;
T=zeros(1,N);
for i=1:1:N/2
    for j=1:1:N
        %if line_class1_X(i) == X(j)        % +++++++++++++++++ ЗАМЕНА
        if cos_class1_X(i) == X(j)
            T(j) = 1;
        end
    end
end

% ПУНКТ 2
 % установка всех параметров
speed = 0.0025;          % оптимальная скорость обуч.
method = 'trainb';        % метод обучения (trainr - всегда, trainb - пакетный режим) НЕ ИСПОЛЬЗОВАТЬ trains
EP = 20;                       % оптимально кол-во эпох
lrn = 'learnp';             % один параметров перцептрона
                                     %   learnp - без нормализации + пакетный
                                     %   learnpn - с нормализацией
                                     
net = newp([0 1; 0 1], 1,'hardlim',lrn);  % Создать персептрон с двойным входом
net.IW{1,1} = rand(1, 2) ; net.b{1} = rand(1, 1) ; % вес.коэф. и коэф.смещ. - рандомные
net.trainFcn = method;             
net.trainParam.epochs = EP; 
net.trainParam.lr = speed;           
%[net, TR] = train(net,P,T);         % обучения         КОММЕНТИРОВАТЬ СТРОЧКУ ВО
                                                                         %        ВРЕМЯ ВИЗУАЛИЗАЦИИ




Y = sim(net,P);         % проверка
errors=0;
% сравнение с контрольной выборкой
for i=1:1:N
    if Y(i) ~= T(i)             
        errors = errors +1;
    end
end
errors          %количество ошибок


% ВИЗУАЛИЗАЦИЯ ОБУЧЕНИЯ         (все автоматизировано)
ep_X = [0 1];                           % массив Kx и Ky
ep_Y=zeros(1, 2);                   % массив b
ep_error = zeros (EP, 1);       % массив ошибок
                                                   % Ky * y=Kx * x+b

net = newp([0 1; 0 1], 1,'hardlim',lrn);  % Создать персептрон с двойным входом
net.IW{1,1} = rand(1, 2) ; net.b{1} = rand(1, 1) ; % вес.коэф. и смещ - рандомные
net.trainFcn = method;    
net.trainParam.epochs = 1;       % вывод на экран по одной эпохе в цикле
net.trainParam.lr = speed;         % оптимальная скорость обуч

hold on;
%plot(line_class1_X, line_class1_Y , '.', 'Color', [.9 .1 .1]);                     % +++++++++++++++++ ЗАМЕНА
%plot(line_class2_X, line_class2_Y , '.', 'Color', [.1 .9 .1]);
plot(cos_class1_X, cos_class1_Y , '.', 'Color', [.9 .1 .1]);                        
plot(cos_class2_X, cos_class2_Y , '.', 'Color', [.1 .9 .1]);

%for i=1:1:EP 
    for i=2:1:EP                           % ТОЛЬКО ДЛЯ ПАКЕТНОГО РЕЖИМА
    net.trainParam.epochs = i; 
    
    % обучения
    net = train(net,P,T);  
        
    % построение графиков
    tmp = zeros(2, 1);
    tmp (:,1) = net.IW{1,1};
    tmp_b = net.b{1};
    ep_Y(1) = tmp_b / tmp(2)
    ep_Y(2) = (tmp(1) + tmp_b) / tmp(2);
    plot(ep_X, -ep_Y );
        
    % оценка ошибки
    Y = sim(net,P);
    for j=1:1:N
        if Y(j) ~= T(j)
            ep_error(i) = ep_error(i) +1;
        end
    end
    
end
%plot(line_X, line_Y , 'Color', [.9 .0 .0]);     % +++++++++++++++++ ЗАМЕНА
plot(cos_X, cos_Y , 'Color', [.9 .0 .0]);
% массив ошибок
ep_error
% **************************** ВИЗУАЛИЗАЦИЯ окончена ********************

%}
