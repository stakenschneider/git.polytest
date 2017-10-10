net = newp( [0 1], 1 )                % ? на экране весь объект; 

net.inputWeights{1,1}.initFcn='rands'; 
net.biases{1}.initFcn='rands'; 
net = init( net );                    % ? инициализация сл.значениями; 
% net.IW{1,1}, net.b{1}                 % ? новые значения; 
p = {[0] [1] [0.5]};                  % ? последовательность входов; 
a = sim( net, p )                     % ? моделирование сети;

