net=network(1,3,[1 1 1]',[1 0 0]',[[0 1 0]' [0 0 1]' [0 0 0]'],[0 0 1]);
net.inputs{1}.size=2;

net.layers{1}.size=16;
net.layers{2}.size=4;
net.layers{3}.size=1;

net.layers{1}.transferFcn='hardlims';
net.layers{2}.transferFcn='hardlim';
net.layers{3}.transferFcn='hardlim';


net.IW{1,1} = [ [1 0];[-1 0]; [0 1];[ 0 -1];[ 1 -0];[ -1 0];[ 0 -1];[ 0 1];[ 1 0];[ -1 0];[ 0 -1];[ 0 1];[ 1 0];[ -1 0];[ 0 -1];[ 0 1] ];
% net.IW{1,2} = [  -1 1 0 0 -1 1 0 0 -1 1 ]';

net.b{1}= [ -0.2 1 0 0.2   -0.2 0.4 0.8 -0.2   -0.4 0.8 0.8 -0.6   -0.6 0.8 0.6 -0.4 ]';

net.LW{2,1} = [[ 1 1 1 1 0 0 0 0 0 0 0 0 0 0 0 0];
               [ 0 0 0 0 1 1 1 1 0 0 0 0 0 0 0 0];
               [ 0 0 0 0 0 0 0 0 1 1 1 1 0 0 0 0];
               [ 0 0 0 0 0 0 0 0 0 0 0 0 1 1 1 1];];

net.b{2}= -[3.5 3.5 3.5 3.5]';

net.LW{3,2} = [ 1 1 1 1 ];
net.b{3}= -0.5;

    
hold on;


for i = 1:1:20
    for j = 1:1:20
        if sim(net,[i/20;j/20]) == 0
            plot( i/20, j/20 ,'o', 'Color', [.9 .1 .1]);
        else
            plot( i/20, j/20 ,'o', 'Color', [.1 .1 .9]);
        end
    end
end

% for i = 1:1:20
%     for j = 1:1:20
%         if (sim(net,[i/20;j/20]) == a{1}) 
%             plot( i/20, j/20 ,'o', 'Color', [.9 .1 .1]);
%         elseif (sim(net,[i/20;j/20]) == a{20})
%              plot( i/20, j/20 ,'o', 'Color', [.9 .1 .1]);
%                      elseif (sim(net,[i/20;j/20]) == a{16})
%              plot( i/20, j/20 ,'o', 'Color', [.9 .1 .1]);
%                                   elseif (sim(net,[i/20;j/20]) == a{11})
%              plot( i/20, j/20 ,'o', 'Color', [.9 .1 .1]);
%                                                elseif (sim(net,[i/20;j/20]) == a{9})
%              plot( i/20, j/20 ,'o', 'Color', [.9 .1 .1]);
%                                                             elseif (sim(net,[i/20;j/20]) == a{4})
%              plot( i/20, j/20 ,'o', 'Color', [.9 .1 .1]);
%                                                                          elseif (sim(net,[i/20;j/20]) == a{8})
%              plot( i/20, j/20 ,'o', 'Color', [.9 .1 .1]);
%         else
%             plot( i/20, j/20 ,'o', 'Color', [.1 .1 .9]);
%         end
%     end
% end

hold off; 
axis([0 1 0 1]);

