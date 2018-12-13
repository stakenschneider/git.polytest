function practic
N = 5;            % число заявок в сети
M = 4;            % число узлов
S = 8;
c = [1 1 1 1];                                                % стоимостные коэф
a = [1;1;1;1];   

p = [0   0.1  0.3  0.6;...
    0.2  0    0.2  0.6;...
    0.4  0.1  0    0.5;...
    0.3  0.2  0.5  0];


    
for j = 1:M
    test(j,1) = 0;
end
test(1,1) = 1;


w = fsolve(@wfun,test)

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

for i = 1:M
    lb(i,1) = 0;
end

for i = 1:M
    Aeq(i) = 1;
end
function [ctmp , ceqtmp] = limitation(x)
    
ctmp = 0;
    for i = 1:M
        ctmp = ctmp+  c(i)*x(i)^a(i);
    end
    ctmp = ctmp  - S;
    ceqtmp = [];
end

fun = @(x)(-findlambda(w,x));
[my_u,fval] = fmincon(fun,w,[],[],[],[],lb,[],@limitation,optimoptions('fmincon','Algorithm','sqp'))
end



my_u =

    1.9747
    1.2062
    2.1763
    2.6428


fval =

   -1.2195