#include "iceaction.h"
#include "input.h"

////////////////////////////////////////////////////////////////////////////
int CAct_62::OnTimer()				//アイスニードル
{
//定型文////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////
	double sx,sy;
	int target=ActiveCharacter()->GetTarget();
	double length=mBattleMode->GetShotSpeed(mActiveCharacter,target,24,&sx,&sy);



	for(int k=0;k<3;k++){
		int timer=mModeTimer-k*12;
		if(timer==1){
			int x=pInput->Rand(15)-7;
			int y=pInput->Rand(15)-7;
			ActiveCharacter()->ChangeAni(13*ANI_TYPE,16,true);
			mActionEffect[k]=GetTopEffect().SetEffect(
							BE_SMALL_SHOT,
							ActiveCharacter()->GetCX()+x,
							ActiveCharacter()->GetCY()+y,
							BE_SPEED(sx,sy),11);
		}
		if(timer-1==int(length)){
			int i;
			double sx=(GetCharacter(target)->GetSX()-ActiveCharacter()->GetSX())*BATTLE_SQUARE_SIZE;
			double sy=(GetCharacter(target)->GetSY()-ActiveCharacter()->GetSY())*BATTLE_SQUARE_SIZE;
			double length=hypot(sx,sy)/12;
			sx=sx/length;
			sy=sy/length;
			for(i=0;i<8;i++){
				double x=(pInput->Rand(700)-350)/100.0;
				double y=(pInput->Rand(700)-350)/100.0;
				GetTopEffect().SetEffect(BE_FALL_PARTICUL,
							GetCharacter(target)->GetCX(),
							GetCharacter(target)->GetCY(),
							BE_SPEED((sx+x),(sy+y)),16*0x10000+12,
							BLEND_ADD);
			}

			int no,*target;
			target=GetTarget(&no);

			Attack(	mActiveCharacter,
					target,no,
					60,MIN_ATK_LV_1,
					AT_TYPE_MAGIC,
					AT_ELE_ICE,0,SE_ICE);

			mActionEffect[k]->Destroy();
			if(k==2)	BreakTargetLock();
		}
	}
	if(mModeTimer-55>=int(length)){
		return 1;
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////
int CAct_63::OnTimer()				//フロストバイト
{
//定型文////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////
	if(mModeTimer==1){
		ActiveCharacter()->ChangeAni(13*ANI_TYPE,40,true);
		mActionEffect[0]=GetTopEffect().SetEffect(
						BE_CHARACTER,
						0,
						0,
						ActiveCharacter()->GetTarget(),1,
						BLEND_SOLID,0x000000ff);
		mActionEffect[1]=GetTopEffect().SetEffect(
						BE_CHARACTER,
						0,
						0,
						ActiveCharacter()->GetTarget(),1,
						BLEND_SOLID_ADD,0x00ffffff);
	}else if(mModeTimer==64){
		int no,*target;
		target=GetTarget(&no);

		for(int i=0;i<no;i++){
			int t=target[i]&0xffff;
			int elepower=GetElementPower(mActiveCharacter,t,AT_ELE_ICE,AT_ELE_ICE,0);
			SetStatus(mActiveCharacter,t,7,int(elepower));
		}
		BreakTargetLock();
	}else if(mModeTimer==100){
		return 1;
	}

	if(mModeTimer<64){ 
		int target=ActiveCharacter()->GetTarget();
		int turn=1;
		if(!CheckEnemy(mActiveCharacter)) turn=-1;

		double sx=(800+pInput->Rand(400))*turn/100.0;
		double sy=(400+pInput->Rand(300))/100.0;

		GetTopEffect().SetEffect(
				BE_FADE_IN_PARTICUL,
				GetCharacter(target)->GetCX()-(64+pInput->Rand(32))*turn,
				GetCharacter(target)->GetCY()-(pInput->Rand(64))*GetCharacter(target)->GetSize(),
				BE_SPEED(sx,sy),12+16*0x10000,
				BLEND_ADD,0x00ffffff);
		if(mModeTimer%8==0){
			PlayWave(SE_ICE);
		}
	}
	if(mActionEffect[0]){
		int alpha;
		if(mModeTimer<48){
			alpha=mModeTimer*4;
		}else if(mModeTimer<64){
			alpha=48*4;;
		}else if(mModeTimer<70){
			alpha=(70-mModeTimer)*32;
		}else {
			mActionEffect[0]->Destroy();
			mActionEffect[1]->Destroy();
			mActionEffect[0]=NULL;
			mActionEffect[1]=NULL;
			return 0;
		}
		alpha*=0x01000000;
		mActionEffect[0]->SetColor(alpha+0x0000ff);
		mActionEffect[1]->SetColor(alpha+0xffffff);
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////
int CAct_65::OnTimer()				//フローズンソード
{
//定型文////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////
	int target=ActiveCharacter()->GetTarget();
	int sy=GetCharacter(target)->GetSY()+GetCharacter(target)->GetSize()-1;
	switch(mModeTimer){
		case 2:
			ActiveCharacter()->ChangeAni(2*ANI_TYPE,1,true);
			ActiveCharacter()->SetGoalSquare(4,sy,12,1,false);
			break;
		case 16+8*0:
		case 16+8*1:
		case 16+8*2:
		case 16+8*3:
			{
				ActiveCharacter()->ChangeAni(9*ANI_TYPE,32,true);
				PlayWave(SE_ICE);
				int no=(mModeTimer-16)/8;
				int turn;
				if(!CheckEnemy(mActiveCharacter,target))	turn=1;
				else	turn=-1;
				
				int x=ActiveCharacter()->GetCX()-turn*0;
				int y=ActiveCharacter()->GetCY();
				int pa=38+(180-turn*90)*0x10000;
				int pb=64+0*0x10000;
				mActionEffect[no]=GetTopEffect().SetEffect(
									BE_ROTATE,x,y,pa,pb,
									BLEND_STANDARD);
				mActionEffect[no+4]=GetTopEffect().SetEffect(
									BE_ROTATE,x,y,pa,pb,
									BLEND_SOLID);
			}
			break;
		case 64+4*0:
				ActiveCharacter()->ChangeAni(11*ANI_TYPE,16,true);
		case 64+4*1:
		case 64+4*2:
		case 64+4*3:
			{
				int no=(mModeTimer-64)/4;

				int sx;
				if(!CheckEnemy(mActiveCharacter,target))	sx=-1;
				else	sx=1;
				mActionEffect[no+4]=GetTopEffect().SetEffect(
									BE_MOVE,
									0,0,
									BE_SPEED((sx*BATTLE_SQUARE_SIZE/2),0),38,
									BLEND_STANDARD);
				mActionEffect[no+4]->SetPos(mActionEffect[no]->GetX(),mActionEffect[no]->GetY());
				mActionEffect[no]->Destroy();
				mActionEffect[no]=NULL;
			}
			break;
		case 64+3:
				ActiveCharacter()->ChangeAni(10*ANI_TYPE,16,true);
				break;
		case 96:
			return 1;

	}

	if(mModeTimer>=64 && mModeTimer%2==0){
		int x=-(mModeTimer-64)/2;
		int no,*target;
		target=GetTarget(&no);
		
		ChargeBreak();	//チャージ消去

		for(int i=0;i<no;i++){
			int sx=GetCharacter(target[i]&0xffff)->GetSX();
			if(CheckEnemy(mActiveCharacter,target[i]&0xffff)){
				sx=-sx+8;
			}
			if(4+x<=sx && sx<12+x && (sx+x)%2==0){
				Attack(	mActiveCharacter,
						target[i]&0xffff,
						45,MIN_ATK_LV_2+50,
						AT_TYPE_MAGIC,
						AT_ELE_ICE,0,SE_HARD_ICE,
						target[i]/0x10000);
			}
		}
	}
	if(mModeTimer>=16){
		for(int i=0;i<4;i++){
			if(mActionEffect[i]){
				int t=mModeTimer-16-i*8;
				int y=int(-cos(double(t/6.0))*32+ActiveCharacter()->GetCY()+32);
				mActionEffect[i]->SetPos(mActionEffect[i]->GetX(),y);
				if(mActionEffect[i+4]){
					if(t<8){
						mActionEffect[i+4]->SetPos(mActionEffect[i]->GetX(),y);
						mActionEffect[i+4]->SetColor(D3DCOLOR_ARGB(255-32*t,255,255,255));
					}else{
						mActionEffect[i+4]->Destroy();
						mActionEffect[i+4]=NULL;
					}
				}
			}
		}
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////
int CAct_66::OnTimer()				//コールドスリープ
{
//定型文////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////
	D3DCOLOR COLOR=0x000000ff;
	switch(mModeTimer){
		case 1:
			mActionEffect[0]=GetTopEffect().SetEffect(
									BE_GRADATION_V,
									0,0,
									500,COLOR,
									BLEND_STANDARD,COLOR);
			break;
		case 90:
			int no,*target;
			target=GetTarget(&no);
			for(int i=0;i<no;i++){
				int t=target[i]&0xffff;
				if(t==mActiveCharacter)	continue;
				SetStatus(mActiveCharacter,t,1,150,8);
			}
			BreakTargetLock();
			break;
		case 100:
			if(mActionEffect[0])	mActionEffect[0]->Destroy();
			return 1;
			break;
	}
	for(int i=0;i<2;i++){
		GetTopEffect().SetEffect(
			BE_FADE_IN_PARTICUL,
			(pInput->Rand(640)),
			(pInput->Rand(320)),
			BE_SPEED(0,2),12+32*0x10000,
			BLEND_ADD,0x00ffffff);
	}
	if(mActionEffect[0]){
		if(mModeTimer<60){
			mActionEffect[0]->SetColor(COLOR+ (mModeTimer*0x02000000));
		}else if(mModeTimer>90){
			mActionEffect[0]->SetColor(COLOR+((100-mModeTimer)*0x0b000000));
		}
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////
int CAct_67::OnTimer()				//アイシクルスラッサー
{
//定型文////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////
	const int hit=6;
	const int interval=8;
	const int start=8;
	const int movetime=16;
	const D3DCOLOR color=0xff101040;

	int target=ActiveCharacter()->GetTarget();
	if(mModeTimer<=interval*hit){
		for(int i=0;i<2;i++){
			double rad=pInput->Rand(360)*3.141592/180;
			double r=pInput->Rand(160);
			double length=pow(r,1.2)/4;
			int x=int(cos(rad)*length);
			int y=int(sin(rad)*length);
			GetTopEffect().SetEffect(BE_FADE_IN_PARTICUL,
						GetCharacter(target)->GetCX()+x,
						GetCharacter(target)->GetCY()+y-16,
						BE_SPEED(0,1),32*0x10000+54,
						BLEND_ADD,color);
		}
	}
	for(int k=0;k<hit;k++){
		int timer=mModeTimer-k*interval;
		if(timer==start){
			double rad=pInput->Rand(360)*3.141592/180;
			double x=cos(rad)*640;
			double y=sin(rad)*640;
			ActiveCharacter()->ChangeAni(13*ANI_TYPE,interval,true);
			mActionEffect[k]=GetTopEffect().SetEffect(
							BE_MOVE,
							int(GetCharacter(target)->GetCX()-x),
							int(GetCharacter(target)->GetCY()-y),
							BE_SPEED((x/movetime),(y/movetime)),53);
		}
		if(timer==start+movetime){
			for(int i=0;i<32;i++){
				double x=(pInput->Rand(3000)-1500)/100.0;
				double y=(pInput->Rand(3000)-1500)/100.0;
				GetTopEffect().SetEffect(BE_FALL_PARTICUL,
							GetCharacter(target)->GetCX(),
							GetCharacter(target)->GetCY(),
							BE_SPEED(x,y),16*0x10000+12,
							BLEND_ADD);
			}

			int no,*target;
			target=GetTarget(&no);

			Attack(	mActiveCharacter,
					target,no,
					80,MIN_ATK_LV_3,
					AT_TYPE_MAGIC,
					AT_ELE_ICE,0,SE_HARD_ICE);
			if(k==hit-1)	BreakTargetLock();
		}
		if(timer==start+movetime*2){
			mActionEffect[k]->Destroy();
			mActionEffect[k]=NULL;
		}
		if(mActionEffect[k]){
			for(int i=0;i<8;i++){
				double x=(pInput->Rand(500)-250)/100.0;
				double y=(pInput->Rand(500)-250)/100.0;
				GetTopEffect().SetEffect(BE_FADE_IN_PARTICUL,
							int(mActionEffect[k]->GetX()),
							int(mActionEffect[k]->GetY()),
							BE_SPEED(x,y),32*0x10000+54,
							BLEND_ADD,color);
			}
		}
	}
	if(mModeTimer>=interval*hit+50){
		return 1;
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////
int CAct_68::OnTimer()				//ヒールレイン
{
//定型文////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////
	D3DCOLOR COLOR=0x000000ff;
	switch(mModeTimer){
		case 1:
			mActionEffect[0]=GetTopEffect().SetEffect(
									BE_GRADATION_V,
									0,0,
									500,COLOR,
									BLEND_STANDARD,COLOR);
			break;
		case 90:
			int no,*target;
			target=GetTarget(&no);
			for(int i=0;i<no;i++){
				int t=target[i]&0xffff;
				SetStatus(mActiveCharacter,t,12,100);
			}
			BreakTargetLock();
			break;
		case 100:
			if(mActionEffect[0])	mActionEffect[0]->Destroy();
			return 1;
			break;
	}
	if(pInput->Rand(4)==0){
		GetTopEffect().SetEffect(BE_STANDARD,
			pInput->Rand(640),
			pInput->Rand(480),
			6,0,BLEND_ADD,0xff0000ff);
		PlayWave(SE_HEAL);
	}
	for(int i=0;i<8;i++){
		GetTopEffect().SetEffect(
			BE_FADE_IN_PARTICUL,
			(pInput->Rand(640))-80,
			(pInput->Rand(480))-200,
			BE_SPEED(6,12),51+32*0x10000,
			BLEND_STANDARD,0x00ffffff);
	}
	if(mActionEffect[0]){
		if(mModeTimer<30){
			mActionEffect[0]->SetColor(COLOR+ (mModeTimer*0x04000000));
		}else if(mModeTimer>90){
			mActionEffect[0]->SetColor(COLOR+((100-mModeTimer)*0x0b000000));
		}
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////
int CAct_69::OnTimer()				//スノーマンラッシュ
{
//定型文////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////
	const int BODY_TIME			=32;

	const int ARM_TIME			=40;
	const int ARM_INTERVAL		=10;
	const int ARM_FALL_SPEED	=32;
	const int ARM_FALL_TIME		=16;
	const double ARM_RAD[2]={3.141592/2-1,3.141592/2+1};

	const int HEAD_TIME			=60;
	const int HEAD_FALL_SPEED	=32;
	const int HEAD_FALL_TIME	=16;

	const int FINISH_TIME		=120;
	const int END_TIME			=140;

	D3DCOLOR COLOR=0x000000ff;
	if(mModeTimer==1){
		mActionEffect[0]=GetBottomEffect().SetEffect(
								BE_GRADATION_V,
								0,0,
								500,COLOR,
								BLEND_STANDARD,COLOR);
	}

	int target=ActiveCharacter()->GetTarget();
	int dir=1;
	if(!CheckEnemy(mActiveCharacter,target))	dir=-dir;

	//本体作成
	if(mModeTimer<=BODY_TIME){
		if(mModeTimer%2==0){
			PlayWave(SE_HIT);
		}
		int cx=GetCharacter(target)->GetCX();
		int cy=GetCharacter(target)->GetCY();
		for(int j=0;j<16;j++){
			double rad=pInput->Rand(360)*3.141592/180;
			double len=pInput->Rand(320)*0.1;
			double x=cos(rad)*len;
			double y=sin(rad)*len*0.8;
			GetTopEffect().SetEffect(BE_SNOW_MAN,
					int(cx-x*18),int(cy-y*18),
					BE_SPEED(x,y),mModeTimer*0x10000+54,
					BLEND_ADD);
		}
	}
	//腕を飛ばす部分
	for(int i=0;i<2;i++){
		int t=mModeTimer-ARM_INTERVAL*i;
		if(t==ARM_TIME){
			double rad=ARM_RAD[i]+(pInput->Rand(101)-50)*0.005;
			double x=cos(rad)*ARM_FALL_SPEED;
			double y=sin(rad)*ARM_FALL_SPEED;
			mActionEffect[i+2]=GetTopEffect().SetEffect(
							BE_MOVE,
							int(GetCharacter(target)->GetCX()-x*(ARM_FALL_TIME+4)),
							int(GetCharacter(target)->GetCY()+16-y*(ARM_FALL_TIME+4)),
							BE_SPEED(x,y),72);
		}
		if(t==ARM_TIME+ARM_FALL_TIME){
			int no,*target;
			target=GetTarget(&no);
			Attack(	mActiveCharacter,
					target,no,
					150,MIN_ATK_LV_4,
					AT_TYPE_MAGIC,
					AT_ELE_ICE,0,SE_HARD_ICE);
			mActionEffect[i+2]->OperationD(0,0);
		}
	}

	//頭を飛ばす部分
	if(mModeTimer==HEAD_TIME){
		int sx=HEAD_FALL_SPEED*dir/3;
		int x=GetCharacter(target)->GetCX()-sx*HEAD_FALL_TIME;
		int y=GetCharacter(target)->GetCY()-HEAD_FALL_SPEED*HEAD_FALL_TIME-96;
		mActionEffect[1]=GetTopEffect().SetEffect(
						BE_MOVE_NO_SPIN,
						x,y,
						BE_SPEED(sx,HEAD_FALL_SPEED),73+(dir<0?TURN_HORIZONTAL:0)*0x10000);
	}else if(mModeTimer==HEAD_TIME+HEAD_FALL_TIME){
		int cx=GetCharacter(target)->GetCX();
		int cy=GetCharacter(target)->GetCY()-30;
		for(int j=0;j<32;j++){
			double sx=(pInput->Rand(10000)-5000)/200.0;
			double sy=(pInput->Rand(10000)-7000)/200.0;
			GetTopEffect().SetEffect(BE_FALL_PARTICUL,
					int(cx+sx),int(cy+sy),
					BE_SPEED((sx/8),(sy/8)),32*0x10000+54,
					BLEND_ADD);
		}
		int no,*target;
		target=GetTarget(&no);
		Attack(	mActiveCharacter,
				target,no,
				150,MIN_ATK_LV_4,
				AT_TYPE_MAGIC,
				AT_ELE_ICE,0,SE_FIRE);

		mActionEffect[1]->OperationD(0,0);
	}

	//トドメ
	if(mModeTimer==FINISH_TIME){
		if(mActionEffect[1]){	mActionEffect[1]->Destroy();	mActionEffect[1]=NULL;}
		for(int i=0;i<2;i++){
			if(mActionEffect[i+2]){
				double x=cos(ARM_RAD[i])*2;
				double y=sin(ARM_RAD[i])*2;
				mActionEffect[i+2]->OperationD(x,y);
			}
		}
		int cx=GetCharacter(target)->GetCX();
		int cy=GetCharacter(target)->GetCY()-96;
		for(int j=0;j<96;j++){
			double sx=(pInput->Rand(10000)-5000)/80.0;
			double sy=(pInput->Rand(10000)-7000)/80.0;
			GetTopEffect().SetEffect(BE_FALL_PARTICUL,
					int(cx+sx),int(cy+sy),
					BE_SPEED((sx/8),(sy/8)),32*0x10000+54,
					BLEND_ADD);
		}
		int no,*target;
		target=GetTarget(&no);
		Attack(	mActiveCharacter,
				target,no,
				300,MIN_ATK_LV_4,
				AT_TYPE_MAGIC,
				AT_ELE_ICE,0,SE_EXPLOSION);
		BreakTargetLock();
	}
	if(mModeTimer==END_TIME){
		if(mActionEffect[0]){	mActionEffect[0]->Destroy();	mActionEffect[0]=NULL;}
		return 1;
	}
	//全体エフェクト
	if(mModeTimer<=FINISH_TIME){
		for(int i=0;i<2;i++){
			GetTopEffect().SetEffect(
				BE_FADE_IN_PARTICUL,
				(pInput->Rand(640))-10*dir,
				(pInput->Rand(480))-100,
				BE_SPEED(dir,3),12+32*0x10000,
				BLEND_ADD,0x00ffffff);
		}
	}else{
		for(int i=0;i<2;i++){
			if(mActionEffect[i+2]){
				double x,y;
				mActionEffect[i+2]->OperationDP(&x,&y);
				y+=0.2;
				mActionEffect[i+2]->OperationD(x,y);
				int alpha=(FINISH_TIME+16-mModeTimer)*16;
				if(alpha<0)	alpha=0;
				mActionEffect[i+2]->SetColor(alpha<<24 | 0xffffff);
			}
		}
	}
	if(mActionEffect[0]){
		if(mModeTimer<30){
			mActionEffect[0]->SetColor(COLOR+ (mModeTimer*0x04000000));
		}else if(mModeTimer>END_TIME-10){
			mActionEffect[0]->SetColor(COLOR+((END_TIME-mModeTimer)*0x0b000000));
		}
	}
	return 0;
}