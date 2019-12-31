#include "magicaction.h"
#include "input.h"
////////////////////////////////////////////////////////////////////////////
int CActMagicShoot::OnTimer()				//マジックシュート
{
//定型文////////////////////////////////////////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////////////////////////////////////////

	double sx,sy;
	int target=ActiveCharacter()->GetTarget();
	double length=mBattleMode->GetShotSpeed(mActiveCharacter,target,32,&sx,&sy);

	if(mModeTimer==1){
		ActiveCharacter()->ChangeAni(13*ANI_TYPE,20,true);
		GetTopEffect().SetEffect(BE_MOVE,
						ActiveCharacter()->GetCX(),
						ActiveCharacter()->GetCY(),
						BE_SPEED(sx,sy),5,BLEND_ADD);
	}

	if(mModeTimer-1==int(length)){
		int no,*target;
		target=GetTarget(&no);

		Attack(	mActiveCharacter,
				target,no,
				100,MIN_ATK_LV_0,
				AT_TYPE_MAGIC,
				AT_ELE_NONE,0,SE_HIT);
		BreakTargetLock();
	}
	if(mModeTimer-35>=int(length)){
		return 1;
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////
int CAct_86::OnTimer()				//インパクト
{
//定型文////////////////////////////////////////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////////////////////////////////////////
	const int START = 24;
	const int SIZE = 320;
	const int TIME = 12;
	switch(mModeTimer){
		case 1:
			int t;
			t=ActiveCharacter()->GetTarget();
			for(int i=0;i<32;i++){
				int angle=pInput->Rand(360);
				int length=pInput->Rand(640)+200;

				GetTopEffect().SetEffect(
						BE_CONCENTRATE,
						GetCharacter(t)->GetCX(),
						GetCharacter(t)->GetCY(),
						angle+length*0x10000,21+(START-2)*0x10000,
						BLEND_ADD,0xffb0b0ff);
			}
			PlayWave(SE_PARAM_UP);
			break;

		case START:
			int no,*target;
			target=GetTarget(&no);

			mActionEffect[0] = GetBottomEffect().SetEffect(
				BE_LARGE_SPREAD,
				GetCharacter(ActiveCharacter()->GetTarget())->GetCX(),
				GetCharacter(ActiveCharacter()->GetTarget())->GetGCY()+32,
				22,SIZE + TIME*0x10000,BLEND_ADD);

			Attack(mActiveCharacter,
					target,no,
					160,MIN_ATK_LV_1,
					AT_TYPE_MAGIC,
					AT_ELE_NONE,0,-1);
			BreakTargetLock();

		case START+4:
			PlayWave(SE_WAVE);
			break;

		case START+TIME+20:
			return 1;	break;
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////
int CAct_87::OnTimer()				//レイ
{
//定型文////////////////////////////////////////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////////////////////////////////////////
	const int	 INTARVAL	=5;
	const int	 TIME		=INTARVAL*3;
	const int	 HIT_NO		=12;
	const int	 N			=TIME*TIME/2;
	const double DIV		=65536.0;

	if(mModeTimer==1){
		mActionParam[0]=new int[HIT_NO];
		mActionParam[1]=new int[HIT_NO];
		mActionParam[2]=new int[HIT_NO];
		memset(mActionParam[0],-1,sizeof(int)*HIT_NO);
	}

	for(int i=0;i<HIT_NO;i++){
		int time=mModeTimer-i*INTARVAL;
		if(time==2){
			mActionParam[0][i]=GetRandomTarget();

			double sx=(pInput->Rand(2000)+4000)/100.0;
			if(CheckEnemy(mActiveCharacter,mActionParam[0][i]))	sx=-sx;
			double sy=(pInput->Rand(10000)-5000)/100.0;
			int x=ActiveCharacter()->GetCX();
			int y=ActiveCharacter()->GetCY();
			
			int color[3];
			memset(color,0,sizeof(color));
			color[pInput->Rand(3)]=0x80;
			color[pInput->Rand(3)]=0xff;
			mActionEffect[i%ACTION_EFFECT_NO]=GetTopEffect().SetEffect(
											BE_TAIL_LINE,
											x,y,
											BE_SPEED(sx,sy),32+6*0x10000,
											BLEND_ADD,D3DCOLOR_ARGB(255,color[0],color[1],color[2]));

			double gx=GetCharacter(mActionParam[0][i])->GetCX()-x-sx*TIME;
			double gy=GetCharacter(mActionParam[0][i])->GetCY()-y-sy*TIME;

			mActionParam[1][i]=int((gx/N)*DIV);
			mActionParam[2][i]=int((gy/N)*DIV);
		}else if(2<time && time<2+TIME){
			double sx,sy;
			mActionEffect[i%ACTION_EFFECT_NO]->OperationDP(&sx,&sy);
			sx+=mActionParam[1][i]/DIV;
			sy+=mActionParam[2][i]/DIV;
			mActionEffect[i%ACTION_EFFECT_NO]->OperationD(sx,sy);
		}else if(time==2+TIME){
			int no,*target;
			target=GetTarget(
							mActionParam[0][i],
							4,&no);		//効果範囲は射るに依存！

			Attack(	mActiveCharacter,
					target,no,
					35,MIN_ATK_LV_2,
					AT_TYPE_MAGIC,
					AT_ELE_NONE,0,SE_SLASH);
			if(i==HIT_NO-1)		BreakTargetLock();
		}
	}

	if(mModeTimer>=INTARVAL*HIT_NO+TIME+20){
		delete[] mActionParam[0];
		delete[] mActionParam[1];
		delete[] mActionParam[2];
		mActionParam[0]=NULL;
		mActionParam[1]=NULL;
		mActionParam[2]=NULL;
		return 1;
	}

	return 0;
}
////////////////////////////////////////////////////////////////////////////
int CAct_88::OnTimer()				//ブラックホール
{
//定型文////////////////////////////////////////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////////////////////////////////////////
	const int TIME = 120;
	int center=ActiveCharacter()->GetTarget();
	
	if(mModeTimer==1){
		PlayWave(SE_BLACK_HOLE);
		int no,*target;
		target=GetTarget(&no);
		int x=GetCharacter(center)->GetCX();
		int y=GetCharacter(center)->GetCY();
		for(int i=0;i<no;i++){
			int t=target[i]&0xffff;
			int size=GetCharacter(center)->GetSize()*64;
			GetCharacter(t)->SetGoal(x-size,y-size,TIME+40,false);
		}
	}else if(mModeTimer==TIME-1){
		GetTopEffect().SetEffect(BE_FLASH,
			0,0,32,0,BLEND_STANDARD,0xffffffff);
		int no,*target;
		target=GetTarget(&no);
		for(int i=0;i<no;i++){
			int t=target[i]&0xffff;
			int size=GetCharacter(center)->GetSize()*64;
			GetCharacter(t)->Return(1,false,true);
		}
	}else if(mModeTimer==TIME){
		int no,*target;
		target=GetTarget(&no);

		Attack(mActiveCharacter,
				target,no,
				400,MIN_ATK_LV_3,
				AT_TYPE_MAGIC,
				AT_ELE_NONE,0,-1);
		BreakTargetLock();
	}else if(mModeTimer==TIME+30){
		return 1;
	}

	if(mModeTimer<TIME){
		if(pInput->Rand(16)==0){
			GetTopEffect().SetEffect(BE_FLASH,
				0,0,4,0,BLEND_ADD,0xff808080);
		}
		if(mModeTimer%DAMAGE_TIMER==1){
			int no,*target;
			target=GetTarget(&no);
			for(int i=0;i<no;i++){
				int t=target[i]&0xffff;;
				GetCharacter(t)->SetDamageEffect(false);
			}
		}
		int size=(TIME-mModeTimer)*480/TIME;
		int angle=mModeTimer*2;
		CBattleEffect* effect;
		if(mModeTimer%2==0){
			effect=&GetBottomEffect();
		}else{
			effect=&GetTopEffect();
		}
		mActionEffect[0] = effect->SetEffect(
			BE_BLACK_HOLE,
			GetCharacter(center)->GetCX(),
			GetCharacter(center)->GetCY(),
			58+angle*0x10000,size+mModeTimer*0x10000,BLEND_SUB,0xff282828);
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////
int CAct_91::OnTimer()				//エンシャントルーン
{
//定型文////////////////////////////////////////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////////////////////////////////////////
	const int EFFECT_TIME	= 128;
	const int INTERVAL		= 10;

	int target=ActiveCharacter()->GetTarget();
	if(mModeTimer==1){
		ChangeFieldColor(0xb0000000,32);
		ActiveCharacter()->ChangeAni(13*ANI_TYPE,EFFECT_TIME+16,true);
		for(int i=0;i<8;i++){
			mActionEffect[i]=GetTopEffect().SetEffect(
								BE_SMALL_ANIMATION,
								GetCharacter(target)->GetCX(),
								GetCharacter(target)->GetCY(),
								74,0,BLEND_ADD,0xff00ff00);
			mActionEffect[i]->OperationA(pInput->Rand(16),0);
			
			int color[3];
			memset(color,0,sizeof(color));
			color[pInput->Rand(3)]=0x80;
			color[pInput->Rand(3)]=0xff;
			mActionEffect[i]->SetColor(D3DCOLOR_ARGB(255,color[0],color[1],color[2]));
		}
	}else if(mModeTimer<EFFECT_TIME){
	
		if(mModeTimer%48==20){
			PlayWave(SE_RIVIVAL);
		}
	}else if(mModeTimer>=EFFECT_TIME && mModeTimer<EFFECT_TIME+INTERVAL*4 && (mModeTimer-EFFECT_TIME)%INTERVAL==0){

		ChangeFieldColor(0x00000000,1);
		int num= (mModeTimer-EFFECT_TIME)/INTERVAL;
		int no,*target;
		target=GetTarget(&no);
		for(int i=0;i<2;i++){
			int effect=i+num*2;
			for(int i=0;i<32;i++){
				GetTopEffect().SetEffect(BE_PARTICUL,
					pInput->Rand(640),
					pInput->Rand(640)-160,
					BE_SPEED(0,pInput->Rand(100)*0.01+0.5),26+64*0x10000,BLEND_ADD,mActionEffect[effect]->GetColor());
			}
			mActionEffect[effect]->Destroy();
			mActionEffect[effect]=NULL;
		}
		GetTopEffect().SetEffect(
			BE_FLASH,
			0,0,
			8,8,BLEND_STANDARD,0xffffffff);

		int mag=ActiveCharacter()->GetMagAtk();
		for(int i=0;i<no;i++){
			int t=target[i]&0xffff;
			switch(num){
				case 0:
					ParamChange(mActiveCharacter,t,UP_ATK,mag/2);
					ParamChange(mActiveCharacter,t,UP_MATK,mag/2);
					ParamChange(mActiveCharacter,t,UP_MDEF,mag/2);
					ParamChange(mActiveCharacter,t,UP_DEF,mag/2);
					break;
				case 1:
					SetStatus(mActiveCharacter,t,15,100);
					break;
				case 2:
					SetStatus(mActiveCharacter,t,11,100);
					break;
				case 3:
					SetStatus(mActiveCharacter,t,24,100);
					BreakTargetLock();
					break;
			}
			
			GetTopEffect().SetEffect(
				BE_FLASH_CHARACTER,
				0,0,
				target[i] & 0xffff,0,BLEND_SOLID_ADD,0xff00ff00);
		}
	}else if(mModeTimer>=EFFECT_TIME+INTERVAL*6){
		return 1;
	}

	int centerx;
	const int centery=80;
	if(CheckEnemy(mActiveCharacter,target))	centerx=480;
	else									centerx=160;
	for(int i=0;i<8;i++){
		if(mActionEffect[i]){
			int time=mModeTimer;
			if(time>EFFECT_TIME)	time=EFFECT_TIME;
			const int EFFECT_X =320;
			const int EFFECT_Y =240;
			const double EFFECT_Z =0.01;
			double rad=mModeTimer*mModeTimer*0.0012+i*3.141592*2/8;
			double size=(double)(EFFECT_TIME-time)/EFFECT_TIME;
			if(size<0.1)	size=0.1;
			mActionEffect[i]->SetPos(
				centerx+int(cos(rad)*EFFECT_X*size),
				centery+int(sin(rad)*EFFECT_Y*size - EFFECT_Z*time*time+160)
				);
			D3DCOLOR color=mActionEffect[i]->GetColor();
			if(mModeTimer<64){
				color=(color & 0x00ffffff) + mModeTimer*4*0x01000000;
				mActionEffect[i]->SetColor(color);
			}

			double sx=(pInput->Rand(300)-150)/100.0;

			GetTopEffect().SetEffect(BE_PARTICUL,
						int(mActionEffect[i]->GetX()),
						int(mActionEffect[i]->GetY()-16),
						BE_SPEED(sx,4),16*0x10000+26,
						BLEND_ADD,color);
			if(mModeTimer%6==0){
				PlayWave(SE_STAR);
			}
		}
	}
	return 0;
}