function test_2
% переменные 
S = 8;
N = 5;                                                          % число заявок в сети
M = 4;                                                          % число узлов

p = [0   0.1  0.3  0.6;...
    0.2  0    0.2  0.6;...
    0.4  0.1  0    0.5;...
    0.3  0.2  0.5  0];

m = [1;1;1;1];                                               % число каналов в iм узле
c = [1 1 1 1];                                               % стоимостные коэф
a = [1;1;1;1];                                               % коэф нелинейности

%% находение вероятностей w
for j = 1:M
    test(j,1) = 0;
end
test(1,1) = 1;

w = fsolve(@wfun,test);

    function F = wfun(w)
        for j = 1:M
            sum_t = 0;
            for i = 1:M
                sum_t = sum_t + w(i)*p(i,j);
            end
            F(j) = sum_t - w(j);
            my = j;
        end
        
        for j=my:10
            F(j) = 0;
        end
        F = [F(1); F(2); F(3); F(4); F(5); F(6); F(7); F(8); F(9); F(10); sum(w) - 1];
    end

%% find lambda
function [ lambda ] = findlambda(w,u)
    for i = 1:M
        P_old(i,1) = 1;
    end
        
    for r = 1:N
        
        % step 1
        for i = 1:M
            sum_1 = 0;
            for n = 1:r
                if (n >= m(i))  
                    u(i) = m(i)*u(i);
                else
                    u(i) = n*u(i);
                end
               
                sum_1 = sum_1 + (n/u(i))*P_old(i,n);
            end
          t(i) = sum_1;
        end

        % step 2
        s=0;
        for i = 1:M
            s = s + (w(i)*t(i))/w(1);  
        end
        lambda = r/s;
    
        % step 3
        for i = 1:M   
            if (n >= m(i))  
                u(i) = m(i)*u(i);
            else
                u(i) = n*u(i);
            end
            
            for n = 1:r
                P(i,n+1) = ((w(i)*lambda)/(w(1)*u(i)))*P_old(i,n);
            end
            
            sum = 0;
            for n = 1:r
                sum = sum + P(i,n);
            end
        
            P(i,1) = 1 - sum;

        end
        P_old = P;
    end  
end

%% нелинейное ограничение
function [ctmp , ceqtmp] = limitation(x)
ctmp = 0;
    for i = 1:M
        ctmp = ctmp + c(i)*x(i)^a(i);
    end
    ctmp = ctmp  - S;
    ceqtmp = [];
end

%% оптимизация
for i = 1:M
    lb(i,1) = 0;
end

fun = @(x)(-findlambda(w,x));
% минимизация с оптимизационными параметрами, определенными в структурной опции
[my_u,fval] = fmincon(fun,w,[],[],[],[],lb,[],@limitation,optimoptions('fmincon','Algorithm','sqp'))

end