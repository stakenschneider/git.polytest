#include <iostream>
using namespace std;

const int N = 100;


class Set
{
    int size;
    int array[N];
    
    int Find_num(int num);
    
private:
    
    friend bool operatorCompare(Set set1, Set set2)
    {
        if (set1.size != set2.size)
        {
            cout << "¬ Ï‡ÒÒË‚‡ı ‡ÁÌÓÂ ÍÓÎË˜ÂÒÚ‚Ó ˝ÎÂÏÂÌÚÓ‚\n";
            return 0;
        }
        else
        {
            for (int i = 0; i < set1.size; i++)
            {
                if (set1.array[i] != set2.array[i])
                {
                    cout << "«Ì‡˜ÂÌËˇ Ï‡ÒÒË‚Ó‚ ÌÂ ‡‚Ì˚\n";
                    return 0;
                }
            }
        }
        
        return 1;
    }
    
    
public:
    
    Set() { size = 0; }
    
   
    
    
    int Length_of() { return size; }
    
    bool In(int num);
    
    void Print_set(); //‚˚‚Ó‰
    
    Set operator+(int); //‚‚Ó‰
    
    Set operator+(Set Uni);					//ÒÎÓÊÂÌËÂ ÏÌÓÊÂÒÚ‚
    Set operator-(int num);					//Û‰‡ÎÂÌËÂ ˝ÎÂÏÂÌÚ‡
    Set operatorIntersaction(Set Inter);	//ÔÂÂÒÂ˜ÂÌËÂ
    Set operatorCopy(Set Cpy);				//ÍÓÔËÓ‚‡ÌËÂ
    
};

int Set::Find_num(int num)
{
    int i;
    
    for (i = 0; i<size; i++)
        if (array[i] == num)
            return i;
    
    return -1;
}

bool Set::In(int num)
{
    if (Find_num(num) != -1)
        return true;
    return false;
}



void Set::Print_set()
{
    cout << "{ ";
    for (int i = 0; i<size; i++)
        cout << array[i] << " ";
    cout << "}\n";
}

Set Set::operator+(int num)
{
    Set curr_set;
    
    curr_set = *this;
    
    curr_set.array[curr_set.size] = num;
    curr_set.size++;
    
    return curr_set;
}

Set Set::operator+(Set Uni)
{
    Set curr_set = *this;
    
    for (int i = 0; i<Uni.size; i++)
        curr_set = curr_set + Uni.array[i];
    
    return curr_set;
    
}

Set Set::operator-(int num)
{
    Set newset;
    int i = Find_num(num);
    
    for (int j = 0; j<size; j++)
        if (j != i)
            newset = newset + array[j];
    
    return newset;
}


Set Set::operatorIntersaction(Set Inter)
{
    Set curr_set = *this;                
    
    for (int i = 0; i<Inter.size; i++)
        curr_set = curr_set - Inter.array[i];
    
    return curr_set;
}

Set Set::operatorCopy(Set Cpy)
{
    Set curr_set = *this;
    
    for (int i = 0; i < size; i++)
        curr_set.array[i] = Cpy.array[i];
    
    return curr_set;
    
}


int main()
{
    setlocale (LC_ALL , "rus");
    return 1;
}
