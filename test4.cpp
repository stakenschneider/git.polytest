//
//  Mass
//
//  Created by Мария Волкова on 22.10.14.
//  Copyright (c) 2014 Мария Волкова.
//
// Из элементов массива А{2п) получить массивы В{п) и С(п) следующим образом. Выбрать в массиве А два наиболее близких по значению элемента; меньший из них поместить в массив В, а больший — в массив С. Продолжить выбор из оставшихся элементов до полного заполнения массивов В и С.

#include <time.h>
#include <iostream>
using namespace std;

int main(void) {
    setlocale( LC_ALL, "Rus");
    int i, n, z, q, ind1, ind2, iost, temp, min, s;
    int *a;
    int *b;
    int *c;
    a=new int[i];
    b=new int[n];
    c=new int[n];
    
    cout << "Введите размерность массива A: ";
    cin >> z;
    cout << "\n";
    q=z%2;
    
    if (z<=0 || q!=0 )
    {
        cout <<"Введены неккоректные данные\n";
    } else {
        
        
    s=z/2;
    for (i=0;i<z;i++)
    {
        a[i]=rand()%100;
        cout << a[i] << endl;
    }
    cout<<"\nМассив B и C\n";
        
        min=1000;
        
        for (i=0; i<s; i++)
        {
            for (iost=i+1; iost<z-i ;iost++)
            {
            temp=abs(a[i]-a[iost]);
                if (min>temp){
            min=temp;
            ind1=i;
            ind2=iost;
                }
        
                if (ind1>ind2) {
                   b[i]=a[ind1];
                    c[i]=a[ind2];
                } else {
                   b[i]=a[ind2];
                   c[i]=a[ind1];
                }
             }
            a[ind1]=1000;
            a[ind2]=1000;
            min=1000;
        }
        
        
        for (n=0;n<s;n++)
        {
            cout << b[n] << endl;
            cout<<"\t";
            cout << c[n] << endl;
        }
        
        
    }
    delete []a;
    delete []b;
    delete []c;
    return 0;
}
