#include "windaction.h"
#include "input.h"

////////////////////////////////////////////////////////////////////////////
int CAct_70::OnTimer()				//ウインドカッター
{
//定型文////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////
	int target=ActiveCharacter()->GetTarget();
	int sy=GetCharacter(target)->GetSY()+GetCharacter(target)->GetSize()-1;
	if(mModeTimer==8){
		ActiveCharacter()->SetGoalSquare(4,sy,16,1,false);
	}
	if(mModeTimer==32){
		ActiveCharacter()->ChangeAni(13*ANI_TYPE,16,true);
		int turn;
		if(!CheckEnemy(mActiveCharacter,target))	turn=0;
		else	turn=TURN_HORIZONTAL;
		mActionEffect[0]=GetTopEffect().SetEffect(
							BE_STRETCH,
							ActiveCharacter()->GetCX(),
							ActiveCharacter()->GetCY()+16,
							13+turn*0x10000,128,BLEND_ADD);
	}
	if(32<mModeTimer && mModeTimer<48){
		mActionEffect[0]->OperationA(128,(mModeTimer-32)*8);
	}
	if(mModeTimer==64){
		ActiveCharacter()->ChangeAni(13*ANI_TYPE,16,true);
		mActionEffect[0]->Destroy();

		int sx;
		if(!CheckEnemy(mActiveCharacter,target))	sx=-1;
		else	sx=1;
		GetTopEffect().SetEffect(	BE_MOVE,
							ActiveCharacter()->GetCX(),
							ActiveCharacter()->GetCY()+16,
							BE_SPEED((sx*BATTLE_SQUARE_SIZE/2),0),14,
							BLEND_ADD);
	}
	if(mModeTimer>=64 && mModeTimer%2==0){
		int x=(mModeTimer-64)/2;
		if(!CheckEnemy(mActiveCharacter,target))	x=x*-1;
		int no,*target;
		target=GetTarget(&no);

		ChargeBreak();	//チャージ消去

		for(int i=0;i<no;i++){
			if(GetCharacter(target[i]&0xffff)->GetSX()==4+x){
				Attack(	mActiveCharacter,
						target[i]&0xffff,
						120,MIN_ATK_LV_1,
						AT_TYPE_MAGIC,
						AT_ELE_WIND,0,SE_WIND,
						target[i]/0x10000);
			}
		}
		if(mModeTimer>=64+10)	BreakTargetLock();
	}

	if(mModeTimer>=80){
		return 1;
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////
int CAct_71::OnTimer()				//ガストウインド
{
//定型文////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////
	if(mModeTimer==1){
		int no,*target;
		target=GetTarget(&no);

		mActionParam[0]=new int;
		*mActionParam[0]=no;
		mActionParam[1]=new int[no];
		memcpy(mActionParam[1],target,sizeof(int)*no);
	}

	int no=*mActionParam[0];
	int *target=mActionParam[1];
	int dir=(CheckEnemy(mActiveCharacter,ActiveCharacter()->GetTarget())?1:-1);

	if(mModeTimer==1){
		PlayWave(SE_GUST);
	}
	if(mModeTimer<20){
		for(int i=0;i<2;i++){
			int x=320-dir*340;
			int y=pInput->Rand(480);
			double sx=-(pInput->Rand(1500)-750)/100.0+dir*40;
			double sy=(pInput->Rand(1500)-750)/100.0;
			int spin=pInput->Rand(32)+16;
			GetTopEffect().SetEffect(BE_SMALL_MOVE_SPIN,
						x,y,BE_SPEED(sx,sy),30+spin*0x10000);
		}
	}
	if(10<=mModeTimer&&mModeTimer%2==0){
		int x=(mModeTimer-10)/2;

		for(int i=0;i<no;i++){
			if(target[i]<0)	continue;
			int t=target[i]&0xffff;
			if(GetCharacter(t)->GetSX()==4+x*dir){
				int y=GetCharacter(t)->GetSY();
				int x=4+8*dir;
				MoveCharacter(t,x,y,false);
				GetCharacter(t)->SetGoalSquare(x,y,24,GetCharacter(t)->GetSize(),true);
				target[i]=-1;
			}
		}
	}

	if(mModeTimer>60){
		delete mActionParam[0];
		delete[] mActionParam[1];
		mActionParam[0]=NULL;;
		mActionParam[1]=NULL;
		return 1;
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////
int CAct_73::OnTimer()				//エアースラッシュ
{
//定型文////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////
	const int WIND_TIME =32;
	const int SLASH_TIME=32+WIND_TIME;
	const int END_TIME=16+SLASH_TIME;
	int dir=(CheckEnemy(mActiveCharacter,ActiveCharacter()->GetTarget())?1:-1);

	if(mModeTimer<WIND_TIME){
		if(mModeTimer%6==0){
			PlayWave(SE_SWING);
		}
		if(mModeTimer%(WIND_TIME/ACTION_EFFECT_NO)==0){
			int no=mModeTimer/(WIND_TIME/ACTION_EFFECT_NO);
			int x=pInput->Rand(320)+160-dir*80;
			int y=pInput->Rand(320);
			const int size=256;
			mActionEffect[no]=GetTopEffect().SetEffect(
				BE_STRETCH,x,y,
				31+(dir>0?TURN_HORIZONTAL:0)*0x10000,size+size*0x10000,
				BLEND_ADD,0x00ffffff);
		}
	}else	if(mModeTimer<SLASH_TIME){
		int x=pInput->Rand(320)+160+dir*160;
		int y=pInput->Rand(320);

		GetTopEffect().SetEffect(BE_STANDARD,
			x,y,
			28,(dir>0?TURN_HORIZONTAL:0),BLEND_ADD);

		if(mModeTimer%4==2){
			PlayWave(SE_WIND);
		}
	}
	if(mModeTimer==WIND_TIME+6){
		int no,*target;
		target=GetTarget(&no);

		Attack(mActiveCharacter,target,no,
				120,MIN_ATK_LV_2,
				AT_TYPE_MAGIC,
				AT_ELE_WIND,0,SE_WIND);
		BreakTargetLock();
	}

	for(int i=0;i<ACTION_EFFECT_NO;i++){
		if(mActionEffect[i]){
			int t=mModeTimer-i*(WIND_TIME/ACTION_EFFECT_NO);
			if(t>32){
				mActionEffect[i]->Destroy();
				mActionEffect[i]=NULL;
				continue;
			}
			mActionEffect[i]->SetPos(
				mActionEffect[i]->GetX()+dir*32,
				mActionEffect[i]->GetY());
			if(t>8)	t=16-t;
			mActionEffect[i]->SetColor((0x80*t/8)*0x01000000+0xffffff);
		}
	}

	if(mModeTimer>=END_TIME){
		for(int i=0;i<ACTION_EFFECT_NO;i++){
			if(mActionEffect[i])	mActionEffect[i]->Destroy();
		}
		return 1;
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////
int CAct_74::OnTimer()				//ラピッドラビット
{
//定型文////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////
	int no,*target;
	target=GetTarget(&no);

	if(mModeTimer%4==2){
		if(mModeTimer<64){
			int sx,turn;
			if(!CheckEnemy(mActiveCharacter,ActiveCharacter()->GetTarget())){
				sx=1;
				turn=TURN_HORIZONTAL;
			}else{
				sx=-1;
				turn=0;
			}
			GetTopEffect().SetEffect(	BE_MOVE,
								320-sx*360,
								pInput->Rand(320)+40,
								BE_SPEED(sx*16,0),39+turn*0x10000,
								BLEND_STANDARD);
		}
	}
	if(mModeTimer==64){
		for(int i=0;i<no;i++){
			SetStatus(mActiveCharacter,target[i]&0xffff,11,100);
			GetTopEffect().SetEffect(
				BE_FLASH_CHARACTER,
				0,0,
				target[i] & 0xffff,0,BLEND_SOLID_ADD,StatusData[11].mColor);
			BreakTargetLock();
		}
	}else if(mModeTimer==80){
		return 1;
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////
int CAct_75::OnTimer()				//カッティングストーム
{
//定型文////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////
	const int INTARVAL	=4;
	const int HIT_NO	=20;

	if(mModeTimer==1){
		mActionParam[1]=new int[HIT_NO];
		memset(mActionParam[1],-1,sizeof(int)*8);
	}

	int t=ActiveCharacter()->GetTarget();
	int dir=-1;
	if(CheckEnemy(mActiveCharacter,t ))	dir=-dir;

	if(mModeTimer<HIT_NO*INTARVAL){
		for(int i=0;i<2;i++){
			int x=pInput->Rand(640)-dir*320;;
			int y=pInput->Rand(480);
			GetTopEffect().SetEffect(BE_TAIL_LINE,
				x,y,
				BE_SPEED(dir*48,0),16+2*0x10000,BLEND_ADD,0xff008000);
		}
	}

	for(int i=0;i<HIT_NO;i++){
		int time=mModeTimer-i*INTARVAL;

		if(time==1){
			mActionParam[1][i]=pInput->Rand(4);

			int sx=dir*BATTLE_SQUARE_SIZE/2;
			int y=mActionParam[1][i]*BATTLE_SQUARE_SIZE+64;
			PlayWave(SE_MISS);
			for(int j=0;j<4;j++){
				int x=320-sx*(10+j);
				D3DCOLOR color=D3DCOLOR_ARGB(255/(j+1),255,255,255);
				GetTopEffect().SetEffect(	BE_MOVE,x,y,
									BE_SPEED(sx,0),14,BLEND_ADD,color);
			}
		}
		if(time>=11 && time%2==1){
			int x=(time-11)/2*dir;

			int	field[BATTLE_SQUARE_WIDTH][BATTLE_SQUARE_Height];
			GetAttackArea(	mActiveCharacter,
							GetCharacter(t)->GetSX(),
							mActionParam[1][i],
							1,
							CR_HLINE,
							field,100);
			int no,*target;
			target=GetTarget(field,-1,&no);

			ChargeBreak();	//チャージ消去

			for(int j=0;j<no;j++){
				if(GetCharacter(target[j]&0xffff)->GetSX()==4+x){
					Attack(	mActiveCharacter,
							target[j]&0xffff,
							40,MIN_ATK_LV_3,
							AT_TYPE_MAGIC,
							AT_ELE_WIND,0,SE_WIND,
							target[j]/0x10000);
				}
			}
		}
	}

	if(mModeTimer>=INTARVAL*HIT_NO + 30){
		delete[] mActionParam[1];
		return 1;
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////
int CAct_76::OnTimer()				//スカイウォーカー
{
//定型文////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////
	int no,*target;
	target=GetTarget(&no);
	if(mModeTimer<64){ 
		for(int i=0;i<no;i++){
			int t=target[i]&0xffff;
			GetCharacter(t)->SetSpeedZ(1.2);
			GetCharacter(t)->ChangeAni((2+(mModeTimer%4/2))*ANI_TYPE,32,true);
			if(mModeTimer%2==0){
				int turn=1;
				if(!CheckEnemy(mActiveCharacter,t)) turn=-1;

				double sx=(300+pInput->Rand(300))*turn/100.0;
				double sy=(150+pInput->Rand(300))/100.0;

				GetTopEffect().SetEffect(
					BE_SPIN_PARTICUL,
					GetCharacter(t)->GetCX()+pInput->Rand(65)-32,
					GetCharacter(t)->GetGCY()+pInput->Rand(16)+16-mModeTimer/4,
					BE_SPEED(sx,sy),56+32*0x10000,
					BLEND_ADD,0xff80ff80);
				PlayWave(SE_STAR);
			}
		}
	}

	switch(mModeTimer){
		case 1:
			break;
		case 64:
			for(int i=0;i<no;i++){
				int t=target[i]&0xffff;
				GetCharacter(t)->SetSpeedZ(28);
			}
			break;
		case 80:
			for(int i=0;i<no;i++){
				int t=target[i]&0xffff;
				SetStatus(mActiveCharacter,t,13,100);
			}
			break;
		case 120:
			return 1;
			break;
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////
int CAct_77::OnTimer()				//トルネード
{
//定型文////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////
	const int WIND_TIME =96;
	const int SLASH_TIME=48;
	const int END_TIME=16+WIND_TIME;
	int dir=(CheckEnemy(mActiveCharacter,ActiveCharacter()->GetTarget())?1:-1);

	if(mModeTimer==1){
		PlayWave(SE_TORNADE);
		ChangeFieldColor(0x80000000,16);
	}
	if(mModeTimer<WIND_TIME){
		if(mModeTimer%(WIND_TIME/ACTION_EFFECT_NO)==0){
			//int no=mModeTimer/(WIND_TIME/ACTION_EFFECT_NO);
			int x=320+dir*(80+mModeTimer*4)+pInput->Rand(65)-32;
			int y=240+pInput->Rand(65)-32;
			const int size=480;
			for(int i=0;i<32;i++){
				GetTopEffect().SetEffect(
					BE_TORNADE_SPIN,x,y,
					pInput->Rand(320)+(size)*0x10000,30,
					BLEND_STANDARD,0xffffffff);
			}
			for(int i=0;i<8;i++){
				GetTopEffect().SetEffect(
					BE_TORNADE_LINE,x,y,
					pInput->Rand(320)+(size)*0x10000,32+(pInput->Rand(3)+3)*0x10000,
					BLEND_ADD,0xff108010);
			}
		}
	}
	if(mModeTimer>SLASH_TIME){
		int x=pInput->Rand(320)+160+dir*160;
		int y=pInput->Rand(320);

		GetTopEffect().SetEffect(BE_STANDARD,
			x,y,
			28,(dir>0?TURN_HORIZONTAL:0),BLEND_ADD);
		if(mModeTimer%8==0 && mModeTimer<=SLASH_TIME+8*4){
			int no,*target;
			target=GetTarget(&no);
			GetTopEffect().SetEffect(BE_FLASH,
				0,0,8,0,BLEND_STANDARD,0xd0ffffff);

			Attack(mActiveCharacter,target,no,
					90,	MIN_ATK_LV_4,
					AT_TYPE_MAGIC,
					AT_ELE_WIND,AT_OPT_ANTI_AIR | AT_OPT_ANTI_AIR_ELEMENT_,SE_WIND);
			if(mModeTimer>=SLASH_TIME+8*4)	BreakTargetLock();
		}
	}

	if(mModeTimer>=END_TIME){
		return 1;
	}
	return 0;
}