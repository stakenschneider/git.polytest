% עוûומיו ף נןםן‎רא םופןהב נןףלוהןקבפולרמשט נעיגליצומיך

function practic_2
S = 8;
% w = fsolve(fun,[1;0;0;0]);
w = [0.2425 ; 0.1234 ; 0.2763 ; 0.3577];
mu = w;

%recalculate Nmean(mean count of tasks in every node) using given mu
Nmean = findnmean(w,mu,5);
Nmean1 = findnmean(w,mu,4);
 
%recalcutate mu(intensity of every node) using given Nmean   b
mu = findmu(S,Nmean,Nmean1)

function [ Nmean ] = findnmean(W,Mu,N )
%find Nmean using given Mu vector
    T = [0;0;0;0];
    Nmean = [0;0;0;0];
    for r=1:1:N
        for i=1:1:4
            T(i) = 1/Mu(i)*(1+Nmean(i));            
        end
        s = 0;
        for i=1:1:4
            s = s+W(i)*T(i)/W(1);           
        end
        lambda1 = r/s;
        for i=1:1:4
            Nmean(i) = W(i)/W(1)*lambda1*T(i);           
        end
    end   
    
end

function [ Mu ] = findmu( S,Nmean,Nmean1 )
   ss = 0;
    Mu = [0;0;0;0];
    for i=2:1:4
       ss = ss+(Nmean(i)-Nmean1(i));
    end
    Mu(1) = S/(1+ss/(Nmean(1)-Nmean1(1)));
    for i=2:1:4
       Mu(i) = Mu(1)*(Nmean(i)-Nmean1(i))/(Nmean(1)-Nmean1(1));
    end
end

function [ lambda ] = findlambda(W,Mu,N)
    T = [0;0;0;0];
    Nmean = [0;0;0;0];
    
    for r=1:1:N
        for i=1:1:4
            T(i) = 1/Mu(i)*(1+Nmean(i)); 
        end
           
             
        s = 0;
        for i=1:1:4
            s = s+W(i)*T(i)/W(1);           
        end
        lambda = r/s;
        for i=1:1:4
            Nmean(i) = W(i)/W(1)*lambda*T(i);           
        end
    end 
end

Aeq = [1 1 1 1];
beq = S;
fun = @(x)(-findlambda(w,x,5));
[xmu,fval] = fmincon(fun,mu,[],[],Aeq,beq,[0;0;0;0],[],[],optimoptions('fmincon','Algorithm','sqp'))

mu = xmu;
mu0 = mu;
Nmean = findnmean(w,mu,5);
Nmean1 = findnmean(w,mu,4);
mu = findmu(S,Nmean,Nmean1);
err = max(abs(mu-mu0))

end
