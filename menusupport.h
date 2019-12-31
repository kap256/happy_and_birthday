#pragma once

#include "gamewindow.h"
#include "characterdata.h"


class CSelectCharacterSystem{

	bool	mLive;

	int		mX;
	int		mY;
	int		mCharacter;
	bool	mReserve;

	int		mCursorTimer;
	int		mMoveTimer;
	bool	mLeft;
public:
	CSelectCharacterSystem();
	void	Init(int x,int y,int character,bool reserve);
	void	Update();
	void	OnDraw();
	int		OnTimer(bool cursor);
	void	Destroy(void);
	void	SetReserve(bool rev);

private:
	int		ChangeCharacter(int change);
	int		GetCharacter(int vector,int length);
};
class CSelectCharacterShopSystem{

	bool			mLive;

	int				mMember[MAX_CHARACTER_NUMBER+1];
	CDTTexture		mMemberTex[MAX_CHARACTER_NUMBER+1];
	int				mMemberNo;

	int				mCharacter;
	CPlaneWindow	mWindow;

public:
	CSelectCharacterShopSystem();
	void	Init(int x,int y,bool allmember,bool closet);
	int		OnTimer(bool cursor);
	void	OnDraw();
	void	Destroy(void);

	int		GetCharacter(int no){return mMember[no];}
	int		GetCharacter(void){return GetCharacter(mCharacter);}
};

class CBirthStone:public CElement{
public:
	CBirthStone(void);
	CBirthStone(int id);
	CBirthStone(const char* name,int id);
	void	Draw(CPlaneWindow* window,int x,int y,int param,D3DCOLOR color=0xffffffff) const;
	int		GetHeight(void)const{return 32;};
private:
	void DrawStone(CPlaneWindow* window,CDTTexture& tex,int x,int y,int tx,int ty) const; 
};

class CBirthStoneSystem{
public:
	static const int MONTH_LIST_X		= 20;
	static const int MONTH_LIST_Y		= 20;
	static const int MONTH_LIST_WIDTH	= 280;

	static void GetBSParam(int character,int* param);

private:
	bool mLive;
	int  mDeleteTimer;

	int		mCharacter;
	int		mFreeSpace;
	bool	mSenken;

	CListWindow				mMonthList;
	CPlaneWindow			mSkillList;
	CPlaneWindow			mSkillTitle;
	CSelectCharacterSystem	mSWindow;

	int		mSkill[MAX_SKILL_MONTH];
	int		mLearn[MAX_SKILL_MONTH];

	int		mSelectFS;
public:
	CBirthStoneSystem();
	void	Init(int character,int fs=-1);
	void	InitMonthList(void);
	void	ResetMonthList(void);
	void	InitSkillList(void);
	void	OnDraw();
	int		OnTimer(bool timer);
	void	Destroy(void);
	void	Update(void);

	int				GetCursor(void);
	const CElement*	GetCursorItem(void);
	int				GetCharacter(void){return mCharacter;};
};
class COverItemSystem{
	static const int SELECT_X[5];

	bool	mLive;
	int		mDeleteTimer;

	int		mTimer;
	int		mCursor;
	int		mMode;
	int		mCharacter;
	int		mMoveTimer;
	bool	mLeft;
	bool	mNoItem;

	CPlaneWindow			mWindow;
	CItemHelpWindow			mHelpWindow;
	CListWindow				mTempItem;
public:
	COverItemSystem();
	~COverItemSystem();
	void	Init();
	bool	OnTimer(bool timer);
	void	OnDraw();
	void	Destroy(void);

	void	ResetList(void);
private:
	void	GetHelp(void);
};

class CRemoveBirthStoneEle:public CElement{
	mutable CDTTexture mTex;
public:
	CRemoveBirthStoneEle(void);
	CRemoveBirthStoneEle(int id);
	void	Draw(CPlaneWindow* window,int x,int y,int param,D3DCOLOR color=0xffffffff) const;
	int		GetHeight(void)const{return 48;};
};
