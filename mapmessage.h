#pragma once

//#include "main.h"
#include "gamewindow.h"
#include "number.h"
#include "mapconst.h"

const int MAP_MESSAGE_NO	=8;
const int MAP_SELECT_MAX	=4;


class CMapMessage :public CPlaneWindow{
protected:
	char			mMessage[256];
	int				mOrigin;
	CNumber			mMapMesX;
	CNumber			mMapMesY;
	CNumber			mMapMesZ;
	int				mMesTimer;
	int				mParam;

public:
	CMapMessage(void);
	void	InitMapMessage(const char* mes,CNumber x,CNumber y,CNumber z,int origin,int para);
	int 	GetMapMessageNo(const char* mes,int& n);
	void 	AddMapMessage(const char* mes,int& n,const char* text);
	virtual void	OnTimer(void);
	virtual void	OnDraw(int cx,int cy);
	void	DrawWindow(int cx,int cy);
	void	CulculateXY(int w,int h,int cx,int cy);
	int		GetTextEnd(const char* text,int no);
	void	QuickDestroy();

};

class CMapSelectWindow :public CMapMessage{
	int mGoto[MAP_SELECT_MAX];
	int mMesTop[MAP_SELECT_MAX];
	int mNo;
	int mSelect;
public:
	void InitSelectWindow(const char* mes,CNumber x,CNumber y,CNumber z,int origin,int no,int* go);
	void	OnTimer(void);
	void	OnDraw(int cx,int cy);
	int		GetGotoLine(void){return mGoto[mSelect];};
	int		GetGotoLine(int n){return mGoto[n];};
};

const	double	MMES_SPEED_Z		=0.3;
const	int		MMES_TIME_VANISH	=48;
const	int		MMES_TIME_DELETE	=16+MMES_TIME_VANISH;
const	int		MMES_WIDTH			=7;


class CMapMiniMessage{
	char		*mMessage;
	int			mX;
	int			mY;
	double		mZ;
	D3DCOLOR	mColor;
	int			mSize;
	int			mTimer;
	CMapMiniMessage*	mNext;

public:
	CMapMiniMessage();
	~CMapMiniMessage();
	void		SetMessage(int x,int y,int z,const char* mes,D3DCOLOR color);
	void		Init(int x,int y,int z,const char* mes,D3DCOLOR color);
	void		OnTimer(void);
	void		OnDraw(int cx,int cy);
	void		AllDelete(void);
};
