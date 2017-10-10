clear; clc;
N=300;
% exe 1
% {
% X = rand(2, N)
% 
% fid = fopen('my_file.dat', 'wb');     % �������� ����� �� ������ 
% if fid == -1                     % �������� ������������ �������� 
%     error('File is not opened'); 
% end 
% 
% fwrite(fid, X, 'double');   % ������ ������� � ���� (40 ����) 
% fclose(fid);                % �������� ����� 
% }
fid = fopen('my.txt', 'rb');     % �������� ����� �� ������ 
if fid == -1                     % �������� ������������ �������� 
    error('File is not opened'); 
end 
 
X = zeros(N);
Y = zeros(N);
X = fread(fid, N, 'double');     % ������ 5 �������� double 
Y = fread(fid, N, 'double');

fclose(fid);                     % �������� �����
%plot(X,Y ,'.');




%****************
% ����� ""�""
b= -0.0072;   % ����� �������� �����
line_class1_X = zeros(1,N/2);  % ������� N/2
line_class1_Y = zeros(1,N/2);
line_class2_X = zeros(1,N/2);
line_class2_Y = zeros(1,N/2);
i_cl1=1; i_cl2=1;
% ���������� ����� ����� ������ �� ��� ������
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


% �����-������� (�����)
line_X = zeros(1, N);
line_Y = zeros(1, N);
for i=1:1:N
    line_X(i) = i/N;
    line_Y(i) = line_X(i) + b;
    if line_Y(i) < 0
        line_Y(i) = 0;
    end
end
%����� �� �����
%{
hold on;
plot(line_class1_X, line_class1_Y , '.', 'Color', [.9 .1 .1]);
plot(line_class2_X, line_class2_Y , '.', 'Color', [.1 .9 .1]);
plot(line_X, line_Y , 'Color', [.0 .0 .0]);
%}



%****************
% ����� ""�""
b= 0.5181;   % ����� �������� �����


cos_class1_X = zeros(1,N/2);  % ������� N/2
cos_class1_Y = zeros(1,N/2);
cos_class2_X = zeros(1,N/2);
cos_class2_Y = zeros(1,N/2);
i_cl1=1; i_cl2=1;
% ���������� ����� ������� �� ��� ������
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

% cos -  ������-������� (�����) 
cos_X = zeros(1, N);
cos_Y = zeros(1, N);
for i=1:1:N
    cos_X(i) = i/N;
    cos_Y(i) = 0.25*cos(10*cos_X(i)) + b;
end
% ����� �� �����
%{
hold on;
plot(cos_class1_X, cos_class1_Y , '.', 'Color', [.9 .1 .1]);
plot(cos_class2_X, cos_class2_Y , '.', 'Color', [.1 .9 .1]);
plot(cos_X, cos_Y , 'Color', [.0 .0 .0]);
%}


% ������������� ������
% ��� �������� �����������
P=zeros(2,N);
P(1,:)=X;
P(2,:)=Y;
T=zeros(1,N);
for i=1:1:N/2
    for j=1:1:N
        %if line_class1_X(i) == X(j)        % +++++++++++++++++ ������
        if cos_class1_X(i) == X(j)
            T(j) = 1;
        end
    end
end

% ����� 2
 % ��������� ���� ����������
speed = 0.0025;          % ����������� �������� ����.
method = 'trainb';        % ����� �������� (trainr - ������, trainb - �������� �����) �� ������������ trains
EP = 20;                       % ���������� ���-�� ����
lrn = 'learnp';             % ���� ���������� �����������
                                     %   learnp - ��� ������������ + ��������
                                     %   learnpn - � �������������
                                     
net = newp([0 1; 0 1], 1,'hardlim',lrn);  % ������� ���������� � ������� ������
net.IW{1,1} = rand(1, 2) ; net.b{1} = rand(1, 1) ; % ���.����. � ����.����. - ���������
net.trainFcn = method;             
net.trainParam.epochs = EP; 
net.trainParam.lr = speed;           
%[net, TR] = train(net,P,T);         % ��������         �������������� ������� ��
                                                                         %        ����� ������������




Y = sim(net,P);         % ��������
errors=0;
% ��������� � ����������� ��������
for i=1:1:N
    if Y(i) ~= T(i)             
        errors = errors +1;
    end
end
errors          %���������� ������


% ������������ ��������         (��� ����������������)
ep_X = [0 1];                           % ������ Kx � Ky
ep_Y=zeros(1, 2);                   % ������ b
ep_error = zeros (EP, 1);       % ������ ������
                                                   % Ky * y=Kx * x+b

net = newp([0 1; 0 1], 1,'hardlim',lrn);  % ������� ���������� � ������� ������
net.IW{1,1} = rand(1, 2) ; net.b{1} = rand(1, 1) ; % ���.����. � ���� - ���������
net.trainFcn = method;    
net.trainParam.epochs = 1;       % ����� �� ����� �� ����� ����� � �����
net.trainParam.lr = speed;         % ����������� �������� ����

hold on;
%plot(line_class1_X, line_class1_Y , '.', 'Color', [.9 .1 .1]);                     % +++++++++++++++++ ������
%plot(line_class2_X, line_class2_Y , '.', 'Color', [.1 .9 .1]);
plot(cos_class1_X, cos_class1_Y , '.', 'Color', [.9 .1 .1]);                        
plot(cos_class2_X, cos_class2_Y , '.', 'Color', [.1 .9 .1]);

%for i=1:1:EP 
    for i=2:1:EP                           % ������ ��� ��������� ������
    net.trainParam.epochs = i; 
    
    % ��������
    net = train(net,P,T);  
        
    % ���������� ��������
    tmp = zeros(2, 1);
    tmp (:,1) = net.IW{1,1};
    tmp_b = net.b{1};
    ep_Y(1) = tmp_b / tmp(2)
    ep_Y(2) = (tmp(1) + tmp_b) / tmp(2);
    plot(ep_X, -ep_Y );
        
    % ������ ������
    Y = sim(net,P);
    for j=1:1:N
        if Y(j) ~= T(j)
            ep_error(i) = ep_error(i) +1;
        end
    end
    
end
%plot(line_X, line_Y , 'Color', [.9 .0 .0]);     % +++++++++++++++++ ������
plot(cos_X, cos_Y , 'Color', [.9 .0 .0]);
% ������ ������
ep_error
% **************************** ������������ �������� ********************

%}
