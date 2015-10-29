//
//
//  Created by Екатерина Волкова on 29.10.15.
//  Copyright (c) 2015 Маша. All rights reserved.
//

#include <iostream>
#include <iomanip>
#include <bitset>


int main(void)
{
    double d ;
    std::cout << "Input: ";
    std::cin >> d ;
    unsigned long long *i = reinterpret_cast<unsigned long long *>(&d);
    std::cout << "OutputOutput: " << std::bitset<64>(*i) << std::endl;
    std::cout << "первая цифра знак числа (о если +, 1 если -)\nследущие 8 бит - смещенная значение\n23 бита - дробная часть мантисы";
    return 0;
}
