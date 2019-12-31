#ifndef _DTESTTEXTURE_
#define _DTESTTEXTURE_

#include <d3d9.h>
#include <d3dx9.h>
#include <dxerr9.h>
#include "texturelist.h"

#define		TEXTURE_CS

class CDTTexture{
private:
	static CTextureList* mAllTexture;

private:
	CTextureList*		mTexture;
	bool				mLoaded;
#ifdef TEXTURE_CS
	CRITICAL_SECTION	mCS;
#endif


public:
	CDTTexture(void);
	~CDTTexture(void);
	bool ReleaseForLoadTexture(const char *filename);
	bool Release();

	void SetTexture(LPDIRECT3DTEXTURE9 texture,const char* filename,int x,int y,bool rt,D3DCOLOR color);
	bool SeachTexture(const char* filename);

	bool GetSize(float* x,float* y);
	bool CheckRenderTarget(void){return mTexture->mRenderTarget;};

	void SetLoaded(bool load);
	bool CheckLoad(void);
	void LoadLock(void);

	static void DeleteAllTexture(void);
	static void EscapeAllTexture(void);
	static CTextureList*	GetTextureList(void){return mAllTexture;};

	LPDIRECT3DTEXTURE9 GetTexture(void);

	void	SaveTexture(LPCTSTR pDestFile)
	{
		D3DXSaveTextureToFile(pDestFile,D3DXIFF_PNG,GetTexture(),NULL);
	}
};

#endif