#include "ending.h"
#include "input.h"
#include "cursor.h"
#include "changemode.h"
#include "menudata.h"
#include "mapmode.h"

CEndingMode::CEndingMode()
{
}
CEndingMode::~CEndingMode()
{
}

void CEndingMode::Init(int mode)
{
	mTimer=-1;
	mMode=mode;
}

void CEndingMode::ChangeMode(int mode)
{
	mTimer=-1;
	mMode=mode;
	
	if((Flag.mClearData & 0x3) == 0x3){
		if(mMode>=4){
			EndEnding(true);
		}else	if(mMode>=2){
			EndEnding();
		}
	}else if(mMode>=3){
		EndEnding(true);
	}
}
void CEndingMode::OnTimer()
{
	mTimer++;
}
void CEndingMode::EndEnding(bool force)
{
	if((Flag.mClearData & 0x3) == 0x3 && (!force)){
		MapMode.EndEventSkip();
		ChangeMapInfo.ChangeMap(648,2,2,0,0);
	}else{
		ChangeMapInfo.ChangeMap(-2,0,0,0,0);
		MenuData.SetMenu(DiaryMenu,0);
	}
}
void CEndingMode::OnDraw()
{
	bool ret=false;
	switch(mMode){
		case 0:
			ret=DrawTitle();	break;
		case 1:
			ret=DrawStaff();	break;
		default:
			ret=DrawEnd();		break;
	}

	if(!ret){
		ChangeMode(mMode+1);
	}
}
bool CEndingMode::DrawTitle()
{
	static const int BLACK_TIME		= 8;
	static const int WHITE_UP_TIME	= 16 + BLACK_TIME;
	static const int WHITE_TIME		= 16 + WHITE_UP_TIME;
	static const int WHITE_DOWN_TIME= 64 + WHITE_TIME;
	static const int BLACK_UP_TIME	= 64 + WHITE_DOWN_TIME;
	static const int END_TIME		= 16 + BLACK_UP_TIME;

	if(mTimer==1){
		DTest.LoadTexture(mTex,"data\\otherimg\\title");
	}
	if(mTimer<BLACK_TIME){
		return true;
	}
	
	if(mTimer<WHITE_UP_TIME){
		int t=mTimer-BLACK_TIME;
		int c=t*0xff/(WHITE_UP_TIME-BLACK_TIME);
		DTest.DrawBox(0,0,640,480,c*0x01000000 + 0xffffff);
	}else{
		DTest.DrawTexture(mTex,0,0,0,640,480,0,0,640,480);

		if(mTimer<WHITE_TIME){
			DTest.DrawBox(0,0,640,480,0xffffffff);
		}else	if(mTimer<WHITE_DOWN_TIME){
			int t=WHITE_DOWN_TIME-mTimer;
			int c=t*0xff/(WHITE_DOWN_TIME-WHITE_TIME);
			DTest.DrawBox(0,0,640,480,c*0x01000000 + 0xffffff);
		}else	if(mTimer<BLACK_UP_TIME){
			int t=mTimer-WHITE_DOWN_TIME;
			int c=t*0xff/(BLACK_UP_TIME-WHITE_DOWN_TIME);
			DTest.DrawBox(0,0,640,480,c<<24);
		}else{
			DTest.DrawBox(0,0,640,480,0xff000000);
		}
	}
	if(mTimer>=END_TIME){
		return false;
	}
	return true;
}
bool CEndingMode::DrawStaff()
{
		const int FADE_IN_TIME	= 48;
		const int JOB_TIME		= 96;
			const int JOB_LETTER_TIME	=4;
			const int JOB_LETTER_NO		=8;
		const int NAME_TIME		= 128;
			const int NAME_FADE		= 32;
		const int FADE_OUT_TIME = 48;
		const int INTERVAL		= 64;
	const int SHOW_TIME		=FADE_IN_TIME+JOB_TIME+NAME_TIME+FADE_OUT_TIME+INTERVAL;

	const int SHOW_NO		=9;
	const int END_TIME		=SHOW_TIME*SHOW_NO;

	static const CStaffInfo sinfo[SHOW_NO]={
		//改行前には、必ず空白を入れること！
		{
			"企画・プログラム・ \n"
			"　イベント・マップデザイン",

			"カレプリ",
		},
		{
			"マップ・戦闘 \n"
			"　キャラクターグラフィック",

			"るきあ",
		},
		{
			"ＣＧ・ \n"
			"　戦闘キャラクターグラフィック",

			"KAGe",
		},
		{
			"物体グラフィック",

			"しば",
		},
		{
			"背景グラフィック",

			"水銀",
		},
		{
			"ミュージック",

			"akima",
		},
		{
			"プログラム協力",

			"生姜\ntgbt",
		},
		{
			"効果音提供",

			"tissue\nザ・マッチメイカァズ",
		},
		{
			"Ｓｐｅｃｉａｌ　Ｔｈａｎｋｓ",

			"電気通信大学\n　X680x0同好会",
		}
	};

	static const CStaffViewPos viewpos[4]={
		{	0,0,			260,240,	360,320},
		{	(640-256),160,	60,100,		160,180},
		{	0,160,			260,100,	360,180},
		{	(640-256),0,	60,240,		160,320}
	};

	if(mTimer>=END_TIME){
		return false;
	}
	int time=mTimer%SHOW_TIME;
	int no=mTimer/SHOW_TIME;

	if(time==0){
		char fname[256];
		sprintf(fname,"data\\otherimg\\ed%d%d",(Flag.mHardMode!=0?1:0),no/2);
		DTest.LoadTexture(mTex,fname);
	}

	int y=-time/2;
	if(no%2==1)	y=(-160)-y;
	const CStaffViewPos& vp=viewpos[no%4];
	DTest.DrawTexture(mTex,0,(no%2)*128,0,128,256,vp.tx,vp.ty+y,256,512);
	DTest.DrawBox(vp.tx,0,vp.tx+256,vp.ty,0xff000000);
	DTest.DrawBox(vp.tx,vp.ty+320,vp.tx+256,480,0xff000000);

	if(FADE_IN_TIME<=time && time<FADE_IN_TIME+JOB_TIME){
		char text[256];
		strcpy(text,sinfo[no].job);
		int t=(time-FADE_IN_TIME)%JOB_LETTER_TIME;
		int letter=(time-FADE_IN_TIME)/JOB_LETTER_TIME;

		for(int i=0;i<JOB_LETTER_NO+1;i++){
			int l=letter-i;
			if(l<=0)	continue;
			text[l*2]='\0';
			
			int alpha=t+JOB_LETTER_TIME*i;
			alpha=alpha*0xff/(JOB_LETTER_NO+JOB_LETTER_TIME);
			if(alpha>0xff)	alpha=0xff;
			DTest.DrawText(4,text,vp.jx,vp.jy,D3DCOLOR_ARGB(255,alpha,alpha,alpha));
		}

	}else if(FADE_IN_TIME+JOB_TIME<=time){
		for(int i=0;i<JOB_LETTER_NO+1;i++){
			DTest.DrawText(4,sinfo[no].job,vp.jx,vp.jy,0xffffffff);
		}
		unsigned int alpha=(time-(FADE_IN_TIME+JOB_TIME))*0xff/NAME_FADE;
		if(alpha>0xff)	alpha=0xff;
		DTest.DrawText(9,sinfo[no].name,vp.nx,vp.ny,(alpha<<24)|0x00ffffff);
	}
	
	if(time<FADE_IN_TIME){
		unsigned int alpha=(FADE_IN_TIME-time)*0xff/FADE_IN_TIME;
		DTest.DrawBox(0,0,640,480,alpha<<24);
	}
	if(time>=SHOW_TIME-INTERVAL){
		DTest.DrawBox(0,0,640,480,0xff000000);
	}else if(time>=SHOW_TIME-INTERVAL-FADE_OUT_TIME){
		unsigned int alpha=(time-SHOW_TIME+FADE_OUT_TIME+INTERVAL)*0xff/FADE_OUT_TIME;
		DTest.DrawBox(0,0,640,480,alpha<<24);
	}


	return true;
}
bool CEndingMode::DrawEnd()
{
	static const int BLACK_TIME		= 8;
	static const int FADE_IN_TIME	= 32 + BLACK_TIME;
	static const int SHOW_TIME		= 240 + FADE_IN_TIME;
	static const int FADE_OUT_TIME	= 32 + SHOW_TIME;
	static const int END_TIME		= 16 + FADE_OUT_TIME;

	if(mTimer==0){
		char fname[256];
		sprintf(fname,"data\\otherimg\\ed2%d",mMode-2);
		DTest.LoadTexture(mTex,fname);
	}
	if(mTimer<BLACK_TIME){
		return true;
	}

	DTest.DrawTexture(mTex,0,0,0,640,480,0,0,640,480);

	if(mTimer<FADE_IN_TIME){
		int t=FADE_IN_TIME-mTimer;
		int c=t*0xff/(FADE_IN_TIME-BLACK_TIME);
		DTest.DrawBox(0,0,640,480,c<<24);
	}else	if(mTimer<SHOW_TIME){
	}else	if(mTimer<FADE_OUT_TIME){
		int t=mTimer-SHOW_TIME;
		int c=t*0xff/(FADE_OUT_TIME-SHOW_TIME);
		DTest.DrawBox(0,0,640,480,c<<24);
	}else{
		DTest.DrawBox(0,0,640,480,0xff000000);
	}
	if(FADE_IN_TIME<mTimer && mTimer<SHOW_TIME){
		mTimer=100;
		for(int key=4;key<=6;key++){
			if(pInput->GetKey(key)==1){
				mTimer=SHOW_TIME;
				break;
			}
		}
	}
	if(mTimer>=END_TIME){
		return false;
	}
	return true;
}