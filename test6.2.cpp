//
//  Many.cpp
//  test6
//
//  Created by Мария Волкова on 18.12.14.
//
//  Реализировать класс множество (целых чисел). Требуемые методы: +конструктор, +деструктор, ввод, +вывод, +копирование, +сложение множеств, пересечение множеств, +добавление в множество.

#include <iostream>
using namespace std;

class Many
{
    int *array;
    int size;
    
public:
    
    Many() //конструктор
    {
        array=0;
        size=0;
    }
    
    Many(int num) //конструктор с параметром
    {
        size=num;
        array=new int [size];
        for(int i = 0; i < size; i++)
        {
            array[i] = 0;
        }
    }
    
    ~Many() //деструктор
    {
        delete  [] array;
    }
    
    void outmany() // вывод
    {
        for(int i = 0; i < size; i++)
        {
            cout << array[i] << " | ";
        }
        cout << std::endl << std::endl;
    }
    
    void input ()// ввод
    {
        for (int i=0; i<size; i++)
        {
            cin>> arrInt.array[i];
        }
    }
    
   int operator+( &array arrInt, &array arrInt2) //сложение множеств
    {
        array = new int array[size*2];
        arrInt.array
        for (int i=size+1, i<size*2, i++)
        {
            int r=0;
            arrInt.array[i]=arrInt2.array[r];
            r++;
        }
        return array;
    }
    
    Many& operator =(Many &arrInt2) //копирование множеств
    {
        delete [] array;
        
        size = arrInt2.size;
        array = new int [size];
        for (int i = 0; i < size; i++)
        {
            array[i] = arrInt2.array[i];
        }
        
        return *this;
    }
    
    friend bool operator ==(Many &arrInt, Many &arrInt2 )//сравнение множеств
    {
        if(arrInt.size!= arrInt2.size) //сравниваем размеры массивов
        {
            cout << "В массивах разное количество элементов\n";
            return 0;
        }
        else //проверяем равны ли данныев в ячейках
        {
            for (int i = 0; i < arrInt.size; i++)
            {
                if(arrInt.array[i] != arrInt2.array[i])
                {
                    cout << "Значения массивов не равны\n";
                    return 0;
                }
            }  
        }
        
        return 1;
    }
    
    int add ( &array arrInt ) //добовление в множество
    {
        arrInt.array[size+1]=ad;
    }
    
    
    
    int &operator[](int j) //перегрузка []
    {
        return array[j];
    }
};
    




int main() {
setlocale (LC_ALL , "rus");

Many ArrayInt(5);    //создаем объект класса и записываем в него данные

cout << "Введите массив ArrayInt:  ";
ArrayInt.input();
cout << "Массив ArrayInt:  ";
ArrayInt.outmany();
    
Many ArrayInt2(5); //создаем второй объект класса
    
cout << "Введите массив ArrayInt2:  ";
ArrayInt2.input();
cout << "Массив ArrayInt2: ";
ArrayInt2.outmany();   //вывод данных массива на экран

if(ArrayInt == ArrayInt2)//сравнение двух объектов класса
{
    cout << "Массив ArrayInt равен массиву ArrayInt2\n";
} else cout<<"!Массивы не равны";
    
Many ArrayInt3(10); // третий объект класса с массивом из 10 элементов
    
cout << "Массив ArrayInt3: ";
ArrayInt3.outmany();
ArrayInt3 = ArrayInt; // копируем массив из ArrayInt в ArrayInt3
cout << "Массив ArrayInt3 после копирования: \n";
ArrayInt3.outmany();
    
int ad=0;
cout << "Введите элемент который вы хотите добавить в множество ArrayInt : \n";
cin>> ad;
ArrayInt.add();
cout << "Массив ArrayInt после добавления : \n";
ArrayInt3.outmany();
    
cout << "После сложения множеств : \n";
//  вывод
 
cout<<"Пересечением множеств ArrayInt и ArrayInt2 является: ";
// пересечения
    
return 0;
}
