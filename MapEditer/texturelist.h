#ifndef _TEXTURELIST_
#define _TEXTURELIST_

#include <d3d9.h>
#include <d3dx9.h>
#include <dxerr9.h>

typedef struct CTextureList{
	LPDIRECT3DTEXTURE9 lpDirectTexture;

	char		mFileName[256];
	int			mNumber;
	int			mWidth;
	int			mHeight;
	D3DCOLOR	mColor;
	bool		mRenderTarget;

	LPD3DXBUFFER  mBuffer;
	CTextureList* mNextList;


	CTextureList(void);
	~CTextureList(void);
	void Release(void);
	void ReleaseBuffer(void);

	static CTextureList* SetTexture(CTextureList* texturelist,LPDIRECT3DTEXTURE9 texture,const char* filename,int x,int y,bool rt,D3DCOLOR color);
	static CTextureList* CheckDelete(CTextureList* texturelist);

	void CreateTexture(LPDIRECT3DTEXTURE9 texture,const char* filename,int x,int y,bool rt,D3DCOLOR color);
	CTextureList* SeachTexture(const char* filename);
	void AllDelete(void);
	void AllEscape(void);

}CTextureList;

#endif
