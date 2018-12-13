function test_2
% переменные 
N = 5;            % число заявок в сети
M = 4;            % число узлов
S = 8;

p = [0   0.1  0.3  0.6;...
    0.2  0    0.2  0.6;...
    0.4  0.1  0    0.5;...
    0.3  0.2  0.5  0];

m = [1;1;1;1]; % число каналов в iм узле
c = [1;1;1;1]; % стоимостные коэф
MU = [0;0;0;0];
a = [1;1;1;1]; % коэф нелинейности
%% находение вероятностей w

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
        F = [F(1); F(2); F(3); F(4); sum(w) - 1];
    end

%% начальные ui  пропорционально wi
% for i = 1:M
%     u(i) = S*w(i);
% end

%  findlambda(w,u,N);

%% find lambda
function [ lambda ] = findlambda(w,u,N)

    for l = 1:M
        P_old(l,1) = 1;
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
            s = s + w(i)*t(i)/w(1);  
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
                P(i,n+1) = (w(i)*lambda)/(w(1)*u(i))*P_old(i,n);
            end
            
            sum = 0;
            for n = 1:r
                sum = sum + P(i,n);
            end
        
            for n = 1:r
                P(1,n) = 1 - sum;
            end  
        end
        P_old = P;
        MU=u;
    end  
end

%% стоимость сети
 S = 0;
 for i = 1:M
     S = S + c(i)*MU(i)^a(i)
 end

%% оптимизация
for i = 1:M
    lb(i,1) = 0;
end

for i = 1:M
    Aeq(i) = 1;
end

fun = @(x)(-findlambda(w,x,N));
% минимизация с оптимизационными параметрами, определенными в структурной опции
[my_u,fval] = fmincon(fun,w,[],[],Aeq,S,lb,[],[],optimoptions('fmincon','Algorithm','sqp'))

end


   