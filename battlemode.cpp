#include "battlemode.h"
#include "changemode.h"
#include "input.h"
#include "cursor.h"
#include "month.h"
#include "menudata.h"
#include "option.h"

#include "standardaction.h"
#include "swordaction.h"
#include "spearaction.h"
#include "axeaction.h"
#include "bowaction.h"
#include "fireaction.h"
#include "iceaction.h"
#include "windaction.h"
#include "earthaction.h"
#include "magicaction.h"
#include "specialaction.h"
#include "itemaction.h"
#include "enemyaction.h"

#define OFF_BGM()	{if(Flag.mMapBGMNo!=8 && Flag.mMapBGMNo!=16 && !(mBattleOption & BOPT_BGM_NO_CHANGE))	SetBGMFadeOut(BM_WIN_TIME);}


extern CDTTexture	gBackGround;

CBattleMode::CBattleMode()
{
	int i;
	for(i=0;i<MAX_PARTY_NUMBER;i++){
		mCharacter[i]=&mPlayer[i];
	}
	for(i=0;i<MAX_ENEMY_NUMBER;i++){
		mCharacter[i+ENEMY]=&mEnemy[i];
	}
}

void CBattleMode::AfterStartUp(void)
{
	DTest.CreateTexture(mBackGround,640,480);
}
void CBattleMode::LoadTexture(void)
{
	for(int i=0;i<MAX_ENEMY_NUMBER;i++){
		mCharacter[i+ENEMY]=&mEnemy[i];
	}
	for(int i=0;i<MAX_PET_NUMBER;i++){
		mCharacter[i+PET]=&mPet[i];
	}
	for(int i=0;i<MAX_WALL_NUMBER;i++){
		mCharacter[i+WALL]=&mWall[i];
	}

	for(int i=0;i<CHARACTER_NO;i++){
		mCharacter[i]->ClearAllStatus();
		mCharacter[i]->DeleteCharacter();
	}

	char filename[256];
#ifdef _BATTLE_BG_
	EncountInfo.mField=_BATTLE_BG_;
#endif
	sprintf(filename,"data\\battlefield\\field%02d",EncountInfo.mField);
	DTest.LoadTexture(mField,filename);

	DTest.LoadTexture(mFieldEffectTex,"data\\battlefield\\fieldeffect",0xff008080);

	for(int i=0;i<MAX_PARTY_NUMBER;i++){
		mCharacter[i]->LoadImage(Flag.mParty[i]);
	}
}
void CBattleMode::LoadBattle(void)
{
	Flag.CheckFreeSpace();
	for(int i=0;i<MAX_CHARACTER_NUMBER;i++){
		Flag.mSaveCursor.Set(SC_P_PLAYER_TARGET+i,-1);
		Flag.mSaveCursor.Set(SC_P_ENEMY_TARGET+i,-1);
	}
	//char filename[256];

	memset(mMoveCharacter,0,sizeof(mMoveCharacter));
	memset(mFieldFlash,0,sizeof(mFieldFlash));
	memset(mFieldEffect,0,sizeof(mFieldEffect));
	mColorChangeTimer=-1;
	mMagicField=0;
	mFieldColor=BattleFieldColor[mMagicField];
	mFieldShakeX=0;
	mFieldShakeY=0;
	mTargetLineFrom=-1;
	mEscapeCount=0;
	mBattleOption=EncountInfo.mOption;
	mTurn=0;
	mActionList.clear();
	mActionNameList.clear();
	mIntervalTimer=-1;
	mIntervalType=0;
	mDrawBG=false;

	for(int i=0;i<MAX_PARTY_NUMBER;i++){
		mCharacter[i]->LoadCharacter(	Flag.mParty[i],
										Flag.mFormationX[i]+5,
										Flag.mFormationY[i],
										mBattleOption);
	}

	int addlv=MonthData[Flag.mCurrentMonth].GetBaseLv();
	addlv+=EncountInfo.mLvUp;

#ifdef _ENEMY_LOCK_
	EncountInfo.mReader	=_ENEMY_LOCK_TYPE_;
	EncountInfo.mLv		=_ENEMY_LOCK_LV_;
	EncountInfo.mMember	=_ENEMY_LOCK_NO_;
#endif

	if(EncountInfo.mPartyNo<0){
		static int  enemyfield[BATTLE_SQUARE_Height][BATTLE_SQUARE_Height];
		static int	enemyrank[MAX_ENEMY_NUMBER];
		memset(enemyfield,0,sizeof(enemyfield));
		memset(enemyrank,-1,sizeof(enemyrank));

		SetEnemy(0,EncountInfo.mReader,EncountInfo.mLv+addlv,enemyfield,mBattleOption);
		enemyrank[0]=0x7fffffff;
 
		int i=1;
		for(int test=0;test<EncountInfo.mMember*2;test++){
			if(i>=EncountInfo.mMember)	break;
			int lv=EncountInfo.mLv+addlv;
			int enemy=Flag.mEnemyType[pInput->Rand(Flag.mEnemyTypeNo)];
#ifdef _ENEMY_LOCK_
			enemy=_ENEMY_LOCK_TYPE_;
#endif
			int ret=SetEnemy(i,enemy,lv,enemyfield,mBattleOption);
			if(ret>0){
				enemyrank[i]=mEnemy[i].GetSX()*BATTLE_SQUARE_Height-mEnemy[i].GetSY();
				i+=ret;
			}
		}

		bool turn=true;
		while(turn){
			turn=false;
			for(int n=1;n<EncountInfo.mMember;n++){
				if(enemyrank[n] > enemyrank[n-1]){
					turn=true;
					CBattleCharacter* c		=mCharacter[ENEMY+n];
					mCharacter[ENEMY+n]		=mCharacter[ENEMY+n-1];
					mCharacter[ENEMY+n-1]	=c;
					int z			=enemyrank[n];
					enemyrank[n]	=enemyrank[n-1];
					enemyrank[n-1]	=z;
				}
			}
		}
	}else{
		const CEnemyPartyData &party=EnemyPartyData[EncountInfo.mPartyNo];
		int i=0;
		while(party.mEnemy[i]>=0){
			mEnemy[i].LoadCharacter(party.mEnemy[i],party.mLv[i]+addlv,party.mX[i],party.mY[i],mBattleOption,(i==0));
			i++;
		}
	}
	for(int i=0;i<CHARACTER_NO;i++){
		mCharacter[i]->SetID(i);
	}

	mDMesID=0;
	mMessage.AllDelete();
	mTopEffect.AllDelete();
	mBottomEffect.AllDelete();

	mWindow.InitPlaneWindow(BATTLE_WINDOW_X,BATTLE_WINDOW_Y_UNDER,430,480-BATTLE_WINDOW_Y-10,true);

	ChangeMode(BM_START);
	if(mBattleOption & BOPT_BGM_NO_CHANGE){
		PlayBGM(Flag.mMapBGMNo);
	}else if(mBattleOption & BOPT_BGM_BOSS){
		PlayBGM(16);
	}else if(mBattleOption & BOPT_LAST_BATTLE_NORMAL){
		PlayBGM(18);
	}else if(mBattleOption & BOPT_LAST_BATTLE_HARD){
		PlayBGM(18);
	}else{
		PlayBGM(8);
	}
	CheckBiology();
	
	for(int i=0;i<CHARACTER_NO;i++){
		if(mCharacter[i]->SearchASkill(385)){
			mBottomEffect.SetEffect(BE_HALO,0,0,67+i*0x10000,0);
		}
	}

	if(mBattleOption & BOPT_FIRST_ATTACK){
		for(int i=0;i<MAX_ENEMY_NUMBER;i++){
			int n=i+MAX_PARTY_NUMBER;
			if(mCharacter[n]->GetLive()){
				const int speed=16;
				mTopEffect.SetEffect(BE_SMALL_SHOT,
					mCharacter[n]->GetSX()*BATTLE_SQUARE_SIZE+BATTLE_SQUARE_SIZE/2*mCharacter[n]->GetSize()+BATTLE_SQUARE_X+speed*BM_START_TIME_STONE,
					mCharacter[n]->GetSY()*BATTLE_SQUARE_SIZE+BATTLE_SQUARE_SIZE/2*mCharacter[n]->GetSize()+BATTLE_SQUARE_Y-speed*BM_START_TIME_STONE,
					BE_SPEED(-speed,speed),49,BLEND_STANDARD);

			}
		}
	}
#ifdef _BATTLE_FPS_
	DTest.SetFPS(_BATTLE_FPS_);
#endif
	
//ラスボスH用例外処理
	if(mBattleOption & BOPT_LAST_BATTLE_HARD){
		DTest.LoadTexture(mFieldBG,"data\\battlefield\\fieldbg");
	}
}
int	CBattleMode::SetEnemy(int no,int enemy,int lv,int enemyfield[BATTLE_SQUARE_Height][BATTLE_SQUARE_Height],int param,int xrand)
{
	int size=EnemyBaseData[enemy].mSize;
	if(no>=8)	return size;

	int x=EnemyBaseData[enemy].mLine-pInput->Rand(xrand);
	int y=pInput->Rand(BATTLE_SQUARE_Height);
	if(lv<0)		lv=0;
	if(lv>MAX_LV)	lv=MAX_LV;

	int max=BATTLE_SQUARE_Height-size;

	for(int i=0;i<8;i++){
		if(x<0)		x=0;
		if(x>max)	x=max;
		if(y<0)		y=0;
		if(y>max)	y=max;

		if(!CheckField(x,y,size,enemyfield)){
			for(int i=0;i<size;i++){
				for(int j=0;j<size;j++){
					enemyfield[x+i][y+j]=1;
				}
			}
			mEnemy[no].LoadCharacter(enemy,lv,x,y,param,(no==0));
			if(size>=2)	return 3;
			return 1;
		}
		switch(pInput->Rand(4)){
			case 0:	x++;	break;
			case 1:	x--;	break;
			case 2:	y++;	break;
			case 3:	y--;	break;
		}
	}
	return  0;
}
int	CBattleMode::SetPet(int no,int type,int master,int field[BATTLE_SQUARE_WIDTH][BATTLE_SQUARE_Height],int param,int xrand)
{
	int size=PetData[type].mSize;
	if(no>=MAX_PET_NUMBER)	return 0;

	int x=BATTLE_SQUARE_Height+PetData[type].mLine+pInput->Rand(xrand);
	int y=pInput->Rand(BATTLE_SQUARE_Height);

	int maxy=BATTLE_SQUARE_Height-size;
	int maxx=BATTLE_SQUARE_WIDTH-size;
	int minx=BATTLE_SQUARE_Height+1;

	for(int i=0;i<256;i++){
		if(x<minx)					x=minx;
		if(x>maxx)					x=maxx-1;
		if(y<0)						y=0;
		if(y>maxy)					y=maxy;

		if(!CheckField(x,y,size,field)){
			for(int i=0;i<size;i++){
				for(int j=0;j<size;j++){
					field[x+i][y+j]=1;
				}
			}
			mPet[no].LoadCharacter(type,mCharacter[master]->GetCData(),x,y,param);
			return 1;
		}
		switch(pInput->Rand(4)){
			case 0:	x++;	break;
			case 1:	x--;	break;
			case 2:	y++;	break;
			case 3:	y--;	break;
		}
	}
	return  0;
}


void CBattleMode::OnTimer()
{
	mTimer++;
	mModeTimer++;

	mWindow.OnTimer();
	int i;
	for(i=0;i<CHARACTER_NO;i++){
		mCharacter[i]->OnTimer(mMode,mModeTimer);
	}
	mMessage.OnTimer();
	mTopEffect.AllTimer();
	mBottomEffect.AllTimer();

	if(mColorChangeTimer>=0){
		if(mColorChangeTimer<=0){
			mFieldColor=mColorChangeGoal;
		}else{
			unsigned char* c=(unsigned char*)&mFieldColor;
			unsigned char* goal=(unsigned char*)&mColorChangeGoal;
			for(int i=0;i<4;i++){
				c[i]+=(goal[i]-c[i])/mColorChangeTimer;
			}
		}
		mColorChangeTimer--;
	}


	switch(mMode){
		case BM_START:
			int start;
			if(mBattleOption & BOPT_FIRST_ATTACK)	start=BM_START_TIME_STONE;
			else									start=BM_START_TIME;
			if(mModeTimer>start){
				//投石判定
				if(mBattleOption & BOPT_FIRST_ATTACK){
					mTopEffect.AllDelete();
					for(int i=0;i<MAX_ENEMY_NUMBER;i++){
						int n=i+MAX_PARTY_NUMBER;
						if(mCharacter[n]->GetLive()){
							if(SetStatus(-1,n,26,100)){
								mTopEffect.SetEffect(BE_STANDARD,
									mCharacter[n]->GetCX(),
									mCharacter[n]->GetCY(),
									1,0,BLEND_ADD);
								mCharacter[n]->SetDamageEffect(false);
							}else{
								mCharacter[n]->SetAvoid();
								PlayWave(SE_MISS);
							}

						}
					}
				}

				//敵の先制攻撃かの分岐
				if((mBattleOption&BOPT_ENEMY_FIRST_ATTACK) && mTurn<=0){
					ChangeMode(BM_BATTLE);
				}else{
					ChangeMode(BM_START_MENU);
				}
			}
			break;
		case BM_BATTLE:
			if(mModeTimer==BM_TURN_START_TIME){
				for(int i=0;i<MAX_PARTY_NUMBER;i++){
					if(mMoveCharacter[i]){
						mPlayer[i].Move(mMoveX[i],mMoveY[i],BM_INTARVAL_TIME/2);
						mMoveCharacter[i]=false;
					}
				}
				SetInterval(BM_INTARVAL_RUN_START,1);
			}

			if(mIntervalTimer>=0){
				mIntervalTimer--;
				if(mIntervalTimer<0){
					Interval();
					mIntervalType=0;
					SetInterval(BM_INTARVAL_TIME,0);
				}
			}

			Action();
			break;
		case BM_TURN_END:
			if(mModeTimer>BM_INTARVAL_TIME + BM_INTARVAL_RUN_START){
				AfterTurnEnd();
			}
			break;
		case BM_ESCAPE:

			if(mModeTimer>BM_ESCAPE_TIME){
				ReturnMapInfo.ReturnMap(true);
			}
			break;
		case BM_ESCAPE_FAILED:
			for(i=0;i<MAX_PARTY_NUMBER;i++){
				if(mPlayer[i].GetLive()){
					if(mPlayer[i].GetX()>640-64){
						int time=BM_ESCAPE_FAILED_TIME-BM_ESCAPE_TIME+10;
						mPlayer[i].ChangeAni(8*ANI_TYPE,time,true);
						mPlayer[i].SetGoalSquare(	mPlayer[i].GetSX(),
													mPlayer[i].GetSY(),
													time,
													mPlayer[i].GetSize(),
													true);
						PlayWave(SE_HIT);
					}
				}
			}
			if(mModeTimer>BM_ESCAPE_FAILED_TIME){
				ChangeMode(BM_BATTLE);
			}
			break;
		case BM_WIN:
			Action();
			if(mActionList.size()!=0)	mModeTimer=1;
			if(mModeTimer>BM_WIN_TIME){
				ChangeMode(BM_RESULT);
				//ReturnMapInfo.ReturnMap(false);
			}
			break;
		case BM_RESULT:
			if(mModeTimer>=BM_RESULT_TIME){
				ChangeMode(BM_RESULT_EXP);
			}
			break;
		case BM_LOSE_START:
			Action();
			if(mActionList.size()!=0)	mModeTimer=1;
			if(mModeTimer>=BM_LOSE_START_TIME && mActionList.size()<=0){
				if(mBattleOption & BOPT_LOSE_OK){
					ReturnMapInfo.ReturnMap(true);
				}else{
					ChangeMode(BM_LOSE);
				}
			}
			break;
		case BM_LOSE:
			if(mModeTimer==BM_LOSE_SE_TIME){
				PlayWave(SE_GAME_OVER);
			}
			if(mModeTimer>=BM_LOSE_TIME && !MenuData.GetMenuOpening()){
				ChangeMapInfo.ChangeMap(-2,0,0,0,0);
			}
			break;
	}
	

	bool menu=mBattleMenu.OnTimer();
	if(mMode<BM_BATTLE){
		mWindow.Move(BATTLE_WINDOW_X,BATTLE_WINDOW_Y);
	}else{
		mWindow.Move(BATTLE_WINDOW_X,BATTLE_WINDOW_Y_UNDER);
	}
	if(menu){
		Cursor.Escape();
	}

	//メッセージウインドウ関連
	std::list<CBattleAction*>::iterator				it;
	std::list<CBattleActionNameWindow>::iterator	winit;
	winit = mActionNameList.begin();
	it = mActionList.begin();

	try{
		if(winit == mActionNameList.end()){
			throw 1;
		}
		if(it != mActionList.end() && winit->GetAction()!=(*it)){
			winit->Destroy();
			throw 1;
		}
	}catch(int n){
		if(it != mActionList.end()){
			if((*it)->CheckMessageWindow()){
				CBattleActionNameWindow win(*it);
				win.InitPlaneWindow(200,8,240,40);
				mActionNameList.push_front(win);
			}
		}
	}
	winit = mActionNameList.begin();

	while( winit != mActionNameList.end() ) {
		winit->OnTimer();
		if(!winit->GetLive()){
			winit=mActionNameList.erase(winit);
		}else{
			winit++;
		}
	}
}
void CBattleMode::OnDraw(void)
{
	if((mMode&0xfffffff0)==BM_RESULT || mMode==BM_LOSE){
		D3DCOLOR color=0xffd0d0d0;
		if(mMode==BM_RESULT || mMode==BM_LOSE){
			if(mModeTimer<=0){
				DTest.SetRenderTarget(&gBackGround);
				DTest.Clear();
				DrawBattle();
				DTest.SetRenderTarget(&mBackGround);
				DTest.Clear();
				DrawBattle();
			}
			DTest.SetRenderTarget(&mBackGround);
			for(int i=0;i<2;i++){
				int x,y;
				switch(mMode){
					case BM_RESULT:
						x=pInput->Rand(5)-2;
						y=pInput->Rand(5)-2;
						break;
					case BM_LOSE:
						x=pInput->Rand(9)-4;
						y=pInput->Rand(5)-2;
						break;

				}
				DTest.DrawTexture(	gBackGround,0,
									0,0,640,480,
									x,y,
									640,480,0x40ffffff);
			}
			DTest.SetRenderTarget(&gBackGround);
			DTest.DrawTexture(mBackGround,0,0,0,640,480,0,0,640,480,color);

			int c=(16-mModeTimer)*2;
			if(c<0)	c=0;
			color=color+(c*0x00010000+c*0x00000100+c*0x00000001);
			DTest.SetRenderTarget(NULL);
		}
		DTest.SetBlend(BLEND_NONE);
		DTest.DrawTexture(mBackGround,0,0,0,640,480,0,0,640,480,color);
		DTest.SetBlend(BLEND_STANDARD);
	}else{
		if(mDrawBG){
			DTest.SetRenderTarget(&mBackGround);
			DTest.Clear();
			DrawBattle();
			DTest.SetRenderTarget(NULL);
			mDrawBG=false;
		}
		DrawBattle();
	}

	if(mMode==BM_LOSE && mModeTimer>=BM_LOSE_SE_TIME){
		const int dtime=6;
		int timer=mModeTimer-BM_LOSE_SE_TIME;
		static char name[128];
		sprintf(name,"%s","Game\n  Over...");

		const int		x			=50;
		const int		y			=80;
		const int		textmax		=16;
		if(timer<dtime*textmax){
			int n=((timer)/dtime);
			int color=(timer)%(dtime)*0x0b000000;
			if(color==0)	color=0x00010000;
			name[n+1]='\0';
			DTest.DrawText(7,name,x+1,y+1,color);
			DTest.DrawText(7,name,x,y,color|0x00ffffff);
			color +=0x7f000000;
			name[n]='\0';
			DTest.DrawText(7,name,x+1,y+1,color);
			DTest.DrawText(7,name,x,y,color|0x00ffffff);
			if(n>=1){
				name[n-1]='\0';
				DTest.DrawText(7,name,x+1,y+1,0xff000000);
				DTest.DrawText(7,name,x,y,0xffffffff);
			}
		}else{
			DTest.DrawText(7,name,x+1,y+1,0xff000000);
			DTest.DrawText(7,name,x,y,0xffffffff);
		}

		if(mModeTimer>BM_LOSE_TIME){
			DTest.DrawBox(0,0,640,480,0xff000000);
		}else if(mModeTimer>BM_LOSE_TIME-BM_LOSE_BLACK_TIME){
			int c=(mModeTimer-(BM_LOSE_TIME-BM_LOSE_BLACK_TIME))*0xff/BM_LOSE_BLACK_TIME;
			DTest.DrawBox(0,0,640,480,0x01000000*c);
		}
	}

	mBattleMenu.DrawAll();
}
void CBattleMode::FlashField(int x,int y,D3DCOLOR color)
{
	int dx=BATTLE_SQUARE_X+x*BATTLE_SQUARE_SIZE+32;
	int dy=BATTLE_SQUARE_Y+y*BATTLE_SQUARE_SIZE+64;
	int &f=mFieldFlash[x][y];
	DTest.DrawBox(	dx,dy,
					dx+BATTLE_SQUARE_SIZE,dy+BATTLE_SQUARE_SIZE,
					color/2 & 0xff000000);
	DTest.SetBlend(BLEND_ADD);
	DTest.DrawBox(	dx,dy,
					dx+BATTLE_SQUARE_SIZE,dy+BATTLE_SQUARE_SIZE,
					color);
	DTest.SetBlend(BLEND_STANDARD);
}

void CBattleMode::DrawBattle(void)
{
//ラスボスH用例外処理
	if(mBattleOption & BOPT_LAST_BATTLE_HARD){
		DTest.GetDeveice()->SetSamplerState(0,D3DSAMP_ADDRESSU,D3DTADDRESS_MIRROR);
		DTest.GetDeveice()->SetSamplerState(0,D3DSAMP_ADDRESSV,D3DTADDRESS_MIRROR);
		DTest.DrawTexture(mFieldBG,0,mTimer/4,mTimer/4,320,240,0,0,640,480);
		DTest.GetDeveice()->SetSamplerState(0,D3DSAMP_ADDRESSU,D3DTADDRESS_WRAP);
		DTest.GetDeveice()->SetSamplerState(0,D3DSAMP_ADDRESSV,D3DTADDRESS_WRAP);
	}

	DTest.DrawTexture(mField,0,0,0,320,240,0,0,640,480);
	if(mFieldShakeX || mFieldShakeY){
		DTest.DrawTexture(mField,0,0,0,320,240,mFieldShakeX,mFieldShakeY,640,480);
	}
	DTest.DrawBox(0,0,640,480,mFieldColor);

	std::list<CBattleAction*>::iterator it;
	D3DCOLOR flashcolor;
	if(mActionList.size()>0){
		it = mActionList.begin();
		int time=-1;
		if( it != mActionList.end() ) {
			time=(*it)->GetTimer();
			if(time>=0)	memset(mFieldFlash,0,sizeof(mFieldFlash));
		}
		int t=BM_INTARVAL_TIME -(time+BM_INTARVAL_TIME);
		flashcolor=(t*12)*0x01000000;
	}else{
		flashcolor=(int(sin(mTimer/16.0)*48)+(96))*0x01000000;
	}
	for(int x=0;x<BATTLE_SQUARE_WIDTH;x++){
		for(int y=0;y<BATTLE_SQUARE_Height;y++){
			if(mFieldEffect[x][y]){
				int ani=((mFieldEffect[x][y]&&FE_ANIMATION) >> FE_ANI_SHIFT);
				int tx=ani/8 * 128 + ((mTimer/16)%4)*32;
				int ty=ani%8 * 32;
				int dx=BATTLE_SQUARE_X+x*BATTLE_SQUARE_SIZE+32;
				int dy=BATTLE_SQUARE_Y+y*BATTLE_SQUARE_SIZE+64;
				DTest.DrawTexture(mFieldEffectTex,0,tx,ty,32,32,dx,dy,64,64,0xffffffff);

			}
			if(mFieldFlash[x][y]){
				int &f=mFieldFlash[x][y];
				D3DCOLOR c;
				if(f<=100){
					c=(((0xff00*f)/100)&0xff00) + (((0xff*(100-f))/100)&0xff);
					c=c<<8;
				}else{
					c=0xff0000ff;
				}
				FlashField(x,y,flashcolor+c);
			}
		}
	}


	if(mTargetLineFrom>=0){
		CBattleCharacter* from=mCharacter[mTargetLineFrom];
		CBattleCharacter* to=mCharacter[mTargetLineTo];
		int fx=from->GetSX();
		int fy=from->GetSY();
		int fs=from->GetSize();
		int tx=to->GetSX();
		int ty=to->GetSY();
		int ts=to->GetSize();
		DTest.DrawLine(	BATTLE_SQUARE_X+fx*BATTLE_SQUARE_SIZE+fs*32+32,
						BATTLE_SQUARE_Y+fy*BATTLE_SQUARE_SIZE+fs*32+64,
						BATTLE_SQUARE_X+tx*BATTLE_SQUARE_SIZE+ts*32+32,
						BATTLE_SQUARE_Y+ty*BATTLE_SQUARE_SIZE+ts*32+64,
						LINE_WIDTH,0xff8080ff);
	}

	int i;
	for(i=0;i<CHARACTER_NO;i++){
		mCharacter[i]->DrawShadow();
	}

	CBattleCharacter*	drawcharacter[CHARACTER_NO];
	int					drawz[CHARACTER_NO];
	for(i=0;i<CHARACTER_NO;i++){
		drawcharacter[i]=mCharacter[i];
		drawz[i]=drawcharacter[i]->GetY()+drawcharacter[i]->GetSize()*32;
	}

	it = mActionList.begin();
	while( it != mActionList.end() ) {
		int ac=(*it)->GetActiveCharacter();
		drawz[ac]++;
		it++;
	}

	bool turn=true;
	while(turn){
		turn=false;
		for(int n=1;n<CHARACTER_NO;n++){
			if(drawz[n] < drawz[n-1]){
				turn=true;
				CBattleCharacter* c	=drawcharacter[n];
				drawcharacter[n]	=drawcharacter[n-1];
				drawcharacter[n-1]	=c;
				int z		=drawz[n];
				drawz[n]	=drawz[n-1];
				drawz[n-1]	=z;
			}
		}
	}

	mBottomEffect.AllDraw();

	for(i=0;i<CHARACTER_NO;i++){
		drawcharacter[i]->OnDraw();
		drawcharacter[i]->DrawStatus(0xffffffff);
		if(CheckField(drawcharacter[i],mFieldFlash)){
			DTest.SetBlend(BLEND_SOLID_ADD);
			drawcharacter[i]->OnDraw((flashcolor/2)|0xffffff,false);
			DTest.SetBlend(BLEND_STANDARD);
		}
	}

	mTopEffect.AllDraw();
	static int messagefield[BATTLE_SQUARE_WIDTH][BATTLE_SQUARE_Height];
	memset(messagefield,0,sizeof(messagefield));
	mMessage.OnDraw(messagefield);

	//if(/*(mMode==BM_INTARVAL && mModeTimer>10)
	//	|| */(mMode==BM_ACTION)){

	std::list<CBattleActionNameWindow>::iterator	winit;
	winit = mActionNameList.end();
	while( winit != mActionNameList.begin() ) {
		winit--;
		winit->OnDraw();
	}
				//mActionNameWindow.InitPlaneWindow(200,8,240,40,true);
				//mActionNameWindow.OnDraw();
				//mActionNameWindow.DrawText(6,ComSkillData[act].GetName(),-8,-8,0,DT_CENTER);
	//}

	mWindow.OnDraw();

	static const int HP_Y=25;
	static const int SP_Y=70;
	static const int PARSON_WIDTH=90;
	mWindow.DrawText(0,"Name",0,0);
	mWindow.DrawText(0,"HP",0,HP_Y);
	mWindow.DrawText(0,"SP",0,SP_Y-15);

	for(i=0;i<MAX_PARTY_NUMBER;i++){
		if(!mPlayer[i].GetLive())	continue;
		int drawx=(MAX_PARTY_NUMBER-i)*PARSON_WIDTH-80;
		mWindow.DrawText(0,mPlayer[i].GetName(),drawx,0,0,DT_RIGHT);

		static char text[64];
		int HP_X=drawx;
		int SP_X=drawx;

		int hp=mPlayer[i].GetHP();
		int lp=mPlayer[i].GetMaxHP();
		int mlp=mPlayer[i].GetMaxLP();

		int barx=mWindow.GetWidth()-drawx-PARSON_WIDTH-12;
		const int BAR_Y	=	HP_Y+14;
		const int BAR_W	=	PARSON_WIDTH-10;
		const int BAR_H	=	3;
		mWindow.DrawBox(barx,BAR_Y,BAR_W,BAR_H,0xff404040);
		mWindow.DrawBox(barx,BAR_Y,BAR_W*lp/mlp,BAR_H,0xffff0000);
		int barhp=hp;
		if(barhp>lp)	barhp=lp;
		mWindow.DrawBox(barx,BAR_Y,BAR_W*barhp/mlp,BAR_H,0xff00d000);

		sprintf(text,"%d",hp);
		mWindow.DrawText(8,text,HP_X+45,HP_Y,0,DT_RIGHT);
		mWindow.DrawText(8,"/",HP_X+35,HP_Y,0,DT_RIGHT);
		sprintf(text,"%d",lp);
		mWindow.DrawText(8,text,HP_X,HP_Y,0,DT_RIGHT);

		int j=MAX_CSKILL_LEVEL-1;
		while(1){
			int y=SP_Y-(j/2)*18;
			int x=(j/2)*20+5;
			sprintf(text,"%d",mPlayer[i].GetSP(j));
			mWindow.DrawText(8,text,SP_X+j*30-x,y,0,DT_RIGHT);
			j--;
			if(j<0)	break;
			mWindow.DrawText(8,"/",SP_X+j*30+20-x,y,0,DT_RIGHT);
		}
		int st=mPlayer[i].GetDrawStatus();
		if(st>=0){
			mWindow.DrawText(2,StatusData[st].GetName(),drawx-4,SP_Y+24,0,DT_RIGHT);
		}
	}
	
}

int		CBattleMode::GetPossibleTarget(CBattleCharacter* character,int *target,int* possible)
{
	int skill=character->GetAction();
	
	int no=0;
	if(ComSkillData[skill].mRange==CR_NONE || ComSkillData[skill].mRange==CR_SELF){
		bool enemy=character->Enemy();
		if(ComSkillData[skill].mTarget)	enemy=!enemy;
		if(!enemy){
			target[0]=0;
		}else{
			target[0]=MAX_PARTY_NUMBER;
		}
		no=1;
		if(possible){
			*possible=1;
		}
	}else{
		no=GetPossibleTarget(character->mID,skill,target,possible);
	}
	return no;
}
int		CBattleMode::GetCharacterNo(CBattleCharacter* character)
{
	for(int i=0;i<CHARACTER_NO;i++){
		if(character==mCharacter[i])	return i;
	}
	return -1;
}


bool	CBattleMode::SetCharacterMove(int x,int y,int character)
{
	for(int i=0;i<MAX_PARTY_NUMBER;i++){
		if(	i!=character
			&& mPlayer[i].GetLive()
			&& mPlayer[i].GetSX()==x
			&& mPlayer[i].GetSY()==y){
			return false;
		}

		if(	mMoveCharacter[i]
			&& mMoveX[i]==x
			&& mMoveY[i]==y){
			return false;
		}
	}
	mMoveCharacter[character]=true;
	mMoveX[character]=x;
	mMoveY[character]=y;
	return true;
}
void	CBattleMode::ClearCharacterMove(int character)
{
	mMoveCharacter[character]=false;
}

void	CBattleMode::ChangeMode(int mode)
{
	mMode=mode;
	mModeTimer=0;
	int i;
	switch(mMode){
		case BM_START_MENU:
			mBattleMenu.AllDelete();
			mBattleMenu.SetMenu(BMenuTurnStart,mBattleOption);
			break;
		case BM_CHECK_MENU:
			mBattleMenu.SetMenu(BMenuCheck,0);
			break;

		case BM_BATTLE:
			TurnStart();
			break;
		case BM_TURN_END:
			TurnEnd();
			break;
		case BM_ESCAPE:
			for(i=0;i<MAX_PARTY_NUMBER;i++){
				if(mPlayer[i].GetActive()){
						mPlayer[i].SetGoalSquare(	mPlayer[i].GetSX()+5,
													mPlayer[i].GetSY(),
													BM_ESCAPE_TIME,
													mPlayer[i].GetSize(),
													false);
				}
			}
			if(1){	//逃走判定
				int fail;
				if(mBattleOption&BOPT_ESCAPE_OK){
					fail=0;
				}else{
					double plv=0;
					double elv=0;
					int pno=0;
					int eno=0;
					int max_plv=0;
					int max_elv=0;
					bool escapeup=false;
					for(i=0;i<MAX_PARTY_NUMBER;i++){
						if(mPlayer[i].GetCData() && mPlayer[i].GetLive()){
							pno++;
							int lv =mPlayer[i].GetLv();
							plv+=lv;
							if(max_plv<lv)	max_plv=lv;
							if(mPlayer[i].SearchASkill(272)){
								escapeup=true;
							}
						}
					}
					for(i=0;i<MAX_ENEMY_NUMBER;i++){
						if(mEnemy[i].GetCData() && !(mEnemy[i].GetDead())){
							int no=mEnemy[i].GetSize()*3-2;
							eno+=no;
							
							int lv =mEnemy[i].GetLv();
							elv+=lv*no;
							if(max_elv<lv)	max_elv=lv;
						}
					}
					plv/=pno;
					plv*=(pno+6);
					elv/=eno;
					elv*=(eno+6);
					fail=int(30-plv*0.4+elv*0.42);
					if(fail>100)			fail=100;
					fail-=mEscapeCount*20;
					if(fail<0)					fail=0;
					if(Flag.mHardMode>0)		fail=100-((100-fail)/2);
					if(max_plv-15 >= max_elv)	fail-=25;
					if(escapeup)				fail/=2;
				}
				mEscapeCount++;
			#ifdef _ESCAPE_
				fail=0;
			#endif
				if(mBattleOption & BOPT_NO_ESCAPE)	fail=1000;
				if(pInput->Rand(100)<fail){
					mMode=BM_ESCAPE_FAILED;
				}else{
					OFF_BGM();
				}
			}
			break;
		case BM_RESULT_EXP:
			if(mBattleOption & BOPT_END_BGM_OFF){
				Flag.mMapBGMNo=0;
				SetBGMFadeOut(20);
			}else{
				PlayBGM(Flag.mMapBGMNo);
			}

			if(mBattleOption & BOPT_NO_RESULT){
				ReturnMapInfo.ReturnMap(false);
			}else{
				mBattleMenu.SetMenu(BMenuResultExp,0);
			}
			mFieldBG.Release();
			break;
		case BM_LOSE_START:
			break;
	}
	if(mMode & BM_PERSONAL_MENU){
		int player=((mMode-BM_PERSONAL_MENU)%BM_PM_BACK)/BM_PM_NUMBER;
		int add=1;
		if(mMode & BM_PM_BACK){
			player--;
			add=-1;
		}
		while(1){
			if(player<0){
				ChangeMode(BM_START_MENU);	
				return;
			}else if(player>=MAX_PARTY_NUMBER){
				PlayWave(SE_YES);
				ChangeMode(BM_BATTLE);	
				return;
			}
			if(mPlayer[player].GetActive()){
				if(mPlayer[player].SearchStatus(2)>0 || mPlayer[player].SearchStatus(5)>0){
					SetRandomAction(player);
				}else{
					break;
				}
			}
			player+=add;
		}

		switch((mMode-BM_PERSONAL_MENU)%BM_PM_NUMBER){
			case BM_PM_START:
				mBattleMenu.AllDelete(1);
				mBattleMenu.SetMenu(BMenuPersonal,player);
				break;

			case BM_PM_SKILL:
			case BM_PM_MAGIC:
			case BM_PM_SPECIAL:
				mBattleMenu.SetMenu(BMenuSelectSkill,(mMode-BM_PERSONAL_MENU)%BM_PM_NUMBER+player*BM_PM_NUMBER);
				break;

			case BM_PM_ITEM:
				mBattleMenu.SetMenu(BMenuUseItem,player);
				break;

			case BM_PM_TARGET:
				mBattleMenu.SetMenu(BMenuSetTarget,player);
				break;

			case BM_PM_MOVE:
				mBattleMenu.SetMenu(BMenuMoveCharacter,player);
				break;
		}
	}
}
void	CBattleMode::CheckBiology(void)
{
	Flag.mBiology=false;
	for(int i=0;i<MAX_PARTY_NUMBER;i++){
		if(!mPlayer[i].GetActive())	continue;
		if(mPlayer[i].SearchASkill(291)){
			Flag.mBiology=true;
		}
	}
}

void	CBattleMode::SetInterval(int time,int type)
{
	if(mIntervalTimer<time)	mIntervalTimer=time;
	if(mIntervalType<type)	mIntervalType=type;
}

void	CBattleMode::TurnStart(void)
{
	mBattleMenu.AllDelete();
	int i;

	for(i=0;i<MAX_ENEMY_NUMBER;i++){
		mEnemy[i].SetActionAuto();
	}
	for(i=0;i<CHARACTER_NO;i++){
		mCharacter[i]->TurnStart();
	}
	for(i=0;i<MAX_PET_NUMBER;i++){
		SetRandomAction(i+PET);
	}
/*	if(mTurn<=0 && mBattleOption & BOPT_FIRST_ATTACK){	//先制攻撃時
		for(i=0;i<MAX_ENEMY_NUMBER;i++){
			mEnemy[i].NoActionCount();
		}
	}
*/
}
bool	CBattleMode::AfterAction()
{
	int i;

	bool act[CHARACTER_NO];
	memset(act,0,sizeof(act));

	//行動中のキャラクターを除外
	std::list<CBattleAction*>::iterator it;
	it = mActionList.begin();
	while( it != mActionList.end()){
		int actac=(*it)->GetActiveCharacter();
		act[actac]=true;
		int tgno,*acttg;
		acttg=(*it)->GetTargetLock(&tgno);
		for(int i=0;i<tgno;i++){
			act[acttg[i]&0xffff]=true;
		}
		it++;
	}
	
	for(i=0;i<CHARACTER_NO;i++){
		if(act[i])	continue;

	//フェニックス復活
		if(mCharacter[i]->GetDead()){
			if(mCharacter[i]->SearchStatus(22)>=0){
				mCharacter[i]->ClearStatus(22);
				HealHP(-1,i,mCharacter[i]->GetMaxHP(),mDMesID,true);
			}
		}
	//魅了、暴走解除による行動キャンセル
		if(mCharacter[i]->GetCharm()){
			if(mCharacter[i]->SearchStatus(2)<=0){
				CancelAction(i);
			}
		}
		if(mCharacter[i]->GetBerserk()){
			if(mCharacter[i]->SearchStatus(5)<=0){
				CancelAction(i);
			}
		}
	//ペットの消滅
		if(i>=PET){
			if(mCharacter[i]->GetDead()){
				mCharacter[i]->DeleteCharacter();
			}
		}
	}

	//死んだ敵を黒くする
	for(i=0;i<MAX_ENEMY_NUMBER;i++){
		if(mEnemy[i].GetDead() && mEnemy[i].GetDeathBlack()<=0){
			mEnemy[i].SetDeathBlack(1);
		}
	}

	//戦闘終了確認
	if(CheckBattleEnd()){
		OFF_BGM();
		return true;
	}

	return false;
}
void	CBattleMode::Interval()
{
	int acharacter=-1;
	int i;

	if(!Flag.mBattleSpeed && mIntervalType==0)	return;
	ChangeFieldColor(BattleFieldColor[mMagicField],16);
	if(AfterAction())	return;

	//敵のターゲット指定
	for(i=0;i<MAX_ENEMY_NUMBER;i++){
		int act=mEnemy[i].GetAction();
		if(act>=0)	mEnemy[i].SetTargetAuto();
	}
	unsigned int bestwt=0xffffffff;

	//アクティブキャラクターの取得、他
	for(i=0;i<CHARACTER_NO;i++){
		int param=mCharacter[i]->SearchStatus(21);
		if(param>0){
			mCharacter[i]->SetTarget(param-1);
		}
		if(mCharacter[i]->GetAction()>=0){		
			int target=mCharacter[i]->GetTarget();
			if((target<0) || (CHARACTER_NO<target))	continue;
			unsigned int wt=mCharacter[i]->SetWaitTime(mCharacter[target]);
			if(wt<bestwt){
				bestwt=wt;
				acharacter=i;
			}
		}
	}

	if(acharacter<0){
		if(mActionList.size()<=0)	ChangeMode(BM_TURN_END);
		return;
	}

	//まやかしスキル所持者の消去
	bool active[CHARACTER_NO];
	memset(active,0,sizeof(active));
	std::list<CBattleAction*>::iterator it;
	it = mActionList.begin();
	while( it != mActionList.end()){
		int actac=(*it)->GetActiveCharacter();
		active[actac]=true;
		it++;
	}
	for(i=0;i<CHARACTER_NO;i++){
		if(mCharacter[i]->GetAction()<0 &&(!active[i])){
			if(mCharacter[i]->SearchASkill(306)){
				mCharacter[i]->DeleteCharacter();
			}
		}
	}
	//行動取得、ターゲットの再指定
	int act=mCharacter[acharacter]->GetAction();
	int target=mCharacter[acharacter]->GetTarget();
	if(act<0)		return;

	if(mCharacter[target]->GetDead() && !(ComSkillData[act].mType&CS_RIVIVAL)){
		SetRandomTarget(acharacter,act);
	}
	target=mCharacter[acharacter]->GetTarget();
	if(ComSkillData[act].mType&CS_MOVE){
		//移動型の衝突検知
		target=CheckTarget(	mCharacter[acharacter],
						mCharacter[acharacter]->GetTarget(),
						act);
	}

	//すでに行動しているキャラクターが衝突しないような検知部
	int TargetNo ;
	int t[CHARACTER_NO];
	bool locked[CHARACTER_NO];
	memset(locked,0,sizeof(locked));
	memcpy(t,CBattleAction::GetTargetLock(&TargetNo,act,acharacter),sizeof(t));
	bool error=false;

	it = mActionList.begin();

	for(int i=0;i<TargetNo;i++){
		if(mCharacter[(t[i]&0xffff)]->GetDead() && (mCharacter[(t[i]&0xffff)]->SearchStatus(22)>0)){
			//死亡かつフェニックス状態の対象が居る場合はエラー
			error=true;
		}
	}
	while( it != mActionList.end()){
		int actac=(*it)->GetActiveCharacter();
		locked[actac]=true;
		if(actac==acharacter)	error=true;
		for(int i=0;i<TargetNo;i++){
			if((t[i]&0xffff)==actac)			error=true;
		}
		int tgno,*acttg;
		acttg=(*it)->GetTargetLock(&tgno);
		for(int i=0;i<tgno;i++){
			locked[acttg[i]&0xffff]=true;
			if((acttg[i]&0xffff)==acharacter)	error=true;
			if(!(*it)->CheckTargetLock())	continue;
			for(int j=0;j<TargetNo;j++){
				if((t[j]&0xffff)==(acttg[i]&0xffff)){
					error=true;
				}
			}
		}
		it++;
	}

	//手が空いているものは、本来の位置に移動
	for(int i=0;i<CHARACTER_NO;i++){
		if(locked[i])	continue;
		GetCharacter(i)->Return(BM_INTARVAL_TIME/2,false,true);
	}
	if(error)		return;
	if(TargetNo<0)	return;

	//行動リストに行動を登録
	mCharacter[acharacter]->SetTarget(target);
	SetActionList(act,acharacter);


	//行動による予備動作の実行
	if(ComSkillData[act].mType&CS_MOVE){
		//移動型の予備動作
		if(target!=acharacter && !(ComSkillData[act].mType&CS_NO_WALK)){
			int x=mCharacter[target]->GetSX();
			int y=mCharacter[target]->GetSY();
			int s=mCharacter[target]->GetSize();
			mCharacter[acharacter]->ChangeAni(-1,0,true);
			mCharacter[acharacter]->SetGoalSquare(x,y,BM_INTARVAL_TIME,s,false);
		}else{
			mCharacter[acharacter]->ChangeAni(1*ANI_TYPE,BM_INTARVAL_TIME,true);
		}
	}else{
		if(ComSkillData[act].mType & CS_ITEM){
		//アイテム型の予備動作
			mCharacter[acharacter]->ChangeAni(10*ANI_TYPE+2,BM_INTARVAL_TIME,true);
			//int s=mCharacter[target]->GetSize();
			int x=mCharacter[target]->GetBaseCX();
			int y=mCharacter[target]->GetBaseCY();
			int ui;
			if(ComSkillData[act].mType & CS_ENEMY_ITEM){
				ui=((ComSkillData[act].mType & 0xff000) >> 12);
				ui+=256;
			}else{
				ui=mCharacter[acharacter]->GetUseItem();
			}
			mTopEffect.SetEffect(BE_ITEM,
							mCharacter[acharacter]->GetCX(),
							mCharacter[acharacter]->GetCY(),
							x+y*0x10000,ui);
			if((mCharacter[target]->GetSX() > mCharacter[acharacter]->GetSX()) && acharacter<4){
				mCharacter[acharacter]->SetTurn(true);
			}
		}else if(ComSkillData[act].mType & CS_MAGIC){
		//魔法型の予備動作
			mCharacter[acharacter]->ChangeAni(13*ANI_TYPE,BM_INTARVAL_TIME+12,true);
			PlayWave(SE_MAGIC);
			int element=(ComSkillData[act].mType &0xf000)  / 0x1000;
			static const D3DCOLOR color[5]={0xff00ffff,0xffff0000,0xff0000ff,0xff00ff00,0xffffff00};
			mTopEffect.SetEffect(BE_MAGIC_START,
							mCharacter[acharacter]->GetCX(),
							mCharacter[acharacter]->GetCY(),
							color[element%5],0);
		}else{
		//それ以外の予備動作
			mCharacter[acharacter]->ChangeAni(1*ANI_TYPE,BM_INTARVAL_TIME,true);
		}
	}
	
	//攻撃対象を光らせる
	GetTargetArea(	acharacter,
					mCharacter[acharacter]->GetTarget(),
					act,
					mFieldFlash);

	return;
}
bool	CBattleMode::CheckBattleEnd(void)
{
	int i;

	bool end=true;
	for(i=0;i<MAX_PARTY_NUMBER;i++){
		//if(!mPlayer[i].GetDead()){
		if((!mPlayer[i].GetDead()) || (mPlayer[i].SearchStatus(22)>0)){
			end=false;
		}else{
			mPlayer[i].SetAction(-1);
		}
	}
	if(end){
		ChangeMode(BM_LOSE_START);
		return true;
	}

	end=true;
	for(i=0;i<MAX_ENEMY_NUMBER;i++){
		if((!mEnemy[i].GetDead()) || (mEnemy[i].SearchStatus(22)>0)){
			bool option=false;
			if(mEnemy[i].SearchASkillType(AS_OPTION,true)>0)	option=true;
			if(i>0 && (mBattleOption & BOPT_KILL_LEADER))		option=true;

			if(!option){
				end=false;
			}
		}else{
			mEnemy[i].SetAction(-1);
		}
	}
	if(end){
		ChangeMode(BM_WIN);
		return true;
	}

	return false;
}
void	CBattleMode::EndAction(void)
{
}
void	CBattleMode::SetTotalMessage(int id)
{
	int		total[CHARACTER_NO][NMES_TYPEMAX];
	int		totaldmg[CHARACTER_NO][NMES_TYPEMAX];
	int		colortype[CHARACTER_NO][NMES_TYPEMAX];

	CBattleMessage* mes=&mMessage;
	memset(total		,0,sizeof(total));
	memset(totaldmg		,0,sizeof(totaldmg));
	memset(colortype	,-1,sizeof(colortype));

	while(mes){
		CNumberMessage* dmes=dynamic_cast<CNumberMessage*>(mes);
		if(dmes){
			if(dmes->mID == id){
				total[dmes->mTarget][dmes->mType]++;
			}
		}
		mes=mes->GetNext();
	}

	mes=&mMessage;
	while(mes){
		CNumberMessage* dmes = dynamic_cast<CNumberMessage*> (mes);
		if(dmes){
			if((total[dmes->mTarget][dmes->mType]>=2) && (dmes->mID == id)){
				totaldmg[dmes->mTarget][dmes->mType]+=dmes->mDmg;
				if(colortype[dmes->mTarget][dmes->mType]<0){
					colortype[dmes->mTarget][dmes->mType]=dmes->mColor;
				}else if (colortype[dmes->mTarget][dmes->mType]!=dmes->mColor){
					colortype[dmes->mTarget][dmes->mType]=0;
				}
				dmes->Kill();
			}
		}
		mes=mes->GetNext();
	}
	for(int i=0;i<CHARACTER_NO;i++){
		for(int j=0;j<NMES_TYPEMAX;j++){
			if(total[i][j]>=2){
				char text[128];
				sprintf(text,"{|%d",totaldmg[i][j]);
				SetMessage(i,text,CNumberMessage::GetColor1(colortype[i][j],j),CNumberMessage::GetColor2(colortype[i][j],j));
			}
		}
	}
}
void	CBattleMode::CancelAction(int target)
{
	if(mCharacter[target]->GetAction()==204)		return;//夢の卵専用処理

	int count=mCharacter[target]->AddActionCount();
	if(count>=mCharacter[target]->GetActionNo()){
		mCharacter[target]->SetAction(-1);
		mCharacter[target]->ClearStatus(11);
	}else{
		mCharacter[target]->SetActionAuto();
	}
	if(mCharacter[target]->SearchASkill(306)){
		mCharacter[target]->DeleteCharacter();
	}

}
void	CBattleMode::TurnEnd(void)
{
	bool end=true;
	for(int i=0;i<CHARACTER_NO;i++){
		if(!mCharacter[i]->TurnEnd(this)){
			end=false;
		}
	}

	if(end){
		AfterTurnEnd();
	}
}
void	CBattleMode::AfterTurnEnd(void)
{
	for(int i=0;i<CHARACTER_NO;i++){
		if(!mCharacter[i]->GetLive())	continue;
		mCharacter[i]->Return(BM_INTARVAL_TIME/2,false,true);

		//元気印
		if(mCharacter[i]->SearchASkill(316)){
			if(pInput->Rand(100)<50){
				CureStatus(-1,i);
			}
		}
		//毒の効果
		if(mCharacter[i]->SearchStatus(0)>0){
			Attack(-1,i,mCharacter[i]->GetMaxLP()/5,0,AT_TYPE_CONST,AT_ELE_NONE,0,SE_POISON,100,NULL);
		}
		//フィールドダメージ
		int fe=GetFieldEffect(mCharacter[i]->GetSX(),mCharacter[i]->GetSY());
		if(fe && !mCharacter[i]->CheckAir()){
			int type;
			int element;
			int se;
			switch(fe & FE_TYPE){
				case FE_TYPE_MAGIC:		type=AT_MAGIC | AT_ATK_CONST | AT_DEF_MAGIC | AT_LONG_LANGE;	break;
			}
			switch(fe & FE_ELEMENT){
				case FE_ELE_FIRE:		element=AT_ELE_FIRE;	se=SE_FIRE;	break;
			}
			Attack(-1,i,fe&0xffff,0,type,element,0,se,100,NULL);
		}
		//腐敗の効果
		if(mCharacter[i]->SearchStatus(6)>0){
			LostLP(-1,i,mCharacter[i]->GetMaxHP()/10,mDMesID);
		}
		//ＨＰ自動回復
		if(mCharacter[i]->SearchASkill(251) || mCharacter[i]->SearchStatus(12)>0){
			HealHP(-1,i,int(mCharacter[i]->GetMaxHP()*0.2),mDMesID);
		}

		//眠り,魅了,麻痺、封印、暴走解除
		for(int j=1;j<=5;j++){
			int param=mCharacter[i]->SearchStatus(j);
			if(param>0){
				if(pInput->Rand(param)<1){
					mCharacter[i]->ClearStatus(j);
				}else{
					mCharacter[i]->SetStatus(j,param-mCharacter[i]->GetSize());
				}
			}
		}
		//ブースター、ラピッド、防御、挑発、必殺、気絶解除
		mCharacter[i]->ClearStatus(10);
		mCharacter[i]->ClearStatus(11);
		mCharacter[i]->ClearStatus(15);
		mCharacter[i]->ClearStatus(21);
		mCharacter[i]->ClearStatus(24);
		mCharacter[i]->ClearStatus(26);

		//モール解除
		if(mCharacter[i]->SearchStatus(14)>0){
			mCharacter[i]->ClearStatus(14);
			mCharacter[i]->SetSpeedZ(8*BATTLE_GRAVITY);
		}
		//重力解除
		int param=mCharacter[i]->SearchStatus(9);
		if(param>0){
			mCharacter[i]->SetStatus(9,param-1);
			if(param<3){
				if(pInput->Rand(2)==0){
					mCharacter[i]->ClearStatus(9);
				}
			}

		}
		//ためる解除
		if(mCharacter[i]->SearchStatus(8)<=1){
			mCharacter[i]->ClearStatus(8);
		}
		//分身解除
		param=mCharacter[i]->SearchStatus(19);
		if(param>0){
			mCharacter[i]->SetStatus(19,param-1);
		}
		//オートヒール解除
		param=mCharacter[i]->SearchStatus(12);
		if(param>0){
			mCharacter[i]->SetStatus(12,param-1);
		}
		//ターゲット解除
		param=mCharacter[i]->SearchStatus(25);
		if(param>0){
			mCharacter[i]->SetStatus(25,param-1);
		}
		//アンデッド復活
		if(mCharacter[i]->SearchStatus(23)>0){
			int lp=int(mCharacter[i]->GetMaxHP()*100 / mCharacter[i]->GetMaxLP()*0.3);
			if(mCharacter[i]->SearchASkill(393))	lp*=3;
			if(pInput->Rand(100)<lp){
				HealHP(-1,i,mCharacter[i]->GetMaxHP(),mDMesID,true);
			}
		}
	}

	for(int x=0;x<BATTLE_SQUARE_WIDTH;x++){
		for(int y=0;y<BATTLE_SQUARE_Height;y++){
			int& fe=mFieldEffect[x][y];
			if(fe){
				int t=((fe&FE_TIMER) >> FE_TIMER_SHIFT);
				t--;
				if(t<=0){
					fe = 0;
				}else{
					fe = (fe&FE_TIMER_MASK) | t<<FE_TIMER_SHIFT;
				}
				
			}
		}
	}
	if(AfterAction()){
		return ;
	}

	mMagicField=FIELD_NORMAL;
	ChangeFieldColor(BattleFieldColor[mMagicField],16);

	CheckBiology();

	if((mBattleOption & BOPT_2_TURN) && mTurn>=1){
		ReturnMapInfo.ReturnMap(true);
		return;
	}

	ChangeMode(BM_START_MENU);
	mTurn++;
	mDMesID++;
}
void	CBattleMode::SetMessage(int target,const char* text,D3DCOLOR color1,D3DCOLOR color2)
{
	int x=mCharacter[target]->GetX();
	int y=mCharacter[target]->GetY()+(mCharacter[target]->GetSize()-1)*BATTLE_SQUARE_SIZE;

	CBattleMessage** end;
	end=mMessage.SearchEnd();
	*end = new CBattleMessage;
	(*end)->Init(x,y,text,color1,color2);
}
void	CBattleMode::SetNumberMessage(int target,int dmg,int type,const char* text,D3DCOLOR color,bool crt,int id)
{
	int x=mCharacter[target]->GetX();
	int y=mCharacter[target]->GetY()+(mCharacter[target]->GetSize()-1)*BATTLE_SQUARE_SIZE;

	CNumberMessage* dmes;
	dmes=new CNumberMessage;
	dmes->Init(x,y,dmg,type,id,target,text,color,crt);

	CBattleMessage** end;
	end=mMessage.SearchEnd();
	*end = dmes;
}

int*	CBattleMode::Attack(int player,int* target,int targetno,int power,int minatk,int type,int element,int option,int sound,CBattleAction* act)
{
	static int	AllDamage[CHARACTER_NO];
	
	ChargeBreak(player);	//チャージ消去

	if(targetno<=0){	//ターゲットが居ない場合、ミスした効果音を流して終了
		if( type & AT_AVOID){
			PlayWave(SE_MISS);
		}else{
			PlayWave(sound);
		}
		return 0;
	}
						//対象全体に攻撃を行う
	for(int i=0;i<targetno;i++){
		AllDamage[i]=Attack(player,
							target[i]&0xffff,
							power,minatk,
							type,element,option,sound,
							target[i]/0x10000,act);
	}
	return AllDamage;
}


void CBattleMode::ChargeBreak(int player)
{
	if(player<0)	return;
	if(mCharacter[player]->SearchStatus(8)>0){
		mCharacter[player]->SetStatus(8,1);	
	}
}
int	CBattleMode::Attack(int player,int target,int power,int minatk,int type,int element,int option,int sound,int ratio,CBattleAction* act )
{
//この関数を呼び出す際は、必ず　ChargeBreak　すること！
	if(target<0)	return -1;
//// 基本情報 ////////////////---------------------------------------------------
	int dmesid;
	if(act){
		dmesid=act->GetDMesID();
	}else{
		dmesid=mDMesID;
	}
	int mainele = element;
	bool air=mCharacter[target]->CheckAir();
	bool life=!mCharacter[target]->GetDead();

//// 事前処理 ////////////////------------------------------------------------------
//デバッグ用オプション
#ifdef _POOR_MAN_
		power=0;
#endif
#ifdef _REFLECT_
	if(player>=0){
		target=player;
	}
#endif

	if(player>=0){
//分身わかめ
		if(mCharacter[player]->SearchStatus(19)>0 && !(option&AT_OPT_NO_DOUBLE)){
			Attack(player,target,power,minatk,type,element,option | AT_OPT_NO_DOUBLE,sound,int(ratio*0.3),act);
		}
//技魔法反転
		if(mCharacter[player]->SearchASkill(354) && !(option&AT_OPT_NO_TWIN_HIT)){
			if(type & AT_SKILL){
				type=type & (~AT_SKILL) & (~AT_DEF_WEAPON) & (~AT_AVOID) & (~AT_CRITICUL);
				type=type | AT_MAGIC | AT_DEF_MAGIC;
			}else if(type & AT_MAGIC){
				type=type & (~AT_MAGIC) & (~AT_DEF_MAGIC) ;
				type=type | AT_SKILL | AT_DEF_WEAPON | AT_AVOID | AT_CRITICUL;
			}
		}
//チャージ補正
		if((mCharacter[player]->SearchStatus(8)>0) && !(option&AT_OPT_NO_TWIN_HIT)){
			power*=2;
		}
//対空攻撃		
		if((type & AT_SKILL) && mCharacter[player]->SearchASkill(337)){
			option	=option | AT_OPT_ANTI_AIR;
		}
//対不死攻撃	
		if((type & AT_SKILL) && mCharacter[player]->SearchASkill(368)){
			option	=option | AT_OPT_ANTI_UNDEAD;
		}
//マジッククリティカル
		if((type & AT_MAGIC) && mCharacter[player]->SearchASkill(348)){
			type=type | AT_CRITICUL;
		}
	}

//ダブルヒット
	if(player>=0 && !(option&AT_OPT_NO_TWIN_HIT) && (type & AT_SKILL)){
		if(mCharacter[player]->SearchASkill(304)){
			Attack(player,target,power,minatk,type,element,option|AT_OPT_NO_TWIN_HIT,sound,ratio,act);
		}
	}

//飛行に無効
	if(air && (option&AT_OPT_NO_AIR)){
		return -1;
	}
//実行者の戦闘不能によるキャンセル
	if(player>0 && mCharacter[player]->GetDead()){
		return -1;
	}

//マジックリフレクト
	if((type & AT_MAGIC) && !(option & AT_OPT_NO_REFLECT) && !(mCharacter[target]->GetDead()) && (player>=0)){
		bool reflect=false;
		if(mCharacter[target]->SearchASkill(350)){
			reflect=true;
		}
		int st=mCharacter[target]->SearchStatus(20);
		if(st>0){
			mCharacter[target]->SetStatus(20,st-1);
			reflect=true;
		}
		if(reflect){
			int turn;
			if(mCharacter[target]->Enemy())	turn=TURN_HORIZONTAL;
			else	turn=0;

			mTopEffect.SetEffect(
				BE_STANDARD,
				mCharacter[target]->GetCX(),
				mCharacter[target]->GetCY(),
				55,turn,
				BLEND_ADD,0x80ffffff);

			SetMessage(target,"REFLECT!",0xffffffff);

			Attack(player,player,power,minatk,type,element,option | AT_OPT_NO_REFLECT,sound,ratio,act);
			return false;
		}
	}

////// 基本的な威力の取得 ////////////////------------------------------------------------------
	int atk=0;
	int def=0;

//基本攻撃力の取得
	if(player>=0){
		switch(type & AT_ATK){
			case AT_ATK_WEAPON:
				atk=mCharacter[player]->GetAtk();
				if(mMagicField==FIELD_POW)	power+=power/2;
				if(mMagicField==FIELD_MAG)	power=power/2;
				break;
			case AT_ATK_MAGIC:
				atk=mCharacter[player]->GetMagAtk();
				if(mMagicField==FIELD_MAG)	power+=power/2;
				if(mMagicField==FIELD_POW)	power=power/2;
				break;
			case AT_ATK_CONST:
				atk=power;
				power=100;
				break;
		}
	//下限値修正
		if(!(mCharacter[player]->Enemy())){
			if(atk<minatk/2){
				atk=minatk;
			}else if(atk<minatk*3/2){
				atk=minatk+(atk-minatk/2)/2;
			}
		}
	}else{
		atk=power;
		power=100;
	}
	

//基本防御力の取得
	switch(type & AT_DEF){
		case AT_DEF_WEAPON:
			def=mCharacter[target]->GetDef();		break;
		case AT_DEF_MAGIC:
			def=mCharacter[target]->GetMagDef();	break;
		case AT_DEF_ZERO:
			def=0;									break;
	}
//デバッグオプション
#ifdef _SUPER_MAN_
	if(!(mBattleOption & BOPT_SUPER_ENEMY)){
		if(target>=4){
			power*=_SUPER_MAN_;
			def=0;
		}else{
			power=0;
		}
	}
#endif
//// 防御力無視に関連する処理 ///////////------------------------------------------------------

//超強敵オプション
	if(mBattleOption & BOPT_SUPER_ENEMY){
		if(target>=4){
			power=0;
		}else{
			def=0;
			power*=10;
		}
	}
//すこし強敵オプション
	if(mBattleOption & BOPT_LP_LOCK_){
		if(target<4){
			if(def>atk/2)	def=atk/2;
		}
	}
	
//睡眠時の弱体化
	if(mCharacter[target]->SearchStatus(1)>0){
		type=type & (~AT_AVOID);
	}

//防御力無視・防御力半減
	if(player>=0){
		int dbreak=0;
		switch(type & AT_DEF){
			case AT_DEF_WEAPON:
				dbreak=mCharacter[player]->SearchASkillType(AS_DEF_BREAK,false);	break;
			case AT_DEF_MAGIC:
				dbreak=mCharacter[player]->SearchASkillType(AS_MDEF_BREAK,false);	break;
		}
		def=(100-dbreak)*def/100;
	}
//斧技などによる防御力低下処理
	if(option & AT_OPT_DEF_BREAK){
		int lv=option & AT_OPT_DEF_BREAK;
		switch(lv){
			case 1:		def=def/2;	break;
			case 2:		def=0;		break;
		}
	}

//// クリティカル率に関連する処理 ///////////------------------------------------------------------
	int crtper=0;
	bool crt=false;
	bool crtflash=false;
//クリティカルが発生する場合の基準発生率
	if(type & AT_CRITICUL){
		crtper=2;
	}
//対空補正
	if(air && (option&AT_OPT_ANTI_AIR)){
		crtper=100;
	}
//対不死補正
	if(mCharacter[target]->SearchASkill(365) && (option&AT_OPT_ANTI_UNDEAD)){
		crtper=100;
		option = option | AT_OPT_LIFE_BREAK; 
	}

	if(player>=0){
//対巨補正
		if(mCharacter[target]->GetSize()>=2 && (type & AT_SKILL) && mCharacter[player]->SearchASkill(363)){
			crtper=100;
		}
//ステータス異常・クリティカルによる補正
		if(mCharacter[player]->SearchStatus(24)>0){
			crtper=100;
		}

		if(crtper>0){
//ギャンブラーによる補正
			if((type & AT_AVOID) && mCharacter[player]->SearchASkill(353) ){
				crtper=100;
				option= option | AT_OPT_HALF_HIT;
			}
//クリティカル＋による補正
			crtper+=mCharacter[player]->SearchASkillType(AS_CRT_UP,true);
		}
	}

//技によるクリティカル率アップ補正
	if(option & AT_OPT_CRT_UP){
		crtper*=4;
	}
//クリティカル防御
	if(mCharacter[target]->SearchASkill(361)){
		crtper=0;
	}
//ガラスの体
	if(mCharacter[target]->SearchASkill(391)){
		crtper*=2;
	}
//オプション
#ifdef _ALL_CRITICAL_
	crtper=100;
#endif

//// 属性効果の計算 ///////////------------------------------------------------------
//技への属性付与
	if(type & AT_SKILL){
		if(player>=0){
			element=(element | mCharacter[player]->SearchASkillType(AS_ADD_ELEMENT,true));
		}
	}
//雷神の術、樹霊の術
	if(player>=0){
		if(mCharacter[player]->SearchASkill(344)){
			if((element & AT_ELE_FIRE) || (element & AT_ELE_WIND)){
				element=(element | AT_ELE_FIRE | AT_ELE_WIND);
			}
		}
		if(mCharacter[player]->SearchASkill(345)){
			if((element & AT_ELE_ICE) || (element & AT_ELE_EARTH)){
				element=(element | AT_ELE_ICE | AT_ELE_EARTH);
			}
		}
	}

//属性効果の計算
	int selectedelement=AT_ELE_NONE;
	int elepower=GetElementPower(player,target,element,mainele,option,&selectedelement);

//// ダメージブースター等、倍加系の係数計算 ///////////------------------------------------------------------
	double boost=1.0;

	bool dboost=false;
	bool dreduce=false;
	bool bboost=false;

	if(player>=0){
		if(mCharacter[player]->SearchASkill(255))	dboost=true;
		if(mCharacter[player]->SearchASkill(307))	dreduce=true;
		if(mCharacter[player]->SearchStatus(10)>0)	bboost=true;
		if(mCharacter[player]->SearchASkill(342) && mTurn==0)	boost+=1.0;
	}
	if(mCharacter[target]->SearchASkill(255))	dboost=true;
	if(mCharacter[target]->SearchASkill(307))	dreduce=true;
	if(mCharacter[target]->SearchStatus(10)>0)	bboost=true;

	if(dboost)		boost+=0.5;
	if(dreduce)		boost-=0.5;
	if(bboost)		boost+=1.0;



//// 回避判定 ///////////------------------------------------------------------
	if(type & AT_AVOID){
		if(!CheckHit(player,target,type,option)){
			Miss(target);
			return -1;
		}
	}
//// 最終ダメージ計算 ///////////------------------------------------------------------
//クリティカルの判定
const double DEF_POW=1.00;
const double CRT_POW=1.50;
	if(crtper>pInput->Rand(100)){
		crtflash=true;
		if(atk*(1-1/CRT_POW) < def*DEF_POW ){
			def=0;
		}else{
			crt=true;
		}
	}
//防御力によるダメージ減算処理
	def=int(def*DEF_POW);
	double damage=atk-def;
	if(damage<0)	damage=0;

//クリティカルによるダメージ倍増
	if(crt){
		damage*=CRT_POW;
	}

//威力によるダメージ倍加
	damage=damage*power/100;
//立ち位置修正
	damage=damage*ratio/100;


//ひとりぼっちの夢補正　(属性効果で無効化させる)
#ifndef _SUPER_MAN_
	if(GetTurn()<3 && mCharacter[target]->SearchASkill(403))	elepower=0;
#endif
//属性による修正
	damage=damage*elepower/100;
//倍加係数による修正
	damage*=boost;

//防御
	if(mCharacter[target]->SearchStatus(15)>0)	damage/=2;

//インスタントバリア
	int shield=mCharacter[target]->SearchStatus(16);
	if(shield>0){
		int turn;
		if(mCharacter[target]->Enemy())	turn=TURN_HORIZONTAL;
		else	turn=0;

		mTopEffect.SetEffect(
			BE_STANDARD,
			mCharacter[target]->GetCX(),
			mCharacter[target]->GetCY(),
			40,turn,
			BLEND_ADD,0x40ffffff);
		mCharacter[target]->SetStatus(16,shield-1);
		damage=damage/10;
	}

//乱数修正
	if(!(type & AT_NO_RAND)){
		damage=damage*(950+pInput->Rand(100))/1000;
	}
//トドメをさせない攻撃による修正
	if(option&AT_OPT_NO_DEATH){
		int hp=mCharacter[target]->GetHP();
		if(damage>hp-1){
			damage=hp-1;
		}
	}
//カウンターニードルの取得。戦闘不能になると消滅するため。
	int counter_needle=mCharacter[target]->SearchStatus(17);

#ifndef _SUPER_MAN_
//ダメージカンスト
	if(act && !(mCharacter[target]->GetDead())){
		int *totaldmg=act->GetTotalDamage();
		damage=int(damage);
		totaldmg[target]+=int(damage);
		if(totaldmg[target]>99999){
			damage-=totaldmg[target]-99999;
			totaldmg[target]=99999;
			if(damage<0)	damage=0;
		}
	}else{
		if(damage>99999)	damage=99999;
	}
	

//属性吸収
	int healele=mCharacter[target]->SearchASkillType(AS_ELE_HEAL,true);
	if((healele | element) == healele){
		bool heal=true;
		if(player>=0){
			if(mCharacter[player]->SearchASkill(249)){
				heal=false;
			}else if(mCharacter[player]->SearchASkill(248)){
				heal=false;
				damage=0;
			}
		}
		if(heal){
			HealHP(player,target,int(damage),mDMesID,false);
			return -1;
		}
	}
#endif


//// ダメージの処理 ///////////------------------------------------------------------

//ダメージの設定
	int lplost;
	if((mBattleOption & BOPT_LP_LOCK_) && target>=4){
		lplost=0;
		if(int(damage)>0){
			mCharacter[target]->SetDamageEffect(crtflash);
		}
	}else{
		lplost=mCharacter[target]->SetDamage(int(damage),crtflash,option);
	}

//表示メッセージの作成
	char text[128];
	if(lplost>0){
		sprintf(text,"%d",lplost);
	}else{
		sprintf(text,"%d",int(damage));
	}
	if(crtflash){
		strcat(text,"!");
	}

//色指定等と実際の表示
	if(lplost>0){
		SetNumberMessage(target,lplost,NMES_TYPE_LP,text,0,crtflash,dmesid);
	}else{
		int type=0;
		for(int i=0;i<AT_ELE_NO;i++){
			int e=1<<i;
			if(e==selectedelement){
				type=i;
				if(elepower<99){
					type+=AT_ELE_NO*1;
				}
				break;
			}
		}
		SetNumberMessage(target,int(damage),NMES_TYPE_DMG,text,type,crtflash,dmesid);
	}

//トドメ時のLPダメージ表示
	if(lplost<0 && (target<MAX_PARTY_NUMBER || (option & AT_OPT_LIFE_BREAK))){
		sprintf(text,"%d",-lplost);
		SetNumberMessage(target,-lplost,NMES_TYPE_LP,text,0,crtflash,dmesid);
	}
//攻撃音の再生
	PlayWave(sound);


//// 追加効果の類の処理 ///////////------------------------------------------------------
//ねむり、魅了の解除
	if(int(damage)>0){
		for(int j=1;j<=2;j++){
			int param=mCharacter[target]->SearchStatus(j);
			if(pInput->Rand(param+2)<2){
				mCharacter[target]->ClearStatus(j);
			}
		}
	}

//対不死による復活状態の抹消
	if(option&AT_OPT_ANTI_UNDEAD){
		mCharacter[target]->SetStatus(23,0);
	}
	if(player>=0){
//足止め
		if(mCharacter[player]->SearchASkill(292)){
			SetCancel(player,target,20);
		}
//武器攻撃時の追加効果の類
		if(type&AT_SKILL){

		//武器破壊
			if(crtflash){
				if(mCharacter[player]->SearchASkill(351)){
					int atk=mCharacter[player]->GetAtk();
					ParamChange(player,player,UP_ATK,-atk/3);
				}
			}

		//ステータス異常追加系
			int st=mCharacter[player]->SearchASkillType(AS_ADD_STATUS,true);
			for(int i=0;i<MAX_STATUS_NUMBER;i++){
				if(st & (1<<i)){
					SetStatus(player,target,i,30,-1,false);
				}
			}

		//物理反射系
			if(!(type & AT_LONG_LANGE)){
				int spine=-1;
				int ele=AT_ELE_NONE;
				int counter=mCharacter[target]->SearchASkillType(AS_COUNTER,true);
				if(counter_needle>0 ){
					spine=int(damage);
					if(counter_needle>0){
						if(counter_needle<=1 || mCharacter[target]->GetDead()){
							mCharacter[target]->ClearStatus(17);
						}else{
							mCharacter[target]->SetStatus(17,counter_needle-1);
						}
					}
				}else if(counter && life){
					spine=int(damage);
					ele=counter;
				}
				if(spine>=0){
					Attack(-1,player,spine,0,
						AT_TYPE_NO_RAND,ele,
						AT_OPT_NO_EFFECT,SE_SPINE,100,act);
				}
			}
			
		//パラメーター低下系
			st=mCharacter[player]->SearchASkillType(AS_ADD_ST_DOWN,true);
			for(int i=1;i<=PARAM_UP_NO;i=i<<1){
				if(st & i){
					if(pInput->Rand(100)<30){
						switch(i){
							case UP_SPD:
								ParamChange(player,target,i,-mCharacter[target]->GetBaseSpeed()*3/10);	break;
							case UP_ATK:
								ParamChange(player,target,i,-mCharacter[target]->GetBaseAtk()*3/10);	break;
							case UP_DEF:
								ParamChange(player,target,i,-mCharacter[target]->GetBaseDef()/2);		break;
							case UP_MATK:
								ParamChange(player,target,i,-mCharacter[target]->GetBaseMagAtk()*3/10);	break;
							case UP_MDEF:
								ParamChange(player,target,i,-mCharacter[target]->GetBaseMagDef()/2);	break;
						}
					}
				}
			}
		}
	}

	return int(damage);
}
int		CBattleMode::GetElementPower(int player,int target,int element,int mainele,int option,int* selected)
{
	bool eleset=false;
	int elepower=0;
	for(int i=1;i<(1<<AT_ELE_NO);i=(i<<1)){
		if(!(element & i))	continue;
		int pow;
		switch(i){
			case AT_ELE_NONE:	pow=mCharacter[target]->SearchASkillType(AS_NO_ELE_GUARD,true);	break;
			case AT_ELE_FIRE:	pow=mCharacter[target]->SearchASkillType(AS_FIRE_GUARD,true);	break;
			case AT_ELE_ICE:	pow=mCharacter[target]->SearchASkillType(AS_ICE_GUARD,true);	break;
			case AT_ELE_WIND:	pow=mCharacter[target]->SearchASkillType(AS_WIND_GUARD,true);	break;
			case AT_ELE_EARTH:	pow=mCharacter[target]->SearchASkillType(AS_EARTH_GUARD,true);	break;
			default:	pow=0;
		}
		if(pow>0x7fff)	pow-=0xffff;
		if(pow>90)		pow=90;
		if(mCharacter[target]->SearchASkill(349)){
			const int up[4]  ={AT_ELE_WIND	,AT_ELE_EARTH	,AT_ELE_FIRE	,AT_ELE_ICE	};
			const int down[4]={AT_ELE_EARTH	,AT_ELE_WIND	,AT_ELE_ICE		,AT_ELE_FIRE};
			if(i==up[mTurn%4])		pow-=100;
			if(i==down[mTurn%4])	pow+=50;
		}

		if(player>=0){
			int powup=mCharacter[player]->SearchASkillType(AS_ELE_POW_UP,true);
			pow=pow*(100+powup)/100;

			if(pow>0){
				int ebreak=mCharacter[player]->SearchASkillType(AS_ELEMENT_BREAK,false);
				pow=pow*(100-ebreak)/100;
				if(option & AT_OPT_ANTI_AIR_ELEMENT_ && mCharacter[target]->CheckAir()){
					pow=0;
				}
			}
		}
		pow=100-pow;

		switch(i){
			case AT_ELE_FIRE:
				if(mMagicField==FIELD_FIRE)	pow+=pow/2;
				if(mMagicField==FIELD_ICE)	pow=pow/2;
				break;
			case AT_ELE_ICE:
				if(mMagicField==FIELD_ICE)	pow+=pow/2;
				if(mMagicField==FIELD_FIRE)	pow=pow/2;
				break;
			case AT_ELE_WIND:
				if(mMagicField==FIELD_WIND)	pow+=pow/2;
				if(mMagicField==FIELD_EARTH)pow=pow/2;
				break;
			case AT_ELE_EARTH:
				if(mMagicField==FIELD_EARTH)pow+=pow/2;
				if(mMagicField==FIELD_WIND)	pow=pow/2;
				break;
		}
		if(!(mainele & i))	pow--;

		if(elepower<pow){
			elepower=pow;
			if(selected){
				*selected=i;
			}
		}
	}

	return elepower;
}
bool	CBattleMode::CheckHit(int player,int target,int type,int option)
{
	if(mCharacter[target]->GetDead())	return true;

	int tspd=mCharacter[target]->GetSpeed();
	int pspd;
	if(player>=0){
		pspd=mCharacter[player]->GetSpeed();
	}else{
		pspd=1;
	}

	const int BASE_HIT	= 95;
	const int CHANGE	= 25;
	int hit;
	if(tspd<pspd){
		hit = BASE_HIT + CHANGE- (CHANGE * tspd/pspd);
	}else{
		hit = BASE_HIT - CHANGE + (CHANGE * pspd/tspd);
	}
	hit-=mCharacter[target]->SearchASkillType(AS_AVOID_UP,true);

	if(mCharacter[target]->CheckAir()){
		if(!(type & AT_LONG_LANGE)){
			hit-=30;
		}
		if(option & AT_OPT_ANTI_AIR){
			hit+=50;
		}
		if(player>=0){
			if(mCharacter[player]->SearchASkill(347))	return false;
		}
	}
	if(player>=0){
		if(mCharacter[target]->GetSize()>=2 && (type & AT_SKILL) && mCharacter[player]->SearchASkill(363)){
			hit+=50;
		}

		if(mCharacter[player]->SearchASkill(250)){
			hit+=50;
		}
	}

	if( option & AT_OPT_HALF_HIT)	hit/=2;
	if(player>=0){
		if(mCharacter[player]->SearchStatus(18)>0)	hit/=2;
		if(mCharacter[player]->SearchStatus(5)>0)	hit/=2;
	}

	if(hit<20)	hit=20;
	if(pInput->Rand(100)<(100-hit)){
		return false;
	}
	return true;
}
void	CBattleMode::Miss(int target)
{
	SetMessage(target,"MISS!",0xffffffff);
	mCharacter[target]->SetAvoid();
	PlayWave(SE_MISS);
}

int		CBattleMode::HealHP(int player,int target,int power,int dmesid,bool revival)
{
	if(power<=0)	power=1;

	if(mCharacter[target]->GetHP()<=0){
		if(!revival)	return 0;
		mCharacter[target]->ChangeAni(6*ANI_TYPE,8,true);
	}

	if(player>=0){
		int hup=mCharacter[player]->SearchASkillType(AS_HP_HEAL_UP,true);
		power=power*(100+hup)/100;
		if(mCharacter[player]->SearchASkill(308)){
			HealLP(player,target,int(power*0.03));
		}
	}
	if(power>99999)	power=99999;

	power=mCharacter[target]->HealHP(power);

	char text[128];
	sprintf(text,"%d",power);

	SetNumberMessage(target,power,NMES_TYPE_HEAL,text,0,false,dmesid);
	PlayWave(SE_HEAL);

	return power;
}
int		CBattleMode::HealLP(int player,int target,int power)
{
	if(power>99999)	power=99999;
	power=mCharacter[target]->HealLP(power);

	char text[128];
	sprintf(text,"%d",power);

	SetMessage(target,text,0xff00ffff);
	PlayWave(SE_HEAL);
	return power;
}
int		CBattleMode::LostLP(int player,int target,int power,int dmesid)
{
	int ret=mCharacter[target]->LostLP(power);

	char text[128];
	sprintf(text,"%d",ret);

	SetNumberMessage(target,power,NMES_TYPE_LP,text,0,false,dmesid);
	return ret;
}
int		CBattleMode::HealSP(int player,int target,int lv,int power)
{
	power=mCharacter[target]->HealSP(lv,power);

	char text[128];
	sprintf(text,"SP%d +%d",lv+1,power);

	SetMessage(target,text,0xff00ffff);
	PlayWave(SE_HEAL);

	return power;
}
int		CBattleMode::LostSP(int player,int target,int lv,int power)
{
	mCharacter[target]->LostSP(lv,power);

	char text[128];
	sprintf(text,"SP%d -%d",lv+1,power);

	SetMessage(target,text,0xffff0000);

	return power;
}
int		CBattleMode::ParamChange(int player,int target,int param,int up)
{
	if(mCharacter[target]->GetDead()){
		return 0;
	}
	if (up<0 && mCharacter[target]->SearchASkill(336)){
		up=0;
	}
	if(player>=0 && up>0 && mCharacter[player]->SearchASkill(425)){
		up*=2;
	}
	int plus=0;
	const char* paramname="?";

	switch(param){
		case UP_ATK:
			plus=mCharacter[target]->SetAtkUp(up);
			paramname="AT";
			break;
		case UP_DEF:
			plus=mCharacter[target]->SetDefUp(up);
			paramname="DF";
			break;
		case UP_MATK:
			plus=mCharacter[target]->SetMagAtkUp(up);
			paramname="MA";
			break;
		case UP_MDEF:
			plus=mCharacter[target]->SetMagDefUp(up);
			paramname="MD";
			break;
		case UP_SPD:
			plus=mCharacter[target]->SetSpdUp(up);
			paramname="SPD";
			break;
	}

	char text[128];
	sprintf(text,"%s%+d",paramname,plus);

	D3DCOLOR color=0xffffffff;
	if(up>0){
		color=0xff00ff00;
		PlayWave(SE_PARAM_UP);
	}else if (up<0){
		color=0xff8080ff;
		PlayWave(SE_PARAM_DOWN);
	}
	SetMessage(target,text,color);

	return plus;
}
int		CBattleMode::SetStatus(int player,int target,int st,int hit,int param,bool showmiss)
{
	hit*=10;
	if(mCharacter[target]->GetDead()){
		return 0;
	}
	if(param<0)		param=StatusData[st].mParam;

	if(StatusData[st].mBad){
#ifdef _SUPER_MAN_
		return 0;
#endif
		int resist=mCharacter[target]->SearchASkillType(AS_ANTI_STATUS,false);
		hit=hit*(100-resist)/100;
	}
	if(player>=0){
		if(mCharacter[player]->SearchASkill(378)){
			hit+=hit/2;
		}
	}

	if(player<0 || !mCharacter[player]->SearchASkill(400)){
		int p=1<<st;
		int sk=mCharacter[target]->SearchASkillType(AS_STATUS_GUARD,true);
		if((p & sk)  || hit<=0){
			if(showmiss){
				SetMessage(target,"GUARD",0xffb0b0b0);
			}
			return 0;
		}
	}

	if(pInput->Rand(1000)>=hit){
		if(showmiss){
			SetMessage(target,"MISS!",0xffb0b0b0);
		}
		return 0;
	}

	if(!mCharacter[target]->SetStatus(st,param)){
		//エラー
		return 0;
	}
	SetMessage(target,StatusData[st].mEnglish,StatusData[st].mColor);

	if(st==1 || st==26){	//眠り、気絶
		mCharacter[target]->SetAction(-1);
	}
	if(st==2){	//魅了
		SetRandomTarget(target,mCharacter[target]->GetAction());
	}
	if(st==3){	//麻痺
		int act=mCharacter[target]->GetAction();
		int type=type=CS_SKILL | CS_SWORD | CS_SPEAR | CS_AXE | CS_BOW ;
		if(ComSkillData[act].mType & type){
			CancelAction(target);
		}
	}
	if(st==4){	//封印
		int act=mCharacter[target]->GetAction();
		if(ComSkillData[act].mType & CS_MAGIC){
			CancelAction(target);
		}
	}
	if(st==5){	//暴走
		SetRandomTarget(target,mCharacter[target]->GetAction());
	}
	if(st==14){	//ダイブ
		mCharacter[target]->NoActionCount();
		CancelAction(target);
	}
	//グラビティーとスカイウォークの背反
	if(st==9)	mCharacter[target]->ClearStatus(13);
	if(st==13)	mCharacter[target]->ClearStatus(9);

	PlayWave(StatusData[st].mSE);
	return param;

}
bool		CBattleMode::CureStatus(int player,int target)
{
	if(mCharacter[target]->GetDead()){
		return false;
	}

	if(mCharacter[target]->SearchStatus(2)>0){
		CancelAction(target);
	}
	if(mCharacter[target]->SearchStatus(5)>0){
		CancelAction(target);
	}

	if(mCharacter[target]->ClearBadStatus()){
		SetMessage(target,"CURE!",0xff00ff00);

		PlayWave(SE_CURE);
		return true;
	}
	return false;
}
bool		CBattleMode::DispelStatus(int player,int target)
{
	SetMessage(target,"DISPEL!",0xff808080);
	PlayWave(SE_HARD_SLASH);

	return mCharacter[target]->ClearGoodStatus();
}
bool		CBattleMode::SetCancel(int player,int target,int base)
{
	if(player==target)							return false;
	if(mCharacter[target]->GetDead())			return false;
	if(mCharacter[target]->SearchASkill(362))	return false;

	if(player>=0){
		int psp=mCharacter[player]->GetTech();
		int tsp=mCharacter[target]->GetTech();
		double ratio = double(psp-tsp)/psp ;
		if(ratio < -1 )	ratio =-1;
		base=int(base+base*ratio/2);
	}

	if(pInput->Rand(100)<base){
		SetMessage(target,"CANCEL!",0xffffffff);
		CancelAction(target);
		return true;
	}
	return false;
}

void	CBattleMode::MoveCharacter(int target,int x,int y,bool move)
{
	int	lockedfield[BATTLE_SQUARE_WIDTH][BATTLE_SQUARE_Height];
	bool locked[CHARACTER_NO];
	memset(locked,0,sizeof(locked));
	int start,end;
	int left,right;

	if(target<MAX_PARTY_NUMBER || target>=MAX_PARTY_NUMBER+MAX_ENEMY_NUMBER){
		start=0;	end=CHARACTER_NO;
		left=5;		right=9;
	}else{
		start=MAX_PARTY_NUMBER;	end=CHARACTER_NO;
		left=0;		right=4;
	}

	int size=mCharacter[target]->GetSize();

	if(x<left)						x=left;
	if(right-size<x)				x=right-size;
	if(y<0)							y=0;
	if(BATTLE_SQUARE_Height-size<y)	y=BATTLE_SQUARE_Height-size;

	mCharacter[target]->Move(x,y,BM_INTARVAL_TIME/2,move);
	locked[target]=true;

	bool finish;
	for(int i=0;i<128;i++){
		finish=true;
		CreateCharacterField(locked,lockedfield);
		for(int j=start;j<end;j++){
			if(locked[j])	continue;
			if(CheckField(mCharacter[j],lockedfield)){
				MoveNear(j,left,right,lockedfield,move);
				locked[j]=true;
				finish=false;
				CreateCharacterField(locked,lockedfield);
			}

			if(!finish)	break;
		}
		if(finish)	break;
	}
}

bool	CBattleMode::MoveNear(int target,int left,int right,int	field[BATTLE_SQUARE_WIDTH][BATTLE_SQUARE_Height],bool move)
{
	int size=mCharacter[target]->GetSize();
	int sx=mCharacter[target]->GetSX();
	int sy=mCharacter[target]->GetSY();

	int pri[BATTLE_SQUARE_WIDTH*BATTLE_SQUARE_Height];
	int no=0;

	right-=size;
	int bottom=BATTLE_SQUARE_Height-size;

	for(int x=left;x<=right;x++){
		for(int y=0;y<=bottom;y++){
			if(!CheckField(x,y,size,field)){
				int l=abs(sx-x)+abs(sy-y);
				pri[no]=256-l+x*0x10000+y*0x1000000;
				no++;
			}
		}
	}
	if(no<=0)	return false;
	int m=PriorityRand(pri,no,0xffff);
	int mx=(pri[m]/0x10000)&0xff;
	int my=pri[m]/0x1000000;
	mCharacter[target]->Move(mx,my,BM_INTARVAL_TIME/2,move);
	return true;
}
bool	CBattleMode::CheckEnemy(int player,int target)
{
	if(0<=target && target<PET ){
		return !(mCharacter[target]->Enemy());
	}
	return mCharacter[player]->Enemy();
}
int*	CBattleMode::GetTarget(int player,int center,int act,int *no,bool liveonly)
{
//	static int	AllTarget[CHARACTER_NO];
	static int  Field[BATTLE_SQUARE_WIDTH][BATTLE_SQUARE_Height];

	GetTargetArea(player,center,act,Field);

	return GetTarget(player,Field,center,no,liveonly);
/*
	int targetno=0;
	int topt=center;
	if(mCharacter[topt]->GetLive()){
		int field=CheckField(mCharacter[topt],Field);
		if(field && mCharacter[topt]->SearchStatus(14)<=0){
			AllTarget[targetno]=topt+field*0x10000;
			targetno++;
		}
	}

	for(int i=0;i<CHARACTER_NO;i++){
		if(i==topt)									continue;
		if(liveonly && mCharacter[i]->GetDead())	continue;
		if(mCharacter[i]->SearchStatus(14)>0)		continue;
		if(mCharacter[i]->GetLive()){
			int field=CheckField(mCharacter[i],Field);
			if(field){
				AllTarget[targetno]=i+field*0x10000;
				targetno++;
			}
		}
	}
	if(no)	*no=targetno;

	return AllTarget;
*/
}
int*	CBattleMode::GetTarget(int player,int field[BATTLE_SQUARE_WIDTH][BATTLE_SQUARE_Height],int center,int *no,bool liveonly)
{
	static int	AllTarget[CHARACTER_NO];

	int targetno=0;
	int topt=center;
	if(topt>=0 && mCharacter[topt]->GetLive()){
		int f=CheckField(mCharacter[topt],field);
		if(f && mCharacter[topt]->SearchStatus(14)<=0){
			AllTarget[targetno]=topt+f*0x10000;
			targetno++;
		}
	}

	int setplayer=0;	//使用者は、ターゲットの最後に追加する例外処理。
							//アースクエイクなどで死んでも、全体にダメージを与えるため。
	for(int i=0;i<CHARACTER_NO;i++){
		if(i==topt)									continue;
		if(liveonly && mCharacter[i]->GetDead())	continue;
		if(mCharacter[i]->SearchStatus(14)>0)		continue;
		if(mCharacter[i]->GetLive()){
			int f=CheckField(mCharacter[i],field);
			if(f){
				if(i==player){
					setplayer=f;
				}else{
					AllTarget[targetno]=i+f*0x10000;
					targetno++;
				}
			}
		}
	}

	if(setplayer){//使用者は、ターゲットの最後に追加する例外処理。
		AllTarget[targetno]=player+setplayer*0x10000;
		targetno++;
	}

	if(no)	*no=targetno;

	return AllTarget;
}

int	CBattleMode::SetRandomTarget(int player,int act)
{
	int target[CHARACTER_NO];
	int possible;
	int no=GetPossibleTarget(player,act,target,&possible);
	if(possible>0)	no=possible;
	if(no<=0)	return -1;
	int t=target[pInput->Rand(no)];
	mCharacter[player]->SetTarget(t);
	return 0;
}
int	CBattleMode::SetRandomAction(int player)
{
	if(!mCharacter[player]->GetLive())	return -1;
	int skill[MAX_CSKILL];
	int skillno=mCharacter[player]->GetUsableCSkill(skill);
	if(skillno>0){
		int act=skill[pInput->Rand(skillno)];
		mCharacter[player]->SetAction(act);
		SetRandomTarget(player,act);
		return act;
	}
	return -1;
}
int	CBattleMode::GetPossibleTarget(int player,int act,int* target,int* possible)
{
	bool enemy=mCharacter[player]->Enemy();
	if(ComSkillData[act].mTarget)				enemy=!enemy;
	if(mCharacter[player]->SearchStatus(5)>0)	enemy=!mCharacter[player]->Enemy();
	if(mCharacter[player]->SearchStatus(2)>0)	enemy=!enemy;
	if(mCharacter[player]->SearchASkill(382))	enemy=!enemy;

	int character[CHARACTER_NO];
	int	max=0;
	if(!enemy){
		for(int i=0;i<MAX_PARTY_NUMBER;i++){
			character[max]=i;
			max++;
		}
		for(int i=0;i<MAX_PET_NUMBER;i++){
			character[max]=i+PET;
			max++;
		}
	}
	if(enemy){
		for(int i=0;i<MAX_ENEMY_NUMBER;i++){
			character[max]=i+ENEMY;
			max++;
		}
	}


	int no=0;
	for(int i=0;i<max;i++){
		CBattleCharacter* bc=mCharacter[character[i]];
		if(!bc->GetLive())	continue;
		if(!bc->GetCData())	continue;

		bool ok=!(bc->GetDead());
		if(ComSkillData[act].mType&CS_RIVIVAL)	ok=!ok;
		if(bc->SearchASkill(383))				ok=false;
		if(ok){
			if(	BattleMode.CheckTarget(mCharacter[player],character[i],act)==character[i]){
				target[no]=character[i];
				no++;
			}
		}
	}
	if(!possible){
		return no;
	}

	*possible=no;

	for(int i=0;i<max;i++){
		CBattleCharacter* bc=mCharacter[character[i]];
		if(bc->GetCData() && (!(bc->GetDead()) || (ComSkillData[act].mType&CS_RIVIVAL)) ){
			bool add=true;
			for(int j=0;j<no;j++){
				if(target[j]==character[i]){
					add=false;
					break;
				}
			}
			if(add){
				target[no]=character[i];
				no++;
			}
		}
	}

	return no;
}

void	CBattleMode::CreateCharacterField(bool* character,int	field[BATTLE_SQUARE_WIDTH][BATTLE_SQUARE_Height])
{
	memset(field,0,sizeof(int)*BATTLE_SQUARE_WIDTH*BATTLE_SQUARE_Height);
	for(int i=0;i<CHARACTER_NO;i++){
		if(character){
			if(!character[i])	continue;
		}
		if(!mCharacter[i]->GetLive())	continue;

		int x=mCharacter[i]->GetSX();
		int y=mCharacter[i]->GetSY();
		int size=mCharacter[i]->GetSize();
		for(int j=0;j<size;j++){
			for(int k=0;k<size;k++){
				field[x+j][y+k]=1;
			}
		}
	}
}

int	CBattleMode::CheckField(CBattleCharacter* character,int	field[BATTLE_SQUARE_WIDTH][BATTLE_SQUARE_Height])
{
	int size=character->GetSize();
	int x=character->GetSX();
	int y=character->GetSY();
	return CheckField(x,y,size,field);
}
int	CBattleMode::CheckField(int x,int y,int size,int field[BATTLE_SQUARE_WIDTH][BATTLE_SQUARE_Height])
{
	int max=0;
	for(int i=0;i<size;i++){
		int tx=i+x;
		for(int j=0;j<size;j++){
			int ty=j+y;
			if(field[tx][ty]>max){
				max=field[tx][ty];
			}
		}
	}
	return max;
}
int*	CBattleMode::GetTargetArea(int player,int target,int skill,int	field[BATTLE_SQUARE_WIDTH][BATTLE_SQUARE_Height],int* fn)
{
	static int	AllFriend[CHARACTER_NO];

	int fno;
	int rt=CheckTarget(	mCharacter[player],
							target,
							skill,&fno,AllFriend);

	int ratio=int(100*pow(0.7,fno-1));

	GetAttackArea(	player,
					mCharacter[rt]->GetSX(),
					mCharacter[rt]->GetSY(),
					mCharacter[rt]->GetSize(),
					ComSkillData[skill].mRange,
					field,ratio);
	if(fn)	*fn=fno;
	return AllFriend;
}



int	CBattleMode::CheckTarget(CBattleCharacter* player,int target,int skill,int* fno,int *fc)
{
	int playerno=GetCharacterNo(player);
	if(fno)	*fno=1;
	if(fc){
		fc[0]=playerno;
	}

	if( (!(ComSkillData[skill].mType&CS_MOVE))
		||(player->Enemy() == mCharacter[target]->Enemy())
		||(player->CheckAir()) ){
		return target;
	}


	double m,n;
	double x1,x2,y1,y2;
	x1=player->GetSX();
	y1=player->GetSY();
	x2=mCharacter[target]->GetSX();
	y2=mCharacter[target]->GetSY();

	int move;
	if(x1>x2){
		move=-1;
	}else{
		move=1;
	}
	x2-=move*0.5;

	m = (y1-y2)/(x1-x2);
	n = y1-m*x1;
	

	int character[BATTLE_SQUARE_WIDTH][CHARACTER_NO];
	int cnum[BATTLE_SQUARE_WIDTH];
	int fcharacter[BATTLE_SQUARE_WIDTH][CHARACTER_NO];
	int fnum[BATTLE_SQUARE_WIDTH];
	memset(cnum,0,sizeof(cnum));
	memset(fnum,0,sizeof(fnum));

	if(player->Enemy()){
		for(int i=0;i<MAX_PARTY_NUMBER;i++){
			int x=mPlayer[i].GetSX();
			character[x][cnum[x]]=i;
			cnum[x]++;
		}
		for(int i=0;i<MAX_PET_NUMBER;i++){
			int x=mPet[i].GetSX();
			character[x][cnum[x]]=i+PET;
			cnum[x]++;
		}
		for(int i=0;i<MAX_ENEMY_NUMBER;i++){
			int x=mCharacter[ENEMY+i]->GetSX() + (mCharacter[ENEMY+i]->GetSize()-1);
			if(x<0)	x=0;
			fcharacter[x][fnum[x]]=i+ENEMY;
			fnum[x]++;
		}
	}else{
		for(int i=0;i<MAX_ENEMY_NUMBER;i++){
			int x=mCharacter[ENEMY+i]->GetSX() + (mCharacter[ENEMY+i]->GetSize()-1);
			if(x<0)	x=0;
			character[x][cnum[x]]=i+ENEMY;
			cnum[x]++;
		}
		for(int i=0;i<MAX_PARTY_NUMBER;i++){
			int x=mPlayer[i].GetSX();
			fcharacter[x][fnum[x]]=i;
			fnum[x]++;
		}
		for(int i=0;i<MAX_PET_NUMBER;i++){
			int x=mPet[i].GetSX();
			fcharacter[x][fnum[x]]=i+PET;
			fnum[x]++;
		}
	}
	for(int i=0;i<MAX_WALL_NUMBER;i++){
		int x=mWall[i].GetSX();
		character[x][cnum[x]]=i+WALL;
		cnum[x]++;
	}

	int vline=player->GetSX();
	int goal=mCharacter[target]->GetSX();
	//int targetsize=mCharacter[target]->GetSize();
	int targetsize=1;

	while(0<=vline && vline<BATTLE_SQUARE_WIDTH){
		if(vline==goal)	return target;
		for(int i=0;i<cnum[vline];i++){
			if(target==character[vline][i])	return target;
			if(	mCharacter[character[vline][i]]->GetActive()){
				if(mCharacter[character[vline][i]]->SearchASkill(343)){
					return character[vline][i];
				}
				int s=mCharacter[character[vline][i]]->GetSize();
				double x=mCharacter[character[vline][i]]->GetSX()+(s-1)/2.0;
				double y=mCharacter[character[vline][i]]->GetSY()+(s-1)/2.0;
				double size=(s-targetsize/2+0.5)*HIT_SIZE;
				size=size/BATTLE_SQUARE_SIZE;

				double length=(y-m*x-n)/sqrt(1+m*m);
				if(length<0)	length=-length;
				if(length<size){
					return CheckTarget(player,character[vline][i],skill,fno,fc);
				}
			}
		}
		for(int i=0;i<fnum[vline];i++){
			if(	mCharacter[fcharacter[vline][i]]->GetActive()){
				int s=mCharacter[fcharacter[vline][i]]->GetSize();
				double x=mCharacter[fcharacter[vline][i]]->GetSX()+(s-1)/2.0;
				double y=mCharacter[fcharacter[vline][i]]->GetSY()+(s-1)/2.0;
				double size=(s+1)*HIT_SIZE;
				size=size/BATTLE_SQUARE_SIZE/2;

				double length=(y-m*x-n)/sqrt(1+m*m);
				if(length<0)	length=-length;
				if(length<size){
					if(fcharacter[vline][i]!=playerno){
						if(fno){
							if(fc)	fc[(*fno)]=fcharacter[vline][i];
							(*fno)++;
						}
					}
				}
			}
		}
		vline+=move;
	}

	return target;
}
void CBattleMode::GetAttackArea(int player,int x,int y,int size,int type,int field[BATTLE_SQUARE_WIDTH][BATTLE_SQUARE_Height],int ratio)
{
	memset(field,0,BATTLE_SQUARE_WIDTH*BATTLE_SQUARE_Height*sizeof(int));

	int i;
	int left,right;
	if(x<BATTLE_SQUARE_WIDTH/2){
		left=0;right=BATTLE_SQUARE_WIDTH/2+1;
	}else if (x>BATTLE_SQUARE_WIDTH/2){
		left=BATTLE_SQUARE_WIDTH/2;right=BATTLE_SQUARE_WIDTH;
	}else{
		if((player<MAX_PARTY_NUMBER) || ( MAX_ENEMY_NUMBER<=player)){
			left=0;right=BATTLE_SQUARE_WIDTH/2+1;
		}else{
			left=BATTLE_SQUARE_WIDTH/2;right=BATTLE_SQUARE_WIDTH;
		}
	}

	double cx=x+(size-1)*0.5;
	double cy=y+(size-1)*0.5;
	if(mCharacter[player]->SearchASkill(402)){
		switch(type){
		case CR_SELF:
		case CR_NONE:
		case CR_S_ALL:
			break;
		default:
			type=CR_ALL;
		}
	}
	switch(type){
		case CR_VLINE:
			for(i=0;i<BATTLE_SQUARE_Height;i++){
				field[x+size-1][i]=ratio;
			}
			break;
		case CR_HLINE:
			for(i=left;i<right;i++){
				field[i][y+size-1]=ratio;
			}
			break;
		case CR_CIRCLE:
			for(i=left;i<right;i++){
				for(int j=0;j<BATTLE_SQUARE_Height;j++){
					int l=int((cx-i)*(cx-i)+(cy-j)*(cy-j));
					if(l<2){
						field[i][j]=ratio;
					}
				}
			}
			break;
		case CR_BIG_CIRCLE:
			for(i=left;i<right;i++){
				for(int j=0;j<BATTLE_SQUARE_Height;j++){
					int l=int(abs(cx-i)+abs(cy-j));
					//int l=int((cx-i)*(cx-i)+(cy-j)*(cy-j));
					if(l<3){
						field[i][j]=ratio;
					}
				}
			}
			break;
		case CR_S_ALL:
			left=0;right=BATTLE_SQUARE_WIDTH;
		case CR_ALL:
			for(i=left;i<right;i++){
				for(int j=0;j<BATTLE_SQUARE_Height;j++){
					field[i][j]=ratio;
				}
			}
			break;
		case CR_NONE:
			break;
		case CR_SELF:
			x=mCharacter[player]->GetSX();
			y=mCharacter[player]->GetSY();
		default:
			if(player>=0 && size<=1){
				if(mCharacter[player]->SearchASkill(290) && type==CR_ONE_M){
					for(i=left;i<right;i++){
						for(int j=0;j<BATTLE_SQUARE_Height;j++){
							int l=(x-i)*(x-i)+(y-j)*(y-j);
							if(l<2){
								field[i][j]=int(ratio*0.3);
							}
						}
					}
				}
			}
			for(i=0;i<size;i++){
				for(int j=0;j<size;j++){
					field[x+i][y+j]=ratio;
				}
			}
			break;
	}
}

void		CBattleMode::ChangeFieldColor(D3DCOLOR color,int time)
{
	mColorChangeGoal=color;
	mColorChangeTimer=time;
}
double	CBattleMode::GetShotSpeed(int player,int target,int spd,double *x,double *y)
{
	int px=mCharacter[player]->GetCX();
	int py=mCharacter[player]->GetCY();
	int tx=mCharacter[target]->GetCX();
	int ty=mCharacter[target]->GetCY();

	return GetShotSpeed(px,py,tx,ty,spd,x,y);
}
double	CBattleMode::GetShotSpeed(int px,int py,int tx,int ty,int spd,double *x,double *y)
{
	int sx=tx-px;
	int sy=ty-py;
	if(sx==0 && sy==0)		sx=1;
	double length=hypot(sx,sy)/spd;
	sx=int(sx/length);
	sy=int(sy/length);

	if(x)	*x=sx;
	if(y)	*y=sy;
	return length;
}

double	CBattleMode::GetShotSpeedByTime(int player,int target,int time,double *x,double *y)
{
	int px=mCharacter[player]->GetCX();
	int py=mCharacter[player]->GetCY();
	int tx=mCharacter[target]->GetCX();
	int ty=mCharacter[target]->GetCY();

	return GetShotSpeedByTime(px,py,tx,ty,time,x,y);
}
double	CBattleMode::GetShotSpeedByTime(int px,int py,int tx,int ty,int time,double *x,double *y)
{
	int sx=tx-px;
	int sy=ty-py;
	if(sx==0 && sy==0)		sx=1;
	sx=int(sx/time);
	sy=int(sy/time);

	if(x)	*x=sx;
	if(y)	*y=sy;
	return 0;
}
int		CBattleMode::GetRandomTarget(int player,int action)
{
	int no,*target ;
	int t=mCharacter[player]->GetTarget();
	target=GetTarget(player,t,action,&no,true);
	int min=0;
	if(no>=2){
		if(mCharacter[target[0] & 0xff]->GetDead()){
			min=1;
			no--;
		}
	}
	if(no<=0){
		return mCharacter[player]->GetTarget();
	}
	return target[pInput->Rand(no) + min]&0xff;
}

void	CBattleMode::SetFieldEffect(int x,int y,int pow,int option,int time)
{
	if(pow>0xffff)	pow=0xffff;
	if(time>0xf)	time=0xf;

	int effect=pow | option;
	int& fe=mFieldEffect[x][y]; 
	if((fe & 0x0fff0000) == (effect & 0x0fff0000)){ 
		if(fe < effect){
			fe=effect;
		}
		if(((fe&FE_TIMER) >> FE_TIMER_SHIFT) < time){
			fe=(fe & FE_TIMER_MASK)|(time<<FE_TIMER_SHIFT);
		}
	}else{
		fe = effect | (time<<FE_TIMER_SHIFT);
	}
}

void	CBattleMode::Action(void)
{
	std::list<CBattleAction*>::iterator it;
	it = mActionList.begin();
	while( it != mActionList.end() ) 
	{
      int ret=(*it)->OnTimer();
	  if(ret>0){
			(*it)->End();
			CancelAction((*it)->GetActiveCharacter());
			std::list<CBattleActionNameWindow>::iterator	winit;
			winit = mActionNameList.begin();
			while( winit != mActionNameList.end() ) {
				if(winit->GetAction() == (*it)){
					winit->Destroy();
				}
				winit++;
			}
			delete (*it);
			it=mActionList.erase(it);
	  }else{
		 it++;
	  }
	}
}
////////////////////////////////////////////////////////////////////////////////////////////
void CBattleMode::SetActionList(int action,int acharacter)
{
	CBattleAction* BattlAction=NULL;

	int iparama;
	mCharacter[acharacter]->GetUseItemParam(&iparama,NULL);
	switch(action){
//技/////////////////////////////////////////////////////////////////////////////////
		case 0:		BattlAction=new CActNormalAttack(SE_HIT,1,100);				break;
		case 1:		BattlAction=new CActNormalAttack(SE_SLASH,0,100);			break;
		case 2:		BattlAction=new CActThrust();								break;
		case 3:		BattlAction=new CActNormalAttack(SE_CHOP,3,100);			break;
		case 4:		BattlAction=new CActShoot(100);								break;
		case 5:		BattlAction=new CActLaser();								break;
//剣技
		case 8:		BattlAction=new CAct_8();									break;
		case 9:		BattlAction=new CAct_9();									break;
		case 10:	BattlAction=new CAct_10();									break;
		case 11:	BattlAction=new CActSpinAttack(120,MIN_ATK_LV_1);			break;
		case 12:	BattlAction=new CAct_12();									break;
		case 13:	BattlAction=new CAct_13();									break;
		case 14:	BattlAction=new CAct_14();									break;
		case 15:	BattlAction=new CAct_15();									break;
		case 16:	BattlAction=new CAct_SevenStars(100,MIN_ATK_LV_4+200,7,0,17,7,SE_HARD_SLASH);	break;
		case 17:	BattlAction=new CAct_SevenStars(75,0,12,AT_OPT_HALF_HIT,66,1,SE_SLASH);		break;
//槍技
		case 18:	BattlAction=new CActSpinAttack(100,MIN_ATK_LV_0);			break;
		case 19:	BattlAction=new CAct_19();									break;
		case 20:	BattlAction=new CAct_20();									break;
		case 21:	BattlAction=new CAct_21();									break;
		case 22:	BattlAction=new CAct_22();									break;
		case 23:	BattlAction=new CAct_23();									break;
		case 24:	BattlAction=new CAct_24();									break;
		case 25:	BattlAction=new CAct_25();									break;
		case 26:	BattlAction=new CAct_26();									break;
		case 27:	BattlAction=new CAct_27();									break;
//斧技
		case 28:	BattlAction=new CActHardAttack(160,3,SE_CHOP);				break;
		case 29:	BattlAction=new CAct_29();									break;
		case 30:	BattlAction=new CAct_30();									break;
		case 31:	BattlAction=new CAct_31();									break;
		case 32:	BattlAction=new CAct_32();									break;
		case 33:	BattlAction=new CAct_33_34(UP_ATK,44);						break;
		case 34:	BattlAction=new CAct_33_34(UP_DEF,45);						break;
		case 35:	BattlAction=new CAct_35();									break;
		case 36:	BattlAction=new CAct_36();									break;
		case 37:	BattlAction=new CAct_37();									break;
//弓技
		case 38:	BattlAction=new CActShoot(200);								break;
		case 39:	BattlAction=new CAct_39();									break;
		case 40:	BattlAction=new CAct_40();									break;
		case 41:	BattlAction=new CAct_41();									break;
		case 42:	BattlAction=new CAct_42();									break;
		case 43:	BattlAction=new CAct_43();									break;
		case 44:	BattlAction=new CAct_44();									break;
		case 45:	BattlAction=new CAct_45();									break;
		case 46:	BattlAction=new CAct_46();									break;
		case 47:	BattlAction=new CAct_47();									break;
//格闘
		case 48:	BattlAction=new CActHardAttack(200,1,SE_HARD_HIT);			break;
		case 49:	BattlAction=new CAct_185();									break;
		case 50:	BattlAction=new CActBokoboko();								break;
//魔法////////////////////////////////////////////////////////////////////////////////////
		case 53:	BattlAction=new CActMagicShoot();							break;
//炎魔法
		case 54:	BattlAction=new CAct_54();									break;
		case 55:	BattlAction=new CAct_55();									break;
		case 56:	BattlAction=new CActParamUp(0,0xff800000);					break;
		case 57:	BattlAction=new CAct_57();									break;
		case 58:	BattlAction=new CAct_58();									break;
		case 59:	BattlAction=new CAct_59();									break;
		case 60:	BattlAction=new CAct_60();									break;
		case 61:	BattlAction=new CAct_61();									break;
//氷魔法
		case 62:	BattlAction=new CAct_62();									break;
		case 63:	BattlAction=new CAct_63();									break;
		case 64:	BattlAction=new CActParamUp(1,0xff000080);					break;
		case 65:	BattlAction=new CAct_65();									break;
		case 66:	BattlAction=new CAct_66();									break;
		case 67:	BattlAction=new CAct_67();									break;
		case 68:	BattlAction=new CAct_68();									break;
		case 69:	BattlAction=new CAct_69();									break;
//風魔法
		case 70:	BattlAction=new CAct_70();									break;
		case 71:	BattlAction=new CAct_71();									break;
		case 72:	BattlAction=new CActParamUp(2,0xff008000);					break;
		case 73:	BattlAction=new CAct_73();									break;
		case 74:	BattlAction=new CAct_76();	break;	//ラピッドとスカウォを入れ替えた
		case 75:	BattlAction=new CAct_75();									break;
		case 76:	BattlAction=new CAct_74();	break;	//ラピッドとスカウォを入れ替えた
		case 77:	BattlAction=new CAct_77();									break;
//土魔法
		case 78:	BattlAction=new CAct_78();									break;
		case 79:	BattlAction=new CAct_79();									break;
		case 80:	BattlAction=new CActParamUp(3,0xff606000);					break;
		case 81:	BattlAction=new CAct_81();									break;
		case 82:	BattlAction=new CAct_82();									break;
		case 83:	BattlAction=new CAct_83();									break;
		case 84:	BattlAction=new CAct_84();									break;
		case 85:	BattlAction=new CAct_85();									break;
//無属性魔法
		case 86:	BattlAction=new CAct_86();									break;
		case 87:	BattlAction=new CAct_87();									break;
		case 88:	BattlAction=new CAct_88();									break;
		case 89:	BattlAction=new CActReflection(2);							break;
		case 91:	BattlAction=new CAct_91();									break;
//特殊行動////////////////////////////////////////////////////////////////////////////////
		case 92:	BattlAction=new CAct_92();									break;
		case 93:	BattlAction=new CAct_93();									break;
		case 94:	BattlAction=new CActCure(64,2,true,true);					break;
		case 95:	BattlAction=new CActRevival(35,true);						break;
		case 96:	BattlAction=new CActMagicField(FIELD_FIRE);					break;
		case 97:	BattlAction=new CActMagicField(FIELD_ICE);					break;
		case 98:	BattlAction=new CActMagicField(FIELD_WIND);					break;
		case 99:	BattlAction=new CActMagicField(FIELD_EARTH);				break;
		case 100:	BattlAction=new CActMagicField(FIELD_POW);					break;
		case 101:	BattlAction=new CActMagicField(FIELD_MAG);					break;
//ステータス異常系
		case 102:	BattlAction=new CActGus(0);									break;
		case 103:	BattlAction=new CAct_103();									break;
		case 104:	BattlAction=new CAct_104();									break;
		case 105:	BattlAction=new CAct_105();									break;
		case 106:	BattlAction=new CAct_106();									break;
		case 107:	BattlAction=new CAct_107();									break;
		case 108:	BattlAction=new CActGus(6);									break;
//キャラ専用
		case 111:	BattlAction=new CAct_111();									break;
		case 112:	BattlAction=new CAct_112();									break;
		case 114:	BattlAction=new CAct_114();									break;
		case 115:	BattlAction=new CAct_115();									break;
		case 116:	BattlAction=new CAct_116();									break;
		case 117:	BattlAction=new CActCallPet(0);								break;
		case 118:	BattlAction=new CActCallPet(1);								break;
		case 119:	BattlAction=new CActCallPet(2);								break;
		case 120:	BattlAction=new CActCallPet(3);								break;
		case 121:	BattlAction=new CActCallPet(4);								break;
		case 122:	BattlAction=new CActCallPet(5);								break;
		case 123:	BattlAction=new CActCallPet(6);								break;
		case 124:	BattlAction=new CActCallPet(7);								break;
		case 125:	BattlAction=new CActCallPet(8);								break;
		case 126:	BattlAction=new CActCallPet(9);								break;
//アイテム/////////////////////////////////////////////////////////////////////////
		case 128:	BattlAction=new CAct_128();									break;
		case 129:	BattlAction=new CAct_129();									break;
		case 130:	BattlAction=new CAct_130(iparama);							break;
		case 131:	BattlAction=new CAct_131(iparama);							break;
		case 132:	BattlAction=new CAct_132(iparama);							break;
		case 133:	BattlAction=new CActCure(128,4,false,false);				break;
		case 134:	BattlAction=new CActRevival(iparama,true);					break;
		case 135:	BattlAction=new CActBarrier(iparama);						break;
		case 136:	BattlAction=new CAct_136(iparama);							break;
		case 137:	BattlAction=new CAct_137(iparama);							break;
		case 138:	BattlAction=new CActReflection(iparama);					break;
		case 139:	BattlAction=new CAct_139();									break;
		case 140:	BattlAction=new CAct_140();									break;
//敵専用////////////////////////////////////////////////////////////////////////////////
		case 144:	BattlAction=new CActNormalAttack(SE_HIT,1,100);				break;
		case 145:	BattlAction=new CActNormalAttack(SE_CHOP,8,100);			break;
		case 146:	BattlAction=new CAct_146();									break;
		case 147:	BattlAction=new CAct_147();									break;
		case 148:	BattlAction=new CActRevival(100,false);						break;
		case 149:	BattlAction=new CActBarrier(3);								break;
		case 150:	BattlAction=new CAct_150();									break;
		case 151:	BattlAction=new CAct_151();									break;
		case 152:	BattlAction=new CAct_152();									break;
		case 153:	BattlAction=new CAct_153();									break;
		case 154:	BattlAction=new CAct_154();									break;
		case 155:	BattlAction=new CActReflection(1);							break;
		case 156:	BattlAction=new CAct_156();									break;
		case 157:	BattlAction=new CAct_157();									break;
		case 158:	BattlAction=new CAct_158();									break;
		case 159:	BattlAction=new CActCallEnemy(2,4,0);						break;
//ここだけ暴走攻撃//////////////////////////////
		case 160:	BattlAction=new CActNormalAttack(SE_BERSERK,1,150);			break;
		case 161:	BattlAction=new CActNormalAttack(SE_BERSERK,1,250);			break;
		case 162:	BattlAction=new CActNormalAttack(SE_BERSERK,1,400);			break;
		case 163:	BattlAction=new CActNormalAttack(SE_BERSERK,1,600);			break;
		case 164:	BattlAction=new CActNormalAttack(SE_BERSERK,1,800);			break;
//ここまで暴走攻撃//////////////////////////////
		case 167:	BattlAction=new CAct_87();									break;
		case 168:	BattlAction=new CAct_168();									break;
		case 169:	BattlAction=new CActCallEnemy(6,8,60);						break;
		case 170:	BattlAction=new CAct_170();									break;
		case 172:	BattlAction=new CAct_172();									break;
		case 173:	BattlAction=new CAct_173();									break;
		case 175:	BattlAction=new CActCallEnemy(4,4,20);						break;
		case 176:	BattlAction=new CActSong(2,0x00ff8080);						break;
		case 177:	BattlAction=new CActSong(5,0x00ff0000);						break;
		case 178:	BattlAction=new CActSong(7,0x00404040);						break;
		case 179:	BattlAction=new CActSong(100,0x0000ff00);					break;
		case 180:	BattlAction=new CAct_180();									break;
		case 181:	BattlAction=new CAct_181();									break;
		case 182:	BattlAction=new CAct_182();									break;
		case 183:	BattlAction=new CAct_183();									break;
		case 184:	BattlAction=new CAct_184();									break;
		case 186:	BattlAction=new CAct_186();									break;
		case 187:	BattlAction=new CAct_187();									break;
		case 188:	BattlAction=new CAct_188();									break;
		case 189:	BattlAction=new CAct_132(5000);								break;
		case 190:	BattlAction=new CAct_131(300);								break;
		case 191:	BattlAction=new CAct_137(3);								break;
		case 192:	BattlAction=new CAct_130(500);								break;
		case 193:	BattlAction=new CAct_136(5);								break;
		case 194:	BattlAction=new CActSkillDrain(0,3,0xff2020ff);				break;
		case 195:	BattlAction=new CActSkillDrain(1,1,0xff20ff20);				break;
		case 196:	BattlAction=new CActSkillDrain(2,1,0xffffff20);				break;
		case 197:	BattlAction=new CActSkillDrain(3,1,0xffff2020);				break;
		case 198:	BattlAction=new CAct_198();									break;
		case 199:	BattlAction=new CAct_199();									break;
//夢の卵専用
		case 200:	BattlAction=new CAct_200_203(63,false);						break;
		case 201:	BattlAction=new CAct_200_203(64,false);						break;
		case 202:	BattlAction=new CAct_200_203(65,false);						break;
		case 203:	BattlAction=new CAct_200_203(66,true);						break;
		case 204:	BattlAction=new CAct_204();									break;
//ここまで
		case 205:	BattlAction=new CAct_131(1000);								break;
		case 206:	BattlAction=new CAct_206();									break;
		case 207:	BattlAction=new CActSkillDrain(0,10,0xff2020ff);			break;
		case 208:	BattlAction=new CActSkillDrain(1,5,0xff20ff20);				break;
		case 209:	BattlAction=new CActDeathWord(7,0xff000040);				break;
		case 210:	BattlAction=new CActDeathWord(0,0xff40b040);				break;
		case 211:	BattlAction=new CActDeathWord(1,0xffb0b0ff);				break;
		case 212:	BattlAction=new CActDeathWord(3,0xffffff00);				break;
		case 213:	BattlAction=new CActDeathWord(4,0xff4040ff);				break;
		case 214:	BattlAction=new CActDeathWord(2,0xffff8080);				break;
		case 215:	BattlAction=new CActDeathWord(5,0xffff0000);				break;
		case 216:	BattlAction=new CActDeathWord(6,0xffff00ff);				break;
		case 217:	BattlAction=new CActBarrier(3);								break;
		case 218:	BattlAction=new CActReflection(5);							break;
		case 219:	BattlAction=new CActRevival(80,true);						break;
		case 223:	BattlAction=new CAct_223();							break;

		case 224:	BattlAction=new CActParamDown(0,0xffff0000);				break;
		case 225:	BattlAction=new CActParamDown(1,0xff0000ff);				break;
		case 226:	BattlAction=new CAct_174();									break;
		case 227:	BattlAction=new CAct_222();											break;
		case 228:	BattlAction=new CAct_171();									break;
	}

	if(!BattlAction){
		BattlAction=new CBattleAction();
		BattlAction->SetError();
	}

	BattlAction->Init(action,acharacter,mDMesID);
	mActionList.push_front(BattlAction);
	mCharacter[acharacter]->SetAction(-1);
	mDMesID++;
}

D3DCOLOR BattleFieldColor[FIELD_TYPE]={
	0x00000000,
	0x60ff4000,
	0x600000ff,
	0x6000ff00,
	0x60ffff00,
	0x60ff8000,
	0x608000ff
};