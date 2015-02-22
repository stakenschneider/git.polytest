#pragma once
#include "SpielFeld.h"

class CBrett : public CSpielFeld
{
public:

	CBrett(void);
	~CBrett(void);

	void Zeichne(char x_location, char y_location, char count_hor, char count_vetr, char bgr_color_1, char bgr_color_2);
	void Trennlinie(char x_location, char y_location);
	void RedrawFeld(char x_location, char y_location, char color);

};
