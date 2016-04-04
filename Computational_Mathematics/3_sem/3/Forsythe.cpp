#include "Forsythe.h"
#include <math.h>

inline void fehl(void (*f)(Float t, Float *Y, Float *dY), unsigned int neqn, Float *Y, Float t, Float h, Float *dY,
                 Float *F1, Float *F2, Float *F3, Float *F4, Float *F5, Float *S)
{
    Float ch;
    unsigned int k;
    
    ch = h/(Float)4;
    for(k=0;k<neqn;k++) F5[k] = Y[k] + ch*dY[k];
    f(t+ch, F5, F1);
    
    ch = (Float)3*h/(Float)32;// 3*h/32
    for(k=0;k<neqn;k++) F5[k] = Y[k] + ch*(dY[k] + (Float)3*F1[k]);
    f(t+(Float)3*h/(Float)8, F5, F2);
    
    ch = h/(Float)2197;// h/2197
    for(k=0;k<neqn;k++) F5[k] = Y[k] + ch*((Float)1932*dY[k] + ((Float)7296*F2[k] - (Float)7200*F1[k]));
    f(t+(Float)12*h/(Float)13, F5, F3);
    
    ch = h/(Float)4104;// h/4104
    for(k=0;k<neqn;k++) F5[k] = Y[k] + ch*(((Float)8341*dY[k] - (Float)845*F3[k]) +
                                           ((Float)29440*F2[k] - (Float)32832*F1[k]));
    f(t+h, F5, F4);
    
    ch = h/(Float)20520;// h/20520
    for(k=0;k<neqn;k++) F1[k] = Y[k] + ch*(((Float)(-6080)*dY[k] + ((Float)9295*F3[k] - (Float)5643*F4[k])) +
                                           ((Float)41040*F1[k] - (Float)28352*F2[k]));
    f(t+h/(Float)2, F1, F5);
    
    //¬˚˜ËÒÎËÚ¸ ÔË·ÎËÊ∏ÌÌÓÂ Â¯ÂÌËÂ ‚ ÚÓ˜ÍÂ t+h
    ch = h/(Float)7618050;
    for(k=0;k<neqn;k++) S[k] = Y[k] + ch*(((Float)902880*dY[k] + ((Float)3855735*F3[k] - (Float)1371249*F4[k])) +
                                          ((Float)3953664*F2[k] + (Float)277020*F5[k]));
}

bool rkf45(struct rkf *p)
{
    struct rkf_inside *w;
    Float *dY, *F1, *F2, *F3, *F4, *F5,
    dt, u26, rer, scale, ae, hmin, eeoet, esttol, s;
    unsigned int MFlag, k;
    bool output, hfaild;
    
    //œÓ‚ÂˇÂÏ ‚ıÓ‰Ì˚Â Ô‡‡ÏÂÚ˚
    if(p->neqn < 1 || p->re < (Float)0 || p->ae < (Float)0 || p->flag == 0 || p->flag < -2 || p->flag > 8)
    {
        p->flag = 8;//Œ¯Ë·Í‡ ‚ıÓ‰ÌÓÈ ËÌÙÓÏ‡ˆËË
        return false;
    }
    
    MFlag = ABS(p->flag);
    w = (struct rkf_inside *)p->work;
    dY = (Float *)(p->work + sizeof(struct rkf_inside));
    F1 = dY + p->neqn;
    F2 = F1 + p->neqn;
    F3 = F2 + p->neqn;
    F4 = F3 + p->neqn;
    F5 = F4 + p->neqn;
    
    if(MFlag != 1)//≈ÒÎË ˝ÚÓ ÛÊÂ ÌÂ ÔÂ‚ÓÂ Ó·‡˘ÂÌËÂ Í rkf45
    {
        //œÓ‚ÂËÚ¸ ‚ÓÁÏÓÊÌÓÒÚ¸ ÔÓ‰ÓÎÊÂÌËˇ
        if(p->t == p->tout && w->KFlag != 3)
        {
            p->flag = 8;//Œ¯Ë·Í‡ ‚ıÓ‰ÌÓÈ ËÌÙÓÏ‡ˆËË
            return false;
        }
        if(MFlag != 2)
        {//p->flag = 3,4,5,6,7 ËÎË 8
            if(p->flag != 3)
            {
                if(p->flag == 4) w->nfe = 0;
                else if(p->flag != 5 || p->ae <= 0) return true;//»ÌÚÂ„ËÓ‚‡ÌËÂ ÌÂÎ¸Áˇ ÔÓ‰ÓÎÊ‡Ú¸, Ú.Í.
                //ÔÓÎ¸ÁÓ‚‡ÚÂÎ¸ ÌÂ ‚˚ÔÓÎÌËÎ ËÌÒÚÛÍˆËÈ, ÒÓÓÚ‚ÂÚÒÚ‚Û˛˘Ëı ÁÌ‡˜ÂÌËˇÏ p->flag = 5,6,7 ËÎË 8
            }
            //œÂÂÓÔÂ‰ÂÎËÚ¸ ÁÌ‡˜ÂÌËÂ p->flag, ÛÒÚ‡ÌÓ‚ÎÂÌÌÓÂ ÔË ÔÂ‰˚‰Û˘ÂÏ Ó·‡˘ÂÌËË
            p->flag = w->JFlag;
            if(w->KFlag == 3) MFlag = ABS(w->JFlag);
        }
        else
        {//p->flag = +2 ËÎË -2
            if(w->KFlag == 3 || w->init == 0)
            {
                //œÂÂÓÔÂ‰ÂÎËÚ¸ ÁÌ‡˜ÂÌËÂ p->flag, ÛÒÚ‡ÌÓ‚ÎÂÌÌÓÂ ÔË ÔÂ‰˚‰Û˘ÂÏ Ó·‡˘ÂÌËË
                p->flag = w->JFlag;
                if(w->KFlag == 3) MFlag = ABS(w->JFlag);
            }
            else
            {
                if(w->KFlag == 4) w->nfe = 0;
                else if(w->KFlag == 5){if(p->ae == 0.0) return true;}
                else if(w->KFlag == 6 && p->re <= w->SaveRE && p->ae <= w->SaveAE) return true;
            }
        }
    }
    
    //—Óı‡ÌËÚ¸ ‚ıÓ‰ÌÓÂ ÁÌ‡˜ÂÌËÂ p->flag Ë ÛÒÚ‡ÌÓ‚ËÚ¸ ÁÌ‡˜ÂÌËÂ FLAG, ÒÓÓÚ‚ÂÚÒÚ‚Û˛˘ÂÂ ÔÓ‰ÓÎÊÂÌË˛, ‰Îˇ ·Û‰Û˘ÂÈ ÔÓ‚ÂÍË
    w->JFlag = p->flag;
    w->KFlag = 0;
    //—Óı‡ÌËÚ¸ ÁÌ‡˜ÂÌËˇ p->re & p->ae ‰Îˇ ‚ıÓ‰ÌÓÈ ÔÓ‚ÂÍË ÔË ÔÓÒÎÂ‰Û˛˘Ëı Ó·‡˘ÂÌËˇı
    w->SaveRE = p->re;
    w->SaveAE = p->ae;
    
    rer = (Float)2*(Float)EPSILON + (Float)REMIN;
    u26 = (Float)26*(Float)EPSILON;
    
    if(p->re < rer)
    {
        p->flag = w->KFlag = 3;
        p->re = rer;
        return false;
    }
    
    dt = p->tout - p->t;
    
    if(MFlag == 1 || w->init == 0)
    {
        Float temp;
        bool toln;
        if(MFlag == 1)
        {
            w->kop = 0;
            p->f(p->t, p->Y, dY);
            w->nfe = 1;
            if(p->t == p->tout)
            {
                w->init = 0;
                p->flag = 2;
                return false;
            }
        }
        
        w->init = 1;
        w->h = ABS(dt);
        toln = false;
        for(k = 0; k < p->neqn; k++)
        {
            Float tol;
            tol = p->re*ABS(p->Y[k]) + p->ae;
            if(tol > 0)
            {
                Float ypk;
                toln = true;
                ypk = ABS(dY[k]);
                if(ypk*w->h*w->h*w->h*w->h*w->h > tol) w->h = (Float)pow(tol/ypk, 0.2);
            }
            else break;
        }
        temp = u26*MAX(ABS(p->t), ABS(dt));
        if(!toln || w->h < temp) w->h = temp;
        w->JFlag = (p->flag > 0) ? 2 : -2;
    }
    
    w->h = SIGN2(w->h, dt);
    if(ABS(w->h) >= 2.0*ABS(dt)) w->kop++;
    if(w->kop == 100)
    {
        w->kop = 0;
        p->flag = 7;
        return false;
    }
    
    if(ABS(dt) <= u26*ABS(p->t))
    {
        for(k = 0;k < p->neqn;k++)
        {
            p->Y[k] += dt*dY[k];
            p->f(p->tout, p->Y, dY);
            w->nfe++;
        }
        p->t = p->tout;
        p->flag = 2;
        return false;
    }
    
    output = false;
    scale = (Float)2/p->re;
    ae = scale*p->ae;
    
    do
    {
        hfaild = false;
        hmin = u26*ABS(p->t);
        dt = p->tout - p->t;
        
        if(ABS(dt) < 2.0*ABS(w->h))
        {
            if(ABS(dt) > ABS(w->h)) w->h = dt/(Float)2;
            else
            {
                output = true;
                w->h = dt;
            }
        }
        
        while(1)
        {
            if(w->nfe > MAXNFE)
            {
                p->flag = w->KFlag = 4;
                return false;
            }
            
            fehl(p->f, p->neqn, p->Y, p->t, w->h, dY, F1, F2, F3, F4, F5, F1);
            w->nfe += 5;
            eeoet = 0.0;
            
            for(k=0;k<p->neqn;k++)
            {
                Float et;
                et = ABS(p->Y[k]) + ABS(F1[k]) + ae;
                if(et > 0.0)
                {
                    Float ee;
                    ee = ABS(((Float)(-2090)*dY[k]+((Float)21970*F3[k]-(Float)15048*F4[k]))+((Float)22528*F2[k]-(Float)27360*F5[k]))/et;
                    if(eeoet < ee) eeoet = ee;
                }
                else
                {
                    p->flag = w->KFlag = 5;
                    return false;
                }
            }
            
            esttol = ABS(w->h)*eeoet*scale/(Float)752400;
            
            if(esttol <= (Float)1) break;
            
            hfaild = true;
            output = false;
            s = (Float)0.1;
            if(esttol < (Float)59049) s = (Float)0.9/(Float)pow(esttol, 0.2);
            w->h *= s;
            if(ABS(w->h) <= hmin)
            {
                p->flag = w->KFlag = 6;
                return false;
            }
        }
        
        p->t += w->h;
        for(k=0;k<p->neqn;k++) p->Y[k] = F1[k];
        p->f(p->t, p->Y, dY);
        w->nfe++;
        
        s = 5.0;
        if(esttol > (Float)1.889568e-4) s = (Float)0.9/(Float)pow(esttol, 0.2);
        if(hfaild) s = MIN(s, (Float)1.0);
        
        if(w->h != 0) if(w->h > 0) w->h = ABS(MAX(s*w->h, hmin));
        else w->h = -ABS(MAX(-s*w->h, hmin));
        
        if(output)
        {
            p->t = p->tout;
            p->flag = 2;
            return false;
        }
    }while(p->flag > 0);
    
    p->flag = -2;
    return false;
}

Float FMin(Float (*F)(Float), Float a, Float b, Float tol)
{
    Float c = (Float)((Float)3.0-(Float)sqrt(5.0))/(Float)2,//Ò - ˝ÚÓ ‚ÓÁ‚Â‰∏ÌÌ‡ˇ ‚ Í‚‡‰‡Ú ‚ÂÎË˜ËÌ‡, Ó·‡ÚÌ‡ˇ Í ÁÓÎÓÚÓÏÛ ÒÂ˜ÂÌË˛
    d, e = (Float)0, eps = (Float)sqrt(EPSILON),//eps ÔË·ÎËÁËÚÂÎ¸ÌÓ ‡‚ÌÓ Í‚‡‰‡ÚÌÓÏÛ ÍÓÌ˛ ËÁ ÓÚÌÓÒËÚÂÎ¸ÌÓÈ Ï‡¯ËÌÌÓÈ ÚÓ˜ÌÓÒÚË
    xm, p, q, r, tol1, tol2, u, v = a+c*(b-a), w = v, x = v, fx = F(v), fu, fv = fx, fw = fx;
    
    while(1)
    {
        xm = (a+b)/(Float)2;
        tol1 = eps*ABS(x) + tol/(Float)3;
        tol2 = tol1*(Float)2;
        
        //œÓ‚ÂËÚ¸ ÍËÚÂËÈ ÓÍÓÌ˜‡ÌËˇ
        if(ABS(x-xm) <= (tol2-(b-a)/(Float)2)) return x;
        
        if(ABS(e) > tol1)//≈ÒÎË ÁÓÎÓÚÓÂ ÒÂ˜ÂÌËÂ ÌÂ ÚÂ·ÛÂÚÒˇ
        {
            //œÓÒÚÓËÚ¸ Ô‡‡·ÓÎÛ
            r = (x-w)*(fx-fv);
            q = (x-v)*(fx-fw);
            p = (x-v)*q - (x-w)*r;
            q = (Float)2*(q-r);
            if(q > 0) p = -p;
            else q = -q;//q = ABS(q)
            r = e;
            e = d;
            
            //œËÂÏÎÂÏ‡ ÎË Ô‡‡·ÓÎ‡
            if(ABS(p) >= ABS(q*r/(Float)2) || p <= q*(a-x) || p >= q*(b-x))
            {
                //ÿ‡„ ÁÓÎÓÚÓ„Ó ÒÂ˜ÂÌËˇ
                if(x >= xm) e = a-x;
                else e = b-x;
                d = c*e;
            }
            else
            {
                //ÿ‡„ Ô‡‡·ÓÎË˜ÂÒÍÓÈ ËÌÚÂÔÓÎˇˆËË
                d = p/q;
                u = x+d;
                //F ÌÂ ÒÎÂ‰ÛÂÚ ‚˚˜ËÒÎˇÚ¸ ÒÎË¯ÍÓÏ ·ÎËÁÍÓ Í 'a' ËÎË 'b'
                if((u-a) < tol2 || (b-u) < tol2) d = SIGN2(tol1, xm-x);
            }
        }
        else
        {
            //ÿ‡„ ÁÓÎÓÚÓ„Ó ÒÂ˜ÂÌËˇ
            if(x >= xm) e = a-x;
            else e = b-x;
            d = c*e;
        }
        
        //F ÌÂ ÒÎÂ‰ÛÂÚ ‚˚˜ËÒÎˇÚ¸ ÒÎË¯ÍÓÏ ·ÎËÁÍÓ Í 'x'
        if(ABS(d) >= tol1) u = x+d;
        else u = x + SIGN2(tol1, d);
        fu = F(u);
        
        //œËÒ‚ÓËÚ¸ ÌÓ‚˚Â ÁÌ‡˜ÂÌËˇ Ô‡‡ÏÂÚ‡Ï 'a', 'b', 'v', 'w' Ë 'x'
        if(fu <= fx)
        {
            if(u >= x) a = x;
            else b = x;
            v = w;
            fv = fw;
            w = x;
            fw = fx;
            x = u;
            fx = fu;
            continue;
        }
        if(u < x) a = u;
        else b = u;
        if(fu <= fw || w == x)
        {
            v = w;
            fv = fw;
            w = u;
            fw = fu;
        }
        else if(fu <= fv || v == x || v == w)
        {
            v = u;
            fv = fu;
        }
    }
    
    return x;
}

Float Zeroin(Float (*F)(Float), Float a, Float b, Float tol)
{
    Float c, d, e, fa = F(a), fb = F(b), fc, tol1, xm, p, q, r, s;
    
    while(1)
    {
        c = a;
        fc = fa;
        e = d = b-a;
        do
        {
            if(ABS(fc) < ABS(fb))
            {
                a = b;
                b = c;
                c = a;
                fa = fb;
                fb = fc;
                fc = fa;
            }
            //œÓ‚ÂÍ‡ ÒıÓ‰ËÏÓÒÚË
            tol1 = (Float)2*EPSILON*ABS(b) + tol/(Float)2;
            xm = (c-b)/(Float)2;
            if(ABS(xm) <= tol1 || fb == 0) return b;
            //ÕÂÓ·ıÓ‰ËÏ‡ ÎË ·ËÒÂÍˆËˇ
            if(ABS(e) >= tol1 && ABS(fa) > ABS(fb))
            {
                //¬ÓÁÏÓÊÌ‡ ÎË Í‚‡‰‡ÚË˜Ì‡ˇ ËÌÚÂÔÓÎˇˆËˇ
                if(a != c)
                {
                    //Œ·‡ÚÌ‡ˇ Í‚‡‰‡ÚË˜Ì‡ˇ ËÌÚÂÔÓÎˇˆËˇ
                    q = fa/fc;
                    r = fb/fc;
                    s = fb/fa;
                    p = s*((Float)2*xm*q*(q-r) - (b-a)*(r-(Float)1));
                    q = (q-(Float)1)*(r-(Float)1)*(s-(Float)1);
                }
                else
                {
                    //ÀËÌÂÈÌ‡ˇ ËÌÚÂÔÓÎˇˆËˇ
                    s = fb/fa;
                    p = (Float)2*xm*s;
                    q = (Float)1 - s;
                }
                //¬˚·‡Ú¸ ÁÌ‡ÍË
                if(p > 0) q = -q;
                else p = -p;//p = ABS(p)
                
                //œËÂÏÎÂÏ‡ ÎË ËÌÚÂÔÓÎˇˆËˇ
                if(((Float)2*p) >= ((Float)3*xm*q - ABS(tol1*q)) || p >= ABS(e*q/(Float)2)) e = d = xm;//¡ËÒÂÍˆËˇ
                else
                {
                    e = d;
                    d = p/q;
                }
            }
            else e = d = xm;//¡ËÒÂÍˆËˇ
            //«‡‚Â¯ËÚ¸ ¯‡„
            a = b;
            fa = fb;
            if(ABS(d) > tol1) b += d;
            else b += SIGN2(tol1, xm);
            fb = F(b);
        }while(fb*SIGN(fc) <= 0);
    }
}

Float Quanc8(Float (*F)(Float) , Float a, Float b, Float ae, Float re, Float *errest, int *nofun, Float *flag)
{
    const Float w0 = (Float)3956/(Float)14175,
				w1 = (Float)23552/(Float)14175,
				w2 = (Float)(-3712)/(Float)14175,
				w3 = (Float)41984/(Float)14175,
				w4 = (Float)(-18160)/(Float)14175;
    Float result = 0.0, area = 0.0, x0 = a, f0, stone = (b-a)/(Float)16, step, cor11 = 0.0, qprev = 0.0, qnow, qdiff, qleft, esterr, tolerr,
    qright[31], f[16], x[16], fsave[8][30], xsave[8][30];
    
    //*** ›“¿œ 1 *** œËÒ‚ÓÂÌËÂ Ì‡˜‡Î¸Ì˚ı ÁÌ‡˜ÂÌËÈ ÔÂÂÏÂÌÌ˚Ï, ÌÂ Á‡‚ËÒˇ˘ËÏ ÓÚ ËÌÚÂ‚‡Î‡. √ÂÌÂËÓ‚‡ÌËÂ ÍÓÌÒÚ‡ÌÚ.
#define LEVMIN	1
#define LEVMAX	30
#define LEVOUT	6
#define NOMAX	5000
    int levmax = LEVMAX, nofin = NOMAX - 8*(LEVMAX-LEVOUT+(1<<(LEVOUT+1))),//≈ÒÎË '*nofun' ‰ÓÒÚË„‡ÂÚ ÁÌ‡˜ÂÌËˇ 'nofin', ÚÓ ÚÂ‚Ó„‡
    lev = 0, nim = 1, i, j;
    
    //œËÒ‚ÓËÚ¸ ÌÛÎÂ‚˚Â ÁÌ‡˜ÂÌËˇ ÔÂÂÏÂÌÌ˚Ï ÒÛÏÏ‡Ï
    *flag = 0.0;
    *errest = 0.0;
    *nofun = 0;
    
    if(a == b) return 0.0;
    
    //*** ›“¿œ 2 *** œËÒ‚ÓÂÌËÂ Ì‡˜‡Î¸Ì˚ı ÁÌ‡˜ÂÌËÈ ÔÂÂÏÂÌÌ˚Ï, Á‡‚ËÒˇ˘ËÏ ÓÚ ËÌÚÂ‚‡Î‡, ‚ ÒÓÓÚ‚ÂÚÒÚ‚ËË Ò ÔÂ‚˚Ï ËÌÚÂ‚‡ÎÓÏ
    x[15] = b;
    f0 = F(x0);
    x[7] = (x0+b)/(Float)2;
    x[3] = (x0+x[7])/(Float)2;
    x[11] = (x[7]+b)/(Float)2;
    x[1] = (x0+x[3])/(Float)2;
    x[5] = (x[3]+x[7])/(Float)2;
    x[9] = (x[7]+x[11])/(Float)2;
    x[13] = (x[11]+b)/(Float)2;
    
    for(j=1;j<=15;j+=2) f[j] = F(x[j]);
    *nofun = 9;
    
    //*** ›“¿œ 3 *** ŒÒÌÓ‚Ì˚Â ‚˚˜ËÒÎÂÌËˇ
    while(1)
    {
        x[0] = (x0+x[1])/(Float)2;
        f[0] = F(x[0]);
        for(j=2;j<=14;j+=2)
        {
            x[j] = (x[j-1]+x[j+1])/(Float)2;
            f[j] = F(x[j]);
        }
        nofun += 8;
        step = (x[15]-x0)/(Float)16;
        qleft = (w0*(f0+f[7]) + w1*(f[0]+f[6]) + w2*(f[1]+f[5]) + w3*(f[2]+f[4]) + w4*f[3]) * step;
        qright[lev] = (w0*(f[7]+f[15]) + w1*(f[8]+f[14]) + w2*(f[9]+f[13]) + w3*(f[10]+f[12]) + w4*f[11]) * step;
        qnow = qleft + qright[lev];
        qdiff = qnow - qprev;
        area += qdiff;
        
        //*** ›TAœ 4 *** œÓ‚ÂÍ‡ ÒıÓ‰ËÏÓÒÚË ‰Îˇ ËÌÚÂ‚‡Î‡
        esterr = ABS(qdiff)/(Float)1023;
        tolerr = MAX(ae, re*ABS(area))*(step/stone);
        
        if(lev < LEVMIN || ((lev < levmax) && (*nofun <= nofin) && (esterr > tolerr)))
        {
            //*** ›TAœ 5 *** —ıÓ‰ËÏÓÒÚË ÌÂÚ. ”ÒÚ‡ÌÓ‚ËÚ¸ ÒÎÂ‰Û˛˘ËÈ ËÌÚÂ‚‡Î
            nim <<= 1;
            lev++;
            //«‡ÔÓÏÌËÚ¸ ˝ÎÂÏÂÌÚ˚, ÓÚÌÓÒˇ˘ËÂÒˇ Í Ô‡‚ÓÈ ÔÓÎÓ‚ËÌÂ ËÌÚÂ‚‡Î‡, ‰Îˇ ·Û‰Û˘Â„Ó ËÒÔÓÎ¸ÁÓ‚‡ÌËˇ
            for(i=0;i<8;i++)
            {
                fsave[i][lev-1] = f[i+8];
                xsave[i][lev-1] = x[i+8];
            }
            //—Ó·‡Ú¸ ˝ÎÂÏÂÌÚ˚, ÓÚÌÓÒˇ˘ËÂÒˇ Í ÎÂ‚ÓÈ ÔÓÎÓ‚ËÌÂ ËÌÚÂ‚‡Î‡ ‰Îˇ ÌÂÏÂ‰ÎÂÌÌÓ„Ó ËÒÔÓÎ¸ÁÓ‚‡ÌËˇ
            qprev = qleft;
            for(i=7;i>=0;i--)
            {
                f[i*2+1] = f[i];
                x[i*2+1] = x[i];
            }
            continue;
        }
        
        if(lev >= levmax) *flag += (Float)1.0;
        else if(*nofun > nofin)
        {
            nofin <<= 1;
            levmax = LEVOUT;
            *flag += (b-x0)/(b-a);
            //“ÂÍÛ˘ÂÂ ÔÂ‰ÂÎ¸ÌÓÂ ÁÌ‡˜ÂÌËÂ „ÎÛ·ËÌ˚ ‰ÂÎÂÌËˇ ÔÓÔÓÎ‡Ï ‡‚ÌÓ 'levmax'
        }
        
        //*** ›TAœ 7 *** —ıÓ‰ËÏÓÒÚ¸ ‰Îˇ ËÌÚÂ‚‡Î‡ ËÏÂÂÚ ÏÂÒÚÓ. œË·‡‚ËÚ¸ Ó˜ÂÂ‰Ì˚Â ÒÎ‡„‡ÂÏ˚Â Í ÔÂÂÏÂÌÌ˚Ï ÒÛÏÏ‡Ï
        result += qnow;
        *errest += esterr;
        cor11 += qdiff/(Float)1023;
        
        //”ÒÚ‡ÌÓ‚ËÚ¸ ÒÎÂ‰Û˛˘ËÈ ËÌÚÂ‚‡Î
        while(nim & 1)//ÂÒÎË nim - ÌÂ˜∏ÚÌÓÂ
        {
            nim /= 2;
            lev--;
        }
        nim++;
        if(lev <= 0) break;
        
        //—Ó·‡Ú¸ ˝ÎÂÏÂÌÚ˚, ÌÂÓ·ıÓ‰ËÏ˚Â ‰Îˇ ÒÎÂ‰Û˛˘Â„Ó ËÌÚÂ‚‡Î‡
        qprev = qright[lev-1];
        x0 = x[15];
        f0 = f[15];
        for(i=0;i<8;i++)
        {
            f[i*2+1] = fsave[i][lev-1];
            x[i*2+1] = xsave[i][lev-1];
        }
    }
    
    //*** ›TAœ 8 *** «‡ÍÎ˛˜ËÚÂÎ¸Ì˚Â ÓÔÂ‡ˆËË Ë ‚˚ıÓ‰
    result += cor11;
    //Œ·ÂÒÔÂ˜ËÚ¸, ˜ÚÓ·˚ ÁÌ‡˜ÂÌËÂ ÔÂÂÏÂÌÌÓÈ '*errest' ·˚ÎÓ ÌÂ ÏÂÌ¸¯Â ÛÓ‚Ìˇ ÓÍÛ„ÎÂÌËÈ
    if(*errest == (Float)0.0) return result;
    Float temp;
    while(1)
    {
        temp = ABS(result) + *errest;
        if(temp != ABS(result)) return result;
        *errest *= (Float)2;
    }
}

void Decomp(unsigned int n, Float *A, Float *cond, int *ipvt)
{
    unsigned int i, j, k;
    int kp1, m;
    Float ek, t, anorm, ynorm, znorm, *work;
    if(!n) return;//‡ÁÏÂÌÓÒÚ¸ Ï‡ÚËˆ˚ ÌÂ ÏÓÊÂÚ ·˚Ú¸ ÌÛÎÂ‚ÓÈ
    ipvt[n-1]=1;
    if(n == 1)//—ÎÛ˜‡È Ï‡ÚËˆ˚ 1x1
    {
        *cond = 1.0;
        if(*A == 0.0) *cond = MAXIMUM;//“Ó˜Ì‡ˇ ‚˚ÓÊ‰ÂÌÌÓÒÚ¸
        return;
    }
    
    //¬˚˜ËÒÎËÚ¸ 1-ÌÓÏÛ Ï‡ÚËˆ˚ A
    anorm = 0.0;
    for(j=0;j<n;j++)
    {
        t=0.0;
        for(i=j;i<n*n;i+=n) t += ABS(A[i]);
        if (t > anorm) anorm = t;
    }
    
    //√‡ÛÒÒÓ‚‡ ËÒÍÎ˛˜ÂÌËÂ Ò ˜‡ÒÚË˜Ì˚Ï ‚˚·ÓÓÏ ‚Â‰Û˘Â„Ó ˝ÎÂÏÂÌÚ‡
    for(k=0;k<(n-1);k++)
    {
        kp1 = k + 1;
        //Õ‡ÈÚË ‚Â‰Û˘ËÈ ˝ÎÂÏÂÌÚ
        m = k*n+k;
        for(i=(kp1*n + k);i<n*n;i+=n) if(ABS(A[i]) > ABS(A[m])) m = i;
        m /= n;
        ipvt[k] = m;
        if(m != k) ipvt[n-1] = -ipvt[n-1];
        
        t = A[m*n+k];
        A[m*n+k] = A[k*(n+1)];
        A[k*(n+1)] = t;
        
        //œÓÔÛÒÚËÚ¸ ˝ÚÓÚ ¯‡„, ÂÒÎË ‚Â‰Û˘ËÈ ˝ÎÂÏÂÌÚ ‡‚ÂÌ ÌÛÎ˛
        if(t != 0.0)
        {
            //¬˚˜ËÒÎËÚ¸ ÏÌÓÊËÚÂÎË
            for(i=(kp1*n + k);i<n*n;i+=n) A[i] = -A[i]/t;
            //œÂÂÒÚ‡‚ÎˇÚ¸ Ë ËÒÍÎ˛˜‡Ú¸ ÔÓ ÒÚÓÎ·ˆ‡Ï
            for(j=kp1;j<n;j++)
            {
                int temp = k-j;
                
                t = A[m*n+j];
                A[m*n+j] = A[k*n+j];
                A[k*n+j] = t;
                
                if(t != 0.0) for(i=(kp1*n+j);i<n*n;i+=n) A[i] += A[i+temp]*t;
            }
        }
    }
    
    work = new Float[n];
    //–Â¯ËÚ¸ ÒËÒÚÂÏÛ (Ú‡ÌÒÔÓÌËÓ‚‡ÌÌ‡ˇ A)*Y = I
    for(k=0;k<n;k++)
    {
        t=0.0;
        if(k != 0) for(i=k,j=0;j<(k-1);i+=n, j++) t += A[i]*work[j];
        if(t < 0.0) ek = -1.0;
        else ek = 1.0;
        Float temp = A[k*(n+1)];
        if(temp == 0.0)
        {
            *cond = MAXIMUM;
            delete [] work;
            return;
        }
        work[k] = -(ek+t)/temp;
    }
    
    k = n-2;
    do
    {
        t = work[k];
        kp1 = k+1;
        for(i=kp1*n+k,j=kp1; j<n; i+=n,j++) t += A[i]*work[j];
        work[k] = t;
        m = ipvt[k];
        if(m != k)
        {
            t = work[m];
            work[m] = work[k];
            work[k] = t;
        }
    }while(k--);
    
    ynorm = 0.0;
    for(i=0;i<n;i++) ynorm += ABS(work[i]);
    //–Â¯ËÚ¸ ÒËÒÚÂÏÛ A*Z = Y
    Solve(n, A, work, ipvt);
    znorm=0.0;
    for(i=0;i<n;i++) znorm += ABS(work[i]);
    //ŒˆÂÌËÚ¸ Ó·ÛÒÎÓ‚ÎÂÌÌÓÒÚ¸
    *cond = anorm*znorm/ynorm;
    if (*cond < 1.0) *cond = 1.0;
    delete [] work;
}


void Solve(unsigned int n, Float *A, Float *b, int *ipvt)
{
    unsigned int k, i;
    int m;
    Float t;
    if(!n) return;
    //œˇÏÓÈ ıÓ‰
    if(n == 1)
    {
        b[0] /= *A;
        return;
    }
    
    for(k=0;k<(n-1);k++)
    {
        m = ipvt[k];
        
        t = b[m];
        b[m] = b[k];
        b[k] = t;
        
        for(i=k+1;i<n;i++) b[i] += A[i*n+k]*t;
    }
    //Œ·‡ÚÌ‡ˇ ÔÓ‰ÒÚ‡ÌÓ‚Í‡
    for(k=n-1;k>0;k--)
    {
        b[k] /= A[k*(n+1)];
        t = -b[k];
        for(i=0;i<k;i++) b[i] += A[i*n+k]*t;
    }
    b[0] /= *A;
}

void Spline(unsigned int n, Float *X, Float *Y, Float *B, Float *C, Float *D)
{
    if(n <= 2)
    {
        if(n == 2)
        {
            B[1] = B[0] = (Y[1]-Y[0])/(X[1]-X[0]);
            C[0] = C[1] = D[0] = D[1] = (Float)0;
        }
        return;
    }
    
    int i;
    Float t;
    n--;
    //œÓÒÚÓËÚ¸ Ú∏ı‰Ë‡„ÓÌ‡Î¸ÌÛ˛ ÒËÒÚÂÏÛ
    //B - ƒË‡„ÓÌ‡Î¸; D - Õ‡‰‰Ë‡„ÓÌ‡Î¸; C - Ô‡‚˚Â ˜‡ÒÚË
    D[0] = X[1] - X[0];
    C[1] = (Y[1] - Y[0])/D[0];
    for(i=1;i<(int)n;i++)
    {
        D[i] = X[i+1] - X[i];
        B[i] = (Float)2*(D[i-1]+D[i]);
        C[i+1] = (Y[i+1]-Y[i])/D[i];
        C[i] = C[i+1] - C[i];
    }
    //√‡ÌË˜Ì˚Â ÛÒÎÓ‚Ëˇ. “ÂÚ¸Ë ÔÓËÁ‚Ó‰Ì˚Â ‚ ÚÓ˜Í‡ı X[0] Ë X[n-1] ‚˚˜ËÒÎˇ˛ÚÒˇ Ò ÔÓÏÓ˘¸˛ ‡Á‰ÂÎ∏ÌÌ˚ı ‡ÁÌÓÒÚÂÈ
    B[0] = -D[0];
    B[n] = -D[n-1];
    C[0] = C[n] = (Float)0;
    
    if(n != 2)
    {
        C[0] = C[2]/(X[3] - X[1]) - C[1]/(X[2]-X[0]);
        C[n] = C[n-1]/(X[n] - X[n-2]) - C[n-2]/(X[n-1]-X[n-3]);
        C[0] *= D[0]*D[0]/(X[3]-X[0]);
        C[n] *= -D[n-1]*D[n-1]/(X[n] - X[n-3]);
    }
    
    //œˇÏÓÈ ıÓ‰
    for(i=1;i<=(int)n;i++)
    {
        t = D[i-1]/B[i-1];
        B[i] -= t*D[i-1];
        C[i] -= t*C[i-1];
    }
    
    //Œ·‡ÚÌ‡ˇ ÔÓ‰ÒÚ‡ÌÓ‚Í‡
    C[n] /= B[n];
    for(i=(n-1);i>=0;i--) C[i] = (C[i] - D[i]*C[i+1])/B[i];
    
    //¬˚˜ËÒÎËÚ¸ ÍÓ˝ÙÙËˆËÂÌÚ˚ ÔÓÎËÌÓÏÓ‚
    B[n] = (Y[n] - Y[n-1])/D[n-1] + D[n-1]*(C[n-1]+(Float)2*C[n]);
    for(i=0;i<(int)n;i++)
    {
        B[i] = (Y[i+1]-Y[i])/D[i] - D[i]*(C[i+1]+(Float)2*C[i]);
        D[i] = (C[i+1]-C[i])/D[i];
        C[i] *= (Float)3;
    }
    C[n] *= (Float)3;
    D[n] = D[n-1];
}

Float SEval(unsigned int n, Float u, Float *X, Float *Y, Float *B, Float *C, Float *D)
{
    int i = 0;
    if(n < 2) return (Float)0;// ÓÎË˜ÂÒÚ‚Ó ˝ÍÒÔÂËÏÂÌÚ‡Î¸Ì˚ı ÚÓ˜ÂÍ ÌÂ ÏÓÊÂÚ ·˚Ú¸ ÏÂÌ¸¯Â ‰‚Ûı
    if(u < X[0] || u > X[1])
    {
        int j = n, k;
        //ƒ‚ÓË˜Ì˚È ÔÓËÒÍ
        do
        {
            k = (i + j)/2;
            if(u < X[k]) j = k;
            else i = k;
        }while(j > (i+1));
    }
    //¬˚˜ËÒÎËÚ¸ ÒÔÎ‡ÈÌ
    Float dx = u - X[i];
    return Y[i] + dx*(B[i] + dx*(C[i] + dx*D[i]));
}
