//
//  main.cpp
//  kr4
//
//  Created by Мария Волкова on 26.12.14.
//  Copyright (c) 2014 Мария Волкова. All rights reserved.

//Во входном файле in.txt содержится последовательность цифр от 0 до 9, например:

//1 4 4 4 3 3 2 2 2 2 4 3 3 3 0 0 0 0 1 2 3 5 1 1 1 1 1 5 5 5 5 1 5 5 5

//Найти в файле самую длинную непрерывную последовательность из одинаковых цифр (в примере выделена жирным шрифтом). Вывести в выходной файл out.txt образующую ее цифру и длину, например:
//1, длина 5

//Если таких последовательностей несколько, вывести информацию о любой из них.

#include <iostream>
#include <fstream>

int main(void) {
    using namespace std;
    setlocale(LC_ALL, "rus");
    ifstream fin("in.txt");
    ofstream fout("out.txt");
    int n=1, b=0;
    if (!fin.is_open())
    {
        cout<<"такого файла нет";
        return -1;
    }

    int i=0, max=0, c=0;
    string str;
    getline(fin, str);
    unsigned long q=str.length();
    
    int *mass=new int[q];
    
    fin.seekg(0);
    while (!fin.eof()) {
        fin>>mass[i];
        i++;
    }
    
    cout<<"\n";
    
    for (int i=0; i<q; i++)
    {
        if (mass[i]==mass[i+1]) {
            n++;
            b=mass[i];
        } else {
            if(n>max){
                max=n;
                c=b;
            }
            n=1;
        }
    }
    
    fout<<c<<", length "<<max<<endl;
    
    fin.close();
    fout.close();
    delete [] mass;
    return 0;
}
