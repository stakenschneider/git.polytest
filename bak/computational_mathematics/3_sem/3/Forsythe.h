//-------------------------------------------------------------------
//		ForsytheLib v 0.1 beta 5
//	œÓ„Ë ËÁ ‘ÓÒ‡ÈÚÓ‚ÒÍÓ„Ó Ô‡ÍÂÚ‡, ÔÂÂÔËÒ‡Ì˚Â Ò Fortran'‡ Ì‡ —++
//
//	Copyright (c) 2002 Eugene S.B.
//
//	If you find any bugs in this program, please, contact me.
//	E-Mail: <esb@hotbox.ru>
//-------------------------------------------------------------------

#ifndef __FORSYTHE_H__
#define __FORSYTHE_H__

#include <float.h>

#define FT_FLOAT	0	//'float'
#define FT_DOUBLE	1	//'double'
#define FT_LDOUBLE	2	//'long double'

//«‰ÂÒ¸ ÏÓÊÌÓ ÔÂÂÓÔÂ‰ÂÎËÚ¸ ‚Â˘ÂÒÚ‚ÂÌÌ˚È ÚËÔ, ÍÓÚÓ˚È ·Û‰ÂÚ ËÒÔÓÎ¸ÁÓ‚‡Ú¸Òˇ ‚ ‚˚˜ËÒÎÂÌËˇı.
//≈ÒÎË ‚‡ÊÌ‡ ÒÍÓÓÒÚ¸ - ËÒÔÓÎ¸ÁÛÈÚÂ 'float' (Ú.Â. FT_FLOAT), ÂÒÎË ÌÛÊÌ‡ ÚÓ˜ÌÓÒÚ¸ - ËÒÔÓÎ¸ÁÛÈÚÂ 'double' (FT_DOUBLE)
//ËÎË 'long double' (FT_LDOUBLE).
#define FLOATTYPE	FT_DOUBLE

#if FLOATTYPE == FT_FLOAT
typedef float Float;
#define EPSILON FLT_EPSILON
#define MAXIMUM FLT_MAX
#elif FLOATTYPE == FT_DOUBLE
typedef double Float;
#define EPSILON DBL_EPSILON
#define MAXIMUM DBL_MAX
#elif FLOATTYPE == FT_LDOUBLE
typedef long double Float;
#define EPSILON LDBL_EPSILON
#define MAXIMUM LDBL_MAX
#else
#error Invalid Float Type
#endif

//ÕÂÍÓÚÓ˚Â ÔÓÎÂÁÌ˚Â Ï‡ÍÓÒ˚
#define ABS(x) (((x) >= 0) ? (x) : -(x))
#define MAX(x, y) (((x) >= (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define SIGN0(x) (((x) > 0) ? 1 : -1)
#define SIGN(x) (((x) == 0) ? 0 : SIGN0(x))
#define SIGN2(a, b) (SIGN(b)*ABS(a))

//—ÚÛÍÚÛ‡ ‰Îˇ ‚ıÓ‰Ì˚ı Ë ‚˚ıÓ‰Ì˚ı Ô‡‡ÏÂÚÓ‚ rkf45
struct rkf
{
    void (*f)(Float t, Float *Y, Float *dY);
    Float *Y, t, tout, re, ae;
    unsigned int neqn;
    int flag;
    unsigned char *work;//ÔÓ ˝ÚÓÏÛ ÛÍ‡Á‡ÚÂÎ˛ ÌÂÓ·ıÓ‰ËÏÓ ‚˚‰ÂÎËÚ¸ '6*neqn*sizeof(Float) + sizeof(struct rkf_inside)' ·‡ÈÚ
};

// ÓÌÒÚ‡ÌÚ˚ ‰Îˇ ÛÔ‡‚ÎÂÌËˇ ‡·ÓÚÓÈ rkf45

//	REMin - ˝ÚÓ ÏËÌËÏ‡Î¸ÌÓÂ ‰ÓÔÛÒÚËÏÓÂ ÁÌ‡˜ÂÌËÂ ‰Îˇ ÓÚÌÓÒËÚÂÎ¸ÌÓÈ ÔÓ„Â¯ÌÓÒÚË (re). œÓÔ˚ÚÍË Á‡ÔÓÒËÚ¸ Û
//ÙÛÌÍˆËË ·ÓÎÂÂ ‚˚ÒÓÍÛ˛ ÚÓ˜ÌÓÒÚ¸ Ó·˚˜ÌÓ ÌÂ˝ÙÙÂÍÚË‚Ì˚, Ë ÔÓ˝ÚÓÏÛ ‰ÓÎÊÌ˚ ËÁ·Â„‡Ú¸Òˇ.
#define REMIN (1.0e-12) //Relative error minimum

//	Ã‡ÍÒËÏ‡Î¸ÌÓÂ ‰ÓÔÛÒÚËÏÓÂ ÁÌ‡˜ÂÌËÂ ÍÓÎÎË˜ÂÒÚ‚‡ ‚˚˜ËÒÎÂÌËÈ ÔÓËÁ‚Ó‰Ì˚ı (Ú.Â. ÍÓÎÎË˜ÂÒÚ‚Ó ‚˚˜ËÒÎÂÌËÈ ÙÛÌÍˆËË f)
//œËÌˇÚÓÂ Á‰ÂÒ¸ ÁÌ‡˜ÂÌËÂ ÒÓÓÚ‚ÂÚÒÚ‚ÛÂÚ ÔËÏÂÌÓ 5000 ¯‡„‡Ï
#define MAXNFE 30000 //Maximum number of Function E...?

/* —ÚÛÍÚÛ‡ ‰Îˇ ‚ÌÛÚÂÌÌÂ„Ó ËÒÔÓÎ¸ÁÓ‚‡ÌËˇ ‚ rkf45 */
//		E∏ Ì‡Ë·ÓÎÂÂ ËÌÚÂÂÒÌ˚Â ÔÓÎˇ:
// h	-	ÔÂ‰ÔÓÎ‡„‡ÂÏ˚ı ‡ÁÏÂ ¯‡„‡ ‰Îˇ Ó˜ÂÂ‰ÌÓ„Ó ˝Ú‡Ô‡
// nfe	-	Ò˜∏Ú˜ËÍ ˜ËÒÎ‡ ‚˚˜ËÒÎÂÌËÈ ÙÛÌÍˆËË
struct rkf_inside
{
    unsigned long nfe, kop, init;
    int JFlag, KFlag;
    Float h, SaveRE, SaveAE;
};
//		≈ÒÎË ‚‡Ò ËÌÚÂÂÒÛ˛Ú ÁÌ‡˜ÂÌËˇ ÔÓÎÂÈ ‰‡ÌÌÓÈ ÒÚÛÍÚÛ˚ ÔÓÒÎÂ ‡·ÓÚ˚ rkf45,
// ÚÓ Ëı ÏÓÊÌÓ ÔÓÎÛ˜ËÚ¸ ˜ÂÂÁ ÛÍ‡Á‡ÚÂÎ¸ work ‚ 'struct rkf', Ì‡ÔËÏÂ:
/*
	struct rkf_inside *p;
	struct rkf RKF;
	unsigned char work[6*(NEQN*sizeof(Float)) + sizeof(struct rkf_inside)];
	...
	RKF.f = ...
	RKF.t = ...
	RKF.work = work;
	...
	rkf45(&RKF);
	p = (struct rkf_inside *)RKF.work;
	... = p->nfe;
	... = p->h;
	...
 */

//****************** ÃÂÚÓ‰ –ÛÌ„Â- ÛÚÚ‡ ‘ÂÎ¸·Â„‡ 4-5 ÔÓˇ‰Í‡ ******************
//	œÓ Ò‡‚ÌÂÌË˛ Ò ÓË„ËÌ‡Î¸ÌÓÈ ‚ÂÒËÂÈ ˝ÚÓÈ ÔÓ„‡ÏÏ˚ (‘ÓÚ‡ÌÓ‚ÒÍÓÈ) Ò‰ÂÎ‡Ì˚ ÌÂÍÓÚÓ˚Â ËÁÏÂÌÂÌËˇ:
//		- ÔË ÏÌÓ„ÓÍ‡ÚÌ˚ı ‚˚ÁÓ‚‡ı rkf45 Ì‡ ÒÍÓÓÒÚ¸ ÒËÎ¸ÌÓ ‚ÎËˇÂÚ ÍÓÎÎË˜ÂÒÚ‚Ó Ô‡‡ÏÂÚÓ‚ ÙÛÌÍˆËË,
// ÔÓ˝ÚÓÏÛ Á‰ÂÒ¸ Ô‡‡ÏÂÚ ÚÓÎ¸ÍÓ 1. “ÂÏ ÌÂ ÏÂÌÂÂ ‚ÒÂ ÓË„ËÌ‡Î¸Ì˚Â Ô‡‡ÏÂÚ˚ Ì‡ıÓ‰ˇÚÒˇ ‚ ÒÚÛÍÚÛÂ struct rkf.
//		- ‘ÓÚ‡ÌÓ‚ÒÍËÂ Ô‡‡ÏÂÚ˚ WORK Ë IWORK Ó·˙Â‰ËÌÂÌ˚ ‚ 1 (work; ÒÏ. ÓÔËÒ‡ÌËÂ struct rkf)
//		- œË ÌÓÏ‡Î¸ÌÓÈ ‡·ÓÚÂ ÙÛÌÍˆËˇ ‚ÓÁ‚‡˘‡ÂÚ false. ¬ ÒÎÛ˜‡Â ÊÂ Ù‡Ú‡Î¸Ì˚ı Ó¯Ë·ÓÍ ·Û‰ÂÚ ‚ÓÁ‚‡˘ÂÌÓ true.
//		!!!!!!  —ÎÂ‰ÛÂÚ Û˜ËÚ˚‚‡Ú¸, ˜ÚÓ ÂÒÎË ‰‡ÊÂ rkf45 ‚ÂÌÛÎ‡ false, ÚÓ ËÁ ˝ÚÓ„Ó ÌÂ ÒÎÂ‰ÛÂÚ, ˜ÚÓ ‚Ò∏ ÌÓÏ‡Î¸ÌÓ -> ÌÛÊÌÓ
// Ó·ˇÁ‡ÚÂÎ¸ÌÓ ÔÓ‚ÂËÚ¸ ÁÌ‡˜ÂÌËÂ ÙÎ‡„‡ Ì‡ ‚˚ıÓ‰Â, ÚÓÎ¸ÍÓ ÚÓ„‰‡ ÏÓÊÌÓ ·Û‰ÂÚ Û·Â‰ËÚ¸Òˇ, ˜ÚÓ ËÌÚÂ„ËÓ‚‡ÌËÂ Á‡‚Â¯ËÎÓÒ¸ ÛÒÔÂ¯ÌÓ.
// ƒ‡ÌÌ˚È ÏÂı‡ÌËÁÏ Í‡ÊÂÚÒˇ ÏÌÂ ·ÓÎÂÂ „Ë·ÍËÏ, ˜ÂÏ ÚÓÚ ÍÓÚÓ˚È ÔÂ‰ÎÓÊÂÌ ÔÂ‚ÓÌ‡˜‡Î¸Ì˚ÏË ‡‚ÚÓ‡ÏË (‚ ÒÚ‡ÓÈ ‚ÂÒËË
// ÔË ‚ÓÁÌËÍÌÓ‚ÂÌËË Ù‡Ú‡Î¸Ì˚ı Ó¯Ë·ÓÍ Á‡‚Â¯‡Î‡Ò¸ ‚Òˇ ÔÓ„‡ÏÏ‡, ÌÂ ‰‡‚‡ˇ ÔÓ„‡ÏÏËÒÚÛ ‚ÓÁÏÓÊÌÓÒÚË Ó·‡·ÓÚ‡Ú¸ ‰‡ÌÌÛ˛ ÒËÚÛ‡ˆË˛)

//Copyright (c) 1976 H.A.Watts, L.F.Shampine (Sandia Laboratories, Albuquerque, New Mexico)
bool rkf45(struct rkf *p);

//Copyright (c) 1973 Ricard Brent
Float FMin(Float (*F)(Float), Float a, Float b, Float tol);
Float Zeroin(Float (*F)(Float), Float a, Float b, Float tol);

Float Quanc8(Float (*F)(Float), Float a, Float b, Float ae, Float re, Float *errest, int *nofun, Float *flag);
void Decomp(unsigned int n, Float *A, Float *cond, int *ipvt);
void Solve(unsigned int n, Float *A, Float *b, int *ipvt);
void Spline(unsigned int n, Float *X, Float *Y, Float *B, Float *C, Float *D);
Float SEval(unsigned int n, Float u, Float *X, Float *Y, Float *B, Float *C, Float *D);

#endif //__FORSYTHE_H__
