#include "dtestfont.h"
LPD3DXSPRITE CDTFont::mSprite=NULL;

CDTFont::CDTFont(){
	mFont=NULL;
	mSprite=NULL;
	mInit=false;
}
CDTFont::~CDTFont(){
	Release();
}
bool CDTFont::Release(void)
{
	//mInit=false;
	bool ret=false;
	if(mFont){
		mFont->Release();
		mFont=NULL;
		ret=true;
	}
	return ret ;
}
void CDTFont::SetFont(LPD3DXFONT font,int size,const char* fontname,int weight,bool italic)
{
	mInit=true;
	mFont=font;
	mSize=size;
	mName=fontname;
	mWeight=weight;
	mItalic=italic;
}
