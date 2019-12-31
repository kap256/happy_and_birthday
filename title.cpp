#include "title.h"
#include "cursor.h"
#include "input.h"
#include "changemode.h"
#include "flag.h"
#include "month.h"
#include "menudata.h"

CTitleMode::CTitleMode()
{
}
CTitleMode::~CTitleMode()
{
}

void CTitleMode::Init()
{
	DTest.LoadTexture(mTitleTex,"data\\otherimg\\title",0);
	mTimer=0;
	mCursor=0;
	mItemNo=0;

	bool cont=false;
	for(int i=0;i<SAVE_DATA_NO;i++){
		static char fname[256];
		FILE* pFile;
		sprintf(fname,"save\\save%02d",i);
		if(pFile=fopen(fname,"rb")){
			cont=true;
			fclose(pFile);
			break;
		}
	}

	if(cont){
		mItemList[mItemNo]=1;
		mItemNo++;
	}
	mItemList[mItemNo]=2;
	mItemList[mItemNo+1]=4;
	mItemNo+=2;
	if(GlobalFlag.mSoundTest){
		mItemList[mItemNo]=0;
		mItemNo++;
	}
	mItemList[mItemNo]=3;
	mItemNo+=1;
	PlayBGM(25);	
	Flag.mChangeOptionTitle=false;
}
void CTitleMode::OnTimer()
{
	mTimer++;
	mShowCursor=true;

	if(mTimer>CURSOR_START){
		if(pInput->GetRepeatKey(1)==1){
			mCursor--;
			if(mCursor<0)	mCursor=mItemNo-1;
			PlayWave(SE_MOVE);
		}
		if(pInput->GetRepeatKey(3)==1){
			mCursor++;
			if(mCursor>=mItemNo)	mCursor=0;
			PlayWave(SE_MOVE);
		}
		int x,y;
		GetItemPos(mCursor,&x,&y);
		Cursor.Move(x-30,y+20);

		if(pInput->GetKey(4)==1){
			PlayWave(SE_YES);
			mShowCursor=false;
			switch(mItemList[mCursor]){
				case 0:
					MenuData.SetMenu(SoundTestMenu,0);
					break;
				case 1:
					MenuData.SetMenu(DiaryMenu,1);
					break;
				case 2:
					Flag.Reset();
					Flag.mChangeOptionTitle=false;
					MenuData.SetMenu(SelectDifficult,0);
					break;
				case 3:
					DTest.Exit();
					return;
				case 4:
					Flag.mChangeOptionTitle=true;
					MenuData.SetMenu(ConfigMenu,0);
					break;
			}
		}
	}	
}
void CTitleMode::OnDraw()
{
	static const char* ITEM_NAME[MAX_ITEM]={
		"Sound Test",
		"Continue",
		"New Game",
		"Exit",
		"Option"
	};

	if(mTimer<BLACK_TIME){
		return;
	}
	
	if(mTimer<WHITE_UP_TIME){
		int t=mTimer-BLACK_TIME;
		int c=t*0xff/(WHITE_UP_TIME-BLACK_TIME);
		DTest.DrawBox(0,0,640,480,c*0x01000000 + 0xffffff);
	}else{
		DTest.DrawTexture(mTitleTex,0,0,0,640,480,0,0,640,480);

		int x,y;
		GetItemPos(mCursor,&x,&y);
		y+=16;
		DTest.DrawBox(x,y,x+150,y+10,COLOR_SELECT);
		
		for(int i=0;i<mItemNo;i++){
			int x,y;
			GetItemPos(i,&x,&y);
			DTest.DrawText(4,ITEM_NAME[mItemList[i]],x,y,0xff62301f);
			DTest.DrawText(4,ITEM_NAME[mItemList[i]],x+1,y-2,0xffffffff);
		}
#ifdef _TRIAL_VERSION_
		x=430;	y=220;
		DTest.DrawText(4,"`Trial Version`",x,y,0xff62301f);
#endif

		if(mShowCursor)		Cursor.OnDraw();
		if(mTimer<WHITE_TIME){
			DTest.DrawBox(0,0,640,480,0xffffffff);
		}else	if(mTimer<WHITE_DOWN_TIME){
			int t=WHITE_DOWN_TIME-mTimer;
			int c=t*0xff/(WHITE_DOWN_TIME-WHITE_TIME);
			DTest.DrawBox(0,0,640,480,c*0x01000000 + 0xffffff);
		}
	}
}
void CTitleMode::Destroy(void)
{
	mTitleTex.Release();
}
void CTitleMode::GetItemPos(int no,int *x,int *y)
{
	*x=ITEM_X+ITEM_X_MOVE*no;
	*y=ITEM_Y+ITEM_H*no-ITEM_H*mItemNo/2;
}