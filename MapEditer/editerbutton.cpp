#include "editerbutton.h"
#include "stdio.h"


void CEditerButton::Init(int x,int y,int img,D3DCOLOR color)
{
	mX=x;
	mY=y;
	mImg=img;
	mColor=color;
}

bool CEditerButton::OnTimer(CDTTexture &tex,int mx,int my)
{
	if(mX<=mx && mx<=mX+32 && mY<=my && my<=mY+32){
		DTest.DrawBox(mX,mY,mX+32,mY+32,0xffffff00);
		DTest.DrawTexture(tex,0,mImg%4*32,mImg/4*32,32,32,mX,mY);
		return true;
	}

	DTest.DrawBox(mX,mY,mX+32,mY+32,mColor);
	DTest.DrawTexture(tex,0,mImg%4*32,mImg/4*32,32,32,mX,mY,32,32,0xffb0b0b0);
	DTest.DrawTexture(tex,0,mImg%4*32,mImg/4*32,32,32,mX-2,mY-2);
	return false;
}
