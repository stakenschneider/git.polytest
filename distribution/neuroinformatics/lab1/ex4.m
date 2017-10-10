net = newp([0 1; 0 1], 1);
net.inputs{1}.size=2;
net.layers{1}.size=8;
net.layers{1}.transferFcn='hardlim';

net.IW{1,1} =  [[0 -1]; [-1 0]; 
    [0 1]; [-1 0]; 
    [0 -1]; [1 0]; 
    [0 1]; [1 0];];

net.b{1}=[0.4 0.4  -0.9 0.4  0.4 -0.9     -0.9 -0.9]';

hold on;

for i = 1:1:4
    for j = 1:1:4
        if sim(net,[i/4;j/4]) == 0
            plot( i/4, j/4 ,'o', 'Color', [.9 .1 .1]);
        else
            plot( i/4, j/4 ,'o', 'Color', [.1 .1 .9]);
        end
    end
end
hold off; 
axis([0 1.25 0 1.25]);
