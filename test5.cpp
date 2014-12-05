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
    int  n = 0, j = 0, cmmnd;
  
    ifstream fin("input.txt");  // Поток fin будем использовать для чтения
    ofstream fout("output.txt"); // Поток fout будем использовать для записи

    //fin.open ("input.txt");
    fin >> cmmnd;
    cout << "Колличество ячеек: " << cmmnd;
    cout<<"\n";
 
    if (cmmnd<0) cout<<"Некоректный ввод\n";
    else {
    
    int *dtc;     // указатель который проходит по ячейкам
    dtc = new int[cmmnd];
    n=cmmnd/2;  //стартовая позиция указателя
    
    for (int i = 0; i < cmmnd; i++) dtc[i] = 0;
    cout << "\n";
    
    string str;    //строка с командами которые мы считываем из файла
    
    if (!fin.is_open())
    {
        cout << "Такого файла не существует";
        return -1;
    }
    getline(fin, str); // читает строку из файла до символа перевода строки
                 
        
    for (int i = 0; i < str.length(); i++)  //цикл для прохождения каждого символа строки
    {
        switch ( str[i] )
        {
            case '>':
            //по условию указатель переходит на след ячейке
            if (n<cmmnd)
            n=n+1; else goto stop;
            break;
                               
            case '<':
            //по условию указатель переходит на предыдущ ячейку
            if (n > 0)
            n=n-1;
            else goto stop;
            break;
                                  
            case '-':
            //значение под указателем уменьш на 1
            dtc[n]=dtc[n]-1;
            break;
                                  
            case '+':
            //значение под указателем увелич на 1
            dtc[n]=dtc[n]+1;
            break;
                                  
            case '*':
            //ввывод значение под счетчиком
            fout << dtc[n]<<endl; // запись
            break;
                                  
            case '[':
            //если значение под дачиком равно 0, в качестве следующей команды следует воспринимать идущюю за след командой ]
            if (dtc[n]==0)
               {
                   for ( i=i; str[i]!=']' && i < cmmnd ;i++);
                   i+=1;
               }
            break;
                                  
            case ']':
            //если значение под дачиком не равно 0 в качестве следующей команды следует принимать идущую за предыдущей командой [
            if (dtc[n]!=0)
               {
                    for (i=i; str[i]!='[' && i >= 0 ;i--);
                    i+=1;
               }
            break;
                                  
            case ' ':
            case '\n':
            case '\t':
            break;
                                  
            default:
            cout << "Неверный символ";
            cout<<"\n";
            // ошибка при посторонних значениях
                
        }
    }
    
   for (int i = 0; i < cmmnd; i++)
   cout<<dtc[i]<<" ";
    
   if (false) {
         stop: cout << "Датчик вышел за рамки конвеера";
              }
                 
   delete[]dtc;
   cout<<"\n";
    }
}

