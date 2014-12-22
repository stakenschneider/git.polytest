#pragma once
using std::istream;
using std::ostream;

static int error = -1;

class Many
{
	friend istream &operator>> (istream &, Many &); // перегруженный оператор ввода
	friend ostream &operator<< (ostream &, const Many &); // перегруженный оператор вывода

private:
	int *array; //указатель на массив
	int size; //размер массива

public:
	Many(); //конструктор
	Many(int); //конструктор
	~Many(); //деструктор

	int& operator[](int j);
	const Many &operator= (const Many &); //копирование множеств
	bool operator== (const Many &) const; 
	bool operator+= (int);//добавление в множество
	bool operator+ (const Many &);//сложение множеств

	bool intersection(const Many &);//пересечение множеств
	
	void clear();
	int getSize() const; // возвратить количество элементов массива
	int* safeCopy();
};
