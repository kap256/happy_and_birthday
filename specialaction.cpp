#include "specialaction.h"
#include "input.h"
////////////////////////////////////////////////////////////////////////////
int CAct_92::OnTimer()				//‰ž‹}ˆ’u
{
//’èŒ^•¶////////////////////////////////////////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////////////////////////////////////////
	if(mModeTimer==1){
		ActiveCharacter()->ChangeAni(6*ANI_TYPE,32,true);
	}else if(mModeTimer==20){
		int no,*target;
		target=GetTarget(&no);

		GetTopEffect().SetEffect(BE_STANDARD,
			GetCharacter(ActiveCharacter()->GetTarget())->GetCX(),
			GetCharacter(ActiveCharacter()->GetTarget())->GetCY(),
			6,0,BLEND_ADD,0xff00ff00);

		for(int i=0;i<no;i++){
			HealHP(	mActiveCharacter,
					target[i]&0xffff,
					int(GetCharacter(target[i]&0xffff)->GetMaxHP()*0.8));
		}
		BreakTargetLock();
	}else if(mModeTimer>=50){
		return 1;
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////
int CAct_93::OnTimer()				//j•Ÿ
{
//’èŒ^•¶////////////////////////////////////////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////////////////////////////////////////
	const int EFFECT_TIME = 96;

	int target=ActiveCharacter()->GetTarget();
	if(mModeTimer==1){
		ActiveCharacter()->ChangeAni(13*ANI_TYPE,EFFECT_TIME+16,true);
		mActionEffect[0]=GetTopEffect().SetEffect(
							BE_SMALL_ANIMATION,
							GetCharacter(target)->GetCX(),
							GetCharacter(target)->GetCY(),
							41,0,BLEND_ADD,0xff00ff00);
	}else if(mModeTimer==EFFECT_TIME){

		for(int i=0;i<16;i++){
			GetTopEffect().SetEffect(BE_FALL_PARTICUL,
					int(mActionEffect[0]->GetX()),
					int(mActionEffect[0]->GetY()-16),
					251+pInput->Rand(11)+(251+pInput->Rand(6))*0x10000,
					48*0x10000+41,
					BLEND_ADD,0xff00ff00);
		}
		mActionEffect[0]->Destroy();
		mActionEffect[0]=NULL;
		int no,*target;
		target=GetTarget(&no);

		for(int i=0;i<no;i++){
			HealHP(	mActiveCharacter,
					target[i]&0xffff,
					int(GetCharacter(target[i]&0xffff)->GetMaxHP()*0.5));
			
			GetTopEffect().SetEffect(
				BE_FLASH_CHARACTER,
				0,0,
				target[i] & 0xffff,0,BLEND_SOLID_ADD,0xff00ff00);
		}
		BreakTargetLock();
	}else if(mModeTimer>=EFFECT_TIME+32){
		return 1;
	}

	if(mActionEffect[0]){
		const int EFFECT_X =240;
		const int EFFECT_Y =160;
		const double EFFECT_Z =0.5;
		double rad=mModeTimer*0.1;
		double size=(double)(EFFECT_TIME-mModeTimer)/EFFECT_TIME;
		mActionEffect[0]->SetPos(
			GetCharacter(target)->GetCX()+int(cos(rad)*EFFECT_X*size),
			GetCharacter(target)->GetCY()+int(sin(rad)*EFFECT_Y*size - EFFECT_Z*mModeTimer)
			);
		mActionEffect[0]->OperationA(mModeTimer/2,0);

		double sx=(pInput->Rand(300)-150)/100.0;

		GetTopEffect().SetEffect(BE_PARTICUL,
					int(mActionEffect[0]->GetX()),
					int(mActionEffect[0]->GetY()-16),
					BE_SPEED(sx,4),16*0x10000+26,
					BLEND_ADD,0xff00ff00);
		if(mModeTimer%6==0){
			PlayWave(SE_STAR);
		}
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////
int CAct_103::OnTimer()				//Ã–°p
{
//’èŒ^•¶////////////////////////////////////////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////////////////////////////////////////
	const int TIME = 96;
	if(mModeTimer>5){
		if(mActionEffect[0]){
			if(mModeTimer>5+TIME){
				mActionEffect[0]->Destroy();
				mActionEffect[0]=NULL;
			}else{
				int t=mModeTimer-5;
				int s=int(sin(t*3.141592/TIME)*0x140);
				if(s<0)	s=0;
				if(s>0xff)	s=0xff;
				D3DCOLOR color=0xffffff+s*0x01000000;
				mActionEffect[0]->SetColor(color);

				int angle=int(sin(t*3.141592/32)*40);
				mActionEffect[0]->OperationA(angle,0);
				if(t%32==1){
					PlayWave(SE_CLOCK);
				}
			}
		}
	}
	if(mModeTimer==5){
		int no,*target;
		target=GetTarget(&no);
		ActiveCharacter()->ChangeAni(13*ANI_TYPE,64,true);
		mActionEffect[0]=GetTopEffect().SetEffect(
				BE_ROTATE,
				GetCharacter(ActiveCharacter()->GetTarget())->GetCX(),
				GetCharacter(ActiveCharacter()->GetTarget())->GetCY()-100,
				23,64+0*0x10000);

		mActionEffect[0]->SetColor(0x00000000);
	}else	if(mModeTimer==70){
		int no,*target;
		target=GetTarget(&no);

		for(int i=0;i<no;i++){
			SetStatus(mActiveCharacter,target[i]&0xffff,1,100);
		}
		BreakTargetLock();
	}else if(mModeTimer==110){
		return 1;
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////
int CAct_104::OnTimer()				//—U˜f
{
//’èŒ^•¶////////////////////////////////////////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////////////////////////////////////////
	if(mModeTimer==1){
		int t=ActiveCharacter()->GetTarget();
		mActionEffect[0]=GetTopEffect().SetEffect(
				BE_STRETCH,
				GetCharacter(t)->GetCX(),
				GetCharacter(t)->GetCY()+40,
				34,128+128*0x10000,BLEND_STANDARD);
	}else if(mModeTimer==60){
		int no,*target;
		target=GetTarget(&no);

		for(int i=0;i<no;i++){
			int t=target[i]&0xffff;
			SetStatus(mActiveCharacter,t,2,100);
		}
		BreakTargetLock();
	}else if(mModeTimer==80){
		mActionEffect[0]->Destroy();
		return 1;
	}

	if(mModeTimer%4==0){
		PlayWave(SE_STAR);
	}
	if(mModeTimer%20==0){
		ActiveCharacter()->ChangeAni(13*ANI_TYPE,12,true);
	}

	if(mActionEffect[0]){
		double x=mActionEffect[0]->GetX();
		double y=mActionEffect[0]->GetY()-1.5;
		mActionEffect[0]->SetPos(x,y);

		D3DCOLOR color;
		if(mModeTimer<40){
			color=mModeTimer*6*0x01;
		}else if(mModeTimer<60){
			color=240;
		}else{
			color=(80-mModeTimer)*12*0x01;
		}

		double sx=(pInput->Rand(1200)-600)/100.0;
		double sy=(pInput->Rand(200))/100.0;
		GetTopEffect().SetEffect(BE_FALL_PARTICUL,
						int(x+sx*6),
						int(y-50+pInput->Rand(65)-32),
						BE_SPEED(sx,sy),26+32*0x10000,
						BLEND_ADD,0xff000000+color*0x010000);

		mActionEffect[0]->SetColor(color*0x01000000+0xffffff);

		double rad=mModeTimer*0.6;
		int size=int(cos(rad)*128);
		mActionEffect[0]->OperationA(size,128);
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////
int CAct_105::OnTimer()				//“d‹CƒVƒ‡ƒbƒN
{
//’èŒ^•¶////////////////////////////////////////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////////////////////////////////////////
	if(mModeTimer>=24 && mModeTimer<48){
		PlayWave(SE_ELECTRIC);
	}
	if(mModeTimer==1){
		ActiveCharacter()->ChangeAni(6*ANI_TYPE,24,true);
	}else if(mModeTimer==24){
		ActiveCharacter()->ChangeAni(13*ANI_TYPE,16,true);
		int no,*target;
		target=GetTarget(&no);
		for(int i=0;i<no;i++){
			GetCharacter(target[i]&0xffff)->SetDamageEffect(true);
		}
	}else if(mModeTimer==32){
		int no,*target;
		target=GetTarget(&no);

		for(int i=0;i<no;i++){
			int t=target[i]&0xffff;
			if(SetStatus(mActiveCharacter,t,3,100)>0){
				GetCharacter(t)->SetDamageEffect(true);
			}
		}
		BreakTargetLock();
	}else if(mModeTimer==55){
		return 1;
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////
int CAct_106::OnTimer()				//••ˆó
{
//’èŒ^•¶////////////////////////////////////////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////////////////////////////////////////
	const int	START_TIME	= 1;
	const int	STOP_TIME	= 16;
	const int	DELETE_TIME	= STOP_TIME+20;
	const int	END_TIME	= DELETE_TIME+16;

	const int	MAX_SIZE	= 240;
	const int	MIN_SIZE	= 128;

	if(mModeTimer==START_TIME){
		ActiveCharacter()->ChangeAni(6*ANI_TYPE,DELETE_TIME+8,true);
		PlayWave(SE_MAGIC);
		int no,*target;
		target=GetTarget(&no);
		mActionEffect[0]=GetTopEffect().SetEffect(
				BE_STRETCH,
				GetCharacter(ActiveCharacter()->GetTarget())->GetCX(),
				GetCharacter(ActiveCharacter()->GetTarget())->GetCY(),
				77,MAX_SIZE+MAX_SIZE*0x10000,BLEND_STANDARD,0x00ffffff);
	}
	if(mModeTimer<=STOP_TIME){
		if(mActionEffect[0]){
			int size=(MAX_SIZE-MIN_SIZE)*(STOP_TIME-mModeTimer)+MIN_SIZE;
			mActionEffect[0]->OperationA(size,size);
			D3DCOLOR color=(mModeTimer*0xff/STOP_TIME*0x01000000) | 0x00ffffff;
			mActionEffect[0]->SetColor(color);
		}
	}else if(mModeTimer==DELETE_TIME){
		int no,*target;
		target=GetTarget(&no);

		for(int i=0;i<no;i++){
			int t=target[i]&0xffff;
			if(SetStatus(mActiveCharacter,t,4,100)>0){
				GetTopEffect().SetEffect(
					BE_FLASH_CHARACTER,
					0,0,
					target[i] & 0xffff,32,BLEND_SOLID,0xff000000);
				GetCharacter(target[i] & 0xffff)->SetDamageEffect(false);
			}
		}
		BreakTargetLock();
	}else if(DELETE_TIME<mModeTimer && mModeTimer<END_TIME){
		D3DCOLOR color=((END_TIME-mModeTimer)*0xff/(END_TIME-DELETE_TIME)*0x1000000) | 0x00ffffff;
		mActionEffect[0]->SetColor(color);
	}else if(mModeTimer==END_TIME){
		if(mActionEffect[0]){
			mActionEffect[0]->Destroy();
			mActionEffect[0]=NULL;
		}
	}else if(mModeTimer>=END_TIME+10){
		return 1;
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////
int CAct_107::OnTimer()				//‹¶íŽm‚Ìƒcƒ{
{
//’èŒ^•¶////////////////////////////////////////////
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
				GetCharacter(ActiveCharacter()->GetTarget())->GetCX(),
				GetCharacter(ActiveCharacter()->GetTarget())->GetCY(),
				65,turn,BLEND_ADD);

			PlayWave(SE_SLASH);
			for(int i=0;i<no;i++){
				int t=target[i]&0xffff;
				if(SetStatus(mActiveCharacter,t,5,100)>0){
				}
			}
			BreakTargetLock();
			break;
		case 7:
			ActiveCharacter()->ChangeAni(10*ANI_TYPE,22,true);
			break;

		case 35:
			return 1;	break;
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////
int CAct_111::OnTimer()				//–ü‚µ‚ÌŒõ
{
//’èŒ^•¶////////////////////////////////////////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////////////////////////////////////////
	const int TIME = 30;
	switch(mModeTimer){
		case TIME+10:
			int no,*target;
			target=GetTarget(&no);
			for(int i=0;i<no;i++){
				HealHP(	mActiveCharacter,
						target[i]&0xffff,
						GetCharacter(target[i]&0xffff)->GetMaxHP(),
						false);
				CureStatus(mActiveCharacter,target[i]&0xffff);
				GetTopEffect().SetEffect(
					BE_FLASH_CHARACTER,
					0,0,
					target[i] & 0xffff,0,BLEND_SOLID_ADD,0xff0000ff);
				BreakTargetLock();
			}
			for(int i=0;i<160;i++){
				GetTopEffect().SetEffect(BE_PARTICUL,
					pInput->Rand(640),
					pInput->Rand(640)-160,
					BE_SPEED(0,pInput->Rand(100)*0.01+0.5),26+64*0x10000,BLEND_ADD,0xff0080ff);
			}
			GetTopEffect().SetEffect(
				BE_FLASH,
				0,0,
				8,8,BLEND_STANDARD,0xff00ffff);
			break;
		case TIME+20:
			return 1;
	}
	if(mModeTimer<TIME){
		int turn=1;
		if(CheckEnemy(mActiveCharacter))	turn=-1;
		for(int i=0;i<4;i++){
			GetTopEffect().SetEffect(BE_STANDARD,
				320+(pInput->Rand(160)+mModeTimer*16-160)*turn,
				pInput->Rand(480),
				6,0,BLEND_ADD,0xff1020ff);
		}
		
		if(mModeTimer%4==1)	PlayWave(SE_STAR);
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////
int CAct_112::OnTimer()				//–½‚Ì‹P‚«
{
//’èŒ^•¶////////////////////////////////////////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////////////////////////////////////////
	const int SPOT_TIME=160;
	const int SPOT_START_TIME=32;
	if(mModeTimer==1){
		PlayWave(SE_RIVIVAL);
		ChangeFieldColor(0x80000000,16);
		int target=ActiveCharacter()->GetTarget();

		mActionEffect[0]=GetBottomEffect().SetEffect(BE_SPOT_LIGHT,
			GetCharacter(target)->GetCX(),
			GetCharacter(target)->GetCY(),
			78+0x10000,SPOT_TIME,BLEND_ADD,0x00ffffff);
		mActionEffect[1]=GetTopEffect().SetEffect(BE_SPOT_LIGHT,
			GetCharacter(target)->GetCX(),
			GetCharacter(target)->GetCY(),
			78,SPOT_TIME,BLEND_ADD,0x00ffffff);

		double sy=double(GetCharacter(target)->GetCY()+64+SPOT_TIME/2)/SPOT_TIME;
		mActionEffect[2]=GetTopEffect().SetEffect(BE_MOVE,
			GetCharacter(target)->GetCX(),
			-64,
			BE_SPEED(0,sy),20,BLEND_ADD,0xffffffff);
		mActionEffect[3]=GetTopEffect().SetEffect(BE_SMALL_SHOT,
			GetCharacter(target)->GetCX(),
			-64,
			BE_SPEED(0,sy),41,BLEND_STANDARD,0x8080ff80);
	}else if(mModeTimer<SPOT_TIME){
		int target=ActiveCharacter()->GetTarget();
		double sy=double(GetCharacter(target)->GetCY()+64-mModeTimer+SPOT_TIME/2)/SPOT_TIME;
		mActionEffect[2]->OperationD(0,sy);
		mActionEffect[3]->OperationD(0,sy);
		if(mModeTimer%48==1){
			PlayWave(SE_RIVIVAL);
		}
	}else if(mModeTimer>=SPOT_TIME && mModeTimer<SPOT_TIME+48){
		ChangeFieldColor(0x00000000,1);
		if(mModeTimer%16==SPOT_TIME%16){
			int no,*target;
			target=GetTarget(&no);

			int type=(mModeTimer-SPOT_TIME)/16;
			for(int i=0;i<no;i++){
				switch(type){
					case 0:
						HealLP(	mActiveCharacter,
								target[i]&0xffff,
								GetCharacter(target[i]&0xffff)->GetMaxLP());
						break;
					case 1:
						HealHP(	mActiveCharacter,
								target[i]&0xffff,
								GetCharacter(target[i]&0xffff)->GetMaxLP(),
								true);
						break;
					case 2:
						BreakTargetLock();
						if(!CureStatus(mActiveCharacter,target[i]&0xffff)){
								return 0;
						}
						break;
				}
			}
			
			GetTopEffect().SetEffect(
				BE_FLASH,
				0,0,
				16,16,BLEND_STANDARD,0xffffffff);
		}
	}else if(mModeTimer>=SPOT_TIME+64){
		return 1;
	}

	if(	mActionEffect[0]){
		if(mModeTimer>=SPOT_TIME){
			for(int i=0;i<4;i++){
				mActionEffect[i]->Destroy();
				mActionEffect[i]=NULL;
			}
		}else{
			D3DCOLOR spotcolor=0x80ffffff;
			if(mModeTimer<SPOT_START_TIME){
				spotcolor=((mModeTimer*0x80/SPOT_START_TIME)<<24)+0x00ffffff;
			}
			mActionEffect[0]->SetColor(spotcolor);
			mActionEffect[1]->SetColor(spotcolor);
		}
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////
int CAct_114::OnTimer()				//–hŒä
{
//’èŒ^•¶////////////////////////////////////////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////////////////////////////////////////
	if(mModeTimer==10){
		ActiveCharacter()->ChangeAni(13*ANI_TYPE,20,true);
		int turn;
		if(CheckEnemy(mActiveCharacter))	turn=TURN_HORIZONTAL;
		else	turn=0;
		GetTopEffect().SetEffect(
			BE_FLASH_CHARACTER,
			0,0,
			mActiveCharacter,0,BLEND_SOLID_ADD,0xffff8000);

		CBattleEffect* effect=GetTopEffect().SetEffect(
			BE_STANDARD,
			ActiveCharacter()->GetCX(),
			ActiveCharacter()->GetCY(),
			18,turn,BLEND_ADD,0xffff8000);

		SetStatus(mActiveCharacter,mActiveCharacter,15,100);
		BreakTargetLock();
	}else if(mModeTimer==30){
		return 1;
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////
int CAct_115::OnTimer()				//’§”­
{
//’èŒ^•¶////////////////////////////////////////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////////////////////////////////////////
	switch(mModeTimer){
	case 1:
	case 41:
		PlayWave(SE_PROVOKE_1);
		ActiveCharacter()->ChangeAni((pInput->Rand(5)+9)*ANI_TYPE,32,true);
		break;
	case 60:
		int no,*target;
		target=GetTarget(&no);

		for(int i=0;i<no;i++){
			int t=target[i]&0xffff;
			int act=GetCharacter(t)->GetAction();
			bool b=ComSkillData[act].mTarget;
			if(GetCharacter(t)->Enemy()==ActiveCharacter()->Enemy())	b=!b;
			if(b){
				if(SetStatus(mActiveCharacter,t,21,100,mActiveCharacter+1)>0){
					GetTopEffect().SetEffect(BE_STANDARD,GetCharacter(t)->GetCX(),GetCharacter(t)->GetCY(),42,0,BLEND_STANDARD);
				}
			}
		}
		BreakTargetLock();
	case 20:
		PlayWave(SE_PROVOKE_2);
		ActiveCharacter()->ChangeAni(0,32,true);
		break;
	case 80:
		return 1;
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////
int CAct_116::OnTimer()				//‚½‚ß‚é
{
//’èŒ^•¶////////////////////////////////////////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////////////////////////////////////////
	if(mModeTimer==10){
		ActiveCharacter()->ChangeAni(6*ANI_TYPE,20,true);
		GetTopEffect().SetEffect(
			BE_FLASH_CHARACTER,
			0,0,
			mActiveCharacter,0,BLEND_SOLID_ADD,0xffff8000);
	}else if(mModeTimer==30){
		int turn;
		if(CheckEnemy(mActiveCharacter))	turn=TURN_HORIZONTAL;
		else	turn=0;

		CBattleEffect* effect=GetTopEffect().SetEffect(
			BE_STANDARD,
			ActiveCharacter()->GetCX(),
			ActiveCharacter()->GetCY(),
			18,turn,BLEND_ADD,0xffff8000);

		SetStatus(mActiveCharacter,mActiveCharacter,8,100);
		BreakTargetLock();
	}else if(mModeTimer==60){
		return 1;
	}
	return 0;
}