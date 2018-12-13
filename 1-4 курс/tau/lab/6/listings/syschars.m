clear all;
close all;
clc;
format compact;

A = [0 1; -0.75 -2];
B = [0; 1];
C = [0.75 0];

K=[0.1 5; 5 10; 10 5; 10 10];
L=[place(A', C', [-0.1, -0.2]).', place(A', C', [-2, -4]).', place(A', C', [-16, -32]).'];
e=[0 0 0 0; 0 0 0.5 -0.5; 0 0 -0.5 0.5; 0 0 0.7 0.7];
t=0:0.01:8;

for i=1:4
    obs_sys=get_observer(L(:,3), K(i,:));
    sys=get_sys(K(i,:));
    fig = figure;
    hold on;
    lsim(obs_sys, ones(size(t)), t, e(2,:), '--r');
    lsim(sys, ones(size(t)), t);
    legend('with observer', 'without observer');
    hold off;
    K(i,:)
    disp('With observer');
    [omega, stab, ~]=quality(obs_sys)
    disp('Without observer');
    [omega, stab, ~]=quality(sys)
    saveas(fig, strcat('k',num2str(i)),'png')
end

obs_sys=get_observer(L(:,3), K(4,:));
sys=get_sys(K(4,:));

for i=1:4
    fig = figure;
    hold on;
    lsim(obs_sys, ones(size(t)), t, e(i,:), '--r');
    lsim(sys, ones(size(t)), t);
    legend('with observer', 'without observer');
    hold off;
    saveas(fig, strcat('e',num2str(i)),'png')
end

disp('With observer');
for i=1:3
    obs_sys=get_observer(L(:,i), K(4,:));
    fig = figure;
    hold on;
    lsim(obs_sys, ones(size(t)), t, e(2,:), '--r');
    lsim(sys, ones(size(t)), t);
    legend('with observer', 'without observer');
    hold off;
    L(:,i) 
    [omega, stab, ~]=quality(obs_sys)
    saveas(fig, strcat('g',num2str(i)),'png')
end
disp('Without observer');
[omega, stab, ~]=quality(sys)
