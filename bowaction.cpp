#include "bowaction.h"
#include "input.h"

////////////////////////////////////////////////////////////////////////////
int CAct_39::OnTimer()				//バードショット
{
//定型文////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////
	double sx,sy;
	int target=ActiveCharacter()->GetTarget();
	double length=mBattleMode->GetShotSpeed(mActiveCharacter,target,48,&sx,&sy);

	switch(mModeTimer){
		case 1:
			ActiveCharacter()->ChangeAni(12*ANI_TYPE,16,true);
			break;
		case 8:
			PlayWave(SE_BOW);
			GetTopEffect().SetEffect(BE_MOVE,
							ActiveCharacter()->GetCX(),
							ActiveCharacter()->GetCY(),
							BE_SPEED(sx,sy),19,BLEND_ADD);
			ActiveCharacter()->ChangeAni(9*ANI_TYPE+3,35,true);
			break;

	}

	if(mModeTimer-8==int(length) && !mActionParam[0]){
		int no,*target;
		target=GetTarget(&no);

		int *dmg=Attack(
					mActiveCharacter,
					target,no,
					170,MIN_ATK_LV_1,
					AT_TYPE_BOW,
					AT_ELE_NONE,AT_OPT_ANTI_AIR,SE_SHOOT);

		mActionParam[0]=(int*)(true);

		for(int i=0;i<no;i++){
			if(dmg[i]>0){
				GetCharacter(target[i] & 0xffff)->SetGoal(
						GetCharacter(target[i] & 0xffff)->GetX(),
						GetCharacter(target[i] & 0xffff)->GetY(),
						30,true
						);
			}
		}
		BreakTargetLock();
	}
	if(mModeTimer-40>=int(length)){
		return 1;
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////
int CAct_40::OnTimer()				//毒矢
{
//定型文////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////

	double sx,sy;
	int target=ActiveCharacter()->GetTarget();
	double length=mBattleMode->GetShotSpeed(mActiveCharacter,target,48,&sx,&sy);


	switch(mModeTimer){
		case 1:
			ActiveCharacter()->ChangeAni(6*ANI_TYPE,16,true);
			break;
		case 21:
			ActiveCharacter()->ChangeAni(12*ANI_TYPE,16,true);
			break;
		case 24:
			PlayWave(SE_BOW);
			GetTopEffect().SetEffect(BE_MOVE,
							ActiveCharacter()->GetCX(),
							ActiveCharacter()->GetCY(),
							BE_SPEED(sx,sy),4,BLEND_ADD);
			ActiveCharacter()->ChangeAni(9*ANI_TYPE+3,30,true);
			break;

	}

	if(mModeTimer-24==int(length)){
		int no,*target,*hit;
		target=GetTarget(&no);

		hit=Attack(	mActiveCharacter,
				target,no,
				150,MIN_ATK_LV_1,
				AT_TYPE_BOW,
				AT_ELE_NONE,0,SE_SHOOT);

		for(int i=0;i<no;i++){
			if(hit[i]<0)	continue;
			for(int j=0;j<8;j++){
				int x=pInput->Rand(33)-16;
				int y=pInput->Rand(33)-16;
				int sx=pInput->Rand(5)-2;
				int sy=pInput->Rand(3)-5;
				D3DCOLOR c=StatusData[0].mColor+pInput->Rand(63)*0x000100;
				GetTopEffect().SetEffect(BE_PARTICUL,
							GetCharacter(target[i]&0xffff)->GetCX()+x,
							GetCharacter(target[i]&0xffff)->GetCY()+48+y,
							BE_SPEED(sx,sy),24*0x10000+21,
							BLEND_STANDARD,c);
			}
			SetStatus(mActiveCharacter,target[i]&0xffff,0,150);
		}
		BreakTargetLock();
	}
	if(mModeTimer-50>=int(length)){
		return 1;
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////
int CAct_41::OnTimer()				//マルチプルショット
{
//定型文////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////
	const int	 INTARVAL	=2;
	const int	 TIME		=INTARVAL*3;
	const int	 HIT_NO		=8;

	double sx,sy;
	int target=ActiveCharacter()->GetTarget();
	double length=mBattleMode->GetShotSpeed(mActiveCharacter,target,48,&sx,&sy);

	for(int i=0;i<HIT_NO;i++){
		int time=(mModeTimer-1)-i*INTARVAL;
		switch(time){
			case 0:
				ActiveCharacter()->ChangeAni(12*ANI_TYPE,16,true);
				break;
			case 1:
				PlayWave(SE_BOW);
				double at,len;
				at=atan2(sy,sx);
				len=hypot(sx,sy);
				at=at+(pInput->Rand(100)-50)*0.002;
				sx=cos(at)*len;
				sy=sin(at)*len;
				GetTopEffect().SetEffect(BE_MOVE,
								ActiveCharacter()->GetCX(),
								ActiveCharacter()->GetCY(),
								BE_SPEED(sx,sy),4,BLEND_ADD);
				ActiveCharacter()->ChangeAni(9*ANI_TYPE+3,20,true);
				break;

		}

		if(time-4==int(length)){
			int no,*target;
			target=GetTarget(&no);

			Attack(	mActiveCharacter,
					target,no,
					40,MIN_ATK_LV_1,
					AT_TYPE_BOW,
					AT_ELE_NONE,AT_OPT_HALF_HIT,SE_SHOOT);
			if(i==HIT_NO-1)	BreakTargetLock();
		}
	}
	if(mModeTimer-HIT_NO*INTARVAL-50>=int(length)){
		return 1;
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////
int CAct_42::OnTimer()				//スターダストアロー
{
//定型文////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////
	double sx,sy;
	int target=ActiveCharacter()->GetTarget();
	double length=mBattleMode->GetShotSpeed(mActiveCharacter,target,32,&sx,&sy);

	switch(mModeTimer){
		case 1:
		case 21:
		case 31:
			ActiveCharacter()->ChangeAni(12*ANI_TYPE,16,true);
			break;
		case 14:
			sy+=int(length+2)*4;
		case 24:
			sy-=int(length+2)*2;
		case 34:
			PlayWave(SE_BOW);
			int color[3];
			memset(color,0,sizeof(color));
			color[pInput->Rand(3)]=0x80;
			color[pInput->Rand(3)]=0xff;

			mActionEffect[mModeTimer/10-1]=GetTopEffect().SetEffect(
							BE_TAIL_MOVE,
							ActiveCharacter()->GetCX(),
							ActiveCharacter()->GetCY(),
							BE_SPEED(sx,sy),36,
							BLEND_ADD,D3DCOLOR_ARGB(255,color[0],color[1],color[2]));
			ActiveCharacter()->ChangeAni(9*ANI_TYPE+3,30,true);
			break;

	}
	if(mActionEffect[0]){
		int x,y;
		mActionEffect[0]->OperationP(&x,&y);
		mActionEffect[0]->OperationA(x,y-4);
	}
	if(mActionEffect[1]){
		int x,y;
		mActionEffect[1]->OperationP(&x,&y);
		mActionEffect[1]->OperationA(x,y+4);
	}

	switch(mModeTimer-14-int(length)){
		case 20:
			BreakTargetLock();
			mActionEffect[2]=NULL;
		case 10:
			mActionEffect[1]=NULL;
		case 0:
			mActionEffect[0]=NULL;
			int no,*target;
			target=GetTarget(&no);

			Attack(	mActiveCharacter,
					target,no,
					95,MIN_ATK_LV_2,
					AT_TYPE_BOW,
					AT_ELE_NONE,AT_OPT_ANTI_AIR,SE_SHOOT);
			break;
		case 50:
			return 1;
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////
int CAct_43::OnTimer()				//影縫い
{
//定型文////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////
	double sx,sy;
	int target=ActiveCharacter()->GetTarget();
	double length=mBattleMode->GetShotSpeed(
		ActiveCharacter()->GetCX(),ActiveCharacter()->GetCY(),
		GetCharacter(target)->GetCX(),GetCharacter(target)->GetGCY(),16,&sx,&sy);

	switch(mModeTimer){
		case 1:
			ActiveCharacter()->ChangeAni(12*ANI_TYPE,24,true);
			break;
		case 20:
			sy-=int(length+2)*1;
			PlayWave(SE_BOW);

			mActionEffect[0]=GetTopEffect().SetEffect(
							BE_SMALL_SHOT,
							ActiveCharacter()->GetCX(),
							ActiveCharacter()->GetCY(),
							BE_SPEED(sx,sy),43,
							BLEND_STANDARD);
			ActiveCharacter()->ChangeAni(9*ANI_TYPE+3,30,true);
			break;

	}
	if(mActionEffect[0]){
		int x,y;
		mActionEffect[0]->OperationP(&x,&y);
		mActionEffect[0]->OperationA(x,y+2);
	}

	switch(mModeTimer-20-int(length)){
		case 1:
			mActionEffect[0]->OperationA(0,0);
			mActionEffect[1]=mActionEffect[0];
			mActionEffect[0]=NULL;
			int no,*target,*hit;
			target=GetTarget(&no);

			hit=Attack(	mActiveCharacter,
					target,no,
					220,MIN_ATK_LV_2,
					AT_TYPE_BOW,
					AT_ELE_NONE,0,SE_SHOOT);
			for(int i=0;i<no;i++){
				if(hit[i]<0)	continue;
				
				GetTopEffect().SetEffect(
					BE_FLASH_CHARACTER,
					0,0,
					target[i] & 0xffff,0,BLEND_STANDARD,0xff000000);

				SetStatus(mActiveCharacter,target[i]&0xffff,3,150);
				SetStatus(mActiveCharacter,target[i]&0xffff,4,150);
			}
			BreakTargetLock();
			break;
		case 30:
			mActionEffect[1]->Destroy();
			return 1;
			break;
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////
int CAct_44::OnTimer()				//アローレイン
{
//定型文////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////
	const int	SHOT_INTARVAL	=4;
	const int	SHOT_NO			=16;
	const int	BEFORE_TIME		=SHOT_INTARVAL*SHOT_NO;
	const int	HIT_TIME		=BEFORE_TIME+8;
	const int	HIT_INTARVAL	=10;
	const int	HIT_NO			=3;
	const int	HIT_END_TIME	=HIT_TIME+HIT_INTARVAL*HIT_NO;
	const int	END_TIME		=HIT_TIME+HIT_INTARVAL*HIT_NO+10;

	int turn;
	if(CheckEnemy(mActiveCharacter))	turn=1;
	else	turn=-1;

	for(int i=0;i<SHOT_NO;i++){
		int time=mModeTimer-i*SHOT_INTARVAL;
		switch(time){
			case 1:
				ActiveCharacter()->ChangeAni(12*ANI_TYPE,16,true);
				break;
			case 3:
				ActiveCharacter()->ChangeAni(13*ANI_TYPE,16,true);
				GetTopEffect().SetEffect(
							BE_MOVE,
							ActiveCharacter()->GetCX(),
							ActiveCharacter()->GetCY(),
							BE_SPEED((turn*((pInput->Rand(100)+150)/200.0)*24),(-((pInput->Rand(100)+150)/200.0)*24)),4,BLEND_ADD);
				ActiveCharacter()->ChangeAni(9*ANI_TYPE+3,20,true);
				PlayWave(SE_BOW);
				break;

		}
	}
	if(mModeTimer>BEFORE_TIME && mModeTimer<HIT_END_TIME){
		GetTopEffect().SetEffect(
					BE_SMALL_SHOT,
					320+(pInput->Rand(480)-320)*turn,
					-80,
					BE_SPEED((turn*((pInput->Rand(100)+150)/200.0)*24),(((pInput->Rand(100)+150)/200.0)*24)),43);
		if(mModeTimer>=HIT_TIME){
			if((mModeTimer-HIT_TIME)%HIT_INTARVAL==0){
				int no,*target;
				target=GetTarget(&no);

				Attack(	mActiveCharacter,
						target,no,
						65,MIN_ATK_LV_2,
						AT_TYPE_BOW,
						AT_ELE_NONE,AT_OPT_HALF_HIT,SE_SHOOT);
			}
		}
	}
	if(mModeTimer>=END_TIME){
		return 1;
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////
int CAct_45::OnTimer()				//シュートザムーン
{
//定型文////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////
	const D3DCOLOR	EFFECT_COLOR=0x00ffffb0;
	const int		EFFECT_LENGTH=480;

	if(mModeTimer==1){
		int no,*target;
		target=GetTarget(&no);
		mActionParam[0]=new int[no];
		mActionParam[1]=new int;
		mActionParam[2]=new int[no];
		mActionParam[3]=new int;
		mActionParam[4]=new int;
		mActionParam[5]=new int;
		mActionParam[6]=new int;

		memcpy(mActionParam[0],target,sizeof(int)*no);
		*mActionParam[1]=no;

		double sx,sy;
		int t=ActiveCharacter()->GetTarget();
		double length=mBattleMode->GetShotSpeed(mActiveCharacter,t,48,&sx,&sy);

		*mActionParam[4]=int(length);
		*mActionParam[5]=int(sx);
		*mActionParam[6]=int(sy);
	}
	switch(mModeTimer){
		case 1:
			ActiveCharacter()->ChangeAni(12*ANI_TYPE,16,true);
			break;
		case 8:
			PlayWave(SE_BOW);
			GetTopEffect().SetEffect(BE_MOVE,
							ActiveCharacter()->GetCX(),
							ActiveCharacter()->GetCY(),
							BE_SPEED(*mActionParam[5],*mActionParam[6]),4,
							BLEND_ADD);
			ActiveCharacter()->ChangeAni(9*ANI_TYPE+3,35,true);
			break;

	}

	int time=mModeTimer-(*mActionParam[4]);
	if(time==8){
		int &no=*mActionParam[1];

		int *dmg=Attack(
					mActiveCharacter,
					mActionParam[0],no,
					150,MIN_ATK_LV_3,
					AT_TYPE_BOW,
					AT_ELE_NONE,AT_OPT_ANTI_AIR,SE_SHOOT);

		memcpy(mActionParam[2],dmg,sizeof(int)*no);
		*mActionParam[3]=0;

		for(int i=0;i<no;i++){
			if(mActionParam[2][i]>=0){
				int t=mActionParam[0][i] & 0xffff;
				GetCharacter(t)->SetGoal(
						GetCharacter(t)->GetX(),
						GetCharacter(t)->GetY(),
						30,true
						);
				*mActionParam[3]=1;
			}
		}
		if(*mActionParam[3]){
			ChangeFieldColor(0x80000000,16);
		}else{
			BreakTargetLock();
		}
	}
	if(time>8){
		if(*mActionParam[3]==0){
			if(time>=45){
				delete[] mActionParam[0];
				delete   mActionParam[1];
				delete[] mActionParam[2];
				delete   mActionParam[3];
				delete   mActionParam[4];
				delete   mActionParam[5];
				delete   mActionParam[6];
				return 1;
			}
		}else{
			int &no=*mActionParam[1];
			if(time>24 && time<100){
				for(int i=0;i<no;i++){
					if(mActionParam[2][i]>=0){
						int t=mActionParam[0][i] & 0xffff;
						GetCharacter(t)->SetSpeedZ(BATTLE_GRAVITY);
					}
				}
			}
			if(time==24){
				int t=mActionParam[0][0] & 0xffff;
				for(int i=no-1;i>=0;i--){
					if(mActionParam[2][i]>=0){
						t=mActionParam[0][i] & 0xffff;
						GetCharacter(t)->ChangeAni(8*ANI_TYPE,63,true);
					}
				}
				PlayWave(SE_MAGIC);
				for(int i=0;i<ACTION_EFFECT_NO;i++){
					double rad=(3.141592*2)/ACTION_EFFECT_NO+i;
					int x=int(sin(rad)*EFFECT_LENGTH);
					int y=int(cos(rad)*EFFECT_LENGTH);
					mActionEffect[i]=GetTopEffect().SetEffect(
						BE_ANIMATION,
						GetCharacter(t)->GetCX()+x,
						GetCharacter(t)->GetCY()+y,
						52,0,BLEND_STANDARD,EFFECT_COLOR);
				}

			}
			if(time==24+60){
				ActiveCharacter()->ChangeAni(12*ANI_TYPE,16,true);
			}
			if(time==24+64){
				GetTopEffect().SetEffect(BE_FLASH,
					0,0,8,0,BLEND_STANDARD,0xffffffff);
				int target=mActionParam[0][0] & 0xffff;
				for(int j=0;j<no;j++){
					if(mActionParam[2][j]>=0){
						target=mActionParam[0][j] & 0xffff;
						break;
					}
				}
				double sx=(GetCharacter(target)->GetCX()-ActiveCharacter()->GetCX());
				double sy=(GetCharacter(target)->GetCY()-ActiveCharacter()->GetCY());
				double length=hypot(sx,sy)/200;
				sx=sx/length;
				sy=sy/length;

				GetTopEffect().SetEffect(
					BE_TAIL_LINE,
					int(ActiveCharacter()->GetCX()-sx),
					int(ActiveCharacter()->GetCY()-sy),
					BE_SPEED(sx,sy),32+2*0x10000,
					BLEND_STANDARD,EFFECT_COLOR | 0xff000000);

				Attack(mActiveCharacter,
					mActionParam[0],no,
					280,MIN_ATK_LV_3,
					AT_TYPE_BOW ^ AT_AVOID,
					AT_ELE_NONE,AT_OPT_ANTI_AIR,SE_HARD_SLASH);
				ActiveCharacter()->ChangeAni(9*ANI_TYPE+3,35,true);

			}

			if(mActionEffect[0]){
				if(time<=24+32){
					for(int i=0;i<ACTION_EFFECT_NO;i++){
						int t=time-24;
						mActionEffect[i]->SetColor(EFFECT_COLOR | 0x02000000*t);
						double rad=(3.141592*2)/ACTION_EFFECT_NO+i+t*0.1;
						int target=mActionParam[0][0] & 0xffff;
						for(int j=0;j<no;j++){
							if(mActionParam[2][j]>=0){
								target=mActionParam[0][j] & 0xffff;
								break;
							}
						}
						int x=int(sin(rad)*EFFECT_LENGTH*(32-t)/32);
						int y=int(cos(rad)*EFFECT_LENGTH*(32-t)/32);
						mActionEffect[i]->SetPos(
								GetCharacter(target)->GetCX()+x,
								GetCharacter(target)->GetCY()+y
							);
					}
				}else if(time>=24+64){
					for(int i=0;i<ACTION_EFFECT_NO;i++){
						mActionEffect[i]->Destroy();
						mActionEffect[i]=NULL;
					}
				}
			}
			if(time==100){
				BreakTargetLock();
			}

			if(time>=120){
				delete[] mActionParam[0];
				delete   mActionParam[1];
				delete[] mActionParam[2];
				delete   mActionParam[3];
				delete   mActionParam[4];
				delete   mActionParam[5];
				delete   mActionParam[6];
				return 1;
			}
		}
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////
int CAct_46::OnTimer()				//愛の天使
{
//定型文////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////
	const int		TIME		= 60;
	const double	MOVE_SPEED	= 0.05;
	const int		BASE_Y		= 32; 
	const int		AFTER_Y		= BASE_Y - int(TIME*TIME*MOVE_SPEED/2);

	double sx,sy;
	int target=ActiveCharacter()->GetTarget();
	double length=mBattleMode->GetShotSpeed(
		ActiveCharacter()->GetCX(),ActiveCharacter()->GetCY(),
		GetCharacter(target)->GetCX(),GetCharacter(target)->GetGCY()+AFTER_Y,16,&sx,&sy);

	switch(mModeTimer){
		case 1:
			int t;
			t=ActiveCharacter()->GetTarget();
			ActiveCharacter()->ChangeAni(12*ANI_TYPE,TIME,true);
			mActionEffect[0]=GetTopEffect().SetEffect(
					BE_STRETCH,
					GetCharacter(t)->GetCX(),
					GetCharacter(t)->GetCY() + BASE_Y,
					34,128+128*0x10000,BLEND_STANDARD);
			PlayWave(SE_MAGIC);
			ChangeFieldColor(0x80000000,32);

			break;
		case TIME:
			
			PlayWave(SE_BOW);
			mActionEffect[1]=GetTopEffect().SetEffect(BE_SMALL_SHOT,
							ActiveCharacter()->GetCX(),
							ActiveCharacter()->GetCY(),
							BE_SPEED(sx,sy-length*0.5),43);
			ActiveCharacter()->ChangeAni(9*ANI_TYPE+3,35,true);
			break;
	}

	int time = mModeTimer - int(length) -TIME+2;
	switch(time){
		case 0:
			mActionEffect[1]->OperationD(0,0);
			PlayWave(SE_SLEEP);
			double x,y;
			x=mActionEffect[0]->GetX();
			y=mActionEffect[0]->GetY();
			mActionEffect[0]->SetPos(x+(sx/8),y);
			break;
		case 1:
			x=mActionEffect[0]->GetX();
			y=mActionEffect[0]->GetY();
			mActionEffect[0]->SetPos(x-(sx/8),y);
			break;
		case 30:
			int no,*target,*dmg;
			target=GetTarget(&no);
			dmg=Attack(
						mActiveCharacter,
						target,no,
						360,MIN_ATK_LV_3,
						AT_TYPE_BOW,
						AT_ELE_NONE,0,SE_CHARM);
			x=mActionEffect[0]->GetX();
			y=mActionEffect[0]->GetY();

			for(int i=0;i<no;i++){
				int t=target[i] & 0xff;
				SetStatus(mActiveCharacter,t,2,250);
			}
			for(int i=0;i<32;i++){
				double sx=(pInput->Rand(1200)-600)/100.0;
				double sy=(pInput->Rand(800)-300)/100.0;
						
				GetTopEffect().SetEffect(BE_FALL_PARTICUL,
						int(x+sx*6),
						int(y-50+pInput->Rand(65)-32),
						BE_SPEED(sx,sy),63+32*0x10000,
						BLEND_ADD);
			}
			mActionEffect[0]->Destroy();
			mActionEffect[0]=NULL;
			ChangeFieldColor(0x00000000,32);
			BreakTargetLock();
			break;
		case 60:
			mActionEffect[1]->Destroy();
			return 1;

	}

	if(mActionEffect[1]){
		double x,y;
		mActionEffect[1]->OperationDP(&x,&y);
		if(time<0){
			mActionEffect[1]->OperationD(x,y+1);
		}else if(time>30){
			mActionEffect[1]->OperationD(sx/8,y+0.5);
		}
	}
	if(mActionEffect[0] && mModeTimer<=TIME){
		int t=(mModeTimer-TIME);
		double x=mActionEffect[0]->GetX();
		double y=mActionEffect[0]->GetY();
		if(1){
			mActionEffect[0]->SetPos(x,y+t*MOVE_SPEED);
		}

		D3DCOLOR color;
		if(mModeTimer<30){
			color=mModeTimer*8*0x01;
		}else{
			color=0xffffffff;
		}

		double sx=(pInput->Rand(1200)-600)/100.0;
		double sy=(pInput->Rand(200))/100.0;
		GetTopEffect().SetEffect(BE_FALL_PARTICUL,
						int(x+sx*6),
						int(y-50+pInput->Rand(65)-32),
						BE_SPEED(sx,sy),26+32*0x10000,
						BLEND_ADD,0xff000000+color*0x010000);

		mActionEffect[0]->SetColor(color*0x01000000+0xffffff);

		double rad=(t*t*t*0.0005);
		int size=int(cos(rad)*128);
		mActionEffect[0]->OperationA(size,128);
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////
int CAct_47::OnTimer()				//ソウルブレイク
{
//定型文////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////
	const int ARROW_SIZE	 = 80;
	const int ARROW_NO		 = 6;
	const int ARROW_TIME	 = 16;
	const int ARROW_INTERVAL = 6;
	const int ARROW_END		 = ARROW_NO*ARROW_INTERVAL+ARROW_TIME+3;

	const int SOUL_TIME		 =16;
	const int SOUL_SPEED	 =4;
	const int SOUL_LENGTH	 =SOUL_TIME*SOUL_SPEED;

	const int FINISH_TIME	 = 100;
	const int SOUL_NO		 = ACTION_EFFECT_NO;
	const int END_TIME		 = 120;

	int target=ActiveCharacter()->GetTarget();
	if(mModeTimer == 1){
		ChangeFieldColor(0xb0000000,32);
	}
	//矢の結界
	for(int i=0;i<ARROW_NO;i++){
		int t=mModeTimer-i*ARROW_INTERVAL;
		double sx,sy;
		double rad=3.141592*2*i/ARROW_NO;
		int x=ActiveCharacter()->GetCX() + int(cos(rad)*ARROW_SIZE);
		int y=ActiveCharacter()->GetCY() + int(sin(rad)*ARROW_SIZE*0.7)+16;
 
		mBattleMode->GetShotSpeedByTime(
			x,y,
			GetCharacter(target)->GetCX(),GetCharacter(target)->GetGCY(),ARROW_TIME,&sx,&sy);

		if(t==1){
			ActiveCharacter()->ChangeAni(12*ANI_TYPE,24,true);
		}
		if(t==3){
			CBattleEffect *effect=&GetTopEffect();
			if(sin(rad)>0)	effect=&GetBottomEffect();
			sy-=ARROW_TIME*2;
			PlayWave(SE_BOW);

			mActionEffect[i]=effect->SetEffect(
							BE_SMALL_SHOT,
							ActiveCharacter()->GetCX(),
							ActiveCharacter()->GetCY(),
							BE_SPEED(sx,sy),43,
							BLEND_STANDARD);
			ActiveCharacter()->ChangeAni(9*ANI_TYPE,24,true);
		}
		if(mActionEffect[i]){
			if(t<3+ARROW_TIME){
				int x,y;
				mActionEffect[i]->OperationP(&x,&y);
				mActionEffect[i]->OperationA(x,y+4);
			}else if(t==3+ARROW_TIME){
				mActionEffect[i]->OperationA(0,0);
			}
		}
	}
	//結界ビーム
	if(mModeTimer==ARROW_END){
		int x=GetCharacter(target)->GetCX();
		int y=GetCharacter(target)->GetGCY()+64;
		PlayWave(SE_MAGIC);
		for(int i=0;i<32;i++){
			CBattleEffect *effect= &GetBottomEffect();
			if(i<16)	effect=&GetTopEffect();
			double rad=i*3.141592*2/32;
			int nx=int(x+cos(rad)*ARROW_SIZE);
			int ny=int(y+(sin(rad)*ARROW_SIZE)*0.7);

			effect->SetEffect(
					BE_TAIL_LINE,
					nx-BATTLE_SQUARE_X,ny-BATTLE_SQUARE_Y+32,
					BE_SPEED(0,-32),16+4*0x10000,
					BLEND_ADD,0xffff8000);
			
		}

		mActionEffect[ACTION_EFFECT_NO-1]=GetTopEffect().SetEffect(
				BE_CHARACTER,
				GetCharacter(target)->GetX(),GetCharacter(target)->GetY(),
				target,0,BLEND_STANDARD,0x80ffffff);
	}
	if(mModeTimer>=ARROW_END && mModeTimer<=ARROW_END+SOUL_TIME){
		mActionEffect[ACTION_EFFECT_NO-1]->SetPos(
			mActionEffect[ACTION_EFFECT_NO-1]->GetX(),
			mActionEffect[ACTION_EFFECT_NO-1]->GetY()-SOUL_SPEED);
	}

	//トドメ
	if(mModeTimer==FINISH_TIME-4){
		ActiveCharacter()->ChangeAni(12*ANI_TYPE,24,true);
	}
	if(mModeTimer==FINISH_TIME){
		GetTopEffect().SetEffect(BE_FLASH,
			0,0,8,0,BLEND_STANDARD,0xffffffff);
		double sx=(GetCharacter(target)->GetCX()-ActiveCharacter()->GetCX());
		double sy=(GetCharacter(target)->GetCY()-SOUL_LENGTH-ActiveCharacter()->GetCY());
		double length=hypot(sx,sy)/200;
		sx=sx/length;
		sy=sy/length;
		
		int no,*target,*dmg;
		target=GetTarget(&no);

		GetTopEffect().SetEffect(
			BE_TAIL_LINE,
			int(ActiveCharacter()->GetCX()-sx),
			int(ActiveCharacter()->GetCY()-sy),
			BE_SPEED(sx,sy),32+2*0x10000,
			BLEND_STANDARD,0xffff8000);

		dmg=Attack(mActiveCharacter,
			target,no,
			666,MIN_ATK_LV_4,
			AT_TYPE_BOW,
			AT_ELE_NONE,0,SE_HARD_SLASH);

		for(int i=0;i<no;i++){
			int t=target[i] & 0xff;
			SetStatus(mActiveCharacter,t,7,1000);
		}
		BreakTargetLock();
		ActiveCharacter()->ChangeAni(9*ANI_TYPE+3,35,true);

		ChangeFieldColor(0x00000000,16);
		
		for(int i=0;i<ARROW_NO;i++){
			if(mActionEffect[i]){
				mActionEffect[i]->Destroy();
				mActionEffect[i]=NULL;
			}
		}
		mActionEffect[ACTION_EFFECT_NO-1]->Destroy();
		
		int t=ActiveCharacter()->GetTarget();
		for(int i=0;i<SOUL_NO;i++){
			mActionEffect[i]=GetTopEffect().SetEffect(
					BE_CHARACTER,
					GetCharacter(t)->GetX(),GetCharacter(t)->GetY()-SOUL_LENGTH,
					t,0,BLEND_STANDARD,0x80ffffff);
		}
	}
	
	//後始末
	if(mModeTimer>FINISH_TIME){
		int alpha=(FINISH_TIME+16-mModeTimer)*8;
		for(int i=0;i<SOUL_NO;i++){
			if(mActionEffect[i]){
				if(alpha<0){
					mActionEffect[i]->Destroy();
					mActionEffect[i]=NULL;
				}else{
					double rad=3.141592*2*i/SOUL_NO;
					
					mActionEffect[i]->SetPos(
						mActionEffect[i]->GetX()+cos(rad)*16,
						mActionEffect[i]->GetY()+sin(rad)*16);
					mActionEffect[i]->SetColor(alpha<<24 | 0xffffff);
				}
			}
		}
	}

	if(mModeTimer==END_TIME){
		return 1;
	}
	return 0;
}