function res=quality(k0, k1)
    root=roots([1 (k1) (k0+1)]);
    res(1,1)=max(abs(imag(root)));
    res(2,1)=geomean(abs(root));
    res(3,1)=min(abs(real(root)));
end