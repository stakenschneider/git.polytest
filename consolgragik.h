//    #define BLACK 0	#define RED        4	#define DARK_GRAY    8		#define LIGHT_RED     12
//    #define BLUE  1	#define MAGENTA    5	#define LIHGT_BLUE   9		#define LIGHT_MAGENTA 13
//    #define GREEN 2	#define BROWN      6	#define LIGHT_GREEN  10		#define YELLOW        14
//    #define CYAN  3	#define LIGHT_GRAY 7	#define LIGHT_CYAN   11		#define WHITE         15

#pragma once

class CConsolenGrafik
{
public:

	CConsolenGrafik(void);
	~CConsolenGrafik(void);

	void Position(int x, int y);
	void FarbeSetzen(int i_textFarbe, int i_hintergrundFarbe);
	void ZeichenKette(int von_x, int bis_x, int i_zeile, int i_hintergrundFarbe);
	void ZeichenKette(int von_x, int bis_x, int i_zeile, int i_textFarbe, int i_hintergrundFarbe);
	void ZeichenKette(int von_x, int bis_x, int i_line, int i_zeichen, int i_textFarbe, int i_hintergrundFarbe);
	void ZeichenSpalte(int i_spalte, int von_y, int bis_y, int i_hintergrundFarbe);
	void ZeichenSpalte(int i_spalte, int von_y, int bis_y, int i_textFarbe, int i_hintergrundFarbe);
	void ZeichenSpalte(int i_spalte, int von_y, int bis_y, int i_zeichen, int i_textFarbe, int i_hintergrundFarbe);
	void Farben(int von_x, int bis_x, int von_y, int bis_y, int i_Farbe);
	void Farben(int von_x, int bis_x, int von_y, int bis_y, int i_textFarbe, int i_hintergrundFarbe);
	void Umramung(int von_x, int von_y, int bis_x, int bis_y, char type);
	void BgrFarbe(int farbe);

};
