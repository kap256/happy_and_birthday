#pragma once

//#include "main.h"
#include "mapobject.h"
#include "mapmessage.h"
#include "eventdata.h"
#include "number.h"
#include "mapconst.h"
#include "option.h"

const int MAX_GOTO	=4;
const int ESKIP_BLACK_OUT =32;

#ifdef 	_DRAW_DATA_FRIEND_
class _DRAW_DATA_FRIEND_ ;
#endif
extern double MAP_CHIP_SIZE;
class CMapMode {
	friend			class	CNumber;
#ifdef _DEBUG
	friend			class	CStartMenu;
	friend			void DebugOutput();
#endif

#ifdef 	_DRAW_DATA_FRIEND_
	friend			class	_DRAW_DATA_FRIEND_;
#endif

private:
	int				mMapNo;
	int				mBGColor;
	CDTTexture		mBGImg;
	int				mMapData[MAP_SIZE_X][MAP_SIZE_Y][MAP_SIZE_Z];
	int				mMapSizeX;
	int				mMapSizeY;

	double			mCameraX;
	double			mCameraY;
	double			mCameraSpeed;
	int				mCameraGoalX;
	int				mCameraGoalY;

	D3DCOLOR		mMapColor;

	int				mMapFlag[MAX_MAP_FLAG];

	CDTTexture		mMapChipImg[MAP_CHIP_NO-1];
	CPlaneWindow	mWaitWindow;
	CPlaneWindow	mSkipWindow;

	CMapObject*		mObject[OBJECT_NO];
	CMapMessage		mMessage[MAP_MESSAGE_NO];
	CMapMiniMessage mMiniMessage;

	CMapSelectWindow mSelectWindow;
	CEvent*			 mSelectEvent;
	int				 mSelectDefault; 

	int				mTime;
	int				mNoWalkTimer;
	bool			mNoTalk;
	int				mDecideKeyTimer;
	int				mCancelKeyTimer;
	int				mMapNameTimer;
	int				mMapNameSize;
	int				mMonthTitleTimer;

	int				mEventID;
	int				mEventReturn;
	CEvent			mEvent;
	CEvent*			mRuningEvent;
	CEventHeader	mEventHeader;
	CEventData		mEventData;
	CEventData		mEncountEvent;
	CEventData		mBoxEvent;

	CEventData*		mGotoData[MAX_GOTO];
	int				mGotoLine[MAX_GOTO];
	int				mGotoCurrent;

	bool			mHawkEye;
	int				mStop;
	bool			mTrapReverse;
	int				mTrapStop;
	int				mEnemyLvUp;
	unsigned int	mEventSkip;

	int				mBGMWait;
	CDTTexture		mImage;
	bool			mShowImage;
public:

#ifdef _SHOW_OBJECT_IMG_NO_
	bool mDebugShowImgNo;
#endif

	CMapMode(void);
	~CMapMode(void);
	void AfterStartUp(void);
	void LoadMap(int mapno,int x,int y,int z,int dir);
	bool LoadMapData(char* fname);

	void OnTimer(void);
	void OnDraw(void);
	void DrawMap(int camerax,int cameray);

	int SendMessage(int event,int parama,int paramb);

	void DoEventSkip(void);
	bool GetEventSkip(void){return (mEventSkip>=0x80000000+ESKIP_BLACK_OUT);};

	//ワナ関連
	void SetTrapSlow(bool slow){mObject[0]->SetBaseSpeed(int(mObject[0]->GetBaseSpeed()*(slow?2:0.5)));};
	void SetTrapReverse(bool rev){mTrapReverse=rev;};
	void SetTrapStop(int stop){mTrapStop=stop;};

	void SetDamage(CMapObject* object,int dmg);

	int  GetChip(int x,int y,int z);
	int  GetChipType(int x,int y,int z);
	int	 GetMapHeight(int x,int y,int z);
	void GetMapSize(int* x,int* y){if(!x||!y)return; *x=mMapSizeX; *y=mMapSizeY;};

	bool BGMWait();
	void SetBGMWait(int wait){mBGMWait=wait;};
private:

	void DrawMapTitle();
	void DrawMonthTitle();
#ifdef _DEBUG
	void DrawDebug();
#endif

	CEvent* NewLine(int event,int parama,int paramb,CEventData* current=NULL,bool check=true);

	CMapObject*	CreateObject(int objno,int x,int y,int z,int dir,int speed,int movefreq,
							int img,bool hit,int gravity,int showno);

	CMapObject*	CreatePlayerObject(int objno,int x,int y,int z,int dir);

	CMapObject*	CreateAAObject(int objno,int x,int y,int z,int dir,int speed,int movefreq,
									int img,int koma,int anispd,bool hit,int gravity,int showno);

	CMapObject*	CreateHighObject(int objno,int x,int y,int z,int dir,int speed,int movefreq,
								int img,bool hit,int gravity,int showno);
	CMapObject*	CreateWoodObject(int objno,int x,int y,int z,int dir,int speed,int movefreq,
								int img,bool hit,int gravity,int showno);
	CMapObject*	CreateHighAniObject(int objno,int x,int y,int z,int dir,int speed,int movefreq,
								int img,bool hit,int gravity,int showno);
	CMapObject*	CreateAniObject(int objno,int x,int y,int z,int dir,int speed,int movefreq,
								int img,bool hit,int gravity,int showno);
	CMapObject*	CreateBigObject(int objno,int x,int y,int z,int dir,int speed,int movefreq,
								int img,bool hit,int gravity,int showno);
	CMapObject*	CreateBigAniObject(int objno,int x,int y,int z,int dir,int speed,int movefreq,
								int img,bool hit,int gravity,int showno);
	CMapObject*	CreatePutObject(int objno,int x,int y,int z,int speed,int img,int showno);
	CMapObject*	CreateLiftObject(int objno,int x,int y,int z,int dir,int speed,int movefreq,int img,int showno);

	CMapObject*	CreateNTObject(int objno,int x,int y,int z,int img,int showno,
									int cycle,int timer,int pow,int se);
	CMapObject*	CreateStoneObject(int objno,int x,int y,int z,int dir);

	CMapObject*	CreateEnemyObject(int objno,int x,int y,int z,int dir,int speed,int movefreq,
									int img,bool hit,int gravity,int showno,
									int movemode,int search,int reader,int lv,int member,int field,int option);
	CMapObject*	CreateEnemyObject(int objno,int x,int y,int z,int dir,int speed,int movefreq,
									int img,bool hit,int gravity,int showno,
									int movemode,int search,int party);

	CMapObject*	CreateBoxObject(int objno,int x,int y,int z,int dir,int box);
	CMapObject*	CreateSavePoint(int objno,int x,int y,int z);

	bool		DeleteObject(int no);

	void		SetCamera(int x,int y,bool move);
	void		WaitBreak(bool force);


	CEventData* GetEventData(int line);
	void		KillEvent(CEvent *event,CEvent *old);
	void		KillEvent(int id);
	void		GoToLine(int line,CEvent *event);

	void		Sleep(CEvent *event,CEvent *old);
	void		RunAll(void);
	int			Run(CEvent *event,CEvent *old);
//////////ここから個別イベント///////
	void SetMessage(CEvent *event);
	void DeleteMessage(CEvent *event);
	void DeleteMessageQuick(CEvent *event);

	void SetObject(CEvent *event,int type);
	void SetLiftObject(CEvent *event);
	void SetPutObject(CEvent *event);
	void SetAAObject(CEvent *event);
	void SetNormalTrap(CEvent *event);
	void SetEnemy(CEvent *event);
	void SetEnemyLocked(CEvent *event);
	void SetBox(CEvent *event);
	void SetSavePoint(CEvent *event);
	void DeleteObject(CEvent *event);

	void SetObjPos(CEvent *event);
	void SetObjGoal(CEvent *event);
	void SetObjGoalDir(CEvent *event);
	void SetObjDir(CEvent *event);
	void SetObjImg(CEvent *event);
	void SetObjColor(CEvent *event);
	void SetObjSpeed(CEvent *event);
	void SetObjGrav(CEvent *event);
	void SetObjParam(CEvent *event);
	void SetObjView(CEvent *event);
	void SetObjZSpeed(CEvent *event);

	bool Wait(CEvent *event);
	void Stop(CEvent *event);
	void SetCamera(CEvent *event);
	void SetCameraSpeed(CEvent *event);
	void PlayWaveEvent(CEvent *event);
	void PlayBGMEvent(CEvent *event);
	void SetFadeEvent(CEvent *event);
	void SetFadeOutEvent(CEvent *event);
	void MonthTitle(CEvent *event);
	void ChangeMapTitle(CEvent *event);
	void MapColor(CEvent *event);
	void BGColor(CEvent *event);
	void SetMapChip(CEvent *event);
	void GetMapChip(CEvent *event);
	void ChangeAllChip(CEvent *event);

	void Store(CEvent *event);
	void Goto(CEvent *event);
	void NewLine(CEvent *event);
	bool Kill(CEvent *event);
	void If(CEvent *event);
	void IfBox(CEvent *event);
	void IfBox3D(CEvent *event);
	void Select(CEvent *event,int no);


	void StartEventSkip(CEvent *event);
public:
	void EndEventSkip(void);
private:
	void ReturnVillageEvent(CEvent *event);
	void OpenMenu(CEvent *event);
	void Shop(CEvent *event);
	void ShopMan(CEvent *event);
	void Tutorial(CEvent *event);
	void FleaMarket(CEvent *event);
	void Farm(CEvent *event);
	void Seed(CEvent *event);
	void Sell(CEvent *event);
	bool NextMonth(CEvent *event);
	void SetFriend(CEvent *event);
	void SetEggFriend(CEvent *event);
	void SetParty(CEvent *event);
	void LeaveParty(CEvent *event);
	void ChangeMap(CEvent *event);
	void Battle(CEvent *event);
	void AddEnemyLevel(CEvent *event);
	void GetItem(CEvent *event);
	void SpendItem(CEvent *event);
	void AddMarket(CEvent *event);
	void HealAll(CEvent *event);

	void KnockBack(CEvent *event);
	void ShowImage(CEvent *event);

	void Ending(CEvent *event);
	void TrueEnd(CEvent *event);

	void Encount(CEvent *event);
	void CheckBattle(CEvent *event);
	void OpenBox(CEvent *event);
};
