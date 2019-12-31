#ifndef _DTESTTEXTURE_
#define _DTESTTEXTURE_

#include <d3d9.h>
#include <d3dx9.h>
#include <dxerr9.h>
#include "texturelist.h"

class CDTTexture{
private:
	static CTextureList* mAllTexture;

private:
	CTextureList* mTexture;

public:
	CDTTexture(void);
	~CDTTexture(void);
	bool Release(const char *filename=NULL);

	void SetTexture(LPDIRECT3DTEXTURE9 texture,const char* filename,int x,int y,bool rt,D3DCOLOR color);
	bool SeachTexture(const char* filename);

	bool GetSize(float* x,float* y){if(!mTexture)return false;*x=float(mTexture->mWidth);*y=float(mTexture->mHeight);return true;};
	bool CheckRenderTarget(void){return mTexture->mRenderTarget;};

	static void DeleteAllTexture(void);
	static void EscapeAllTexture(void);
	static CTextureList*	GetTextureList(void){return mAllTexture;};

	LPDIRECT3DTEXTURE9 GetTexture(void);
};

#endif