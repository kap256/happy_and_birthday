#pragma once

#include "main.h"
#include "battleconst.h"
#include "gamewindow.h"
#include "characterdata.h"

enum BattleMenuType{
	BMenuTurnStart,
	BMenuPersonal,
	BMenuSelectSkill,
	BMenuUseItem,
	BMenuSetTarget,
	BMenuMoveCharacter,
	BMenuResultExp,
	BMenuCheck,
};

class CBattleMenu{
protected:
	int				mDeleteTimer;
	bool			mUpdate;
	CListWindow		mList;
	CBattleMenu*	mNext;
public:
						CBattleMenu();
	virtual				~CBattleMenu();
	virtual	void		Init(int param);
	virtual bool		OnTimer(void);
			void		DrawAll(void);
	virtual void		OnDraw(void);
	virtual void		Destroy(void);
			void		SetMenu(BattleMenuType type,int param);
			void		AllDelete(int n=0);
	virtual void		Update(void){return;};
protected:
	bool		CheckTop();
	D3DCOLOR	GetColor();

	void		FlashCharacter(int chara,bool reset=true);
};

class CTurnStart:public CBattleMenu{
	static const int MENU_NO		=3;
public:
	void	Init(int param);
	bool	OnTimer(void);
};


class CPersonal:public CBattleMenu{
	static const int MENU_NO		=5;

	int		mPlayerNum;
	int		mWeapon[MAX_ITEM+2];
	int		mWeaponNo;
	bool	mSkill[3];
	CSelectWindow			mSWindow;
	CCharaDataWindowMini	mCWindow;
public:
	void	Init(int param);
	bool	OnTimer(void);
	void	OnDraw(void);
	void	Update(void);
	void	Destroy(void);
private:
	void	ChangeWeapon(int w);
};

class CSelectSkill:public CBattleMenu{
	static const int SP_WIDTH	=40;

	CPlaneWindow mSPWin;

	int		mSPGoal;
	double	mSPX;
	int		mPlayerNum;
	int		mSkillType;
	int		mSkill[MAX_CSKILL];
	bool	mSP[MAX_CSKILL];
	int		mSaveCursor;
public:
	CSelectSkill();

	void	Init(int param);
	bool	OnTimer(void);
	void	OnDraw(void);
	void	Destroy(void);
	void	Update(void);
};

class CSetTarget:public CBattleMenu{

	int		mPlayerNum;
	int		mSkill;
	int		mTarget[CHARACTER_NO];
	int		mRealTarget;
	int		mTimer;
	int		mOldCursor;
	int		mSaveCursor;
public:
	CSetTarget();
	void	Init(int param);
	bool	OnTimer(void);
	void	Destroy(void);
};

class CMoveCharcter:public CBattleMenu{

	int		mPlayerNum;
	int		mX;
	int		mY;
	int		mTimer;
public:
	void	Init(int param);
	bool	OnTimer(void);
	void	OnDraw(void);
	void	Destroy(void);
};

class CResultExp:public CBattleMenu{
	static const int TIMER_START_ADD	=	120;
	static const int TIMER_END_ADD		=	TIMER_START_ADD+60;
	static const int LV_UP_TIME			=	60;
	CPlaneWindow mWindow;
	CPlaneWindow mMoneyWindow;
	CPlaneWindow mSubWindow;

	int		mTimer;
	double	mExp[MAX_PARTY_NUMBER];
	int		mMoney;
	int		mLvUp[MAX_PARTY_NUMBER];

public:
	void	Init(int param);
	bool	OnTimer(void);
	void	OnDraw(void);
	void	Destroy(void);
};

class CUseItem:public CBattleMenu{

	int		mPlayerNum;

public:
	void	Init(int param);
	bool	OnTimer(void);
	void	Destroy(void);
	void	Update(void);
};

class CCheck:public CBattleMenu{
public:
	void	Init(int param);
	bool	OnTimer(void);
	void	Destroy(void);
};