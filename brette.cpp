#include "StdAfx.h"
#include "Brette.h"
#include "ConsolenGrafik.h"

CConsolenGrafik BrettScreen;

CBrett::CBrett(void)
{
}

CBrett::~CBrett(void)
{
}

void CBrett::Zeichne(char x_location, char y_location, char count_hor, char count_vetr, char bgr_color_1, char bgr_color_2)
{
	for (char i=0, x = x_location; i < count_hor , x < x_location+count_hor*3; i++, x = x+3 )
		for (char j=0, y = y_location; j < count_vetr , y < y_location+count_vetr*2 ; j++, y=y+2 )
		{
			if((j+i)%2!=0) 
			{
				BrettScreen.Farben(x,x+2,y,y+1,0, bgr_color_1);
			} 
			else
			{
				BrettScreen.Farben(x,x+2,y,y+1,0, bgr_color_2);
			}
		}
}

void CBrett::Trennlinie(char x_location, char y_location)
{
		for (char j=0, y = 1+y_location; j<10 , y < y_location+21 ; j++, y=y+2 )
		{
			BrettScreen.Farben(x_location,x_location,y,y+1,0,4);
		}
}

void CBrett::RedrawFeld(char x_location, char y_location, char color)
{
	for (char x = x_location; x < x_location+3; x = x+3 )
		for (char y = y_location; y < y_location+2 ; y=y+2 )
		{
			BrettScreen.Farben(x,x+2,y,y+1,0, color);
		}
}
