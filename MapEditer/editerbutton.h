#ifndef _EDITERBUTTON_
#define _EDITERBUTTON_

#include "dtest.h"
#include "../main.h"


class CEditerButton{
	int mX;
	int mY;
	int mImg;
	D3DCOLOR mColor;

public:
	void Init(int x,int y,int img,D3DCOLOR color);
	bool OnTimer(CDTTexture &tex,int mx,int my);
};

#endif