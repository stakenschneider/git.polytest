%РЕКУРЕНТНАЯ ПРОЦЕДУРА НАХОЖДЕНИЯ

function practic
 % Nmean - среднее количество задач в каждом узле
 % mu - интенсивность каждого узла
 % N - число заявок в сети
 % S - стоимость сети
 % M - число узлов  
 
% нахождение частот w

c = [1;1;1;1;1]; % стоимостные коэф
a = [1;1;1;1;1]; % коэф нелинейности

N = 5;            % число заявок в сети
M = 4;            % число узлов


p = [0   0.1  0.3  0.6;...
    0.2  0    0.2  0.6;...
    0.4  0.1  0    0.5;...
    0.3  0.2  0.5  0];

for j = 1:M
    test(j,1) = 0;
end
test(1,1) = 1;


w = fsolve(@wfun,test)
sum(w)

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

mu=w;

function [ lambda ] = findlambda(W,Mu)
    T = [0;0;0;0];
    Nmean = [0;0;0;0];
    
    for r=1:1:N
        for i=1:M
            T(i) = 1/Mu(i)*(1+Nmean(i)); 
        end
           
             
        s = 0;
        for i=1:M
            s = s+W(i)*T(i)/W(1);           
        end
        
        lambda = r/s;
        for i=1:M
            Nmean(i) = W(i)/W(1)*lambda*T(i);         
        end
    end 
end

S = 0;
for i = 1:M
    S = S + c(i)*mu(i)^a(i);
end
S=8;

sum(mu)
fun = @(x)(-findlambda(w,x));
% минимизация с оптимизационными параметрами, определенными в структурной опции
[my_u,fval] = fmincon(fun,mu,[],[],[1 1 1 1],S,[0;0;0;0],[],[],optimoptions('fmincon','Algorithm','sqp'))
% sum(my_u)
end