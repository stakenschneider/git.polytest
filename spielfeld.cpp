#include "StdAfx.h"
#include "SpielFeld.h"
#include "ConsolenGrafik.h"
#include "Brette.h"

CConsolenGrafik  Screen2;
CBrett Segment;

CSpielFeld::CSpielFeld(void)
{
	for (char i = 0; i< 12; i++)
		for (char j = 0; j< 8; j++)
		{
			m_Belegung[j][i] = 0;
			m_Farben[j][i] = 6;
		}
}

CSpielFeld::~CSpielFeld(void)
{
}

void CSpielFeld::ZeroArray(void)
{
	for (char i = 0; i< 12; i++)
		for (char j = 0; j< 8; j++)
		{
			m_Belegung[j][i] = 0;
			m_Farben[j][i] = 6;
		}
}


void CSpielFeld::Schreiben(char m_Stellung, char m_actuelleSpalte, char m_actuelleZeile, char m_actuelleFigur, char m_actuelleFarbe)
{
	switch(m_actuelleFigur)
	{
	case 1: T_Figur(m_Stellung, m_actuelleSpalte, m_actuelleZeile, m_actuelleFarbe); break;
	case 2: L_Figur(m_Stellung, m_actuelleSpalte, m_actuelleZeile, m_actuelleFarbe); break;
	case 3: J_Figur(m_Stellung, m_actuelleSpalte, m_actuelleZeile, m_actuelleFarbe); break;
	case 4: O_Figur(m_Stellung, m_actuelleSpalte, m_actuelleZeile, m_actuelleFarbe); break;
	case 5: S_Figur(m_Stellung, m_actuelleSpalte, m_actuelleZeile, m_actuelleFarbe); break;
	case 6: Z_Figur(m_Stellung, m_actuelleSpalte, m_actuelleZeile, m_actuelleFarbe); break;
	case 7: I_Figur(m_Stellung, m_actuelleSpalte, m_actuelleZeile, m_actuelleFarbe); break;
	}
}

void CSpielFeld::Tabele(void)
{	
	Screen2.FarbeSetzen(7,1);
	Screen2.Umramung(15,2,26,15,'D');
	Screen2.Position(17,1);
	printf("Belegung");
	
	for (char i = 0; i < 12; i++)
		for (char j = 0; j < 8; j++)
		{
			Screen2.Position(17+j,3+i);
			printf("%i", m_Belegung[j][i]);
		}

	Screen2.Umramung(2,2,13,15,'D');
	Screen2.Position(5,1);
	printf("Farben");
	
	for (char i = 0; i < 12; i++)
		for (char j = 0; j < 8; j++)
		{
			Screen2.Position(4+j,3+i);
			printf("%i", m_Farben[j][i]);
		}
}

void CSpielFeld::Loeschen(void)
{
	char Zeile = 12;
	char Summe = 0;
	char SteckBelegung[8];
	char SteckFarben[8];

	for (char i = 0; i < 12; i++)
	{
		for (char j = 0; j < 8; j++)
		{
			Summe += m_Belegung[j][i];
		}
		if (Summe == 8) 
		{
			Zeile = i;
			Summe = 0;
		}
		Summe = 0;
	}

	if (Zeile < 12)
	{
		// начинает перемещать все верхние строки вниз
		for (char i = Zeile-1; i > 0; i--)
		{
			for (char j = 0; j < 8; j++) 
			{
				// получил строку
				SteckBelegung[j] = m_Belegung[j][i];
				SteckFarben[j] = m_Farben[j][i];
			}
			for (char j = 0; j < 8; j++) 
			{
				// поместил на строку ниже
				m_Belegung[j][i+1] = SteckBelegung[j];
				m_Farben[j][i+1] = SteckFarben[j];
				// отображает цвет
				Segment.RedrawFeld(28+j*3, (i+1)*2, SteckFarben[j]);
			}
		}
		Loeschen();
	}
}

void CSpielFeld::T_Figur(char m_Stellung, char x, char y, char color)
{
	switch(m_Stellung)
	{
	case 1:
		SaveFeld(x,y,color);
		SaveFeld(x+1,y,color);
		SaveFeld(x+2,y,color);
		SaveFeld(x+1,y+1,color);
		break;
	case 2:
		SaveFeld(x,y+1,color);
		SaveFeld(x+1,y,color);
		SaveFeld(x+1,y+1,color);
		SaveFeld(x+1,y+2,color);
		break;
	case 3:
		SaveFeld(x+1,y,color);
		SaveFeld(x,y+1,color);
		SaveFeld(x+1,y+1,color);
		SaveFeld(x+2,y+1,color);
		break;
	case 4:
		SaveFeld(x,y,color);
		SaveFeld(x,y+1,color);
		SaveFeld(x+1,y+1,color);
		SaveFeld(x,y+2,color);
		break;
	}
}

void CSpielFeld::L_Figur(char m_Stellung, char x, char y, char color)
{
	switch(m_Stellung)
	{
	case 1:
		SaveFeld(x,y,color);
		SaveFeld(x+1,y,color);
		SaveFeld(x+2,y,color);
		SaveFeld(x,y+1,color);
		break;
	case 2:
		SaveFeld(x,y,color);
		SaveFeld(x+1,y,color);
		SaveFeld(x+1,y+1,color);
		SaveFeld(x+1,y+2,color);
		break;
	case 3:
		SaveFeld(x+2,y,color);
		SaveFeld(x,y+1,color);
		SaveFeld(x+1,y+1,color);
		SaveFeld(x+2,y+1,color);
		break;
	case 4:
		SaveFeld(x,y,color);
		SaveFeld(x,y+1,color);
		SaveFeld(x,y+2,color);
		SaveFeld(x+1,y+2,color);
		break;
	}
}

void CSpielFeld::J_Figur(char m_Stellung, char x, char y, char color)
{
	switch(m_Stellung)
	{
	case 1:
		SaveFeld(x,y,color);
		SaveFeld(x+1,y,color);
		SaveFeld(x+2,y,color);
		SaveFeld(x+2,y+1,color);
		break;
	case 2:
		SaveFeld(x,y+2,color);
		SaveFeld(x+1,y,color);
		SaveFeld(x+1,y+1,color);
		SaveFeld(x+1,y+2,color);
		break;
	case 3:
		SaveFeld(x,y,color);
		SaveFeld(x,y+1,color);
		SaveFeld(x+1,y+1,color);
		SaveFeld(x+2,y+1,color);
		break;
	case 4:
		SaveFeld(x,y,color);
		SaveFeld(x,y+1,color);
		SaveFeld(x,y+2,color);
		SaveFeld(x+1,y,color);
		break;
	}
}

void CSpielFeld::I_Figur(char m_Stellung, char x, char y, char color)
{
	if(m_Stellung == 1 || m_Stellung == 3)
	{
		SaveFeld(x,y,color);
		SaveFeld(x+1,y,color);
		SaveFeld(x+2,y,color);
		SaveFeld(x+3,y,color);
	}
	else
	{
		SaveFeld(x,y-1,color);
		SaveFeld(x,y,color);
		SaveFeld(x,y+1,color);
		SaveFeld(x,y+2,color);
	}
}

void CSpielFeld::O_Figur(char m_Stellung, char x, char y, char color)
{
	SaveFeld(x,y,color);
	SaveFeld(x+1,y,color);
	SaveFeld(x,y+1,color);
	SaveFeld(x+1,y+1,color);
}

void CSpielFeld::Z_Figur(char m_Stellung, char x, char y, char color)
{
	switch(m_Stellung)
	{
	case 1:
		SaveFeld(x,y,color);
		SaveFeld(x+1,y,color);
		SaveFeld(x+1,y+1,color);
		SaveFeld(x+2,y+1,color);
		break;
	case 2:
		SaveFeld(x+1,y,color);
		SaveFeld(x+1,y+1,color);
		SaveFeld(x,y+1,color);
		SaveFeld(x,y+2,color);
		break;
	case 3:
		SaveFeld(x,y,color);
		SaveFeld(x+1,y,color);
		SaveFeld(x+1,y+1,color);
		SaveFeld(x+2,y+1,color);
		break;
	case 4:
		SaveFeld(x+1,y,color);
		SaveFeld(x+1,y+1,color);
		SaveFeld(x,y+1,color);
		SaveFeld(x,y+2,color);
		break;
	}
}

void CSpielFeld::S_Figur(char m_Stellung, char x, char y, char color)
{
	switch(m_Stellung)
	{
	case 1:
		SaveFeld(x+1,y,color);
		SaveFeld(x+2,y,color);
		SaveFeld(x,y+1,color);
		SaveFeld(x+1,y+1,color);
		break;
	case 2:
		SaveFeld(x,y,color);
		SaveFeld(x,y+1,color);
		SaveFeld(x+1,y+1,color);
		SaveFeld(x+1,y+2,color);
		break;
	case 3:
		SaveFeld(x+1,y,color);
		SaveFeld(x+2,y,color);
		SaveFeld(x,y+1,color);
		SaveFeld(x+1,y+1,color);
		break;
	case 4:
		SaveFeld(x,y,color);
		SaveFeld(x,y+1,color);
		SaveFeld(x+1,y+1,color);
		SaveFeld(x+1,y+2,color);
		break;
	}
}

void CSpielFeld::SaveFeld(char x_location, char y_location, char color)
{
	m_Belegung[x_location][y_location] = 1;
	m_Farben[x_location][y_location] = color;
}
