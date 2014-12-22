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

 
Many::~Many()
{
	clear();
}

int& Many::operator[](int j) //перегрузка []
{
	if (j < 0 || j > size)
	{
		std::cerr << "Попытка обращения к несуществующему элементу множества." << std::endl;
		return error;
	}
	return array[j];
}

istream &operator>> (istream & is, Many &m)
{
	is.clear();
	for (int ix = 0; ix < m.size; ix++)
		is >> m.array[ix]; // заполняем массив объекта obj 
	return is; // позволяет множественный ввод, типа cin >> x >> y >> z >> ...
}

ostream &operator<< (ostream &os, const Many &m)
{
	if (m.size > 0)
	{
		os << m.array[0];
		for (int ix = 1; ix < m.size; ix++)
			os << ", " << m.array[ix];
	}
	os << std::endl; 
	return os; 
}

const Many &Many::operator= (const Many &m) // оператор присваивания
{
	if (&m != this) // чтобы не выполнялось самоприсваивание
	{
		if (size != m.size)
		{
			delete[] array; // освободить пространство
			size = m.size; // установить нужный размер массива
			array = new int[size]; // выделить память под копируемый массив
		}

		for (int ix = 0; ix < size; ix++)
			array[ix] = m.array[ix]; // скопировать массив
	}
	return *this; // разрешает множественное присваивани
}

bool Many::operator+ (const Many &m)
{
	//	---		copy	---
	int* safe = safeCopy();
	//	---		clear	---
	if (safe != NULL)	delete[] array;
	array = new int[size + m.size];
	//	---		set		---
	int i = 0;
	if (safe != NULL)
	{
		for (; i < size; i++)
			array[i] = safe[i];
	}
	delete[] safe;
	for (int j = 0; j < m.size; j++, i++)
		array[i] = m.array[j];
	size += m.size;
	return true;
}

bool Many::operator+= (int val) // оператор добавления элемента в конец массива
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

bool Many::operator== (const Many &m) const	// оператор сравнения
{
	if (size != m.size)
		return false; // массивы с разным количеством элементов
	for (int ix = 0; ix < size; ix++)
		if (array[ix] != m.array[ix])
			return false; // массивы не равны
	return true; // массивы равны
}

bool Many::intersEction(const Many &m)
{
	//	copy
	int* safe = safeCopy();
	int s = size;
	if (safe != NULL)
	{
		 //	clear
		clear();
		//	set
		for (int i = 0; i < s; i++)
			for (int j = 0; j < m.size; j++)
				if (safe[i] == m.array[j])
					*this += safe[i];
	}
	delete[] safe;
	return true;
}

//	utils
void Many::clear()
{
	delete[] array;
	size = NULL;
}

int Many::getSize() const // возвратить количество элементов массива
{
	return size;
}

int* Many::safeCopy()
{
	if (size <= 0)
		return NULL;
	//	copy
	int* safe = new int[size];
	for (int i = 0; i < size; i++)
		safe[i] = array[i];
	return safe;
}
