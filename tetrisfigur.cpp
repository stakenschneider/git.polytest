#include "StdAfx.h"
#include "TetrisFiguren.h"
#include "ConsolenGrafik.h"
#include "SpielFeld.h"
#include <time.h>
#include <conio.h>
#include <stdlib.h>
#include <windows.h>

CConsolenGrafik Screen;
CSpielFeld Felder;

#define BLACK			0
#define BLUE			1
#define GREEN			2
#define CYAN			3
#define RED				4
#define MAGENTA			5
#define OLIVEN			6
#define LIGHT_GRAY		7
#define DARK_GRAY		8
#define LIHGT_BLUE		9
#define LIGHT_GREEN		10
#define LIGHT_CYAN		11	
#define LIGHT_RED		12
#define LIGHT_MAGENTA	13
#define YELLOW			14
#define WHITE			15

#define BGRND			6

#define UP				72
#define LEFT			75
#define RIGHT			77
#define DOWN			80

CTetrisFiguren::CTetrisFiguren(void):
										m_Stellung(1), 
										m_actuelleFigur(1), 
										m_nextFigur(1), 
										m_actuelleFarbe(14),
										m_nextFarbe(14), 
										m_xMax(45),
										m_setzeInZeile(12),
										m_sonderfall(false),
										m_voll(true)
{
}

CTetrisFiguren::~CTetrisFiguren(void)
{
}

void CTetrisFiguren::DrawFeld(char x_location, char y_location, char color)
{
	for (char x = x_location; x < x_location+3; x = x+3 )
		for (char y = y_location; y < y_location+2 ; y=y+2 )
		{
			Screen.Farben(x,x+2,y,y+1,0, color);
		}
}

void CTetrisFiguren::T_Figur(char m_Stellung, char x, char y, char color)
{
	switch(m_Stellung)
	{
	case 1:
		DrawFeld(x,y,color);
		DrawFeld(x+3,y,color);
		DrawFeld(x+6,y,color);
		DrawFeld(x+3,y+2,color);
		m_aktueleBreite = 3;
		m_profil[0] = 0;
		m_profil[1] = 1;
		m_profil[2] = 0;
		m_profil[3] = 0;
		//if(m_actuelleSpalte >= 7) 
		m_xMax = 43;
		m_yMax = 20;
		m_xMin = 28;
		break;
	case 2:
		DrawFeld(x,y+2,color);
		DrawFeld(x+3,y,color);
		DrawFeld(x+3,y+2,color);
		DrawFeld(x+3,y+4,color);
		m_aktueleBreite = 2;
		m_profil[0] = 1;
		m_profil[1] = 2;
		m_profil[2] = 0;
		m_profil[3] = 0;
		m_xMax = 45;
		m_yMax = 18;
		m_xMin = 28;
		break;
	case 3:
		DrawFeld(x+3,y,color);
		DrawFeld(x,y+2,color);
		DrawFeld(x+3,y+2,color);
		DrawFeld(x+6,y+2,color);
		m_aktueleBreite = 3;
		m_profil[0] = 1;
		m_profil[1] = 1;
		m_profil[2] = 1;
		m_profil[3] = 0;
		//if(m_actuelleSpalte >= 7) 
		m_xMax = 43;
		m_yMax = 20;
		m_xMin = 28;
		break;
	case 4:
		DrawFeld(x,y,color);
		DrawFeld(x,y+2,color);
		DrawFeld(x+3,y+2,color);
		DrawFeld(x,y+4,color);
		m_aktueleBreite = 2;
		m_profil[0] = 2;
		m_profil[1] = 1;
		m_profil[2] = 0;
		m_profil[3] = 0;
		m_xMax = 45;
		m_yMax = 18;
		m_xMin = 28;
		break;
	}
}

void CTetrisFiguren::L_Figur(char m_Stellung, char x, char y, char color)
{
	switch(m_Stellung)
	{
	case 1:
		DrawFeld(x,y,color);
		DrawFeld(x+3,y,color);
		DrawFeld(x+6,y,color);
		DrawFeld(x,y+2,color);
		m_aktueleBreite = 3;
		m_profil[0] = 1;
		m_profil[1] = 0;
		m_profil[2] = 0;
		m_profil[3] = 0;
		m_xMax = 43;
		m_yMax = 20;
		m_xMin = 28;
		break;
	case 2:
		DrawFeld(x,y,color);
		DrawFeld(x+3,y,color);
		DrawFeld(x+3,y+2,color);
		DrawFeld(x+3,y+4,color);
		m_aktueleBreite = 2;
		m_profil[0] = 0;
		m_profil[1] = 2;
		m_profil[2] = 0;
		m_profil[3] = 0;
		m_xMax = 45;
		m_yMax = 18;
		m_xMin = 28;
		break;
	case 3:
		DrawFeld(x+6,y,color);
		DrawFeld(x,y+2,color);
		DrawFeld(x+3,y+2,color);
		DrawFeld(x+6,y+2,color);
		m_aktueleBreite = 3;
		m_profil[0] = 1;
		m_profil[1] = 1;
		m_profil[2] = 1;
		m_profil[3] = 0;
		m_xMax = 43;
		m_yMax = 20;
		m_xMin = 28;
		break;
	case 4:
		DrawFeld(x,y,color);
		DrawFeld(x,y+2,color);
		DrawFeld(x,y+4,color);
		DrawFeld(x+3,y+4,color);
		m_aktueleBreite = 2;
		m_profil[0] = 2;
		m_profil[1] = 2;
		m_profil[2] = 0;
		m_profil[3] = 0;
		m_xMax = 45;
		m_yMax = 18;
		m_xMin = 28;
		break;
	}
}

void CTetrisFiguren::J_Figur(char m_Stellung, char x, char y, char color)
{
	switch(m_Stellung)
	{
	case 1:
		DrawFeld(x,y,color);
		DrawFeld(x+3,y,color);
		DrawFeld(x+6,y,color);
		DrawFeld(x+6,y+2,color);
		m_aktueleBreite = 3;
		m_profil[0] = 0;
		m_profil[1] = 0;
		m_profil[2] = 1;
		m_profil[3] = 0;
		m_xMax = 43;
		m_yMax = 20;
		m_xMin = 28;
		break;
	case 2:
		DrawFeld(x,y+4,color);
		DrawFeld(x+3,y,color);
		DrawFeld(x+3,y+2,color);
		DrawFeld(x+3,y+4,color);
		m_aktueleBreite = 2;
		m_profil[0] = 2;
		m_profil[1] = 2;
		m_profil[2] = 0;
		m_profil[3] = 0;
		m_xMax = 45;
		m_yMax = 18;
		m_xMin = 28;
		break;
	case 3:
		DrawFeld(x,y,color);
		DrawFeld(x,y+2,color);
		DrawFeld(x+3,y+2,color);
		DrawFeld(x+6,y+2,color);
		m_aktueleBreite = 3;
		m_profil[0] = 1;
		m_profil[1] = 1;
		m_profil[2] = 1;
		m_profil[3] = 0;
		m_xMax = 43;
		m_yMax = 20;
		m_xMin = 28;
		break;
	case 4:
		DrawFeld(x,y,color);
		DrawFeld(x,y+2,color);
		DrawFeld(x,y+4,color);
		DrawFeld(x+3,y,color);
		m_aktueleBreite = 2;
		m_profil[0] = 2;
		m_profil[1] = 0;
		m_profil[2] = 0;
		m_profil[3] = 0;
		m_xMax = 45;
		m_yMax = 18;
		m_xMin = 28;
		break;
	}
}

void CTetrisFiguren::I_Figur(char m_Stellung, char x, char y, char color)
{
	if(m_Stellung == 1 || m_Stellung == 3)
	{
		DrawFeld(x,y,color);
		DrawFeld(x+3,y,color);
		DrawFeld(x+6,y,color);
		DrawFeld(x+9,y,color);
		m_aktueleBreite = 4;
		m_sonderfall = false;
		m_profil[0] = 0;
		m_profil[1] = 0;
		m_profil[2] = 0;
		m_profil[3] = 0;
		m_xMax = 40;
		m_yMax = 22;
		m_xMin = 28;
	}
	else
	{
		DrawFeld(x+3,y-2,color);
		DrawFeld(x+3,y,color);
		DrawFeld(x+3,y+2,color);
		DrawFeld(x+3,y+4,color);
		m_aktueleBreite = 1;
		m_sonderfall = true;
		m_profil[0] = 2;
		m_profil[1] = 0;
		m_profil[2] = 0;
		m_profil[3] = 0;
		m_xMax = 45;
		m_yMax = 18;
		m_xMin = 25;
	}
}

void CTetrisFiguren::O_Figur(char m_Stellung, char x, char y, char color)
{
	DrawFeld(x,y,color);
	DrawFeld(x+3,y,color);
	DrawFeld(x,y+2,color);
	DrawFeld(x+3,y+2,color);
	m_aktueleBreite = 2;
	m_profil[0] = 1;
	m_profil[1] = 1;
	m_profil[2] = 0;
	m_profil[3] = 0;
	m_xMax = 45;
	m_yMax = 20;
	m_xMin = 28;
}

void CTetrisFiguren::Z_Figur(char m_Stellung, char x, char y, char color)
{
	switch(m_Stellung)
	{
	case 1:
		DrawFeld(x,y,color);
		DrawFeld(x+3,y,color);
		DrawFeld(x+3,y+2,color);
		DrawFeld(x+6,y+2,color);
		m_aktueleBreite = 3;
		m_profil[0] = 0;
		m_profil[1] = 1;
		m_profil[2] = 1;
		m_profil[3] = 0;
		m_xMax = 43;
		m_yMax = 20;
		m_xMin = 28;
		break;
	case 2:
		DrawFeld(x+3,y,color);
		DrawFeld(x+3,y+2,color);
		DrawFeld(x,y+2,color);
		DrawFeld(x,y+4,color);
		m_aktueleBreite = 2;
		m_profil[0] = 2;
		m_profil[1] = 1;
		m_profil[2] = 0;
		m_profil[3] = 0;
		m_xMax = 45;
		m_yMax = 18;
		m_xMin = 28;
		break;
	case 3:
		DrawFeld(x,y,color);
		DrawFeld(x+3,y,color);
		DrawFeld(x+3,y+2,color);
		DrawFeld(x+6,y+2,color);
		m_aktueleBreite = 3;
		m_profil[0] = 0;
		m_profil[1] = 1;
		m_profil[2] = 1;
		m_profil[3] = 0;
		m_xMax = 43;
		m_xMin = 28;
		m_yMax = 20;
		break;
	case 4:
		DrawFeld(x+3,y,color);
		DrawFeld(x+3,y+2,color);
		DrawFeld(x,y+2,color);
		DrawFeld(x,y+4,color);
		m_aktueleBreite = 2;
		m_profil[0] = 2;
		m_profil[1] = 1;
		m_profil[2] = 1;
		m_profil[3] = 0;
		m_xMax = 45;
		m_yMax = 18;
		m_xMin = 28;
		break;
	}
}

void CTetrisFiguren::S_Figur(char m_Stellung, char x, char y, char color)
{
	switch(m_Stellung)
	{
	case 1:
		DrawFeld(x+3,y,color);
		DrawFeld(x+6,y,color);
		DrawFeld(x,y+2,color);
		DrawFeld(x+3,y+2,color);
		m_aktueleBreite = 3;
		m_profil[0] = 1;
		m_profil[1] = 1;
		m_profil[2] = 0;
		m_profil[3] = 0;
		m_xMax = 43;
		m_yMax = 20;
		m_xMin = 28;
		break;
	case 2:
		DrawFeld(x,y,color);
		DrawFeld(x,y+2,color);
		DrawFeld(x+3,y+2,color);
		DrawFeld(x+3,y+4,color);
		m_aktueleBreite = 2;
		m_profil[0] = 1;
		m_profil[1] = 2;
		m_profil[2] = 0;
		m_profil[3] = 0;
		m_xMax = 45;
		m_yMax = 18;
		m_xMin = 28;
		break;
	case 3:
		DrawFeld(x+3,y,color);
		DrawFeld(x+6,y,color);
		DrawFeld(x,y+2,color);
		DrawFeld(x+3,y+2,color);
		m_aktueleBreite = 3;
		m_profil[0] = 1;
		m_profil[1] = 1;
		m_profil[2] = 0;
		m_profil[3] = 0;
		m_xMax = 43;
		m_yMax = 20;
		m_xMin = 28;
		break;
	case 4:
		DrawFeld(x,y,color);
		DrawFeld(x,y+2,color);
		DrawFeld(x+3,y+2,color);
		DrawFeld(x+3,y+4,color);
		m_aktueleBreite = 2;
		m_profil[0] = 1;
		m_profil[1] = 2;
		m_profil[2] = 0;
		m_profil[3] = 0;
		m_xMax = 45;
		m_yMax = 18;
		m_xMin = 28;
		break;
	}
}

void CTetrisFiguren::Erstellen(void)
{
	Felder.Tabele();
	m_x = 28+(3*3);
	m_y = 0;
	m_Stellung = 1;
	m_actuelleSpalte = 4;
	m_actuelleZeile = 0;
	m_actuelleFigur = m_nextFigur;
	m_actuelleFarbe = m_nextFarbe;
	Draw();
	//Abfragen();

	srand ( time(NULL) );
	m_nextFigur = 1+ rand() % 7;
	m_nextFarbe = 8+ rand() % 7;
	if (m_nextFarbe == 8) m_nextFarbe = 4;
}

void CTetrisFiguren::Draw(void)
{
	switch(m_actuelleFigur)
	{
	case 1: T_Figur(m_Stellung, m_x, m_y, m_actuelleFarbe); break;
	case 2: L_Figur(m_Stellung, m_x, m_y, m_actuelleFarbe); break;
	case 3: J_Figur(m_Stellung, m_x, m_y, m_actuelleFarbe); break;
	case 4: O_Figur(m_Stellung, m_x, m_y, m_actuelleFarbe); break;
	case 5: S_Figur(m_Stellung, m_x, m_y, m_actuelleFarbe); break;
	case 6: Z_Figur(m_Stellung, m_x, m_y, m_actuelleFarbe); break;
	case 7: I_Figur(m_Stellung, m_x, m_y, m_actuelleFarbe); break;
	}
}

void CTetrisFiguren::Clear(void)
{
	switch(m_actuelleFigur)
	{
	case 1: T_Figur(m_Stellung, m_x, m_y, BGRND); break;
	case 2: L_Figur(m_Stellung, m_x, m_y, BGRND); break;
	case 3: J_Figur(m_Stellung, m_x, m_y, BGRND); break;
	case 4: O_Figur(m_Stellung, m_x, m_y, BGRND); break;
	case 5: S_Figur(m_Stellung, m_x, m_y, BGRND); break;
	case 6: Z_Figur(m_Stellung, m_x, m_y, BGRND); break;
	case 7: I_Figur(m_Stellung, m_x, m_y, BGRND); break;
	}
}

void CTetrisFiguren::MoveDown(void)
{
	unsigned char i_tick = 0;
	char c_input = 0; 

	while(m_y < m_yMax)
	{
		i_tick++;
		Debug();
        
        if(kbhit()) c_input = getch();

		if(c_input == 72 ) // nach oben
		{
			if(m_actuelleFigur != 7)
			{
				Clear();
				if(m_Stellung < 4)++m_Stellung;
				else m_Stellung = 1;
				Rotieren();
				Draw();
				Abfragen();
				c_input = 0;
			}
			else
			if(m_actuelleZeile !=0)
			{
				Clear();
				if(m_Stellung < 4)++m_Stellung;
				else m_Stellung = 1;
				Rotieren();
				Draw();
				Abfragen();
				c_input = 0;
			}
		}
		if(c_input == 75 ) // nach links
		{
			Clear();
			if (m_x > m_xMin)
			{
				m_x -= 3;
				--m_actuelleSpalte;
			}
			Draw();
			Abfragen();

			c_input = 0;
		}
		if(c_input == 77 ) // nach rechts
		{
			Clear();
			if (m_x < m_xMax)
			{
				m_x += 3;
				++m_actuelleSpalte;
			}
			Draw();
			Abfragen();

			c_input = 0;
		}
		if(c_input == 80 ) // nach unten
		{
			Clear();
			Abfragen();
			m_actuelleZeile = m_setzeInZeile;
			m_y = m_yMax;
			Draw();
			break;

			c_input = 0;
		}
		if(c_input == 32 ) // Pause
		{
			c_input = getch();
			if(c_input == 32)
			c_input = 0;
		}

		Sleep(10);
		if(i_tick >= 100)
		{
			Clear();
			m_y += 2;
			Draw();
			i_tick = 0;
			++m_actuelleZeile;
			Abfragen();
		}
    }
	Schreiben();
	Loeschen();
	if ( m_setzeInZeile < 2) m_voll = false;
}

void CTetrisFiguren::Vorschau(void)
{
	Screen.FarbeSetzen(7,1);
	Screen.Umramung(53,2,64,15,'D');
	Screen.Position(55,1);
	printf("Nechste");

	switch(m_actuelleFigur)
	{
	case 1: T_Figur(2, 56, 4, 1); break;
	case 2: L_Figur(2, 56, 4, 1); break;
	case 3: J_Figur(2, 56, 4, 1); break;
	case 4: O_Figur(2, 56, 4, 1); break;
	case 5: S_Figur(2, 56, 4, 1); break;
	case 6: Z_Figur(2, 56, 4, 1); break;
	case 7: I_Figur(2, 55, 6, 1); break;
	}

	switch(m_nextFigur)
	{
	case 1: T_Figur(2, 56, 4, m_nextFarbe); break;
	case 2: L_Figur(2, 56, 4, m_nextFarbe); break;
	case 3: J_Figur(2, 56, 4, m_nextFarbe); break;
	case 4: O_Figur(2, 56, 4, m_nextFarbe); break;
	case 5: S_Figur(2, 56, 4, m_nextFarbe); break;
	case 6: Z_Figur(2, 56, 4, m_nextFarbe); break;
	case 7: I_Figur(2, 55, 6, m_nextFarbe); break;
	}
}

void CTetrisFiguren::Rotieren(void)
{
	// коррекция поворота для I_Figur
	if(m_actuelleFigur == 7)
	{
		if(m_actuelleSpalte >= 6)
		{
				m_x = 40;
				m_actuelleSpalte = 5;
		}

		if(m_sonderfall)
		{
			--m_actuelleSpalte;
		}
		else
		{
			++m_actuelleSpalte;
		}
	}
	else 
	{
		if(m_actuelleSpalte >= 7)
		{
			m_x -= 3;
			--m_actuelleSpalte;
		}
	}
}

void CTetrisFiguren::Abfragen(void)
{
	// хранит строки в массиве которые уже заняты фигурами
	char * belegteZeile = new char[m_aktueleBreite];
	// хранит растояние от строк в массиве которые уже заняты фигурами до возможной строки 
	// после падения актуальной фигуры
	char * kurzAbstand = new char[m_aktueleBreite];
	
	for (char i = 0; i < m_aktueleBreite; i++) belegteZeile[i]=12;
	
	for (char j = 0; j < m_aktueleBreite; j++)
	{
		for (char i = 0; i < 12; i++)
		{
			// здесь сохраняется номер строки которая занята
			if(Felder.m_Belegung[(m_actuelleSpalte-1)+j][i] > 0) 
			{
				belegteZeile[j] = i;
				break;
			}
		}
	}

	// расчитываем в какую строку должна быть размещена падающая фигура
	// для этого используем профиль фигуры
	
	m_setzeInZeile = 12;

	for (char i = 0; i < m_aktueleBreite; i++) 
	{
		kurzAbstand[i] = belegteZeile[i]-(m_profil[i]+1);
	}

	for (char i = 0; i < m_aktueleBreite; i++) if ( m_setzeInZeile > kurzAbstand[i] ) m_setzeInZeile = kurzAbstand[i];

	m_yMax = m_setzeInZeile*2;

	Screen.Position(5,16);
	for (char i = 0; i < m_aktueleBreite; i++) printf("%i ", belegteZeile[i]);

	delete []belegteZeile;
	delete []kurzAbstand;
}

void CTetrisFiguren::Schreiben(void)
{
	Felder.Schreiben(m_Stellung, m_actuelleSpalte-1, m_actuelleZeile, m_actuelleFigur, m_actuelleFarbe);
}

void CTetrisFiguren::Loeschen(void)
{
	Felder.Loeschen();
}

void CTetrisFiguren::InitArrays(void)
{
	Felder.ZeroArray();
}

void CTetrisFiguren::Debug(void)
{
	Screen.FarbeSetzen(7,1);
	Screen.Position(2,16);
	printf("m_actuelleSpalte = %i",m_actuelleSpalte);
	Screen.Position(2,17);
	printf("m_actuelleFigur = %i",m_actuelleFigur);
	Screen.Position(2,18);
	printf("m_x = %i",m_x);
	Screen.Position(2,19);
	printf("m_xMax = %i",m_xMax);
	Screen.Position(2,20);
	printf("m_setzeInZeile = %i",m_setzeInZeile);
	Screen.Position(2,21);
	printf("m_yMax = %i",m_yMax);
	/*
	printf("m_actuelleZeile = %i",m_actuelleZeile);
	printf("m_actuelleFarbe = %i",m_actuelleFarbe);
	printf("m_nextFarbe = %i",m_nextFarbe);
	printf("m_aktueleBreite = %i",m_aktueleBreite);
	printf("m_setzeInZeile = %i",m_setzeInZeile);
	printf("m_nextFigur = %i",m_nextFigur);
	printf("m_Stellung = %i",m_Stellung);
	
	
	printf("m_xMin = %i",m_xMin);
	printf("m_y = %i",m_y);
	
	*/
}
