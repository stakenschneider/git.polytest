function test_2
% S = 8;            % ��������� ����
remember = 0;
lambda = 0;

N = 5;            % ����� ������ � ����
M = 4;            % ����� �����

p = [0  0.1 0.3 0.6;...
    0.2 0   0.2 0.6;...
    0.4 0.1 0   0.5;...
    0.3 0.2 0.5 0];

P = [1 0 ;...
    0 0 ;...
    0 0 ;...
    0 0 ];

m = [2;3;4;5]; % ����� ������� � i� ����
c = [1;1;1;1]; % ����������� ����
a = [1;1;1;1]; % ���� ������������

t = [0;0;0;0]; % ����� ���������� � ����
P = [0;0;0;0]; % ����� ���-���
w = [0;0;0;0]; % �������
u = [0;0;0;0]; % ������������� ������� ����
% n = [0;0;0;0]; % ���������� ������ � ����  

%% ��������� ������������ w

w = fsolve(@wfun,[1;0;0;0]); 
sum(w);

    function F = wfun(w)
        for j = 1:M
            er = 0;
            for i = 1:M
                er = er + w(i)*p(i,j);
            end
            F(j) = er - w(j);
        end
        F = [F(1); F(2); F(3); F(4) ; sum(w) - 1];
    end

mu = w;

%% find lambda
function [ lambda ] = findlambda(w,u)
    for r = 1:N
        % step 1
        for i=1:M
            er = 0;
            for n = 1:r
                if (n < m(i))  
                    u(i) = n*u(i);
                else
                    u(i) = m(i)*u(i);
                end
            er = er + n/u(i)*P(n,r);     
            end
          t(i) = er;
        end

        % step 2
        s=0;
        for i=1:M
            s = s+w(i)*t(i)/w(1);  
        end
        lambda = r/s;
    
        % step 3
        
        P = fsolve(@Pfun,[1 0 0 0])
        remember = r;
    end  
end

%% ��� ������� i��� ���� ������� ����� ���-���
function F = Pfun(P) 
    for i = 1:M            
        for n = 1:remember
            if (n < m(i))  
                u(i) = n*u(i);
            else
                u(i) = m(i)*u(i);
            end
            
            F(1) = - P(n,remember) + (w(i)*lambda)/(w(1)*u(i))*P(n-1,remember-1);
        end
            
        sum = 0;
        for k = 1:remember
            sum = sum + P(n,remember);
        end
        
        for n = 0:remember
            F(2) = - P(0,remember) + 1 - sum;
        end  
            F = [F(1); F(2)];
    end

end

%% ��������� ����
S = 0;
for i = 1:M
    S = S + c(i)*u(i)^a(i);
end

%% �����������
fun = @(x)(-findlambda(w,x));
% ����������� � ���������������� �����������, ������������� � ����������� �����
% [my_u,fval] = fmincon(fun,mu,[],[],[1 1 1 1],S,[0;0;0;0],[],[],optimoptions('fmincon','Algorithm','sqp'))

end

   