// volkova maria 13501/3
// вводится дд чч мм отправления поезда и время в пути в том же формате. на вывод время прибытия в дд чч мм

#include <iostream>
using namespace std;
int main(void)
{
    int n,h1,h2,m1, m2, m, h3, m3, d3,q;
    cout << "Departure time (dd,hh,mm): ";
    cin >>n>>h1>>m1;
    cout <<"In-way time (dd,hh,mm): ";
    cin >>m>>h2>>m2;
    m3=(m1+m2)%60;                    //находим остаток от деления минут
    q=(m1+m2)/60;                     //выделяем часы
    h3=(h1+h2+q)%24;                  //количество дней
    d3=m+n+(h1+h2+q)/24;              //кол во дней
    cout <<"Arrive time "<<"\n"<<"Day:"<<d3<<"\n"<<"Hour:"<<h3<<"\n"<<"Min:"<<m3<< endl;
    return 0;
}

