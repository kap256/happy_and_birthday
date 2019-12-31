#pragma once

#include "battleeffect.h"
#include "battleconst.h"
#include "battlecharacter.h"
#include "battlemode.h"

const int ACTION_EFFECT_NO		=16;
const int ACTION_PARAM_NO		=8;
class CBattleAction{
protected:
	bool	mMessageWindow;
	bool	mError;
	bool	mTargetLock;
	int		mModeTimer;
	int		mAction;
	int		mActiveCharacter;
	int		mDMesID;
	CBattleEffect*	mFieldColorEffect;

	int			mTotalDmage[CHARACTER_NO];
	CDTTexture	mQuickLoad[MAX_QUICK_LOAD];

	CBattleEffect*	mActionEffect[ACTION_EFFECT_NO];
	int*			mActionParam[ACTION_PARAM_NO];

	static CBattleMode*	mBattleMode;
	
public:
	CBattleAction();
	virtual ~CBattleAction();

	void Init(int act,int character,int dmesid);

	virtual void	StartAction();
	virtual int		OnTimer();
	virtual void	End();
			int		OnError();

	static int* GetTargetLock(int *no,int act,int character);
	int*	GetTargetLock(int *no){return GetTargetLock(no,mAction,mActiveCharacter);};
	bool	CheckTargetLock(){return mTargetLock;};

	void	SetError(int act=6);
	bool	CheckMessageWindow(){bool m=mMessageWindow;	mMessageWindow=false;	return m;};

	int		GetActiveCharacter(){return mActiveCharacter;};
	int		GetAction()			{return mAction;};
	int		GetTimer()			{return mModeTimer;};
	int		GetDMesID()			{return mDMesID;};
	int*	GetTotalDamage()	{return mTotalDmage;};

protected:
	void	BreakTargetLock();
	int*	GetTarget(int* no,bool liveonly=false);
	int*	GetTarget(int center,int act,int* no,bool liveonly=false)		
		{return mBattleMode->GetTarget(mActiveCharacter,center,act,no,liveonly);};
	int*	GetTarget(int field[BATTLE_SQUARE_WIDTH][BATTLE_SQUARE_Height],int center,int *no,bool liveonly=false)	
		{return mBattleMode->GetTarget(mActiveCharacter,field,center,no,liveonly);};
	void	GetAttackArea(int player,int x,int y,int size,int type,int	field[BATTLE_SQUARE_WIDTH][BATTLE_SQUARE_Height],int ratio=100)	
		{return mBattleMode->GetAttackArea(player,x,y,size,type,field,ratio);};

	CBattleCharacter*	ActiveCharacter()
		{return GetCharacter(mActiveCharacter);};
	static CBattleCharacter*	GetCharacter(int no)				
		{return mBattleMode->mCharacter[no];};
	bool				CheckEnemy(int player,int target=-1)
		{return mBattleMode->CheckEnemy(player,target);};

	CBattleEffect&		GetTopEffect()
		{return mBattleMode->mTopEffect;};
	CBattleEffect&		GetBottomEffect()
		{return mBattleMode->mBottomEffect;};
	void	ChangeFieldColor(D3DCOLOR color,int time);
	void	SetFieldShake(int x,int y)
		{mBattleMode->mFieldShakeX=x;mBattleMode->mFieldShakeX=y;};
	void	GetFieldShake(int* x,int* y)
		{*x=mBattleMode->mFieldShakeX;*y=mBattleMode->mFieldShakeX;};
	
	int*	Attack(int player,int* target,int targetno,int power,int minatk,int type,int element,int option,int sound)
		{return mBattleMode->Attack(player,target,targetno,power,minatk,type,element,option,sound,this);};
	int		Attack(int player,int target,int power,int minatk,int type,int element,int option,int sound,int ratio)
		{return mBattleMode->Attack(player,target,power,minatk,type,element,option,sound,ratio,this);};
	void	ChargeBreak()
		{return mBattleMode->ChargeBreak(mActiveCharacter);};
	int		GetElementPower(int player,int target,int element,int mainele,int option,int* selected=NULL)
		{return mBattleMode->GetElementPower( player, target, element, mainele, option,selected);};

	int		HealHP(int player,int target,int power,bool revival=false)
		{return mBattleMode->HealHP(player,target,power,mDMesID,revival);};
	int		HealLP(int player,int target,int power)
		{return mBattleMode->HealLP(player,target,power);};
	int		LostLP(int player,int target,int power)
		{return mBattleMode->LostLP(player,target,power,mDMesID);};
	int		HealSP(int player,int target,int power,int lv)
		{return mBattleMode->HealSP(player,target,power,lv);};
	int		LostSP(int player,int target,int power,int lv)
		{return mBattleMode->LostSP(player,target,power,lv);};

	int		ParamChange(int player,int target,int param,int up)
		{return mBattleMode->ParamChange(player,target,param,up);};
	int		SetStatus(int player,int target,int st,int hit,int param=-1,bool showmiss=true)
		{return mBattleMode->SetStatus(player,target,st,hit,param,showmiss);};
	bool	CureStatus(int player,int target)
		{return mBattleMode->CureStatus(player,target);};
	bool	DispelStatus(int player,int target)
		{return mBattleMode->DispelStatus(player,target);};
	bool	SetCancel(int target,int base)
		{return mBattleMode->SetCancel(mActiveCharacter,target,base);};
	void	MoveCharacter(int target,int x,int y,bool move=true)
		{return mBattleMode->MoveCharacter(target,x,y,move);};

	int		GetRandomTarget()
		{return mBattleMode->GetRandomTarget(mActiveCharacter,mAction);};
};