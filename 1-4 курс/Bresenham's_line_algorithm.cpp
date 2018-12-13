//
//  main.cpp
//  Bresenham's line algorithm
//
//  Created by Мария Волкова on 06.11.15.
//  Copyright (c) 2015 Маша. All rights reserved.
//

#include <iostream>

#include "math.h"

#define roundf(x) floor(x + 0.5f)

void line( int x1, int y1, int x2, int y2)
{
    int dx = (x2 - x1 >= 0 ? 1 : -1);
    int dy = (y2 - y1 >= 0 ? 1 : -1);
    
    int lengthX = abs(x2 - x1);
    int lengthY = abs(y2 - y1);
    
    int length;
    
    if (lengthX > lengthY)
        length = lengthX;
        else length = lengthY;
   
    
    if (length == 0)
    {
          std :: cout << x1<<" "<< y1 << " ";
    }
    
    if (lengthY <= lengthX)
    {
        // Начальные значения
        int x = x1;
        int y = y1;
        int d = -lengthX;
        
        // Основной цикл
        length++;
        while(length--)
        {
                     std :: cout << x<<" "<< y <<" ";
            x += dx;
            d += 2 * lengthY;
            if (d > 0) {
                d -= 2 * lengthX;
                y += dy;
            }
        }
    }
    else
    {
        // Начальные значения
        int x = x1;
        int y = y1;
        int d = - lengthY;
        
        // Основной цикл
        length++;
        while(length--)
        {
            std :: cout << x<<" "<< y <<" ";
            y += dy;
            d += 2 * lengthX;
            if (d > 0) {
                d -= 2 * lengthY;
                x += dx;
            }
        }
    }
}


int main (void)
{ line( 2, 4, 5, 7);
    
}
