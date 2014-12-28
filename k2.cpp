//
//  main.cpp
//  kr5
//
//  Created by Мария Волкова on 26.12.14.
//  All rights reserved.

//Во входном файле in.txt содержатся две строчки целых чисел, например:

//-1 5 -7 -3 9 4 3 5
//4 6 -7 2 10 -7

//Строчки могут иметь различную длину. Вывести в выходной файл out.txt все числа, которые присутствуют только в одной строчке входного файла (то есть, присутствуют в первой и отсутствуют во второй, или, наоборот, присутствуют во второй и отсутствуют в первой). Каждое число должно присутствовать в выходном файле не более одного раза (даже если оно встречается два и более раз в одной из строчек входного файла). Например:
//-1 5 -3 9 3 6 2 10

#include <iostream>
#include <fstream>
#include <string>
int main(void) {
    using namespace std;
    setlocale(LC_ALL, "rus");
    ifstream fin("in.txt");
    ofstream fout("out.txt");
  
    
    if (!fin.is_open())
    {
        cout<<"такого файла нет";
        return -1;
    }
    int q=0;
    string str;
    
    getline(fin, str);
    fin.seekg(0);
    unsigned long c=str.length()/2-1;
    int *mass1 =new int [c];
    for (int i=0; i<c; i++)
    {
        fin>>mass1[i];
    }
    for (int i=0; i<c; i++)
    {
        cout<<mass1[i]<<" ";
    }
    
    cout<<"\n";
    
    fin.seekg(0);
    getline(fin, str);
    fin.seekg(str.length());
    
    unsigned long b=str.length()/2-3;
    int *mass2 =new int [b];
    for (int i=0; i<b; i++)
    {
        fin>>mass2[i];
    }
    for (int i=0; i<b; i++)
    {
        cout<<mass2[i]<<" ";
    }
  
    for (int i=0;i<c;i++)
    {
        for (int j=0;j<b;j++)
        {
            if (mass1[i]==mass2[j])
            {
                q++;
            }
        }
        if (q==0) fout<<mass1[i]<<" ";
        q=0;
    }
    
    for (int i=0;i<b;i++)
    {
        for (int j=0;j<c;j++)
        {
            if (mass2[i]==mass1[j])
            {
                q++;
            }
        }
        if (q==0) fout<<mass2[i]<<" ";
        q=0;
    }
    
    cout<<"\n";
    delete[] mass1;
    delete[] mass2;
    return 0;
}
