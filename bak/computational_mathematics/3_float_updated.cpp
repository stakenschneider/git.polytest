#define _MATH_USE_DEFINE
#include<iostream>
#include<cmath>
#include <fstream>

int main (void)
{
	float Sn, An, x;
	double E = 0.00001;
	An = 1.0;
	Sn = 0.0;
	x = 10.0;
	std::ofstream fout("output.txt" , std::ios::app);  

	for (int n = 1;; n++)
	{
		Sn = Sn + An;
		An = An*(x) / n;

		float e = abs(An) / abs(Sn);

		float Er = abs(Sn - exp(-x));  // относительная погрешность 
		float Ea = abs(Sn - exp(-x)) / exp(-x);   // абсолютная погрешность  

		fout << "n=" << n<< " Sn=" << Sn << " An=" << An <<" Aбсолютная:" << Ea << " Относительная:"<< Er<<std :: endl; 
		if (e < E)
		{
			Sn = 1 / Sn;
			fout << "n=" << n<< " Sn=" << Sn << " An=" << An <<" Aбсолютная:" << Ea << " Относительная:"<< Er<<std :: endl; 
			break;
		}
	}
	return 0;
}

