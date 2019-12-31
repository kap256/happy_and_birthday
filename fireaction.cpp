#include "fireaction.h"
#include "input.h"

////////////////////////////////////////////////////////////////////////////
int CAct_54::OnTimer()				//フレイムボール
{
//定型文////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////
	double sx,sy;
	int target=ActiveCharacter()->GetTarget();
	double length=mBattleMode->GetShotSpeed(mActiveCharacter,target,24,&sx,&sy);
	sy+=int(100/length);



	if(mModeTimer==1){
		int x=ActiveCharacter()->GetCX();
		int y=ActiveCharacter()->GetCY();
		ActiveCharacter()->ChangeAni(13*ANI_TYPE,16,true);
		mActionEffect[0]=GetTopEffect().SetEffect(
						BE_SMALL_SHOT,
						x,
						y,
						BE_SPEED(0,-8),9);
	}

	if(mModeTimer==12){
		mActionEffect[0]->OperationA(0,0);
	}
	if(mModeTimer==24){
		mActionEffect[0]->OperationD(sx,sy);
	}
	if(mModeTimer-24==int(length)){
		int i;
		for(i=0;i<16;i++){
			double x=(pInput->Rand(901)-450)/100.0;
			double y=(pInput->Rand(500)-600)/100.0;
			GetTopEffect().SetEffect(BE_FALL_PARTICUL,
						GetCharacter(target)->GetCX(),
						GetCharacter(target)->GetCY(),
						BE_SPEED(x,y),32*0x10000+10,
						BLEND_ADD);
		}

		int no,*target;
		target=GetTarget(&no);

		Attack(	mActiveCharacter,
				target,no,
				120,MIN_ATK_LV_1,
				AT_TYPE_MAGIC,
				AT_ELE_FIRE,0,SE_FIRE);
		BreakTargetLock();

		mActionEffect[0]->Destroy();
	}
	if(mModeTimer-55>=int(length)){
		return 1;
	}


	return 0;
}
////////////////////////////////////////////////////////////////////////////
int CAct_55::OnTimer()				//シャイニング
{
//定型文////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////
	switch(mModeTimer){
		case 1:
			ActiveCharacter()->ChangeAni(13*ANI_TYPE,40,true);
			break;
		case 10:

			int no,*target;
			target=GetTarget(&no);

			GetTopEffect().SetEffect(BE_FLASH,
							0,0,
							32,0,
							BLEND_ADD,0xffffb0b0);

			PlayWave(SE_FLASH);

			for(int i=0;i<no;i++){
				int t=target[i]&0xffff;
				if(SetCancel(t,40)){
					GetCharacter(t)->SetDamageEffect(false);
				}
			}
			BreakTargetLock();
			break;

		case 45:
			return 1;	break;
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////
int CAct_57::OnTimer()				//ファイアウォール
{
//定型文////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////
	int param=0;
	const int TIME=8;
	const int AFTER=48;
	switch(mModeTimer){
		case 1+TIME:
			param=1;
		case 1:
			int turn;
			if(CheckEnemy(mActiveCharacter))	turn=1;
			else	turn=-1;
			ActiveCharacter()->ChangeAni(13*ANI_TYPE,16,true);
			mActionEffect[param]=GetTopEffect().SetEffect(
							BE_SMALL_SHOT,
							ActiveCharacter()->GetCX(),
							ActiveCharacter()->GetCY(),
							BE_SPEED((-turn*24),(-24*(param*2-1))),9);
			PlayWave(SE_PARAM_UP);
		break;
	
		case AFTER:
			for(int i=0;i<2;i++){
				int target=ActiveCharacter()->GetTarget();
				int turn=1;
				if(!CheckEnemy(mActiveCharacter))	turn*=-1;
				int dir=(i*2-1);
				mActionEffect[i]=GetTopEffect().SetEffect(
								BE_SMALL_SHOT,
								GetCharacter(target)->GetCX()-turn*36,
								-dir*300+240,
								BE_SPEED((turn*3),(24*dir)),9);
			}
			break;
		case AFTER+12:
			int no,*target;
			target=GetTarget(&no);

			Attack(	mActiveCharacter,
					target,no,
					180,MIN_ATK_LV_2+50,
					AT_TYPE_MAGIC,
					AT_ELE_FIRE,0,SE_FIRE);
			BreakTargetLock();
			break;
		case 100:
			return 1;
	}
	if(mModeTimer>AFTER && mModeTimer<AFTER+24 && mModeTimer%4==0){
		PlayWave(SE_FIRE);
	}
	for(int i=0;i<2;i++){
		if(mActionEffect[i]){
			if(mModeTimer>AFTER && mModeTimer<AFTER+24){
				int bx,by;
				bx=int(mActionEffect[i]->GetX());
				by=int(mActionEffect[i]->GetY());
				for(int i=0;i<8;i++){
				double x=(pInput->Rand(501)-250)/100.0;
				double y=(pInput->Rand(1600)-800)/100.0;
				GetTopEffect().SetEffect(BE_PARTICUL,
							bx,by,
							BE_SPEED(x,y),32*0x10000+10,
							BLEND_ADD);
				}
			}else{
				int time=mModeTimer-TIME*i;
				if(time<24){
					int turn=0;
					if(time<10){
						if(CheckEnemy(mActiveCharacter))	turn=1;
						else	turn=-1;
					}
					int dir=i*2-1;
					double sx,sy;

					mActionEffect[i]->OperationDP(&sx,&sy);
					mActionEffect[i]->OperationD(sx+turn*3,sy+dir*2);

					int y=int(mActionEffect[i]->GetY());
					if(y<-160 || 640<y) mActionEffect[i]=NULL;
				}
			}
		}
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////
int CAct_58::OnTimer()				//バーンブースター
{
//定型文////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////
	int no,*target;
	target=GetTarget(&no);

	if(mModeTimer<60){
		if(mModeTimer%2==1){
			for(int i=0;i<no;i++){
				int angle=pInput->Rand(360);
				GetTopEffect().SetEffect(
								BE_CONCENTRATE,
								GetCharacter(target[i]&0xffff)->GetCX(),
								GetCharacter(target[i]&0xffff)->GetCY(),
								angle+128*0x10000,10+32*0x10000,
								BLEND_ADD,0xffff8000);
			}
		}
		if(mModeTimer%16==1){
			PlayWave(SE_PARAM_UP);
		}
	}else if(mModeTimer==100){
		int turn=(CheckEnemy(mActiveCharacter,ActiveCharacter()->GetTarget())?1:-1);
		for(int i=0;i<no;i++){
			for(int j=0;j<16;j++){
				double x=(pInput->Rand(1000)*turn)/100.0;
				double y=(pInput->Rand(300)-500)/100.0;
				GetTopEffect().SetEffect(BE_FALL_PARTICUL,
							GetCharacter(target[i]&0xffff)->GetCX()-32*turn,
							GetCharacter(target[i]&0xffff)->GetGCY(),
							BE_SPEED(x,y),32*0x10000+10,
							BLEND_ADD);
			}
			
			GetTopEffect().SetEffect(
				BE_FLASH_CHARACTER,
				0,0,target[i] & 0xffff,0,BLEND_SOLID,0xffff0000);
			SetStatus(mActiveCharacter,target[i]&0xffff,10,100);
		}
		BreakTargetLock();
	}else if(mModeTimer==120){
		return 1;
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////
int CAct_59::OnTimer()				//エクスプロージョン
{
//定型文////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////
	const int TIME=16;
	const int SIZE=1280;
	if(mModeTimer<60){
		int angle=pInput->Rand(360);
		int t=(120-mModeTimer)/2;
		for(int i=0;i<2;i++){
			GetTopEffect().SetEffect(
							BE_CONCENTRATE,
							GetCharacter(ActiveCharacter()->GetTarget())->GetCX(),
							GetCharacter(ActiveCharacter()->GetTarget())->GetCY(),
							angle+(t*8)*0x10000,10+t*0x10000,
							BLEND_ADD,0xffffffff);
		}
		if(mModeTimer%16==1){
			PlayWave(SE_PARAM_UP);
		}
	}else if(mModeTimer==100){
		int t=ActiveCharacter()->GetTarget();

		for(int i=0;i<8;i++){
			GetTopEffect().SetEffect(
					BE_LARGE_SPREAD,
					GetCharacter(ActiveCharacter()->GetTarget())->GetCX(),
					GetCharacter(ActiveCharacter()->GetTarget())->GetGCY()+32,
					50,(SIZE*i/8) + TIME * 0x10000);
		}
		int no,*target;
		target=GetTarget(&no);

		Attack(	mActiveCharacter,
				target,no,
				300,MIN_ATK_LV_3,
				AT_TYPE_MAGIC,
				AT_ELE_FIRE,0,SE_EXPLOSION);
		BreakTargetLock();
	}

	if(mModeTimer>=150){
		return 1;
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////
int CAct_60::OnTimer()				//フェニックス
{
//定型文////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////
	const int	START_TIME	= 10;
	const int	ANI_TIME	= 16;
	const int	END_TIME	= START_TIME+ANI_TIME*4;;

	if(mModeTimer==START_TIME){
		PlayWave(SE_MAGIC);
		int no,*target;
		target=GetTarget(&no);
		for(int i=0;i<4;i++){
			const int ani[4]={0,1,4,5};
			mActionEffect[i]=GetTopEffect().SetEffect(
					BE_ANIMATION,
					GetCharacter(ActiveCharacter()->GetTarget())->GetCX(),
					GetCharacter(ActiveCharacter()->GetTarget())->GetGCY()-16,
					57,0,BLEND_ADD,0x00ffffff);
			mActionEffect[i]->OperationA(ani[i],0);
		}
	}else if(mModeTimer==END_TIME){
		int no,*target;
		target=GetTarget(&no);
		for(int i=0;i<no;i++){
			SetStatus(mActiveCharacter,target[i]&0xffff,22,100);
		}
		BreakTargetLock();
	}else if(mModeTimer==END_TIME+ANI_TIME+10){
		if(mActionEffect[0]){
			for(int i=0;i<4;i++){
				mActionEffect[i]->Destroy();
				mActionEffect[i]=NULL;
			}
		}
		return 1;
	}
	
	if(mActionEffect[0]){
		for(int i=0;i<4;i++){
			if(mModeTimer%2==0){
				mActionEffect[i]->SetPos(mActionEffect[i]->GetX(),mActionEffect[i]->GetY()-1);
			}
			int t=mModeTimer-ANI_TIME*i;
			int alpha;
			if(t<0){
				alpha=0;
			}else if(t<ANI_TIME){
				alpha=t*0xff/ANI_TIME;
			}else if(t<ANI_TIME*2){
				alpha=(ANI_TIME*2-t)*0xff/ANI_TIME;
			}else{
				alpha=0;
			}
			mActionEffect[i]->SetColor(D3DCOLOR_ARGB(alpha,255,255,255));
		}
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////
int CAct_61::OnTimer()				//セイントスター
{
//定型文////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////
	const int ATTACK_TIME	= 120;
	const int END_TIME		= ATTACK_TIME+30;
	if(mModeTimer==1){
		for(int i=0;i<5;i++){
			mActionEffect[i]=GetBottomEffect().SetEffect(
				BE_LINE,0,0,0,0,BLEND_ADD);
			mActionEffect[i]->OperationA(400,0);
		}
		
		ChangeFieldColor(0x80000000,32);
	}
	if(mModeTimer==ATTACK_TIME){
		if(mActionEffect[0]){
			for(int i=0;i<5;i++){
				mActionEffect[i]->Destroy();
				mActionEffect[i]=NULL;
			}
		}
		GetTopEffect().SetEffect(BE_FLASH,
			0,0,8,0,BLEND_ADD,0xffffb080);
		ChangeFieldColor(0x00000000,1);

		int no,*target;
		target=GetTarget(&no);

		Attack(	mActiveCharacter,
				target,no,
				620,MIN_ATK_LV_4,
				AT_TYPE_MAGIC,
				AT_ELE_FIRE,AT_OPT_ANTI_UNDEAD,SE_EXPLOSION);
		BreakTargetLock();
		
	}
	if(mModeTimer>=ATTACK_TIME&& mModeTimer<=ATTACK_TIME+20){
		if( mModeTimer%4==0){
			PlayWave(SE_FIRE);
		}
		int t=ActiveCharacter()->GetTarget();
		int ex,ey;
		ex=GetCharacter(t)->GetCX();
		ey=GetCharacter(t)->GetGCY()+64;
		for(int i=0;i<48;i++){
			double rad=pInput->Rand(314)*2/100.0;
			double length=pInput->Rand(100)/100.0;
			int x=int(ex+cos(rad)*length*100);
			int y=int(ey+sin(rad)*length*66);
			double sx=(pInput->Rand(1001)-500)/100.0;
			double sy=(pInput->Rand(1000)-2600)/100.0;
			GetTopEffect().SetEffect(BE_FALL_PARTICUL,
						x,y,
						BE_SPEED(sx,sy),64*0x10000+10,
						BLEND_ADD);
		}
	}
	if(mModeTimer==END_TIME){
		return 1;
	}
	
	if(mActionEffect[0]){
		const double RAD=3.141592*2/5;
		int target=ActiveCharacter()->GetTarget();
		int ex,ey;
		ex=GetCharacter(target)->GetCX();
		ey=GetCharacter(target)->GetGCY()+64;
		int t=80-mModeTimer;
		if(t<0)	t=0;
		int	width = t*t/6+160;
		int	height =width*2/3;
		double  baserad = mModeTimer*mModeTimer*0.0010;
		int alpha=mModeTimer*16;
		if(alpha>255)	alpha=255;
		D3DCOLOR color=0x00ff4000+alpha*0x01000000;
		for(int i=0;i<5;i++){
			double x=ex+cos(i*RAD+baserad)*width;
			double y=ey+sin(i*RAD+baserad)*height;
			mActionEffect[i]->SetPos(x,y);
			mActionEffect[i]->OperationD(ex+cos((i+2)*RAD+baserad)*width , ey+sin((i+2)*RAD+baserad)*height );
			mActionEffect[i]->SetColor(color);
			
			for(int i=0;i<4;i++){
				double sx=(pInput->Rand(201)-100)/100.0;
				double sy=(pInput->Rand(400)-1200)/100.0;
				GetTopEffect().SetEffect(BE_FALL_PARTICUL,
							int(x),int(y),
							BE_SPEED(sx,sy),16*0x10000+10,
							BLEND_ADD);
			}
		}

		if(mModeTimer%4==0){
			PlayWave(SE_EXPLOSION);
		}
	}
	return 0;
}