//
//  Кривые Безье
//
//  Created by Мария Волкова on 23.11.15.
//  Copyright (c) 2015 Маша. All rights reserved.
//

#include <iostream>

int main(void)
{
    using namespace std;
    cout << "Точка А:" << endl;
    float x1, x2, y1, y2;
    cin >> x1 >> y1;
    cout << "Tochka B" <<  endl;
    cin >> x2 >> y2;
    
    cout << "\nДля X" << endl;
    for (float t = 0.1; t < 1.1; t += 0.1)
    {
         cout << (float) (x1 * (1 - t) + t * x2) <<  endl;
    }
    
    cout << "\nДля У" << endl;
    for (float t = 0.1; t < 1.1; t += 0.1)
    {
         cout <<  (float)(y1 * (1 - t) + t * y2) <<  endl;
    }
}
