#include "standardaction.h"
#include "input.h"
#include "month.h"

////////////////////////////////////////////////////////////////////////////
	
CActNormalAttack::CActNormalAttack(int sound,int effect,int pow)
{
	mSound=sound;
	mEffect=effect;
	mPow=pow;
}
int CActNormalAttack::OnTimer()				//í èÌçUåÇ
{
//íËå^ï∂////////////////////////////////////////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////////////////////////////////////////

	switch(mModeTimer){
		case 1:
			ActiveCharacter()->ChangeAni(9*ANI_TYPE,8,true);
			break;
		case 4:
			int no,*target;
			target=GetTarget(&no);

			ActiveCharacter()->ChangeAni(11*ANI_TYPE,12,true);

			int turn;
			if(CheckEnemy(mActiveCharacter))	turn=TURN_HORIZONTAL;
			else	turn=0;
			GetTopEffect().SetEffect(BE_STANDARD,
				GetCharacter(target[0]&0xffff)->GetCX(),
				GetCharacter(target[0]&0xffff)->GetCY(),
				mEffect,turn,BLEND_ADD);

			Attack(	mActiveCharacter,
					target,no,
					mPow,0,
					AT_TYPE_WEAPON,
					AT_ELE_NONE,0,mSound);
			BreakTargetLock();
			break;
		case 7:
			ActiveCharacter()->ChangeAni(10*ANI_TYPE,16,true);
			break;

		case 30:
			return 1;
	}
	return 0;
}


////////////////////////////////////////////////////////////////////////////
int CActThrust::OnTimer()				//ìÀÇ≠
{
//íËå^ï∂////////////////////////////////////////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////////////////////////////////////////

	switch(mModeTimer){
		case 1:
			ActiveCharacter()->ChangeAni(9*ANI_TYPE+2,16,true);
			break;
		case 4:
			int no,*target;
			target=GetTarget(&no);

			ActiveCharacter()->ChangeAni(12*ANI_TYPE+1,16,true);

			int turn;
			if(CheckEnemy(mActiveCharacter))	turn=TURN_HORIZONTAL;
			else	turn=0;
			GetTopEffect().SetEffect(BE_STANDARD,
				GetCharacter(ActiveCharacter()->GetTarget())->GetCX(),
				GetCharacter(ActiveCharacter()->GetTarget())->GetCY(),
				2,turn,BLEND_ADD);

			Attack(	mActiveCharacter,
					target,no,
					100,0,
					AT_TYPE_WEAPON,
					AT_ELE_NONE,0,SE_THRUST);
			BreakTargetLock();

			break;
		case 7:
			ActiveCharacter()->ChangeAni(11*ANI_TYPE+2,16,true);
			break;

		case 30:
			return 1;	break;
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////
CActShoot::CActShoot(int pow)
{
	mPow=pow;
}
int CActShoot::OnTimer()				//éÀÇÈ
{
//íËå^ï∂////////////////////////////////////////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////////////////////////////////////////

	double sx,sy;
	int target=ActiveCharacter()->GetTarget();
	double length=mBattleMode->GetShotSpeed(mActiveCharacter,target,48,&sx,&sy);

	switch(mModeTimer){
		case 1:
			ActiveCharacter()->ChangeAni(12*ANI_TYPE,16,true);
			break;
		case 4:
			PlayWave(SE_BOW);
			GetTopEffect().SetEffect(BE_MOVE,
							ActiveCharacter()->GetCX(),
							ActiveCharacter()->GetCY(),
							BE_SPEED(sx,sy),4,BLEND_ADD);
			ActiveCharacter()->ChangeAni(9*ANI_TYPE+3,20,true);
			break;

	}

	if(mModeTimer-4==int(length)){
		int no,*target;
		target=GetTarget(&no);

		Attack(	mActiveCharacter,
				target,no,
				mPow,0,
				AT_TYPE_BOW,
				AT_ELE_NONE,0,SE_SHOOT);
		BreakTargetLock();
	}
	if(mModeTimer-30>=int(length)){
		return 1;
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////
int CActLaser::OnTimer()				//ÉåÅ[ÉUÅ[
{
//íËå^ï∂////////////////////////////////////////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////////////////////////////////////////

	switch(mModeTimer){
		case 1:
			ActiveCharacter()->ChangeAni(12*ANI_TYPE,16,true);
			int no,*target;
			target=GetTarget(&no);
			Attack(	mActiveCharacter,
					target,no,
					100,0,
					AT_TYPE_LASER,
					AT_ELE_NONE,0,SE_LASER);

			int tx,ty;
			tx=GetCharacter(ActiveCharacter()->GetTarget())->GetCX()-ActiveCharacter()->GetCX();
			tx*=1024;
			tx+=ActiveCharacter()->GetCX();
			ty=GetCharacter(ActiveCharacter()->GetTarget())->GetCY()-ActiveCharacter()->GetCY();
			ty*=1024;
			ty+=ActiveCharacter()->GetCY();

			mActionEffect[0]=GetTopEffect().SetEffect(
							BE_LINE,
							ActiveCharacter()->GetCX(),
							ActiveCharacter()->GetCY(),
							tx,ty,BLEND_ADD,0xffff0000);
			BreakTargetLock();
			break;
		case 3:
			if(mActionEffect[0]){
				mActionEffect[0]->Destroy();
			}
			break;
		case 25:
			return 1;
	}
	return 0;
}

////////////////////////////////////////////////////////////////////////////
CActHardAttack::CActHardAttack(int pow,int effect,int sound)
{
	mPow=pow;
	mEffect=effect;
	mSound=sound;
}
int CActHardAttack::OnTimer()				//ä€ëæêÿÇËÇ»Ç«
{
//íËå^ï∂////////////////////////////////////////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////////////////////////////////////////
	switch(mModeTimer){
		case 1:
			PlayWave(SE_MISS);
			ActiveCharacter()->ChangeAni(10*ANI_TYPE,22,true);
			break;
		case 4:
			ActiveCharacter()->ChangeAni(11*ANI_TYPE,12,true);
			break;
		case 7:
			ActiveCharacter()->ChangeAni(9*ANI_TYPE,40,true);
			break;
		case 25:
			int no,*target;
			target=GetTarget(&no);

			ActiveCharacter()->ChangeAni(11*ANI_TYPE,12,true);

			int turn;
			if(CheckEnemy(mActiveCharacter))	turn=TURN_HORIZONTAL;
			else	turn=0;
			GetTopEffect().SetEffect(BE_STANDARD,
				GetCharacter(ActiveCharacter()->GetTarget())->GetCX(),GetCharacter(ActiveCharacter()->GetTarget())->GetCY(),
				mEffect,turn,BLEND_ADD);

			Attack(	mActiveCharacter,
					target,no,
					mPow,MIN_ATK_LV_0,
					AT_TYPE_WEAPON,
					AT_ELE_NONE,0,mSound);
			BreakTargetLock();

			break;
		case 28:
			ActiveCharacter()->ChangeAni(10*ANI_TYPE,12,true);
			break;

		case 45:
			return 1;
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////
CActSpinAttack::CActSpinAttack(int pow,int min)
{
	mPow=pow;
	mMin=min;
}
int CActSpinAttack::OnTimer()				//âÒì]ÉMÉäÇ»Ç«
{
//íËå^ï∂////////////////////////////////////////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////////////////////////////////////////

	int turn;
	if(CheckEnemy(mActiveCharacter))	turn=TURN_HORIZONTAL;
	else	turn=0;
	switch(mModeTimer){
		case 1:
			ActiveCharacter()->ChangeAni(10*ANI_TYPE+1,16,true);
			break;
		case 3:
			ActiveCharacter()->ChangeAni(11*ANI_TYPE+1,12,true);
			GetTopEffect().SetEffect(BE_LARGE,
				ActiveCharacter()->GetCX(),ActiveCharacter()->GetCY(),
				16,turn,BLEND_ADD);
			break;
		case 5:
			ActiveCharacter()->ChangeAni(9*ANI_TYPE+1,18,true);

			int no,*target;
			target=GetTarget(&no);

			Attack(	mActiveCharacter,
					target,no,
					mPow,mMin,
					AT_TYPE_WEAPON,
					AT_ELE_NONE,0,SE_SLASH);
			BreakTargetLock();

			break;
		case 7:
			ActiveCharacter()->SetTurn(true);
			ActiveCharacter()->ChangeAni(9*ANI_TYPE,16,true);
			break;
		case 9:
			ActiveCharacter()->ChangeAni(11*ANI_TYPE,12,true);
			break;
		case 11:
			ActiveCharacter()->ChangeAni(10*ANI_TYPE,12,true);
			turn=turn^TURN_HORIZONTAL;
			turn=turn | TURN_VERTICAL;
			GetTopEffect().SetEffect(BE_LARGE,
				ActiveCharacter()->GetCX(),ActiveCharacter()->GetCY(),
				16,turn,BLEND_ADD);
			break;
		case 13:
			ActiveCharacter()->SetTurn(false);
			ActiveCharacter()->ChangeAni(10*ANI_TYPE+1,16,true);
			break;
		case 15:
			ActiveCharacter()->ChangeAni(11*ANI_TYPE+1,12,true);
			break;
		case 17:
			ActiveCharacter()->ChangeAni(9*ANI_TYPE+1,20,true);
			break;


		case 40:
			return 1;	break;
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////
CActParamUp::CActParamUp(int param,D3DCOLOR color)
{
	mParam=param;
	mColor=color;
}
int CActParamUp::OnTimer()				//ÉpÉâÉÅÅ[É^ÉAÉbÉvån
{
//íËå^ï∂////////////////////////////////////////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////////////////////////////////////////

	if(mModeTimer==10){
		ActiveCharacter()->ChangeAni(13*ANI_TYPE,32,true);

		int no,*target;
		target=GetTarget(&no);
		int turn;
		if(CheckEnemy(mActiveCharacter))	turn=TURN_HORIZONTAL;
		else	turn=0;

		int mag=ActiveCharacter()->GetMagAtk();
		for(int i=0;i<no;i++){
			GetTopEffect().SetEffect(
				BE_FLASH_CHARACTER,
				0,0,
				target[i] & 0xffff,0,BLEND_SOLID_ADD,mColor);
			GetTopEffect().SetEffect(
				BE_STANDARD,
				GetCharacter(target[i]&0xffff)->GetCX(),
				GetCharacter(target[i]&0xffff)->GetCY(),
				18,turn,BLEND_ADD,mColor);

			switch(mParam){
				case 0:
					//mag=ActiveCharacter()->GetBaseMagAtk();
					ParamChange(mActiveCharacter,target[i]&0xffff,UP_ATK,mag/3+30);
					ParamChange(mActiveCharacter,target[i]&0xffff,UP_MATK,mag/3+30);
					break;
				case 1:
					ParamChange(mActiveCharacter,target[i]&0xffff,UP_MDEF,mag/6+15);
					break;
				case 2:
					ParamChange(mActiveCharacter,target[i]&0xffff,UP_SPD,int(log(float(mag))*10)+10);
					break;
				case 3:
					ParamChange(mActiveCharacter,target[i]&0xffff,UP_DEF,mag/6+15);
					break;
			}
			BreakTargetLock();
		}
	}else if(mModeTimer==40){
		return 1;
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////
CActParamDown::CActParamDown(int param,D3DCOLOR color)
{
	mParam=param;
	mColor=color;
}
int CActParamDown::OnTimer()				//ÉpÉâÉÅÅ[É^É_ÉEÉìån
{
//íËå^ï∂////////////////////////////////////////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////////////////////////////////////////

	int no,*target;
	target=GetTarget(&no);

	if(mModeTimer<32){
		for(int i=0;i<no;i++){
			int t=target[i]&0xffff;
			GetCharacter(t)->SetDamageEffect(false);
		}

		if(mModeTimer%16==1)	PlayWave(SE_PARAM_DOWN);
		for(int i=0;i<4;i++){
			GetTopEffect().SetEffect(
						BE_FADE_IN_PARTICUL,
						GetCharacter(ActiveCharacter()->GetTarget())->GetCX()+pInput->Rand(255)-127,
						GetCharacter(ActiveCharacter()->GetTarget())->GetGCY()-160,
						BE_SPEED(0,32),35+8*0x10000,
						BLEND_ADD,mColor);
		}

	}else if(mModeTimer==32){
		ActiveCharacter()->ChangeAni(13*ANI_TYPE,32,true);

		int no,*target;
		target=GetTarget(&no);
		int turn;
		if(CheckEnemy(mActiveCharacter))	turn=TURN_HORIZONTAL;
		else	turn=0;

		for(int i=0;i<no;i++){
			int t=target[i]&0xffff;
			GetTopEffect().SetEffect(
				BE_FLASH_CHARACTER,
				0,0,
				t,0,BLEND_SOLID_ADD,mColor);
			GetCharacter(t)->SetDamageEffect(false);

			int mag=ActiveCharacter()->GetMagAtk();
			switch(mParam){
				case 0:
					mag=ActiveCharacter()->GetBaseMagAtk();
					ParamChange(mActiveCharacter,target[i]&0xffff,UP_ATK,-mag/2);
					break;
				case 1:
					ParamChange(mActiveCharacter,target[i]&0xffff,UP_DEF,-mag/2);
					break;
			}
			BreakTargetLock();
		}
	}else if(mModeTimer==50){
		return 1;
	}

	return 0;
}
////////////////////////////////////////////////////////////////////////////
CActCure::CActCure(int size,int no,bool down,bool heal)
{
	mSize=size;
	mNo=no;
	mDown=down;
	mHeal=heal;
}
int CActCure::OnTimer()				//é°ó√ån
{
//íËå^ï∂////////////////////////////////////////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////////////////////////////////////////

	int target=ActiveCharacter()->GetTarget();
	int x=GetCharacter(target)->GetCX();
	int y=GetCharacter(target)->GetCY()+32;
	if(mModeTimer==1){
		if(mDown){
			ActiveCharacter()->ChangeAni(6*ANI_TYPE,58,true);
		}
		mActionEffect[0]=GetBottomEffect().SetEffect(
							BE_STRETCH,
							x,
							y,
							25,mSize*2+mSize*2*0x10000,
							BLEND_ADD);
	}else if(mModeTimer==40){
		int no,*target;
		target=GetTarget(&no);

		for(int i=0;i<no;i++){
			bool cure=CureStatus(
						mActiveCharacter,
						target[i]&0xffff);
			if(!cure && mHeal){
				HealHP(	mActiveCharacter,
						target[i]&0xffff,
						int(GetCharacter(target[i]&0xffff)->GetMaxHP()*0.25));
			}
		}

		BreakTargetLock();

	}else if(mModeTimer>=64){
		mActionEffect[0]->Destroy();
		mActionEffect[0]=NULL;
		return 1;
	}

	if(mActionEffect[0]){
		int t;
		if(mModeTimer<32){
			t=mModeTimer;
		}else{
			t=64-mModeTimer;
		}
		mActionEffect[0]->SetColor(0x04000000*t+0xffffff);

		if(mModeTimer%6==0){
			PlayWave(SE_STAR);
		}

		for(int i=0;i<mNo;i++){
			double rad=pInput->Rand(1000)*3.141592*2/1000.0;
			int s=int(sin(rad)*mSize*2/3);
			int c=int(cos(rad)*mSize);
			GetTopEffect().SetEffect(BE_PARTICUL,
						x+c,
						y+s,
						BE_SPEED(0,-4),16*0x10000+26,
						BLEND_ADD,0xff00ff00);
		}
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////
CActGus::CActGus(int st)
{
	mST=st;
}
int CActGus::OnTimer()				//ÉKÉXån
{
//íËå^ï∂////////////////////////////////////////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////////////////////////////////////////

	const D3DCOLOR c=StatusData[mST].mColor&0x00ffffff;
	const int TIME = 48;
	if(mModeTimer>5){
		if(mActionEffect[0]){
			if(mModeTimer>=5+TIME){
				mActionEffect[0]->Destroy();
				mActionEffect[0]=NULL;
			}else{
				int t=mModeTimer-5;
				D3DCOLOR color=c+((0xff000000/TIME*(TIME-t))&0xff000000);
				mActionEffect[0]->SetColor(color);
				mActionEffect[0]->OperationA(t*16,t*16);
			}
		}
	}
	if(mModeTimer==5){
		int no,*target;
		target=GetTarget(&no);
		ActiveCharacter()->ChangeAni(13*ANI_TYPE,64,true);
		mActionEffect[0]=GetTopEffect().SetEffect(
				BE_STRETCH,
				GetCharacter(ActiveCharacter()->GetTarget())->GetCX(),
				GetCharacter(ActiveCharacter()->GetTarget())->GetCY(),
				20,0+0*0x10000,BLEND_ADD);

		mActionEffect[0]->SetColor(c+0xff000000);

		PlayWave(SE_GUS);
	}else	if(mModeTimer==30){
		int no,*target;
		target=GetTarget(&no);

		for(int i=0;i<no;i++){
			for(int j=0;j<8;j++){
				int x=pInput->Rand(33)-16;
				int y=pInput->Rand(33)-16;
				int sx=pInput->Rand(5)-2;
				int sy=pInput->Rand(3)-5;
				D3DCOLOR color=c+pInput->Rand(63)*0x000100;
				GetTopEffect().SetEffect(BE_PARTICUL,
							GetCharacter(target[i]&0xffff)->GetCX()+x,
							GetCharacter(target[i]&0xffff)->GetCY()+48+y,
							BE_SPEED(sx,sy),24*0x10000+21,
							BLEND_STANDARD,color);
			}
			SetStatus(mActiveCharacter,target[i]&0xffff,mST,100);

		}
		BreakTargetLock();
	}else if(mModeTimer==60){
		return 1;
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////
CActMagicField::CActMagicField(int field)
{
	mField=field;
}
int CActMagicField::OnTimer()				//åãäEån
{
//íËå^ï∂////////////////////////////////////////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////////////////////////////////////////
	if(mModeTimer==1){
		mBattleMode->mMagicField=mField;
		mBattleMode->ChangeFieldColor(BattleFieldColor[mField],48);
		ActiveCharacter()->ChangeAni(13*ANI_TYPE,40,true);
		PlayWave(SE_MAGIC);
		BreakTargetLock();
	}else if(mModeTimer==48){
		return 1;
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////
CActReflection::CActReflection(int pow)
{
	mPow=pow;
}
int CActReflection::OnTimer()				//ÉäÉtÉåÉNÉVÉáÉì
{
//íËå^ï∂////////////////////////////////////////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////////////////////////////////////////
	if(mModeTimer==1){
		int no,*target;
		target=GetTarget(&no);

		int turn;
		if(GetCharacter(ActiveCharacter()->GetTarget())->Enemy())	turn=TURN_HORIZONTAL;
		else	turn=0;

		for(int i=0;i<no;i++){
			int t=target[i]&0xffff;
			mActionEffect[i%ACTION_EFFECT_NO]=GetTopEffect().SetEffect(
				BE_STRETCH,
				GetCharacter(t)->GetCX(),
				GetCharacter(t)->GetCY(),
				55+turn*0x10000,128+128*0x10000,
				BLEND_ADD,0x00ffffff);
			SetStatus(mActiveCharacter,t,20,100,mPow);
		}
		BreakTargetLock();
	}else if(mModeTimer==50){
		return 1;
	}
	for(int i=0;i<ACTION_EFFECT_NO;i++){
		if(mActionEffect[i]){
			if(mModeTimer<=16){
				mActionEffect[i]->SetColor(0x0b000000*mModeTimer | 0xffffff);
			}else if(mModeTimer<32){
			}else if(mModeTimer<48){
				mActionEffect[i]->SetColor(0x0b000000*(48-mModeTimer) | 0xffffff);
			}else{
				mActionEffect[i]->Destroy();
				mActionEffect[i]=NULL;
			}
		}
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////
CActBarrier::CActBarrier(int pow)
{
	mPow=pow;
}
int CActBarrier::OnTimer()				//ÉCÉìÉXÉ^ÉìÉgÉoÉäÉA
{
//íËå^ï∂////////////////////////////////////////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////////////////////////////////////////
	if(mModeTimer==1){
		int no,*target;
		target=GetTarget(&no);

		int turn;
		if(GetCharacter(ActiveCharacter()->GetTarget())->Enemy())	turn=TURN_HORIZONTAL;
		else	turn=0;

		mActionEffect[0]=GetTopEffect().SetEffect(
			BE_STRETCH,
			GetCharacter(ActiveCharacter()->GetTarget())->GetCX(),
			GetCharacter(ActiveCharacter()->GetTarget())->GetCY(),
			40+turn*0x10000,128+128*0x10000,
			BLEND_ADD,0x00ffffff);

		for(int i=0;i<no;i++){
			SetStatus(mActiveCharacter,target[i]&0xffff,16,100,mPow);
		}
		BreakTargetLock();
	}else if(mModeTimer==50){
		return 1;
	}
	if(mActionEffect[0]){
		if(mModeTimer<=16){
			mActionEffect[0]->SetColor(0x0b000000*mModeTimer | 0xffffff);
		}else if(mModeTimer<32){
		}else if(mModeTimer<48){
			mActionEffect[0]->SetColor(0x0b000000*(48-mModeTimer) | 0xffffff);
		}else{
			mActionEffect[0]->Destroy();
			mActionEffect[0]=NULL;
		}
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////
CActRevival::CActRevival(int pow,bool live)
{
	mPow=pow;
	mLive=live;
}
int CActRevival::OnTimer()				//ëhê∂éËíi
{
//íËå^ï∂////////////////////////////////////////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////////////////////////////////////////
	if(mModeTimer==1){
		PlayWave(SE_RIVIVAL);
		ChangeFieldColor(0x80000000,16);
		int no,*target;
		target=GetTarget(&no);

		for(int i=0;i<no;i++){
			if(!mLive){
				if(!GetCharacter(target[i]&0xffff)->GetDead())	continue;
			}
			GetBottomEffect().SetEffect(BE_SPOT_LIGHT,
				GetCharacter(target[i]&0xffff)->GetCX(),
				GetCharacter(target[i]&0xffff)->GetCY(),
				75+0x10000,96+32*0x10000,BLEND_ADD,0x80ffffff);
			GetTopEffect().SetEffect(BE_SPOT_LIGHT,
				GetCharacter(target[i]&0xffff)->GetCX(),
				GetCharacter(target[i]&0xffff)->GetCY(),
				75,96+32*0x10000,BLEND_ADD,0x80ffffff);
		}
	}else if(mModeTimer==64){
		ChangeFieldColor(0x00000000,16);
		int no,*target;
		target=GetTarget(&no);

		for(int i=0;i<no;i++){
			if(!mLive){
				if(!GetCharacter(target[i]&0xffff)->GetDead())	continue;
			}
			HealHP(	mActiveCharacter,
					target[i]&0xffff,
					GetCharacter(target[i]&0xffff)->GetMaxHP()*mPow/100,
					true);
		}
		BreakTargetLock();
	}else if(mModeTimer>=80){
		return 1;
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////
CActSkillDrain::CActSkillDrain(int lv,int pow,D3DCOLOR color)
{
	mLv=lv;
	mPow=pow;
	mColor=color;
}
int CActSkillDrain::OnTimer()				//ÉXÉLÉãÉhÉåÉCÉì
{
//íËå^ï∂////////////////////////////////////////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////////////////////////////////////////
	const int TIME = 24;
	int no,*target;
	target=GetTarget(&no);

	if(mModeTimer==1){
		ActiveCharacter()->ChangeAni(13*ANI_TYPE,TIME,true);
	}
	int pow=mPow;
	if(ActiveCharacter()->SearchASkill(419))	pow*=2;

	bool end=true;
	for(int i=0;i<no;i++){
		int time=mModeTimer-i*4;
		int t=target[i]&0xffff;
		double sx,sy;
		double length=mBattleMode->GetShotSpeed(mActiveCharacter,t,32,&sx,&sy);
		if(time==1){
			for(int i=0;i<16;i++){
				int angle=i*360/16;
				const int length=200;

				GetTopEffect().SetEffect(
						BE_CONCENTRATE,
						GetCharacter(t)->GetCX(),
						GetCharacter(t)->GetCY(),
						angle+length*0x10000,21+(TIME-1)*0x10000,
						BLEND_ADD,mColor);
			}
			PlayWave(SE_PARAM_UP);
		}
		if(time==TIME){
			PlayWave(SE_POO);
			mActionEffect[i%ACTION_EFFECT_NO]=GetTopEffect().SetEffect(BE_SMALL_SHOT,
							GetCharacter(t)->GetCX(),
							GetCharacter(t)->GetCY(),
							BE_SPEED(-sx,-sy),54,BLEND_ADD,mColor | 0xff808080);
			GetCharacter(t)->SetDamageEffect(false);
			LostSP(	mActiveCharacter,
					t,
					mLv,pow);
			if(i==no-1)	BreakTargetLock();
		}

		if(time-TIME==int(length)){
			mActionEffect[i%ACTION_EFFECT_NO]->Destroy();
			mActionEffect[i%ACTION_EFFECT_NO]=NULL;
			GetTopEffect().SetEffect(BE_STANDARD,
				ActiveCharacter()->GetCX(),
				ActiveCharacter()->GetCY(),
				6,0,BLEND_ADD,0xff0000ff);
			HealSP(	mActiveCharacter,
					mActiveCharacter,
					mLv,pow);
		}
		if(time-40<=int(length)){
			end=false;
		}
	}
	if(end){
		return 1;
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////
CActCallPet::CActCallPet(int no)
{
	mNo=no;
}
int CActCallPet::OnTimer()				//ÉyÉbÉgåƒÇ—èoÇµ
{
//íËå^ï∂////////////////////////////////////////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////////////////////////////////////////
	switch(mModeTimer){
		case 1:
			{
				ActiveCharacter()->ChangeAni(13*ANI_TYPE,16,true);
				static int  field[BATTLE_SQUARE_WIDTH][BATTLE_SQUARE_Height];
				memset(field,0,sizeof(field));

				for(int i=0;i<CHARACTER_NO;i++){
					if(!GetCharacter(i)->GetLive())	continue;
					int x=GetCharacter(i)->GetSX();
					int y=GetCharacter(i)->GetSY();
					int size=GetCharacter(i)->GetSize();
					for(int i=0;i<size;i++){
						for(int j=0;j<size;j++){
							field[x+i][y+j]=1;
						}
					}
				}

				try{
					for(int i=0;i<MAX_PET_NUMBER;i++){
						if(mBattleMode->mPet[i].GetLive() && mBattleMode->mPet[i].GetNo()==mNo){
							throw 0;
						}
					}
					int i;
					for(i=0;i<MAX_PET_NUMBER;i++){
						if(mBattleMode->mPet[i].GetLive()){
							continue ;
						}
						if(mBattleMode->SetPet(i,mNo,mActiveCharacter,field,mBattleMode->mBattleOption)==0){
							throw 0;
						}
						PlayWave(SE_CALL);
						if(ActiveCharacter()->SearchASkill(408)){
							mBattleMode->SetRandomAction(i+PET);
						}
						break;
					}
					if(i>=MAX_PET_NUMBER){
						throw 0;
					}
				}catch(int n){
					n=0;
					int act=ActiveCharacter()->GetAction(true);
					ActiveCharacter()->HealSP(ComSkillData[act].mSP,1);
					mModeTimer=0;

					SetError(6);
					return 0;
				}
			}

			break;
			
		case 20:
			return 1;
			break;
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////
CActDeathWord::CActDeathWord(int st,D3DCOLOR color)
{
	mST=st;
	mColor=color;
}
int CActDeathWord::OnTimer()				//ÇÃÇÎÇ¢ÇÃåæót
{
//íËå^ï∂////////////////////////////////////////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////////////////////////////////////////
	const int SPREAD_TIME	= 128;

	int target=ActiveCharacter()->GetTarget();
	if(mModeTimer==1){
		ChangeFieldColor((mColor&0x80ffffff),SPREAD_TIME-16);
		ActiveCharacter()->ChangeAni(13*ANI_TYPE,SPREAD_TIME,true);
		for(int i=0;i<ACTION_EFFECT_NO;i++){
			mActionEffect[i]=GetTopEffect().SetEffect(
								BE_SMALL_ANIMATION,
								ActiveCharacter()->GetCX(),
								ActiveCharacter()->GetCY(),
								74,0,BLEND_STANDARD,mColor);
			mActionEffect[i]->OperationA(pInput->Rand(16),0);
		}
		PlayWave(SE_BLACK_HOLE);
	}else if(mModeTimer==SPREAD_TIME-16){
		int no,*target;
		target=GetTarget(&no);

		for(int i=0;i<no;i++){
			SetStatus(mActiveCharacter,target[i]&0xffff,mST,65535);
		}
		BreakTargetLock();
	}else if(mModeTimer>=SPREAD_TIME+4){
		return 1;
	}

	int centerx=ActiveCharacter()->GetCX();
	int centery=ActiveCharacter()->GetCY();
	for(int i=0;i<ACTION_EFFECT_NO;i++){
		if(mActionEffect[i]){
			int time=mModeTimer;
			if(time>SPREAD_TIME){
				mActionEffect[i]->Destroy();
				mActionEffect[i]=NULL;
				continue;
			}
			const int EFFECT_X =640;
			const int EFFECT_Y =320;
			double rad=mModeTimer*mModeTimer*0.0012+i*3.141592*2/ACTION_EFFECT_NO;
			double size=double(time)/SPREAD_TIME;
			mActionEffect[i]->SetPos(
				centerx+int(cos(rad)*EFFECT_X*size),
				centery+int(sin(rad)*EFFECT_Y*size)
				);
		}
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////
int CActBokoboko::OnTimer()				//É{ÉRÉ{ÉR
{
//íËå^ï∂////////////////////////////////////////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////////////////////////////////////////

	const int HIT_TIME	= 12;
	const int HIT_NO	= 4;
	const int END_TIME	= 16;


	if(mModeTimer<HIT_TIME*HIT_NO){
		int no,*target;
		int turn=(mModeTimer/HIT_TIME)%2;
		target=GetTarget(&no);
		int t=ActiveCharacter()->GetTarget();

		bool dmg=false;
		ActiveCharacter()->SetTurn(turn!=0);
		if(!CheckEnemy(mActiveCharacter))	turn=1-turn;
		switch(mModeTimer%HIT_TIME){
			case 1:
				ActiveCharacter()->ChangeAni(9*ANI_TYPE+2,16,true);

				int tsize,csize,ssize;
				tsize=GetCharacter(t)->GetSize();
				csize=ActiveCharacter()->GetSize();
				ssize=BATTLE_SQUARE_SIZE;
				ActiveCharacter()->SetGoal(
					GetCharacter(t)->GetCX()+(turn?ssize:-ssize)*(tsize) - csize*ssize*3/4,
					ActiveCharacter()->GetY(),
					1,false);
				break;
			case 4:
				ActiveCharacter()->ChangeAni(11*ANI_TYPE+1,16,true);

				GetTopEffect().SetEffect(BE_STANDARD,
					GetCharacter(ActiveCharacter()->GetTarget())->GetCX(),
					GetCharacter(ActiveCharacter()->GetTarget())->GetCY(),
					1,0,BLEND_ADD);

				dmg=true;
				

				break;
			case 7:
				ActiveCharacter()->ChangeAni(10*ANI_TYPE+2,20,true);
				break;
		}

		if(dmg){
			Attack(	mActiveCharacter,
					target,no,
					75,0,
					AT_TYPE_WEAPON,
					AT_ELE_NONE,0,SE_HIT);
			if(mModeTimer/HIT_TIME==HIT_NO-1)	BreakTargetLock();
		}
	}else if(mModeTimer>HIT_TIME*HIT_NO+END_TIME){
		return 1;
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////
int CActCallEnemy::OnTimer()				//åƒÇ—èoÇµ
{
//íËå^ï∂////////////////////////////////////////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////////////////////////////////////////
	CBattleMode &bm=*mBattleMode;
	switch(mModeTimer){
		case 1:

			{
				ActiveCharacter()->ChangeAni(13*ANI_TYPE,16,true);
				PlayWave(SE_CALL);
				static int  enemyfield[BATTLE_SQUARE_Height][BATTLE_SQUARE_Height];
				memset(enemyfield,0,sizeof(enemyfield));

				int x=bm.mEnemy[0].GetSX();
				int y=bm.mEnemy[0].GetSY();
				enemyfield[x][y]=1;

				for(int i=0;i<MAX_ENEMY_NUMBER;i++){
					if(bm.mEnemy[i].GetDead()){
						bm.mEnemy[i].DeleteCharacter();
					}else{
						int x=bm.mEnemy[i].GetSX();
						int y=bm.mEnemy[i].GetSY();
						int size=bm.mEnemy[i].GetSize();
						for(int i=0;i<size;i++){
							for(int j=0;j<size;j++){
								enemyfield[x+i][y+j]=1;
							}
						}
					}
				}

				int count =0;
				for(int i=0;i<MAX_ENEMY_NUMBER-1;i++){
					if(bm.mEnemy[i].GetLive()){
						continue ;
					}
					
					int elv=MonthData[Flag.mCurrentMonth].GetBaseLv()-pInput->Rand(3) + mLv;
					int enemy=Flag.mEnemyType[pInput->Rand(Flag.mEnemyTypeNo)];
					bm.SetEnemy(i,enemy,elv,enemyfield,bm.mBattleOption);
					count++;
					if(count>=mMin){
						if(pInput->Rand(2) < 1)	break;
					}
					if(count>=mMax)	break;
				}
			}

			break;
			
		case 20:
			return 1;
			break;
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////
int CActSong::OnTimer()				//êlãõÇÃâÃ
{
//íËå^ï∂////////////////////////////////////////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////////////////////////////////////////

	const int END_TIME =80;
	switch(mModeTimer){
		case 1:
			ChangeFieldColor(mColor | 0x80000000,64);
			ActiveCharacter()->ChangeAni(13*ANI_TYPE,END_TIME,true);
			PlayWave(SE_SONG);
			break;
			
		case END_TIME:
			int no,*target;
			target=GetTarget(&no);

			for(int i=0;i<no;i++){
				int t=target[i]&0xffff;
				if(mST==100){
					HealHP(mActiveCharacter,t,2000,true);
				}else{
					SetStatus(mActiveCharacter,t,mST,100);
				}
			}
			BreakTargetLock();
			break;
		case END_TIME+32:
			return 1;
	}
	if(mModeTimer<END_TIME-32){
		for(int i=0;i<1;i++){
			int x=pInput->Rand(840)-100;
			int y=pInput->Rand(500);
			int size=pInput->Rand(40)+40;
			int life=128;

			double sx=size*0.02;
			if(!CheckEnemy(mActiveCharacter))	sx=-sx;;
			double sy=0;
			CBattleEffect *effect=GetTopEffect().SetEffect(
					BE_STRETCH_FADE_MOVE_NO_SPIN,
					x,y,
					BE_SPEED(sx,sy),71 + life*0x10000,
					BLEND_STANDARD,mColor|0xff000000);
			effect->OperationA(size,size);
		}
	}
	return 0;
}