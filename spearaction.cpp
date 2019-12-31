#include "spearaction.h"
#include "input.h"

////////////////////////////////////////////////////////////////////////////
int CAct_19::OnTimer()				//‚È‚¬•¥‚¢
{
//’èŒ^•¶////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////
	switch(mModeTimer){
		case 1:
			ActiveCharacter()->SetGoal(
				ActiveCharacter()->GetX(),
				ActiveCharacter()->GetY()+60,
				12,true);
			break;
		case 16:
			ActiveCharacter()->ChangeAni(10*ANI_TYPE+1,16,true);
			break;
		case 24:
			ActiveCharacter()->SetGoal(
				ActiveCharacter()->GetX(),
				ActiveCharacter()->GetY()-120,
				4,false);

			int no,*target;
			target=GetTarget(&no);

			ActiveCharacter()->ChangeAni(11*ANI_TYPE+1,12,true);

			int turn;
			if(CheckEnemy(mActiveCharacter))	turn=0;
			else	turn=TURN_HORIZONTAL;

			mActionEffect[0]=GetTopEffect().SetEffect(
				BE_STRETCH,
				GetCharacter(ActiveCharacter()->GetTarget())->GetCX(),
				GetCharacter(ActiveCharacter()->GetTarget())->GetCY()+240,
				17,64+0*0x10000,BLEND_ADD);

			Attack(	mActiveCharacter,
					target,no,
					120,MIN_ATK_LV_1,
					AT_TYPE_WEAPON,
					AT_ELE_NONE,0,SE_THRUST);

			BreakTargetLock();
			break;
		case 27:
			ActiveCharacter()->ChangeAni(9*ANI_TYPE+1,18,true);
			break;

		case 50:
			return 1;	break;
	}
	if(mActionEffect[0]){
		if(mModeTimer<30){
			int size=(mModeTimer-24)*96;
			mActionEffect[0]->OperationA(64,size);
			mActionEffect[0]->SetPos(
				mActionEffect[0]->GetX(),
				mActionEffect[0]->GetY()-48);
		}else{
			mActionEffect[0]->Destroy();
			mActionEffect[0]=NULL;
		}
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////
int CAct_20::OnTimer()				//ˆê‘M
{
//’èŒ^•¶////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////
	switch(mModeTimer){
		case 10:
			PlayWave(SE_MISS);
			break;
		case 30:
			int no,*target;
			target=GetTarget(&no);

			ActiveCharacter()->ChangeAni(12*ANI_TYPE+2,24,true);

			int turn;
			if(CheckEnemy(mActiveCharacter))	turn=TURN_HORIZONTAL;
			else	turn=0;
			mActionEffect[0]=GetTopEffect().SetEffect(
						BE_STRETCH_ROTATE,
						GetCharacter(target[0]&0xffff)->GetCX(),
						GetCharacter(target[0]&0xffff)->GetCY(),
						17+(90)*0x10000,128+0*0x10000,BLEND_ADD);

			Attack(	mActiveCharacter,
					target,no,
					120,MIN_ATK_LV_1,
					AT_TYPE_WEAPON,
					AT_ELE_NONE,AT_OPT_CRT_UP,SE_THRUST);

			ActiveCharacter()->SetGoal(
				(turn?600:-40),
				ActiveCharacter()->GetY(),
				4,false);

			BreakTargetLock();
			break;
		case 60:
			return 1;	break;
	}

	if(mActiveCharacter < MAX_PARTY_NUMBER){
		if(mModeTimer==1){
			ActiveCharacter()->ChangeAni(9*ANI_TYPE+2,64,true);
		}else	if(10<mModeTimer && mModeTimer<=18){
			int t=mModeTimer-10;
			ActiveCharacter()->TurnWeapon(t*2*3.141592/8);
		}
	}else{
		switch(mModeTimer){
			case 1:
				ActiveCharacter()->ChangeAni(10*ANI_TYPE+1,64,true);	break;
			case 10:
				ActiveCharacter()->ChangeAni(11*ANI_TYPE+1,64,true);	break;
			case 13:
				ActiveCharacter()->ChangeAni(9*ANI_TYPE+1,64,true);	break;
			case 20:
				ActiveCharacter()->ChangeAni(9*ANI_TYPE+2,64,true);	break;
		}
	}
	if(mActionEffect[0]){
		const int TIME = 16;
		const int SIZE = 2400;
		int time=mActionEffect[0]->GetTimer();
		if(time<TIME){
			mActionEffect[0]->OperationA(128,time*SIZE/TIME);
			if(time>TIME/2){
				mActionEffect[0]->SetColor((0xff*(TIME-time)/(TIME/2))*0x01000000|0xffffff);
			}
		}else{
			mActionEffect[0]->Destroy();
			mActionEffect[0]=NULL;
		}
	}

	return 0;
}
////////////////////////////////////////////////////////////////////////////
int CAct_21::OnTimer()				//‘«•¥‚¢
{
//’èŒ^•¶////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////
	switch(mModeTimer){
		case 1:
			ActiveCharacter()->ChangeAni(6*ANI_TYPE,16,true);
			break;
		case 10:
			int no,*target,*hit;
			target=GetTarget(&no);

			ActiveCharacter()->ChangeAni(11*ANI_TYPE,12,true);

			int turn;
			if(CheckEnemy(mActiveCharacter))	turn=TURN_HORIZONTAL;
			else	turn=0;

			GetTopEffect().SetEffect(BE_STANDARD,
				GetCharacter(ActiveCharacter()->GetTarget())->GetCX(),GetCharacter(ActiveCharacter()->GetTarget())->GetCY(),
				24,turn,BLEND_ADD);

			target=GetTarget(&no);

			hit=Attack(	mActiveCharacter,
						target,no,
						140,MIN_ATK_LV_1,
						AT_TYPE_WEAPON,
						AT_ELE_NONE,AT_OPT_NO_AIR,SE_HIT);

			for(int i=0;i<no;i++){
				if(hit[i]<0)	continue;
				int t=target[i]&0xffff;
				SetCancel(t,100);
			}
			
			BreakTargetLock();

			break;
		case 13:
			ActiveCharacter()->ChangeAni(10*ANI_TYPE,16,true);
			break;

		case 35:
			return 1;	break;
	}

	return 0;
}
////////////////////////////////////////////////////////////////////////////
int CAct_22::OnTimer()				//•—n—ô
{
//’èŒ^•¶////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////
	double sx,sy;
	int target=ActiveCharacter()->GetTarget();
	double length=mBattleMode->GetShotSpeed(mActiveCharacter,target,32,&sx,&sy);



	switch(mModeTimer){
		case 1:
			ActiveCharacter()->ChangeAni(10*ANI_TYPE+1,16,true);
			break;
		case 14:
			PlayWave(SE_SWING);
			GetTopEffect().SetEffect(BE_MOVE,
							ActiveCharacter()->GetCX(),
							ActiveCharacter()->GetCY(),
							BE_SPEED(sx,sy),14,BLEND_ADD);
			ActiveCharacter()->ChangeAni(11*ANI_TYPE+1,16,true);
			break;
		case 17:
			ActiveCharacter()->ChangeAni(9*ANI_TYPE+1,16,true);
			break;

	}

	if(mModeTimer-14==int(length)){
		int no,*target;
		target=GetTarget(&no);

		Attack(	mActiveCharacter,
				target,no,
				160,MIN_ATK_LV_1+30,
				AT_TYPE_BOW,
				AT_ELE_WIND,0,SE_SLASH);
		BreakTargetLock();
	}
	if(mModeTimer-35>=int(length)){
		return 1;
	}

	return 0;
}
////////////////////////////////////////////////////////////////////////////
int CAct_23::OnTimer()				//‘åŽÔ—Ö
{
//’èŒ^•¶////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////
	const int START = 30;
	if(mModeTimer<=START){
		switch(mModeTimer){
			case 20:
				ActiveCharacter()->ChangeAni(10*ANI_TYPE+1,16,true);
				break;
			case 17:
				ActiveCharacter()->ChangeAni(10*ANI_TYPE,16,true);
				break;
			case 15:
				ActiveCharacter()->ChangeAni(11*ANI_TYPE,12,true);
				break;
			case 13:
				ActiveCharacter()->SetTurn(false);
				ActiveCharacter()->ChangeAni(9*ANI_TYPE,18,true);
				break;
			case 11:
				ActiveCharacter()->ChangeAni(9*ANI_TYPE+1,16,true);
				break;
			case 9:
				ActiveCharacter()->ChangeAni(11*ANI_TYPE+1,12,true);
				break;
			case 7:
				ActiveCharacter()->SetTurn(true);
				ActiveCharacter()->ChangeAni(10*ANI_TYPE+1,12,true);
				break;
			case 5:
				ActiveCharacter()->ChangeAni(10*ANI_TYPE,16,true);
				break;
			case 3:
				ActiveCharacter()->ChangeAni(11*ANI_TYPE,12,true);
				break;
			case 1:
				PlayWave(SE_GUST);
				ActiveCharacter()->ChangeAni(9*ANI_TYPE,20,true);
				break;
		}
	}else if(mModeTimer<START+20){
		const int SPIN_X=100;
		const int SPIN_Y=70;
		int turn;
		int move;
		if(CheckEnemy(mActiveCharacter)){
			turn=TURN_HORIZONTAL;
			move=1;
		}else{
			turn=0;
			move=-1;
		}
		switch(mModeTimer-START){
			case 1:
				ActiveCharacter()->ChangeAni(10*ANI_TYPE+1,16,true);
				ActiveCharacter()->SetPos(
					ActiveCharacter()->GetX()+SPIN_X*move,
					ActiveCharacter()->GetY(),0);
				break;
			case 3:
				ActiveCharacter()->ChangeAni(11*ANI_TYPE+1,12,true);
				break;
			case 5:
				ActiveCharacter()->ChangeAni(9*ANI_TYPE+1,18,true);

				int no,*target;
				target=GetTarget(&no);

				Attack(	mActiveCharacter,
						target,no,
						170,MIN_ATK_LV_2,
						AT_TYPE_WEAPON,
						AT_ELE_NONE,0,SE_THRUST);

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
		}
		
		//if(mModeTimer%2==1){
			if(pInput->Rand(2)){
				turn=turn^TURN_HORIZONTAL;
				turn=turn | TURN_VERTICAL;
			}
			GetTopEffect().SetEffect(BE_LARGE,
				ActiveCharacter()->GetCX()+pInput->Rand(SPIN_X*2)-SPIN_X,
				ActiveCharacter()->GetCY()+pInput->Rand(SPIN_Y*2)-SPIN_Y,
				16,turn,BLEND_ADD);
		//}
		double rad=mModeTimer*2*3.141592/20.0;
		ActiveCharacter()->SetPos(
			int(ActiveCharacter()->GetX()+SPIN_X*sin(rad)*move/3),
			int(ActiveCharacter()->GetY()-SPIN_Y*cos(rad)*move/3),0);
	}else if(mModeTimer>START+50){
		return 1;
	}

	return 0;
}
////////////////////////////////////////////////////////////////////////////
int CAct_24::OnTimer()				//•s“®“Ë‚«
{
//’èŒ^•¶////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////
	const int STOP=40;
	const int SMASH=30;
	const int MOVE=40;
	int turn;
	if(CheckEnemy(mActiveCharacter))	turn=1;
	else	turn=-1;
	switch(mModeTimer){
		case 1:
			{
				int target=ActiveCharacter()->GetTarget();
				int x=ActiveCharacter()->GetSX();
				int y=ActiveCharacter()->GetSY();
				int s=ActiveCharacter()->GetSize();
				GetCharacter(target)->ChangeAni(-1,0,true);
				GetCharacter(target)->SetGoalSquare(x,y,50,s,false);
				GetCharacter(target)->ChangeAni(0*ANI_TYPE,64,true);
			}
			PlayWave(SE_GRAVITY);
			ActiveCharacter()->ChangeAni(9*ANI_TYPE+2,64,true);
			ChangeFieldColor(0x80000000,48);
			break;
		case 60:
			int no,*target,*hit;
			target=GetTarget(&no);

			ActiveCharacter()->ChangeAni(12*ANI_TYPE+1,64,true);
			ActiveCharacter()->SetGoal(
				ActiveCharacter()->GetX()+turn*MOVE,
				ActiveCharacter()->GetY(),
				1,0,true);
			GetTopEffect().SetEffect(BE_FLASH,
				0,0,8,0,BLEND_STANDARD,0xffffffff);

			hit=Attack(	mActiveCharacter,
					target,no,
					420,MIN_ATK_LV_2+50,
					AT_TYPE_WEAPON,
					AT_ELE_NONE,0,SE_EXPLOSION);
			for(int i=0;i<no;i++){
				if(hit[i]<0)	continue;
				GetCharacter(target[i]&0xffff)->ChangeAni(8*ANI_TYPE+2,SMASH+STOP,true);
			}

			break;
		case 60+STOP:
			ChangeFieldColor(0x80000000,7);
			ActiveCharacter()->ChangeAni(11*ANI_TYPE+2,SMASH,true);
			ActiveCharacter()->SetGoal(
				ActiveCharacter()->GetX()+turn*MOVE*4,
				ActiveCharacter()->GetY(),
				4,0,true);
			{
				int target=ActiveCharacter()->GetTarget();
				GetCharacter(target)->Return(SMASH,true,true);
				GetCharacter(target)->SetTurn(true);
			}
			BreakTargetLock();
			break;

		case 60+STOP+SMASH:
			{
				int target=ActiveCharacter()->GetTarget();
				GetCharacter(target)->SetTurn(false);
			}
			return 1;	break;
	}

	return 0;
}
////////////////////////////////////////////////////////////////////////////
int CAct_25::OnTimer()				//‹Á“V“®’n
{
//’èŒ^•¶////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////
	const int START = 60;
	const int MOVE_TIME = 40;
	if(mModeTimer<=START){
		switch(mModeTimer){
			case 1:
				int t;
				t=ActiveCharacter()->GetTarget();
				int x;
				if(CheckEnemy(t)){
					x=240;
				}else{
					x=400;
				}
				ActiveCharacter()->SetGoal(
					x-ActiveCharacter()->GetSize()*BATTLE_SQUARE_SIZE,
					200-ActiveCharacter()->GetSize()*BATTLE_SQUARE_SIZE,
					MOVE_TIME,true);

				ActiveCharacter()->ChangeAni(9*ANI_TYPE,MOVE_TIME,true);
				ActiveCharacter()->SetTurn(false);
				break;
			case MOVE_TIME-2:
				ActiveCharacter()->ChangeAni(11*ANI_TYPE,10,true);
				break;
			case 1+MOVE_TIME:
				int no,*target,*hit;
				target=GetTarget(&no);
				ActiveCharacter()->ChangeAni(10*ANI_TYPE,10,true);

				PlayWave(SE_EXPLOSION);
				hit=Attack(	mActiveCharacter,
						target,no,
						110,MIN_ATK_LV_3,
						AT_TYPE_WEAPON,
						AT_ELE_NONE,AT_OPT_NO_AIR,SE_EXPLOSION);
				for(int i=0;i<no;i++){
					if(hit[i]>0){
						GetCharacter(target[i] & 0xffff)->SetGoal(
							GetCharacter(target[i] & 0xffff)->GetX(),
							GetCharacter(target[i] & 0xffff)->GetY(),
							40,true
						);
					}
				}
				GetBottomEffect().SetEffect(
					BE_LARGE_SPREAD,
					GetCharacter(ActiveCharacter()->GetTarget())->GetCX(),
					GetCharacter(ActiveCharacter()->GetTarget())->GetGCY()+32,
					22,1280 + 20*0x10000,BLEND_ADD);

				break;
			case 10+MOVE_TIME:
				ActiveCharacter()->ChangeAni(10*ANI_TYPE+1,20,true);
				ActiveCharacter()->SetGoal(
							ActiveCharacter()->GetX(),
							ActiveCharacter()->GetY(),
							30,true
						);

		}
		if(mModeTimer>MOVE_TIME){
			int shake=(START-mModeTimer)/2;
			int nx=int(sin(mModeTimer*2.0)*shake);
			int ny=int(cos(mModeTimer*5.23)*shake);
			SetFieldShake(nx,ny);
		}else if(mModeTimer<MOVE_TIME-2){
			ActiveCharacter()->ChangeAni((mModeTimer%2+2)*ANI_TYPE,2,true);
		}

	}else if(mModeTimer<START+20){
		const int SPIN_X=100;
		const int SPIN_Y=70;
		int turn;
		int move;
		if(CheckEnemy(ActiveCharacter()->GetTarget())){
			turn=0;
			move=-1;
		}else{
			turn=TURN_HORIZONTAL;
			move=1;
		}
		if(mModeTimer%4==0)	PlayWave(SE_SLASH);
		switch(mModeTimer-START){
			case 1:
				ActiveCharacter()->ChangeAni(10*ANI_TYPE+1,16,true);
				break;
			case 3:
				ActiveCharacter()->ChangeAni(11*ANI_TYPE+1,12,true);
				break;
			case 5:
				ActiveCharacter()->ChangeAni(9*ANI_TYPE+1,18,true);

				int no,*target,*hit;
				target=GetTarget(&no);

				ActiveCharacter()->SetGoal(
							ActiveCharacter()->GetX(),
							ActiveCharacter()->GetY(),
							20,true
						);
				hit=Attack(	mActiveCharacter,
						target,no,
						110,MIN_ATK_LV_3,
						AT_TYPE_WEAPON,
						AT_ELE_NONE,AT_OPT_ANTI_AIR,SE_THRUST);
				for(int i=0;i<no;i++){
					if(hit[i]>0){
						GetCharacter(target[i] & 0xffff)->SetGoal(
							GetCharacter(target[i] & 0xffff)->GetX(),
							GetCharacter(target[i] & 0xffff)->GetY(),
							20,true
						);
					}
				}
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
		}
		
		int x=ActiveCharacter()->GetCX()+pInput->Rand(SPIN_X*2)-SPIN_X;
		int y=ActiveCharacter()->GetCY()+pInput->Rand(SPIN_Y*2)-SPIN_Y;
		//int x=pInput->Rand(320)+160+move*160;
		//int y=pInput->Rand(320)-ActiveCharacter()->GetZ();

		GetTopEffect().SetEffect(BE_STANDARD,
			x,y,
			28,turn,BLEND_ADD);


		double rad=mModeTimer*2*3.141592/20.0;
		ActiveCharacter()->SetPos(
			int(ActiveCharacter()->GetX()+SPIN_X*sin(rad)*move/3),
			int(ActiveCharacter()->GetY()-SPIN_Y*cos(rad)*move/3),
			ActiveCharacter()->GetZ());
	}else if(mModeTimer>START+60){
		return 1;
	}

	return 0;
}
////////////////////////////////////////////////////////////////////////////
int CAct_26::OnTimer()				//ƒJƒ~ƒJƒ[ƒXƒgƒ‰ƒCƒN
{
//’èŒ^•¶////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////

	const int CHARGE		= 64;
	const int AIR_INTERVAL	= 2;

	int turn;
	if(CheckEnemy(mActiveCharacter))	turn =  1;
	else								turn = -1;
	if(mModeTimer==1){
		int target=ActiveCharacter()->GetTarget();
		int x=GetCharacter(target)->GetSX();
		int y=GetCharacter(target)->GetSY();
		int s=GetCharacter(target)->GetSize();
		ActiveCharacter()->SetGoalSquare(x,y,512,s,false);
	}else if(mModeTimer< CHARGE){
		if(mModeTimer % AIR_INTERVAL==0){
			int n=(mModeTimer/AIR_INTERVAL)%(8-1) + 1 ;
			int x=ActiveCharacter()->GetCX() + pInput->Rand(96)-48;
			int y=40+ pInput->Rand(32)-16;
			CBattleEffect *e;
			if(y>40)	e=&GetTopEffect();
			else		e=&GetBottomEffect();
			int turn=0;
			if(CheckEnemy(mActiveCharacter))		turn=TURN_HORIZONTAL;
			mActionEffect[n]=e->SetEffect(BE_STRETCH,
				x,ActiveCharacter()->GetGCY()+y,
				61 + turn*0x10000,0,BLEND_ADD);
		}
		if(mModeTimer % 6==0){
			PlayWave(SE_GUST);
		}
	}else if (mModeTimer== CHARGE){
		int t=ActiveCharacter()->GetTarget();
		int px=ActiveCharacter()->GetX();
		int py=ActiveCharacter()->GetY();
		int tx=GetCharacter(t)->GetX();
		int ty=GetCharacter(t)->GetY();
		int ps=ActiveCharacter()->GetSize();
		int x=int((tx-px)*1.3) + px;
		int y=int((ty-py)*1.3) + py;

		ActiveCharacter()->SetGoal(x,y,2,false);

		int no,*target,*hit;
		target=GetTarget(&no);

		GetTopEffect().SetEffect(BE_FLASH,
			0,0,8,0,BLEND_STANDARD,0xffffffff);
		int rad=int(atan2(double(ty-py),double(tx-px)) * 100);
		mActionEffect[0]=GetTopEffect().SetEffect(
					BE_STRETCH_ROTATE,
					GetCharacter(t)->GetCX(),
					GetCharacter(t)->GetCY(),
					17+(rad*360/314/2+90)*0x10000,128+0*0x10000,BLEND_ADD);

		hit=Attack(	mActiveCharacter,
				target,no,
				700,MIN_ATK_LV_3+100,
				AT_TYPE_WEAPON,
				AT_ELE_NONE,0,SE_HARD_THRUST);
		for(int i=0;i<no;i++){
			if(hit[i]<0)	continue;
			int hp=ActiveCharacter()->GetHP();
			ActiveCharacter()->SetHP(hp-hp/2);
			break;
		}
		BreakTargetLock();
		
		ActiveCharacter()->ChangeAni(11*ANI_TYPE+2,30,true);
	}

	if(mActionEffect[0]){
		const int TIME = 16;
		const int SIZE = 2400;
		int time=mActionEffect[0]->GetTimer();
		if(time<TIME){
			mActionEffect[0]->OperationA(128,time*SIZE/TIME);
			if(time>TIME/2){
				mActionEffect[0]->SetColor((0xff*(TIME-time)/(TIME/2))*0x01000000|0xffffff);
			}
		}else{
			mActionEffect[0]->Destroy();
			mActionEffect[0]=NULL;
		}
	}
	for(int i=1;i<8;i++){
		if(mActionEffect[i]){
			const int TIME=(8-2) * AIR_INTERVAL;
			int t=mActionEffect[i]->GetTimer();
			if(t<TIME){
				int size=t*12;
				mActionEffect[i]->OperationA(size,size);
				mActionEffect[i]->SetPos(mActionEffect[i]->GetX() -turn*4, mActionEffect[i]->GetY() - 5);
				unsigned char alpha= (TIME-t)*255 /TIME;
				mActionEffect[i]->SetColor(D3DCOLOR_ARGB(alpha,255,255,255));
			}else{
				mActionEffect[i]->Destroy();
				mActionEffect[i]=NULL;
			}
		}
	}



	if(mModeTimer>110){
		for(int i=0;i<ACTION_EFFECT_NO;i++){
			if(mActionEffect[i]){
				mActionEffect[i]->Destroy();
				mActionEffect[i]=NULL;
			}
		}
		return 1;
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////
int CAct_27::OnTimer()				//–³ŒÀ—†ù
{
//’èŒ^•¶////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////
	const int HIT_NO	=24;
	const int HIT_TIME	=4;
	const int HIT_START	=16;
	const int HIT_END	=HIT_NO*HIT_TIME+HIT_START;
	
	if(mModeTimer==1){
		PlayWave(SE_GUST);
		ActiveCharacter()->ChangeAni(9*ANI_TYPE+2,64,true);
		ChangeFieldColor(0xb0000000,HIT_START);
	}else	if(1<mModeTimer && mModeTimer<=9){
		int t=mModeTimer-2;
		ActiveCharacter()->TurnWeapon(t*2*3.141592/8);
	}else	if(HIT_START<=mModeTimer && mModeTimer<HIT_END){
		int time=(mModeTimer-HIT_START)%HIT_TIME;
		int t=ActiveCharacter()->GetTarget();
		if(time==0){
			ActiveCharacter()->ChangeAni(9*ANI_TYPE+2,64,true);
			ActiveCharacter()->SetPosSquare(
				GetCharacter(t)->GetSX(),
				GetCharacter(t)->GetSY(),
				GetCharacter(t)->GetSize());
		}else if(time==1){
			ActiveCharacter()->ChangeAni(12*ANI_TYPE+1,64,true);
			int no,*target;
			target=GetTarget(&no);
			Attack(	mActiveCharacter,
					target,no,
					20,MIN_ATK_LV_4,
					AT_TYPE_WEAPON,
					AT_ELE_NONE,0,SE_THRUST);

			int angle=int(90+sin(mModeTimer/4.0)*10);
			ActiveCharacter()->SetGoal(
				int(GetCharacter(t)->GetCX()-sin(angle*3.141592/180)*40)-64,
				int(GetCharacter(t)->GetGCY()+cos(angle*3.141592/180)*40)-64,
				2,false);
			GetTopEffect().SetEffect(
				BE_TURN_SPREAD_SLASH,
				GetCharacter(target[0]&0xffff)->GetCX(),
				GetCharacter(target[0]&0xffff)->GetCY(),
				17+angle*0x10000,3000+32*0x10000,BLEND_ADD);
		}else if(time==2){
			ActiveCharacter()->ChangeAni(11*ANI_TYPE+2,64,true);
		}else if(time==3){
		}
	}else	if(mModeTimer>=HIT_END){
		if(mModeTimer==HIT_END){
			int t=ActiveCharacter()->GetTarget();
			PlayWave(SE_GUST);
			ActiveCharacter()->ChangeAni(9*ANI_TYPE+2,64,true);
			ActiveCharacter()->SetPosSquare(
				GetCharacter(t)->GetSX(),
				GetCharacter(t)->GetSY(),
				GetCharacter(t)->GetSize());
		}
		if(mModeTimer<=HIT_END+16){
			int t=mModeTimer-HIT_END;
			ActiveCharacter()->TurnWeapon(t*2*3.141592/8);
		}
		switch(mModeTimer){
			case HIT_END+32:
				ActiveCharacter()->ChangeAni(12*ANI_TYPE+1,64,true);
				int no,*target;
				target=GetTarget(&no);
				
				int turn;
				turn=1;
				ActiveCharacter()->SetGoal(
					int(GetCharacter(ActiveCharacter()->GetTarget())->GetCX()-turn*100-64),
					int(GetCharacter(ActiveCharacter()->GetTarget())->GetGCY()-64),
					2,false);
				Attack(	mActiveCharacter,
						target,no,
						250,MIN_ATK_LV_4,
						AT_TYPE_WEAPON,
						AT_ELE_NONE,0,SE_HARD_THRUST);
				
				GetTopEffect().SetEffect(
					BE_TURN_SPREAD_SLASH,
					GetCharacter(target[0]&0xffff)->GetCX(),
					GetCharacter(target[0]&0xffff)->GetCY(),
					17+90*0x10000,3000+32*0x10000,BLEND_ADD);
				
				GetTopEffect().SetEffect(BE_FLASH,
					0,0,8,0,BLEND_STANDARD,0xffffffff);
				BreakTargetLock();
				break;
			case HIT_END+34:
				ActiveCharacter()->ChangeAni(11*ANI_TYPE+2,16,true);
				break;

			case HIT_END+55:
				return 1;	break;
		}
	}

	return 0;
}