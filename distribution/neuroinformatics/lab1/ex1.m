net = newp( [0 1], 1 )                % ? �� ������ ���� ������; 

net.inputWeights{1,1}.initFcn='rands'; 
net.biases{1}.initFcn='rands'; 
net = init( net );                    % ? ������������� ��.����������; 
% net.IW{1,1}, net.b{1}                 % ? ����� ��������; 
p = {[0] [1] [0.5]};                  % ? ������������������ ������; 
a = sim( net, p )                     % ? ������������� ����;

