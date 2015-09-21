#include <iostream>
#include <string>

using namespace std;

string dectobin(int num) // функция перевода
{
    string result; // строка для результата
    for (; num!=0; num>>=1) // цикл со сдвигом вправо на 1 бит (работает быстрее чем деление на 2,
        result.insert(0, num&1 ? "1" : "0"); // вставка цифры в начало строки (1 если последний бит 1, 0 если 0)
    return result; 
}

int main()
{
    int num;
    cout << "Enter number: ";
    cin >> num;
    cout << dectobin( num );
    return 0;
}
