#include "dtesttexture.h"

CTextureList* CDTTexture::mAllTexture=NULL;
CDTTexture::CDTTexture()
{
	mLoaded=true;
	mTexture=NULL;
#ifdef TEXTURE_CS
	InitializeCriticalSection(&mCS);
#endif
}
CDTTexture::~CDTTexture()
{
	Release();
#ifdef TEXTURE_CS
	DeleteCriticalSection(&mCS);
#endif
}
bool CDTTexture::ReleaseForLoadTexture(const char *filename)
{
//	InitLock();
	if(!mAllTexture)	return true;
	if(!mTexture)		return true;
	if(filename){
		if(strcmp(filename,mTexture->mFileName)==0){
			return false;
		}
	}
	mTexture->ChangeNumber(-1);
	mTexture=NULL;
//	mAllTexture=CTextureList::CheckDelete(mAllTexture);
	return true;
}
bool CDTTexture::Release(void)
{
	LoadLock();
	if(!mAllTexture)	return true;
	if(!mTexture)		return true;
	mTexture->ChangeNumber(-1);
	mTexture=NULL;
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
	LoadLock();
	if(!mAllTexture){
		return NULL;
	}
	if(mTexture){
		return mTexture->GetTexture();
	}
	return NULL;
}
bool CDTTexture::GetSize(float* x,float* y)
{
	LoadLock();
	if(!mTexture){
		return false;
	}
	*x=float(mTexture->mWidth);
	*y=float(mTexture->mHeight);
	return true;
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
void CDTTexture::SetLoaded(bool load)
{
#ifdef TEXTURE_CS
	EnterCriticalSection(&mCS);
#endif
	mLoaded=load;
#ifdef TEXTURE_CS
	LeaveCriticalSection(&mCS);
#endif
}
void CDTTexture::LoadLock(void)
{
	while(1){
#ifdef TEXTURE_CS
		EnterCriticalSection(&mCS);
#endif
		if(mLoaded){	
#ifdef TEXTURE_CS
			LeaveCriticalSection(&mCS);
#endif
			return;
		}
#ifdef TEXTURE_CS
		LeaveCriticalSection(&mCS);
#endif
		Sleep(0);
	}
}

bool CDTTexture::CheckLoad(void)
{
#ifdef TEXTURE_CS
	EnterCriticalSection(&mCS);
#endif
	bool ret;
	ret=mLoaded;
#ifdef TEXTURE_CS
	LeaveCriticalSection(&mCS);
#endif
	return ret;
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

