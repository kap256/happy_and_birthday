#pragma once


#include <d3d9.h>

#include "battleconst.h"

const	int	BMES_TIME_BOUND		=16;
const	int	BMES_TIME_ONE_BOUND	=4;
const	int	BMES_TIME_VANISH	=128+BMES_TIME_BOUND;
const	int	BMES_TIME_DELETE	=16;
const	int	BMES_WIDTH			=14;


class CBattleMessage{
protected:
	char			*mMessage;
	int				mX;
	int				mY;
	D3DCOLOR		mColor1;
	D3DCOLOR		mColor2;
	int				mSize;
	int				mTimer;
	int				mDeleteTimer;
	CBattleMessage*	mNext;

public:
	CBattleMessage();
	virtual ~CBattleMessage();
	CBattleMessage**	SearchEnd(void);
	void				Init(int x,int y,const char* mes,D3DCOLOR color){Init(x,y,mes,color,color);};
	void				Init(int x,int y,const char* mes,D3DCOLOR color1,D3DCOLOR color2);
	void				OnTimer(void);
	void				OnDraw(int	depth[BATTLE_SQUARE_WIDTH][BATTLE_SQUARE_Height]);
	virtual void		DrawMessage(int	depth[BATTLE_SQUARE_WIDTH][BATTLE_SQUARE_Height]);
	void				Kill();
	void				AllDelete(void);
	CBattleMessage*		GetNext(void){return mNext;};
};

const int NMES_TYPEMAX  = 3;
	const int NMES_TYPE_DMG = 0;
	const int NMES_TYPE_LP  = 1;
	const int NMES_TYPE_HEAL= 2;

class CNumberMessage:public CBattleMessage{
public:
	static D3DCOLOR GetColor1(int color,int type);
	static D3DCOLOR GetColor2(int color,int type);

public:
	int mType;
	int mDmg;
	int mID;
	int mTarget;
	int mColor;
	bool mCrt;

public:
	virtual ~CNumberMessage(){};
	void Init(int x,int y,int dmg,int type,int id,int target,const char* mes,int color,bool crt);
};