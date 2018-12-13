//
//  Created by Тимур on 21.09.15.
//  Copyright (c) 2015 Тимур. All rights reserved.
//

// перевод чисел из 10ой системы счисления в двоичную

#include <iostream>
#include <string>

int main()
{
    double numeric;
    
    while (1){    // бесконечный цикл для проверки
    std :: cout << "Введите число ";

    std :: cin >> numeric;
    
    int num = (int) numeric;
    
    double frac = numeric - num;
    
    std :: string result;
    
    for (; num!=0; num>>=1)
        result.insert(0, num&1 ? "1" : "0"); // вставка цифры в начало строки
    
    std :: cout << result << ".";
       
    for (int i = 1 ; i<9; i++)         //точность 8 знаков после запятой
    {
        double buff = frac * 2;
        short int integer = (int)buff;  //выделяется целая часть
        std :: cout << integer;
        frac = buff - integer;
    }
    std :: cout <<"\n";
    }
}
