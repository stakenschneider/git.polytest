#pragma once

#define SPALTEN 8
#define ZEILEN  12
class CSpielFeld
{
private:
	void SaveFeld(char x_location, char y_location, char color);

public:
	char m_spalten;
	char m_zeilen;

	char m_locale1;
	char m_locale2;
	char m_locale3;
	char m_locale4;

	char m_Belegung[SPALTEN][ZEILEN];
	char m_Farben[SPALTEN][ZEILEN];

	CSpielFeld(void);
	~CSpielFeld(void);

	void ZeroArray(void);
	void Schreiben(char m_Stellung, char m_actuelleSpalte, char m_actuelleZeile, char m_actuelleFigur, char m_actuelleFarbe);
	void Tabele(void);
	void Loeschen(void);

	void T_Figur(char m_Stellung, char x, char y, char color);
	void L_Figur(char m_Stellung, char x, char y, char color);
	void J_Figur(char m_Stellung, char x, char y, char color);
	void I_Figur(char m_Stellung, char x, char y, char color);
	void O_Figur(char m_Stellung, char x, char y, char color);
	void Z_Figur(char m_Stellung, char x, char y, char color);
	void S_Figur(char m_Stellung, char x, char y, char color);
};
