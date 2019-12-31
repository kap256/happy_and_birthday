
#include "option.h"
#include "main.h"
#include "mapmode.h"
#include "battlemode.h"
#include "village.h"
#include "input.h"
#include "gamewindow.h"
#include "cursor.h"
#include "menudata.h"
#include "changemode.h"
#include "itemdata.h"
#include "flag.h"
#include "title.h"
#include "resource.h"
#include "ending.h"
#include "bgmdata.h"
#include "trial_demo.h"


#ifndef _NO_USE_GGSOUND_
#include "GGSound.h"
#endif

#ifdef _DEBUG
void DebugOutput();
bool DOutput=true;
#endif

CFlag			Flag;
CGlobalFlag		GlobalFlag;

CDTTexture		SystemTex;
CDTTexture		StatusTex[STATUS_TEX_NUMBER];
static CInput	Input;

CInput*			pInput=&Input;
CCursor			Cursor;
CMenuData		MenuData;

CMapMode		MapMode;
CBattleMode		BattleMode;
CVillageMode	VillageMode;
CTitleMode		TitleMode;
CEndingMode		EndingMode;
#ifdef _TRIAL_VERSION_
CTrialDemo		TrialDemo;
#endif

CEncountInfo	EncountInfo;
CReturnMapInfo	ReturnMapInfo;
CChangeMapInfo	ChangeMapInfo;

int	GameMode=GMODE_TITLE;

CDTTexture	gBackGround;


void StartUp()
{
	EncountInfo.mMember=8;

#ifdef	_TRIAL_VERSION_
	DTest.SetWindowTitle("Happy & Birthday Å`Trial VersionÅ`");
#else
	DTest.SetWindowTitle("Happy & Birthday");
#endif
	DTest.SetIcon((LPCSTR) IDI_ICON1);
	DTest.SetWindowSize(640,480);
	

	GlobalFlag.Load();

	int i;
	for(i=0;i<MAX_EQUIPMENT_NUMBER;i++){
		AllItem[i]=&EquipmentData[i];
	}
	for(i=0;i<MAX_USE_ITEM_NUMBER;i++){
		AllItem[i+ID_UI]=&UseItemData[i];
	}
	for(i=0;i<MAX_SEED_NUMBER;i++){
		AllItem[i+ID_SD]=&SeedData[i];
	}

	AllItem[MAX_ITEM_NUMBER]=&NoneItemData;

	Flag.Reset();
#ifdef	_LOCK_FULL_SCREEN_
	DTest.FullScreen(_FULL_SCREEN_);
#endif
}

void AfterStartUp()
{
	DTest.CreateFont(0,15,"ÇlÇr ÇoÉSÉVÉbÉN",1000);
	DTest.CreateFont(1,10,"ÇlÇr ÇoÉSÉVÉbÉN",1000);
	DTest.CreateFont(2,12,"ÇlÇr ÇoÉSÉVÉbÉN",1000);
	DTest.CreateFont(3,15,"ÇlÇr ÇoÉSÉVÉbÉN",0);
	DTest.CreateFont(4,26,"ÇlÇr Çoñæí©",1000);
	DTest.CreateFont(5,15,"ÇlÇr ñæí©",1000);
	DTest.CreateFont(6,22,"ÇlÇr ÇoÉSÉVÉbÉN",1000);
	DTest.CreateFont(7,48,"ÇlÇr ñæí©",1000);
	DTest.CreateFont(8,14,"ÇlÇr ÇoÉSÉVÉbÉN",1000);
	DTest.CreateFont(9,32,"ÇlÇr Çoñæí©",1000);
	DTest.CreateFont(10,17,"ÇlÇr ÇoÉSÉVÉbÉN",1000);
	DTest.CreateFont(11,256,"ÇlÇr Çoñæí©",1000);
	DTest.CreateFont(12,56,"ÇlÇr ñæí©",1000);

	DTest.LoadTexture(SystemTex,"data\\otherimg\\system",0xff008080);
	for(int i=0;i<STATUS_TEX_NUMBER;i++){
		char fname[128];
		sprintf(fname,"data\\otherimg\\st%d",i);
		DTest.LoadTexture(StatusTex[i],fname,0xff008080);
	}
	DTest.CreateTexture(gBackGround,640,480);

	for(int i=0;i<ITEM_TEX_NUMBER;i++){
		char fname[256];
		sprintf(fname,"data\\itemicon\\icon%d",i);
		DTest.LoadTexture(ItemTex[i],fname,0xff008080);
	}
	
#ifndef _NO_USE_GGSOUND_

	#ifdef _DEBUG
		GGSound_Initialize(NULL,"GGLog.txt");
	#else
		GGSound_Initialize(NULL);
	#endif

	for(int n=0;n<NumberOfSound;n++){
		char file[256];
		sprintf(file,"data\\se\\%02d.wav",n);
		GGSound_Load(n+8,file,false);
		GGSound_SetVolume(n+8,SOUND_VOLUME);
	}

#endif

	MapMode.AfterStartUp();
	BattleMode.AfterStartUp();
//	VillageMode.Init();
	TitleMode.Init();
	
	DTest.SetFPS(60);
}

void OnTimer()
{
	if(DTest.GetKey(DIK_ESCAPE)==1){
		if(/*!MenuData.SearchMenu(typeid( CKeyConfigMenu )) && */!MenuData.SearchMenu(typeid( CExitYesNo ))){
			if(GameMode==GMODE_TITLE){
				DTest.Exit();
			}else{
				MenuData.SetMenu(ExitYesNo,0);
			}
			return;
		}
	}
	
#ifdef _DEBUG
	if(DTest.GetKey(DIK_Q)==1)		DTest.FullScreen(true);
	if(DTest.GetKey(DIK_W)==1)		DTest.FullScreen(false);
#endif

	DTest.DrawSkip(false);
#ifndef _NO_USE_GGSOUND_
	GGSound_CallByFrame();
#endif
	Input.Reset();

	Flag.mPlayTime++;

	if(MenuData.OnTimer()){
		EncountInfo.mTimer++;
		if(EncountInfo.mEncount){
			if(EncountInfo.mTimer>ENCOUNT_TIME){
				BattleMode.LoadBattle();
				EncountInfo.mEncount=false;
				GameMode=GMODE_BATTLE;
			}
		}else if(ReturnMapInfo.mReturnMap){
			ReturnMapInfo.mTimer++;
			if(ReturnMapInfo.mTimer==RETURN_MAP_TIME){
				GameMode=GMODE_MAP;
				MapMode.OnTimer();
			}else if(ReturnMapInfo.mTimer>RETURN_MAP_TIME+RETURN_MAP_TIME_AFTER){
				ReturnMapInfo.mReturnMap=false;
			}
		}else if(ChangeMapInfo.mChangeMap){
			MapMode.BGMWait();
			ChangeMapInfo.mTimer++;
			if(MapMode.GetEventSkip() && ChangeMapInfo.mTimer<RETURN_MAP_TIME){
				ChangeMapInfo.mTimer=RETURN_MAP_TIME;
			}
			if(ChangeMapInfo.mTimer==RETURN_MAP_TIME){
				if(ChangeMapInfo.mNo>=0){
					if(!ChangeMapInfo.mLoad){
						MapMode.LoadMap(ChangeMapInfo.mNo,
										ChangeMapInfo.mX,
										ChangeMapInfo.mY,
										ChangeMapInfo.mZ,
										ChangeMapInfo.mDir);
						GameMode=GMODE_MAP;
						MenuData.SetDrawBG(true);
					}
				}else{
					MapMode.EndEventSkip();
					ChangeMapInfo.mChangeMap=false;
					ChangeMapInfo.mTimer=255;
					switch(ChangeMapInfo.mNo){
						case -1:
							GameMode=GMODE_VILLAGE;
							VillageMode.Init();
							break;
						case -2:
							GameMode=GMODE_TITLE;
							TitleMode.Init();
							break;
				#ifdef _TRIAL_VERSION_
						case -3:
							GameMode=GMODE_TRIAL;
							TrialDemo.Init();
							break;
				#endif
					}
				}
			}else if(ChangeMapInfo.mTimer>RETURN_MAP_TIME+RETURN_MAP_TIME_AFTER){
				ChangeMapInfo.mChangeMap=false;
			}
			if(ChangeMapInfo.mLoad){
				if(ChangeMapInfo.mNo>=0){
				MapMode.LoadMap(ChangeMapInfo.mNo,
								ChangeMapInfo.mX,
								ChangeMapInfo.mY,
								ChangeMapInfo.mZ,
								ChangeMapInfo.mDir);
				GameMode=GMODE_MAP;
				MenuData.SetDrawBG(true);
				}else{
					MapMode.EndEventSkip();
					ChangeMapInfo.mChangeMap=false;
					ChangeMapInfo.mTimer=255;
					switch(ChangeMapInfo.mNo){
						case -1:
							GameMode=GMODE_VILLAGE;
							VillageMode.Init();
							break;
						case -2:
							GameMode=GMODE_TITLE;
							TitleMode.Init();
							break;
					}
				}
			}
		}else{
			switch(GameMode){
				case GMODE_MAP:
#ifndef _DEBUG
					DTest.DrawSkip(!MapMode.GetEventSkip());
#endif
					Cursor.Escape();
					MapMode.BGMWait();
					MapMode.OnTimer();		break;
				case GMODE_BATTLE:
#ifndef _DEBUG
					//DTest.DrawSkip(true);
#endif
					BattleMode.OnTimer();	break;
				case GMODE_VILLAGE:
					VillageMode.OnTimer();	break;
				case GMODE_TITLE:
					TitleMode.OnTimer();	break;
				case GMODE_ENDING:
					EndingMode.OnTimer();	break;
		#ifdef _TRIAL_VERSION_
				case GMODE_TRIAL:
					TrialDemo.OnTimer();	break;
		#endif
			}
		}
	}

	Cursor.OnTimer();

}

void OnDraw()
{
	
#ifdef _DEBUG
	if(DOutput){
		DebugOutput();
		DOutput=false;
	}
#endif
	static	bool	sStop=false;

	int blacktime=0xffff;

	bool stop=false;
	if(MenuData.GetMenuOpend())		stop=true;
	if(EncountInfo.mEncount)		stop=true;
	if(ReturnMapInfo.mReturnMap){
		if(ReturnMapInfo.mTimer!=RETURN_MAP_TIME)	stop=true;
		blacktime=ReturnMapInfo.mTimer;
	}
	if(ChangeMapInfo.mChangeMap){
		if(ChangeMapInfo.mTimer!=RETURN_MAP_TIME)	stop=true;
		blacktime=ChangeMapInfo.mTimer;
	}

	if(stop){
		if(!sStop){
			DTest.SetRenderTarget(&gBackGround);
			DTest.Clear();
		}
	}else{
		sStop=false;
	}

	if(!sStop){
		switch(GameMode){
			case GMODE_MAP:
				MapMode.OnDraw();		break;
			case GMODE_BATTLE:
				BattleMode.OnDraw();	break;
			case GMODE_VILLAGE:
				VillageMode.OnDraw();	break;
			case GMODE_TITLE:
				TitleMode.OnDraw();		break;
			case GMODE_ENDING:
				EndingMode.OnDraw();	break;
		#ifdef _TRIAL_VERSION_
			case GMODE_TRIAL:
				TrialDemo.OnDraw();		break;
		#endif
		}
		if(stop){
			sStop=true;
			DTest.SetRenderTarget(NULL);
		}
	}

	int normalbrackout=0;

	if(	EncountInfo.mTimer<ENCOUNT_TIME+ENCOUNT_TIME_AFTER
	&&	EncountInfo.mTimer>ENCOUNT_TIME){
		normalbrackout=(ENCOUNT_TIME+ENCOUNT_TIME_AFTER-EncountInfo.mTimer)*0xff/ENCOUNT_TIME_AFTER;

	}else	if(	blacktime<=RETURN_MAP_TIME){
		normalbrackout=blacktime*0xff/RETURN_MAP_TIME;

	}else	if(	blacktime<RETURN_MAP_TIME+RETURN_MAP_TIME_AFTER
	&&	blacktime>RETURN_MAP_TIME){
		normalbrackout=(RETURN_MAP_TIME+RETURN_MAP_TIME_AFTER-blacktime)*0xff/RETURN_MAP_TIME_AFTER;
	}

	if(stop){
		DTest.SetBlend(BLEND_NONE);
		if(EncountInfo.mEncount){
			if(EncountInfo.mTimer<ENCOUNT_TIME){
				int size=EncountInfo.mTimer*(120/ENCOUNT_TIME);
				if(size>80)	size=80;
				D3DCOLOR color=0xffffffff;
				if(EncountInfo.mTimer>ENCOUNT_TIME/2){
					color=(ENCOUNT_TIME-EncountInfo.mTimer)*0xff/(ENCOUNT_TIME/2)*0x010101+0xff000000;
				}
				DTest.DrawTexture(gBackGround,0,size*4,size*3,640-size*8,480-size*6,0,0,640,480,color);
			}
		}else{
			DTest.DrawTexture(gBackGround,0,0,0,640,480,0,0);
		}
		DTest.SetBlend(BLEND_STANDARD);
	}

	if(normalbrackout>0){
		DTest.DrawBox(0,0,640,480,normalbrackout*0x1000000);
	}

	MenuData.OnDraw();
	if(GameMode!=GMODE_TITLE || !TitleMode.ShowCursor()){
		Cursor.OnDraw();
	}
}
void End()
{
	GlobalFlag.Save();
}
/////////////////////////////////////////////////////////////////////////
void PlayWave(int sound)
{
#ifndef _PLAY_SOUND_
	return;
#endif
	if(sound<0)	return;
	if(Flag.mSE){
#ifndef _NO_USE_GGSOUND_
		GGSound_Play(sound+8,false);
#endif
	}
}

void PlayBGM(int sound,bool force)
{
#ifndef _PLAY_SOUND_
	return;
#endif
	if(sound<=0 || BGM_MAX<=sound){
#ifndef _NO_USE_GGSOUND_
		GGSound_Stop(0);
#endif
	}else{
		GlobalFlag.mSound[sound]=true;
		if(Flag.mBGMNo!=sound && (Flag.mBGM || force)){
			char file[256];
			sprintf(file,"data\\bgm\\%02d.ogg",sound);
#ifdef _SET_BGM_
			sprintf(file,"data\\bgm\\%02d.ogg",_SET_BGM_);
#endif	
			
#ifndef _NO_USE_GGSOUND_
			GGSound_Unload(0);

			int loopstart=BGMData[sound].mLoopStart;
			bool loop=true;
			if(loopstart<0){
				loopstart=0;
				loop=false;
			}
			GGSound_Load(0,file,true,loopstart);
			//CBufferManager::Instance()->GetBuffer( 0 )->SetVolumeDB( SOUND_VOLUME );
			GGSound_Play(0,loop);
			GGSound_SetVolume(0,SOUND_VOLUME);
#endif
		}
	}
	Flag.mBGMNo=sound;
}
void SetBGMFadeOut(int time)
{
//	GGSound_SetFadeOut(0,GGSound_GetVolume(0),0,(unsigned long)(time*16.66666));
	
#ifndef _NO_USE_GGSOUND_
	GGSound_SetFadeOut(0,GGSound_GetVolume(0),0,(unsigned long)(time*16.66666));
#endif
	Flag.mBGMNo=0;
}
void SetBGMFade(int time,int vol)
{
#ifndef _NO_USE_GGSOUND_
	if(time==0){
		GGSound_SetVolume(0,SOUND_VOLUME*vol/100);
	}else{
		GGSound_SetFade(0,GGSound_GetVolume(0),SOUND_VOLUME*vol/100,(unsigned long)(time*16.66666));
	}
#endif
}
//////////////////////////////////////////////////////////////////////////
int	PriorityRand(int* pri,int no,int and,int min)
{
	if(no<=0)	return -1;

	int max=0;
	for(int i=0;i<no;i++){
		if((pri[i]&and)>max)	max=pri[i]&and;
	}

	max-=min;

	int* best=new int[no];
	int bestno=0;
	for(int i=0;i<no;i++){
		if((pri[i]&and)>=max){
			best[bestno]=i;
			bestno++;
		}
	}

	int rnd=pInput->Rand(bestno);
	int ret=best[rnd];

	delete[] best;
	return ret;
}
D3DCOLOR	BlendColor(D3DCOLOR color1,D3DCOLOR color2)
{
	unsigned char c[4];
	unsigned char *c1,*c2;
	c1=(unsigned char *)(&color1);
	c2=(unsigned char *)(&color2);
	for(int i=0;i<4;i++){
		c[i]=c1[i]*c2[i]/0xff;
	}

	D3DCOLOR *ret;
	ret=(D3DCOLOR *)(c);
	return *ret;
}
