#include "itemaction.h"
#include "input.h"

////////////////////////////////////////////////////////////////////////////
int CAct_128::OnTimer()				//ライフリカバリー
{
//定型文////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////
	if(mModeTimer==1){
		int no,*target;
		target=GetTarget(&no);

		GetTopEffect().SetEffect(BE_STANDARD,
			GetCharacter(ActiveCharacter()->GetTarget())->GetCX(),
			GetCharacter(ActiveCharacter()->GetTarget())->GetCY(),
			6,0,BLEND_ADD,0xff00ff00);

		int heal;
		ActiveCharacter()->GetUseItemParam(&heal,NULL);
		for(int i=0;i<no;i++){
			HealLP(	mActiveCharacter,
					target[i]&0xffff,
					heal);
			HealHP(	mActiveCharacter,
					target[i]&0xffff,
					GetCharacter(target[i]&0xffff)->GetMaxHP());
		}
		BreakTargetLock();
	}else if(mModeTimer>=40){
		return 1;
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////
int CAct_129::OnTimer()				//チャージスキル
{
//定型文////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////
	if(mModeTimer==1){
		int no,*target;
		target=GetTarget(&no);

		GetTopEffect().SetEffect(BE_STANDARD,
			GetCharacter(ActiveCharacter()->GetTarget())->GetCX(),
			GetCharacter(ActiveCharacter()->GetTarget())->GetCY(),
			6,0,BLEND_ADD,0xff0000ff);
		int heal,lv;
		ActiveCharacter()->GetUseItemParam(&heal,&lv);
		for(int i=0;i<no;i++){
			HealSP(	mActiveCharacter,
					target[i]&0xffff,
					lv,heal);
		}
		BreakTargetLock();
	}else if(mModeTimer>=40){
		return 1;
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////
int CAct_130::OnTimer()				//プラントボムＳ
{
//定型文////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////
	if(mModeTimer==1){
		int t=ActiveCharacter()->GetTarget();
		int i;
		for(i=0;i<16;i++){
			double x=(pInput->Rand(900)-450)/100.0;
			double y=(pInput->Rand(500)-600)/100.0;
			GetTopEffect().SetEffect(BE_FALL_PARTICUL,
						GetCharacter(t)->GetCX(),
						GetCharacter(t)->GetCY(),
						BE_SPEED(x,y),32*0x10000+10,
						BLEND_ADD);
		}

		int no,*target;
		target=GetTarget(&no);

		Attack(	mActiveCharacter,
				target,no,
				mPow,0,
				AT_TYPE_NO_RAND,
				AT_ELE_NONE,0,SE_FIRE);
		BreakTargetLock();
	}
	if(mModeTimer>=30){
		return 1;
	}

	return 0;
}
////////////////////////////////////////////////////////////////////////////
int CAct_131::OnTimer()				//プラントボムＬ
{
//定型文////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////
	const int TIME=16;
	const int SIZE=640;
	if(mModeTimer==1){
		int t=ActiveCharacter()->GetTarget();

		int no,*target;
		target=GetTarget(&no);

		mActionEffect[0]=GetTopEffect().SetEffect(
				BE_LARGE_STRETCH,
				GetCharacter(ActiveCharacter()->GetTarget())->GetCX(),
				GetCharacter(ActiveCharacter()->GetTarget())->GetGCY()+32,
				50,0);
		Attack(	mActiveCharacter,
				target,no,
				mPow,0,
				AT_TYPE_NO_RAND,
				AT_ELE_NONE,0,SE_FIRE);
		BreakTargetLock();
	}

	if(mActionEffect[0]){
		int t=mModeTimer-1;
		if(t>TIME){
			mActionEffect[0]->Destroy();
			mActionEffect[0]=NULL;
		}else{
			int size=t*SIZE/TIME;
			D3DCOLOR color;
			if(t<TIME/3){
				color=0xffffffff;
			}else{
				color=((0xff000000/TIME*3/2*(TIME-t)) & 0xff000000) + 0xffffff;
			}
			mActionEffect[0]->OperationA(size,size);
			mActionEffect[0]->SetColor(color);
		}
	}
	if(mModeTimer>=30){
		return 1;
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////
int CAct_132::OnTimer()				//ヒーリング
{
//定型文////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////
	if(mModeTimer<40){
		for(int i=0;i<2;i++){
			int x=pInput->Rand(640)+320;
			int y;
			if(x>640){
				y=x-640;
				x=640;
			}else{
				y=-32;
			}

			double sx=(-pInput->Rand(1500)-850)/100.0;
			double sy=(pInput->Rand(1000)+500)/100.0;
			GetTopEffect().SetEffect(BE_PARTICUL,
						x,y,BE_SPEED(sx,sy),32*0x10000+29);
		}
	}

	if(mModeTimer==20){

		int no,*target;
		target=GetTarget(&no);
		int turn;
		if(CheckEnemy(mActiveCharacter,ActiveCharacter()->GetTarget()))	turn=TURN_HORIZONTAL;
		else	turn=0;

		for(int i=0;i<no;i++){
			GetTopEffect().SetEffect(
				BE_FLASH_CHARACTER,
				0,0,
				target[i] & 0xffff,0,BLEND_SOLID_ADD,0xff00ff00);
			HealHP(mActiveCharacter,target[i] & 0xffff,mPow);
		}
		BreakTargetLock();
	}else if (mModeTimer>=60){
		return 1;
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////
int CAct_136::OnTimer()				//カウンターニードル
{
//定型文////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////
	if(mModeTimer==1){
		int no,*target;
		target=GetTarget(&no);

		int turn;
		if(CheckEnemy(mActiveCharacter,ActiveCharacter()->GetTarget()))	turn=TURN_HORIZONTAL;
		else	turn=0;

		mActionEffect[0]=GetTopEffect().SetEffect(
			BE_STRETCH,
			GetCharacter(ActiveCharacter()->GetTarget())->GetCX(),
			GetCharacter(ActiveCharacter()->GetTarget())->GetCY(),
			47+turn*0x10000,0+0*0x10000,
			BLEND_STANDARD,0xb0ffffff);

		for(int i=0;i<no;i++){
			SetStatus(mActiveCharacter,target[i]&0xffff,17,100,mPow);
		}
		BreakTargetLock();
	}else if(mModeTimer==48){
		return 1;
	}
	if(mActionEffect[0]){
		if(mModeTimer<=8){
			int size=mModeTimer*16;
			mActionEffect[0]->OperationA(size,size);
		}else if(mModeTimer<16){
		}else if(mModeTimer<32){
			mActionEffect[0]->SetColor(0x0b000000*(32-mModeTimer) | 0xffffff);
		}else{
			mActionEffect[0]->Destroy();
			mActionEffect[0]=NULL;
		}
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////
int CAct_137::OnTimer()				//ダブル
{
//定型文////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////
	if(mModeTimer==1){	
		int no,*target;
		target=GetTarget(&no);

		for(int i=0;i<no;i++){
			SetStatus(mActiveCharacter,target[i]&0xffff,19,100,mPow);
		}
		BreakTargetLock();
	}else if(mModeTimer==48){
		return 1;
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////
int CAct_139::OnTimer()				//エスケープ
{
//定型文////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////
	switch(mModeTimer){
		case 1:
			GetTopEffect().SetEffect(BE_FLASH,
							0,0,
							32,0,
							BLEND_ADD,0xffffffff);

			PlayWave(SE_FLASH);
			mBattleMode->mBattleOption=(mBattleMode->mBattleOption) | BOPT_ESCAPE_OK;
			mBattleMode->ChangeMode(BM_ESCAPE);
			return 1;
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////
int CAct_140::OnTimer()				//ガードブレイク
{
//定型文////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////
	if(mModeTimer==1){
		int no,*target;
		target=GetTarget(&no);

		int down;
		ActiveCharacter()->GetUseItemParam(&down,NULL);
		for(int i=0;i<no;i++){
			for(int j=0;j<8;j++){
				int x=pInput->Rand(33)-16;
				int y=pInput->Rand(33)-16;
				int sx=pInput->Rand(5)-2;
				int sy=pInput->Rand(3)-5;
				D3DCOLOR c=0x808080ff;
				GetTopEffect().SetEffect(BE_PARTICUL,
							GetCharacter(target[i]&0xffff)->GetCX()+x,
							GetCharacter(target[i]&0xffff)->GetCY()+48+y,
							BE_SPEED(sx,sy),24*0x10000+21,
							BLEND_STANDARD,c);
			}
			ParamChange(mActiveCharacter,target[i]&0xffff,UP_DEF,-down);
			ParamChange(mActiveCharacter,target[i]&0xffff,UP_MDEF,-down);
		}
		BreakTargetLock();
	}
	if(mModeTimer>=40){
		return 1;
	}
	return 0;
}