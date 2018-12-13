//
//  Created by Маша on 21.09.15.
//  Copyright (c) 2015 Маша. All rights reserved.
//

// перевод чисел из 10ой системы счисления в  n-ую

#include <iostream>

int sis; //основание системы

void convert(const int numeric, int notation) //функция перевода чисел (число , основание системы)
{
    if(numeric)
    {
        convert(numeric/notation,notation);  //рекурсивная ф-ция
        printf("%d" , numeric%notation );
    }
}


void residue(double num , int notation) // ф-ция для дробной части
{
    for (int i = 1 ; i<9; i++)         //точность 8 знаков после запятой
    {
    double mult = num * notation;
    short int integer = (int)mult;  //выделяется целая часть
    std :: cout << integer;
    num = mult - integer;
    }
}


class Decompose
{
    int num;
    double frac;
    
public:
    void GetParts(double n , int notation) //функция выделяет целую часть и
    {
        num = (int) n;  // выделяется целая часть числа
        
        double frac = n - num; //дробная часть числа

        convert(num, sis);  // перевод целой части
        
        if (frac != 0)  //дробная часть будет переводиться в том случае если она есть
        {
        std :: cout << ".";
        residue (frac , sis); //перевод дробной части
        }
    }
};


int main()
{
    Decompose obj;
    
    double number;
    
    std :: cout << "Введите число ";
    std :: cin >> number;
    
    std :: cout << "Введите основание системы ";
    std :: cin >> sis;

    obj.GetParts(number , sis);
}
