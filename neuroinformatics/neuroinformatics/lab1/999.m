
net=network(1,2,[1 1]',[1 0]',[[0 1 ]' [0 0]'],[0 1]);
net.inputs{1}.size=5;
net.layers{1}.size=5;
net.layers{2}.size=1;
net.layers{1}.transferFcn='hardlim';
net.layers{2}.transferFcn='hardlim';
net.IW{1,1}=[          [1 1 -1 -1 1];       % 25
                       [1 -1 -1 -1 -1];    % 16
                       [-1 1 -1 -1 1];     % 9
                       [-1 -1 1 -1 -1];     % 4
                       [-1 -1 -1 -1 1]];     % 1
net.b{1}=-[5.5 5.5 5.5 5.5 5.5]';
net.LW{2,1}=  [1 1 1 1 1];
net.b{2}=-0.5;
view(net)
%gensim(net);
sim(net,{ [-1; -1; -1; 1; 1; 1;]  [-1; 1; -1; -1; -1; -1;] [-1; 1; -1; -1; 1; -1;] [-1; 1; 1; -1; -1; -1;] [1; -1; 1; -1; -1; 1;] [-1; -1; -1; -1; 1; 1] [-1; -1; 1; -1; 1; 1] [1; -1; -1; 1; -1; 1] [1; 1; -1; -1; 1; 1] [-1; 1; 1; -1; 1; 1] })
