//    #define BLACK 0	#define RED        4	#define DARK_GRAY    8		#define LIGHT_RED     12
//    #define BLUE  1	#define MAGENTA    5	#define LIHGT_BLUE   9		#define LIGHT_MAGENTA 13
//    #define GREEN 2	#define OLIVEN     6	#define LIGHT_GREEN  10		#define YELLOW        14
//    #define CYAN  3	#define LIGHT_GRAY 7	#define LIGHT_CYAN   11		#define WHITE         15

#include "stdafx.h"
#include "Brette.h"
#include "ConsolenGrafik.h"
#include "TetrisFiguren.h"
#include <conio.h>

void main()
{
	char c_input;
	bool b_weiter = true;

	CConsolenGrafik Screen;
	CTetrisFiguren Figur;
	CBrett Brett;

	Screen.BgrFarbe(1);

	do
	{
		Screen.Farben(28,51,0,23,6);
		Figur.InitArrays();
		while(Figur.m_voll)
		{
			Figur.Erstellen();
			Figur.Vorschau();
			Figur.MoveDown();
		}
		c_input = getch();
		if(c_input == 27) 
			b_weiter = false;
		else 
			Figur.m_voll = true;
	} while(b_weiter);

	getchar();
}
