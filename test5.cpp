//
//  test5
//
//  Created by Мария Волкова on 29.10.14.
//
// входной файл содержит последовательность команд совершаемых над специальным устройством представляющим собой конвеер на N ячеек и датчик двигающийся над этим конвеером

#include <iostream>
#include <string>
#include <fstream>

int main(void)

{
    using namespace std;
    setlocale(LC_ALL, "rus");
    int  n,i,cmmnd;
  
    ifstream in;  // Поток in будем использовать для чтения
    ofstream out; // Поток out будем использовать для записи

      in.open ("input.txt");
        in >> cmmnd;
        cout<<cmmnd;
        in.close();
 
    if (cmmnd<0) cout<<"Некоректный ввод"; else {
        
    int *dtc;     // указатель который бегает по массиву
        
    dtc = new int[cmmnd];       //массив для строки

 
    n=cmmnd/2;  //стартовая позиция указателя
    
    for (int i = 0; i < cmmnd; i++) /*цикл построения нач строки (все 0)*/ dtc[i] = 0;
    cout<<"\n";
    
    string str;
    in.open ("input.txt");
    getline(in, str); // getline – читает строку из файла до символа перевода строки char(10)
    in.close();
        
        
    for (int i = 0; i < cmmnd; i++)  //цикл для прохождения каждого символа строки
    {
        switch ( str[i] )
        {
            case '>':
                n=n+1;
                break;
            case '<':
                n=n-1;
                break;
            case '-':
                dtc[n]=dtc[n]-1;
                break;
            case '+':
                dtc[n]=dtc[n]+1;
                break;
            case '*':
               out.open("output.txt"); // создаём объект класса ofstream для записи и связываем его
               out << dtc[n]; // запись строки в файл
               out.close(); // закрываем файл
                
                break;
            case '[':
               if (dtc[n]==0)
              {
                  for (int j=i; j!='/0' ;j++)
                      i=j+1;
               }
                break;
            case ']':
                if (dtc!=0) {
                    for (int j=i; j!=0 ;j--)
                        i=j+1;
                }
                break;
           // default: ;
                
        }
    }
    
   for (int i = 0; i < cmmnd; i++)
       cout<<dtc[i];
    
    delete[]dtc;
    }
}

