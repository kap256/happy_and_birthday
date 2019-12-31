#pragma once

#include "action.h"

class CActNormalAttack: public CBattleAction{
	int mSound;
	int mEffect;
	int mPow;
public:
	CActNormalAttack(int sound,int effect,int pow);
	int		OnTimer();
};

class CActThrust: public CBattleAction{
public:
	int		OnTimer();
};
class CActShoot: public CBattleAction{
	int mPow;
public:
	CActShoot(int pow);
	int		OnTimer();
};
class CActLaser: public CBattleAction{
public:
	int		OnTimer();
};
class CActHardAttack: public CBattleAction{
	int mPow;
	int	mEffect;
	int mSound;
public:
	CActHardAttack(int pow,int effect,int sound);
	int		OnTimer();
};
class CActSpinAttack: public CBattleAction{
	int mPow;
	int	mMin;
public:
	CActSpinAttack(int pow,int min);
	int		OnTimer();
};
class CActParamUp: public CBattleAction{
	int			mParam;
	D3DCOLOR	mColor;
public:
	CActParamUp(int param,D3DCOLOR color);
	int		OnTimer();
};
class CActParamDown: public CBattleAction{
	int			mParam;
	D3DCOLOR	mColor;
public:
	CActParamDown(int param,D3DCOLOR color);
	int		OnTimer();
};
class CActCure: public CBattleAction{
	int			mSize;
	int			mNo;
	bool		mDown;
	bool		mHeal;
public:
	CActCure(int size,int no,bool down,bool heal);
	int		OnTimer();
};
class CActGus: public CBattleAction{
	int	mST;
public:
	CActGus(int st);
	int		OnTimer();
};
class CActMagicField: public CBattleAction{
	int	mField;
public:
	CActMagicField(int field);
	int		OnTimer();
};
class CActReflection: public CBattleAction{
	int	mPow;
public:
	CActReflection(int pow);
	int		OnTimer();
};
class CActBarrier: public CBattleAction{
	int	mPow;
public:
	CActBarrier(int pow);
	int		OnTimer();
};
class CActRevival: public CBattleAction{
	int	mPow;
	bool mLive;
public:
	CActRevival(int pow,bool live);
	int		OnTimer();
};
class CActSkillDrain: public CBattleAction{
	int	mLv;
	int mPow;
	D3DCOLOR mColor;
public:
	CActSkillDrain(int lv,int pow,D3DCOLOR color);
	int		OnTimer();
};
class CActCallPet: public CBattleAction{
	int	mNo;
public:
	CActCallPet(int no);
	int		OnTimer();
};
class CActDeathWord: public CBattleAction{
	int	mST;
	D3DCOLOR mColor;
public:
	CActDeathWord(int st,D3DCOLOR color);
	int		OnTimer();
};
class CActBokoboko: public CBattleAction{
public:
	int		OnTimer();
};
class CActCallEnemy: public CBattleAction{
	int mMin;
	int mMax;
	int mLv;
public:
	CActCallEnemy(int min,int max,int lv){mMin=min;mMax=max;mLv=lv;}
	int		OnTimer();
};
class CActSong: public CBattleAction{
	int mST;
	D3DCOLOR mColor;
public:
	CActSong(int st,D3DCOLOR color){mST=st;mColor=color;};
	int		OnTimer();
};