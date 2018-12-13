clear all;
close all; 
clc;
format compact;


%% считаем kp и kd
close all; 

kp = 0.01
kd = 1000
numerator = [kd kp]
denumerator = [1 kd kp + 1]
W = tf(numerator, denumerator)
rlocus(W)
% step(W);

syms Wmain Wnoice p t
% 
max = 1;
min = 0;
interval = 0.002;
count = (max - min) / interval + 1;

% % Wz/p = B(p)/(p*D(p))
Wmain = (kp + kd * p) / (p * ( p * p + 1 + kd * p + kp))  ;
lapMain = vpa(ilaplace(Wmain, p, t), 20);
arrayMain = subs(lapMain, t, min:interval:max);

% 1/(p*D(p))
Wnoice = 1 /(p * ( p * p + 1 + kd *p + kp));
lapNoice = vpa(ilaplace(Wnoice, p, t), 20);
arrayNoice = subs(lapNoice, t, min:interval:max);

noice = wgn(count, 1, 0);

% РЁСѓРј
% figure;
% hold on
%     plot(min:interval:max, noice);
%     axis([min max -5 5])
% hold off
% 
% figure;
% hold on
%     plot(min:interval:max, arrayMain);
%     axis([min max 0 1.5])
% hold off

figure;
hold on
    plot(min:interval:max, arrayMain + arrayNoice * diag(noice));
    axis([min max 0 1.5])
hold off




% minReal = intmax('int64');
% minSumm = intmax('int64');
% 
% for kp = 0.01:0.01:1
%     for kd = 0.1:0.1:100
%         results = roots([1 kd kp + 1]);
%  
%         if imag(results(1)) ~= 0 || imag(results(2)) ~= 0
%             continue
%         end
%                
%         if minReal < abs(real(results(1))) && minReal < abs(real(results(2)))
%             continue
%         end
%        
%         
%         minReal = min([abs(real(results(1))) abs(real(results(2)))]);
%         
%         numerator = [kd kp];
%         denominator = [1 kd kp + 1];
%         Wz = tf(numerator, denominator, 'OutputName', 'torque', 'Variable','p');
%         
%         [y, t] = step(Wz, (0:0.01:1000)');
%         summ = sum(abs(y - 1));
%         
%         if minSumm < summ
%             continue
%         end
%         
%         minSumm = double(summ);
%         
%         resultKp = kp;
%         resultKd = kd;
%     end
% end

%% roots
kp = 0.01
% resultKp 
kd = 1000
% resultKd
numerator = [kd kp];
denominator = [1 kd kp + 1];
h = tf(numerator, denominator);
rlocus(h)

%% lle
numerator = [kd kp];
denominator = [1 kd kp + 1];
Wz = tf(numerator, denominator, 'OutputName', 'torque', 'Variable', 'p');

figure;
hold on
    bode(Wz)
hold off
% 
% %% Step function with noice.
% 
syms Wmain Wnoice p t
% 
max = 2;
min = 0;
interval = 0.002;
count = (max - min) / interval + 1;

% % Wz/p = B(p)/(p*D(p))
Wmain = (kp + kd * p) / (p * ( p * p + 1 + kd * p + kp))  ;
lapMain = vpa(ilaplace(Wmain, p, t), 20);
arrayMain = subs(lapMain, t, min:interval:max);

% 1/(p*D(p))
Wnoice = 1 /(p * ( p * p + 1 + kd *p + kp));
lapNoice = vpa(ilaplace(Wnoice, p, t), 20);
arrayNoice = subs(lapNoice, t, min:interval:max);

noice = wgn(count, 1, 0);

% РЁСѓРј
figure;
hold on
    plot(min:interval:max, noice);
    axis([min max -5 5])
hold off

figure;
hold on
    plot(min:interval:max, arrayMain);
    axis([min max 0 1.5])
hold off

figure;
hold on
    plot(min:interval:max, arrayMain + arrayNoice * diag(noice));
    axis([min max 0 1.5])
hold off




%% Roots

results = roots([1 kd kp + 1]);

im = [imag(results(1)) imag(results(2))]

re = [double(real(results(1))) double(real(results(2)))]


% %% Frequency response
 
syms f res(f)
assume(f, 'real')
assumeAlso(f >= 0)

assume(res(f), 'real')
assumeAlso(res(f) >= 0)

zn = kd * kd * f * f + ((kp - f * f + 1)^2);
re = kp* kp - kd * kd + kp + kd*kd * f * f  / zn;
im = kd * f - kd * (f ^ 3) / zn;
res(f) = simplify(sqrt(re^2 + im^2));

disp(sprintf('Bandwidth [0 - %0.10f]', double(abs(solve(res(f)==1/sqrt(2), f)))))


figure;
hold on
    ezplot(res, (0:0.0001:1000)')
    axis([0 1000 0 1.5])
hold off



% 
% figure;
% hold on
%     step(Wz, (0:0.0001:10)')
% hold off



