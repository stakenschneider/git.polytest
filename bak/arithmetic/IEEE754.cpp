//
//
//  Created by Екатерина Волкова on 29.10.15.
//  Copyright (c) 2015 Маша. All rights reserved.
//

#include <iostream>
#include <iomanip>
#include <bitset>


// IEEE754 Double precision 64-bit

int main(void)
{
    double d ;
    std :: cout << "Input: ";
    std :: cin >> d ;
    unsigned long long *i = reinterpret_cast < unsigned long long *> (&d);
    std :: cout << "Output: " << std :: bitset <64> (*i) << std :: endl;
    std :: cout << "первая цифра знак числа (о если +, 1 если -)\nследущие 11 цифр - экспонента\nвсе остальные цифры - мантиса";
    return 0;
}
