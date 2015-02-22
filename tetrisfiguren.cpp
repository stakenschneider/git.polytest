#pragma once

class CTetrisFiguren
{
private:
	void DrawFeld(char x_location, char y_location, char color);

public:

	char m_profil[4];

	char m_actuelleSpalte;
	char m_actuelleZeile;
	char m_actuelleFigur;
	char m_nextFigur;
	char m_actuelleFarbe;
	char m_nextFarbe;
	char m_aktueleBreite;
	char m_setzeInZeile;
	char m_Stellung;
	char m_x;
	char m_xMax;
	char m_xMin;
	char m_y;
	char m_yMax;

	bool m_sonderfall;
	bool m_voll;

	CTetrisFiguren(void);
	~CTetrisFiguren(void);

	void T_Figur(char m_Stellung, char x, char y, char color);
	void L_Figur(char m_Stellung, char x, char y, char color);
	void J_Figur(char m_Stellung, char x, char y, char color);
	void I_Figur(char m_Stellung, char x, char y, char color);
	void O_Figur(char m_Stellung, char x, char y, char color);
	void Z_Figur(char m_Stellung, char x, char y, char color);
	void S_Figur(char m_Stellung, char x, char y, char color);

	void Erstellen(void);
	void Draw(void);
	void Clear(void);
	void MoveDown(void);
	void Vorschau(void);
	void Rotieren(void);
	void Abfragen(void);
	void Schreiben(void);
	void Loeschen(void);
	void InitArrays(void);
	void Debug(void);

};
