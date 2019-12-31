#pragma once

#include "main.h"
#include "mapconst.h"
#include "option.h"

enum EMapObjType{
	MapObjNormal,
	MapObjPlayer,
	MapObjEnemy,
};

class CMapObject{
public:
	static	void	GetComp(int dir,int* x,int* y);

protected:
	int				mID;
	int				mSpeed;
	int				mMoveFreq;
	int				mShowNo;
	int				mGravity;
	bool			mHit;
	bool			mView;
	CDTTexture		mImg;
	int				mParam;

	double			mX;
	double			mY;
	double			mZ;
	int				mDir;
	int				mNextDir;

	double			mSpeedZ;
	bool			mGround;

	int				mFloatTimer;
	int				mSki;
	int				mSkiStartX;
	int				mSkiStartY;
	int				mSkiStartZ;

	int				mGoalX;
	int				mGoalY;
	int				mGoalZ;
	int				mWalkTimer;
	int				mWalkWait;

	int				mAniNo;
	bool			mChangeAni;
	bool			mStop;

#ifdef _SHOW_OBJECT_IMG_NO_
	int		mDebugImageNo;
#endif
public:
#ifdef _DEBUG
	virtual void	DrawDebugData(int x,int y);
#endif

	CMapObject(void);
	~CMapObject(void);
	virtual	void			InitObject(int id,int x,int y,int z,int dir,int speed,int movefreq,
										int img,bool hit,int gravity,int param);
			void			LoadImage(int no);
	virtual int				OnTimer(int mapsizex,int mapsizey,CMapMode *map,CMapObject* object[OBJECT_NO]);
	virtual	void			OnDraw(int cx,int cy);
	virtual void			DrawShadow(int cx,int cy,int z);

	virtual	unsigned int	GetDrawZ(void);
			unsigned int	GetShadowZ(CMapMode *mmode);

			int GetX_32(void);
			int GetY_32(void);
			int GetZ_32(void);

			void	SetPos(int x,int y,int z);
	virtual	void	FixPos(void){mX=int((mX+15)/32)*32;mY=int((mY+15)/32)*32;};
			bool	Move(double x,double y);
	virtual void	SetDir(int dir);
			void	SetSpeed(int speed);
			void	SetGrav(int grav){mGravity=grav;};
			void	SetParam(int param){mParam=param;};
			void	SetView(bool view){mView=view;};
			void	SetZSpeed(double zspeed);
			int		SetGoal(int x,int y,int z);
			int		SetGoalDir(int dir,int l);
			void	SetStop(bool stop);
			void	GoToGoal(){SetPos(mGoalX,mGoalY,mGoalZ);};


			bool	CheckGoal(void);
	virtual	int		CheckHit(CMapObject* obj,bool walk);
	virtual	bool	CheckTalk(CMapObject* obj);

	int GetID(void)			{return mID;};
	int GetX(void)			{return (int)mX;};	
	int GetY(void)			{return (int)mY;};	
	int GetZ(void)			{return (int)mZ;};
	virtual int GetDir(void)		{return mDir;};
	int GetSpeed(void)		{return mSpeed;}
	int GetWalkTimer(void)	{return mWalkTimer;}
	virtual	int			GetMaxDir(void)		{return 4;};
	virtual	EMapObjType	GetObjType(void)	{return MapObjNormal;};


	virtual	void	SetDash(bool dash)			{return;};
	virtual	void	SetEscape(CMapObject* obj)	{return;};
	virtual	bool	SetAction(int type)			{return false;};
	virtual	int		GetAction()					{return 0;};
	virtual	int		GetBaseSpeed()				{return 0;};
	virtual	void	SetBaseSpeed(int speed)		{return;};

	virtual	void GetDrawInfo(int &x,int &y,int &tx,int &ty,int &turn);
	virtual void GetDrawXY(int &drawx,int &drawy);
};


class CPlayerObject:public CMapObject{
	static const int ESCAPE_TIME = 20;

	bool	mDash;
	int		mEscape;
	int		mActionTime;
	int		mActionType;
	int		mBaseSpeed;
public:
	CPlayerObject(void);
	int		OnTimer(int mapsizex,int mapsizey,CMapMode *map,CMapObject* object[OBJECT_NO]);
	void	OnDraw(int cx,int cy);
	void	GetDrawInfo(int &x,int &y,int &tx,int &ty,int &turn);

	void	SetDash(bool dash);
	void	SetEscape(CMapObject* obj);

	
	int		GetBaseSpeed(){return mBaseSpeed;};
	void	SetBaseSpeed(int speed);

	bool	SetAction(int type)	;
	int		GetAction();
	int		CheckHit(CMapObject* obj,bool walk);
	EMapObjType	GetObjType(void)	{return MapObjPlayer;};
};

class CBoxObject:public CMapObject{
	int		mBoxNo;

public:
	void	InitBoxObject(int id,int x,int y,int z,int dir,int no);

	void	OnDraw(int cx,int cy);
	bool	CheckTalk(CMapObject* obj);
#ifdef _DEBUG
	virtual void	DrawDebugData(int x,int y);
#endif
};

class CHighObject:public CMapObject{
public:
			void	OnDraw(int cx,int cy);
	virtual int		CheckHit(CMapObject* obj,bool walk);
};
class CWoodObject:public CHighObject{
	int		mDeleteTimer;
public:
	CWoodObject();
	int		OnTimer(int mapsizex,int mapsizey,CMapMode *map,CMapObject* object[OBJECT_NO]);
	void	GetDrawInfo(int &x,int &y,int &tx,int &ty,int &turn);
	int		CheckHit(CMapObject* obj,bool walk);
private:
	void	Cut(CMapObject* obj);
};
class CHighAniObject:public CHighObject{
	int		mAnimation;
public:
	void	InitObject(int id,int x,int y,int z,int dir,int speed,int movefreq,
										int img,bool hit,int gravity,int param);
	void	GetDrawInfo(int &x,int &y,int &tx,int &ty,int &turn);
	void	SetDir(int dir);
	int		GetDir(void)		{return mAnimation;};
};
class CAniObject:public CMapObject{
	int mAnimation;
public:
	void	InitObject(int id,int x,int y,int z,int dir,int speed,int movefreq,
										int img,bool hit,int gravity,int param);

	virtual	void OnDraw(int cx,int cy){CMapObject::OnDraw(cx,cy);};
	void	GetDrawInfo(int &x,int &y,int &tx,int &ty,int &turn);
	void	SetDir(int dir);
	int		GetDir(void)		{return mAnimation;};
};
class CSavePoint:public CAniObject{
	static const int STAR_NO = 4;
	int		mTimer;
	int		mXRad[STAR_NO];
	int		mYRad[STAR_NO];
	int		mYSpeed[STAR_NO];
	int		mColor[STAR_NO];

public:
	void	InitSavePoint(int id,int x,int y,int z);

    int		OnTimer(int mapsizex,int mapsizey,CMapMode *map,CMapObject* object[OBJECT_NO]);
	void	OnDraw(int cx,int cy);
	bool	CheckTalk(CMapObject* obj);
private:
	void		DrawStar(int x,int y,bool top);
	D3DCOLOR	GetColor(int t);
};
class CBigObject:public CMapObject{
public:
			void	OnDraw(int cx,int cy);
			void	DrawShadow(int cx,int cy,int z);
	virtual int		CheckHit(CMapObject* obj,bool walk);
};
class CBigAniObject:public CBigObject{
public:
	void	GetDrawInfo(int &x,int &y,int &tx,int &ty,int &turn);
};
class CAutoAniObject:public CAniObject{
	int	mAutoAniTimer;
	int	mKoma;
	int	mAniSpeed;

public:
	void	InitAutoAniObject(int id,int x,int y,int z,int dir,int speed,int movefreq
							,int img,int koma,int anispd,bool hit,int gravity,int param);
	int		OnTimer(int mapsizex,int mapsizey,CMapMode *map,CMapObject* object[OBJECT_NO]);
};
class CLiftObject:public CMapObject{
	double mMoveX;
	double mMoveY;
public:
	CLiftObject();
//	void	GetDrawInfo(int &x,int &y,int &tx,int &ty,int &turn);
	virtual int		CheckHit(CMapObject* obj,bool walk);
			int		CheckHit(CMapObject* obj,bool walk,int bottomhit);
	virtual int		OnTimer(int mapsizex,int mapsizey,CMapMode *map,CMapObject* object[OBJECT_NO]);
	virtual unsigned int	GetDrawZ(void);
};

class CPutObject:public CLiftObject{
	int		mHeavy;
public:
	CPutObject();
	void	GetDrawInfo(int &x,int &y,int &tx,int &ty,int &turn);
	int		CheckHit(CMapObject* obj,bool walk);
	int		OnTimer(int mapsizex,int mapsizey,CMapMode *map,CMapObject* object[OBJECT_NO]);
	unsigned int	GetDrawZ(void){return CMapObject::GetDrawZ();}

//	void	FixPos(void){if(mHeavy>0)	return;CMapObject::FixPos();};
};


class CNormalTrapObject:public CAniObject{
	int mCycle;
	int mPow;
	int mSE;

	int mTimer;
	bool mDmg;
public:
	void	InitNTObject(int objno,int x,int y,int z,int img,int showno,
									int cycle,int timer,int pow,int se);
	int		CheckHit(CMapObject* obj,bool walk);
	int		OnTimer(int mapsizex,int mapsizey,CMapMode *map,CMapObject* object[OBJECT_NO]);
};

class CStoneObject:public CMapObject{
public:
	void	InitStoneObject(int objno,int x,int y,int z,int dir);
	int		OnTimer(int mapsizex,int mapsizey,CMapMode *map,CMapObject* object[OBJECT_NO]);
};

class CEnemyObject:public CMapObject{
	int		mMoveMode;
	int		mSearch;

	int		mEncountTimer;
	int		mStartX;
	int		mStartY;

	int		mParamA;
	int		mParamB;

public:
	int		mParty;
	int		mReader;
	int		mLv;
	int		mMember;
	int		mField;
	int		mOption;

public:
	void	InitEnemyObject(int id,int x,int y,int z,int dir,int speed,int movefreq,
						int img,bool hit,int gravity,int showno,
						int movemode,int search,int reader,int lv,int member,int field,int option);

	void	InitEnemyObject(int id,int x,int y,int z,int dir,int speed,int movefreq,
						int img,bool hit,int gravity,int showno,
						int movemode,int search,int party);

	int		OnTimer(int mapsizex,int mapsizey,CMapMode *map,CMapObject* object[OBJECT_NO]);
	void	OnDraw(int cx,int cy);
	int		CheckHit(CMapObject* obj,bool walk);

	EMapObjType	GetObjType(void)	{return MapObjEnemy;};

private:
///////õ“G•ûj/////////
	void	Search_0(CMapObject* object[OBJECT_NO]);
		//0:’ÇÕŒ^
	void	Search_1(CMapObject* object[OBJECT_NO]);
		//1:‚“xŒÀ’è’ÇÕŒ^
	void	Search_2(CMapObject* object[OBJECT_NO]);
		//2:“¦‘–Œ^
	void	Search_3(int dir);
		//3:‰ñ“]Œ^(Œv‰ñ‚è)
		//4:‰ñ“]Œ^(”½Œv‰ñ‚è)
	void	Search_5(CMapObject* object[OBJECT_NO]);
		//5:–å”ÔŒ^(‚wÀ•W‚É•½sˆÚ“®)
	void	Search_6(CMapObject* object[OBJECT_NO]);
		//6:–å”ÔŒ^(‚xÀ•W‚É•½sˆÚ“®)
	void	Search_7(CMapMode *map);
		//7:—ÎŒ^
	void	Search_8(int dir);
		//8:ƒTƒCƒYŒÅ’è‰ñ“]Œ^(Œv‰ñ‚è)
		//9:ƒTƒCƒYŒÅ’è‰ñ“]Œ^(”½Œv‰ñ‚è)
		//10:ƒTƒCƒYŒÅ’è‰•œŒ^
		//11:‰•œŒ^

	void	Search_100(void){return ;};
		//100:‚È‚É‚à‚µ‚È‚¢

};

