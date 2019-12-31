
#include "battlemessage.h"
#include "main.h"
#include "math.h"


CBattleMessage::CBattleMessage()
{
	mMessage=NULL;
	mNext=NULL;
}
CBattleMessage::~CBattleMessage()
{
	delete[] mMessage;
}

CBattleMessage** CBattleMessage::SearchEnd(void)
{
	CBattleMessage* battlemes=this;

	while(battlemes->mNext){
		battlemes=battlemes->mNext;
	}

//	battlemes->mNext=new CBattleMessage;
//	battlemes->mNext->Init(x,y,mes,color);
	return &(battlemes->mNext);
}

void CBattleMessage::Init(int x,int y,const char* mes,D3DCOLOR color1,D3DCOLOR color2)
{

	mX=x;
	mY=y;
	mColor1=color1;
	mColor2=color2;
	if(!mes){
		mes="ERROR!";
	}
	mSize=int(strlen(mes));
	mMessage=new char[mSize+1];
	strcpy(mMessage,mes);

	mTimer=-BMES_TIME_ONE_BOUND*mSize;
	mDeleteTimer=0;
}
void CBattleMessage::OnTimer(void)
{
	mTimer++;
	if(mDeleteTimer>0)	mDeleteTimer++;
	if(mTimer>BMES_TIME_VANISH)	Kill();

	while(mNext && mNext->mDeleteTimer >= BMES_TIME_DELETE){
		CBattleMessage* del=mNext;
		mNext=mNext->mNext;
		delete del;
	}

	if(mNext)	mNext->OnTimer();
}
void CBattleMessage::OnDraw(int depth[BATTLE_SQUARE_WIDTH][BATTLE_SQUARE_Height])
{
	DrawMessage(depth);

	if(mNext)	mNext->OnDraw(depth);
}
void CBattleMessage::DrawMessage(int	depth[BATTLE_SQUARE_WIDTH][BATTLE_SQUARE_Height])
{
	if(mMessage){
		int sx=(mX+BATTLE_SQUARE_SIZE/2)/BATTLE_SQUARE_SIZE;
		int sy=(mY+BATTLE_SQUARE_SIZE/2)/BATTLE_SQUARE_SIZE;

		int start=-mTimer/BMES_TIME_ONE_BOUND;
		if(start<0)	start=0;
		for(int i=start;i<mSize;i++){
			int fx=(mMessage[mSize-i-1]%32)*8;
			int fy=52+(mMessage[mSize-i-1]/32)*12;

			int tx=mX+BATTLE_SQUARE_X+32;
			int ty=mY+BATTLE_SQUARE_Y+80;

			if(sx>=4){
				tx+=64-mSize*BMES_WIDTH;
			}
			tx+=(mSize-i-1)*BMES_WIDTH;

			ty-=int(pow(double(depth[sx][sy]),0.75)*2.0);
			int bound=-mTimer-i*BMES_TIME_ONE_BOUND+BMES_TIME_BOUND;
			if(bound>0){
				int c=BMES_TIME_BOUND/2;
				bound-=c;
				ty+=(bound*bound-c*c)/4;
			}

			D3DCOLOR color1=mColor1;
			D3DCOLOR color2=mColor2;
			if(mDeleteTimer>0){
				D3DCOLOR alpha=((0xff*(BMES_TIME_DELETE-mDeleteTimer)/(BMES_TIME_DELETE))<<24);
				color1=alpha + (mColor1 % 0x01000000);
				color2=alpha + (mColor2 % 0x01000000);
			}
			DTest.DrawTexture(	SystemTex,0,
								fx,fy,8,12,
								tx,ty,16,24,
								color1,color1,color2,color2);
		}

		if(mDeleteTimer>0){
			depth[sx][sy]+=24*(BMES_TIME_DELETE-mDeleteTimer)/(BMES_TIME_DELETE);
		}else{
			depth[sx][sy]+=24;
		}
	}
}
void CBattleMessage::Kill(void)
{
	if(mDeleteTimer<=0)	mDeleteTimer=1;
}
void CBattleMessage::AllDelete(void)
{
	CBattleMessage* mes=mNext;

	while(mes){
		CBattleMessage* del=mes;
		mes=mes->mNext;
		delete del;
	}

	mNext=NULL;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void CNumberMessage::Init(int x,int y,int dmg,int type,int id,int target,const char* mes,int color,bool crt)
{
	mType=type ;
	mColor=color;

	CBattleMessage::Init(x,y,mes,GetColor1(mColor,mType),GetColor2(mColor,mType));
	mDmg	=dmg;
	mID		=id;
	mTarget	=target;
	mCrt	=crt;
}

D3DCOLOR CNumberMessage::GetColor1(int color,int type)
{
	return GetColor2(color/5*5,type);
}
D3DCOLOR CNumberMessage::GetColor2(int color,int type)
{
	static const D3DCOLOR ELE_COLOR[AT_ELE_NO*2]={
//í èÌéû
		0xffffffff,
		0xffff8080,
		0xff80b0ff,
		0xff60ff60,
		0xffffff40,
//ñhå‰Ç≥ÇÍÇΩÇ∆Ç´
		0xffd0d0d0,
		0xffd06060,
		0xff6060d0,
		0xff50d050,
		0xffd0d030
	};

	switch(type){
		case NMES_TYPE_DMG:		return ELE_COLOR[color];
		case NMES_TYPE_LP:		return 0xffff0000;
		case NMES_TYPE_HEAL:	return 0xff00ff00;
	}
	return 0xffffffff;
}