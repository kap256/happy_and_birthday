#pragma once

#include "main.h"
#include "gamewindow.h"
#include "characterdata.h"
#include "menusupport.h"
#include "battlecharacter.h"
#include "treasure.h"
#include "itemdata.h"
#include <iostream>

enum EMenuName{
	StartMenu,
	ItemMenu,
	ItemSubMenu,
	ExchangeItem,
	DumpItem,
	UISelectCharacter,
	BirthStoneMenu,
	LearnSkillMenu,
	LearnSkillYesNo,
	StatusMenu,
	FormationMenu,
	BoxStartMenu,
	BoxSelectMenu,
	BoxOpenMenu,
	OverItemMenu,
	ItemHelp,
	EquipmentHelp,
	SkillHelp,
	ComSkillHelp,
	EventSkip,
	ShopMenu,
	FleaMarketMenu,
	FarmMenu,
	ReturnVillage,
	DiaryMenu,
	ClosetMenu,
	MonthStartMenu,
	ChangeMemberMenu,
	BattleStatusMenu,
	ConfigMenu,
	SellMenu,
	SellYesNoMenu,
	FreeSpaceMenu,
	FSBirthStone,
	FSSelectSkill,
	GlobalConfigMenu,
	KeyConfigMenu,
	SeedMenu,
	DInfoMenu,
	BypathMenu,
	StatusHelp,
	RemoveBirthStone,
	RemoveBSPersonal,
	SelectDifficult,
	ExitYesNo,
	SuccessSaveData,
	CallPetHelp,
	TutorialMenu,
	SoundTestMenu,
	PreciousItemMenu,
};

const int		MENU_BG_TIME		=32;
const int		MENU_BG_SIZE		=32;
const D3DCOLOR	MENU_BG_COLOR		=0x80404000;

const int		MENU_PIECES			=(640+480)/MENU_BG_SIZE/2;
const int		MENU_PIECE_TIME		=MENU_BG_TIME-MENU_PIECES;

const int		ITEM_LIST_WIDTH		=320;
const int		ID_CLOSET			=256;


class CMenuData{
friend CSelectCharacterSystem;
public:
	static	void	InitItemList(int character,CItemListWindow* window,int x,int y,int h=-1);
	static	void	InitItemList(CPlayerData &cdata,CItemListWindow* window,int x,int y,int h);
	static	void	InitItemList(int character,CListWindow* window,int x,int y,int h=-1);
	static	void	InitItemList(CPlayerData &cdata,CListWindow* window,int x,int y,int h);
	static	void	UpdataItemList(int character,CListWindow* window);
	static	void	UpdataItemList(CPlayerData &cdata,CListWindow* window);

	static  void	GetItemListInfo(CPlayerData &cdata,int* maxitem,int* itemlist,int* param);
	static	void	InitClosetList(CListWindow* window,int x,int y,int category,int h,bool none=true,bool pagemove=true);
	static	void	UpdataClosetList(CListWindow* window,int category,bool none=true);
	static	char*	GetHelpText(const char* fname,int no);
	static	char*	GetItemText(int no);
	static	char*	GetHelpTextShort(const char* fname,int no);
	static  char*	GetItemTextShort(int item,bool newline=false);

	static	void	InitReserveCDataWin();

protected:
	static	int		mBGTimer;
	static	bool	mDrawBG;
	static	CCharaDataWindowMenu	mCDataWindow[MAX_CHARACTER_NUMBER+MAX_PARTY_NUMBER];


private:
	int			mDeleteTimer;
	bool		mUpdate;
protected:
	CMenuData*	mNext;

public:
					CMenuData();
	virtual			~CMenuData();
	virtual bool	OnTimer(void);
	virtual void	OnDraw(void);
	virtual void	Update(void);
	virtual void	Destroy(void);
			void	DrawBG(bool time);
			void	SetDrawBG(bool draw){mDrawBG=draw;};
			void	SetMenu(EMenuName menu,int param);
			void	AllDelete(void);
			void	Delete(const type_info& info);

			bool		GetMenuOpend(void){return (mNext && mNext->mDeleteTimer==0);}
			bool		GetMenuOpening(void){return (mNext!=NULL);}
			CMenuData*	SearchMenu(const type_info& info);
	
			bool	UseItem(int character,int item,int target);
			CMenuData* GetNext(void){return mNext;};
///////////////////////ユーズアイテム関数////////////////////////////
			void	UI_LPHeal(int item,int target);
			void	UI_SPHeal(int item,int target);
			void	UI_BirthStone(void);
			void	UI_Save(void);
			void	UI_Closet(void);
};


class CStartMenu:public CMenuData{
private:
	static const int MENU_NO	=11;
	CListWindow		mWindow;
	CPlaneWindow	mInfo;

	int		mNextMenu;
	int		mCharacter;
	int		mReserveNo;

	bool	mCDataMove;

public:
			CStartMenu(int param);
			~CStartMenu();
	bool	OnTimer(void);
	void	OnDraw(void);
	void	Destroy(void);
private:
	void	InitList(void);
	void	SetNextMenu(int next);
};

class CItemMenu:public CMenuData{
public:
	static const int LIST_X = 10;
	static const int LIST_Y = 10;

protected:
	int			mCharacter;
	int			mCategory;
	int			mCategoryAll;
	int			mMode;
	CPlayerData mPData;

	CItemListWindow			mItemList;
	CSelectCharacterSystem	mSWindow;
	CItemHelpWindow			mHelpWindow;
	
	CSelectWindow			mCategoryWindow;
	CListWindow				mClosetList;
	CCharaDataWindowEquip	mCompare;

public:
					CItemMenu(int param);
	virtual			~CItemMenu();
	
	virtual	bool	OnTimer(void);
	virtual	void	OnDraw(void);
			void	Update(void);
	virtual	void	Destroy(void);

	void	OpenCloset();

private:
	void	ChangeCharacter(int chara);
	void	SetReserveParam();
	void	CloseCloset();
	void	ChangeHelp();
	void	CheckItemCursor(int item);
};

class CItemSubMenu:public CMenuData{
private:
	int		mCharacter;
	int		mItem;
	int		mAccessoryTimer;

	CListWindow				mList;
	CPlaneWindow			mTitle;
	CCharaDataWindowEquip	mCompare;
public:
			CItemSubMenu(int param);
			~CItemSubMenu();
	
	bool	OnTimer(void);
	void	OnDraw(void);
	void	Update(void);
	void	Destroy(void);
private:
	void	ChangeWeapon(void);
};

class CExchangeItem:public CItemMenu{
	static const int INFO_Height=ITEM_Height+30+32;

	int		mExchangeCharacter;
	int		mExchangeItem;

	CPlaneWindow			mExchangeInfo;
public:
			CExchangeItem(int param);
			~CExchangeItem();
	
	bool	OnTimer(void);
	void	OnDraw(void);
	void	Destroy(void);
private:
	void	ChangeCharacter(void);
};
class CDumpItem:public CMenuData{
private:
	int		mCharacter;
	int		mItem;

	CListWindow		mList;
	CPlaneWindow	mTitle;

public:
			CDumpItem(int param);
			~CDumpItem();
	
	bool	OnTimer(void);
	void	OnDraw(void);
	void	Destroy(void);
};

class CUIMenuSelectCharacter:public CMenuData{
private:
	int		mUseCharacter;
	int		mUseItem;

	int		mCharacter;

	int		mDTime;

	CPlaneWindow	mTitle;

public:
			CUIMenuSelectCharacter(int param);
			~CUIMenuSelectCharacter();
	
	bool	OnTimer(void);
	void	OnDraw(void);
	void	Destroy(void);
};

class CBirthStoneMenu:public CMenuData{
public:
	CBirthStoneSystem mBirthStoneSystem;

public:
			CBirthStoneMenu(){return;};
			CBirthStoneMenu(int param);
			~CBirthStoneMenu();
	
	virtual bool	OnTimer(void);
			void	OnDraw(void);
	virtual void	Update(void);
			void	Destroy(void);
};
class CLearnSkillMenu:public CMenuData{
protected:
	static const int Y =110;

	int			mCharacter;
	int			mMonth;
	int			mStoneNo[2];
	int			mMessageTimer;
	const char*	mMessage;
	CPlayerData mAfter;
	int			mActiveWindow;
	char*		mHelp;

	CListWindow				mLearnSkill;
	CCharaDataWindowCompare	mCompare;
	CPlaneWindow			mMessageWindow[2];
	CPlaneWindow			mHelpWindow;
	CPlaneWindow			mBirthStoneWindow;
	const CBirthStone*		mBirthStone;
	int						mBSParam;


public:
					CLearnSkillMenu(int param);
	virtual			~CLearnSkillMenu();
	
	virtual	bool	OnTimer(void);
	virtual	void	OnDraw(void);
	virtual void	Update(void);
	virtual	void	Destroy(void);
protected:
	void			SetMessage(const char* mes);
	virtual void	MoveCursor(int no);
	virtual void	Select(void);
};

class CLearnSkillYesNo:public CMenuData{
private:
	int		mCharacter;
	int		mMonth;
	int		mSkill;

	CListWindow		mList;
	CPlaneWindow	mTitle;

public:
			CLearnSkillYesNo(int param);
			~CLearnSkillYesNo();
	
	bool	OnTimer(void);
	void	OnDraw(void);
	void	Destroy(void);
};

class CStatusMenu:public CMenuData{
protected:
	static const int BASE_X		=	60;
	static const int ADD_X		=	10;
	static const int TOTAL_X	=	130;
	static const int START_Y	=	180;
	static const int LINE_Y		=	25;
	static const int LIST_TYPE	=	4;

	int					mCharacterNum;
	int					mListType;
	bool				mDelete;
	bool				mReserve;
	bool				mAllMember;
	bool				mChangeable;
	CBattleCharacter*	mCharacter;
	CPlayerCharacter*	mPlayer;
	CPlaneWindow		mWindow;
	CPlaneWindow		mListTitle;
	CPlaneWindow		mListHelp;
	CListWindow			mList;

public:
					CStatusMenu(int param);
					CStatusMenu();
	virtual			~CStatusMenu();
	virtual	int		Init(int param);
			void	ChangeList(void);
			bool	OnTimer(void);
			void	OnDraw(void);
	virtual void	DrawStatus(void);
			void	DrawParam(const char* name,int no,int param,int base,bool open=true);
			void	Destroy(void);
};
class CFormationMenu:public CMenuData{

	int		mCharacter;
	bool	mSelected;
	int		mX;
	int		mY;
	CPlayerCharacter	mPlayer[MAX_PARTY_NUMBER];
	CPlaneWindow		mWindow;
	int		mTimer;
public:
			CFormationMenu(int param);
			~CFormationMenu();
	
	bool	OnTimer(void);
	void	OnDraw(void);
	void	Destroy(void);
};
class CBoxStartMenu:public CMenuData{

	int		mBox;
	bool	mKnowTrap;
	int		mX;

	CListWindow			mList;
	CPlaneWindow		mWindow;

public:
			CBoxStartMenu(int param);
			~CBoxStartMenu();
	
	bool	OnTimer(void);
	void	Update(void);
	void	OnDraw(void);
	void	Destroy(void);
};
class CBoxSelectMenu:public CMenuData{

	int		mBox;
	bool	mKnowTrap;
	int		mCharacter;
	int		mKey[MAX_KEY_TYPE];
	bool	mUseKey[MAX_PARTY_NUMBER][MAX_KEY_TYPE];
	int		mOpen[MAX_PARTY_NUMBER];

	CPlaneWindow mParamWindow;

public:
			CBoxSelectMenu(int param);
			~CBoxSelectMenu();
	
	bool	OnTimer(void);
	void	OnDraw(void);
	void	Destroy(void);
private:
	void	InitParamWin(void);
};

class COverItemMenu:public CMenuData{
	
	bool	mCDataInited;

	COverItemSystem mSystem;

public:
			COverItemMenu(int param);
			~COverItemMenu();

	bool	OnTimer(void);
	void	Update(void);
	void	OnDraw(void);
	void	Destroy(void);
};
class CItemHelp:public CMenuData{
protected:
	int		mItemNo;
	int		mAllNo;
	char*	mText;

	CPlaneWindow		mWindow;
public:
			CItemHelp(int param);
			~CItemHelp();

	bool	OnTimer(void);
	void	OnDraw(void);
	void	Destroy(void);
};

class CSelectHelp:public CMenuData{
protected:
	static const int	LIST_MAX	=8;
	int					mHeight;
	int					mWidth;
	int					mListSize;
private:
	int					mCursor;
	int					mEleHeight;
	CElement const*		mList[LIST_MAX];

protected:
	CPlaneWindow		mWindow;

public:
			CSelectHelp();
			~CSelectHelp();

	virtual bool	OnTimer(void);
	virtual void	OnDraw(void);
	virtual void	Destroy(void);
protected:
	int	InitList(const CElement* list[],int size,int h,int w);
};

class CEquipmentHelp:public CSelectHelp{
protected:
	int		mItemNo;
	int		mAllNo;
	char*	mText;
public:
			CEquipmentHelp(int param);
			~CEquipmentHelp();

	bool	OnTimer(void);
	void	OnDraw(void);
	void	Destroy(void);
private:
	void	DrawParam(const char* name,int param,int y);
};

class CSkillHelp:public CMenuData{
protected:
	int		mSkillNo;
	char*	mText;

	CPlaneWindow		mWindow;
public:
			CSkillHelp(){return;};
			CSkillHelp(int param);
			~CSkillHelp();

	bool	OnTimer(void);
	void	OnDraw(void);
	void	Destroy(void);
};
class CComSkillHelp:public CSkillHelp{
public:
			CComSkillHelp(int param);
	void	OnDraw(void);
};

class CEventSkipMenu:public CMenuData{
private:
	CListWindow		mList;
	CPlaneWindow	mTitle;

public:
			CEventSkipMenu(int param);
			~CEventSkipMenu();
	
	bool	OnTimer(void);
	void	OnDraw(void);
	void	Destroy(void);
};
class CShopMenu:public CMenuData{
protected:
	int			mShopNo;

	int			mShopMode;
	int			mSelectMember;
	int			mCursorTimer;
	char*		mHelp;

	CListWindow		mList;
	CPlaneWindow	mTalk;
	CPlaneWindow	mInfo;

	CSelectCharacterShopSystem mSWindow;

	CListWindow				mPlayerItem;
	CCharaDataWindowEquip	mCompare;

public:
			CShopMenu();
			CShopMenu(int param);
	virtual	~CShopMenu();
	
	virtual bool	OnTimer(void);
	virtual void	OnDraw(void);
	virtual void	Destroy(void);
protected:
			void	Init(int param);
	virtual void	InitList(void);
	virtual bool	CheckMoney(void);
			void	InitPlayerData(void);
			void	ChangePlayerData(void);
			void	ReturnMode(void);
	virtual bool	Buy(void);
			void	GetHelp(void);
			virtual	void	Update(void){};
};
class CFleaMarketMenu:public CShopMenu{
public:
	CFleaMarketMenu(int param);
	~CFleaMarketMenu();

protected:
	void	InitList(void);
	bool	Buy(void);
};

class CFarmMenu:public CMenuData{
	int		mItemNo[MAX_SEED_NUMBER];
	char	*mText;
	bool	mCDataMove;
	int		mSelectMember;
	int		mMode;
	
	CListWindow				mList;
	CPlaneWindow			mHelp;
	CSelectCharacterSystem	mSWindow;
	CListWindow				mPlayerItem;
public:
	CFarmMenu(int param);
	~CFarmMenu();
	
	virtual bool	OnTimer(void);
	virtual void	OnDraw(void);
	virtual void	Destroy(void);
protected:
	void	SelectFarmItem(void);
	void	SelectCharacterItem(void);

	void	UpdataList(void);
	void	InitPlayerItem(void);
	void	UpdataPlayerItem(void);
	void	SetPlayerItemCursor(void);
	void	GetHelp(void);
};

class CSeedMenu:public CMenuData{
	static const int START_TIME		= 16;
	static const int THROW_TIME		= 32;
	static const int THROW_END_TIME	= START_TIME+THROW_TIME;
	static const int NEW_ITEM_TIME	= THROW_END_TIME+140;
	static const int SHAKE_TIME		= 48;
	static const int JUMP_TIME		= 32;
	static const int END_TIME		= JUMP_TIME+NEW_ITEM_TIME;

	int mTimer;
	int mSeedNo;
	int mNewItem;
	int mNewItemNo;
	char* mHelp;

	CPlaneWindow mSeedWindow;
	CPlaneWindow mWindow;
public:
	CSeedMenu(int param);
	~CSeedMenu();

	void	OnDraw(void);
	void	Destroy(void);
	bool	OnTimer(void);
private:
	bool	CheckSeed(void);
};
class CSellMenu:public CShopMenu{
public:
	CSellMenu(int param);

protected:
	bool	OnTimer(void);
	void	InitList(void);
	void	ChangeCharacter(void);
	void	UpdataList(void);
	void	Update(void);
};
class CSellYesNoMenu:public CMenuData{ 
	CListWindow		mList;
	CPlaneWindow	mTitle;

	int mCharacter;
	int mItemNo;
	int mItemID;
	int mSellNo;
	int	mSellMax;
	int mTimer;
	int mMoveTimer;

public:
			CSellYesNoMenu(int param);
			~CSellYesNoMenu();
	
	bool	OnTimer(void);
	void	OnDraw(void);
	void	Destroy(void);
};

class CReturnVillage:public CMenuData{
private:
	CListWindow		mList;
	CPlaneWindow	mTitle;

public:
			CReturnVillage(int param);
			~CReturnVillage();
	
	bool	OnTimer(void);
	void	OnDraw(void);
	void	Destroy(void);
};
class CDiaryMenu:public CMenuData{
protected:

	CListWindow		mList;
	CListWindow		mYesNo;
	CPlaneWindow	mTitle;

	CSaveDataItem mSaveList[SAVE_DATA_NO+1];
	bool	mLoad;
	bool	mData[SAVE_DATA_NO+1];
	bool	mSuccess;
	int		mFileNo;
	int		mCursorNo;
public:
			CDiaryMenu(int param);
	virtual	~CDiaryMenu();
	
	virtual bool	OnTimer(void);
	virtual void	OnDraw(void);
	virtual void	Destroy(void);
private:
	void	InitYesNoWindow(int y);
	void	LoadSaveDataInfo(int no,int fno);
};

class CClosetMenu:public CMenuData{
public:
	static const int LIST_X_1 = 10;
	static const int LIST_X_2 = 310;
	static const int LIST_Y   = 20;

	static const int HELP_X_1 = -330;
	static const int HELP_X_2 = LIST_X_1-10;
	static const int HELP_X_3 = LIST_X_2+10;
	static const int HELP_Y_1 = 235;
	static const int HELP_Y_2 = 235;
	static const int HELP_Y_3 = 235;

protected:
	int		mCharacter;
	int		mSelect;
	int		mCategory;

	CSelectCharacterShopSystem	mSWindow;
	CItemListWindow				mItemList;
	CSelectWindow				mCategoryWindow;
	//CListWindow					mCategoryList;
	CListWindow					mClosetList;
	CItemHelpWindow				mHelpWindow;
	CCharaDataWindowEquip		mEquipWindow;				

public:
					CClosetMenu(int param);
	virtual			~CClosetMenu();
	
	virtual	bool	OnTimer(void);
	virtual	void	OnDraw(void);
			void	Update(void);
	virtual	void	Destroy(void);
private:
	void GetHelp(int item);
};

class CMonthStartMenu:public CMenuData{
public:
	static const int MENU_NO = 7;

protected:
	CListWindow	mList;
	int			mTempMapNo;

public:
					CMonthStartMenu(int param);
	virtual			~CMonthStartMenu();
	
	virtual	bool	OnTimer(void);
	virtual	void	OnDraw(void);
			void	Update(void);
	virtual	void	Destroy(void);
};
class CChangeMemberMenu:public CMenuData{
	CCharaDataWindowMenu	mCDataWindow[MAX_CHARACTER_NUMBER];
	int mReserve[MAX_CHARACTER_NUMBER+1];
	bool mChangeOk[MAX_CHARACTER_NUMBER+1];
	int mReserveNo;
	int	mChange;
	int mSelect;
	int	mCursor;

	bool mTop;
public:
					CChangeMemberMenu(int param);
	virtual			~CChangeMemberMenu();
	
	virtual	bool	OnTimer(void);
	virtual	void	OnDraw(void);
	virtual	void	Destroy(void);
private:
	void	InitCDWin(void);
	void	MoveCDWin(int cursor);
	void	MoveCursor(int add);
};

class CBattleStatusMenu:public CStatusMenu{
public:
	CBattleStatusMenu(int param);
	virtual	int		Init(int param);
	virtual void	DrawStatus(void);
};

class CConfigMenu:public CMenuData{
private:
	static const int MENU_NO	=7;
	static const int WIN_X		=40;
	static const int WIN_Y		=20;
	static const int WIN_W		=520;
	static const int HELP_H		=90;

	CSelectWindow	mSelectWindow[MENU_NO];
	CListWindow		mAll;
	CPlaneWindow	mHelp;

public:
			CConfigMenu(int param);
			~CConfigMenu();
	bool	OnTimer(void);
	void	OnDraw(void);
	void	Destroy(void);
private:
	void	UpData(void);
};

class CFreeSpaceMenu:public CMenuData{
protected:
	static const int Y =80;

	int		mCharacter;
	int		mFreeSpace;
	char*	mHelp;

	CListWindow	mLearnSkill;
	CDTTexture	mTex[MAX_PARTY_NUMBER];
	bool		mLearnCharacter[MAX_PARTY_NUMBER];

public:
					CFreeSpaceMenu(int param);
	virtual			~CFreeSpaceMenu();
	
	virtual	bool	OnTimer(void);
	virtual	void	OnDraw(void);
	virtual	void	Update(void);
	virtual	void	Destroy(void);
private:
	virtual void	MoveCursor(int no);
};
class CFSBirthStoneMenu:public CBirthStoneMenu{
	int	mFreeSpaceCharacter;
	int	mFreeSpace;
public:
	CFSBirthStoneMenu(int param);
	bool	OnTimer(void);
};
class CFSSelectSkillMenu:public CLearnSkillMenu{
protected:
	int	mFreeSpaceCharacter;
	int	mFreeSpace;

	CPlaneWindow mOldSkill;
public:
	CFSSelectSkillMenu(int param);
	
	virtual	bool	OnTimer(void);
	virtual	void	OnDraw(void);
	virtual	void	Destroy(void);
protected:
	virtual void	MoveCursor(int no);
	virtual void	Select(void);
	int	CheckSkill(void);
};
class CGlobalConfigMenu:public CMenuData{
private:
	static const int WIN_X		=40;
	static const int WIN_Y		=40;
	static const int WIN_W		=420;

	CSelectWindow	mSelectWindow;
	CListWindow		mAll;

public:
			CGlobalConfigMenu(int param);
			~CGlobalConfigMenu();
	bool	OnTimer(void);
	void	OnDraw(void);
	void	Destroy(void);
};
class CKeyConfigMenu:public CMenuData{
private:
	static const int WIN_X		=160;
	static const int WIN_Y		=80;
	static const int WIN_W		=420;

	CListWindow		mList;

public:
			CKeyConfigMenu(int param);
			~CKeyConfigMenu();
	bool	OnTimer(void);
	void	OnDraw(void);
	void	Destroy(void);
};
class CDInfoMenu:public CMenuData{
private:
	CListWindow		mList;
	CPlaneWindow	mWindow;

	int					mMonth;
	int					mOpenTresure;
	int					mKey[MAX_KEY_TYPE];
	CEnemyCharacter*	mEnemy;

public:
			CDInfoMenu(int param);
			~CDInfoMenu();
	bool	OnTimer(void);
	void	OnDraw(void);
	void	Destroy(void);
};


class CBypathMenu:public CMenuData{
	static const int MENU_NO = 2;
private:
	CListWindow		mList;
	CPlaneWindow	mTitle;

public:
			CBypathMenu(int param);
			~CBypathMenu();
	
	bool	OnTimer(void);
	void	OnDraw(void);
	void	Destroy(void);
};

class CStatusHelp:public CMenuData{
protected:
	int		mStatusNo;
	char*	mText;

	CPlaneWindow		mWindow;
public:
			CStatusHelp(int param);
			~CStatusHelp();

	bool	OnTimer(void);
	void	OnDraw(void);
	void	Destroy(void);
};

class CRemoveBirthStoneMenu:public CMenuData{
protected:
	CListWindow		mSWindow;
	CPlaneWindow	mWindow;

	int	mErrorTimer;
public:
			CRemoveBirthStoneMenu(int param);
	virtual	~CRemoveBirthStoneMenu();
	
	virtual bool	OnTimer(void);
	virtual void	OnDraw(void);
	virtual void	Destroy(void);
};

class CRemoveBSPersonal:public CMenuData{
private:
	static const int MONTH_LIST_X		= 340;
	static const int MONTH_LIST_Y		= 30;

	int		mSkill[MAX_SKILL_MONTH];
	int		mLearn[MAX_SKILL_MONTH];
	int		mCharacter;
	int		mMode;
	CListWindow				mList;
	CListWindow				mMonthList;
	CPlaneWindow			mSkillList;
	CCharaDataWindowMenu	mCDataWin;

public:
			CRemoveBSPersonal(int param);
			~CRemoveBSPersonal();

	void	InitMonthList(void);
	void	GetBSParam(int character,int* param);
	void	ResetMonthList(void);
	void	InitSkillList(void);

	bool	OnTimer(void);
	void	OnDraw(void);
	void	Destroy(void);
};
class CSelectDifficult:public CMenuData{
private:
	CListWindow		mList;
	CPlaneWindow	mTitle;
	CPlaneWindow	mHelp;

public:
			CSelectDifficult(int param);
			~CSelectDifficult();
	
	bool	OnTimer(void);
	void	OnDraw(void);
	void	Destroy(void);
};

class CExitYesNo:public CMenuData{
private:
	CListWindow		mList;
	CPlaneWindow	mTitle;

public:
			CExitYesNo(int param);
			~CExitYesNo();
	
	bool	OnTimer(void);
	void	OnDraw(void);
	void	Destroy(void);
};

class CSuccessSaveDataMenu:public CDiaryMenu{
private:
	CPlaneWindow	mHelp;
public:
			CSuccessSaveDataMenu(int param);
			~CSuccessSaveDataMenu();
	
	bool	OnTimer(void);
	void	OnDraw(void);
	void	Destroy(void);
};

class CCallPetHelp:public CSelectHelp{
protected:
	int		mSkillNo;
	int		mPetNo;
	char*	mText;
public:
			CCallPetHelp(int param);
			~CCallPetHelp();

	bool	OnTimer(void);
	void	OnDraw(void);
	void	Destroy(void);
private:
	void	DrawParam(const char* name,int param,int y);
};
class CTutorialMenu:public CMenuData{
protected:
	CDTTexture		mTex;
	CPlaneWindow	mWindow;
	int		mNo;
	int		mMaxPage;
	int		mPage;

	char	mTitle[64];
	char	mSubTitle[64];
	char	mText[1024];
public:
			CTutorialMenu(int param);
			~CTutorialMenu();

	bool	OnTimer(void);
	void	OnDraw(void);
	void	Destroy(void);
private:
	void	ChangePage(int page);
};
class CSoundTestMenu:public CMenuData{
protected:
	CPlaneWindow	mCurrent;
	CListWindow		mBGMWindow;
public:
			CSoundTestMenu(int param);
			~CSoundTestMenu();

	bool	OnTimer(void);
	void	OnDraw(void);
	void	Destroy(void);
};
class CPreciousItemMenu:public CMenuData{
protected:
	CItemHelpWindow		mHelpWindow;
	CSelectWindow		mCategoryWindow;
	CListWindow			mClosetList;

	int mCategory;
	int mCategoryAll;
public:
			CPreciousItemMenu(int param);
			~CPreciousItemMenu();

	bool	OnTimer(void);
	void	OnDraw(void);
	void	Destroy(void);

	void	ChangeHelp(void);
};
///////////////////////////////////////////////////////////////////////////////////
class CBoxOpenMenu:public CMenuData{
	static const int START_TIME=30;
	static const int MOVE_CHARACTER_TIME=20;
	static const int OPEN_TIME=START_TIME+MOVE_CHARACTER_TIME+40;

	static const int BOX_X	=	240;
	static const int BOX_Y	=	80;

	int		mTimer;
	int		mBox;
	int		mCharacter;
	bool	mTrap;
	bool	mEnd;
	double	mExp;
	int		mLvUp;

	int		mX;
	int		mY;
	int		mBoxX;
	int		mBoxY;
	int		mAni;
	int		mTrapParamA;
	int		mTrapParamB;

	CPlaneWindow mWindow;

	CDTTexture	mBoxTex;
	CDTTexture	mCharacterTex;
	CDTTexture	mTrapTex;
public:
			CBoxOpenMenu(int param);
			~CBoxOpenMenu();
	
	bool	OnTimer(void);
	void	OnDraw(void);
	void	Destroy(void);

private:
	void	TrapInit_0(int pow);
	bool	TrapTimer_0(void);
	void	TrapDraw_0(int bow);

	void	TrapInit_2(int pow);
	bool	TrapTimer_2(void);
	void	TrapDraw_2(int size);

	void	TrapInit_4(void);
	bool	TrapTimer_4(void);
	void	TrapDraw_4(void);

	void	TrapInit_5(void);
	bool	TrapTimer_5(void);
	void	TrapDraw_5(void);

	void	TrapInit_6(int type);
	bool	TrapTimer_6(void);
	void	TrapDraw_6(D3DCOLOR color);

	void	TrapInit_8(void);
	bool	TrapTimer_8(void);
	void	TrapDraw_8(void);

	void	TrapInit_9(void);
	bool	TrapTimer_9(void);
	void	TrapDraw_9(void);

	void	TrapInit_10(int div);
	bool	TrapTimer_10(void);
	void	TrapDraw_10(void);

};