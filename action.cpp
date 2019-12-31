
#include "action.h"
#include "main.h"

CBattleMode* CBattleAction::mBattleMode=&BattleMode;
CBattleAction::CBattleAction()
{
	memset(mTotalDmage,0,sizeof(mTotalDmage));
	memset(mActionEffect,0,sizeof(mActionEffect));
	memset(mActionParam,0,sizeof(mActionParam));
	mMessageWindow=true;
	mError=false;
	mFieldColorEffect=NULL;
}
CBattleAction::~CBattleAction()
{
	ChangeFieldColor(0,BM_INTARVAL_TIME);
}

void CBattleAction::Init(int act,int character,int dmesid)
{
	mTargetLock=true;
	mModeTimer=-BM_INTARVAL_TIME;
	mAction=act;
	mActiveCharacter=character;
	mDMesID=dmesid;

	for(int i=0;i<MAX_QUICK_LOAD;i++){
		int qload=ComSkillData[act].mQuickLoad[i];
		if(qload>=0){
			CBattleEffect::LoadTexture(&mQuickLoad[i],qload);
		}else{
			break;
		}
	}
}

void CBattleAction::StartAction()
{
	CBattleCharacter* acharacter=GetCharacter(mActiveCharacter);

	acharacter->SetTurn(false);
	if(ComSkillData[mAction].mSP>0){
		bool sp=acharacter->SpendSP(ComSkillData[mAction].mSP-1);
		if(!sp){
			SetError(7);
		}
	}
}
void	CBattleAction::SetError(int act)
{
	mError=true;
	mAction=act;
	BreakTargetLock();
}
int CBattleAction::OnTimer()
{
	// -1を返す：行動は終了しないが、タイマー関数は終了
	//  1を返す：行動終了。EndActionが呼ばれる。
	//  0を返す：行動続行。タイマー関数も呼ばれる。
	mModeTimer++;
	if(mModeTimer==0)	StartAction();
	if(mModeTimer<=0)	return -1;
	if(mError){
		return OnError();
	}
	int dx,dy;
	bool turn;
	ActiveCharacter()->GetDrawData(dx,dy,turn);
	if(dx==0 && dy==0){
		ActiveCharacter()->ChangeAni(1*ANI_TYPE,1,false);
	}
	return 0;
}
int CBattleAction::OnError()
{
	switch(mModeTimer){
		case 1:
			ActiveCharacter()->SetDamageEffect(false);
			PlayWave(SE_POO);
			break;
		case DAMAGE_TIMER+1:
			ActiveCharacter()->ChangeAni(7*ANI_TYPE,16,true);
			break;
		case DAMAGE_TIMER+20:
			return 1;
	}
	return -1;
}
void CBattleAction::End()
{
	CBattleCharacter* acharacter=GetCharacter(mActiveCharacter);
	if(mAction>=0 && ComSkillData[mAction].mType & CS_ITEM){
		acharacter->SpendUseItem();
	}

	acharacter->SetTurn(false);
	acharacter->ChangeAni(0,-1,true);

	if(mAction>=0){
		acharacter->Return(BM_INTARVAL_TIME-4,true);
	}

	for(int i=0;i<MAX_QUICK_LOAD;i++){
		mQuickLoad[i].Release();
	}

	if(acharacter->SearchASkill(306)){
		acharacter->DeleteCharacter();
	}
//ダメージメッセージの合計を計算。
	mBattleMode->SetTotalMessage(mDMesID);

	mBattleMode->SetInterval(BM_INTARVAL_RUN_START,1);
}
void CBattleAction::BreakTargetLock()
{
	if(mTargetLock){
		mTargetLock=false;
		mBattleMode->SetInterval(4,0);
	}
}
int*	CBattleAction::GetTargetLock(int* no,int act,int character)
{
	int action=act;
	const CComSkillData& cdata=ComSkillData[act];
	if((cdata.mRange==CR_NONE) || (cdata.mType & CS_TLOCK_VHARD)){
		action=220;
	}else	if((cdata.mType & CS_RIVIVAL) || (cdata.mType & CS_TLOCK_HARD)){
		action=221;
	}

	CBattleCharacter *c=GetCharacter(character);
	int target=GetCharacter(character)->GetTarget();
	int *ret=mBattleMode->GetTarget(character,target,action,no,false);

	//夢の卵用の例外処理
	if(ComSkillData[action].mRange>=CR_ALL)	return ret;
	for(int i=0;i<*no;i++){
		if(GetCharacter(ret[i]&0xffff)->SearchASkill(403)){
			return GetTargetLock(no,220,character);
		}
	}
	return ret;
}
int*	CBattleAction::GetTarget(int* no,bool liveonly)					
{
	CBattleCharacter *c=GetCharacter(mActiveCharacter);
	int target=GetCharacter(mActiveCharacter)->GetTarget();
	return mBattleMode->GetTarget(mActiveCharacter,target,mAction,no,liveonly);
}
void	CBattleAction::ChangeFieldColor(D3DCOLOR color,int time)			
{
	//色かえ
	if(!mFieldColorEffect){
		if(color==0){
			return;
		}
		mFieldColorEffect=GetBottomEffect().SetEffect(
			BE_FIELD_COLOR,0,0,
			time,color,0,0x00000000);
	}else{
		mFieldColorEffect->OperationA(time,color);
		if(color==0){
			mFieldColorEffect=NULL;
		}
	}
}