#define _MATH_USE_DEFINE
#include<iostream>
#include<cmath>
#include<math.h>



using namespace std;
int main(void)
{
    setlocale( LC_ALL, "Rus");
    double a, b, d, c, x1, x2;
    
    cout << "Введите коэффициенты квадратного уравнения:\n";
    cin >> a >> b >> c;
    cout << "\n";
    d = sqrt( b*b-4*a*c );   //дискриминант
 
    //стандартный алгоритм поиска корней  
    if (d==0)                       
    {  
		x1 = (-b)/(2*a);
       cout << "Дискриминант\n" << "x: "<< x1 << endl; 
	}
    
    if (d>0)
    { 
	   x1 = (-b+d)/(2*a);
       x2 = (-b-d)/(2*a);
       cout << "Дискриминант\n" << "x1: " << x1 << "  x2: " << x2 << endl; 
	}
    
    if  (d<0) { cout << "Квадратных корней нет\n\n"; }
	


	//алкоритм Виета
	for(double i=-(abs(a)+abs(b)+abs(c)); i<=abs(a)+abs(b)+abs(c) ; i+=0.1)
      {
         for(double j=i; j<=abs(a)+abs(b)+abs(c); j+=0.1)
         {
            if(i+j == -(b/a) && i*j == c/a)
            {
                x1=j;
                x2=i;
                cout << "Виета\n"<< "x1: " << x1 << "  x2: " << x2 << endl; 
            }
        }
      }



    system("pause");
    return 0;
}
