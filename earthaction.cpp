#include "earthaction.h"
#include "input.h"

////////////////////////////////////////////////////////////////////////////
int CAct_78::OnTimer()				//ダートランス
{
//定型文////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////
	int target=ActiveCharacter()->GetTarget();
	if(mModeTimer==1){
		ActiveCharacter()->ChangeAni(10*ANI_TYPE+1,16,true);
	}
	if(mModeTimer==4){
		ActiveCharacter()->ChangeAni(11*ANI_TYPE+1,16,true);
		mActionEffect[0]=GetBottomEffect().SetEffect(
							BE_STRETCH,
							GetCharacter(target)->GetCX(),
							GetCharacter(target)->GetGCY()+70,
							15,0);
	}
	if(mModeTimer==8){
		int no,i,*target;
		target=GetTarget(&no);

		int *dmg=Attack(
				mActiveCharacter,
				target,no,
				200,MIN_ATK_LV_1,
				AT_TYPE_MAGIC,
				AT_ELE_EARTH,AT_OPT_NO_AIR,SE_STONE);

		for(i=0;i<no;i++){
			if(dmg[i]>0){
				GetCharacter(target[i] & 0xffff)->SetGoal(
						GetCharacter(target[i] & 0xffff)->GetX(),
						GetCharacter(target[i] & 0xffff)->GetY(),
						40,true
						);
			}
		}
		BreakTargetLock();
	}
	if(mModeTimer==7){
		ActiveCharacter()->ChangeAni(9*ANI_TYPE+1,22,true);
	}
	if(mModeTimer<=8){
		int timer=mModeTimer-4;
		if(mActionEffect[0]){
			mActionEffect[0]->OperationA(timer*32,timer*32);
			mActionEffect[0]->SetPos(
				mActionEffect[0]->GetX(),
				mActionEffect[0]->GetY()-16);
		}
	}
	if(16<mModeTimer && mModeTimer<32){
		if(mActionEffect[0]){
			int t=32-mModeTimer;
			mActionEffect[0]->OperationA(t*8,t*8);
			mActionEffect[0]->SetPos(
				mActionEffect[0]->GetX(),
				mActionEffect[0]->GetY()+4);
		}
	}
	if(mModeTimer==32){
		mActionEffect[0]->Destroy();
		mActionEffect[0]=NULL;
	}

	if(mModeTimer>55){
		return 1;
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////
int CAct_79::OnTimer()				//グラビティー
{
//定型文////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////
	int no,*target;
	target=GetTarget(&no);

	if(mModeTimer%2==0 && mModeTimer<64){
		for(int i=0;i<no;i++){
			int t=target[i]&0xffff;
			GetCharacter(t)->SetDamageEffect(false);
		}
		PlayWave(SE_GRAVITY);

		GetTopEffect().SetEffect(
					BE_FALL_PARTICUL,
					GetCharacter(ActiveCharacter()->GetTarget())->GetCX()+pInput->Rand(255)-127,
					GetCharacter(ActiveCharacter()->GetTarget())->GetGCY()-480,
					BE_SPEED(0,32),35+20*0x10000,
					BLEND_ADD,0xffb040ff);

	}else if(mModeTimer==64){
		for(int i=0;i<no;i++){
			SetStatus(mActiveCharacter,target[i]&0xffff,9,80);
		}
		BreakTargetLock();
	}else if(mModeTimer==80){
		return 1;
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////
int CAct_81::OnTimer()				//ロックプレス
{
//定型文////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////
const int CHARGE_TIME		= 32;
const int CONCENTRATE_TIME	= 16;
const int UP_START_TIME		= CHARGE_TIME+CONCENTRATE_TIME+8;
const int UP_TIME			= 16;
const int FALL_START_TIME	= UP_START_TIME+UP_TIME;
const int FALL_TIME			= 16;
const int FALL_END_TIME		= FALL_START_TIME+FALL_TIME;
const int DELETE_START_TIME	= FALL_END_TIME+16;
const int DELETE_TIME		= 8;
const int END_TIME			= DELETE_START_TIME+DELETE_TIME;

	int x=-100;
	if(CheckEnemy(mActiveCharacter))	x=-x;

	switch(mModeTimer){
		case 1:
			ActiveCharacter()->ChangeAni(13*ANI_TYPE,CHARGE_TIME,true);

			mActionEffect[0]=GetTopEffect().SetEffect(
								BE_ANIMATION,
								ActiveCharacter()->GetCX()+x,
								ActiveCharacter()->GetCY(),
								37,0,BLEND_STANDARD,0x00ffffff);
			break;
		case FALL_END_TIME:
			int no,*target;
			target=GetTarget(&no);

			Attack(mActiveCharacter,
					target,no,
					300,MIN_ATK_LV_2,
					AT_TYPE_MAGIC,
					AT_ELE_EARTH,0,SE_STONE);
			BreakTargetLock();
			break;
		case FALL_END_TIME+4:
		case FALL_END_TIME+8:
			PlayWave(SE_STONE);
			break;
		case END_TIME:
			mActionEffect[0]->Destroy();
			mActionEffect[0]=NULL;
			return 1;
	}
	if(mActionEffect[0]){
		int target=ActiveCharacter()->GetTarget();
		if(mModeTimer<CHARGE_TIME){
			if(mModeTimer%4==1){
				PlayWave(SE_GRAVITY);
			}
			mActionEffect[0]->SetColor( D3DCOLOR_ARGB(255*mModeTimer/CHARGE_TIME,255,255,255) );

			int angle=pInput->Rand(360);
			GetTopEffect().SetEffect(
							BE_CONCENTRATE,
							ActiveCharacter()->GetCX()+x,
							ActiveCharacter()->GetCY(),
							angle+192*0x10000,35+CONCENTRATE_TIME*0x10000,
							BLEND_ADD,0xfffff00);
		}else if(mModeTimer<UP_START_TIME){
		}else if(mModeTimer<FALL_START_TIME){
			mActionEffect[0]->SetPos(
				mActionEffect[0]->GetX(),
				mActionEffect[0]->GetY()-(640/UP_TIME));
		}else if(mModeTimer<=FALL_END_TIME){
			mActionEffect[0]->SetPos(
				GetCharacter(target)->GetCX(),
				GetCharacter(target)->GetCY()-(640/FALL_TIME)*(FALL_END_TIME-mModeTimer)+32);
		}else if(mModeTimer<DELETE_START_TIME){
			mActionEffect[0]->SetPos(
				GetCharacter(target)->GetCX()+mModeTimer%2*4-2,
				GetCharacter(target)->GetCY()+32);
		}else if(mModeTimer<END_TIME){
			int t=END_TIME-mModeTimer;
			mActionEffect[0]->SetColor( D3DCOLOR_ARGB(255*t/DELETE_TIME,255,255,255) );
		}
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////
int CAct_82::OnTimer()				//グランドウォール
{
//定型文////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////
	int no,*target;
	target=GetTarget(&no);
	switch(mModeTimer){
		case 1:
			for(int i=0;i<4;i++){
				int x=int(BATTLE_SQUARE_SIZE*5);
				int y=int(BATTLE_SQUARE_SIZE*(i+1));
				mActionEffect[i]=GetTopEffect().SetEffect(BE_ANIMATION,x,y,48,0,BLEND_STANDARD);
				mActionEffect[i]->OperationA(5,0);
				mBattleMode->mWall[i].DeleteCharacter();
			}
			break;
		case 20:
			for(int i=0;i<4;i++){
				if(mActionEffect[i]){
					mActionEffect[i]->OperationA(4,0);
				}
			}
			break;
		case 40:
			for(int i=0;i<4;i++){
				if(mActionEffect[i]){
					mActionEffect[i]->OperationA(1,0);
				}
			}
			break;
		case 60:
			for(int i=0;i<4;i++){
				if(mActionEffect[i]){
					mActionEffect[i]->Destroy();
				}
				int hp=ActiveCharacter()->GetMagAtk()*6+200;
				if(hp>5000)	hp=5000;
				mBattleMode->mWall[i].LoadCharacter(0,hp,4,i,mBattleMode->mBattleOption);
			}
			return 1;
	}

	if(mModeTimer%4==1){
		PlayWave(SE_MOVE_BLOCK);
	}
	for(int i=0;i<4;i++){
		int x=int(BATTLE_SQUARE_SIZE*5);
		int y=int(BATTLE_SQUARE_SIZE*(i+2));
		double sx=(pInput->Rand(3000)-1500)/100.0;
		double sy=(pInput->Rand(500)-600)/100.0;
		GetTopEffect().SetEffect(BE_FALL_PARTICUL,
				int(x+sx),y,
				BE_SPEED((sx/6),sy),32*0x10000+49,
				BLEND_STANDARD);

		if(mActionEffect[i]){
			int x=BATTLE_SQUARE_X+int(BATTLE_SQUARE_SIZE*5);
			int y=BATTLE_SQUARE_Y+int(BATTLE_SQUARE_SIZE*(i+1));
			mActionEffect[i]->SetPos(x+pInput->Rand(5)-2,y);
		}
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////
int CAct_83::OnTimer()				//アースクエイク
{
//定型文////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////
	const int END_TIME = 64;

//	DTest.SetFPS(20);
	int shake=(END_TIME-mModeTimer)/2;
	int nx=int(sin(mModeTimer*2.0)*shake);
	int ny=int(cos(mModeTimer*5.23)*shake);
	int dx,dy;
	SetFieldShake(nx,ny);
	GetFieldShake(&dx,&dy);
	dx=nx-dx;
	dy=ny-dy;

	if(mModeTimer==5){
		int no,i,*target;
		target=GetTarget(&no);

		int *dmg=Attack(
				mActiveCharacter,
				target,no,
				400,MIN_ATK_LV_3+100,
				AT_TYPE_MAGIC,
				AT_ELE_EARTH,AT_OPT_NO_AIR,SE_STONE);

		for(i=0;i<no;i++){
			if(dmg[i]>0){
				GetCharacter(target[i] & 0xffff)->SetGoal(
						GetCharacter(target[i] & 0xffff)->GetX(),
						GetCharacter(target[i] & 0xffff)->GetY(),
						40,true
						);
			}
		}

	}

	if(mModeTimer<=END_TIME-16 && mModeTimer%(16/ACTION_EFFECT_NO)==0){
		int no=(mModeTimer/(16/ACTION_EFFECT_NO))%ACTION_EFFECT_NO;
		int target=ActiveCharacter()->GetTarget();
		int x=pInput->Rand(640);
		int y=80+pInput->Rand(300);
		if(mActionEffect[no]){
			mActionEffect[no]->Destroy();
		}
		mActionEffect[no]=GetBottomEffect().SetEffect(
				BE_STRETCH,
				x,y,15,0);
		for(int i=0;i<8;i++){
			double sx=(pInput->Rand(3000)-1500)/100.0;
			double sy=(pInput->Rand(800)-1200)/100.0;
			GetTopEffect().SetEffect(BE_FALL_PARTICUL,
					int(x+sx*2),y,
					BE_SPEED((sx/6),sy),32*0x10000+49,
					BLEND_STANDARD);
		}
		PlayWave(SE_STONE);
	}
	for(int i=0;i<ACTION_EFFECT_NO;i++){
		if(mActionEffect[i]){
			mActionEffect[i]->SetPos(
					mActionEffect[i]->GetX()+dx,
					mActionEffect[i]->GetY()+dy
				);
			int t=mActionEffect[i]->GetTimer();
			if(t<=4){
				mActionEffect[i]->OperationA(t*32,t*32);
				mActionEffect[i]->SetPos(
					mActionEffect[i]->GetX(),
					mActionEffect[i]->GetY()-16);
			}else if(7<=t && t<=15){
				t=16-t;
				mActionEffect[i]->OperationA(t*16,t*16);
				mActionEffect[i]->SetPos(
					mActionEffect[i]->GetX(),
					mActionEffect[i]->GetY()+8);
			}else if(t>15){
				mActionEffect[i]->Destroy();
				mActionEffect[i]=NULL;
			}
		}
	}

	if(mModeTimer>END_TIME){
		SetFieldShake(0,0);
		return 1;
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////
int CAct_84::OnTimer()				//モールダイブ
{
//定型文////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////
	int no,*target;
	target=GetTarget(&no);

	if(mModeTimer==1){
		for(int i=0;i<no;i++){
			int t=target[i]&0xffff;
			GetCharacter(t)->SetGoal(
					GetCharacter(t)->GetX(),
					GetCharacter(t)->GetY(),
					48,true
					);
		}
	}else if(mModeTimer>=48 && mModeTimer<56){
		for(int i=0;i<no;i++){
			int t=target[i]&0xffff;
			int x=GetCharacter(t)->GetCX();
			int y=GetCharacter(t)->GetGCY();
			double sx=(pInput->Rand(3000)-1500)/100.0;
			double sy=(pInput->Rand(500)-600)/100.0;
			GetTopEffect().SetEffect(BE_FALL_PARTICUL,
					int(x+sx),y,
					BE_SPEED((sx/6),sy),32*0x10000+49,
					BLEND_STANDARD);
		}
		PlayWave(SE_MOVE_BLOCK);
	}else if(mModeTimer==56){
		for(int i=0;i<no;i++){
			SetStatus(mActiveCharacter,target[i]&0xffff,14,100);
		}
		return 1;
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////
int CAct_85::OnTimer()				//メテオスウォーム
{
//定型文////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////
	const int FALL_INTARVAL	=8;
	const int FALL_TIME		=32;
	const int FALL_SPEED	=16;
	const int FALL_NO		=16;

	if(mModeTimer==1){
		mActionParam[0]=new int[FALL_NO];
		mActionParam[1]=new int[FALL_NO];
		memset(mActionParam[0],-1,sizeof(int)*8);
		memset(mActionParam[1],-1,sizeof(int)*8);
		ChangeFieldColor(0xb0000000,FALL_INTARVAL);
	}

	for(int i=0;i<FALL_NO;i++){
		int time=mModeTimer-i*FALL_INTARVAL;
		int dir=1;
		if(CheckEnemy(ActiveCharacter()->GetTarget()))	dir=-dir;
		if(time==2){
			mActionParam[0][i]=4 + (pInput->Rand(4)+1)*dir;
			mActionParam[1][i]=pInput->Rand(4);

			int sx=FALL_SPEED*dir;
			int x=mActionParam[0][i]*BATTLE_SQUARE_SIZE+48-sx*FALL_TIME;
			int y=mActionParam[1][i]*BATTLE_SQUARE_SIZE+64-FALL_SPEED*FALL_TIME;
			mActionEffect[i%ACTION_EFFECT_NO]=
				GetTopEffect().SetEffect(
							BE_MOVE,
							x,y,
							BE_SPEED(sx,FALL_SPEED),62);
		}else if(time==2+FALL_TIME){
			int cx=mActionParam[0][i]*BATTLE_SQUARE_SIZE+48;
			int cy=mActionParam[1][i]*BATTLE_SQUARE_SIZE+64;
			for(int j=0;j<64;j++){
				double sx=(pInput->Rand(10000)-5000)/100.0;
				double sy=(pInput->Rand(10000)-7000)/100.0;
				GetTopEffect().SetEffect(BE_FALL_PARTICUL,
						int(cx+sx),int(cy+sy),
						BE_SPEED((sx/8),(sy/8)),32*0x10000+49,
						BLEND_STANDARD);
			}
			const int SIZE = 1280;
			const int TIME = 64;
			for(int j=0;j<3;j++){
				GetTopEffect().SetEffect(
						BE_LARGE_SPREAD,
						cx,
						cy+32,
						50,(SIZE*j/3) + TIME * 0x10000,BLEND_ADD,0xff40b0ff);
			}

			int	field[BATTLE_SQUARE_WIDTH][BATTLE_SQUARE_Height];
			GetAttackArea(	mActiveCharacter,
							mActionParam[0][i],
							mActionParam[1][i],
							1,
							CR_BIG_CIRCLE,
							field,100);
			int no,*target;
			target=GetTarget(field,-1,&no);

			Attack(	mActiveCharacter,
					target,no,
					40,MIN_ATK_LV_4,
					AT_TYPE_MAGIC,
					AT_ELE_EARTH,0,SE_EXPLOSION);
			PlayWave (SE_EXPLOSION);

			if(i==FALL_NO-1)	BreakTargetLock();
			mActionEffect[i%ACTION_EFFECT_NO]->Destroy();
		}
	}

	if(mModeTimer>=FALL_INTARVAL*FALL_NO+FALL_TIME+20){
		delete[] mActionParam[0];
		mActionParam[0]=NULL;
		delete[] mActionParam[1];
		mActionParam[1]=NULL;
		return 1;
	}
	return 0;
}