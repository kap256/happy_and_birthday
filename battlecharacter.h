#pragma once

#include "main.h"
#include "battleconst.h"
#include "characterdata.h"
#include "flag.h"
#include "enemydata.h"
#include "animationdata.h"
#include "enemyopendata.h"

const int DAMAGE_TIMER	=	16;
const int AVOID_TIMER	=	8;
const int AVOID_X		=	20;
const int AVOID_Y		=	0;
const int AVOID_Z		=	5;

const int HIT_SIZE		=	24;
const int STATUS_NO		=	8;

class CBattleCharacter:public CElement{
public:
	static CBattleCharacter* mHelpTarget;
protected:
	bool			mLive;
	int				mNo;
	CDTTexture		mImg;
	D3DCOLOR		mColor;

	int				mTimer;
	int				mAni;
	int				mAniTimer;
	double			mAirCycle;
	bool			mTurn;
	bool			mJump;

	int				mActionCount;
	int				mAction;
	int				mTarget;

	double			mX;
	double			mY;
	double			mZ;
	int				mSX;
	int				mSY;
	int				mGoalX;
	int				mGoalY;
	
	double			mSpeedX;
	double			mSpeedY;
	double			mSpeedZ;
	int				mMoveTime;

	int				mDamageTimer;
	bool			mCrtFlash;
	int				mAvoidTimer;

	CCharacterData*	mCData;
	int				mHP;

	int				mAtkUp;
	int				mDefUp;
	int				mMagAtkUp;
	int				mMagDefUp;
	int				mSpdUp;

	int				mStatus[STATUS_NO];
	int				mStatusParam[STATUS_NO];
	bool			mCharm;
	bool			mBerserk;

public:
	CBattleCharacter();
	virtual			~CBattleCharacter();
	
	virtual void	Draw(CPlaneWindow* window,int x,int y,int param,D3DCOLOR color=0xffffffff) const;
	virtual void	Help(int param)const;
			void	SetID(int id){mID=id;};

	virtual void	LoadCharacter(int no,int x,int y,int param);
			void	DeleteCharacter(void){mLive=false;};
	virtual void	LoadImage(int no);
	virtual	void	OnTimer(int mode,int timer);
	virtual	void	OnDraw(D3DCOLOR color=0,bool weapon=true);
	virtual	void	OnDraw(int x,int y,D3DCOLOR color=0xffffffff,bool weapon=true);
	virtual	int		GetDeathBlack(){return 0;};
	virtual void	SetDeathBlack(int db){};
			void	DrawStatus(D3DCOLOR color);
	virtual void	DrawShadow(void);
			bool	TurnEnd(CBattleMode *bm);
			bool	TurnStart(void);

	virtual	void	Move(int x,int y,int time,bool move=true);
	virtual	void	Return(int time,bool jump,bool force=false);
			void	GetSquareTarget(int &x,int &y,int size);
	virtual	void	SetGoalSquare(int x,int y,int time,int size,bool jump,bool force=false);
	virtual	void	SetGoal(int x,int y,int time,bool jump,bool force=false);
	virtual	void	SetPos(int x,int y,int z);
	virtual	void	SetPosSquare(int x,int y,int size);
			void	SetSpeedZ(double z);
			bool	ChangeAni(int ani,int time,bool force);
			void	SetTurn(bool turn){mTurn=turn;};

			bool	SpendSP(int lv){return mCData->LostSP(lv,1);};
	virtual	bool	SpendUseItem(){return true;};
	virtual	int		GetUseItem(){return 0;};
	virtual	void	GetUseItemParam(int* parama,int* paramb){if(parama)	*parama=0;if(paramb)	*paramb=0;};
			void	Death(int option);
			int		SetDamage(int dmg,bool crt,int option);
			void	SetDamageEffect(bool crt);
			void	SetAvoid(void);
			void	SetHP(int hp){mHP=hp;};
	virtual int		HealHP(int heal);
	virtual int		HealLP(int heal);
	virtual int		LostLP(int pow);
	virtual int		HealSP(int lv,int heal);
	virtual bool	LostSP(int lv,int pow){return mCData->LostSP(lv,pow);};;
	virtual bool	SetStatus(int st,int param);
			bool	ClearStatus(int st);
			int		SearchStatus(int st);
			int		GetStatus(int no){return mStatus[no];};
			int		GetStatusAll(int *st,int* param=NULL);
			int		GetDrawStatus(void);
			void	ClearAllStatus(void);
			bool	ClearBadStatus(void);
			bool	ClearGoodStatus(void);

			bool	CheckLPOver(void);

			bool	GetLive(){return mLive;};
			bool	GetActive();
			bool	GetDead();
			int		GetAniTimer(){return mAniTimer;};
			int		GetSX(){return mSX;};
			int		GetSY(){return mSY;};
			int		GetX(){return (int)mX;};
			int		GetY(){return (int)mY;};
			int		GetZ(){return (int)mZ;};
			int		GetCX(){return (int)mX+GetSize()*32+32;}
			int		GetCY(){return GetGCY()-(int)mZ;};
			int		GetGCY(){return (int)mY+GetSize()*32+32;}
			int		GetBaseCX(){return mSX*BATTLE_SQUARE_SIZE+GetSize()*32+32;}
			int		GetBaseCY(){return GetBaseGCY()-(int)mZ;};
			int		GetBaseGCY(){return  mSY*BATTLE_SQUARE_SIZE+GetSize()*32+32;}

			CCharacterData*	GetCData()		{return mCData;};	
	virtual	const char*		GetName()const	{if(!mCData)	return NULL;return mCData->GetName();};	
	virtual	int				GetNo()const	{return mNo;};
			int				GetHP()			{return mHP;};
			int				GetSP(int no)	{if(!mCData)	return 0;return mCData->GetSP(no);};

			int		GetLv()const {if(!mCData)	return 0;	return mCData->GetLv();};
			int		GetExp(){if(!mCData)	return 0;	return mCData->GetExp();};	

			int		GetMaxHP();	
			int		GetMaxLP(){if(!mCData)	return 0;	return mCData->GetMaxLP();};
			int		GetMaxSP(int no);
			int		GetAtk();
			int		GetDef();
			int		GetMagAtk();
			int		GetMagDef();
			int		GetSpeed();
			int		GetTech();

			int		GetBaseAtk()	{if(!mCData)	return 0;return mCData->GetAtk();};
			int		GetBaseDef()	{if(!mCData)	return 0;return mCData->GetDef();};
			int		GetBaseMagAtk()	{if(!mCData)	return 0;return mCData->GetMagAtk();};
			int		GetBaseMagDef()	{if(!mCData)	return 0;return mCData->GetMagDef();};
			int		GetBaseSpeed()	{if(!mCData)	return 0;return mCData->GetSpeed();};
			int		GetBaseTech()	{if(!mCData)	return 0;return mCData->GetTech();};

	virtual int		SetParamUp(int *old,int up);
			int		SetAtkUp(int up)	{return SetParamUp(&mAtkUp,up);};
			int		SetDefUp(int up)	{return SetParamUp(&mDefUp,up);};
			int		SetMagAtkUp(int up)	{return SetParamUp(&mMagAtkUp,up);};
			int		SetMagDefUp(int up)	{return SetParamUp(&mMagDefUp,up);};
			int		SetSpdUp(int up)	{return SetParamUp(&mSpdUp,up);};

			void	SetAction(int action){mAction=action;	OpenCSkill(action);};
			void	SetTarget(int target){mTarget=target;};
			unsigned int		SetWaitTime(CBattleCharacter* Target);

			bool	SearchASkill(int skill)
						{if(!mCData)	return false;	return mCData->SearchASkill(skill);};
			int		SearchASkillType(int type,bool add)
						{if(!mCData)	return 0;		return mCData->SearchASkillType(type,add);};
			int		GetUsableCSkill(int* skill);
			bool	CheckAir(void);

			//int		GetActionCount(void){return mActionCount;};
			int		AddActionCount(void){mActionCount++;return mActionCount;};
			void	NoActionCount(void){mActionCount=1000;};
			int		GetAction(bool force=false);
			int		GetTarget(void){return mTarget;};

	virtual	bool	Enemy(void)				{return false;};
	virtual	bool	CheckSecondWeapon(void)	{return false;};
	virtual	void	TurnWeapon(double rad)	{return;};
	virtual	int		SetActionAuto(void)		{return 0;};

	virtual	int		GetSize(void)			{if(mCData){return mCData->GetSize();}	return 1;};
	virtual	bool	CheckOpenData(int no)	{return true;};
	virtual	void	SetOpenData(int no)		{return;};
	virtual	void	OpenCSkill(int no)		{return;};
	virtual	int		GetKillNo(void)	 const	{return -1;};
	virtual	void	SetKillNo(void)			{return;};
			int		GetID(void)				{return mID;};
			bool	GetCharm(void)			{return mCharm;};
			bool	GetBerserk(void)		{return mBerserk;};
			int		GetActionNo(void);
	virtual	void	GetDrawData(int &dx,int &dy,bool &turn); 
	virtual	void	GetDrawPositon(int &x,int &y,int &z); 
};

class CPlayerCharacter:public CBattleCharacter{
	CPlayerData*	mPData;
	CDTTexture		mWeaponImg;
	CDTTexture		mSecondWeaponImg;
	bool			mSecondWeapon;
	double			mTurnWeapon;

	int		mUseItem;
public:
	void	LoadCharacter(int no,int x,int y,int param);
	void	LoadWeaponImg(void);
	void	LoadWeaponImg(int weapon,CDTTexture *tex);
	void	OnDraw(int x,int y,D3DCOLOR color=0xffffffff,bool weapon=true);
	void	DrawWeapon(int x,int y,D3DCOLOR color,bool turn,bool top);
	void	DrawWeapon(int x,int y,D3DCOLOR color,bool turn,int weapon,CDTTexture *tex,const CAnimationData& anidata);
	void	GetDrawPositon(int &x,int &y,int &z);

	void	TurnWeapon(double rad)	{mTurnWeapon=rad;};
	void	GetDrawWeaponData(int *x,int *y,double *rad,bool *turn);
	void	GetDrawWeaponData(int *x,int *y,double *rad,bool *wepturn,bool turn,int weapon,const CAnimationData& anidata);

	CPlayerData*	GetPData()		{return mPData;};	
	void	DeleteAllUseItem(void);
	int		SearchUseItem(int no);
	bool	SetUseItem(int item);
	int		GetUseItem(){return mPData->GetItem(mUseItem).item;};
	void	GetUseItemParam(int* parama,int* paramb);
	bool	SpendUseItem(void);
	bool	CheckSecondWeapon(void)	{return mSecondWeapon;};

	const char*	GetName()const		{return mPData->GetName();};
};

class CPetCharacter:public CBattleCharacter{
	CCharacterData		mCharacterData;
public:
	void	LoadCharacter(int no,CCharacterData* master,int x,int y,int param);
	void	LoadImage(int no);
};
class CWallCharacter:public CBattleCharacter{
	CCharacterData		mCharacterData;
public:
	void	LoadCharacter(int no,int hp,int x,int y,int param);
	void	LoadImage(int no);

	void	DrawShadow(void){return ;};

	void	Move(int x,int y,int time,bool move){return;};
	void	SetGoalSquare(int x,int y,int time,int size,bool jump,bool force){return;};
	void	SetGoal(int x,int y,int time,bool jump,bool force){return;};
	void	SetPos(int x,int y,int z){return;};
	void	SetPosSquare(int x,int y,int size){return;};
	
	int		HealHP(int heal){return 0;};
	int		HealLP(int heal){return 0;};
	int		LostLP(int pow){return 0;};
	int		HealSP(int lv,int heal){return 0;};
	bool	LostSP(int lv,int pow){return false;};
	bool	SetStatus(int st,int param){return false;};
	int		SetParamUp(int *old,int up){return 0;};
	void	GetDrawData(int &dx,int &dy,bool &turn); 
};

class CEnemyCharacter:public CBattleCharacter{

	CEnemyData		mEData;
	int				mActionParam;
	int				mDeathBlack;
public:
	~CEnemyCharacter();
	void	LoadCharacter(int no,int lv,int x,int y,int param,bool leader=false);

	int		SetActionAuto(void);
	int		SetTargetAuto(void);
	int		GetDeathBlack(){return mDeathBlack;};
	void	SetDeathBlack(int db){mDeathBlack=db;};

	int		GetMoney(void){return mEData.GetMoney();};
	bool	Enemy(void){return true;};

	bool	CheckOpenData(int no);
	void	SetOpenData(int no);
	void	OpenCSkill(int no);
	int		GetKillNo(void)		 const	{return Flag.mEnemyOpenData[mNo-100].GetKill();};
	void	SetKillNo(void)				{Flag.mEnemyOpenData[mNo-100].SetKill();};
private:
	int		GetUsableCSkillWithParam(int* skill,int* param);
	int		PriorityRand(int* p,int no,int intel,int and=0xffffffff);
//敵の行動の決定関数
	int 	SAA_Random(int* skill,int* param,int skillno);
	int 	SAA_Priority(int* skill,int* param,int skillno,int intel);
	int 	SAA_RPriority(int* skill,int* param,int skillno,int intel);
	int 	SAA_MARotate(int* skill,int* param,int skillno,int intel);
	int 	SAA_NightKing(int* skill,int* param,int skillno,int intel);
	int 	SAA_DeathMember(int* skill,int* param,int skillno,int intel);
	int 	SAA_MARotate16(int* skill,int* param,int skillno,int intel);
	int 	SAA_LeftHP(int* skill,int* param,int skillno,int intel);
	int 	SAA_LeftHPRotate(int* skill,int* param,int skillno,int intel);
	int 	SAA_DeathMemRotate(int* skill,int* param,int skillno,int intel);
	int 	SAA_DreamEgg(int* skill,int* param,int skillno,int intel);

//敵の対象の決定関数
	void	STA_Random			(int param,int* target,int no);
	void	STA_MostDamage		(int param,int* target,int no,int intel);
	void	STA_MostHP			(int param,int* target,int no,int intel);
	void	STA_LowHP			(int param,int* target,int no,int intel);
	void	STA_HiLevel			(int param,int* target,int no,int intel);
	void	STA_MultiHit		(int param,int* target,int no,int intel);
	void	STA_MostLP			(int param,int* target,int no,int intel);
	void	STA_FewSt			(int param,int* target,int no,int intel);
	void	STA_MostSt			(int param,int* target,int no,int intel);
	void	STA_HiAtk			(int param,int* target,int no,int intel);
	void	STA_LowFieldEffect	(int param,int* target,int no,int intel);
	void	STA_LargeSize		(int param,int* target,int no,int intel);
	void	STA_HiDef			(int param,int* target,int no,int intel);
	void	STA_Left			(int param,int* target,int no,int intel);
	void	STA_Self			(int param,int* target,int no,int intel);

//ターゲッティング限定の例外
	void	STA_Targeting		(int param,int* target,int no,int intel);

};