//Created by Мария Волкова on 18.12.14.
//Реализировать класс множество (целых чисел). Требуемые методы: конструктор, деструктор, ввод, вывод, копирование, сложение множеств, добавление в множество, пересечение множеств
#include "many.h"
#include "many.cpp"

using namespace std;

int main() 
{
	setlocale(LC_ALL, "rus");
	
	Many many_one(5);
	//	Ввод множества
	cout << "Введите множество 1:" << endl;
	cin >> many_one;
	//	Вывод множества
	cout << "Множество 1:" << endl;
	cout << many_one;

	Many many_two(5);
	//	Ввод множества
	cout << "Введите множество 2:" << endl;
	cin >> many_two;
	//	Вывод множества
	cout << "Множество 2:" << endl;
	cout << many_two;

	//	Проверка работы сравнения
	cout << ((many_one == many_two) ? "Множество 1 равно множеству 2" : "Множество 1 не равно множеству 2") << endl;
	//	Проверка работы пересечения
	if (many_one.intersection(many_two))
		cout << "Множество 1, после пересечения с множеством 2 равно: " << many_one << endl;
	//	Проверка работы сложения
	if (many_one + many_two)
		cout << "Множество 1, после сложения с множеством 2 равно: " << many_one << endl;

	return false;
}
