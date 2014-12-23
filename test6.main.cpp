//
//  main.cpp
//  test6
//
//  Created by Мария Волкова on 23.12.14.
//  Copyright (c) 2014 Мария Волкова. All rights reserved.
//

#include <stdio.h>
#include <iostream>
#include "many.h"

using namespace std;

int main()
{
    setlocale(LC_ALL, "rus");
    
    Many many_one(5);

    cout << "Введите множество 1:" << endl;
    cin >> many_one;

    cout << "Множество 1:" << endl;
    cout << many_one;
    
    Many many_two(5);

    cout << "Введите множество 2:" << endl;
    cin >> many_two;

    cout << "Множество 2:" << endl;
    cout << many_two;

   cout << ((many_one == many_two) ? "Множество 1 равно множеству 2" : "Множество 1 ne равно множеству 2") << endl;
   
    Many many_tri;
    many_tri = many_one + many_two;
    cout << "Сложение множеств 1 и 2: " << many_tri << endl;
    
    if (many_one.intersection(many_two))
    cout << "Пересечение множества 1 и 2: " << many_one << endl;

    
    many_two += 10;
    
    cout << many_two;
    return false;
}
