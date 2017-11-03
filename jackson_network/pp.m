function pp
% ÐÅÒÅÍÅÎÎÙÅ 
N = 5;            % ÞÉÓÌÏ ÚÁÑ×ÏË × ÓÅÔÉ
M = 4;            % ÞÉÓÌÏ ÕÚÌÏ×
S = 8;

p = [0   0.1  0.3  0.6;...
    0.2  0    0.2  0.6;...
    0.4  0.1  0    0.5;...
    0.3  0.2  0.5  0];

<<<<<<< HEAD
m = [1;1;1;1]; % ����� ������� � i� ����
c = [1;1;1;1]; % ����������� ����
MU = [0;0;0;0];
a = [1;1;1;1]; % ���� ������������
%% ��������� ������������ w
=======
m = [1;1;1;1]; % ÞÉÓÌÏ ËÁÎÁÌÏ× × iÍ ÕÚÌÅ
c = [1;1;1;1]; % ÓÔÏÉÍÏÓÔÎÙÅ ËÏÜÆ
a = [1;1;1;1]; % ËÏÜÆ ÎÅÌÉÎÅÊÎÏÓÔÉ
%% ÎÁÈÏÄÅÎÉÅ ×ÅÒÏÑÔÎÏÓÔÅÊ w
>>>>>>> c0f800e73063329207dc4dd33a03a1c76dc1189d

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

%% ÎÁÞÁÌØÎÙÅ ui  ÐÒÏÐÏÒÃÉÏÎÁÌØÎÏ wi
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

<<<<<<< HEAD
%% ��������� ����
 S = 0;
 for i = 1:M
     S = S + c(i)*MU(i)^a(i)
 end
=======
%% ÓÔÏÉÍÏÓÔØ ÓÅÔÉ
% S = 0;
% for i = 1:M
%     S = S + c(i)*MU(i)^a(i)
% end
>>>>>>> c0f800e73063329207dc4dd33a03a1c76dc1189d

%% ÏÐÔÉÍÉÚÁÃÉÑ
for i = 1:M
    lb(i,1) = 0;
end

for i = 1:M
    Aeq(i) = 1;
end

fun = @(x)(-findlambda(w,x,N));
% ÍÉÎÉÍÉÚÁÃÉÑ Ó ÏÐÔÉÍÉÚÁÃÉÏÎÎÙÍÉ ÐÁÒÁÍÅÔÒÁÍÉ, ÏÐÒÅÄÅÌÅÎÎÙÍÉ × ÓÔÒÕËÔÕÒÎÏÊ ÏÐÃÉÉ
[my_u,fval] = fmincon(fun,w,[],[],Aeq,S,lb,[],[],optimoptions('fmincon','Algorithm','sqp'))

end


   
