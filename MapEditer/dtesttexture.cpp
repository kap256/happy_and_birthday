#include "dtesttexture.h"

CTextureList* CDTTexture::mAllTexture=NULL;
CDTTexture::CDTTexture()
{
	mTexture=NULL;
}
CDTTexture::~CDTTexture()
{
	Release();
}
bool CDTTexture::Release(const char *filename)
{
	if(!mAllTexture)	return true;
	if(!mTexture)	return true;
	if(filename){
		if(strcmp(filename,mTexture->mFileName)==0){
			return false;
		}
	}
	mTexture->mNumber--;
	mTexture=NULL;
//	mAllTexture=CTextureList::CheckDelete(mAllTexture);
	return true;
}
void CDTTexture::SetTexture(LPDIRECT3DTEXTURE9 texture,const char* filename,int x,int y,bool rt,D3DCOLOR color)
{
	if(mAllTexture){
		mTexture=CTextureList::SetTexture(mAllTexture,texture,filename,x,y,rt,color);
	} else {
		mAllTexture=new CTextureList;
		mAllTexture->CreateTexture(texture,filename,x,y,rt,color);
		mTexture=mAllTexture;
	}
}
LPDIRECT3DTEXTURE9 CDTTexture::GetTexture(void)
{
	if(!mAllTexture)	return NULL;
	if(mTexture){
		return mTexture->lpDirectTexture;
	}
	return NULL;
}
bool CDTTexture::SeachTexture(const char *filename)
{
	if(mAllTexture){
		CTextureList* list;
		list=mAllTexture->SeachTexture(filename);
		if(list){
			mTexture=list;
			return true;
		}
	}
	return false;
}

void CDTTexture::DeleteAllTexture(void)
{
	if(mAllTexture) mAllTexture->AllDelete();
	mAllTexture=NULL;
}
void CDTTexture::EscapeAllTexture(void)
{
	if(mAllTexture) mAllTexture->AllEscape();
}

