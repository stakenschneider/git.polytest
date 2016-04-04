#include "Forsythe.h"
#include <math.h>
#include <stdio.h>
#include <iostream>

using namespace std;

void Diff(Float t,Float *y,Float *dy)
{
    dy[0]=(-14)*(y[0])+13*y[1] + cos(1+t);
    dy[1]=20*y[0]-30*y[1]+atan(1+t*t);
}


void RK(Float t,Float * zs, Float *z, Float h)
{
    Float k[2] , k1[2] , k2[2] , k3[2];
    
    Diff(t,zs,k1);
    k1[0]=h*k1[0];
    k1[1]=h*k1[1];
    k[0]=zs[0]+k1[0]/2;
    k[1]=zs[1]+k1[1]/2;
    
    Diff(t+h/2,k,k2);
    k2[0]=h*k2[0];
    k2[1]=h*k2[1];
    k[0]=zs[0] + k1[0]*3/4;
    k[1]=zs[1] + k1[1]*3/4;
    
    Diff(t+h*3/4,k,k3);
    k3[0]=h*k3[0];
    k3[1]=h*k3[1];
    z[0]=zs[0]+(2*k1[0] + 3*k2[0] + 4*k3[0])/9;
    z[1]=zs[1]+(2*k1[1] + 3*k2[1] + 4*k3[1])/9;
}


int main (void)
{
    Float y0[]={0,1} , z1[]={0,1} , z2[2] , tout=0 , e = 0;
    
    unsigned char work[6*(2*sizeof(Float)) + sizeof(struct rkf_inside)];
    rkf var_rkf;
    rkf_inside *p;
    rkf RKF = var_rkf;
    var_rkf.f = Diff;
    var_rkf.Y = y0;
    var_rkf.t = 0;
    var_rkf.tout = 0;
    var_rkf.ae = 0.0001;
    var_rkf.re = 0.0001;
    var_rkf.neqn = 2;
    var_rkf.flag = 1;
    var_rkf.work = work;
    p = (struct rkf_inside *)var_rkf.work;
    
    cout << "RKF45\n";
    while(tout < 1.50001)
    {
        RKF.tout = tout;
        rkf45(&RKF);
        cout << "t:" << tout << "  x1:" << y0[0] << "  x2:" << y0[1] << endl;
        RKF = var_rkf;
        tout+= 0.075;
    }
    
    z1[0]=0;
    z1[1]=1;
    while(e < 1.50001)
    {
        RK(e , z1 , z2 , 0.075);
        cout << "t:" << e << "  x1:" << z1[0] << "  x2:" << z1[1] << endl;
        e+= 0.075;
        z1[0] = z2[0];
        z1[1] = z2[1];
    }
    
    e = 0;
    z1[0]=0;
    z1[1]=1;
    while(e < 1.50001)
    {
        RK(e , z1 , z2 , 0.047);
        cout << "t:" << e << "  x1:" << z1[0] << "  x2:" << z1[1] << endl;
        e+= 0.075;
        z1[0] = z2[0];
        z1[1] = z2[1];
    }
}
