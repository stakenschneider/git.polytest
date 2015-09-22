#define _MATH_USE_DEFINE

#include<iostream>

#include<cmath>

#include<math.h>

using namespace std;

int main(void)

{

double a, b, c, x1, x2;

a=1;

b=10000;

c=0.25;

double d = sqrt( b*b-4*a*c ); //дискриминант

if (d==0) x1 = (-b)/(2*a);

if (d>0)

{

x1 = (-b+d)/(2*a);

x2 = (-b-d)/(2*a);

}

if (d<0) { cout << "Квадратных корней нет\n\n"; }

return 0;

}

//алгоритм для типа float

#define _MATH_USE_DEFINE

#include<iostream>

#include<cmath>

#include<math.h>

using namespace std;

int main(void)

{

float a, b, c, x1, x2;

a=1;

b=10000;

c=0.25;

float d = sqrt( b*b-4*a*c ); //дискриминант

if (d==0) x1 = (-b)/(2*a);

if (d>0)

{

x1 = (-b+d)/(2*a);

x2 = (-b-d)/(2*a);

}

if (d<0) { cout << "Квадратных корней нет\n\n"; }

return 0;

}
