clc;
nfu_A = [0 1; -1 0];

nfu_B = [0; 1];
nfu_C = [1 0];

nfn_A = [0 -1; 1 0];
nfn_B = [1; 0];
nfn_C = [0 1];

kf_A = [i 0; 0 -i];
kf_B = [-0.5*i;0.5*i];
kf_C = [1 1];

nfu_U = [nfu_B nfu_A*nfu_B]
nfu_inv_U = inv(nfu_U)

det(nfu_U)

nfn_U = [nfn_B nfn_A*nfn_B]
nfn_inv_U = inv(nfn_U)

N = [nfn_C' nfn_A'*nfn_C']
det(N)

kf_U = [kf_B kf_A*kf_B]
kf_inv_U = inv(kf_U)

P_nfu_nfn = nfn_U / nfu_U
P_nfu_nfn * nfu_B

P_nfu_kf = kf_U / nfu_U
P_nfu_kf * nfu_B


P_nfn_nfu = nfu_U / nfn_U
P_nfn_nfu * nfn_B 

P_nfn_kf = kf_U / nfn_U
P_nfn_kf * nfn_B


P_kf_nfu = nfu_U / kf_U
P_kf_nfu * kf_B

P_kf_nfn = nfn_U / kf_U
P_kf_nfn * kf_B

