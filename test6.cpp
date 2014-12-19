//
//  Many.cpp
//  test6
//
//  Created by Мария Волкова on 19.12.14.
//
//  Реализировать класс множество (целых чисел). Требуемые методы: +конструктор, +деструктор, ввод, +вывод, +копирование, сложение множеств, пересечение множеств, добавление в множество.

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
    
    void operator +(char*) //сложение множеств
    {
        
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
    
    int &operator[](int j) //перегрузка []
    {
        return array[j];
    }
};
    




int main() {
setlocale (LC_ALL , "rus");

Many ArrayInt(5);    //создаем объект класса и записываем в него данные
ArrayInt[0] = 1;
ArrayInt[1] = 2;
ArrayInt[2] = 3;
ArrayInt[3] = 4;
ArrayInt[4] = 5;
cout << "Массив ArrayInt:  ";
ArrayInt.outmany();
    
Many ArrayInt2(5); //создаем второй объект класса
ArrayInt2[0] = 11;
ArrayInt2[1] = 22;
ArrayInt2[2] = 33;
ArrayInt2[3] = 44;
ArrayInt2[4] = 55;
cout << "Массив ArrayInt2: ";
ArrayInt2.outmany();   //вывод данных массива на экран
    
if(ArrayInt == ArrayInt2)//сравнение двух объектов класса
{
    cout << "Массив ArrayInt равен массиву ArrayInt2\n";
}
else
{
ArrayInt = ArrayInt2;
}
cout << "Массив ArrayInt после копирования:  ";
ArrayInt.outmany();

Many ArrayInt3(10); // третий объект класса с массивом из 10 элементов
cout << "Массив ArrayInt3: ";
ArrayInt3.outmany();
ArrayInt3 = ArrayInt; // копируем массив из ArrayInt в ArrayInt3
cout << "Массив ArrayInt3 после копирования: \n";
ArrayInt3.outmany();
    
return 0;
}
