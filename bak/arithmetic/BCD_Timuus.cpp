#include <iostream>
#include <string>

int main(void) {
    using namespace std;
    
    string bin, str;
    
    cout << "INPUT: ";
    cin >> bin;
    
for (int i = 0; i < bin.length(); i++)
    switch(bin[i]){
        case '0':
            str += "0000";
            break;
            
        case '1':
            str += "0001";
            break;
            
        case '2':
            str += "0010";
            break;
            
        case '3':
            str += "0011";
            break;
            
        case '4':
            str += "0100";
            break;
            
        case '5':
            str += "0101";
            break;
            
        case '6':
            str += "0110";
            break;
            
        case '7':
            str += "0111";
            break;
            
        case '8':
            str += "1000";
            break;
            
        case '9':
            str += "1001";
            break;
            
        case ',':
            str += ",";
            break;
            
        case '.':
            str += ".";
            break;
    }
    
    cout <<"OUTPUT: " << str;
        
    return 0;
}
