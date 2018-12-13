function [ res ] = round( value )
% Функция постобработки - округление
if value < 0.5
    res = 0
else
    res = 1;
end
end
