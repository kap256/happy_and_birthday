#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include <dxerr9.h>

struct CDTFont{
	static LPD3DXSPRITE	mSprite;

	bool			mInit;
	LPD3DXFONT		mFont;
	int				mSize;
	int				mWeight;
	bool			mItalic;
	const char*		mName;

public:
	CDTFont(void);
	~CDTFont(void);
	bool Release(void);

	void		SetFont(LPD3DXFONT font,int size,const char* fontname,int weight,bool italic);
	LPD3DXFONT	GetFont(){return mFont;};
};
