#include "cursor.h"
#include "input.h"


CCursor::CCursor()
{
	mX=CURSOR_START_X;
	mY=CURSOR_START_Y;
	mGoalX=CURSOR_START_X;
	mGoalY=CURSOR_START_Y;
	mTimer=CURSOR_CLICK_TIME;
	mStar=0;
	memset(mStarX,128,sizeof(mStarX));
	memset(mStarY,128,sizeof(mStarY));
}
void	CCursor::OnTimer()
{
	if(mTimer>=CURSOR_CLICK_TIME){
		mX+=(mGoalX-mX)/CURSOR_SPEED;
		mY+=(mGoalY-mY)/CURSOR_SPEED;
	}
	if(mTimer%6==0){
		mStarX[mStar]=int(mX+pInput->Rand(16));
		mStarY[mStar]=int(mY+pInput->Rand(10)+6);
		mStar=(mStar+1)%CURSOR_MAX_STAR;
	}
	for(int j=mStar%1;j<CURSOR_MAX_STAR;j+=1){
		mStarY[j]++;
	}
	mTimer++;
}
void	CCursor::OnDraw()
{
	for(int i=0;i<CURSOR_MAX_STAR;i++){
		int no=i-mStar;
		if(no<0)	no+=CURSOR_MAX_STAR;
		DTest.DrawTexture(	SystemTex,0,160+16*(rand()%2),rand()%2*16,16,16,
							mStarX[i],mStarY[i],16,16,
							(0x80/CURSOR_MAX_STAR*no)*0x01000000+0xffffff);
	}

	int dx;
	if(mTimer<CURSOR_CLICK_TIME){
		dx=4;
	}else{
		switch((mTimer/CURSOR_ANI_TIME)%4){
			case 1:
				dx=2;	break;
			case 3:
				dx=3;	break;
			default:
				dx=1;
		}
	}
	DTest.DrawTexture(SystemTex,0,dx*32,0,32,32,int(mX),int(mY));

}
void	CCursor::Move(int x,int y)
{
	mGoalX=x;
	mGoalY=y;
}
void	CCursor::Click(void)
{
//	PlayWave(SE_YES);
	mTimer=0;
}
void	CCursor::Escape(void)
{
	mGoalX=CURSOR_START_X;
	mGoalY=CURSOR_START_Y;
}
