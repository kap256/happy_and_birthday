#include "trial_demo.h"
#include "changemode.h"
#include "input.h"


#ifdef _TRIAL_VERSION_

CTrialDemo::CTrialDemo(){
}
CTrialDemo::~CTrialDemo(){
}

void CTrialDemo::Init(void){
	mTimer=0;
	DTest.LoadTexture(mTex,"data\\otherimg\\title",0);
	SetBGMFadeOut(120);
}
void CTrialDemo::OnTimer(void){
	mTimer++;

	if(mTimer<WHITE_TIME)	return;
	if(pInput->GetKey(4)==1 ||pInput->GetKey(5)==1 ||pInput->GetKey(6)==1){
		ChangeMapInfo.ChangeMap(-1,0,0,0,0);
	}
}
void CTrialDemo::OnDraw(void){
	
	if(mTimer<BLACK_TIME){
		return;
	}
	
	if(mTimer<WHITE_UP_TIME){
		int t=mTimer-BLACK_TIME;
		int c=t*0xff/(WHITE_UP_TIME-BLACK_TIME);
		DTest.DrawBox(0,0,640,480,c*0x01000000 + 0xffffff);
	}else{
		DTest.DrawTexture(mTex,0,0,0,640,480,0,0,640,480);

		const int x=600;
		const int y=300;
		const char* text="体験版はここまでとなります。\n続きは製品版でお楽しみください。\n\n※次回の公開予定は未定です。";

		int sx,sy;
		const int space=8;
		DTest.GetTextSize(4,text,&sx,&sy);
		DTest.DrawBox(x-sx-space,y-space,x+space,y+sy+space,0x80000000);
		DTest.DrawText(4,text,0,y,0xff000000,DT_RIGHT,x);
		DTest.DrawText(4,text,0,y-2,0xffffffff,DT_RIGHT,x+1);

		if(mTimer<WHITE_TIME){
			DTest.DrawBox(0,0,640,480,0xffffffff);
		}else	if(mTimer<WHITE_DOWN_TIME){
			int t=WHITE_DOWN_TIME-mTimer;
			int c=t*0xff/(WHITE_DOWN_TIME-WHITE_TIME);
			DTest.DrawBox(0,0,640,480,c*0x01000000 + 0xffffff);
		}
	}
}

#endif