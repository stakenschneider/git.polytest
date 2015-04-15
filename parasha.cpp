//
//  Created by Мария Волкова on 29.10.14.
//

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <unordered_map>
#include "func.cpp"


using namespace std;

ifstream fin("levels.txt");

int j=0, lose=0 ,  g=0, b=0 ;

long long length;

struct struct_way
{
    string road;
    string cmnd[40];
    int i,n;
};

int num=0 , n = 0;//номер задания

int convert (const string& s_num)
{
    int repeat_num = 0;
    istringstream ist(s_num);
    ist >> repeat_num;
    return repeat_num;
}

void null (int num , int n)
 {
    n=0;
    num--;
    fin.seekg( length );
    cout << "Попробуйте снова\n";
 }

struct_way walk_func (struct_way way)
{
    n++;
    if (way.road[n] == '|' )
    {
        cout << "Не перепрыгнул стенку\n";
        lose++;
        null(num,n);
    }
    return way;
}


int jump_func (int n)
{
    n+=2;
    
    return n;
}


struct_way if_func (struct_way way) {
    way.i++;
    if (way.cmnd[way.i]=="|")
    { way.i+=2;
        if(way.cmnd[way.i]=="walk")
        {
            if (way.cmnd[way.i+1]=="else")
            {
                way.i+=2;
                if (way.cmnd[way.i]=="walk") way=walk_func(way);
                    if (way.cmnd[way.i]=="jump") {if (way.road[way.n+1]=='|') way=walk_func(way); else n=jump_func(n);}
            } else {if (way.road[n+1]=='|') way=walk_func(way);}
        } else if(way.cmnd[way.i]=="jump")
        {
            if (way.cmnd[way.i+1]=="else")
            {
                way.i+=2;
                if (way.cmnd[way.i]=="walk"){if (way.road[n+1]=='|') n=jump_func(n); else way=walk_func(way);}
                if (way.cmnd[way.i]=="jump") n=jump_func(n);
                    } else
                    {
                        if (way.road[n+1]=='|')
                            n=jump_func(n);}
        }
    }
    if (way.cmnd[way.i]=="-")
    {
        way.i+=3;
        if(way.cmnd[way.i]=="walk")
        {
            if (way.cmnd[way.i+1]=="else")
            {
                way.i+=2;
                if (way.cmnd[way.i]=="walk") way=walk_func(way);
                    if (way.cmnd[way.i]=="jump") {if (way.road[n+1]=='-') way=walk_func(way); else n=jump_func(n);}
            } else {if (way.road[n+1]=='-') way=walk_func(way);}
        } else if(way.cmnd[way.i]=="jump")
        {
            if (way.cmnd[way.i+1]=="else")
            {
                way.i+=2;
                if (way.cmnd[way.i]=="walk"){if (way.road[n+1]=='-') n=jump_func(n); else way=walk_func(way);}
                if (way.cmnd[way.i]=="jump") n=jump_func(n);
                    } else {if (way.road[n+1]=='-') n=jump_func(n);}
        }
    }
    return way;
}







int main(void)
{
    setlocale(LC_ALL , "rus");
    enum
    {
        Walk = 10 , Jump = 11 , Repeat = 12 , If = 13
    };
    
    if (!fin.is_open())
    {
        cout << "Такого файла не существует";
        return -1;
    }
    
    length=fin.tellg();
    
    struct_way way;
    
    std::unordered_map<string, int> hashmap;
    hashmap["walk"] = Walk;
    hashmap["jump"] = Jump;
    hashmap["repeat"] = Repeat;
    hashmap["if"] = If;
    hashmap["end"] = 0;
    
    
    while (!fin.eof()) {
    
        length = fin.tellg();
        getline (fin , way.road);
        unsigned long q=way.road.length();
        
        
        num++;
        cout<<"Задание "<<num<<"."<<"Количество клеточек: "<<q <<"\n"<<way.road<<"\n"<<"Введите команды:\n";
        
        do
        {
            cin>>way.cmnd[j];
            j++;
        } while (way.cmnd[j-1] != "end");
        
        for (int i=0; i<j; i++){
            if(hashmap.count(way.cmnd[i])) {
                switch(hashmap[way.cmnd[i]]) {
                    case Walk:
                        way=walk_func(way);
                        if (way.road[n] == '|' )
                        {
        cout << "Не перепрыгнул стенку\n";
        lose++;
                            null(num,n);
                        }
                        break;
                        
                    case Jump:
                        n=jump_func(n);
                        if (way.road[n] == '|')
                        {
                        cout << "Напрыгнул на стенку\n";
                            lose++; null(num,n);
                        }
                        break;
                        
                    case Repeat:
                        i++;
                        if (way.cmnd[i]=="until")
                        {
                            i++;
                            if(way.cmnd[i]=="+")
                            {
                                i++;
                                if (way.cmnd[i] =="walk")
                                {
                                    while (way.road[n]!='+')
                                        way=walk_func(way);
                                    
                                }
                                
                                if (way.cmnd[i] =="jump")
                                {
                                    while (way.road[n]!='+')
                                        n=jump_func(n);
                                }
                                
                                b=i;
                            }
                        }
                        else
                        {
                            int repeat_num = convert(way.cmnd[i]);
                            i++;
                            if(way.cmnd[i]=="walk") n=n+repeat_num;
                            if(way.cmnd[i]=="jump") n=n+repeat_num*2;
                            g=i;
                            }
                        break;
                        
                        
                        
                    case If:
                        way=if_func(way);
                        break;
                        
                }//конец switch
            } //конец if
            else {
                cout << "Hеправильная команда\n"; null(num,n);
            }
        } //конец for
        
        if (way.road[n] == '+' && lose==0 && n+1==q) cout << "ВЫ КАРАЛЕВА ПАБЕД!\n\n";
        
        if (way.road[n]!='+' && n+1>q) {
            cout << "Вышли за игровое поле\n";
            lose++; null(num,n);
        }
        
        if (way.road[n]!='+' && n+1<q )
        {
            cout << "Не дошли до конца\n";
            lose++; null(num,n);
            
        }
        
        cout<<"\n";
    
    }// конец while eof
    cout<<"Вы прошли все уровни\n";
}
