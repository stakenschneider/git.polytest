#pragma once

#include <iostream>

static int error = -1;

class Many
{
    friend std::istream &operator>> (std::istream &, Many &);
    friend std::ostream &operator<< (std::ostream &, const Many &);
    
private:
    int *array;
    int size;
    
public:
    Many(); //конствруктор по умолчанию
    Many(int); //конструктор с параметром
    Many(const Many& m);
    ~Many(); //деструктор
    
    int& operator[](int j); //перегрузка []
    const Many &operator= (const Many &); //копирование множества
    bool operator== (const Many &) const; //сравнение множеств
    bool operator+= (int); //добовление в множество
    Many operator+ (const Many &);//сложение множеств
    
    bool intersection(const Many &); //пересечение множеств
    
    void clear();
    int getSize() const; //возрат количества элементов множества
    int* safeCopy();
};
