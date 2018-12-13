#define _MATH_USE_DEFINE
#include<iostream>
#include<cmath>
#include <fstream>

float E = 0.00001;

void alg(float Sn, float An, float x)
{
	for (int n = 1; ; n++)
	{ 
		std::ofstream fout("output.txt" , std::ios::app); 

		Sn = Sn + An;     // сумма предыдущих элементов
	    An = An*(-x) / n; // новый элемент

	    float Er = abs(Sn - exp(-x));  // относительная погрешность
	    float Ea = abs(Sn - exp(-x)) / exp(-x);   // абсолютная погрешность 

	    fout << "n=" << n<< " Sn=" << Sn << " An=" << An <<
" Aбсолютная:" << Ea << " Относительная:"<< Er<<std::endl;


	    float e = abs(An) / abs(Sn);

	    if (e < E) break; // выходит из цикла на предложенной погрешности (10^(-5))
	}
}

int main (void)
{

	alg(0.0 , 1.0 , 0.5);  // вычисления для x = 0.5
	alg(0.0 , 1.0 , 5.0);  // вычисления для x = 5
	alg(0.0 , 1.0 , 10.0); // вычисления для x = 10

	system("pause");
	return 0;
}
