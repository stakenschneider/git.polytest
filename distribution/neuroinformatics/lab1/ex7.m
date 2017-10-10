net=network(1,3,[1 1 1]',[1 0 0]',[[0 1 0]' [0 0 1]' [0 0 0]'],[0 0 1]);
net.inputs{1}.size=2;

net.layers{1}.size=32;
net.layers{2}.size=8;
net.layers{3}.size=8;

net.layers{1}.transferFcn='hardlims';
net.layers{2}.transferFcn='hardlim';
net.layers{3}.transferFcn='hardlim';

net.IW{1,1} = [[1 0]; [ -1 0]; [ 0 1]; [ 0 -1]; [  1 0]; [ -1 0]; [ 0 1]; [ 0 -1]; [  1 0]; [ -1 0]; [ 0 1]; [ 0 -1]; [  1 0]; [ -1 0]; [ 0 1]; [0 -1]; [  1 0]; [ -1 0]; [ 0 1]; [0 -1]; [ 1 0]; [-1 0]; [ 0 1]; [ 0 -1]; [  1 0]; [ -1 0]; [ 0 1]; [ 0 -1]; [  1 0]; [ -1 0]; [ 0 1]; [ 0 -1]];
net.b{1}= [-0.4 0.5 -0.3 0.4   -0.3 0.4 -0.3 0.4   -0.3 0.5 -0.4 0.5   -0.5 0.6 -0.3 0.5  -0.3 0.6 0 0.3   0 0.3 0 0.5   0 0.6 -0.5 1   -0.6 1 0 1]';

net.LW{2,1} =[[1 1 1 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0]; ...
              [0 0 0 0 1 1 1 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0]; ...
              [0 0 0 0 0 0 0 0 1 1 1 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0]; ...
              [0 0 0 0 0 0 0 0 0 0 0 0 1 1 1 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0]; ...
              [0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 1 1 1 0 0 0 0 0 0 0 0 0 0 0 0]; ...
              [0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 1 1 1 0 0 0 0 0 0 0 0]; ...
              [0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 1 1 1 0 0 0 0]; ...
              [0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 1 1 1];];          
net.b{2} = -[3.5 3.5 3.5 3.5 3.5 3.5 3.5 3.5 ]';

net.LW{3,2} = [[1 0 0 0 0 0 0 0]; ...
               [0 1 0 0 0 0 0 0]; ...
               [0 0 1 0 0 0 0 0]; ...
               [0 0 0 1 0 0 0 0]; ...
               [0 0 0 0 1 0 0 0]; ...
               [0 0 0 0 0 1 0 0]; ...
               [0 0 0 0 0 0 1 0]; ...
               [0 0 0 0 0 0 0 1];];
net.b{3} = -[0.5 0.5 0.5 0.5 0.5 0.5 0.5 0.5]';


hold on;
for i = 1:1:20
    for j = 1:1:20
        if sim(net,[i/20;j/20]) == [1 0 0 0 0 0 0 0]'
            plot( i/20, j/20 ,'o', 'Color', [.1 .1 .9]);
        end;
        
        if sim(net,[i/20;j/20]) == [0 1 0 0 0 0 0 0]'
            plot( i/20, j/20 ,'o', 'Color', [.1 .1 .7]);
        end;
        
        if sim(net,[i/20;j/20]) == [0 0 1 0 0 0 0 0]'
            plot( i/20, j/20 ,'o', 'Color', [.1 .1 .6]);
        end;
        
        if sim(net,[i/20;j/20]) == [0 0 0 1 0 0 0 0]'
            plot( i/20, j/20 ,'o', 'Color', [.1 .1 .5]);
        end;
        
        if sim(net,[i/20;j/20]) == [0 0 0 0 1 0 0 0]'
            plot( i/20, j/20 ,'o', 'Color', [.1 .1 .4]);
        end;
        
        if sim(net,[i/20;j/20]) == [0 0 0 0 0 1 0 0]'
            plot( i/20, j/20 ,'o', 'Color', [.1 .1 .3]);
        end;
        
        if sim(net,[i/20;j/20]) == [0 0 0 0 0 0 1 0]'
            plot( i/20, j/20 ,'o', 'Color', [.1 .1 .2]);
        end;
        
        if sim(net,[i/20;j/20]) == [0 0 0 0 0 0 0 1]'
            plot( i/20, j/20 ,'o', 'Color', [.1 .1 .1]);
        end;
    end
end
hold off; 
axis([0 1 0 1]);
