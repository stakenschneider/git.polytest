% ��������� ��� ������� ����������� ��������� ��������� ���
clc;
clear all;
lo = 5; 		    % ������������� ����������� �������  	
L = 10;                         % ����� �������
N = 30;                         % ����� ������
M = 33;                         % ����� �����
P = 1 / L;                      % ����������� �������� ������ �� i-�� ������
F = 0.15;                      % ����������� ���������� �������� ������
u_kan=0.5;                   % ������������� �������� ������� �� i-�� ������ 
u_pr=8;                         % ������������� ��������� ������ � ���������� 
 
% ������������� ������������ ������ � ����� ���:
u = ones(1, M);
for i = 1 : M
    if i == 1                                                          % ��������
        u(i) = N-(N-lo);                                           
    elseif i == 2                                                   % ������
        u(i) = lo;
    elseif i == 3                                                   % ��
        u(i) = u_pr;
    elseif (i >= 4) && (i <= 4 + L - 1)                 % ���
        u(i) = u_kan;
    elseif (i >= 4 + L) && (i <= 4 + 2 * L - 1)     % ��
        u(i) = 2*u_kan^(-1);
    elseif (i >= 4 + 2 * L - 1) && (i <= 4 + 3 * L - 1)  % ���
        u(i) =0.1*u_kan^(-1);
    end
end
 
% ������������� ������������� ������ ������ � ���:
w = ones(1, M);
for i = 1 : M
    if i == 1                                                           % ��������
        w(i) = 1;
    elseif i == 2                                                   % ������
        w(i) = 1;
    elseif i == 3                                                   % ��
        w(i) = 1;
    elseif (i >= 4) && (i <= 4 + L - 1)                 % ���
        w(i) = P/(1-F);
    elseif (i >= 4 + L) && (i <= 4 + 2 * L - 1)     % ��
        w(i) = (F*P)/(1-F);
    elseif (i >= 4 + 2 * L - 1) && (i <= 4 + 3 * L - 1)  % ���
        w(i) = P;
    end
end
 
% ��������� �������: (r = 2; k = 1):
G = ones(M, N + 1);
Z = ones(M, N + 1);
for r = 2 : M
    G(r, 1) = 1;
    Z(r, 1) = 1;
end
 
% ��������������� �������� ���������� G:
for r = 1 : M
    for k = 1 : N
        sum = 0;  % ��������� ���������� ��� ���������� �����
        for l = 0 : k
            mul = 1;  % ��������� ���������� ��� ���������� ������������
            for j = 1 : l
                if (r >= 4 + L) || (r == 2)  % j < m(r) [��� �������������� ��-���]
                    mul = mul * j * u(r);
                else                                % j >= m(r) [��� ������������� �����]
                    mul = mul * u(r);
                end
            end
            if r == 1
                Z(r, l + 1) = (w(r)^(l)) / mul;
                G(r, k + 1) = (w(r)^(l)) / mul;
            else
                Z(r, l + 1) = (w(r)^(l)) / mul;
                sum = sum + Z(r, l + 1) * G(r - 1, k - l + 1);
            end
        end
        %���������� ������������� ��������� � ������ G:
        G(r,k+1)=sum;   
    end
end
 
% ����� �����������:
disp('G(33, 30) = '); disp(G(33, 30));
disp('G(33, 31) = '); disp(G(33, 31));
 
%1
%������, ����������� ������ ������ � �������� ������:
P_otkaza=(G(33, 31) - 1/lo*G(33, 30))/G(33, 31);
disp('P(0, 30) = '); disp(P_otkaza);
 
%4
% ��������� ������� ������������� ������ �������, ���������� ���. ������
Lamb_sred = 5 * (1 - P_otkaza);
disp('Lamb_sred = '); disp(Lamb_sred);
 
%2
% ����������� �������� ������� ���������� ������ � �������� ������
% ������� ��������� n_sred_1 (������� ����� ������ � ������ ����-��������) :
n_sred_1 = 0; 
for n = 1 : N
    n_sred_1 = n_sred_1 + (1/5)^n * G(33, N - n + 1);
end
n_sred_1 = n_sred_1 / G(33, 31);
disp('n_sred_1 = '); disp(n_sred_1);
% ������ �������� ������� ����� �:
T_sred = (N - n_sred_1) / Lamb_sred;
disp('T_sred = '); disp(T_sred);
 
%3
% ����������� �������� ����� ������� � ��������������:
n_sred_marsh = N - n_sred_1;
disp('n_sred_marsh = '); disp(n_sred_marsh);

