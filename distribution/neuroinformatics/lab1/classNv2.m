function classN
net=network(1,2,[1 1]',[1 0]',[[0 1]' [0 0]'],[0 1]);
net.inputs{1}.size=2;
net.layers{1}.size=16;
net.layers{2}.size=6;
net.layers{1}.transferFcn='hardlims';
net.layers{2}.transferFcn='hardlim';
net.IW{1,1}=[[0 1];[0 1];[1 -0.25];[1 -0.25];[1 0];
                      [1 0];[1 0.25];[0 1];[1 0.25];[1 -0.25];
                      [0 1];[1 0];[1 0];[1 -0.25];[1 0.25];
                      [1 -0.25]];
net.b{1}=-[10 14 15 21  10        14 9 4 12 14           8 24 26 24 37   2]';
net.LW{2,1}=  [[1 -1 0 0  1  -1 0 0 0 0       0 0 0 0 0    0];
                         [1 -1 1  -1 0     0 0 0 0 0    0 0 0 0 0    0];
                         [1 -1 0 0 0      0 0 0 0 0    0 0 0 1 -1    0];
                         [0 0 0 0 0      0 -1 1 0 0       0 0 0 0 0    1];
                         [0 0 0 0 0      0 0 1 1 -1       -1 0 0 0 0    0];
                         [0 0 0 0 0      0 0 1 0 0       -1 1 -1 0 0    0]];
net.b{2}=-[3.5 3.5 3.5 2.5      3.5          3.5 ]';
%view(net)
%gensim(net)
hold on;
colors = ['r','g','b', 'c','m','y','k','w','b','g'];
p=[];
for i=0:0.05:36
    for j=0:0.05:36
        p=[p [i;j]];
    end
end
     h=plot(p(1,i),p(2,i));
      set(h,'Color',colors(9));
        a=sim(net,p);
        for i=1:1:length(a)
                if(a(:,i)==[1 0 0 0 0 0]')
                    h=plot(p(1,i),p(2,i));
                    set(h,'Color',colors(1));
                end
                if(a(:,i)==[0 1 0 0 0 0]')
                    h=plot(p(1,i),p(2,i));
                    set(h,'Color',colors(2));            
                end
                if(a(:,i)==[0 0 1 0 0 0]')
                    h=plot(p(1,i),p(2,i));
                    set(h,'Color',colors(3));            
                end
                if(a(:,i)==[0 0 0 1 0 0]')
                    h=plot(p(1,i),p(2,i));
                    set(h,'Color',colors(4));            
                end
                if(a(:,i)==[0 0 0 0 1 0]')
                    h=plot(p(1,i),p(2,i));
                    set(h,'Color',colors(5));            
                end
                if(a(:,i)==[0 0 0 0 0 1]')
                    h=plot(p(1,i),p(2,i));
                    set(h,'Color',colors(6));            
                end
        end
hold off;
end