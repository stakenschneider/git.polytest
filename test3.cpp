//
//  вариант 6
//  Created by Мария Волкова on 20.09.14.
//  Copyright (c) 2014 Мария Волкова. All rights reserved.
// вывести на экран таблицу пересчета миль в км и обратно до заданного расстояния в км по возраст как указано в примере (1 миля = 1.609 км )

#include <iostream>

#define K_TO_M 0.6214   //оператор определяет идентификатор K_TO_M как целочисленную константу ( тоже и след строка)
#define M_TO_K 1.6093
using namespace std;
int main(void) {
    int km;
    int k_Cntr = 1;
    int m_Cntr = 1;
    float k1, m1, k2, m2;
    
    cout << "Enter km: ";
    cin >> km;
    cout <<"\n";
    if (km>0 )
    {
    cout << "Miles \t Kilometers" ;
    cout << "\n";
    
    while(1) {                                        //цикл для всех вычислений
        m1 = K_TO_M * k_Cntr;
        k1 = k_Cntr;
        k2 = M_TO_K * m_Cntr;
        m2 = m_Cntr;
        
        if (m1 <= m2) {                                  //проверки для вывода нужных чисел м/км
            cout << m1 << "\t\t\t" << k1 <<"\n";
            ++k_Cntr;                                    //целые знач км для 16 стрчки
            if(k1 >= km) break;                        //выход из цикла если значение больше или равно нашему заданному
        } else {
            cout << m2 << "\t\t\t" << k2 <<"\n";
            ++m_Cntr;                                   //целые знач миль для 18 стрчки
            if(k2 >= km) break;                       //выход из цикла если значение больше или равно нашему заданному
        }
    }
    }
    else cout<<"Некорректные данные\n";
   
    return 0;
}
