//    #define BLACK 0	#define RED        4	#define DARK_GRAY    8		#define LIGHT_RED     12
//    #define BLUE  1	#define MAGENTA    5	#define LIHGT_BLUE   9		#define LIGHT_MAGENTA 13
//    #define GREEN 2	#define OlIVEN     6	#define LIGHT_GREEN  10		#define YELLOW        14
//    #define CYAN  3	#define LIGHT_GRAY 7	#define LIGHT_CYAN   11		#define WHITE         15

#include "StdAfx.h"
#include <windows.h>
#include "ConsolenGrafik.h"

#define PIX printf(" ");
#define ZEICHEN(x) printf("%c", (x));

	HANDLE hConsole;
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

CConsolenGrafik::CConsolenGrafik(void)
{
}

CConsolenGrafik::~CConsolenGrafik(void)
{
}

void CConsolenGrafik::Position(int x, int y)
{
	COORD coord = { x, y };
	SetConsoleCursorPosition(hStdOut, coord);
} 

void CConsolenGrafik::FarbeSetzen(int i_textFarbe, int i_hintergrundFarbe)        
{
   HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
   SetConsoleTextAttribute(hStdOut, (WORD)(( i_hintergrundFarbe << 4) | i_textFarbe ) );
}

void CConsolenGrafik::ZeichenKette(int von_x, int bis_x, int i_zeile, int i_hintergrundFarbe)        
{
   for(int i=von_x; i <= bis_x; i++ ) 
   {
	   Position(i,i_zeile);
	   FarbeSetzen(i_hintergrundFarbe, i_hintergrundFarbe);
	   ZEICHEN(32)
   }
}

void CConsolenGrafik::ZeichenKette(int von_x, int bis_x, int i_zeile, int i_textFarbe, int i_hintergrundFarbe)        
{
   for(int i=von_x; i <= bis_x; i++ ) 
   {
	   Position(i,i_zeile);
	   FarbeSetzen(i_textFarbe, i_hintergrundFarbe);
	   ZEICHEN(32)
   }
}

void CConsolenGrafik::ZeichenKette(int von_x, int bis_x, int i_zeile, int i_zeichen, int i_textFarbe, int i_hintergrundFarbe)        
{
   for(int i=von_x; i <= bis_x; i++ ) 
   {
	   Position(i,i_zeile);
	   FarbeSetzen(i_textFarbe, i_hintergrundFarbe);
	   ZEICHEN(i_zeichen)
   }
}

void CConsolenGrafik::ZeichenSpalte(int i_spalte, int von_y, int bis_y, int i_hintergrundFarbe)        
{
   for(int i=von_y; i <= bis_y; i++ ) 
   {
	   Position(i_spalte,i);
	   FarbeSetzen(i_hintergrundFarbe, i_hintergrundFarbe);
	   ZEICHEN(32)
   }
}

void CConsolenGrafik::ZeichenSpalte(int i_spalte, int von_y, int bis_y, int i_textFarbe, int i_hintergrundFarbe)        
{
   for(int i=von_y; i <= bis_y; i++ ) 
   {
	   Position(i_spalte,i);
	   FarbeSetzen(i_textFarbe, i_hintergrundFarbe);
	   ZEICHEN(32)
   }
}

void CConsolenGrafik::ZeichenSpalte(int i_spalte, int von_y, int bis_y, int i_zeichen, int i_textFarbe, int i_hintergrundFarbe)        
{
   for(int i=von_y; i <= bis_y; i++ ) 
   {
	   Position(i_spalte,i);
	   FarbeSetzen(i_textFarbe, i_hintergrundFarbe);
	   ZEICHEN(i_zeichen)
   }
}

void CConsolenGrafik::Farben(int von_x, int bis_x, int von_y, int bis_y, int i_Farbe)        
{
	for (int x = von_x; x <= bis_x; x++ )
		for (int y = von_y; y <= bis_y ; y++ )
		{
			Position(x,y);
			FarbeSetzen(i_Farbe, i_Farbe);
			PIX
		}
}

void CConsolenGrafik::Farben(int von_x, int bis_x, int von_y, int bis_y, int i_textFarbe, int i_hintergrundFarbe)        
{
	for (int x = von_x; x <= bis_x; x++ )
		for (int y = von_y; y <= bis_y ; y++ )
		{
			Position(x,y);
			FarbeSetzen(i_textFarbe, i_hintergrundFarbe);
			PIX
		}
}

// case 'A' 185-188 ╣ ║ ╗ ╝ 200-206 ╚ ╔ ╩ ╠ ═ ╬
// case 'B' 179-180 │ ┤     191-197 ┐└ ┴ ┬ ├ ─ ┼  217-218 ┘┌
// case 'C' 181,184 ╡ ╕     179 │ 190 ╛ 198 ╞ 205 ═ 207 ╧ 209 ╤ 212 213 216 ╘ ╒ ╪
// case 'D' 182,183 ╢ ╖     196 ─ 189 ╜ 186 ║ 199 ╟ 208 ╨ 210,211, 214 ╥ ╙ ╓ 215 ╫

void CConsolenGrafik::Umramung(int von_x, int von_y, int bis_x, int bis_y, char type)
{
	int i_zch_hor, i_zch_vert, i_zch_eck1, i_zch_eck2, i_zch_eck3, i_zch_eck4, i_zch_zweck1, i_zch_zweck2, i_zch_mitte;

	switch (type)
	{
	case 'A': 
		i_zch_hor = 205;  
		i_zch_vert = 186;
		i_zch_eck1 = 201;
		i_zch_eck2 = 200;
		i_zch_eck3 = 187;
		i_zch_eck4 = 188;
		i_zch_zweck1 = 204;
		i_zch_zweck2 = 185;
		i_zch_mitte = 206;
		break;
	case 'B': 
		i_zch_hor = 196;  
		i_zch_vert = 179;
		i_zch_eck1 = 218;
		i_zch_eck2 = 192;
		i_zch_eck3 = 191;
		i_zch_eck4 = 217;
		i_zch_zweck1 = 195;
		i_zch_zweck2 = 180;
		i_zch_mitte = 197;
		break;
	case 'C': 
		i_zch_hor = 205;  
		i_zch_vert = 179;
		i_zch_eck1 = 213;
		i_zch_eck2 = 212;
		i_zch_eck3 = 184;
		i_zch_eck4 = 190;
		i_zch_zweck1 = 198;
		i_zch_zweck2 = 181;
		i_zch_mitte = 216;
		break;
	case 'D': 
		i_zch_hor = 196;  
		i_zch_vert = 186;
		i_zch_eck1 = 214;
		i_zch_eck2 = 211;
		i_zch_eck3 = 183;
		i_zch_eck4 = 189;
		i_zch_zweck1 = 199;
		i_zch_zweck2 = 182;
		i_zch_mitte = 215;
		break;
	}

	for (int y = von_y; y <= bis_y; y += bis_y - von_y )
		for (int x = von_x + 1; x <= bis_x - 1; x++ )
		{
			Position(x,y);
			ZEICHEN(i_zch_hor)
		}
	for (int x = von_x; x <= bis_x; x += bis_x - von_x )
		for (int y = von_y + 1; y <= bis_y - 1; y++ )
		{
			Position(x,y);
			ZEICHEN(i_zch_vert)
		}
	Position(von_x,von_y);
	ZEICHEN(i_zch_eck1)
	Position(von_x,bis_y);
	ZEICHEN(i_zch_eck2)
	Position(bis_x,von_y);
	ZEICHEN(i_zch_eck3)
	Position(bis_x,bis_y);
	ZEICHEN(i_zch_eck4)
}

void CConsolenGrafik::BgrFarbe(int i_farbe)
{
	for (int y = 0; y <= 24; y++ )
		for (int x = 0; x <= 79 ; x++ )
		{
			Position(x,y);
			FarbeSetzen(i_farbe, i_farbe);
			PIX
		}
}
