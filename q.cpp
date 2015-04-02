//
//  Created by Мария Волкова on 29.10.14.
//
// walk
// jump
// repeat 2
// repeat until +
// if | then jump
// if | then jump else walk
// if - ahead then walk else jump

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <unordered_map>

using namespace std;

ifstream fin("levels.txt");

struct rod
{

};

int convert (const string& s_num)
{
    int repeat_num = 0;
    istringstream ist(s_num);
    ist >> repeat_num;
    return repeat_num;
}
int num=0; //номер задания
void null (int num, int n)
 {
n=0;
 num--;
 fin.seekg(num);
 cout << "Попробуйте снова\n";
     
 }

int walk_func (int n)
{
    n++;
//   if (road[n] == '|' )
// {
//   cout << "Не перепрыгнул стенку\n";
// lose++;
//       null(num,n);
// }
    return n;
}
int jump_func ( int n)
{
    n+=2;
    
    return n;
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
    
    std::unordered_map<string, int> hashmap;
    hashmap["walk"] = Walk;
    hashmap["jump"] = Jump;
    hashmap["repeat"] = Repeat;
    hashmap["if"] = If;
    hashmap["end"] = 0;
    
    
    while (!fin.eof()) {
        string road;
        getline (fin ,road);
        unsigned long q=road.length();
        string cmnd[40];
        int j=0, lose=0 , n = 0, g=0, b=0;
        num++;
        cout<<"Задание "<<num<<"."<<"Количество клеточек: "<<q <<"\n"<<road<<"\n"<<"Введите команды:\n";
        
        do
        {
            cin>>cmnd[j];
            j++;
        } while (cmnd[j-1] != "end");
        
        for (int i=0; i<j; i++){
            if(hashmap.count(cmnd[i])) {
                switch(hashmap[cmnd[i]]) {
                    case Walk:
                        n=walk_func(n);
                        if (road[n] == '|' )
                        {
        cout << "Не перепрыгнул стенку\n";
        lose++;
                            null(num,n);
                        }
                        break;
                        
                    case Jump:
                        n=jump_func(n);
                        if (road[n] == '|')
                        {
                        cout << "Напрыгнул на стенку\n";
                            lose++; null(num,n);                            //goto null;
                        }
                        break;
                        
                    case Repeat:
                        i++;
                        if (cmnd[i]=="until")
                        {
                            i++;
                            if(cmnd[i]=="+")
                            {
                                i++;
                                if (cmnd[i] =="walk")
                                {
                                    while (road[n]!='+')
                                    {
                                        n=walk_func(n);
                                    }
                                }
                                
                                if (cmnd[i] =="jump")
                                {
                                    while (road[n]!='+')
                                    {
                                        n=jump_func(n);
                                    }
                                }
                                
                                b=i;
                                if (cmnd[i]=="if") while (road[n]!='+' && n+1<q){  i=b; i++;
                                    
                                    if (cmnd[i]=="|")
                                    {
                                        i++;
                                        if (cmnd[i]=="then")
                                        {
                                            i++;
                                            
                                            if(cmnd[i]=="walk")
                                            {
                                                if (cmnd[i+1]=="else")
                                                {
                                                    i+=2;
                                                    if (cmnd[i]=="walk") n=walk_func(n);
                                                    if (cmnd[i]=="jump") {if (road[n+1]=='|') n=walk_func(n); else n=jump_func(n);}
                                                } else {if (road[n+1]=='|') n=walk_func(n);}
                                            } else if(cmnd[i]=="jump")
                                            {
                                                if (cmnd[i+1]=="else")
                                                {
                                                    i+=2;
                                                    if (cmnd[i]=="walk"){if (road[n+1]=='|') n=jump_func(n); else n=walk_func(n);}
                                                    if (cmnd[i+1]=="jump") n=jump_func(n);
                                                } else {if (road[n+1]=='|') n=jump_func(n);}
                                            }
                                            
                                            
                                        } else
                                        {
                                            cout<<"Пропустили then\n";
                                            lose++; null(num,n);                                            //goto null;
                                        }
                                    }
                                    
                                    if (cmnd[i]=="-")
                                    {
                                        i++;
                                        if (cmnd[i]=="ahead")
                                        {
                                            i++;
                                            if (cmnd[i]=="then")
                                            {
                                                i++;
                                                if(cmnd[i]=="walk")
                                                {
                                                    if (cmnd[i+1]=="else")
                                                    {
                                                        i+=2;
                                                        if (cmnd[i]=="walk") n=walk_func(n);
                                                        if (cmnd[i]=="jump") {if (road[n+1]=='-') n=walk_func(n); else n=jump_func(n);}
                                                    } else {if (road[n+1]=='-') n=walk_func(n);}
                                                } else if(cmnd[i]=="jump")
                                                {
                                                    if (cmnd[i+1]=="else")
                                                    {
                                                        i+=2;
                                                        if (cmnd[i]=="walk"){if (road[n+1]=='-') n=jump_func(n); else n=walk_func(n);}
                                                        if (cmnd[i]=="jump") n=jump_func(n);
                                                    } else {if (road[n+1]=='-') n=jump_func(n);}
                                                }
                                                
                                                
                                            }else
                                            {
                                                cout<<"Пропустили then\n";
                                                lose++; null(num,n);                                                //goto null;
                                            }
                                        } else {
                                            cout<<"Пропустили ahead\n";
                                            lose++; null(num,n);                                            //goto null;
                                        }
                                    }
                                }
                            }
                        }
                        else
                        {
                            int repeat_num = convert(cmnd[i]);
                            i++;
                            if(cmnd[i]=="walk") n=n+repeat_num;
                            if(cmnd[i]=="jump") n=n+repeat_num*2;
                            g=i;
                            if (cmnd[i]=="if") for(int t=0; t<repeat_num;t++){
                                i=g;
                                i++;
                                
                                if (cmnd[i]=="|")
                                {
                                    i++;
                                    if (cmnd[i]=="then")
                                    {
                                        i++;
                                        
                                        if(cmnd[i]=="walk")
                                        {
                                            if (cmnd[i+1]=="else")
                                            {
                                                i+=2;
                                                if (cmnd[i]=="walk") n=walk_func(n);
                                                if (cmnd[i]=="jump") {if (road[n+1]=='|') n=walk_func(n); else n=jump_func(n);}
                                            } else {if (road[n+1]=='|') n=walk_func(n);}
                                        } else if(cmnd[i]=="jump")
                                        {
                                            if (cmnd[i+1]=="else")
                                            {
                                                i+=2;
                                                if (cmnd[i]=="walk"){if (road[n+1]=='|') n=jump_func(n); else n=walk_func(n);}
                                                if (cmnd[i]=="jump") n=jump_func(n);
                                            } else
                                            {
                                                if (road[n+1]=='|')
                                                    n=jump_func(n);}
                                        }
                                        
                                        
                                    } else
                                    {
                                        cout<<"Пропустили then\n";
                                        lose++; null(num,n);                                       // goto null;
                                    }
                                }
                                
                                if (cmnd[i]=="-")
                                {
                                    i++;
                                    if (cmnd[i]=="ahead")
                                    {
                                        i++;
                                        if (cmnd[i]=="then")
                                        {
                                            i++;
                                            if(cmnd[i]=="walk")
                                            {
                                                if (cmnd[i+1]=="else")
                                                {
                                                    i+=2;
                                                    if (cmnd[i]=="walk") n=walk_func(n);
                                                    if (cmnd[i]=="jump") {if (road[n+1]=='-') n=walk_func(n); else n=jump_func(n);}
                                                } else {if (road[n+1]=='-') n=walk_func(n);}
                                            } else if(cmnd[i]=="jump")
                                            {
                                                if (cmnd[i+1]=="else")
                                                {
                                                    i+=2;
                                                    if (cmnd[i]=="walk"){if (road[n+1]=='-') n=jump_func(n); else n=walk_func(n);}
                                                    if (cmnd[i]=="jump") n=jump_func(n);
                                                } else {if (road[n+1]=='-') n=jump_func(n);}
                                            }
                                            
                                            
                                        }else
                                        {
                                            cout<<"Пропустили then\n";
                                            lose++; null(num,n);                                            //goto null;
                                        }
                                    } else {
                                        cout<<"Пропустили ahead\n";
                                        lose++; null(num,n);                                        //goto null;
                                    }
                                }
                                
                            }
                        }
                        break;
                        
                    case If:
                        i++;
                        if (cmnd[i]=="|")
                        {
                            i++;
                            if (cmnd[i]=="then")
                            {
                                i++;
                                
                                if(cmnd[i]=="walk")
                                {
                                    if (cmnd[i+1]=="else")
                                    {
                                        i+=2;
                                        if (cmnd[i]=="walk") n=walk_func(n);
                                        if (cmnd[i]=="jump") {if (road[n+1]=='|') n=walk_func(n); else n=jump_func(n);}
                                    } else {if (road[n+1]=='|') n=walk_func(n);}
                                } else if(cmnd[i]=="jump")
                                {
                                    if (cmnd[i+1]=="else")
                                    {
                                        i+=2;
                                        if (cmnd[i]=="walk"){if (road[n+1]=='|') n=jump_func(n); else n=walk_func(n);}
                                        if (cmnd[i]=="jump") n=jump_func(n);
                                    } else
                                    {
                                        if (road[n+1]=='|')
                                            n=jump_func(n);}
                                    }
                                
                                
                            } else
                            {
                                cout<<"Пропустили then\n";
                                lose++; null(num,n);                               // goto null;
                            }
                        }
                        if (cmnd[i]=="-")
                        {
                            i++;
                            if (cmnd[i]=="ahead")
                            {
                                i++;
                                if (cmnd[i]=="then")
                                {
                                    i++;
                                    if(cmnd[i]=="walk")
                                    {
                                        if (cmnd[i+1]=="else")
                                        {
                                            i+=2;
                                            if (cmnd[i]=="walk") n=walk_func(n);
                                            if (cmnd[i]=="jump") {if (road[n+1]=='-') n=walk_func(n); else n=jump_func(n);}
                                        } else {if (road[n+1]=='-') n=walk_func(n);}
                                    } else if(cmnd[i]=="jump")
                                    {
                                        if (cmnd[i+1]=="else")
                                        {
                                            i+=2;
                                            if (cmnd[i]=="walk"){if (road[n+1]=='-') n=jump_func(n); else n=walk_func(n);}
                                            if (cmnd[i]=="jump") n=jump_func(n);
                                        } else {if (road[n+1]=='-') n=jump_func(n);}
                                    }
                                    
                                    
                                }else
                                {
                                    cout<<"Пропустили then\n";
                                    lose++; null(num,n);                                  //  goto null;
                                }
                            } else {
                                cout<<"Пропустили ahead\n";
                                lose++; null(num,n);                               // goto null;
                            }
                        }
                        break;
                        
                }//конец switch
            } //конец if
            else {
                cout << "Hеправильная команда\n"; null(num,n);               // goto null;
            }
        } //конец for
        
        if (road[n] == '+' && lose==0 && n+1==q) cout << "ВЫ КАРАЛЕВА ПАБЕД!\n\n";
        
        if (road[n]!='+' && n+1>q) {
            cout << "Вышли за игровое поле\n";
            lose++; null(num,n);            //goto null;
        }
        
        if (road[n]!='+' && n+1<q )
        {
            cout << "Не дошли до конца\n";
            lose++; null(num,n);           // goto null;
            
        }
        
        cout<<"\n";
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
     //   if (lose!=0) {
     //   null: num=num-1;
       //     fin.seekg(num);
         //   n=0;
           // cout << "Побробуйте снова\n\n";
       // }
    }// конец while eof
    cout<<"Вы прошли все уровни\n";
    
}
