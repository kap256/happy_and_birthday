#include "battlecharacter.h"
#include "input.h"
#include "battlemode.h"
#include "option.h"


int	CEnemyCharacter::SetActionAuto(void)
{
	if(!mLive)	return -1;
	int &think=mEData.mThinkType;
	int action=think & 0xff;
	int actoption=think & 0xff00;

	int skill[MAX_CSKILL];
	int allparam[MAX_CSKILL];
	int skillno=GetUsableCSkillWithParam(skill,allparam);

	if(skillno<=0){
		mAction=-1;
		mTarget=0;
		return mAction;
	}

	if(SearchStatus(2)>0){
		action=THINK_ACT_RANDOM;
	}
	if(SearchStatus(5)>0){
		action=THINK_ACT_RANDOM;
	}

//賢さ計算
	int intel = mEData.mInt;
	if(actoption & THINK_AOPT_HALF_INT)			intel/=2;
	if(BattleMode.mBattleOption & BOPT_INT_x3)	intel*=3;
	if(BattleMode.mBattleOption & BOPT_INT_x2)	intel*=3;
	switch(BattleMode.mTurn%8){
		case 0:
			if(actoption & THINK_AOPT_HALF_INT_1TURN)	 intel/=2;
			break;
	}
	if(actoption & THINK_AOPT_MA_NO_CONTINUE){
		for(int i=0;i<skillno;i++){
			if(skill[i]==mAction){
				allparam[i]=0;
				break;
			}
		}
	}

//行動選択分岐
	switch(action){
		case THINK_ACT_PRIORITY:
			mActionParam=SAA_Priority(skill,allparam,skillno,intel);		break;
		case THINK_ACT_ROTATE_PRI:
			mActionParam=SAA_RPriority(skill,allparam,skillno,intel);		break;
		case THINK_ACT_MA_ROTATE:
			mActionParam=SAA_MARotate(skill,allparam,skillno,intel);		break;
		case THINK_ACT_NIGHT_KING:
			mActionParam=SAA_NightKing(skill,allparam,skillno,intel);		break;
		case THINK_ACT_DETH_MEMBER:
			mActionParam=SAA_DeathMember(skill,allparam,skillno,intel);		break;
		case THINK_ACT_MA_ROTATE_16:
			mActionParam=SAA_MARotate16(skill,allparam,skillno,intel);		break;
		case THINK_ACT_LEFT_HP:
			mActionParam=SAA_LeftHP(skill,allparam,skillno,intel);			break;
		case THINK_ACT_LEFT_HP_ROTATE:
			mActionParam=SAA_LeftHPRotate(skill,allparam,skillno,intel);	break;
		case THINK_ACT_DETH_MEM_ROTATE:
			mActionParam=SAA_DeathMemRotate(skill,allparam,skillno,intel);	break;
		case THINK_ACT_DREAM_EGG:
			mActionParam=SAA_DreamEgg(skill,allparam,skillno,intel);	break;
		default:
			mActionParam=SAA_Random(skill,allparam,skillno);				break;
	}
	
//夢の卵専用例外処理
	if(SearchASkill(403)){
		int n=((mHP-1)*4/GetMaxLP());
		if(BattleMode.GetTurn()==2 && GetActionNo()-1 <= mActionCount){
			SetAction(203-n);
			mActionParam=0;
		}else if(n<1){
			SetAction(206);
			mActionParam=0;
		}
	}

	SetTargetAuto();

	return mAction;
}

int	CEnemyCharacter::SetTargetAuto(void)
{
	if(!mLive)	return -1;
	int &think=mEData.mThinkType;
	int target=think & 0x00ff0000;
	int toption=think & 0xff000000;

	if(mAction<0){
		mTarget=0;
		return mTarget;
	}

	if(SearchStatus(2)>0){
		target=THINK_TGT_RANDOM;
	}
	if(SearchStatus(5)>0){
		target=THINK_TGT_RANDOM;
	}

	int alltarget[CHARACTER_NO];
	int possible;
	int targetno=BattleMode.GetPossibleTarget(this,alltarget,&possible);
	if(possible>0){
		targetno=possible;
	}

//賢さ計算
	int intel = mEData.mInt;
	if(toption & THINK_TOPT_HALF_INT)			intel/=2;
	if(toption & THINK_TOPT_QUARTER_INT){
		intel/=4;
	}
	if(BattleMode.mBattleOption & BOPT_INT_x3)	intel*=3;
	if(BattleMode.mBattleOption & BOPT_INT_x2)	intel*=2;
	if(BattleMode.mBattleOption & BOPT_INT_HALF)intel/=2;

	
//ターゲッティング使用時の例外処理
	if(!(toption & THINK_TOPT_NO_TARGETING)){
		int tgt[CHARACTER_NO];
		bool targeting=false;
		for(int i=0;i<targetno;i++){
			CBattleCharacter *chara=BattleMode.mCharacter[alltarget[i]];
			tgt[i]=chara->SearchStatus(25);
			if(tgt[i]>0){
				targeting=true;
			}
		}
		if(targeting){
			int n=PriorityRand(tgt,targetno,intel);
			mTarget=alltarget[n];
			return mTarget;
		}
	}

//対象選択分岐
	switch(target){
		case THINK_TGT_MOST_DAMAGE:
			STA_MostDamage	(mActionParam,alltarget,targetno,intel);			break;
		case THINK_TGT_MOST_HP:
			STA_MostHP		(mActionParam,alltarget,targetno,intel);			break;
		case THINK_TGT_LOW_HP:
			STA_LowHP		(mActionParam,alltarget,targetno,intel);			break;
		case THINK_TGT_HI_LEVEL:
			STA_HiLevel		(mActionParam,alltarget,targetno,intel);			break;
		case THINK_TGT_MULTI_HIT:
			STA_MultiHit	(mActionParam,alltarget,targetno,intel);			break;
		case THINK_TGT_MOST_LP:
			STA_MostLP		(mActionParam,alltarget,targetno,intel);			break;
		case THINK_TGT_FEW_ST:
			STA_FewSt		(mActionParam,alltarget,targetno,intel);			break;
		case THINK_TGT_MOST_ST:
			STA_MostSt		(mActionParam,alltarget,targetno,intel);			break;
		case THINK_TGT_HI_ATK:
			STA_HiAtk		(mActionParam,alltarget,targetno,intel);			break;
		case THINK_TGT_LOW_FEFFECT:
			STA_LowFieldEffect(mActionParam,alltarget,targetno,intel);			break;
		case THINK_TGT_LARGE_SIZE:
			STA_LargeSize	(mActionParam,alltarget,targetno,intel);			break;
		case THINK_TGT_HI_DEF:
			STA_HiDef		(mActionParam,alltarget,targetno,intel);			break;
		case THINK_TGT_LEFT:
			STA_Left		(mActionParam,alltarget,targetno,intel);			break;
		case THINK_TGT_SELF:
			STA_Self		(mActionParam,alltarget,targetno,intel);			break;


		default:
			STA_Random(mActionParam,alltarget,targetno);				break;
	}

	return mTarget;
}

int	CEnemyCharacter::PriorityRand(int* p,int no,int intel,int and)
{
	unsigned int* pri=(unsigned int*)p;
	if(no<=0)	return -1;

	if(intel<0)	intel=mEData.mInt;

	unsigned int* best=new unsigned int[no];
	unsigned int* bestno=new unsigned int[no];
	memcpy(best,pri,sizeof(int)*no);
	for(int i=0;i<no;i++){
		best[i]=best[i]&((unsigned int)and);
		bestno[i]=i;
	}

	bool turn=true;
	while(turn){
		turn=false;
		for(int n=1;n<no;n++){
			if(best[n]>best[n-1]){
				turn=true;

				int i=best[n];
				best[n]=best[n-1];
				best[n-1]=i;

				i=bestno[n];
				bestno[n]=bestno[n-1];
				bestno[n-1]=i;
			}
		}
	}

	int size=0;
	int ret=pInput->Rand(no);
	while(size<no){
		unsigned int ok=best[size];
		while(size<no){
			if(best[size]>=ok){
				size++;
			}else{
				break;
			}
		}
		if(pInput->Rand(100) < intel){
			ret=bestno[pInput->Rand(size)];
			break;
		}
	}


	delete[] best;
	delete[] bestno;
	return ret;
}
////////////////////////////////////////////////////////////
int	CEnemyCharacter::SAA_Random(int* skill,int* param,int skillno)
{
	int rnd=pInput->Rand(skillno);
	SetAction(skill[rnd]);
	return param[rnd];
}
int	CEnemyCharacter::SAA_Priority(int* skill,int* param,int skillno,int intel)
{
	int no=PriorityRand(param,skillno,intel,0xff);
	SetAction(skill[no]);
	return param[no];
}
int	CEnemyCharacter::SAA_RPriority(int* skill,int* param,int skillno,int intel)
{
	int mask=0x0f;
	mask=mask << ((BattleMode.mTurn)%8*4);
	int no=PriorityRand(param,skillno,intel,mask);
	SetAction(skill[no]);
	return param[no];
}
int	CEnemyCharacter::SAA_MARotate(int* skill,int* param,int skillno,int intel)
{
	int mask=0x0f;
	int actno=GetActionNo();
	if(actno<=0)	actno=1;
	int turn=BattleMode.mTurn*actno + mActionCount;
	mask=mask << ((turn)%8*4);
	int no=PriorityRand(param,skillno,intel,mask);
	SetAction(skill[no]);
	return param[no];
}

int	CEnemyCharacter::SAA_NightKing(int* skill,int* param,int skillno,int intel)
{
	int mask=0x0080;
	if(SearchStatus(8)>0){
		if(mActionCount<=0)	mask=0x0800;
		else				mask=0x8000;
	}else if(mActionCount<=0){
		mask=0x0008;
	}
	
	int mlp=GetMaxLP();
	int hp=(GetHP()-1)*4/mlp;
	mask=mask >> hp;

	int turn=(BattleMode.mTurn)%2;
	mask=mask << ((turn)*16);

	int no=PriorityRand(param,skillno,intel,mask);
	SetAction(skill[no]);
	return param[no];
}
int	CEnemyCharacter::SAA_DeathMember(int* skill,int* param,int skillno,int intel)
{
	int deathnum=0;
	for(int i=0;i<MAX_ENEMY_NUMBER;i++){
		if(BattleMode.mEnemy[i].GetLive()){
			if(BattleMode.mEnemy[i].GetDead()){
				deathnum++;
			}
		}
	}
	if(deathnum>7)	deathnum=7;

	int mask=0x0f;
	mask=mask << (deathnum*4);
	int no=PriorityRand(param,skillno,intel,mask);
	SetAction(skill[no]);
	return param[no];
}

int	CEnemyCharacter::SAA_MARotate16(int* skill,int* param,int skillno,int intel)
{
	int mask=0x03;
	int actno=GetActionNo();
	if(actno<=0)	actno=1;
	int turn=BattleMode.mTurn*actno + mActionCount;
	mask=mask << ((turn)%16*2);
	int no=PriorityRand(param,skillno,intel,mask);
	SetAction(skill[no]);
	return param[no];
}
int	CEnemyCharacter::SAA_LeftHP(int* skill,int* param,int skillno,int intel)
{
	unsigned int mask=0xf0000000;
	int mlp=GetMaxLP();
	int hp=(GetHP()-1)*8/mlp;
	mask=mask >> (hp*4);
	int no=PriorityRand(param,skillno,intel,mask);
	SetAction(skill[no]);
	return param[no];
}
int	CEnemyCharacter::SAA_LeftHPRotate(int* skill,int* param,int skillno,int intel)
{
	unsigned int mask=0x10000000;

	int mlp=GetMaxLP();
	int hp=(GetHP()-1)*8/mlp;
	mask=mask >> (hp*4);

	int actno=GetActionNo();
	if(actno<=0)	actno=1;
	int turn=BattleMode.mTurn*actno + mActionCount;
	mask=mask << ((turn)%4);

	int no=PriorityRand(param,skillno,intel,mask);
	SetAction(skill[no]);
	return param[no];
}
int	CEnemyCharacter::SAA_DeathMemRotate(int* skill,int* param,int skillno,int intel)
{
	unsigned int mask=0x00000001;
	int deathnum=0;
	for(int i=0;i<MAX_ENEMY_NUMBER;i++){
		if(BattleMode.mEnemy[i].GetLive()){
			if(BattleMode.mEnemy[i].GetDead()){
				deathnum++;
			}
		}
	}
	if(deathnum>7)	deathnum=7;
	mask=mask << (deathnum*4);

	int actno=GetActionNo();
	if(actno<=0)	actno=1;
	int turn=BattleMode.mTurn*actno + mActionCount;
	mask=mask << ((turn)%4);

	int no=PriorityRand(param,skillno,intel,mask);
	SetAction(skill[no]);
	return param[no];
}

int	CEnemyCharacter::SAA_DreamEgg(int* skill,int* param,int skillno,int intel)
{
	unsigned int mask=0x10000000;

	int mlp=GetMaxLP();
	int hp=(GetHP()-1)*8/mlp;

	int actno=GetActionNo();
	if(actno<=0)	actno=1;
	int turn=BattleMode.mTurn*actno + mActionCount;

	if(BattleMode.mTurn<3){
		mask=mask << (turn%4);
		mask=mask >> (hp*4);
	}else{
		mask=mask << (turn%2);
		mask=mask >> (hp/2*8);
		mask=mask << 2;
		mask=mask >> ((hp%2)*2);
	}

	int no=PriorityRand(param,skillno,intel,mask);
	SetAction(skill[no]);
	return param[no];
}
////////////////////////////////////////////////////////////////////////////////////////
void	CEnemyCharacter::STA_Random(int param,int* target,int no)
{
	mTarget=target[pInput->Rand(no)];
}
void	CEnemyCharacter::STA_MostDamage(int param,int* target,int no,int intel)
{
	int dmg[CHARACTER_NO];
	for(int i=0;i<no;i++){
		CBattleCharacter *chara=BattleMode.mCharacter[target[i]];
		dmg[i]=chara->GetMaxHP() - chara->GetHP();
	}
	int n=PriorityRand(dmg,no,intel);

	mTarget=target[n];
}
void	CEnemyCharacter::STA_MostHP(int param,int* target,int no,int intel)
{
	int hp[CHARACTER_NO];
	for(int i=0;i<no;i++){
		CBattleCharacter *chara=BattleMode.mCharacter[target[i]];
		hp[i]=chara->GetHP();
	}
	int n=PriorityRand(hp,no,0xfffffff0);

	mTarget=target[n];
}
void	CEnemyCharacter::STA_LowHP(int param,int* target,int no,int intel)
{
	int hp[CHARACTER_NO];
	for(int i=0;i<no;i++){
		CBattleCharacter *chara=BattleMode.mCharacter[target[i]];
		hp[i]=0x7ffffff-chara->GetHP();
	}
	int n=PriorityRand(hp,no,0xfffffff0);

	mTarget=target[n];
}
void	CEnemyCharacter::STA_HiLevel(int param,int* target,int no,int intel)
{
	int lv[CHARACTER_NO];
	for(int i=0;i<no;i++){
		CBattleCharacter *chara=BattleMode.mCharacter[target[i]];
		lv[i]=chara->GetLv();
	}
	int n=PriorityRand(lv,no,intel);

	mTarget=target[n];
}
void	CEnemyCharacter::STA_MultiHit(int param,int* target,int no,int intel)
{
	int hit[CHARACTER_NO];
	int pno=BattleMode.GetCharacterNo(this);
	for(int i=0;i<no;i++){
		BattleMode.GetTarget(pno,target[i],mAction,&hit[i],true);
	}
	int n=PriorityRand(hit,no,intel);

	mTarget=target[n];
}
void	CEnemyCharacter::STA_MostLP(int param,int* target,int no,int intel)
{
	int lp[CHARACTER_NO];
	for(int i=0;i<no;i++){
		CBattleCharacter *chara=BattleMode.mCharacter[target[i]];
		lp[i]=chara->GetMaxHP();
	}
	int n=PriorityRand(lp,no,intel,0xfffffff0);

	mTarget=target[n];
}
void	CEnemyCharacter::STA_FewSt(int param,int* target,int no,int intel)
{
	int st[CHARACTER_NO];
	for(int i=0;i<no;i++){
		CBattleCharacter *chara=BattleMode.mCharacter[target[i]];
		st[i]=128-chara->GetStatusAll(NULL);
	}
	int n=PriorityRand(st,no,intel);

	mTarget=target[n];
}
void	CEnemyCharacter::STA_MostSt(int param,int* target,int no,int intel)
{
	int st[CHARACTER_NO];
	for(int i=0;i<no;i++){
		CBattleCharacter *chara=BattleMode.mCharacter[target[i]];
		st[i]=chara->GetStatusAll(NULL);
	}
	int n=PriorityRand(st,no,intel);

	mTarget=target[n];
}
void	CEnemyCharacter::STA_HiAtk(int param,int* target,int no,int intel)
{
	int atk[CHARACTER_NO];
	for(int i=0;i<no;i++){
		CBattleCharacter *chara=BattleMode.mCharacter[target[i]];
		atk[i]=chara->GetAtk();
	}
	int n=PriorityRand(atk,no,intel);

	mTarget=target[n];
}

void	CEnemyCharacter::STA_LowFieldEffect(int param,int* target,int no,int intel)
{
	int fe[CHARACTER_NO];
	for(int i=0;i<no;i++){
		CBattleCharacter *chara=BattleMode.mCharacter[target[i]];
		int x=chara->GetSX();
		int y=chara->GetSY();
		fe[i]=0xffff - (BattleMode.GetFieldEffect(x,y) & 0xffff);
	}

	int n=PriorityRand(fe,no,intel);
	mTarget=target[n];
}

void	CEnemyCharacter::STA_LargeSize(int param,int* target,int no,int intel)
{
	int size[CHARACTER_NO];
	for(int i=0;i<no;i++){
		CBattleCharacter *chara=BattleMode.mCharacter[target[i]];
		size[i]=chara->GetSize();
	}
	int n=PriorityRand(size,no,intel);

	mTarget=target[n];
}
void	CEnemyCharacter::STA_HiDef(int param,int* target,int no,int intel)
{
	int def[CHARACTER_NO];
	for(int i=0;i<no;i++){
		CBattleCharacter *chara=BattleMode.mCharacter[target[i]];
		def[i]=chara->GetDef();
	}
	int n=PriorityRand(def,no,intel);

	mTarget=target[n];
}
void	CEnemyCharacter::STA_Left(int param,int* target,int no,int intel)
{
	int x[CHARACTER_NO];
	for(int i=0;i<no;i++){
		CBattleCharacter *chara=BattleMode.mCharacter[target[i]];
		x[i]=10-chara->GetSX();
	}
	int n=PriorityRand(x,no,intel);

	mTarget=target[n];
}
void	CEnemyCharacter::STA_Self(int param,int* target,int no,int intel)
{
	int x[CHARACTER_NO];
	for(int i=0;i<no;i++){
		CBattleCharacter *chara=BattleMode.mCharacter[target[i]];
		x[i]=(chara==this?1:0);
	}
	int n=PriorityRand(x,no,intel);

	mTarget=target[n];
}

