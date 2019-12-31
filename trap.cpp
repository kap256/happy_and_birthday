#include "menudata.h"
#include "flag.h"
#include "cursor.h"
#include "input.h"
#include "mapmode.h"
#include "changemode.h"


CBoxOpenMenu::CBoxOpenMenu(int param)
{
	mBox=param&0xffff;
	mCharacter	= param/0x10000;
	mTimer=0;
	mEnd=false;

	mX=0;
	mY=90;
	mAni=0;
	mTrapParamA=0;
	mTrapParamB=0;
	mExp=0;
	mLvUp=0;

	Flag.mTreasure[mBox]=true;
	Cursor.Escape();

	char filename[256];
	sprintf(filename,"data\\battlecharacter\\battlecharacter%03d",Flag.mParty[mCharacter]);
	DTest.LoadTexture(mCharacterTex,filename,0xff008080);
	DTest.LoadTexture(mBoxTex,"data\\mapobject\\mapobject100",0xff008080);

	mWindow.InitPlaneWindow(120,160,400,220);

	//ワナ判定	・カギの判定は、一つ前のメニューのタイマーで。
	if(TreasureData[mBox].mTrap>=0){
		int st;
		switch(TrapData[TreasureData[mBox].mTrap].mParam){
			case TRAP_LP:	st=Flag.mPlayerData[Flag.mParty[mCharacter]].GetMaxLP();	break;
			case TRAP_ATK:	st=Flag.mPlayerData[Flag.mParty[mCharacter]].GetAtk();		break;
			case TRAP_DEF:	st=Flag.mPlayerData[Flag.mParty[mCharacter]].GetDef();		break;
			case TRAP_MATK:	st=Flag.mPlayerData[Flag.mParty[mCharacter]].GetMagAtk();	break;
			case TRAP_MDEF:	st=Flag.mPlayerData[Flag.mParty[mCharacter]].GetMagDef();	break;
			case TRAP_SPD:	st=Flag.mPlayerData[Flag.mParty[mCharacter]].GetSpeed();	break;
			case TRAP_TECH:	st=Flag.mPlayerData[Flag.mParty[mCharacter]].GetTech();		break;
			case TRAP_IMP:	st=0;	break;
		}
		if(Flag.mPlayerData[Flag.mParty[mCharacter]].SearchASkill(283)){
			st*=2;
		}
		if(Flag.mPlayerData[Flag.mParty[mCharacter]].SearchASkill(360)){
			st=1;
		}

		const int &tlv=TreasureData[mBox].mTrapLv;
		st-=tlv;
		if(st > pInput->Rand(tlv)){
			mTrap=false;
		}else{
			mTrap=true;
			switch(TreasureData[mBox].mTrap){
				case 0:		TrapInit_0(10);		break;
				case 1:		TrapInit_0(50);		break;
				case 2:		TrapInit_2(5);		break;
				case 3:		TrapInit_2(30);		break;
				case 4:		TrapInit_4();		break;
				case 5:		TrapInit_5();		break;
				case 6:		TrapInit_6(0);		break;
				case 7:		TrapInit_6(1);		break;
				case 8:		TrapInit_8();		break;
				case 9:		TrapInit_9();		break;
				case 10:	TrapInit_10(4);		break;
				case 11:	TrapInit_10(2);		break;
			}
		}
	}else{
		mTrap=false;
	}

	for(int i=0;i<TreasureData[mBox].mItemNo;i++){
		Flag.SetTempItem(TreasureData[mBox].mItem[i]);
	}
	if(mTrap && !TrapData[TreasureData[mBox].mTrap].mGetItem){
		Flag.DropTempItem(false);
	}

	mBoxX=BOX_X;
	mBoxY=BOX_Y;
}

bool CBoxOpenMenu::OnTimer()
{
	bool timer=CMenuData::OnTimer();

	mWindow.OnTimer();

	if(!timer)	return false;
	mTimer++;

	if(!mEnd){
		if(START_TIME < mTimer && mTimer < START_TIME+MOVE_CHARACTER_TIME){
			mX+=10;
			mAni=2+(mTimer/2)%2;
		}else if(START_TIME+MOVE_CHARACTER_TIME < mTimer && mTimer < OPEN_TIME){
			mAni=6;
		}else if(mTimer== OPEN_TIME){
			PlayWave(SE_OPEN_BOX);
		}else if(mTimer>OPEN_TIME){
			if(mTrap){
				switch(TreasureData[mBox].mTrap){
					case 0:	
					case 1:		mEnd=TrapTimer_0();	break;
					case 2:	
					case 3:		mEnd=TrapTimer_2();	break;
					case 4:		mEnd=TrapTimer_4();	break;
					case 5:		mEnd=TrapTimer_5();	break;
					case 6:		mEnd=TrapTimer_6();	break;
					case 7:		mEnd=TrapTimer_6();	break;
					case 8:		mEnd=TrapTimer_8();	break;
					case 9:		mEnd=TrapTimer_9();	break;
					case 10:	mEnd=TrapTimer_10();break;
					case 11:	mEnd=TrapTimer_10();break;
					default:	mEnd=true;			break;
				}
				if(mEnd){
					PlayWave(SE_TRAP_MISS);
				}
			}else{
				if(mTimer>OPEN_TIME+20){
					CPlayerData &pdata=Flag.mPlayerData[Flag.mParty[mCharacter]];
					int plv=pdata.GetLv();
					int tlv=TreasureData[mBox].mExpLv;
					mExp=TRESURE_EXP*(900+pInput->Rand(200))/1000;
					mExp=int(CPlayerData::CulcExp(mExp,plv,tlv,true));
					mExp+=TRESURE_EXP_MIN+0.00001;
					double d=mExp;
					if(pdata.SetExp(d,mExp)){
						mLvUp=1;
						PlayWave(SE_LV_UP);
					}else{
						PlayWave(SE_TRAP_OK);
					}
					mEnd=true;
				}
			}
		}
	}else{
		if(mLvUp>0)	mLvUp++;
		if(pInput->GetKey(4)==1){
			if(TreasureData[mBox].mTrap <0  || (!mTrap) || TrapData[TreasureData[mBox].mTrap].mGetItem){
				SetMenu(OverItemMenu,0);
			}else{
				MenuData.AllDelete();
			}
		}
	}

	return true;
}
void CBoxOpenMenu::OnDraw(){
	mWindow.OnDraw();
	if(mTimer<OPEN_TIME){
		mWindow.DrawTexture(&mBoxTex,0,0,0,64,64,mBoxX,mBoxY,128,128);
	}else{
		mWindow.DrawTexture(&mBoxTex,0,0,64,64,64,mBoxX,mBoxY,128,128);
	}

	int tx=mAni%4*64;
	int ty=mAni/4*64;
	mWindow.DrawTexture(&mCharacterTex,TURN_HORIZONTAL,tx,ty,64,64,mX,mY,128,128);

	if(mTrap && mTimer>=OPEN_TIME){
		switch(TreasureData[mBox].mTrap){
			case 0:		TrapDraw_0(0);			break;
			case 1:		TrapDraw_0(1);			break;
			case 2:		TrapDraw_2(320);		break;
			case 3:		TrapDraw_2(640);		break;
			case 4:		TrapDraw_4();			break;
			case 5:		TrapDraw_5();			break;
			case 6:		TrapDraw_6(0xff000080);	break;
			case 7:		TrapDraw_6(0xff800080);	break;
			case 8:		TrapDraw_8();			break;
			case 9:		TrapDraw_9();			break;
			case 10:	TrapDraw_10();			break;
			case 11:	TrapDraw_10();			break;
		}
	}

	if(mEnd){
		int y=0;
		if(TreasureData[mBox].mTrap>=0){
			if(mTrap){
				mWindow.DrawText(0,"トラップ発動！",0,0,0xffff0000);
				mWindow.DrawText(0,TrapData[TreasureData[mBox].mTrap].mName,100,0);
				mWindow.DrawText(0,TrapData[TreasureData[mBox].mTrap].mHelp,20,25);
				y=70;
				if(!TrapData[TreasureData[mBox].mTrap].mGetItem){
					mWindow.DrawText(0,"アイテムを手に入れることは出来ませんでした。",0,y,0xffff0000);
					y+=30;
				}
			}else{
				mWindow.DrawText(0,"トラップ解除に成功しました！",20,20,0xff0000ff);
				y=50;
			}
		}else{
			mWindow.DrawText(0,"宝箱を開けました！",20,20,0xff0000ff);
			y=50;
		}
		mWindow.DrawText(0,"獲得経験値",40,y,0xff008000);
		char text[128];
		sprintf(text,"%d",(int)mExp);
		mWindow.DrawText(0,text,120,y+16,0xff000000);
		if(mLvUp>0){
			int y=150;
			if(mLvUp<10){
				y-=int(sin(mLvUp*0.3141592)*32);
			}
			mWindow.DrawTexture(&SystemTex,0,0,112,64,32,180,y);
		}
	}
}
/////////////////////////////////////////////////////////////////////////////////
void	CBoxOpenMenu::TrapInit_0(int pow)
{
	DTest.LoadTexture(mTrapTex,"data\\mapobject\\trap00",0xff008080);
	mTrapParamA=BOX_X+20;
	mTrapParamB=Flag.mPlayerData[Flag.mParty[mCharacter]].GetLP()*pow/100;
	if(Flag.mPlayerData[Flag.mParty[mCharacter]].SearchASkill(286)){
		mTrapParamB/=2;
	}
}
bool	CBoxOpenMenu::TrapTimer_0()
{
	if(mTimer>OPEN_TIME+50){
		Flag.mPlayerData[Flag.mParty[mCharacter]].LostLP(mTrapParamB);
		return true;
	}else if (mTimer>OPEN_TIME+30){
		mAni=8;
		mX+=(mTimer%2)*8-4;
		mTrapParamA-=64;
	}else if (mTimer==OPEN_TIME+30){
		PlayWave(SE_SLASH);
	}
	return false;
}
void	CBoxOpenMenu::TrapDraw_0(int bow)
{
	if(mTimer<OPEN_TIME+30){
		mWindow.DrawTexture(&mTrapTex,0,0,0,32,16,BOX_X+20,BOX_Y+60,64,32);
	}else{
		mWindow.DrawTexture(&mTrapTex,0,32,0,32,16,BOX_X+20,BOX_Y+60,64,32);
	}
	mWindow.DrawTexture(&mTrapTex,0,bow*32,16,32,16,mTrapParamA,BOX_Y+60,64,32);
}

/////////////////////////////////////////////////////////////////////////////////
void	CBoxOpenMenu::TrapInit_2(int pow)
{
	DTest.LoadTexture(mTrapTex,"data\\mapobject\\trap02",0xff008080);
	mTrapParamA=pow;
	if(Flag.mPlayerData[Flag.mParty[mCharacter]].SearchASkill(286)){
		mTrapParamA/=2;
	}
}
bool	CBoxOpenMenu::TrapTimer_2()
{
	if(mTimer>OPEN_TIME+50){
		for(int i=0;i<MAX_PARTY_NUMBER;i++){
			int& c=Flag.mParty[i];
			if(c>=0){
				int lost=Flag.mPlayerData[c].GetLP()*mTrapParamA/100;
				Flag.mPlayerData[c].LostLP(lost);
			}
		}
		return true;
	}else if (mTimer>OPEN_TIME+30){
		mAni=8;
		mX+=(mTimer%2)*8-4;
	}else if (mTimer==OPEN_TIME+30){
		PlayWave(SE_EXPLOSION);
	}
	return false;
}
void	CBoxOpenMenu::TrapDraw_2(int size)
{
	const int TIME = 16;
	if(mTimer>OPEN_TIME+30){
		size=(mTimer-(OPEN_TIME+30))*size/TIME;
		int c= TIME-(mTimer-(OPEN_TIME+30));
		if(c<0)	c=0;
		D3DCOLOR color=(0xffffff)+((0xff000000/TIME*c)&0xff000000);
		mWindow.DrawTexture(&mTrapTex,0,0,0,128,128,BOX_X+64-size,BOX_Y+64-size,size*2,size*2,color);
	}
}

///////////////////////////////////////////////////
void	CBoxOpenMenu::TrapInit_4(void)
{
}
bool	CBoxOpenMenu::TrapTimer_4()
{
	if(mTimer>OPEN_TIME+50){
		return true;
	}else if (mTimer>OPEN_TIME+30){
		mBoxY-=30;
	}else if (mTimer==OPEN_TIME+30){
		PlayWave(SE_WARP);
	}
	return false;
}
void	CBoxOpenMenu::TrapDraw_4(void)
{
}

///////////////////////////////////////////////////
void	CBoxOpenMenu::TrapInit_5(void)
{
		ChangeMapInfo.ChangeMap(-1,0,0,0,0);
}
bool	CBoxOpenMenu::TrapTimer_5()
{
	if(mTimer>OPEN_TIME+50){
		return true;
	}else if (mTimer>OPEN_TIME+30){
		mAni=0;
		mY-=30;
	}else if (mTimer==OPEN_TIME+30){
		PlayWave(SE_WARP);
	}
	return false;
}
void	CBoxOpenMenu::TrapDraw_5(void)
{
}

///////////////////////////////////////////////////

void	CBoxOpenMenu::TrapInit_6(int type)
{
	DTest.LoadTexture(mTrapTex,"data\\mapobject\\trap06",0xff008080);
	mTrapParamA=0;
	mTrapParamB=0;
	switch(type){
		case 0:
			MapMode.SetTrapSlow(true);		break;
		case 1:
			MapMode.SetTrapReverse(true);	break;
	}
}
bool	CBoxOpenMenu::TrapTimer_6()
{
	if(mTimer>OPEN_TIME+80){
		return true;
	}else if (mTimer>OPEN_TIME+50){
		mAni=8;
		mX+=(mTimer%2)*8-4;
		mTrapParamA-=64;
	}else if(mTimer==OPEN_TIME+30){
		PlayWave(SE_GUS);
	}
	return false;
}
void	CBoxOpenMenu::TrapDraw_6(D3DCOLOR color)
{
	if(mTimer>OPEN_TIME+30){
		int size=(mTimer-(OPEN_TIME+30))*10;
		int c= 64-(mTimer-(OPEN_TIME+30));
		if(c<0)	c=0;
		color=(color & 0xffffff)+(0x04*c)*0x01000000;
		DTest.SetBlend(BLEND_ADD);
		mWindow.DrawTexture(&mTrapTex,0,0,0,64,64,BOX_X+64-size,BOX_Y+64-size,size*2,size*2,color);
		DTest.SetBlend(BLEND_STANDARD);
	}
}
/////////////////////////////////////////////////////////////////////////////////
void	CBoxOpenMenu::TrapInit_8()
{
	DTest.LoadTexture(mTrapTex,"data\\mapobject\\trap01",0xff008080);
	mTrapParamA=BOX_X+20;
	mTrapParamB=0;
	MapMode.SetTrapStop(6*60);	
}
bool	CBoxOpenMenu::TrapTimer_8()
{
	if(mTimer>OPEN_TIME+40){
		return true;
	}else if (mTimer>OPEN_TIME+20){
		mAni=8;
		mX+=(mTimer%2)*8-4;
		if(mTimer<OPEN_TIME+24){
			mTrapParamA-=16;
		}
	}else if (mTimer==OPEN_TIME+20){
		PlayWave(SE_SLASH);
	}
	return false;
}
void	CBoxOpenMenu::TrapDraw_8()
{
	int tx=0;
	if(mTimer > OPEN_TIME+20){
		tx=32;
	}

	mWindow.DrawTexture(&mTrapTex,0,tx,0,32,32,mTrapParamA,BOX_Y+30,64,64);
}
/////////////////////////////////////////////////////////////////////////////////
void	CBoxOpenMenu::TrapInit_9()
{
	DTest.LoadTexture(mTrapTex,"data\\mapobject\\trap03",0xff008080);
	mTrapParamA=BOX_X+20;
	mTrapParamB=BOX_Y+30;
	Flag.mMoney /= 2;
}
bool	CBoxOpenMenu::TrapTimer_9()
{
	if(mTimer>OPEN_TIME+50){
		return true;
	}else{
		mTrapParamB += mTimer-OPEN_TIME-6;
		if(mTrapParamB > BOX_Y+80)	mTrapParamB=BOX_Y+80;
		mTrapParamA -= 1;
		if(mTimer > OPEN_TIME+16){
			mTrapParamA -=mTimer-OPEN_TIME-16;
		}

	}
	
	if (mTimer==OPEN_TIME+16){
		PlayWave(SE_BUY_ITEM);
	}
	return false;
}
void	CBoxOpenMenu::TrapDraw_9()
{
	if(mTimer > OPEN_TIME+16){
		mWindow.DrawTexture(&mTrapTex,0,16,0,16,16,mTrapParamA-8,mTrapParamB,32,32);
	}

	mWindow.DrawTexture(&mTrapTex,0,0,0,16,16,mTrapParamA,mTrapParamB,32,32);
}

///////////////////////////////////////////////////
void	CBoxOpenMenu::TrapInit_10(int div)
{
	mTrapParamA=div;
}
bool	CBoxOpenMenu::TrapTimer_10()
{
	if(mTimer>OPEN_TIME+50){
		CPlayerData& PData=Flag.mPlayerData[Flag.mParty[mCharacter]];
		for(int i=0;i<MAX_CSKILL_LEVEL;i++){
			int sp=PData.GetSP(i);
			PData.LostSP(i,sp/mTrapParamA);
		}
		return true;
	}else if (mTimer>OPEN_TIME+30){
		mAni=8;
		mX+=(mTimer%2)*4-2;
		if(mTimer%2==0){
			PlayWave(SE_ELECTRIC);
		}
	}
	return false;
}
void	CBoxOpenMenu::TrapDraw_10(void)
{
	if (OPEN_TIME+30<mTimer && mTimer<OPEN_TIME+50){
		if(mTimer%3==0){
			DTest.SetBlend(BLEND_SOLID);
			int tx=mAni%4*64;
			int ty=mAni/4*64;
			mWindow.DrawTexture(&mCharacterTex,TURN_HORIZONTAL,tx,ty,64,64,mX,mY,128,128);
			DTest.SetBlend(BLEND_STANDARD);
		}
	}
}

///////////////////////////////////////////////////
