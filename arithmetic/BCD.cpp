//
//  Двоично-десятичный код
//
//  Created by Мария Волкова on 01.11.15.
//  Copyright (c) 2015 Маша. All rights reserved.
//

#include <iostream>
#include <string>

int main(int argc, const char * argv[]) {
    
    enum
    {
        ZERO = '0',
        ONE,
        TWO,
        THREE,
        FOUR,
        FIVE,
        SIX,
        SEVEN,
        EIGHT,
        NINE,
    };
    
    std :: string num;
    int log = 0;

    std :: cout << "Десятичное число: ";
    std :: cin >> num;
    std :: cout <<"Двоично-десятичный код: ";
    
for (int i = 0; i < num.length(); i++)
    switch(num[i]){
        case ZERO:
            std :: cout << "0000";
            break;
            
        case ONE:
            std :: cout << "0001";
            break;
            
        case TWO:
            std :: cout << "0010";
            break;
            
        case THREE:
            std :: cout << "0011";
            break;
            
        case FOUR:
            std :: cout << "0100";
            break;
            
        case FIVE:
            std :: cout << "0101";
            break;
            
        case SIX:
            std :: cout << "0110";
            break;
            
        case SEVEN:
            std :: cout << "0111";
            break;
            
        case EIGHT:
            std :: cout << "1000";
            break;
            
        case NINE:
            std :: cout << "1001";
            break;
            
        case ',':
            std :: cout << ",";
            break;
            
        case '.':
            std :: cout << ".";
            break;
            
        default:
            log++;
            goto error;
            break;
    }
    
    if (log != 0)
    error :
        std :: cout << "Ошибка ввода!";
        
    return 0;
}
