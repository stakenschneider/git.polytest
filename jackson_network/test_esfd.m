function test_esfd
% nachal'nie znacheniya
S = 10;
N = 6;                          % chislo zayavok v seti
M = 4;                          % chislo uzlov

p = [0   0.1  0.3  0.6;...
    0.2  0    0.2  0.6;...
    0.4  0.1  0    0.5;...
    0.3  0.2  0.5  0];

m = [4;4;4;4];                   % chislo kanalov v im uzle
c = [2 3 2 2];                   % stoimostnyye koef
a = [2;1;4;3];                   % koef nelineynosti

%% nakhodeniye veroyatnostey w
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
    r=1;
    for i = 1:M
        P_old(i,1,r) = 1; %P_old = P(n-1,r-1)
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
%                   sum_1 = sum_1 + ((n-1)/u(i))*P(i,n-1,r-1);
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
            for n = 1:r
                if (n >= m(i))  
                    u(i) = m(i)*u(i);
                else
                    u(i) = n*u(i);
                end 
                
                P(i,n+1) = ((w(i)*lambda)/(w(1)*u(i)))*P_old(i,n);
%                     P(i,n,r) = ((w(i)*lambda)/(w(1)*u(i)))*P(i,n-1,r-1);
            end
            
            sum = 0;
            for n = 2:r
                sum = sum + P(i,n);
            end
        
            P(i,1,r) = 1 - sum;
        end
        P_old = P;
    end  
end

%% nelineynoye ogranicheniye
function [ctmp , ceqtmp] = limitation(x)
ctmp = 0;
    for i = 1:M
        ctmp = ctmp + c(i)*x(i)^a(i);
    end
    ctmp = ctmp  - S;
    ceqtmp = [];
end

%% optimization
for i = 1:M
    lb(i,1) = 0;
end

fun = @(x)(-findlambda(w,x));
[my_u,fval] = fmincon(fun,w,[],[],[],[],lb,[],@limitation,optimoptions('fmincon','Algorithm','sqp'))

% dlya proverki u 
% for i=1:M
%     sss(i) = c(i)*my_u(i)^a(i);
% end
% sum(sss)

end