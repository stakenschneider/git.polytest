#pragma once

template <class T> class ISet
{
public:
    virtual void add (const T& elem) = 0; // добавить элемент
    virtual void remove (const T& elem) = 0; // удалить элемент
    virtual bool contains (const T& elem) const = 0; // содержится ли элемент
    virtual int size () const = 0; // узнать размер множества
    
};
