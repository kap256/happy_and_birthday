#include "axeaction.h"
#include "input.h"

////////////////////////////////////////////////////////////////////////////
int CAct_29::OnTimer()				//ヘビーアタック
{
//定型文////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////
		const int CHARGE = 5;
	switch(mModeTimer){
		case 1+CHARGE:
			ActiveCharacter()->ChangeAni(9*ANI_TYPE,8,true);
			ActiveCharacter()->SetGoal(
				ActiveCharacter()->GetX(),
				ActiveCharacter()->GetY(),
				15,true);
			break;
		case 12+CHARGE:
			int no,*target;
			target=GetTarget(&no);

			ActiveCharacter()->ChangeAni(11*ANI_TYPE,12,true);

			int turn;
			if(CheckEnemy(mActiveCharacter))	turn=TURN_HORIZONTAL;
			else	turn=0;
			GetTopEffect().SetEffect(BE_STANDARD,
				GetCharacter(ActiveCharacter()->GetTarget())->GetCX(),
				GetCharacter(ActiveCharacter()->GetTarget())->GetCY(),
				3,turn,BLEND_ADD);

			Attack(	mActiveCharacter,
					target,no,
					160,MIN_ATK_LV_0,
					AT_TYPE_WEAPON,
					AT_ELE_NONE,AT_OPT_DEF_BREAK_1,SE_CHOP);
			BreakTargetLock();

			break;
		case 15+CHARGE:
			ActiveCharacter()->ChangeAni(10*ANI_TYPE,18,true);
			break;

		case 35+CHARGE:
			return 1;	break;
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////
int CAct_30::OnTimer()				//衝撃波
{
//定型文////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////
		const int START = 13;
	const int SIZE = 480;
	const int TIME = 12;
	switch(mModeTimer){
		case 1:
			ActiveCharacter()->ChangeAni(9*ANI_TYPE,40,true);
			break;

		case 10:
			ActiveCharacter()->ChangeAni(11*ANI_TYPE,12,true);
			break;

		case START:
			int no,*target;
			target=GetTarget(&no);

			ActiveCharacter()->ChangeAni(10*ANI_TYPE,16,true);

			int turn;
			if(CheckEnemy(mActiveCharacter))	turn=TURN_HORIZONTAL;
			else	turn=0;
			GetBottomEffect().SetEffect(
				BE_LARGE_SPREAD,
				GetCharacter(ActiveCharacter()->GetTarget())->GetCX(),
				GetCharacter(ActiveCharacter()->GetTarget())->GetGCY()+32,
				22,SIZE + TIME*0x10000,BLEND_ADD);

			Attack(	mActiveCharacter,
					target,no,
					90,MIN_ATK_LV_1,
					AT_TYPE_WEAPON,
					AT_ELE_NONE,0,-1);
			BreakTargetLock();

			PlayWave(SE_WAVE);

			break;

		case 35:
			return 1;	break;
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////
int CAct_31::OnTimer()				//吹き飛ばし
{
//定型文////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////
	switch(mModeTimer){
		case 1:
			ActiveCharacter()->ChangeAni(10*ANI_TYPE,16,true);
			break;
		case 10:
			int no,*target,*hit;;
			target=GetTarget(&no);

			ActiveCharacter()->ChangeAni(11*ANI_TYPE,12,true);

			int movex;
			int turn;
			if(CheckEnemy(mActiveCharacter)){
				movex=BATTLE_SQUARE_WIDTH;
				turn=0;
			}else{
				turn=TURN_HORIZONTAL;
				movex=-1;
			}

			GetTopEffect().SetEffect(BE_STANDARD,
				GetCharacter(ActiveCharacter()->GetTarget())->GetCX(),GetCharacter(ActiveCharacter()->GetTarget())->GetCY(),
				1,turn,BLEND_ADD);

			hit=Attack(	mActiveCharacter,
					target,no,
					160,MIN_ATK_LV_1,
					AT_TYPE_WEAPON,
					AT_ELE_NONE,0,SE_HIT);

			for(int i=0;i<no;i++){
				if(hit[i]<0)	continue;
				int t=target[i]&0xffff;
				MoveCharacter(t,movex,GetCharacter(t)->GetSY());
				GetCharacter(t)->SetGoalSquare(movex,GetCharacter(t)->GetSY(),25,GetCharacter(t)->GetSize(),true);
			}
			BreakTargetLock();
			break;
		case 13:
			ActiveCharacter()->ChangeAni(9*ANI_TYPE,22,true);
			break;

		case 40:
			return 1;	break;
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////
int CAct_32::OnTimer()				//スピニングブレイク
{
//定型文////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////
	const int CHARGE = 12;
	const int SPIN_SPEED = 2;
	switch(mModeTimer){
		case 1:
			ActiveCharacter()->ChangeAni(6*ANI_TYPE,CHARGE,true);
			break;
		case CHARGE:
			PlayWave(SE_MISS);
			ActiveCharacter()->ChangeAni(9*ANI_TYPE,8,true);
			ActiveCharacter()->SetGoal(
				ActiveCharacter()->GetX(),
				ActiveCharacter()->GetY(),
				28,true);
			break;
		case 1*SPIN_SPEED+CHARGE:
			ActiveCharacter()->ChangeAni(11*ANI_TYPE,16,true);
			break;
		case 2*SPIN_SPEED+CHARGE:
			ActiveCharacter()->ChangeAni(10*ANI_TYPE,12,true);
		case 3*SPIN_SPEED+CHARGE:
			PlayWave(SE_MISS);
			ActiveCharacter()->SetTurn(true);
			ActiveCharacter()->ChangeAni(10*ANI_TYPE+1,16,true);
			break;
		case 4*SPIN_SPEED+CHARGE:
			ActiveCharacter()->ChangeAni(11*ANI_TYPE+1,12,true);
			break;
		case 5*SPIN_SPEED+CHARGE:
			ActiveCharacter()->ChangeAni(9*ANI_TYPE+1,12,true);
			break;
		case 6*SPIN_SPEED+CHARGE:
			PlayWave(SE_MISS);
			ActiveCharacter()->SetTurn(false);
			ActiveCharacter()->ChangeAni(9*ANI_TYPE,16,true);
			break;
		case 7*SPIN_SPEED+CHARGE:
			ActiveCharacter()->ChangeAni(11*ANI_TYPE,16,true);
			break;
		case 8*SPIN_SPEED+CHARGE:
			ActiveCharacter()->ChangeAni(10*ANI_TYPE,12,true);
		case 9*SPIN_SPEED+CHARGE:
			PlayWave(SE_MISS);
			ActiveCharacter()->SetTurn(true);
			ActiveCharacter()->ChangeAni(10*ANI_TYPE+1,16,true);
			break;
		case 10*SPIN_SPEED+CHARGE:
			ActiveCharacter()->ChangeAni(11*ANI_TYPE+1,12,true);
			break;
		case 11*SPIN_SPEED+CHARGE:
			ActiveCharacter()->ChangeAni(9*ANI_TYPE+1,12,true);
			break;
		case 12*SPIN_SPEED+CHARGE:
			ActiveCharacter()->SetTurn(false);
			ActiveCharacter()->ChangeAni(9*ANI_TYPE,16,true);
			break;
		case  13*SPIN_SPEED+CHARGE:
			ActiveCharacter()->ChangeAni(11*ANI_TYPE,12,true);
			int no,*target;
			target=GetTarget(&no);

			int turn;
			if(CheckEnemy(mActiveCharacter))	turn=TURN_HORIZONTAL;
			else	turn=0;
			GetTopEffect().SetEffect(BE_STANDARD,
				GetCharacter(ActiveCharacter()->GetTarget())->GetCX(),
				GetCharacter(ActiveCharacter()->GetTarget())->GetCY(),
				3,turn,BLEND_ADD);

			Attack(	mActiveCharacter,
					target,no,
					250,MIN_ATK_LV_1,
					AT_TYPE_WEAPON,
					AT_ELE_NONE,AT_OPT_DEF_BREAK_1,SE_CHOP);
			BreakTargetLock();
			break;
		case  14*SPIN_SPEED+CHARGE:
			ActiveCharacter()->ChangeAni(10*ANI_TYPE,25,true);
			break;

		case 14*SPIN_SPEED+CHARGE+30:
			return 1;	break;
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////
CAct_33_34::CAct_33_34(int param,int img)
{
	mParam=param;
	mImg=img;
}
int CAct_33_34::OnTimer()			//矛盾砕き
{
//定型文////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////
	switch(mModeTimer){
		case 1:
			int t;
			t=ActiveCharacter()->GetTarget();
			ActiveCharacter()->ChangeAni(9*ANI_TYPE,64,true);
			mActionEffect[0]=GetTopEffect().SetEffect(BE_ANIMATION,
				GetCharacter(t)->GetCX(),GetCharacter(t)->GetCY()-16,
				mImg,0,BLEND_STANDARD,0x00ffffff);
			PlayWave(SE_MAGIC);
			break;
		case 44:
			int no,*target,*hit;
			target=GetTarget(&no);

			ActiveCharacter()->ChangeAni(11*ANI_TYPE,12,true);

			int turn;
			if(CheckEnemy(mActiveCharacter))	turn=TURN_HORIZONTAL;
			else	turn=0;
			GetTopEffect().SetEffect(BE_STANDARD,
				GetCharacter(ActiveCharacter()->GetTarget())->GetCX(),GetCharacter(ActiveCharacter()->GetTarget())->GetCY(),
				3,turn,BLEND_ADD);

			hit=Attack(	mActiveCharacter,
					target,no,
					250,MIN_ATK_LV_2,
					AT_TYPE_WEAPON,
					AT_ELE_NONE,0,SE_HARD_CHOP);

			for(int i=0;i<no;i++){
				if(hit[i]<0)	continue;
				int down=0;
				switch(mParam){
					case UP_ATK:
						down=int(GetCharacter(target[i]&0xffff)->GetBaseAtk()*0.3);	break;
					case UP_DEF:
						down=GetCharacter(target[i]&0xffff)->GetBaseDef()/2;			break;
				}
				ParamChange(mActiveCharacter,target[i]&0xffff,mParam,-down);
			}
			BreakTargetLock();

			mActionEffect[0]->Destroy();
			for(int i=0;i<16;i++){
				int sx=(i%4-2)*2;
				int sy=(i/4-2)*1-1;
				GetTopEffect().SetEffect(BE_BREAK,
					GetCharacter(ActiveCharacter()->GetTarget())->GetCX(),GetCharacter(ActiveCharacter()->GetTarget())->GetCY()-48,
					BE_SPEED(sx,sy),mImg + i*0x10000,
					BLEND_STANDARD,0xb0ffffff);
			}
			break;
		case 47:
			ActiveCharacter()->ChangeAni(10*ANI_TYPE,22,true);
			break;

		case 70:
			return 1;	break;
	}

	if(mActionEffect[0]){
		if(mModeTimer<32){
			D3DCOLOR color=0x06000000*mModeTimer + 0x00ffffff;
			double x=mActionEffect[0]->GetX();
			double y=mActionEffect[0]->GetY();
			mActionEffect[0]->SetPos(x,y-1);
			mActionEffect[0]->SetColor(color);
		}
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////
int CAct_35::OnTimer()				//スカイダイビング
{
//定型文////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////
	const int		MOVE_TIME=80;
	const int		J_START=16;
	const D3DCOLOR	ALPHA=0x80000000;
	switch(mModeTimer){
		case 1:
			ActiveCharacter()->ChangeAni(10*ANI_TYPE,J_START,true);
			break;

		case J_START-3:
			ActiveCharacter()->ChangeAni(11*ANI_TYPE,8,true);
			break;
		case J_START:
			int t;
			t=ActiveCharacter()->GetTarget();
			ActiveCharacter()->SetGoalSquare(
				GetCharacter(t)->GetSX(),
				GetCharacter(t)->GetSY(),
				MOVE_TIME,GetCharacter(t)->GetSize(),true);
			ActiveCharacter()->ChangeAni(9*ANI_TYPE,MOVE_TIME,true);
			ActiveCharacter()->SetTurn(false);
			break;
		case J_START+MOVE_TIME:
			int no,*target;
			target=GetTarget(&no);

			ActiveCharacter()->ChangeAni(10*ANI_TYPE,16,true);
			ActiveCharacter()->SetTurn(false);

			int turn;
			if(CheckEnemy(mActiveCharacter))	turn=TURN_HORIZONTAL;
			else	turn=0;
			GetTopEffect().SetEffect(BE_FLASH,
				0,0,8,0,BLEND_STANDARD,0xffffffff);
			GetTopEffect().SetEffect(BE_STANDARD,
				GetCharacter(ActiveCharacter()->GetTarget())->GetCX(),
				GetCharacter(ActiveCharacter()->GetTarget())->GetCY(),
				3,turn,BLEND_ADD);

			Attack(	mActiveCharacter,
					target,no,
					380,MIN_ATK_LV_2,
					AT_TYPE_WEAPON,
					AT_ELE_NONE,AT_OPT_DEF_BREAK_1 | AT_OPT_ANTI_AIR,SE_HARD_CHOP);
			BreakTargetLock();

			break;
		case J_START+MOVE_TIME+30:
			return 1;	break;
	}

	int starteffect=mModeTimer%ACTION_EFFECT_NO;
	if(J_START<=mModeTimer && mModeTimer<J_START+MOVE_TIME){
		mActionEffect[starteffect]=GetBottomEffect().SetEffect(
			BE_CHARACTER,
			ActiveCharacter()->GetX(),
			ActiveCharacter()->GetY() - ActiveCharacter()->GetZ(),
			mActiveCharacter,0,BLEND_STANDARD,ALPHA | 0x00ffffff);
	}
	for(int i=0;i<ACTION_EFFECT_NO;i++){
		int e=(starteffect+i)%ACTION_EFFECT_NO;
		if(mActionEffect[e]){
			if(i==1){
				mActionEffect[e]->Destroy();
				mActionEffect[e]=NULL;
			}else{
				D3DCOLOR alpha=ALPHA/ACTION_EFFECT_NO*i;
				mActionEffect[e]->SetColor(alpha | 0x00ffffff);
			}
		}
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////
int CAct_36::OnTimer()				//ライフブレイク
{
//定型文////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////
	const int		TIME		= 120;
	const double	MOVE_SPEED	= 0.015;
	const int		BASE_Y		= 32; 
	const int		AFTER_Y		= BASE_Y - int(TIME*TIME*MOVE_SPEED/2);
	switch(mModeTimer){
		case 1:
			int t;
			t=ActiveCharacter()->GetTarget();
			ActiveCharacter()->ChangeAni(9*ANI_TYPE,TIME,true);
			mActionEffect[0]=GetTopEffect().SetEffect(
					BE_STRETCH,
					GetCharacter(t)->GetCX(),
					GetCharacter(t)->GetCY() + BASE_Y,
					34,128+128*0x10000,BLEND_STANDARD);
			break;
		case TIME:
			int no,*target,*hit;
			target=GetTarget(&no);

			ActiveCharacter()->ChangeAni(11*ANI_TYPE,12,true);

			int turn;
			if(CheckEnemy(mActiveCharacter))	turn=TURN_HORIZONTAL;
			else	turn=0;
			GetTopEffect().SetEffect(BE_STANDARD,
				GetCharacter(ActiveCharacter()->GetTarget())->GetCX(),
				GetCharacter(ActiveCharacter()->GetTarget())->GetCY(),
				3,turn,BLEND_ADD);

			hit=Attack(	mActiveCharacter,
					target,no,
					450,MIN_ATK_LV_3+100,
					AT_TYPE_WEAPON,
					AT_ELE_NONE,AT_OPT_LIFE_BREAK,SE_HARD_CHOP);

			mActionEffect[0]->Destroy();
			mActionEffect[0]=NULL;
			for(int i=0;i<16;i++){
				int sx=(i%4-2)*2;
				int sy=(i/4-2)*1-1;
				GetTopEffect().SetEffect(BE_BREAK,
					GetCharacter(ActiveCharacter()->GetTarget())->GetCX(),
					GetCharacter(ActiveCharacter()->GetTarget())->GetCY()+AFTER_Y,
					BE_SPEED(sx,sy),34 + i*0x10000,
					BLEND_STANDARD,0xffffffff);
			}
			BreakTargetLock();

			break;
		case TIME+3:
			ActiveCharacter()->ChangeAni(10*ANI_TYPE,22,true);
			break;

		case TIME+30:
			return 1;	break;
	}

	if(mActionEffect[0]){
		int t=(mModeTimer-TIME);
		double x=mActionEffect[0]->GetX();
		double y=mActionEffect[0]->GetY();
		if(1){
			mActionEffect[0]->SetPos(x,y+t*MOVE_SPEED);
		}

		D3DCOLOR color;
		if(mModeTimer<60){
			color=mModeTimer*3*0x01;
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

		double rad=(t*t*t*0.00005);
		int size=int(cos(rad)*128);
		mActionEffect[0]->OperationA(size,128);
		if(mModeTimer%((mModeTimer+48)/24)==0){
			PlayWave(SE_STAR);
		}
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////
int CAct_37::OnTimer()				//ファーマークラッシュ
{
//定型文////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////
	const int MOVE_TIME = 64;
	const int JUMP_TIME=48;

	int length=200;
	if(mActionParam[0])	length=*mActionParam[0];

	if(mModeTimer==1){
		int target=ActiveCharacter()->GetTarget();
		double cx=ActiveCharacter()->GetCX();
		double cy=ActiveCharacter()->GetCY();
		double tx=GetCharacter(target)->GetCX();
		double ty=GetCharacter(target)->GetCY();
		double rad=atan2(ty-cy,tx-cx);
		double x=cos(rad)*640;
		double y=sin(rad)*640;
		int turn=0;
		if(x>0)	turn=TURN_HORIZONTAL;
		mActionEffect[0]=GetTopEffect().SetEffect(
						BE_MOVE_NO_SPIN,
						int(cx-x),
						int(cy-y),
						BE_SPEED((x/(MOVE_TIME-1)),(y/(MOVE_TIME-1))),60 + turn*0x10000);
		ActiveCharacter()->ChangeAni(6*ANI_TYPE,MOVE_TIME-33,true);

		
		int length=int(hypot(tx-cx,ty-cy)/(640.0/(MOVE_TIME-4)));
		mActionParam[0]=new int;
		*mActionParam[0] = length;
	}else if(mModeTimer==MOVE_TIME-32){
		ActiveCharacter()->SetGoal(
			ActiveCharacter()->GetX(),
			ActiveCharacter()->GetY(),
			36,true);
	}else if(mModeTimer==MOVE_TIME){
		int target=ActiveCharacter()->GetTarget();
		int x=GetCharacter(target)->GetSX();
		int y=GetCharacter(target)->GetSY();
		int s=GetCharacter(target)->GetSize();
		ActiveCharacter()->SetGoalSquare(x,y,length,s,false);
	}else  if(mModeTimer>MOVE_TIME && mModeTimer< MOVE_TIME+length){
		ActiveCharacter()->ChangeAni((mModeTimer%2+2)*ANI_TYPE,2,true);
		ActiveCharacter()->SetSpeedZ(+BATTLE_GRAVITY);
	}else if(mModeTimer==MOVE_TIME+length){
		int no,*target,*dmg;
		target=GetTarget(&no);

			GetTopEffect().SetEffect(BE_STANDARD,
				GetCharacter(ActiveCharacter()->GetTarget())->GetCX(),
				GetCharacter(ActiveCharacter()->GetTarget())->GetCY(),
				1,0,BLEND_ADD);

		dmg=Attack(	mActiveCharacter,
				target,no,
				200,MIN_ATK_LV_4,
				AT_TYPE_WEAPON,
				AT_ELE_NONE , AT_OPT_DEF_BREAK_1  , SE_HARD_HIT);
		for(int i=0;i<no;i++){
			if(dmg[i]>0){
				int t=target[i]&0xffff;
				GetCharacter(t)->SetGoal(
					GetCharacter(t)->GetX(),
					GetCharacter(t)->GetY(),
					JUMP_TIME,true);
			}
		}
		ActiveCharacter()->SetGoal(
			ActiveCharacter()->GetX(),
			ActiveCharacter()->GetY(),
			JUMP_TIME,true);
		ActiveCharacter()->ChangeAni(9*ANI_TYPE,JUMP_TIME,true);
	}else if(mModeTimer==MOVE_TIME+length+JUMP_TIME-4){
		ActiveCharacter()->ChangeAni(11*ANI_TYPE,JUMP_TIME,true);
	}else if(mModeTimer==MOVE_TIME+length+JUMP_TIME){
		int no,*target;
		target=GetTarget(&no);

		GetTopEffect().SetEffect(BE_FLASH,
			0,0,8,0,BLEND_STANDARD,0xffffffff);
		mActionEffect[1] = GetBottomEffect().SetEffect(
				BE_LARGE_STRETCH,
				GetCharacter(ActiveCharacter()->GetTarget())->GetCX(),
				GetCharacter(ActiveCharacter()->GetTarget())->GetGCY()+32,
				22,0,BLEND_ADD);

		Attack(	mActiveCharacter,
				target,no,
				400,MIN_ATK_LV_4,
				AT_TYPE_WEAPON,
				AT_ELE_NONE,AT_OPT_DEF_BREAK_1,SE_EXPLOSION);
		ActiveCharacter()->ChangeAni(10*ANI_TYPE,JUMP_TIME,true);
		
		BreakTargetLock();
	}

	if(mActionEffect[1]){
		const int TIME=32;
		const int SIZE=960;
		int t=mModeTimer-(MOVE_TIME+length+JUMP_TIME);
		if(t>TIME){
			mActionEffect[1]->Destroy();
			mActionEffect[1]=NULL;
			delete mActionParam[0];
			return 1;
		}else{
			int size=t*SIZE/TIME;
			D3DCOLOR color=((0xff000000/TIME*(TIME-t)) & 0xff000000) + 0xffffff;
			mActionEffect[1]->OperationA(size,size);
			mActionEffect[1]->SetColor(color);
		}
	}
	if(mActionEffect[0]){
		if(mModeTimer==MOVE_TIME*2){
			mActionEffect[0]->Destroy();
			mActionEffect[0]=NULL;
		}else{
			if(mModeTimer%6==0){
				PlayWave(SE_SEED_IN_EARTH);
			}
			double sx=(pInput->Rand(3000)-1500)/100.0;
			double sy=(pInput->Rand(500)-600)/100.0;
			GetTopEffect().SetEffect(BE_FALL_PARTICUL,
					int(mActionEffect[0]->GetX())+pInput->Rand(48)-24,
					int(mActionEffect[0]->GetY())+16,
					BE_SPEED((sx/6),sy),32*0x10000+49,
					BLEND_STANDARD);
		}
	}
	return 0;
}