function [omega, stab, root]=quality(sys)
    root=pole(sys);
    omega=geomean(abs(root));
    stab=min(abs(real(root)));
end