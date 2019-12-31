#pragma once

#include "DTestLib/dtest.h"

class CPlaneWindow;

const int ELEMENT_Height	=22;

class CElement{
protected:
	const char	*mName;
public:
	int			mID;

public:
					CElement(void);
					CElement(const char* name,int id);
	virtual			~CElement();
	virtual void	Draw(CPlaneWindow* window,int x,int y,int param,D3DCOLOR color=0xffffffff) const;
	virtual int		GetHeight(void)const{return ELEMENT_Height;};
	virtual void	Help(int param)const;
	virtual const char*	GetName()const {return mName;};
};

class CItem:public CElement{
public:
	int		mHelpID;

public:
	CItem(void);
	CItem(const char* name,int id,int help);
	void	Help(int param) const;
	virtual int		GetHeight(void)const{return ELEMENT_Height;};
};

class CLargeItem:public CItem{
public:
	CLargeItem(const char* name,int id,int help) : CItem(name,id,help){};
	void	Draw(CPlaneWindow* window,int x,int y,int param,D3DCOLOR color=0xffffffff) const
	{	CElement::Draw(window,x,y+ELEMENT_Height/2,param,color);};
	virtual int		GetHeight(void)const{return 40;};
};
class CKeyConfigItem:public CItem{
public:
	CKeyConfigItem(const char* name,int id,int help) : CItem(name,id,help){};
	void	Draw(CPlaneWindow* window,int x,int y,int param,D3DCOLOR color=0xffffffff) const;
};

class CIconItem:public CElement{
public:
	static const int ICON_Height = 34;
	int		mIcon;

public:
	CIconItem(void);
	CIconItem(const char* name,int id,int icon);

	void	Draw(CPlaneWindow* window,int x,int y,int param,D3DCOLOR color=0xffffffff) const;
	int		GetHeight(void)const{return ICON_Height;};
};
class CVillageItem:public CIconItem{
public:
	int		mX;
	int		mY;

public:
	CVillageItem(void);
	CVillageItem(const char* name,int id,int icon,int x,int y);


};

class CSaveDataItem:public CElement{
public:
	static const int SAVE_DATA_HEIGHT = 48;

	int mMonth;
	int mMode;
	int mPlayTime;
	int mClearData;
public:
	CSaveDataItem(void);

	void	Init(int id,int month,int mode,int playtime,int cleardata);
	void	Draw(CPlaneWindow* window,int x,int y,int param,D3DCOLOR color=0xffffffff) const;
	int		GetHeight(void)const{return SAVE_DATA_HEIGHT;};
};

class CSelectDifficultItem:public CItem{
	CDTTexture mTex;
public:
	CSelectDifficultItem(const char* name,int id,int help);
	void	Draw(CPlaneWindow* window,int x,int y,int param,D3DCOLOR color=0xffffffff) const;
	virtual int		GetHeight(void)const{return 80;};
};
/*

*/