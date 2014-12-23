#include "many.h"


Many::Many()
{
    array = NULL;
    size = NULL;
}

Many::Many(int count)
{
    size = count;
    array = new int[size];
    for (int i = 0; i < size; i++)
        array[i] = 0;
}

Many::Many(const Many& m)
{
    if (&m != this)
    {
        if (size != m.size)
        {
            delete[] array;
            size = m.size;
            array = new int[size];
        }
        
        for (int ix = 0; ix < size; ix++)
            array[ix] = m.array[ix];
    }
}

Many::~Many()
{
    
    clear();
}

std::istream &operator>> (std::istream & is, Many &m)
{
    is.clear();
    for (int ix = 0; ix < m.size; ix++)
        is >> m.array[ix];
    return is;
}

std::ostream &operator<< (std::ostream &os, const Many &m)
{
    if (m.size > 0)
    {
        os << m.array[0];
        for (int ix = 1; ix < m.size; ix++)
            os << " " << m.array[ix];
    }
    os << std::endl;
    return os;
}


int& Many::operator[](int j)
{
    if (j < 0 || j > size)
    {
        return error;
    }
    return array[j];
}

const Many &Many::operator= (const Many &m)
{
    if (&m != this)
    {
        if (size != m.size)
        {
            delete[] array;
            size = m.size;
            array = new int[size];
        }
        
        for (int ix = 0; ix < size; ix++)
            array[ix] = m.array[ix];
    }
    return *this;
}


Many Many::operator+ (const Many &m)
{
    Many result;
    for(int i = 0; i < size; i++)
        result += array[i];
    for(int i = 0; i < m.size; i++)
        result += m.array[i];
    return result;
    
}

bool Many::operator+= (int val)
{

    int* safe = safeCopy();
    if (safe != NULL)	delete[] array;
    array = new int[++size];
    for (int i = 0; i < (size - 1); i++)
        array[i] = safe[i];
    array[size - 1] = val;
    delete[] safe;
    return true;
}

bool Many::operator== (const Many &m) const
{
    if (size != m.size)
        return false;
    for (int ix = 0; ix < size; ix++)
        if (array[ix] != m.array[ix])
            return false;
    return true;
}


bool Many::intersection(const Many &m)
{
 
    int* safe = safeCopy();
    int s = size;
    if (safe != NULL)
    {
 
        clear();
        for (int i = 0; i < s; i++)
        {
            for (int j = 0; j < m.size; j++)
            {
                if (safe[i] == m.array[j])
                {
                    *this += safe[i];
                    break;
                }
            }
        }			
    }
    delete[] safe;
    return true;
}


void Many::clear()
{
    delete[] array;
    size = NULL;
}

int Many::getSize() const
{
    return size;
}

int* Many::safeCopy()
{
    if (size <= 0)
        return NULL;
    int* safe = new int[size];
    for (int i = 0; i < size; i++)
        safe[i] = array[i];
    return safe;
}
