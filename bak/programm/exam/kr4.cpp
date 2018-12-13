#include <iostream>
#include <fstream>
#include <string>
int main(void)
{
	using namespace std;
	setlocale (LC_ALL, "rus");
	ifstream fin("in.txt");
	ofstream fout("out.txt");
	string str;
	int j=0;

	if (!fin.is_open())
    {
        cout<<"такого файла нет";
        return -1;
    }

        while (!fin.eof())
    {
		getline(fin,str);
		j++;
    }

        j*=2;
        string *mass = new string[j];
        fin.seekg(0);
        for(int i=0; i<j; i++)
	fin>>mass[i];

        for(int i=1; i<j; i=i+2)
        for(int q=i+2; q<j; q=q+2)
        if(mass[q]<mass[i])
    {
	swap(mass[i],mass[q]);
        swap(mass[i-1],mass[q-1]);
    } else if(mass[q]==mass[i])
    {  if  (mass[i-1]>mass[q-1])
               {      
               	   swap(mass[i],mass[q]);
		   swap(mass[i-1],mass[q-1]);
		}
    }

        for(int i=0; i<j-1; i=i+2)
        fout<<mass[i]<<" "<<mass[i+1]<<"\n";

        delete[]mass;
	fin.close();
	fout.close();
        return 0;
}
