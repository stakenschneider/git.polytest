//во входном файле приведено имя в каждой строчке с номером телефона. вывести на экран все телефоны человека. если нет выдать ошибку
#include <iostream>
#include <fstream>
#include <string>
int main(void) {
    using namespace std;
    setlocale(LC_ALL, "rus");
    ifstream fin("in.txt");
    ofstream fout("out.txt");
    int i=0;
    string line,str,name;
    cout<<"введите имя человека";
    cin>>name;
    if (!fin.is_open())
    {
        cout<<"такого файла нет";
        return -1;
    }
    while (!fin.eof()) {
        fin>>str;
        if (str==name)
        {
            fin>>str;
            cout<<str<<endl;
            i++;
        }
    }
   if (i==0) cout<<"номера нет";
    
    
    return 0;
}
