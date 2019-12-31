#include "swordaction.h"
#include "input.h"

////////////////////////////////////////////////////////////////////////////
int CAct_8::OnTimer()				//水平切り
{
//定型文////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////
	switch(mModeTimer){
		case 1:
			ActiveCharacter()->ChangeAni(10*ANI_TYPE+1,16,true);
			break;
		case 10:
			int no,*target;
			target=GetTarget(&no);

			ActiveCharacter()->ChangeAni(11*ANI_TYPE+1,12,true);

			int turn;
			if(CheckEnemy(mActiveCharacter))	turn=0;
			else	turn=TURN_HORIZONTAL;

			GetTopEffect().SetEffect(BE_STANDARD,
				GetCharacter(ActiveCharacter()->GetTarget())->GetCX(),
				GetCharacter(ActiveCharacter()->GetTarget())->GetCY(),
				0,turn | TURN_VERTICAL,BLEND_ADD);

			Attack(	mActiveCharacter,
					target,no,
					160,MIN_ATK_LV_0,
					AT_TYPE_WEAPON,
					AT_ELE_NONE,0,SE_SLASH);
			BreakTargetLock();

			break;
		case 13:
			ActiveCharacter()->ChangeAni(9*ANI_TYPE+1,18,true);
			break;

		case 35:
			return 1;	break;
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////
int CAct_9::OnTimer()				//ダブルスラッシュ
{
//定型文////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////

	int turn;
	if(mModeTimer>10)	turn=1;	else turn=0;
	switch(mModeTimer){
		case 19:
			BreakTargetLock();
		case 1:
			ActiveCharacter()->ChangeAni(9*ANI_TYPE+turn,18,true);
			break;
		case 4:
		case 16:
			ActiveCharacter()->ChangeAni(11*ANI_TYPE+turn,12,true);

			int no,*target;
			target=GetTarget(&no);

			int t;
			if((CheckEnemy(mActiveCharacter))^(turn!=0))	t=TURN_HORIZONTAL;
			else	t=0;
			if(turn) t=t|TURN_VERTICAL;
			GetTopEffect().SetEffect(BE_STANDARD,
				GetCharacter(ActiveCharacter()->GetTarget())->GetCX(),
				GetCharacter(ActiveCharacter()->GetTarget())->GetCY(),
				0,t,BLEND_ADD);

			Attack(	mActiveCharacter,
					target,no,
					90,MIN_ATK_LV_1,
					AT_TYPE_WEAPON,
					AT_ELE_NONE,0,SE_SLASH);

			break;
		case 7:
		case 13:
			ActiveCharacter()->ChangeAni(10*ANI_TYPE+turn,8,true);
			break;

		case 40:
			return 1;	break;
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////
int CAct_10::OnTimer()				//瞬撃
{
//定型文////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////

	switch(mModeTimer){
		case 1:
			ActiveCharacter()->ChangeAni(9*ANI_TYPE,8,true);
			break;
		case 5:
			int no,*target;
			target=GetTarget(&no);

			ActiveCharacter()->ChangeAni(11*ANI_TYPE,12,true);

			int turn;
			if(CheckEnemy(mActiveCharacter))	turn=TURN_HORIZONTAL;
			else	turn=0;
			GetTopEffect().SetEffect(BE_STANDARD,
				GetCharacter(ActiveCharacter()->GetTarget())->GetCX(),
				GetCharacter(ActiveCharacter()->GetTarget())->GetCY(),
				0,turn,BLEND_ADD);

			Attack(	mActiveCharacter,
					target,no,
					150,MIN_ATK_LV_1,
					AT_TYPE_WEAPON,
					AT_ELE_NONE,0,SE_SLASH);

			ActiveCharacter()->SetGoal(
				ActiveCharacter()->GetX()+(turn?100:-100),
				ActiveCharacter()->GetY(),
				2,false);

			BreakTargetLock();
			break;
		case 7:
			ActiveCharacter()->ChangeAni(10*ANI_TYPE,8,true);
			break;

		case 15:
			return 1;	break;
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////
int CAct_12::OnTimer()				//トライスラスト
{
//定型文////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////

	int no,*target;
	int turn;
	target=GetTarget(&no);

	bool dmg=false;
	switch(mModeTimer){
		case 1:
			ActiveCharacter()->ChangeAni(9*ANI_TYPE+2,16,true);
			break;
		case 14:
			ActiveCharacter()->SetTurn(false);
		case 4:
			ActiveCharacter()->ChangeAni(12*ANI_TYPE+1,16,true);

			if(CheckEnemy(mActiveCharacter))	turn=TURN_HORIZONTAL;
			else	turn=0;
			if(mModeTimer>10){
				turn=turn^TURN_HORIZONTAL;
			}
			GetTopEffect().SetEffect(BE_STANDARD,
				GetCharacter(ActiveCharacter()->GetTarget())->GetCX(),
				GetCharacter(ActiveCharacter()->GetTarget())->GetCY(),
				7,turn,BLEND_ADD);

			ActiveCharacter()->SetGoal(
				ActiveCharacter()->GetX()+(turn?80:-80)*GetCharacter(ActiveCharacter()->GetTarget())->GetSize(),
				ActiveCharacter()->GetY(),
				2,false);
			dmg=true;
			

			break;
		case 17:
			ActiveCharacter()->SetTurn(true);
		case 7:
			ActiveCharacter()->ChangeAni(11*ANI_TYPE+2,20,true);
			break;

		case 10:
			ActiveCharacter()->SetTurn(true);
			ActiveCharacter()->ChangeAni(9*ANI_TYPE+2,16,true);
			break;
		case 20:
			ActiveCharacter()->SetTurn(false);
			ActiveCharacter()->ChangeAni(9*ANI_TYPE,8,true);
			break;
		case 24:
			ActiveCharacter()->ChangeAni(11*ANI_TYPE,12,true);

			if(CheckEnemy(mActiveCharacter))	turn=TURN_HORIZONTAL;
			else	turn=0;
			GetTopEffect().SetEffect(BE_STANDARD,
				GetCharacter(ActiveCharacter()->GetTarget())->GetCX(),
				GetCharacter(ActiveCharacter()->GetTarget())->GetCY(),
				0,turn,BLEND_ADD);
			dmg=true;
			BreakTargetLock();
			break;
		case 27:
			ActiveCharacter()->ChangeAni(10*ANI_TYPE,16,true);
			break;
		case 50:
			return 1;	break;

	}

	if(dmg){
		Attack(	mActiveCharacter,
				target,no,
				100,MIN_ATK_LV_2,
				AT_TYPE_WEAPON,
				AT_ELE_NONE,0,SE_SLASH);
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////
int CAct_13::OnTimer()				//閃光の太刀
{
//定型文////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////
	switch(mModeTimer){
		case 1:
			ActiveCharacter()->ChangeAni(10*ANI_TYPE+1,32,true);
			break;
		case 15:
			ActiveCharacter()->ChangeAni(11*ANI_TYPE+1,32,true);
			break;
		case 30:
			ActiveCharacter()->ChangeAni(9*ANI_TYPE+1,64,true);
			break;
		case 60:
			int no,*target,*hit;
			target=GetTarget(&no);

			ActiveCharacter()->ChangeAni(10*ANI_TYPE,32,true);

			int turn;
			if(CheckEnemy(mActiveCharacter))	turn=TURN_HORIZONTAL;
			else	turn=0;

			mActionEffect[0]=GetTopEffect().SetEffect(
				BE_STRETCH,
				GetCharacter(ActiveCharacter()->GetTarget())->GetCX(),GetCharacter(ActiveCharacter()->GetTarget())->GetCY(),
				32+turn*0x10000,128+128*0x10000);
			GetTopEffect().SetEffect(BE_FLASH,
				0,0,8,0,BLEND_STANDARD,0xffffffff);

			hit=Attack(	mActiveCharacter,
					target,no,
					240,MIN_ATK_LV_2+50,
					AT_TYPE_WEAPON^AT_AVOID,
					AT_ELE_NONE,0,SE_HARD_SLASH);

			for(int i=0;i<no;i++){
				if(hit[i]<0)	continue;
				SetStatus(mActiveCharacter,target[i]&0xffff,7,150);
			}

			ActiveCharacter()->SetGoal(
				ActiveCharacter()->GetX()+(turn?100:-100),
				ActiveCharacter()->GetY(),
				1,false);
			BreakTargetLock();
			break;
		case 95:
			return 1;
	}

	if(mActionEffect[0]){
		int t=mModeTimer-60;
		if(t>=32){
			mActionEffect[0]->Destroy();
			mActionEffect[0]=NULL;
		}else{
			mActionEffect[0]->SetColor(0xffffffff-0x08000000*t);
		}
	}	
	return 0;
}
////////////////////////////////////////////////////////////////////////////
int CAct_14::OnTimer()				//ブレードカルテット
{
//定型文////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////
	int no,*target,*hit;
	int turn;
	target=GetTarget(&no);

	const int MOVE_X	=80;
	const int MOVE_Y	=80;
	const int MOVE_TIME =6;

	ActiveCharacter()->SetSpeedZ(-MOVE_Y/MOVE_TIME);
	int dmg=-1;
	switch(mModeTimer){
		case 1:
			ActiveCharacter()->ChangeAni(10*ANI_TYPE+1,16,true);
			break;
		case 4:
			ActiveCharacter()->ChangeAni(11*ANI_TYPE+1,16,true);

			if(CheckEnemy(mActiveCharacter))	turn=0;
			else	turn=TURN_HORIZONTAL;

			GetTopEffect().SetEffect(BE_STANDARD,
				GetCharacter(ActiveCharacter()->GetTarget())->GetCX(),
				GetCharacter(ActiveCharacter()->GetTarget())->GetCY(),
				0,turn|TURN_VERTICAL,BLEND_ADD);

			ActiveCharacter()->SetGoal(
				ActiveCharacter()->GetX()+(turn?-MOVE_X:MOVE_X)*GetCharacter(ActiveCharacter()->GetTarget())->GetSize(),
				ActiveCharacter()->GetY(),
				2,false);
			dmg=25;
			

			break;
		case 7:
			ActiveCharacter()->ChangeAni(9*ANI_TYPE+1,20,true);
			break;

		case 28:
			ActiveCharacter()->SetTurn(false);
		case 20:
			ActiveCharacter()->ChangeAni(9*ANI_TYPE+0,16,true);
			ActiveCharacter()->SetPos(
				ActiveCharacter()->GetX(),
				ActiveCharacter()->GetY(),
				MOVE_Y);
			int move;
			if(CheckEnemy(mActiveCharacter))	move=MOVE_X;
			else	move=-MOVE_X;
			if(mModeTimer<25){
				move=-move;
			}
			ActiveCharacter()->SetGoal(
				ActiveCharacter()->GetX()+move*GetCharacter(ActiveCharacter()->GetTarget())->GetSize(),
				ActiveCharacter()->GetY(),
				MOVE_TIME,false);
			break;
		case 30:
		case 22:
			ActiveCharacter()->ChangeAni(11*ANI_TYPE,12,true);

			if(CheckEnemy(mActiveCharacter))	turn=TURN_HORIZONTAL;
			else	turn=0;
			if(mModeTimer<25)	turn=turn^TURN_HORIZONTAL;
			GetTopEffect().SetEffect(BE_STANDARD,
				GetCharacter(ActiveCharacter()->GetTarget())->GetCX(),
				GetCharacter(ActiveCharacter()->GetTarget())->GetCY(),
				0,turn,BLEND_ADD);
			dmg=16;
			break;
		case 32:
		case 24:
			ActiveCharacter()->ChangeAni(10*ANI_TYPE,16,true);
			break;
			
		case 20+MOVE_TIME:
				ActiveCharacter()->SetTurn(true);
				break;
		case 40:
				ActiveCharacter()->SetTurn(true);
				ActiveCharacter()->ChangeAni(10*ANI_TYPE+1,32,true);
				break;
		case 54:
			ActiveCharacter()->SetTurn(false);
			ActiveCharacter()->ChangeAni(11*ANI_TYPE+1,16,true);

			if(CheckEnemy(mActiveCharacter))	turn=TURN_HORIZONTAL;
			else	turn=0;

			GetTopEffect().SetEffect(BE_FLASH,
				0,0,8,0,BLEND_STANDARD,0xffffffff);

			ActiveCharacter()->SetGoal(
				ActiveCharacter()->GetX()+(turn?-MOVE_X:MOVE_X)*GetCharacter(ActiveCharacter()->GetTarget())->GetSize(),
				ActiveCharacter()->GetY(),
				2,false);

			Attack(	mActiveCharacter,
				target,no,
				120,MIN_ATK_LV_3,
				AT_TYPE_WEAPON,
				AT_ELE_NONE,0,SE_HARD_SLASH);

			BreakTargetLock();
			break;
		case 56:
			ActiveCharacter()->SetTurn(true);
			ActiveCharacter()->ChangeAni(9*ANI_TYPE+1,24,true);
			break;
		case 56+25:
			ActiveCharacter()->SetTurn(false);
			break;

		case 90:
			return 1;	break;

	}

	if(dmg>=0){
		hit=Attack(	mActiveCharacter,
				target,no,
				120,MIN_ATK_LV_3,
				AT_TYPE_WEAPON,
				AT_ELE_NONE,0,SE_SLASH);
		for(int i=0;i<no;i++){
			if(hit[i]>0){
				GetCharacter(target[i] & 0xffff)->SetGoal(
						GetCharacter(target[i] & 0xffff)->GetX(),
						GetCharacter(target[i] & 0xffff)->GetY(),
						dmg,true);
			}
		}
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////
int CAct_15::OnTimer()				//剣の舞
{
//定型文////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////
	const int HIT=8;

	const int INTERVAL=8;
	const int STIME=3;
	const int MTIME=5;
	
	const int EFFECT_TIME=16;
	const int EFFECT_SIZE=1600;

	if(mModeTimer==1){
		mActionParam[0]=new int;
		mActionParam[2]=new int;
	}
	int effect=(mModeTimer/INTERVAL)%ACTION_EFFECT_NO;
	if( mModeTimer<INTERVAL*HIT){
		const int Animation[3][3]={
			{9*ANI_TYPE+2	,12*ANI_TYPE+1	,11*ANI_TYPE+2	},
			{10*ANI_TYPE+1	,11*ANI_TYPE+1	,9*ANI_TYPE+1	},
			{9*ANI_TYPE		,11*ANI_TYPE	,10*ANI_TYPE	}
		};
		int no,*target;
		int ani=(mModeTimer/INTERVAL)%3;
		switch(mModeTimer%INTERVAL){
			case 1:
				*mActionParam[0]=GetRandomTarget();

				*mActionParam[2]=pInput->Rand(314*2);
				int ex,ey,size;
				size=GetCharacter(*mActionParam[0])->GetSize()-ActiveCharacter()->GetSize();
				ex=GetCharacter(*mActionParam[0])->GetX()+size*32;
				ey=GetCharacter(*mActionParam[0])->GetY()+size*32;
				ActiveCharacter()->SetPos(
					int(ex+cos(*mActionParam[2]*0.01)*120),
					int(ey+sin(*mActionParam[2]*0.01)*120),
					0);
				ActiveCharacter()->SetGoal(
					int(ex-cos(*mActionParam[2]*0.01)*120*(MTIME-STIME)/STIME),
					int(ey-sin(*mActionParam[2]*0.01)*120*(MTIME-STIME)/STIME),
					MTIME,false);
				break;
			case STIME-1:
				ActiveCharacter()->ChangeAni(Animation[ani][0],8,true);
				break;
			case STIME:
				ActiveCharacter()->ChangeAni(Animation[ani][1],8,true);
				target=GetTarget(
								*mActionParam[0],
								4,&no);		//効果範囲は「射る」に依存！

				Attack(	mActiveCharacter,
					target,no,
					60,MIN_ATK_LV_3-30,
					AT_TYPE_WEAPON,
					AT_ELE_NONE,0,SE_SLASH);

				mActionEffect[effect]=GetTopEffect().SetEffect(
					BE_STRETCH_ROTATE,
					GetCharacter(*mActionParam[0])->GetCX(),
					GetCharacter(*mActionParam[0])->GetCY(),
					17+(*mActionParam[2]*360/314/2+90)*0x10000,128+0*0x10000,BLEND_ADD);

				break;
			case STIME+1:
				ActiveCharacter()->ChangeAni(Animation[ani][2],EFFECT_TIME+10,true);
				break;
			case MTIME:
				bool movex;
				movex=(cos(*mActionParam[2]*0.01)>0);
				ActiveCharacter()->SetTurn(CheckEnemy(mActiveCharacter) == movex);
		}
	}else if(mModeTimer==INTERVAL*HIT){
		BreakTargetLock();
	}else if(mModeTimer>=INTERVAL*HIT+EFFECT_TIME+20){
		delete mActionParam[0];
		delete mActionParam[2];
		mActionParam[0]=NULL;
		mActionParam[2]=NULL;
		return 1;
	}

	for(int i=0;i<ACTION_EFFECT_NO;i++){
		int e=(effect-i+ACTION_EFFECT_NO)%ACTION_EFFECT_NO;
		if(mActionEffect[e]){
			int time=mModeTimer%INTERVAL+i*INTERVAL;
			if(time<EFFECT_TIME){
				mActionEffect[e]->OperationA(128,time*EFFECT_SIZE/EFFECT_TIME);
				if(time>EFFECT_TIME/2){
					mActionEffect[e]->SetColor((0xff*(EFFECT_TIME-time)/(EFFECT_TIME/2))*0x01000000|0xffffff);
				}
			}else{
				mActionEffect[e]->Destroy();
				mActionEffect[e]=NULL;
			}
		}
	}

	return 0;
}
////////////////////////////////////////////////////////////////////////////
CAct_SevenStars::CAct_SevenStars(int pow,int min,int hit,int opt,int img,int once,int se)
{
	mPow=pow;
	mMin=min;
	mHit=hit;
	mOpt=opt;
	mImg=img;
	mOnce=once;
	mSE=se;
}
int CAct_SevenStars::OnTimer()		//セブンスターズ・ルナティックブラッド
{
//定型文////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////
	const int EFFECT_TIME	=16;
	const int EFFECT_SIZE	=1600;
	const int BEFORE_TIME	=180;
	const int MOVE_X		=80;

	const int Animation[3]={
		9*ANI_TYPE+2	,10*ANI_TYPE+1	,9*ANI_TYPE	
	};

	if(mModeTimer==1){
		ChangeFieldColor(0x80000000,BEFORE_TIME);
	}
	int target=ActiveCharacter()->GetTarget();
	bool turn=CheckEnemy(mActiveCharacter);
	if(mModeTimer<BEFORE_TIME){
		int t=16;
		if(mModeTimer>40)	t=12;
		if(mModeTimer>60)	t=8;
		if(mModeTimer>80)	t=6;
		if(mModeTimer>100)	t=4;
		if(mModeTimer>120)	t=2;
		if(mModeTimer>140)	t=1;

		if(mModeTimer%t==0 || mModeTimer==1){
			int ex,ey;
			ActiveCharacter()->ChangeAni(1*ANI_TYPE,2,true);
			ex=GetCharacter(target)->GetCX();
			ey=GetCharacter(target)->GetCY();
			double rad=pInput->Rand(mHit)*3.141592*2/mHit;
			ActiveCharacter()->SetPos(
				int(ex+cos(rad)*MOVE_X)-64,
				int(ey+sin(rad)*MOVE_X)-64,
				0);
			if(3.14/2<rad && rad<3.14/2*3)	turn=!turn;
				
			ActiveCharacter()->SetTurn(turn);
			PlayWave(SE_STEP);
		}else{
			ActiveCharacter()->SetPos(
				9999,
				9999,
				0);
		}

	}else if(mModeTimer==BEFORE_TIME){
		ChangeFieldColor(0x00000000,1);
		ActiveCharacter()->SetTurn(false);
		ActiveCharacter()->ChangeAni(10*ANI_TYPE,32,true);

		int ex,ey,size;
		size=GetCharacter(target)->GetSize()-ActiveCharacter()->GetSize();
		ex=GetCharacter(target)->GetX()+size*32;
		ey=GetCharacter(target)->GetY()+size*32;
		ActiveCharacter()->SetPos(
			int(ex+(turn?MOVE_X*1.5:-MOVE_X*1.5)),
			ey,
			0);
		GetTopEffect().SetEffect(BE_FLASH,
			0,0,16,0,BLEND_STANDARD,0xffffffff);
	}
	if(BEFORE_TIME<= mModeTimer && mModeTimer<BEFORE_TIME+mHit/mOnce){
		int *target,no;
		target=GetTarget(&no);
		int t=mModeTimer-BEFORE_TIME;
		for(int i=0;i<mOnce;i++){
			int type=AT_TYPE_WEAPON;
			if(!mOpt)	type=type^AT_AVOID;
			Attack(	mActiveCharacter,
				target,no,
				mPow,mMin,
				type,
				AT_ELE_NONE,mOpt,mSE);
			int angle=(i+t*mOnce)*360/mHit;
			GetTopEffect().SetEffect(
				BE_TURN_SPREAD_SLASH,
				GetCharacter(target[0]&0xffff)->GetCX(),
				GetCharacter(target[0]&0xffff)->GetCY(),
				mImg+angle*0x10000,EFFECT_SIZE+EFFECT_TIME*0x10000,BLEND_ADD);
		}
		PlayWave(mSE);
	}
	if(mModeTimer==BEFORE_TIME+mHit/mOnce){
		BreakTargetLock();
	}

	if(mModeTimer>=BEFORE_TIME+40){
		return 1;
	}
	return 0;
}