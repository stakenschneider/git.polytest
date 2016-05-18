% 1.1
% Считывание выборки Х из файла sample.txt
X=load('sample.txt');
N=16200;
n=10;
% Перемешиваниевыборки
Xperm=X(randperm(length(X)));
% Формированиеподвыборок
for i=1:n
Xpodv(i,1:1620)=Xperm((1+(i-1)*N/10):1:(i*N/10));
end
 
% 1.2.1
% Построение выборочной функции распределения F(x)
% График ступенчатый, чтобы убедиться - увеличить
figure
cdfplot(X)
title('Функция распределения');
 
% 1.2.2
% Построение абсолютной и относительной гистограммы
figure
hist(X,20)
title('Абсолютная гистограмма');
xlabel('x');
ylabel('p(x)');
 
% Создание переменных H и G, в которых содержится количество
% значений в интервале и положение самих интервалов.
[H,G] = hist(X,20);
sum=0;
for i=1:n
sum=sum+H(i)*G(i);
end
figure
bar(G, H/sum, 1)
 
 
% plot(
title('Относительная гистограмма')
xlabel('x');
ylabel('p(x)');
 
 
 
 
 
 
 
 
% 1.3
% Определение точечных оценок:
 
disp('Cреднее арифметическое:');
meanX=mean(X);
disp(meanX);
disp('Медиана:');
medianX=median(X);
disp(medianX);
disp('Размахвыборки:');
rangeX=range(X);
disp(rangeX);
disp('Середина размаха выборки:');
meanRangeX=range(X)/2;
disp(meanRangeX);
 
% центральных моментов
disp('Точечная оценка центральных моментов:');
disp('Второго: ');
centrMoment2=moment(X,2);
disp(centrMoment2);
disp('Третьего: ');
centrMoment3=moment(X,3);
disp(centrMoment3);
disp('Четвертого: ');
centrMoment4=moment(X,4);
disp(centrMoment4);
disp('Определение моды (максимум на графике плотности)');
modeX=mode(X);
disp(modeX);
 
% асимметрии и эксцесса
disp('Точечная оценка асимметрии и эксцесса:');
disp('Асимметрия: ');
skewnessX=skewness(X);
disp(skewnessX);
disp('Эксцесс');
kurtosisX = kurtosis(X);
disp(kurtosisX);
 
disp('Среднеквадратичное отклонение');
stdX=std(X);
disp(stdX);
disp('Дисперсия:');
varX=var(X);
disp(varX);
 
% границ интерквантильного промежутка Jp для Р=0.95 для полной выборки
P=0.95;
disp('Левая/правая граница интерквантильного промежутка Jp для Р=0.95 для полной выборки:');
leftQuantileX=quantile(X,(1-P)/2);
disp(leftQuantileX);
rightQuantileX=quantile(X,(1+P)/2);
disp(rightQuantileX);
 
% 1.3.4
% Характеристики по пп. 1.3.1-1.3.2 по сформированным в п. 1.1 подвыборкам
for i=1:n
disp('Характеристики для подвыборки №');
disp(i);
uniqueXpodv=unique(Xpodv(i,1:1620));
 
disp('Среднееарифметическое:');
meanXpodv(i)=mean(Xpodv(i,1:1620));
disp(meanXpodv(i));    
disp('Медиана:')
medianXpodv(i)=median(Xpodv(i,1:1620));
disp(medianXpodv(i));
disp('Размахвыборки:');
rangeXpodv(i)=range(Xpodv(i,1:1620));
disp(rangeXpodv(i));
disp('Серединаразмахавыборки:');
averageRangeXpodv(i)=rangeXpodv(i)/2;
disp(averageRangeXpodv(i));
 
 
 
disp('Второй центральный момент: ');
samplecentrMoment2(i)=moment(Xpodv(i,1:1620),2);
disp(samplecentrMoment2(i));
disp('Третийцентральныймомент: ');
samplecentrMoment3(i)=moment(Xpodv(i,1:1620),3);
disp(samplecentrMoment3(i));
disp('Четвертый центральный момент: ');
samplecentrMoment4(i)=moment(Xpodv(i,1:1620),4);
disp(samplecentrMoment4(i));
disp('Определение моды (максимум на графике плотности)');
sampleModeX(i)=mode(Xpodv(i,1:1620));
disp(sampleModeX(i));
 
disp('Асимметрия: ');    
skewnessXpodv(i)=skewness(Xpodv(i,1:1620));
disp(skewnessXpodv(i));
disp('Эксцесс: ');
kurtosisXpodv(i)=kurtosis(Xpodv(i,1:1620));
disp(kurtosisXpodv(i));
 
disp('Cреднееквадратическое отклонение:');
stdXpodv(i)=std(Xpodv(i,1:1620));
disp(stdXpodv(i));
disp('Дисперсия:');
varXpodv(i)=var(Xpodv(i,1:1620));
disp(varXpodv(i));
end;
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
%Построение графиков точечных оценок
figure
subplot(5,2,1)
plot(meanXpodv,1,'-b+',meanX,1,'-rd')
title('Cреднее арифметическое')
subplot(5,2,3)
plot(medianXpodv,1,'-b+',medianX,1,'-rd')
title('Медиана')
subplot(5,2,5)
plot(averageRangeXpodv,1,'-b+',meanRangeX,1,'-rd')
title('Середина размаха выборки')
subplot(5,2,2)
plot(varXpodv,1,'-b+',varX,1,'-rd')
title('Оценка дисперсии')
subplot(5,2,4)
plot(stdXpodv,1,'-b+',stdX,1,'-rd')
title('Оценка среднеквадратичного отклонения')
subplot(5,2,7)
plot(samplecentrMoment3,1,'-b+',centrMoment3,1,'-rd')
title('Оценка третьего центрального момента')
subplot(5,2,9)
plot(samplecentrMoment4,1,'-b+',centrMoment4,1,'-rd')
title('Оценка четвертого центрального момента')
subplot(5,2,8)
plot(skewnessXpodv,1,'-b+',skewnessX,1,'-rd')
title('Оценка асимметрии')
subplot(5,2,10)
plot(kurtosisXpodv,1,'-b+',kurtosisX,1,'-rd')
title('Оценка эксцесса')
subplot(5,2,6)
plot(sampleModeX,1,'-b+',modeX,1,'-rd')
title('Оценка моды (максимума на графике плотности)')
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
% 1.4
% Определение интервальных оценок с доверительной вероятностью Q=0.8
% Доверительная вероятность
Q=0.8;
% -первого начального и второго центрального моментов для полной выборки
disp('Интервальная оценка первого начального и второго центрального моментов с доверительной вероятностью Q=0.8 для полной выборки');
% --интервальная оценка первого начального момента
disp('Интервальная оценка первого начального момента');
disp('Левая/правая граница:');
leftStdX=meanX-stdX/sqrt(N)*tinv((1+Q)/2, N-1);
disp(leftStdX);
rightStdX=meanX+stdX/sqrt(N)*tinv((1+Q)/2, N-1);
disp(rightStdX);
% --интервальная оценка второго центрального момента
disp('Интервальная оценка второго центрального момента');
disp('Левая/правая граница:');
leftVarX=varX*(N-1)/chi2inv((1+Q)/2, N-1);
disp(leftVarX);
rightVarX=varX*(N-1)/chi2inv((1-Q)/2, N-1);
disp(rightVarX);
 
% -первого начального и второго центрального моментов для подвыборок
disp('Интервальная оценка первого начального и второго центрального моментов с доверительной вероятностью Q=0.8 для подвыборок');
for i=1:n
disp('Выборка №');
disp(i);
% --интервальная оценка первого начального момента
disp('Интервальная оценка первого начального момента');
disp('Левая/правая граница:');
leftStdXpodv(i)=meanXpodv(i)-stdX/sqrt(N/10)*tinv((1+Q)/2, N/10-1);
disp(leftStdXpodv(i));
rightStdXpodv(i)=meanXpodv(i)+stdX/sqrt(N/10)*tinv((1+Q)/2, N/10-1);
disp(rightStdXpodv(i));
% --интервальная оценка второго центрального момента
disp('Интервальная оценка второго центрального момента');
disp('Левая/правая граница:');
leftVarXpodv(i)=varXpodv(i)*(N/10-1)/chi2inv((1+Q)/2, N/10-1);
disp(leftVarXpodv(i));
rightVarXpodv(i)=varXpodv(i)*(N/10-1)/chi2inv((1-Q)/2, N/10-1);
disp(rightVarXpodv(i));
end
 
% -интерквантильного промежутка J для P=0.95
% --по всей выборке с помощью непараметрических толерантных пределов,
%   симметричных относительно среднего арифметического
k=0;
res=0;
vectvar=sort(X);
while res<=(1-Q)
res=0;
     m=N-k;
for i=m:N
res=res+nchoosek(N,m)*P^m*(1-P)^(N-m);
end
     k=k+1;
end
disp('Интервальная оценка интерквантильного промежутка J для P=0.95 для полной выборки относительно среднего арифметического');
disp('Количество статистически эквивалентных блоков: ');
disp(k);
disp('Левая граница: ');
leftQuantMean=vectvar((k-1)/2);
disp(leftQuantMean);
disp('Праваяграница: ');
rightQuantMean=vectvar(N-(k-1)/2);
disp(rightQuantMean);
 
figure
subplot(2,1,1)
plot(leftQuantileX,1,'ro',rightQuantileX,1,'ro',leftQuantMean,1,'bx',rightQuantMean,1,'bx')
title('Интервальные оценки для интерквантильного промежутка по всей выборке относительно среднего арифметического')
% В качестве непараметрических пределов имеем rightQuantileX-leftQuantileX
% и rightQuantMean-leftQuantMean
 
% -интерквантильноо промежутка J для P=0.95
% --по всей выборке с помощью непараметрических толерантных пределов,
%   симметричных относительно нуля
disp('Интервальная оценка интерквантильного промежутка J для P=0.95 для полной выборки относительно нуля');
disp('Левая граница');
leftQuantZero=leftQuantileX-meanX;
disp(leftQuantZero);
disp('Праваяграница');
rightQuantZero=rightQuantileX-meanX;
disp(rightQuantZero);
 
subplot(2,1,2)
plot(leftQuantZero,1,'ro',rightQuantZero,1,'ro',leftQuantMean,1,'bx',rightQuantMean,1,'bx')
title('Интервальная оценка интерквантильного промежутка J для P=0.95 для полной выборки относительно нуля')
 
disp('Толерантный множитель k=2.375');
 
disp('Интервальная оценка интерквантильного промежутка с помощью параметрических толерантных пределов для подвыборок');
for i=1:n
disp('Подвыборка №');
disp(i);
%Относительно среднего арифметического
disp('Левая граница');
leftQuantParamX(i)=meanXpodv(i)-2.375*stdXpodv(i);
disp(leftQuantParamX(i));
disp('Праваяграница');
rightQuantParamX(i)=meanXpodv(i)+2.375*stdXpodv(i);
disp(rightQuantParamX(i));
%Относительнонуля
rightQuantParamXzero(i)=leftQuantParamX(i)-meanXpodv(i);
rightQuantParamXzero(i)=rightQuantParamX(i)-meanXpodv(i);    
end;
 
figure
plot(leftQuantParamX,1,'bx', rightQuantParamX, 1, 'mx', leftQuantileX, 1, 'rd', rightQuantileX, 1, 'rd');
title('Интервальная оценка всех промежутков с толерантными пределами и интерквантильного промежутка');
figure
plot(leftQuantParamX,1,'bx', leftQuantileX, 1, 'rd');
title('Интервальная оценка всех левых границ промежутков с толерантными пределами и интерквантильного промежутка');
figure
plot(rightQuantParamX,1,'bx', rightQuantileX, 1, 'rd');
title('Интервальная оценка всех правых границ промежутков с толерантными пределами и интерквантильного промежутка');
 
for i = 1:10
tempmas(i)=i;%массив 1-10 для построения графиков
end;
figure
subplot(3,1,1)
plot(leftStdXpodv,tempmas,'ro',rightStdXpodv,tempmas,'ro',leftStdX,0,'bx',rightStdX,0,'bx')
title('Интервальные оценки первого начального момента')
grid on
subplot(3,1,2)
plot(leftVarXpodv,tempmas,'ro',rightVarXpodv,tempmas,'ro',leftVarX,0,'bx',rightVarX,0,'bx')
title('Интервальные оценки второго центрального момента')
grid on
 
%  
%  
% % 2.2 Определение параметров теоретических распределений
% % -с помощью метода моментов
% disp('Определение параметров равномерного распределения: ');
% disp('- методом моментов');
% mmparametr(1)=0.0020978;
% mmparametr(2)=0.242899;
% disp('Параметр 1: ');
% disp(mmparametr(1));
% disp('Параметр 2: ');
% disp(mmparametr(2));
% 
% % -с помощью метода максимального правдоподобия
% disp('- методом максимального правдоподобия:');
% % Численная оценка
% [mmpchparametr(1),mmpchparametr(2)]=lognfit(X);
% disp('-- численное решение: ');
% disp('Параметр 1: ');
% disp(mmpchparametr(1));
% disp('Параметр 2: ');
% disp(mmpchparametr(2));
% 
% % Аналитическая оценка
% mmpanalparametr(1)=min(X);
% mmpanalparametr(2)=max(X);
% disp('-- аналитическое решение: ');
% disp('Параметр 1: ');
% disp(mmpanalparametr(1));
% disp('Параметр 2: ');
% disp(mmpanalparametr(2));
%  
% figure
% cdfplot(X);
% hold on
% plot(mmparametr, [0 1],'r', mmpchparametr, [0 1], 'y',mmpanalparametr,[0 1],'m')
% legend('Эмпирическая функция распределения','Теор. зав-ть для оценки параметров по методу моментов','Теор. зав-ть для оценки параметров по численному ММП','Теор. зав-ть для оценки параметров по аналитическому ММП');
% title('Равномерное распределение');
%  
% figure
% hist(X);
% hold on;
% f1=logncdf(X);
% plot(X,f1, '-.r');
%  
%  
 
 
 
 
 
 
 
 
 
% 2.3 Проверка гипотез относительно выбранных законов распределения
disp('Проверка гипотез относительно выбранных законов распределения');
disp('Равномерное распределение');
disp('Критерий Хи-квадрат.Критическое значение: ');
U=unique(X);
P = calcProbability(calcRepetitions(X,U),N);
PR=zeros(1,N);
for i=1:N
if X(i) == U(1)
PR(i) = P(j);
end;
end;
sum=0;
for i=1:N
sum=sum+((1*PR(i)-X(i)^2)/1);
end
x = sort(X);
n=length(x);
xmin = x(1);
xmax = x(n);
k = round(n^0.5 );
d = (xmax-xmin)/k;
xk(1) = xmin;
for i = 2:(k+1)
xk(i) = xk(i-1)+ d;
end;
for i = 2:length(xk);
pk(i-1) = logncdf(xk(i),100,500) - logncdf(xk(i-1),100,500);
end;
nk = hist(x,k);
chi2 = 0;
for i = 1:k
if(pk(i)~=0)
    chi2 = chi2 + (nk(i) - n*pk(i))^2/(n*pk(i));
end;
end;
disp(chi2);
disp('Статистика');
deadline=chi2inv(0.95,(length(xk) - 1) - 1);
disp(deadline);
disp('Нормальное распределение');
disp('Критерий Хи-квадрат.Критическое значение: ');
U=unique(X);
P = calcProbability(calcRepetitions(X,U),N);
PR=zeros(1,N);
for i=1:N
if X(i) == U(1)
PR(i) = P(j);
end;
end;
sum=0;
for i=1:N
sum=sum+((1*PR(i)-X(i)^2)/1);
end
x = sort(X);
n=length(x);
xmin = x(1);
xmax = x(n);
k = round(n^0.5 );
d = (xmax-xmin)/k;
xk(1) = xmin;
for i = 2:(k+1)
xk(i) = xk(i-1)+ d;
end;
for i = 2:length(xk);
pk(i-1) = normcdf(xk(i)) - normcdf(xk(i-1));
end;
nk = hist(x,k);
chi2 = 0;
for i = 1:k
if(pk(i)~=0)
    chi2 = chi2 + (nk(i) - n*pk(i))^2/(n*pk(i));
end;
end;
disp(chi2);
disp('Статистика');
deadline=chi2inv(0.95,(length(xk) - 1) - 1);
disp(deadline);
disp('Гамма-распределение');
disp('Критерий Хи-квадрат.Критическое значение: ');
U=unique(X);
P = calcProbability(calcRepetitions(X,U),N);
PR=zeros(1,N);
for i=1:N
if X(i) == U(1)
PR(i) = P(j);
end;
end;
sum=0;
for i=1:N
sum=sum+((1*PR(i)-X(i)^2)/1);
end
x = sort(X);
n=length(x);
xmin = x(1);
xmax = x(n);
k = round(n^0.5 );
d = (xmax-xmin)/k;
xk(1) = xmin;
for i = 2:(k+1)
xk(i) = xk(i-1)+ d;
end;
for i = 2:length(xk);
pk(i-1) = gamcdf(xk(i),100,500) - gamcdf(xk(i-1),100,500);
end;
nk = hist(x,k);
chi2 = 0;
for i = 1:k
if(pk(i)~=0)
    chi2 = chi2 + (nk(i) - n*pk(i))^2/(n*pk(i));
end;
end;
disp(chi2);
disp('Статистика');
deadline=chi2inv(0.95,(length(xk) - 1) - 1);
disp(deadline);
 
 
 
%   Критерий Колмогорова-Смирнова
disp('Критерий Колмогорова-Смирнова');
disp('-Распределение Лапласа');
disp('--Критерий: ');
disp(kstest2(Xpodv(1,1:100),logncdf(Xpodv(1,1:100),100,500),0.05));
disp('--Статистика');
disp(0.134);
 
disp('-Гамма-распределение');
disp('--Критерий: ');
disp(kstest2(Xpodv(1,1:100),gamcdf(Xpodv(1,1:100),100,500),0.05));
disp('--Статистика');
disp(0.134);
 
disp('-Нормальное распределение');
disp('--Критерий: ');
disp(kstest2(Xpodv(1,1:100),normcdf(Xpodv(1,1:100)),0.05));
disp('--Статистика');
disp(0.134);
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
%Критерий Мизеса
disp('Критерий Мизеса');
n=length(U);
p_i = 1-0.8^n;
n=length(Xpodv(1,1:1620))
sum = 0;
for i=1:(n-1)
sum = sum + (F(i) - (2*i-1)/(2*n))^2;
end;
disp('-Равномерное распределение');
disp('--Критерий: ');
nw21 = n*(1/(12*N^2)+1/N*sum);
disp(nw21);
disp('--Статистика');
disp(0.4614);
 
disp('-Нормальное распределение');
for i=1:n
   d_i(i)=1/(2*pi*stdXpodv(1))*exp(-(Xpodv(1,i)-meanX)^2/(2*varX));
end;
sum = 0;
for i=1:(n-1)
sum = sum + (d_i(i) - (2*i-1)/(2*n))^2;
end;
disp('--Критерий: ');
nw22 = n*(1/(12*N^2)+1/N*sum);
disp(nw22);
disp('--Статистика');
disp(0.4614);
 
disp('-Гамма-распределение');
for i=1:n
   g_i(i)=Xpodv(1,i)^(0.5-1)*exp(-Xpodv(1,i)/(6.832*sqrt(pi)));
end;
sum = 0;
fori=1:(n-1)
sum = sum + (g_i(i) - (2*i-1)/(2*n))^2;
END;

disp('--Критерий: ');
nw22 = n*(1/(12*N^2)+1/N*sum);
disp(nw22);
disp('--Статистика');
disp(0.2415);
 
 
%Критерий Мизеса
disp('Критерий Мизеса');
n=length(U);
p_i = 1-0.8^n;
n=length(Xpodv(1,1:1620))
sum = 0;
for i=1:(n-1)
sum = sum + (logncdf(i) - (2*i-1)/(2*n))^2;
end;
disp('-Равномерное распределение');
disp('--Критерий: ');
nw21 = n*(1/(12*N^2)+1/N*sum);
disp(nw21);
disp('--Статистика');
disp(0.4614);
 
disp('-Нормальное распределение');
for i=1:n
   d_i(i)=1/(2*pi*stdXpodv(1))*exp(-(Xpodv(1,i)-meanX)^2/(2*varX));
end;
sum = 0;
for i=1:(n-1)
sum = sum + (d_i(i) - (2*i-1)/(2*n))^2;
end;
disp('--Критерий: ');
nw22 = n*(1/(12*N^2)+1/N*sum);
disp(nw22);
disp('--Статистика');
disp(0.4614);
 
disp('-Гамма-распределение');
for i=1:n
   g_i(i)=Xpodv(1,i)^(0.5-1)*exp(-Xpodv(1,i)/(6.832*sqrt(pi)));
end;
sum = 0;
for i=1:(n-1)
sum = sum + (g_i(i) - (2*i-1)/(2*n))^2;
end;
disp('--Критерий: ');
nw22 = n*(1/(12*N^2)+1/N*sum);
disp(nw22);
disp('--Статистика');
disp(0.2415);
 
 
disp('Определение параметров нормального распределения: ');
disp('- методом моментов');
normalmmparametr(1)=meanX;
normalmmparametr(2)=stdX;
disp('Параметр 1: ');
disp(normalmmparametr(1));
disp('Параметр 2: ');
disp(normalmmparametr(2));
% -с помощью метода максимального правдоподобия
disp('- методом максимального правдоподобия:');
% Численная оценка
[normalmmpchparametr(1),normalmmpchparametr(2)]=normfit(X);
disp('-- численное решение: ');
disp('Параметр 1: ');
disp(normalmmpchparametr(1));
disp('Параметр 2: ');
disp(normalmmpchparametr(2));
% Аналитическая оценка
normalmmpanalparametr(1)=meanX;
normalmmpanalparametr(2)=stdX;
disp('-- аналитическое решение: ');
disp('Параметр 1: ');
disp(normalmmpanalparametr(1));
disp('Параметр 2: ');
disp(normalmmpanalparametr(2));
 
 
 
disp('Определение параметров гамма распределения: ');
disp('- методом моментов');
gammmparametr(1)=meanX^2/(mean(X.^2)-meanX^2);
gammmparametr(2)=(mean(X.^2)-meanX^2)/meanX;
disp('Параметр 1: ');
disp(gammmparametr(1));
disp('Параметр 2: ');
disp(gammmparametr(2));
% -с помощью метода максимального правдоподобия
disp('- методом максимального правдоподобия:');
% Численная оценка
gammmpchparametr=gamfit(X);
disp('-- численное решение: ');
disp('Параметр 1: ');
disp(gammmpchparametr(1));
disp('Параметр 2: ');
disp(gammmpchparametr(2));
% Аналитическая оценка
gammmpanalparametr(2)=varX/meanX;
gammmpanalparametr(1)=1/gammmpanalparametr(2)*meanX;
disp('-- аналитическое решение: ');
disp('Параметр 1: ');
disp(gammmpanalparametr(1));
disp('Параметр 2: ');
disp(gammmpanalparametr(2));
