#ifndef _TEXTURELIST_
#define _TEXTURELIST_

#pragma warning(disable:4996)

#include <d3d9.h>
#include <d3dx9.h>
#include <dxerr9.h>

typedef class CTextureList{
	CRITICAL_SECTION	mCS;
	int					mNumber;
	LPDIRECT3DTEXTURE9  lpDirectTexture;

public:

	char		mFileName[256];
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

	void	ChangeNumber	(int i);
	void	SetNumber		(int i);
	int		GetNumber		(void);

	LPDIRECT3DTEXTURE9	GetTexture(void);
	void				SetTexture(LPDIRECT3DTEXTURE9 tex);

}CTextureList;

#endif
