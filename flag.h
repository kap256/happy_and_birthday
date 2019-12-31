#pragma once
#include "characterdata.h"
//#include "itemdata.h"
#include "treasure.h"
#include "mapconst.h"
#include "savecursor.h"
#include "enemyopendata.h"
#include "enemydata.h"
#include "main.h"

const int VILLAGE_SHOP_MAX		=6;
const int FLAG_MAX				=128;
const int TEMP_ITEM_MAX			=10;
const int FLEA_MARKET_ITEM_MAX	=8;
const int MAX_KEY				=7;	//カギじゃないよ！キーコンフィグだよ！

class CGlobalFlag{
public:
	bool	mFullScreen;
	int		mLastSave;
	int		mKeyConfig[MAX_KEY];
	bool	mSoundTest;
	bool	mSound[BGM_MAX];
public:
	CGlobalFlag(void);
	void Save(void);
	void Load(void);
private:
	void Update(void);
};
class CFlag{
public:
	CSaveCursor		mSaveCursor;
	CEnemyOpenData	mEnemyOpenData[ENEMY_NO];
	bool			mBiology;

	bool		mChangeOptionTitle;

	bool		mSE;
	bool		mBGM;
	int			mUseSaveCursor;
	bool		mMapMoveTurn;
	bool		mDropUsedItem;
	bool		mAutoOrderItem;
	bool		mBattleSpeed;
	bool		mAutoDash;

	int			mBGMNo;
	int			mMapBGMNo;
	int			mBGMWait;

	int			mPlayTime;//
	int			mHardMode;//
	int			mClearData;
	int			mMonth;//
	int			mCurrentMonth;
	int			mOldMonth;
	int			mFlag[FLAG_MAX];/////////
	bool		mTreasure[TREASURE_NO];////////////

	CPlayerData	mPlayerData[MAX_CHARACTER_NUMBER];//
	bool		mFriend[MAX_CHARACTER_NUMBER];//
	bool		mEggFriend[MAX_CHARACTER_NUMBER];
	int			mParty[MAX_CHARACTER_NUMBER+MAX_PARTY_NUMBER];//
	int			mFormationX[MAX_PARTY_NUMBER];//
	int			mFormationY[MAX_PARTY_NUMBER];//

	int			mMoney;//
	int			mMarket[MAX_ITEM_NUMBER];//
	bool		mMarketNew[MAX_ITEM_NUMBER];//
	int			mFleaMarketList[FLEA_MARKET_ITEM_MAX];
	bool		mFleaMarketNew[FLEA_MARKET_ITEM_MAX];
	int			mCloset[MAX_ITEM_NUMBER];//
	int			mFarm[MAX_SEED_NUMBER];
	int			mUsedFarm[MAX_SEED_NUMBER];
	CarryItem	mTempItem[TEMP_ITEM_MAX];
	bool		mClosetUse;

	int			mBypathMapNo;
	int			mBypathX;
	int			mBypathY;
	int			mBypathZ;
	int			mBypathDir;
	int			mBypathBGM;

	char		mMapName[256];
	int			mEnemyType[MAX_ENEMY_TYPE];
	int			mEnemyTypeNo;

	bool		mShopNew[5];
	int			mShopOK[VILLAGE_SHOP_MAX];

public:
	void	Reset(void);
	void	NewGame(void);
	void	NewMonth(int month);

	void	Market(void);
	void	GetShopItem(int shop,bool item[MAX_ITEM_NUMBER]);
	void	SetMarket(int* item);

	int		GetAllItem(int item,bool spend=false,int closet=1);
	int		GetUsedFarmItem(int item);
	void	SpendAllItem(int item,bool closet=false);

	int		SetTempItem(int item);
	int		GetTempItem(int no);
	void	DropTempItem(bool closet=true);

	void	SetCloset(int item,int no);
	void	SetCloset(int item){SetCloset(item,1);};
	void	SetCloset(CarryItem item);
	void	SetCloset(CarryItem item,CPlayerData* pdata);
	int		GetCloset(int item,bool one=false);

	int		GetFarm(int no);

	bool	SearchFSkill(int no,bool reserve=false);
	int		GetBusinessAbility(void);
	int		CheckLearnSkill(int skill);
	void	CheckFreeSpace(void);
	void	CheckRareItem(void);
	int		GetDInfo(bool reserve);
	bool	GetDInfo(int month,bool reserve);

	void	AllHeal(void);
	
	void	CheckReserve(void);
	void	AddMember(int character);
	void	ChangeMember(int no,int character);
	int		SearchMember(int character,int max=MAX_CHARACTER_NUMBER+MAX_PARTY_NUMBER);

	void	Save(int no);
	bool	Load(int no);
	bool	Load_0(FILE* pFile,int ver);
	bool	Load_3(FILE* pFile,int ver);
	bool	Load_6(FILE* pFile,int ver);
	bool	Load_7(FILE* pFile,int ver);
	bool	Load_9(FILE* pFile,int ver);

	bool	LoadSuccessData(int no);
	void	LoadSuccessData_0(FILE* pFile,int ver);
	void	LoadSuccessData_9(FILE* pFile,int ver);
};

extern CGlobalFlag GlobalFlag;
extern CFlag Flag;