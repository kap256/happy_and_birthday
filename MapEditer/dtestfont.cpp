#include "dtestfont.h"

CDTFont::CDTFont(){
	mFont=NULL;
	mInit=false;
}
CDTFont::~CDTFont(){
	Release();
}
bool CDTFont::Release(void)
{
	//mInit=false;
	if(mFont){
		mFont->Release();
		mFont=NULL;
		return true;
	}
	return false ;
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
