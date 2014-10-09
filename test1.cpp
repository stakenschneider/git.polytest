//volkova maria 13501/3
//вычисление квадратных корней уравнения



#define _MATH_USE_DEFINE
#include<iostream>
#include<cmath>
using namespace std;
int main(void)
{
    setlocale( LC_ALL, "Rus");
    int a, b, d, c, x1, x2;
    
    cout << "Введите коэффициенты квадратного уравнения:\n";
    cin >>a>>b>>c;
    cout << "\n";
    d=sqrt(b*b-4*a*c);   //считаем дискриминант
 
    //далее стандартный алгоритм поиска корней
    
    if (d==0)                       
    {  x1=(-b)/(2*a);
       cout <<"x: "<< x1 <<endl; }
    
    if (d>0)
    {  x1=(-b+d)/(2*a);
       x2=(-b-d)/(2*a);
       cout <<"x1:"<<x1<<"  x2:" <<x2<< endl; }
    
    if  (d<0) { cout<<"Квадратных корней нет\n\n"; }
    
    return 0;
}
