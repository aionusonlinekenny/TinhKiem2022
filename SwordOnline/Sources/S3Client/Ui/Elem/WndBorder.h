#ifndef WNDBORDER_H_
#define WNDBORDER_H_
#pragma once


#include "../../Represent/iRepresent/KRepresentUnit.h"
enum eTypeEquip
{
	VioletEquip,
	GoldEquip,
	PlatinaEquip,
	NumType
};

void DrawBorder(unsigned char& cFrame, unsigned char& ucTimeDelay, INT& zpos, unsigned char& zdir, unsigned char& ztimedelay, INT x, INT y, INT width, INT height, INT nNature);

#endif
