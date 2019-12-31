#include "enemyaction.h"
#include "input.h"
#include "changemode.h"

////////////////////////////////////////////////////////////////////////////
int CAct_146::OnTimer()				//é©îö
{
//íËå^ï∂////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////
	if(mModeTimer<42){
		ActiveCharacter()->ChangeAni(10*ANI_TYPE+1,16,true);
		if(mModeTimer%6==0){
			ActiveCharacter()->SetSpeedZ(BATTLE_GRAVITY*3);
		}
	}
	if(mModeTimer==42){
		ActiveCharacter()->ChangeAni(11*ANI_TYPE+1,16,true);
	}
	if(mModeTimer==64){
		int no,*target;
		target=GetTarget(&no);

		Attack(	mActiveCharacter,
				target,no,
				500,0,
				AT_TYPE_MAGIC,
				AT_ELE_FIRE,0,SE_FIRE);
		ActiveCharacter()->DeleteCharacter();
		return 1;
	}
	if(48<mModeTimer && mModeTimer<64){
		ActiveCharacter()->ChangeAni(9*ANI_TYPE+1,16,true);
		for(int i=0;i<8;i++){
			double x=(pInput->Rand(1700)-850)/100.0;
			double y=(pInput->Rand(1200)-800)/100.0;
			GetTopEffect().SetEffect(BE_FALL_PARTICUL,
						ActiveCharacter()->GetCX(),
						ActiveCharacter()->GetCY(),
						BE_SPEED(x,y),32*0x10000+10,
						BLEND_ADD);
		}
		if(mModeTimer%5==0){
			PlayWave(SE_FIRE);
		}
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////
int CAct_147::OnTimer()			//ãzåå
{
//íËå^ï∂////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////
	switch(mModeTimer){
		case 1:
			ActiveCharacter()->ChangeAni(9*ANI_TYPE,8,true);
			break;
		case 4:
			ActiveCharacter()->ChangeAni(11*ANI_TYPE,12,true);
			break;
		case 7:
			ActiveCharacter()->ChangeAni(10*ANI_TYPE,40,true);
			break;
		case 30:
			int no,heal,*target,*hit,pow;
			heal=0;
			target=GetTarget(&no);

			pow=100;
			if (ActiveCharacter()->SearchASkill(419))	pow*=2;
			hit=Attack(	mActiveCharacter,
					target,no,
					pow,0,
					AT_TYPE_WEAPON,
					AT_ELE_NONE,0,SE_BLOOD);

			for(int i=0;i<no;i++){
				if(hit[i]<0)	continue;
				heal+=hit[i];
				for(int j=0;j<8;j++){
					int x=pInput->Rand(33)-16;
					int y=pInput->Rand(33)-16;
					double sx=(pInput->Rand(700)-350)/100.0;
					double sy=(pInput->Rand(700)-350)/100.0;
					D3DCOLOR c=0xff800000+pInput->Rand(63)*0x100000;
					GetTopEffect().SetEffect(BE_PARTICUL,
								GetCharacter(target[i]&0xffff)->GetCX()+x,
								GetCharacter(target[i]&0xffff)->GetCY()+y,
								BE_SPEED(sx,sy),24*0x10000+21,
								BLEND_STANDARD,c);
				}
			}
			
			if(heal>0){
				GetTopEffect().SetEffect(
						BE_FLASH_CHARACTER,
						0,0,
						mActiveCharacter,0,BLEND_SOLID_ADD,0xff00ff00);
				HealHP(	mActiveCharacter,
						mActiveCharacter,
						heal );
			}
			BreakTargetLock();
			break;

		case 55:
			return 1;	break;
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////
int CAct_150::OnTimer()				//ó{ï™ãzé˚
{
//íËå^ï∂////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////
	switch(mModeTimer){
		case 1:
			int t;
			t=ActiveCharacter()->GetTarget();
			ActiveCharacter()->ChangeAni(6*ANI_TYPE,100,true);
			mActionEffect[0]=GetTopEffect().SetEffect(
						BE_ANIMATION,
						GetCharacter(t)->GetCX(),
						GetCharacter(t)->GetGCY(),
						33,0);
			PlayWave(SE_MOVE_BLOCK);
			break;
		case 110-4:
			mActionEffect[0]->OperationA(0,0);	break;
		case 5:
		case 110-8:
			mActionEffect[0]->OperationA(1,0);	break;
		case 9:
			mActionEffect[0]->OperationA(4,0);	break;
		case 91:
			BreakTargetLock();	break;

		case 110:
			mActionEffect[0]->Destroy();
			return 1;
	}
	if(mModeTimer%30==0 && mModeTimer>=30){
		mActionEffect[0]->OperationA(5,0);
		int no,heal,*target,*hit;
		heal=0;
		target=GetTarget(&no);
		hit=Attack(	mActiveCharacter,
				target,no,
				50,0,
				AT_TYPE_WEAPON,
				AT_ELE_EARTH,AT_OPT_NO_AIR,SE_BLOOD);

		for(int i=0;i<no;i++){
			if(hit[i]<0)	continue;
			heal+=hit[i];
		}
		
		if(heal>0){
			GetTopEffect().SetEffect(
					BE_FLASH_CHARACTER,
					0,0,
					mActiveCharacter,0,BLEND_SOLID_ADD,0xff00ff00);
			HealHP(	mActiveCharacter,
					mActiveCharacter,
					heal );
		}

	}else if(mModeTimer%30==10){
		mActionEffect[0]->OperationA(4,0);
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////
int CAct_151::OnTimer()				//Ç‹Ç‚Ç©ÇµÇÃñÇèpét
{
//íËå^ï∂////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////
	CBattleMode &bm=*mBattleMode;
	switch(mModeTimer){
		case 30:

			{
				for(int i=0;i<MAX_ENEMY_NUMBER;i++){
					bm.mCharacter[i+ENEMY]=&bm.mEnemy[i];
				}

				GetTopEffect().SetEffect(BE_FLASH,
								0,0,
								32,0,
								BLEND_STANDARD,0xffffffff);

				PlayWave(SE_FLASH);
				static int  enemyfield[BATTLE_SQUARE_Height][BATTLE_SQUARE_Height];
				static int	enemyrank[MAX_ENEMY_NUMBER];
				memset(enemyfield,0,sizeof(enemyfield));
				memset(enemyrank,-1,sizeof(enemyrank));

				bm.mEnemy[0].Move(pInput->Rand(4),pInput->Rand(BATTLE_SQUARE_Height),1);
				int x=bm.mEnemy[0].GetSX();
				int y=bm.mEnemy[0].GetSY();
				bm.mEnemy[0].SetPos(x*BATTLE_SQUARE_SIZE,y*BATTLE_SQUARE_SIZE,0);
				enemyrank[0]=x*BATTLE_SQUARE_Height-y;
				enemyfield[x][y]=1;

				int i=1;
				for(int test=0;test<16;test++){
					if(i>=8)	break;
					bm.mEnemy[i].DeleteCharacter();
					int ret=bm.SetEnemy(i,25,ActiveCharacter()->GetLv(),enemyfield,0,4);
					if(ret>0){
						int x=bm.mEnemy[i].GetSX();
						int y=bm.mEnemy[i].GetSY();
						bm.mEnemy[i].SetPos(x*BATTLE_SQUARE_SIZE,y*BATTLE_SQUARE_SIZE,0);
						enemyrank[i]=x*BATTLE_SQUARE_Height-y;
						bm.mEnemy[i].SetSpdUp(bm.mEnemy[i].GetSpeed()*i);

						i+=ret;
					}
				}

				//return 1;

				
				bool turn=true;
				while(turn){
					turn=false;
					for(int n=1;n<8;n++){
						if(enemyrank[n] > enemyrank[n-1]){
							turn=true;
							CBattleCharacter* c					=bm.mCharacter[MAX_PARTY_NUMBER+n];
							bm.mCharacter[MAX_PARTY_NUMBER+n]	=bm.mCharacter[MAX_PARTY_NUMBER+n-1];
							bm.mCharacter[MAX_PARTY_NUMBER+n-1]	=c;
							int z			=enemyrank[n];
							enemyrank[n]	=enemyrank[n-1];
							enemyrank[n-1]	=z;
						}
					}
				}
				
			}

			return 1;
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////
int CAct_152::OnTimer()				//ÉTÉ\ÉäÇÃì≈êj
{
//íËå^ï∂////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////
	switch(mModeTimer){
		case 1:
			ActiveCharacter()->ChangeAni(9*ANI_TYPE+2,16,true);
			break;
		case 4:
			int no,*target,*hit;
			target=GetTarget(&no);

			ActiveCharacter()->ChangeAni(12*ANI_TYPE+1,16,true);

			int turn;
			if(CheckEnemy(mActiveCharacter))	turn=TURN_HORIZONTAL;
			else	turn=0;
			GetTopEffect().SetEffect(BE_STANDARD,
				GetCharacter(ActiveCharacter()->GetTarget())->GetCX(),GetCharacter(ActiveCharacter()->GetTarget())->GetCY(),
				2,turn,BLEND_ADD);

			hit=Attack(	mActiveCharacter,
					target,no,
					100,0,
					AT_TYPE_WEAPON,
					AT_ELE_NONE,0,SE_THRUST);
			for(int i=0;i<no;i++){
				if(hit[i]<0)	continue;
				SetStatus(mActiveCharacter,target[i]&0xffff,0,100);
				SetStatus(mActiveCharacter,target[i]&0xffff,3,100);
			}
			BreakTargetLock();
			break;
		case 7:
			ActiveCharacter()->ChangeAni(11*ANI_TYPE+2,20,true);
			break;

		case 35:
			return 1;	break;
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////
int CAct_153::OnTimer()				//Ç∑Ç›
{
//íËå^ï∂////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////
	double sx,sy;
	int target=ActiveCharacter()->GetTarget();
	double length=mBattleMode->GetShotSpeed(mActiveCharacter,target,20,&sx,&sy);

	switch(mModeTimer){
		case 1:
			PlayWave(SE_POO);
			mActionEffect[0]=GetTopEffect().SetEffect(BE_TAIL_MOVE,
							ActiveCharacter()->GetCX(),
							ActiveCharacter()->GetCY(),
							BE_SPEED(sx,sy),46,BLEND_STANDARD);
			ActiveCharacter()->ChangeAni(13*ANI_TYPE+3,30,true);
			break;

	}

	if(mModeTimer-1==int(length)){
		int no,*target;
		target=GetTarget(&no);

		for(int i=0;i<no;i++){
			int t=target[i]&0xffff;
			int st=SetStatus(mActiveCharacter,t,18,100);
			if(st){
				if(mActionEffect[0]){
					mActionEffect[0]->Destroy();
					for(i=0;i<16;i++){
						double x=(pInput->Rand(1100)-550)/100.0;
						double y=(pInput->Rand(700)-350)/100.0;
						GetTopEffect().SetEffect(BE_FALL_PARTICUL,
									GetCharacter(t)->GetCX(),
									GetCharacter(t)->GetCY(),
									BE_SPEED(x,y),16*0x10000+21,
									BLEND_STANDARD,0x80000000);
					}
				}
				GetCharacter(t)->SetDamageEffect(false);
			}else{
				GetCharacter(t)->SetAvoid();
				PlayWave(SE_MISS);
			}
		}
		BreakTargetLock();
	}
	if(mModeTimer-30>=int(length)){
		return 1;
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////
int CAct_154::OnTimer()				//ìÀåÇ
{
//íËå^ï∂////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////
int target=ActiveCharacter()->GetTarget();
	int sy=GetCharacter(target)->GetSY();
	if(mModeTimer==8){
		ActiveCharacter()->SetGoalSquare(4,sy,16,1,true);
	}
	if(mModeTimer==24){
		ActiveCharacter()->ChangeAni(10*ANI_TYPE,32,true);
	}
	if(mModeTimer==32){
			ActiveCharacter()->SetGoal(
				(CheckEnemy(mActiveCharacter,target)?600:-40),
				ActiveCharacter()->GetY(),
				16,false);
	}
	if(mModeTimer>=32 && mModeTimer%4==0){
		int x=(mModeTimer-32)/4;
		if(!CheckEnemy(mActiveCharacter,target))	x=x*-1;
		int no,*target;
		target=GetTarget(&no);

		ChargeBreak();	//É`ÉÉÅ[ÉWè¡ãé

		for(int i=0;i<no;i++){
			if(GetCharacter(target[i]&0xffff)->GetSX()==4+x){
				int movex;
				int turn;
				if(CheckEnemy(mActiveCharacter,target[i]&0xffff)){
					movex=BATTLE_SQUARE_WIDTH;
					turn=0;
				}else{
					turn=TURN_HORIZONTAL;
					movex=-1;
				}
				int t=target[i]&0xffff;
				Attack(	mActiveCharacter,
						t,180,0,
						AT_TYPE_WEAPON,
						AT_ELE_NONE,0,SE_HIT,target[i]/0x10000);
			}
		}
	}

	if(mModeTimer>=64){
		return 1;
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////
int CAct_156::OnTimer()				//É}ÉOÉ}ÉtÉBÅ[ÉãÉh
{
//íËå^ï∂////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////
	CBattleMode &bm=*mBattleMode;
	if(mModeTimer==1){
		int		fe[BATTLE_SQUARE_WIDTH][BATTLE_SQUARE_Height];
		bm.GetTargetArea(	mActiveCharacter,
						ActiveCharacter()->GetTarget(),
						mAction,
						fe);
		int i=0;
		for(int x=0;x<BATTLE_SQUARE_WIDTH;x++){
			for(int y=0;y<BATTLE_SQUARE_Height;y++){
				if(fe[x][y]){
					if(i<ACTION_EFFECT_NO){	
						mActionEffect[i]=GetBottomEffect().SetEffect(
										BE_ANIMATION,
										x*BATTLE_SQUARE_SIZE+32,
										y*BATTLE_SQUARE_SIZE+64,
										59,0,
										BLEND_STANDARD);
						i++;
					}
				}
			}
		}
		BreakTargetLock();
	}
	if(mModeTimer==60){
		int		fe[BATTLE_SQUARE_WIDTH][BATTLE_SQUARE_Height];
		bm.GetTargetArea(	mActiveCharacter,
						ActiveCharacter()->GetTarget(),
						mAction,
						fe);
		for(int x=0;x<BATTLE_SQUARE_WIDTH;x++){
			for(int y=0;y<BATTLE_SQUARE_Height;y++){
				if(fe[x][y]){
					int pow=int(fe[x][y]*ActiveCharacter()->GetMagAtk()/100);
					bm.SetFieldEffect(x,y,pow,FE_TYPE_MAGIC | FE_ELE_FIRE | FE_ANI_0 , pInput->Rand(3)+4);
				}
			}
		}
		int i=0;
		while(mActionEffect[i]){
			mActionEffect[i]->Destroy();
			mActionEffect[i]=NULL;
			i++;
		}

		return 1;
	}
	int i=0;
	if(mModeTimer%6==1){
		PlayWave(SE_FIRE);
	}
	while(mActionEffect[i]){
		mActionEffect[i]->SetPos(mActionEffect[i]->GetX()+(mModeTimer%2*4)-2,mActionEffect[i]->GetY());
		switch(mModeTimer){
			case 15:	mActionEffect[i]->OperationA(1,0);	break;
			case 30:	mActionEffect[i]->OperationA(4,0);	break;
			case 45:	mActionEffect[i]->OperationA(5,0);	break;
		}
		double sx=(pInput->Rand(3000)-1500)/100.0;
		double sy=(pInput->Rand(500)-600)/100.0;
		int x=int(mActionEffect[i]->GetX()+32 + sx*4);
		int y=int(mActionEffect[i]->GetY()    + sy*8);
		int ani=49;
		int blend=BLEND_STANDARD;
		if(pInput->Rand(3)==0){
			ani=10;
			blend=BLEND_ADD;
		}
		GetTopEffect().SetEffect(BE_FALL_PARTICUL,
				x,y,
				BE_SPEED((sx/6),sy),32*0x10000+ani,
				blend);

		i++;
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////
int CAct_157::OnTimer()				//ÉTÉìÉ_Å[ÉXÉgÅ[ÉÄ
{
//íËå^ï∂////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////
	int dir=(CheckEnemy(mActiveCharacter,ActiveCharacter()->GetTarget())?1:-1);

	switch(mModeTimer){
		case 1:
			ChangeFieldColor(0xb0000000,8);
			break;
			
		case 32:
			for(int i=0;i<2;i++){
				GetTopEffect().SetEffect(BE_THUNDER,
						dir*160+320,-40,
						10,0,BLEND_ADD,0xffffff00);
			}

			GetTopEffect().SetEffect(BE_FLASH,
				0,0,32,0,BLEND_STANDARD,0xffffff80);
			int no,*target,*hit;
			target=GetTarget(&no);

			hit=Attack(	mActiveCharacter,
					target,no,
					350,0,
					AT_TYPE_MAGIC,
					AT_ELE_FIRE | AT_ELE_WIND,0,SE_EXPLOSION);
			for(int i=0;i<no;i++){
				if(hit[i]<0)	continue;
				SetStatus(mActiveCharacter,target[i]&0xffff,3,100);
			}
			BreakTargetLock();
			break;
		case 70:
			return 1;	break;
	}
	if(mModeTimer<16){
		if(mModeTimer%4==1){
			PlayWave(SE_PARALYSE);
			GetTopEffect().SetEffect(BE_THUNDER,
					(mModeTimer*50-400)*dir+320,-100,
					8,0,BLEND_ADD,0xffffff00);
		}
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////
int CAct_158::OnTimer()				//ÉçÉgÉìÉXÉ|ÉA
{
//íËå^ï∂////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////
	int dir=(CheckEnemy(mActiveCharacter,ActiveCharacter()->GetTarget())?1:-1);
	const int END_TIME =80;
	switch(mModeTimer){
		case 1:
			ChangeFieldColor(0x80ff80ff,64);
			break;
			
		case END_TIME:
			int no,*target,*hit;
			target=GetTarget(&no);

			hit=Attack(	mActiveCharacter,
					target,no,
					150,0,
					AT_TYPE_MAGIC,
					AT_ELE_ICE | AT_ELE_EARTH,AT_OPT_LIFE_BREAK,SE_ROT);
			for(int i=0;i<no;i++){
				if(hit[i]<0)	continue;
				SetStatus(mActiveCharacter,target[i]&0xffff,0,100);
				SetStatus(mActiveCharacter,target[i]&0xffff,6,100);
			}
			BreakTargetLock();
			break;
		case END_TIME+32:
			return 1;
	}
	if(mModeTimer<END_TIME){
		if(pInput->Rand(4)<1){
			PlayWave(SE_ROT);
		}
		for(int i=0;i<6;i++){
			int x=pInput->Rand(600);
			int y=pInput->Rand(500);
			int size=pInput->Rand(80)+10;
			int life=64;

			double sx=size*0.02*dir;
			double sy=0;
			CBattleEffect *effect=GetTopEffect().SetEffect(
					BE_STRETCH_FADE_MOVE,
					x,y,
					BE_SPEED(sx,sy),64 + life*0x10000,
					BLEND_STANDARD,0x40ff00ff);
			effect->OperationA(size,size);
		}
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////
void CAct_168::DrawCircle(int t,int x,int y,CBattleEffect* effect)
{
	double nx=x;
	double ny=y;
	if(t>=112 && t<112+32)	t-=112;
	if(t<=32){
		double rad=t*3.141592*2/32 - 3.141592/2;
		nx=x+cos(rad)*size;
		ny=y+(sin(rad)*size)/2;
	}else if(t<112){
		const double STAR_RAD = 3.141592*2/5;
		int starno=(t-32)/16;
		int time=t%16;
		double start=STAR_RAD*starno*2 - 3.141592/2;
		double end=STAR_RAD*(starno+1)*2 - 3.141592/2;

		double sx=cos(start)*size;
		double sy=(sin(start)*size)/2;
		double ex=cos(end)*size;
		double ey=(sin(end)*size)/2;

		nx=x+(ex*time+sx*(16-time))/16;
		ny=y+(ey*time+sy*(16-time))/16;
	}else{
		nx=x+0;
		ny=y-size/2;
	}
	effect->OperationA(int(nx),int(ny));
	effect->SetPos(nx,ny);
}int CAct_168::OnTimer()				//ÉfÉXÉTÅ[ÉNÉã
{
//íËå^ï∂////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////
	const int CIRCLE_SPEED = 4;

	int target= ActiveCharacter()->GetTarget();
	int x=GetCharacter(target)->GetCX()+BATTLE_SQUARE_X;
	int y=GetCharacter(target)->GetCY()+BATTLE_SQUARE_Y+24;
	switch(mModeTimer){
		case 1:
			ChangeFieldColor(0xb0000000,8);
			
			mActionEffect[0]=GetBottomEffect().SetEffect(
								BE_TAIL_LINE,
								x,y,
								BE_SPEED(0,0),64+4*0x10000,
								BLEND_ADD,0xffff8000);

			mActionEffect[0]->SetPos(x,y);

			break;
			
		case 48:
			int no,*target;
			target=GetTarget(&no);

			ChargeBreak();	//É`ÉÉÅ[ÉWè¡ãé

			for(int i=0;i<no;i++){
				int t=target[i]&0xffff;
				Attack(	mActiveCharacter,
						t,
						GetCharacter(t)->GetHP()-1,0,
						AT_TYPE_NO_RAND,
						AT_ELE_NONE , 0 , SE_PARAM_UP,target[i]/0x10000);
			}
			for(int i=0;i<64;i++){
				CBattleEffect *effect= &GetBottomEffect();
				if(i<32)	effect=&GetTopEffect();
				double rad=i*3.141592*2/64;
				int nx=int(x+cos(rad)*size);
				int ny=int(y+(sin(rad)*size)/2);

				effect->SetEffect(
						BE_TAIL_LINE,
						nx-BATTLE_SQUARE_X,ny-BATTLE_SQUARE_Y+32,
						BE_SPEED(0,-32),16+4*0x10000,
						BLEND_ADD,0xffff8000);
				
			}
			BreakTargetLock();
			break;
		case 60:
			mActionEffect[0]->Destroy();
			return 1;
	}
	if(mModeTimer<32 && mModeTimer%4==1){
		PlayWave(SE_STAR);
	}
	for(int i=0;i<CIRCLE_SPEED;i++){
		DrawCircle((mModeTimer-1)*CIRCLE_SPEED+i-1,x,y,mActionEffect[0]);
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////
int CAct_170::OnTimer()				//ÉtÉçÅ[ÉgÉuÉåÉCÉh
{
//íËå^ï∂////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////
	const int HIT_NO=6;
	const int INTERVAL=8;
	
	if(mModeTimer==1){
		mActionParam[0]=new int[HIT_NO];
		mActionParam[1]=new int[HIT_NO];
		mActionParam[2]=new int[HIT_NO];
		mActionParam[3]=new int[HIT_NO];
	}
	for(int i=0;i<HIT_NO;i++){
		int time=mModeTimer-i*INTERVAL;
		if(time==1){
			int x=ActiveCharacter()->GetCX()+pInput->Rand(61)-30;
			int y=ActiveCharacter()->GetCY()+pInput->Rand(61)-30;

			double sx,sy;
			int target=GetRandomTarget();
			double length=mBattleMode->GetShotSpeed(x,y,GetCharacter(target)->GetCX(),GetCharacter(target)->GetCY(),48,&sx,&sy);
			sy+=int(100/length);

			mActionParam[0][i]=int(length);
			mActionParam[1][i]=int(sx*1000);
			mActionParam[2][i]=int(sy*1000);
			mActionParam[3][i]=target;

			ActiveCharacter()->ChangeAni(13*ANI_TYPE,16,true);
			mActionEffect[i]=GetTopEffect().SetEffect(
							BE_SMALL_MOVE_SPIN,
							x,
							y,
							BE_SPEED(0,-8),68+52*0x10000);
		}

		if(time==12){
			mActionEffect[i]->OperationA(0,0);
		}
		if(time==40-mActionParam[0][i]){
			CBattleEffect* effect=mActionEffect[i];
			mActionEffect[i]=GetTopEffect().SetEffect(
				BE_SMALL_SHOT,
				0,0,
				BE_SPEED(mActionParam[1][i]/1000.0,mActionParam[2][i]/1000.0),68);
			mActionEffect[i]->SetPos(effect->GetX(),effect->GetY());
			effect->Destroy();
		}
		if(time==40){
			int no,*target;
			target=GetTarget(
							mActionParam[3][i],
							4,&no);		//å¯â îÕàÕÇÕÅuéÀÇÈÅvÇ…àÀë∂ÅI

			Attack(	mActiveCharacter,
				target,no,
				60,0,
				AT_TYPE_WEAPON,
				AT_ELE_NONE,0,SE_SLASH);
			if(i==HIT_NO-1)	BreakTargetLock();

		}
	}

	if(mModeTimer>=INTERVAL*HIT_NO+60){
		delete[] mActionParam[0];
		delete[] mActionParam[1];
		delete[] mActionParam[2];
		delete[] mActionParam[3];
		mActionParam[0]=NULL;
		mActionParam[1]=NULL;
		mActionParam[2]=NULL;
		mActionParam[3]=NULL;
		return 1;
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////
int CAct_171::OnTimer()				//ÉTÉCÉåÉìÉgÉèÅ[ÉãÉh
{
//íËå^ï∂////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////
	const int TIME =60;
	if(mModeTimer==1){
		ChangeFieldColor(0xffffffff,4);
	
		int no,*target;
		target=GetTarget(&no);
		for(int i=0;i<no;i++){
			mActionEffect[i%ACTION_EFFECT_NO]=GetTopEffect().SetEffect(BE_CHARACTER,
							0,0,
							target[i]&0xffff,1,
							BLEND_STANDARD,0xff000000);
		}
		GetTopEffect().SetEffect(BE_FLASH,
						0,0,
						16,0,
						BLEND_ADD,0xffffffff);

	}else if(mModeTimer<TIME){
	}else if(mModeTimer==TIME){
		ChangeFieldColor(0x00ffffff,32);

		int no,*target;
		target=GetTarget(&no);
		for(int i=0;i<no;i++){
			bool hit=false;
			if(SetStatus(mActiveCharacter,target[i]&0xffff,1,200))	hit=true;
			if(SetStatus(mActiveCharacter,target[i]&0xffff,3,200))	hit=true;
			if(SetStatus(mActiveCharacter,target[i]&0xffff,4,200))	hit=true;
			if(hit){
				GetCharacter(target[i]&0xffff)->SetDamageEffect(false);
			}
		}
		BreakTargetLock();
	}else if(mModeTimer<TIME+32){
		D3DCOLOR color=(TIME+32-mModeTimer)*0x08000000;
		for(int i=0;i<ACTION_EFFECT_NO;i++){
			if(mActionEffect[i]){
				mActionEffect[i]->SetColor(color);
			}
		}
	}else{
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
int CAct_172::OnTimer()				//à≈ÇÃîgìÆ
{
//íËå^ï∂////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////
	const int END_TIME		= 150;
	const int DMG_TIME		= 64;
	const int DMG_INTERVAL	= 4;
	const int HIT			= 12;

	int shake;
	if(mModeTimer<END_TIME/2){
		shake=mModeTimer/2;
	}else{
		shake=(END_TIME-mModeTimer)/2;
	}
	if(shake>16)	shake=16;
	int nx=int(sin(mModeTimer*2.0)*shake);
	int ny=int(cos(mModeTimer*5.23)*shake);
	//int dx=nx-mFieldShakeX;
	//int dy=ny-mFieldShakeY;
	SetFieldShake(nx,ny);

	if(mModeTimer % 4==1){
		PlayWave(SE_MOVE_BLOCK);
	}
	if(mModeTimer<DMG_TIME){
		for(int i=0;i<3;i++){
			double sx=pInput->Rand(800)*0.01+4;
			int y=pInput->Rand(480)+40;
			GetTopEffect().SetEffect(BE_MOVE,
							-50-y/2,y,
							BE_SPEED(sx,0),20,
							BLEND_SUB,0xffffff80);
		}
	}
	if(mModeTimer>=DMG_TIME && mModeTimer<DMG_TIME+DMG_INTERVAL*HIT && mModeTimer%DMG_INTERVAL==0){
		int no,*target;
		target=GetTarget(&no);

		Attack(mActiveCharacter,
				target,no,
				30,0,
				AT_TYPE_MAGIC,
				AT_ELE_NONE,0,SE_EXPLOSION);
	}
	if(mModeTimer>=END_TIME){
		return 1;
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////
int CAct_173::OnTimer()				//ìSãÖìäÇ∞
{
//íËå^ï∂////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////
	const int THROW_TIME = 24;
	double sx,sy;
	int target=ActiveCharacter()->GetTarget();
	double length=mBattleMode->GetShotSpeed(mActiveCharacter,target,24,&sx,&sy);

	switch(mModeTimer){
		case 1:
			ActiveCharacter()->ChangeAni(9*ANI_TYPE,16,true);
			break;
		case 8:
			PlayWave(SE_GUST);
			mActionEffect[0]=GetTopEffect().SetEffect(BE_SMALL_SHOT,
							ActiveCharacter()->GetCX(),
							ActiveCharacter()->GetCY(),
							BE_SPEED(sx,sy),69);
			for(int i=1;i<ACTION_EFFECT_NO;i++){
				double rsx=sx*i/(ACTION_EFFECT_NO);
				double rsy=sy*i/(ACTION_EFFECT_NO);
				mActionEffect[i]=GetTopEffect().SetEffect(BE_SMALL_SHOT,
								ActiveCharacter()->GetCX(),
								ActiveCharacter()->GetCY(),
								BE_SPEED(rsx,rsy),70);
			}
			ActiveCharacter()->ChangeAni(11*ANI_TYPE,30,true);
		case 12:
			ActiveCharacter()->ChangeAni(10*ANI_TYPE,THROW_TIME*2+1,true);
			break;
		case 12+THROW_TIME:
			mActionEffect[0]->OperationD(-sx,-sy);
			for(int i=1;i<ACTION_EFFECT_NO;i++){
				double rsx=-sx*i/(ACTION_EFFECT_NO);
				double rsy=-sy*i/(ACTION_EFFECT_NO);
				mActionEffect[i]->OperationD(rsx,rsy);
			}
			break;
		case 12+THROW_TIME*2-4:
			ActiveCharacter()->ChangeAni(11*ANI_TYPE,30,true);
			break;
		case 12+THROW_TIME*2:
			ActiveCharacter()->ChangeAni(9*ANI_TYPE,8,true);
			PlayWave(SE_BOW);
			for(int i=0;i<ACTION_EFFECT_NO;i++){
				mActionEffect[i]->Destroy();
				mActionEffect[i]=NULL;
			}
			break;
		case 12+THROW_TIME*2+16:
			return 1;
			break;


	}

	if(mModeTimer-8==int(length)){
		int no,*target;
		target=GetTarget(&no);

		Attack(	mActiveCharacter,
				target,no,
				300,0,
				AT_TYPE_BOW,
				AT_ELE_NONE,AT_OPT_HALF_HIT,SE_HIT);
		BreakTargetLock();
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////
int CAct_174::OnTimer()				//çïéÄíπ
{
//íËå^ï∂////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////
	const int	START_TIME	= 10;
	const int	ANI_TIME	= 16;
	const int	END_TIME	= START_TIME+ANI_TIME*4;;

	if(mModeTimer==START_TIME){
		PlayWave(SE_BLACK_HOLE);
		int no,*target;
		target=GetTarget(&no);
		for(int i=0;i<4;i++){
			const int ani[4]={5,4,1,0};
			mActionEffect[i]=GetTopEffect().SetEffect(
					BE_ANIMATION,
					GetCharacter(ActiveCharacter()->GetTarget())->GetCX(),
					GetCharacter(ActiveCharacter()->GetTarget())->GetGCY()-48,
					57,0,BLEND_SUB,0xff000000);
			mActionEffect[i]->OperationA(ani[i],0);
		}
	}else if(mModeTimer==END_TIME){
		int no,*target;
		target=GetTarget(&no);
		for(int i=0;i<no;i++){
			GetCharacter(target[i]&0xffff)->SetDamageEffect(false);
			SetStatus(mActiveCharacter,target[i]&0xffff,7,250);
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
				mActionEffect[i]->SetPos(mActionEffect[i]->GetX(),mActionEffect[i]->GetY()+1);
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
			mActionEffect[i]->SetColor(D3DCOLOR_ARGB(255,alpha,alpha,alpha));
		}
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////
int CAct_180::OnTimer()				//âŒâäåï
{
//íËå^ï∂////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////
	int target=ActiveCharacter()->GetTarget();
	int sy=GetCharacter(target)->GetSY();
	if(mModeTimer==8){
		ActiveCharacter()->SetGoalSquare(4,sy,16,1,true);
	}
	if(mModeTimer==24){
		ActiveCharacter()->ChangeAni(9*ANI_TYPE,32,true);
		GetTopEffect().SetEffect(
			BE_FLASH_CHARACTER,
			0,0,
			mActiveCharacter,0,BLEND_SOLID_ADD,0xffff0000);
		PlayWave(SE_PARAM_UP);
	}
	if(mModeTimer==48){
			ActiveCharacter()->ChangeAni(11*ANI_TYPE,24,true);
			ActiveCharacter()->SetGoal(
				(CheckEnemy(mActiveCharacter,target)?560:0),
				ActiveCharacter()->GetY(),
				8,false);
	}
	if(mModeTimer==52){
			ActiveCharacter()->ChangeAni(10*ANI_TYPE,24,true);
	}if(mModeTimer==48+2*5+1){
			BreakTargetLock();
	}
	if(mModeTimer>=48 && mModeTimer<=48+2*5){

		int movex;
		int turn;
		if(CheckEnemy(mActiveCharacter,target)){
			movex=BATTLE_SQUARE_WIDTH;
			turn=0;
		}else{
			turn=TURN_HORIZONTAL;
			movex=-1;
		}
		for(int j=0;j<24;j++){
			int t=(turn?1:-1);
			double x=(pInput->Rand(1000)*t)/100.0;
			double y=(pInput->Rand(300)-500)/100.0;
			GetTopEffect().SetEffect(BE_FALL_PARTICUL,
						ActiveCharacter()->GetCX()-32*t	+pInput->Rand(60)-30,
						ActiveCharacter()->GetGCY()		+pInput->Rand(60)-30,
						BE_SPEED(x,y),32*0x10000+10,
						BLEND_ADD);
		}

		if(mModeTimer%2==0){
			PlayWave(SE_FIRE);
			int x=(mModeTimer-48)/2;
			if(!CheckEnemy(mActiveCharacter,target))	x=x*-1;
			int no,*target;
			target=GetTarget(&no);

			ChargeBreak();	//É`ÉÉÅ[ÉWè¡ãé

			for(int i=0;i<no;i++){
				if(GetCharacter(target[i]&0xffff)->GetSX()==4+x){
					int t=target[i]&0xffff;
					Attack(	mActiveCharacter,
							t,200,MIN_ATK_LV_2,
							AT_TYPE_WEAPON,
							AT_ELE_FIRE,0,SE_FIRE,target[i]/0x10000);
				}
			}
		}
	}

	if(mModeTimer>=80){
		return 1;
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////
int CAct_181::OnTimer()				//ïXê·åï
{
//íËå^ï∂////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////
	int turn;
	if(CheckEnemy(mActiveCharacter))	turn=TURN_HORIZONTAL;
	else	turn=0;

	int no,*target;
	target=GetTarget(&no);

	switch(mModeTimer){
		case 1:
			PlayWave(SE_MAGIC);
			for(int i=0;i<128;i++){
				int angle=pInput->Rand(360);
				int length=pInput->Rand(320)+64;
				GetTopEffect().SetEffect(
								BE_CONCENTRATE,
								ActiveCharacter()->GetCX(),
								ActiveCharacter()->GetCY(),
								angle+length*0x10000,54+24*0x10000,
								BLEND_ADD,0xff0000ff);
			}

			ActiveCharacter()->ChangeAni(9*ANI_TYPE,32,true);
			break;

		case 31:
			ActiveCharacter()->ChangeAni(9*ANI_TYPE,8,true);
			break;
		case 34:
			ActiveCharacter()->ChangeAni(11*ANI_TYPE,12,true);

			for(int i=0;i<64;i++){
				double spd=pInput->Rand(300)/10.0+10;
				double rad=pInput->Rand(314*2)/100.0;
				GetTopEffect().SetEffect(BE_PARTICUL,
					GetCharacter(ActiveCharacter()->GetTarget())->GetCX(),GetCharacter(ActiveCharacter()->GetTarget())->GetCY(),
					BE_SPEED(cos(rad)*spd,sin(rad)*spd),11+16*0x10000,BLEND_ADD);
			}

			Attack(	mActiveCharacter,
					target,no,
					200,MIN_ATK_LV_2,
					AT_TYPE_WEAPON,
					AT_ELE_ICE,0,SE_HARD_ICE);
			BreakTargetLock();
			break;
		case 37:
			ActiveCharacter()->ChangeAni(10*ANI_TYPE,8,true);
		case 40:
			PlayWave(SE_HARD_ICE);
			break;

		case 70:
			return 1;	break;
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////
int CAct_182::OnTimer()				//ïóêwåï
{
//íËå^ï∂////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////
	int turn;
	if(CheckEnemy(mActiveCharacter))	turn=TURN_HORIZONTAL;
	else	turn=0;

	int no,*target;
	target=GetTarget(&no);

	switch(mModeTimer){
		case 1:
			ActiveCharacter()->ChangeAni(10*ANI_TYPE+1,16,true);
			break;
		case 3:
			ActiveCharacter()->ChangeAni(11*ANI_TYPE+1,12,true);
			GetTopEffect().SetEffect(BE_LARGE,
				ActiveCharacter()->GetCX(),ActiveCharacter()->GetCY(),
				27,turn,BLEND_ADD);
			PlayWave(SE_GUST);
			break;
		case 5:
			ActiveCharacter()->ChangeAni(9*ANI_TYPE+1,18,true);
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
			turn=turn^TURN_HORIZONTAL;
			turn=turn | TURN_VERTICAL;
			GetTopEffect().SetEffect(BE_LARGE,
				ActiveCharacter()->GetCX(),ActiveCharacter()->GetCY(),
				27,turn,BLEND_ADD);
			break;
		case 13:
			ActiveCharacter()->SetTurn(false);
			ActiveCharacter()->ChangeAni(10*ANI_TYPE+1,16,true);
			break;
		case 15:
			ActiveCharacter()->ChangeAni(11*ANI_TYPE+1,12,true);
			break;
		case 17:
			ActiveCharacter()->ChangeAni(9*ANI_TYPE+1,25,true);
			break;


		case 31:
			ActiveCharacter()->ChangeAni(9*ANI_TYPE,8,true);
			break;
		case 34:
			ActiveCharacter()->ChangeAni(11*ANI_TYPE,12,true);

			GetTopEffect().SetEffect(BE_STANDARD,
				GetCharacter(ActiveCharacter()->GetTarget())->GetCX(),GetCharacter(ActiveCharacter()->GetTarget())->GetCY(),
				28,turn,BLEND_ADD);

			Attack(	mActiveCharacter,
					target,no,
					350,MIN_ATK_LV_2,
					AT_TYPE_WEAPON^AT_AVOID,
					AT_ELE_WIND,0,SE_WIND);
			BreakTargetLock();
			break;
		case 37:
		case 40:
		case 43:
			GetTopEffect().SetEffect(BE_STANDARD,
				GetCharacter(ActiveCharacter()->GetTarget())->GetCX()+pInput->Rand(31)-15,
				GetCharacter(ActiveCharacter()->GetTarget())->GetCY()+pInput->Rand(31)-15,
				28,turn,BLEND_ADD);
			PlayWave(SE_WIND);
			ActiveCharacter()->ChangeAni(10*ANI_TYPE,22,true);
			break;

		case 70:
			return 1;	break;
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////
int CAct_183::OnTimer()				//ínó≤åï
{
//íËå^ï∂////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////
	switch(mModeTimer){
		case 1:
			ActiveCharacter()->ChangeAni(9*ANI_TYPE,32,true);
			
			GetTopEffect().SetEffect(
				BE_FLASH_CHARACTER,
				0,0,
				mActiveCharacter,0,BLEND_SOLID_ADD,0xffffff00);
			PlayWave(SE_PARAM_UP);
			break;
		case 30:
			ActiveCharacter()->ChangeAni(11*ANI_TYPE,12,true);
			break;
		case 34:
			ActiveCharacter()->ChangeAni(10*ANI_TYPE,22,true);
			int no,*target;
			target=GetTarget(&no);

			Attack(	mActiveCharacter,
					target,no,
					120,MIN_ATK_LV_2,
					AT_TYPE_WEAPON,
					AT_ELE_EARTH,AT_OPT_NO_AIR,SE_STONE);

			BreakTargetLock();
			break;
		case 80:
			for(int i=0;i<ACTION_EFFECT_NO;i++){
				if(mActionEffect[i]){
					mActionEffect[i]->Destroy();
					mActionEffect[i]=NULL;
				}
			}
			return 1;
	}

	if(34<=mModeTimer && mModeTimer<=58 && mModeTimer%(16/ACTION_EFFECT_NO)==0){
		int no=(mModeTimer/(16/ACTION_EFFECT_NO))%ACTION_EFFECT_NO;
		int target=ActiveCharacter()->GetTarget();
		
		int x=pInput->Rand(320);
		int y=80+pInput->Rand(300);
		if(!CheckEnemy(mActiveCharacter,target)){
			x=-x;
		}
		x+=320;

		if(mActionEffect[no]){
			mActionEffect[no]->Destroy();
		}
		mActionEffect[no]=GetBottomEffect().SetEffect(
				BE_STRETCH,
				x,y,15,0);
		PlayWave(SE_STONE);
	}
	for(int i=0;i<ACTION_EFFECT_NO;i++){
		if(mActionEffect[i]){
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
	return 0;
}
////////////////////////////////////////////////////////////////////////////
int CAct_184::OnTimer()				//É^Å[ÉQÉbÉeÉBÉìÉO
{
//íËå^ï∂////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////
	const int	START_TIME	= 1;
	const int	STOP_TIME	= 16;
	const int	DELETE_TIME	= STOP_TIME+20;
	const int	END_TIME	= DELETE_TIME+16;

	const int	MAX_SIZE	= 160;
	const int	MIN_SIZE	= 64;

	if(mModeTimer==START_TIME){
		ActiveCharacter()->ChangeAni(13*ANI_TYPE,DELETE_TIME,true);
		PlayWave(SE_MAGIC);
		int no,*target;
		target=GetTarget(&no);
		mActionEffect[0]=GetTopEffect().SetEffect(
				BE_STRETCH,
				GetCharacter(ActiveCharacter()->GetTarget())->GetCX(),
				GetCharacter(ActiveCharacter()->GetTarget())->GetCY(),
				76,MAX_SIZE+MAX_SIZE*0x10000,BLEND_STANDARD,0x00ffffff);
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
			SetStatus(mActiveCharacter,target[i]&0xffff,25,1000);
		}
		BreakTargetLock();
	}else if(DELETE_TIME<mModeTimer && mModeTimer<END_TIME){
		D3DCOLOR color=((END_TIME-mModeTimer)*0xff/(END_TIME-DELETE_TIME)*0x1000000) | 0x00ffffff;
		mActionEffect[0]->SetColor(color);
	}else if(mModeTimer>=END_TIME){
		if(mActionEffect[0]){
			mActionEffect[0]->Destroy();
			mActionEffect[0]=NULL;
		}
		return 1;
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////
int CAct_185::OnTimer()				//ïúèQÇÃàÍåÇ
{
//íËå^ï∂////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////
	int turn;
	if(CheckEnemy(mActiveCharacter))	turn=TURN_HORIZONTAL;
	else	turn=0;

	int no,*target;
	target=GetTarget(&no);

	switch(mModeTimer){
		case 1:
			PlayWave(SE_PARAM_UP);
			GetTopEffect().SetEffect(
				BE_FLASH_CHARACTER,
				0,0,
				mActiveCharacter,0,BLEND_STANDARD,0xff000000);
			ActiveCharacter()->ChangeAni(9*ANI_TYPE,32,true);
			break;

		case 31:
			ActiveCharacter()->ChangeAni(9*ANI_TYPE,8,true);
			break;
		case 34:
			ActiveCharacter()->ChangeAni(11*ANI_TYPE,12,true);

			int t;
			t=ActiveCharacter()->GetTarget();
			for(int i=0;i<32;i++){
				double x=(pInput->Rand(900)-450)/100.0;
				double y=(pInput->Rand(500)-600)/100.0;
				GetTopEffect().SetEffect(BE_FALL_PARTICUL,
							GetCharacter(t)->GetCX(),
							GetCharacter(t)->GetCY(),
							BE_SPEED(x,y),32*0x10000+10,
							BLEND_SUB);
			}
			
			GetTopEffect().SetEffect(
				BE_FLASH_CHARACTER,
				0,0,
				t,0,BLEND_STANDARD,0xff000000);

			Attack(	mActiveCharacter,
					target,no,
					ActiveCharacter()->GetMaxHP() - ActiveCharacter()->GetHP(),0,
					AT_TYPE_NO_RAND | AT_SKILL,
					AT_ELE_NONE,0,SE_EXPLOSION);
			BreakTargetLock();
			break;
		case 37:
			ActiveCharacter()->ChangeAni(10*ANI_TYPE,8,true);
			break;
		case 60:
			return 1;	break;
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////
int CAct_186::OnTimer()				//ó{ï™ãzé˚
{
//íËå^ï∂////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////
	int no,*target;
	target=GetTarget(&no);
	for(int i=0;i<no;i++){
		switch(mModeTimer-i*4){
			case 1:
				int t;
				t=target[i]&0xffff;
				ActiveCharacter()->ChangeAni(6*ANI_TYPE,100,true);
				mActionEffect[i]=GetTopEffect().SetEffect(
							BE_ANIMATION,
							GetCharacter(t)->GetCX(),
							GetCharacter(t)->GetGCY(),
							33,0);
				PlayWave(SE_MOVE_BLOCK);
				break;
			case 120-4:
				mActionEffect[i]->OperationA(0,0);	break;
			case 5:
			case 120-8:
				mActionEffect[i]->OperationA(1,0);	break;
			case 9:
				mActionEffect[i]->OperationA(4,0);	break;

			case 120:
				mActionEffect[i]->Destroy();
		}
	}
	if(mModeTimer==101){
		BreakTargetLock();
	}
	if(mModeTimer>=120+no*4){
		return 1;
	}
	if(mModeTimer>=40 && mModeTimer<=110){
		if(mModeTimer%30==10 && mModeTimer>=30){
			for(int i=0;i<no;i++){
				if(mActionEffect[i])	mActionEffect[i]->OperationA(5,0);
			}
			int heal,*hit;
			heal=0;
			hit=Attack(	mActiveCharacter,
					target,no,
					30,0,
					AT_TYPE_WEAPON,
					AT_ELE_EARTH,AT_OPT_NO_AIR,SE_BLOOD);

			for(int i=0;i<no;i++){
				if(hit[i]<0)	continue;
				heal+=hit[i];
			}
			
			if(heal>0){
				GetTopEffect().SetEffect(
						BE_FLASH_CHARACTER,
						0,0,
						mActiveCharacter,0,BLEND_SOLID_ADD,0xff00ff00);
				HealHP(	mActiveCharacter,
						mActiveCharacter,
						heal );
			}

		}else if(mModeTimer%30==20){
			for(int i=0;i<no;i++){
				mActionEffect[i]->OperationA(4,0);
			}
		}
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////
int CAct_187::OnTimer()				//óêÇÍñn
{
//íËå^ï∂////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////
	int no,*target;
	target=GetTarget(&no);

	bool end=true;
	for(int i=0;i<no;i++){
		int time=mModeTimer-i*16;
		int t=target[i]&0xffff;
		if(GetCharacter(t)->GetDead())	continue ;
		double sx,sy;
		double length=mBattleMode->GetShotSpeed(mActiveCharacter,t,20,&sx,&sy);
		if(time==1){
				PlayWave(SE_POO);
				mActionEffect[i]=GetTopEffect().SetEffect(BE_TAIL_MOVE,
								ActiveCharacter()->GetCX(),
								ActiveCharacter()->GetCY(),
								BE_SPEED(sx,sy),46,BLEND_STANDARD);
				ActiveCharacter()->ChangeAni(13*ANI_TYPE+3,10,true);
		}

		if(time-1==int(length)){
			int st=SetStatus(mActiveCharacter,t,18,100);
			if(st){
				if(mActionEffect[i]){
					mActionEffect[i]->Destroy();
					for(int j=0;j<16;j++){
						double x=(pInput->Rand(1100)-550)/100.0;
						double y=(pInput->Rand(700)-350)/100.0;
						GetTopEffect().SetEffect(BE_FALL_PARTICUL,
									GetCharacter(t)->GetCX(),
									GetCharacter(t)->GetCY(),
									BE_SPEED(x,y),16*0x10000+21,
									BLEND_STANDARD,0x80000000);
					}
				}
				GetCharacter(t)->SetDamageEffect(false);
				if(i==no-1)	BreakTargetLock();
			}else{
				GetCharacter(t)->SetAvoid();
				PlayWave(SE_MISS);
			}
		}
		if(time-30<=int(length)){
			end=false;
		}
	}
	if(end){
		return 1;
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////
int CAct_188::OnTimer()				//100ÉJÉEÉìÉgÉoÉäÉA
{
//íËå^ï∂////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////
	if(mModeTimer<=100){
		int turn;
		if(GetCharacter(ActiveCharacter()->GetTarget())->Enemy())	turn=1;
		else	turn=-1;
		PlayWave(SE_HIT);

		GetTopEffect().SetEffect(
			BE_STANDARD,
			GetCharacter(ActiveCharacter()->GetTarget())->GetCX()+pInput->Rand(100)-50,
			GetCharacter(ActiveCharacter()->GetTarget())->GetCY()+pInput->Rand(100)-50,
			40,(turn>0?TURN_HORIZONTAL:0),
			BLEND_STANDARD,0xffffffff);
	}else if(mModeTimer==101){
		ActiveCharacter()->ChangeAni(13*ANI_TYPE,16,true);
		int no,*target;
		target=GetTarget(&no);	

		GetTopEffect().SetEffect(BE_FLASH,
				0,0,8,0,BLEND_STANDARD,0xffffffff);

		for(int i=0;i<no;i++){
			SetStatus(mActiveCharacter,target[i]&0xffff,16,100,100);
		}
		BreakTargetLock();
	}else if(mModeTimer==120){
		return 1;
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////
int CAct_198::OnTimer()				//ÉåÅ[ÉUÅ[ÉJÉmÉì
{
//íËå^ï∂////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////
	const int LASER_START = 96;
	const int INTERVAL = 4;
	const int HIT = 10;
	if(mModeTimer<LASER_START){
		ActiveCharacter()->ChangeAni(9*ANI_TYPE,LASER_START,true);
		int angle=pInput->Rand(360);
		int t=(LASER_START-mModeTimer)/2;
		if(t<=0)	t=1;
		for(int i=0;i<2;i++){
			GetTopEffect().SetEffect(
							BE_CONCENTRATE,
							ActiveCharacter()->GetCX(),
							ActiveCharacter()->GetCY(),
							angle+(t*8)*0x10000,54+t*0x10000,
							BLEND_ADD,0xff80b0ff);
		}
		if(mModeTimer%16==1){
			PlayWave(SE_PARAM_UP);
		}
	}else if(mModeTimer==LASER_START){
		ActiveCharacter()->ChangeAni(13*ANI_TYPE,64,true);

		int t=ActiveCharacter()->GetTarget();
		int tx=GetCharacter(t)->GetCX();
		int ty=GetCharacter(t)->GetCY();
		GetTopEffect().SetEffect(
			BE_LASER_CANON,
			ActiveCharacter()->GetCX(),
			ActiveCharacter()->GetCY(),
			79+64*0x10000,tx+ty*0x10000,
			BLEND_ADD,0xffffffff);
		GetTopEffect().SetEffect(BE_FLASH,
				0,0,8,0,BLEND_STANDARD,0xffffffff);
	}else if(mModeTimer<=LASER_START+INTERVAL*HIT){
		PlayWave(SE_LASER);
		if((mModeTimer-LASER_START)%INTERVAL==1){
			int no,*target;
			target=GetTarget(&no);

			Attack(	mActiveCharacter,
					target,no,
					10000,0,
					AT_TYPE_NO_RAND,
					AT_ELE_NONE,0,SE_CHOP);
		}
	}else if(mModeTimer==LASER_START+INTERVAL*HIT+1){
		BreakTargetLock();
	}else if(mModeTimer==LASER_START+INTERVAL*HIT+32){
		return 1;
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////
int CAct_199::OnTimer()				//ÉAÉCÉeÉÄÉXÉeÉBÅ[Éã
{
//íËå^ï∂////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////
	bool end=true;
	if(mModeTimer==1){
		mActionParam[0]=new int ;
		*mActionParam[0]=0;
	}
	if(mModeTimer %16 ==1) {
		int no,*target;
		target=GetTarget(&no);
		if((*mActionParam[0])==0){
			*mActionParam[0]=1;
			for(int i=0;i<no;i++){
				int t=target[i]&0xffff;
				CPlayerCharacter* PChara=dynamic_cast<CPlayerCharacter*>(GetCharacter(t));
				if(!PChara)			continue;

				int item=PChara->SearchUseItem(mModeTimer/16);
				if(item<0)			continue;

				int x=ActiveCharacter()->GetBaseCX();
				int y=ActiveCharacter()->GetBaseCY();
				GetTopEffect().SetEffect(BE_ITEM,
								GetCharacter(t)->GetCX(),
								GetCharacter(t)->GetCY(),
								x+y*0x10000,item);
				*mActionParam[0]=0;
				GetCharacter(t)->SetDamageEffect(false);
				ActiveCharacter()->ChangeAni(13*ANI_TYPE,12,true);
				PlayWave(SE_POO);
			}
		}else{
			(*mActionParam[0])++;
			if(*mActionParam[0]>=2){
				for(int i=0;i<no;i++){
					int t=target[i]&0xffff;
					CPlayerCharacter* PChara=dynamic_cast<CPlayerCharacter*>(GetCharacter(t));
					if(!PChara)			continue;
					PChara->DeleteAllUseItem();
				}
				delete mActionParam[0];
				mActionParam[0]=NULL;
				return 1;
			}
		}
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////
int CAct_200_203::OnTimer()			//ñ≤ÇÃóëÅEè¢ä“
{
//íËå^ï∂////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////
	CBattleMode &bm=*mBattleMode;
	switch(mModeTimer){
		case 120:

			{
				GetTopEffect().SetEffect(BE_FLASH,
								0,0,
								64,0,
								BLEND_STANDARD,0xffffffff);
				PlayWave(SE_FLASH);

				bm.mEnemy[0].Move(0,1,1);
				int x=bm.mEnemy[0].GetSX();
				int y=bm.mEnemy[0].GetSY();
				bm.mEnemy[0].SetPos(x*BATTLE_SQUARE_SIZE,y*BATTLE_SQUARE_SIZE,0);

				const CEnemyPartyData& ept=EnemyPartyData[mParty];
				if(ept.mField>=0){
					char filename[256];
					sprintf(filename,"data\\battlefield\\field%02d",ept.mField);
					DTest.LoadTexture(bm.mField,filename,0xff008080);
				}

				for(int i=0;i<MAX_ENEMY_NUMBER;i++){
					if(ept.mEnemy[i]<0)	break;
					
					bm.mEnemy[i+1].DeleteCharacter();
					bm.mEnemy[i+1].LoadCharacter(ept.mEnemy[i],ept.mLv[i],ept.mX[i],ept.mY[i],bm.mBattleOption);
					bm.mEnemy[i+1].SetPos(ept.mX[i]*BATTLE_SQUARE_SIZE,ept.mY[i]*BATTLE_SQUARE_SIZE,0);
				}
			}
			break;

		case 200:

			if(mEnd){
				bm.ChangeMode(BM_WIN);
				SetBGMFadeOut(BM_WIN_TIME);
			}
			return 1;
			break;
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////
int CAct_204::OnTimer()				//ñ≤ïˆÇµ
{
//íËå^ï∂////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////
	CBattleMode &bm=*mBattleMode;
	const int TIME = 128;
	switch(mModeTimer){
		case 1:
			bm.mDrawBG=true;
			return 0;

		case 2:
			mActionEffect[0]=GetBottomEffect().SetEffect(BE_BACK_GROUND,
								0,0,0,0);

			int x,y;
			x=EnemyPartyData[EncountInfo.mPartyNo].mX[0];
			y=EnemyPartyData[EncountInfo.mPartyNo].mY[0];
			bm.mEnemy[0].Move(x,y,1);
			bm.mEnemy[0].SetPos(x*BATTLE_SQUARE_SIZE,y*BATTLE_SQUARE_SIZE,0);

			for(int i=1;i<MAX_ENEMY_NUMBER;i++){
				bm.mEnemy[i].DeleteCharacter();
			}
			
			int no,mlp,hp;
			char filename[256];
			mlp=ActiveCharacter()->GetMaxLP();
			hp=ActiveCharacter()->GetHP();
			no=((mlp-hp-1)*4/mlp);
			sprintf(filename,"data\\battlefield\\field%02d",EncountInfo.mField+no);
			DTest.LoadTexture(bm.mField,filename,0xff008080);

			ActiveCharacter()->NoActionCount();
			bm.mTurn=0;

			break;
		case TIME:
			bm.mFieldShakeX=0;
			bm.mFieldShakeY=0;
			mActionEffect[0]->Destroy();
			ActiveCharacter()->SetHP(ActiveCharacter()->GetMaxHP());
			for(int i=0;i<MAX_PARTY_NUMBER;i++){
				if(!(bm.mPlayer[i].GetLive()))	continue;
				bm.mPlayer[i].SetAction(-1);
				HealLP(mActiveCharacter,i,bm.mPlayer[i].GetMaxLP());
			}
			for(int i=0;i<MAX_PET_NUMBER;i++){
				if(!(bm.mPet[i].GetLive()))	continue;
				bm.mPet[i].SetAction(-1);
			}
			return 1;
	}

	if(mModeTimer%4==1){
		PlayWave(SE_HIT);
	}
	int t=(mModeTimer-TIME/2);
	int shake=(TIME*TIME/4-t*t)/128;
	bm.mFieldShakeX=int(sin(mModeTimer*2.0)*shake);
	bm.mFieldShakeY=int(cos(mModeTimer*5.23)*shake);

	if(mActionEffect[0]){
		int alpha=(TIME-mModeTimer)*255/(TIME);
		D3DCOLOR color=0x00ffffff + alpha*0x01000000;
		mActionEffect[0]->SetPos(bm.mFieldShakeX,bm.mFieldShakeY);
		mActionEffect[0]->SetColor(color);
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////
int CAct_206::OnTimer()				//ñ≤ÇÃèIÇÌÇË
{
//íËå^ï∂////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////
	CBattleMode &bm=*mBattleMode;
	const int TIME = 192;
	switch(mModeTimer){
		case 1:
			bm.mDrawBG=true;
			return 0;

		case 2:
			mActionEffect[0]=GetBottomEffect().SetEffect(BE_BACK_GROUND,
								0,0,0,0);
			for(int i=1;i<MAX_PARTY_NUMBER;i++){
				if(!(bm.mPlayer[i].GetLive()))	continue;
				bm.mPlayer[i].DeleteCharacter();
			}
			for(int i=0;i<MAX_PET_NUMBER;i++){
				if(!(bm.mPet[i].GetLive()))	continue;
				bm.mPet[i].DeleteCharacter();
			}

			if(bm.mPlayer[0].GetDead()){
				bm.mPlayer[0].SetHP(1);
			}

			ActiveCharacter()->NoActionCount();
			break;
		case TIME:
			bm.mFieldShakeX=0;
			bm.mFieldShakeY=0;
			mActionEffect[0]->Destroy();
			return 1;
	}

	if(mModeTimer%4==1){
		PlayWave(SE_HARD_HIT);
	}
	int t=(mModeTimer-TIME/2);
	int shake=(TIME*TIME/4-t*t)/128;
	bm.mFieldShakeX=int(sin(mModeTimer*2.0)*shake);
	bm.mFieldShakeY=int(cos(mModeTimer*5.23)*shake);

	if(mActionEffect[0]){
		int alpha=(TIME-mModeTimer)*255/(TIME);
		D3DCOLOR color=0x00ffffff + alpha*0x01000000;
		mActionEffect[0]->SetPos(bm.mFieldShakeX,bm.mFieldShakeY);
		mActionEffect[0]->SetColor(color);
	}
	return 0;
}

////////////////////////////////////////////////////////////////////////////
int CAct_222::OnTimer()				//ÉfÉBÉXÉyÉã
{
//íËå^ï∂////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////

	int no,*target;
	target=GetTarget(&no);

	bool end=true;
	for(int i=0;i<no;i++){
		int time=mModeTimer-i*8;
		int t=target[i]&0xffff;
		if(time==1){
			for(i=0;i<16;i++){
				int color[3];
				memset(color,0,sizeof(color));
				color[pInput->Rand(3)]=0x80;
				color[pInput->Rand(3)]=0xff;

				double x=(pInput->Rand(1601)-800)/100.0;
				double y=(pInput->Rand(1601)-800)/100.0;
				GetTopEffect().SetEffect(BE_SPIN_PARTICUL,
							GetCharacter(t)->GetCX(),
							GetCharacter(t)->GetCY(),
							BE_SPEED(x,y),32*0x10000+80,
							BLEND_ADD,D3DCOLOR_ARGB(255,color[0],color[1],color[2]));
			}
			if(DispelStatus(mActiveCharacter,t)){
				GetCharacter(t)->SetDamageEffect(false);
			}
		}
		if(time<=30){
			end=false;
		}
	}
	if(end){
		return 1;
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////
int CAct_223::OnTimer()				//ÉOÉâÉrÉeÉBÅ[ÅEÉIÅ[Éã
{
//íËå^ï∂////////
	int ret=CBattleAction::OnTimer();
	if(ret!=0)		return ret;
////////////////
	int no,*target;
	target=GetTarget(&no);

	if(mModeTimer%2==0 && mModeTimer<64){
		for(int i=0;i<no;i++){
			int t=target[i]&0xffff;
			GetCharacter(t)->SetDamageEffect(false);
			int x=GetCharacter(t)->GetCX()+pInput->Rand(255)-127;
			int y=GetCharacter(t)->GetGCY()-480;
			for(int i=0;i<2;i++){
				GetTopEffect().SetEffect(
							BE_FALL_PARTICUL,
							x,y,
							BE_SPEED(0,32),35+20*0x10000,
							BLEND_ADD,0xffb040ff);
			}
		}
		PlayWave(SE_GRAVITY);
	}else if(mModeTimer==64){
		for(int i=0;i<no;i++){
			SetStatus(mActiveCharacter,target[i]&0xffff,9,95);
		}
		BreakTargetLock();
	}else if(mModeTimer==80){
		return 1;
	}
	return 0;
}