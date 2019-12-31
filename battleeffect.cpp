
#include "battleeffect.h"
#include "main.h"
#include "itemdata.h"
#include "battlemode.h"
#include "input.h"


CBattleEffect::CBattleEffect()
{
	mNext=NULL;
	mDelete=false;
	mTimer=0;
	mColor=0xffffffff;
	mBlend=BLEND_STANDARD;
}
CBattleEffect::~CBattleEffect()
{
}
CBattleEffect* CBattleEffect::SetEffect(BATTLE_EFFECT_TYPE type,int x,int y,int parama,int paramb,int blend,D3DCOLOR color)
{
	CBattleEffect* effect=this;

	while(effect->mNext){
		effect=effect->mNext;
	}

	switch(type){
		case BE_STANDARD:
			effect->mNext=new CStandardEffect;			break;
		case BE_ANIMATION:
			effect->mNext=new CAnimationEffect;			break;
		case BE_SMALL_ANIMATION:
			effect->mNext=new CSmallAnimationEffect;	break;
		case BE_HALO:
			effect->mNext=new CHaloEffect;				break;
		case BE_LARGE:
			effect->mNext=new CLargeEffect;				break;
		case BE_ITEM:
			effect->mNext=new CItemEffect;				break;
		case BE_MOVE:
			effect->mNext=new CMoveEffect;				break;
		case BE_MOVE_NO_SPIN:
			effect->mNext=new CMoveNoSpinEffect;		break;
		case BE_STRETCH_FADE_MOVE:
			effect->mNext=new CStretchFadeMoveEffect;	break;
		case BE_STRETCH_FADE_MOVE_NO_SPIN:
			effect->mNext=new CStretchFadeMoveNoSpinEffect;	break;
		case BE_TAIL_MOVE:
			effect->mNext=new CTailMoveEffect;			break;
		case BE_MAGIC_START:
			effect->mNext=new CMagicStartEffect;		break;
		case BE_SMALL_SHOT:
			effect->mNext=new CSmallShotEffect;			break;
		case BE_SMALL_MOVE_SPIN:
			effect->mNext=new CSmallMoveSpinEffect;		break;
		case BE_PARTICUL:
			effect->mNext=new CParticleEffect;			break;
		case BE_FADE_IN_PARTICUL:
			effect->mNext=new CFadeInParticleEffect;	break;
		case BE_SPIN_PARTICUL:
			effect->mNext=new CSpinParticleEffect;		break;
		case BE_FALL_PARTICUL:
			effect->mNext=new CFallParticleEffect;		break;
		case BE_CONCENTRATE:
			effect->mNext=new CConcentrateEffect;		break;
		case BE_STRETCH:
			effect->mNext=new CStretchEffect;			break;
		case BE_LARGE_STRETCH:
			effect->mNext=new CLargeStretchEffect;		break;
		case BE_SPREAD:
			effect->mNext=new CSpreadEffect;			break;
		case BE_TURN_SPREAD:
			effect->mNext=new CTurnSpreadEffect;		break;
		case BE_TURN_SPREAD_SLASH:
			effect->mNext=new CTurnSpreadSlashEffect;	break;
		case BE_LARGE_SPREAD:
			effect->mNext=new CLargeSpreadEffect;		break;
		case BE_ROTATE:
			effect->mNext=new CRotateEffect;			break;
		case BE_STRETCH_ROTATE:
			effect->mNext=new CStretchRotateEffect;		break;
		case BE_CHARACTER:
			effect->mNext=new CCharacterEffect;			break;
		case BE_FLASH_CHARACTER:
			effect->mNext=new CFlashCharacterEffect;	break;
		case BE_FLASH:
			effect->mNext=new CFlashEffect;				break;
		case BE_BOX:
			effect->mNext=new CBoxEffect;				break;
		case BE_GRADATION_V:
			effect->mNext=new CGradationVEffect;		break;
		case BE_LINE:
			effect->mNext=new CLineEffect;				break;
		case BE_TAIL_LINE:
			effect->mNext=new CTailLineEffect;			break;
		case BE_BREAK:
			effect->mNext=new CBreakEffect;				break;
		case BE_TORNADE:
			effect->mNext=new CTornadeEffect;			break;
		case BE_TORNADE_SPIN:
			effect->mNext=new CTornadeSpinEffect;		break;
		case BE_TORNADE_LINE:
			effect->mNext=new CTornadeLineEffect;		break;
		case BE_BLACK_HOLE:
			effect->mNext=new CBlackHoleEffect;			break;
		case BE_THUNDER:
			effect->mNext=new CThunderEffect;			break;
		case BE_SNOW_MAN:
			effect->mNext=new CSnowManEffect;			break;
		case BE_SPOT_LIGHT:
			effect->mNext=new CSpotLightEffect;			break;
		case BE_LASER_CANON:
			effect->mNext=new CLaserCanonEffect;		break;
		case BE_BACK_GROUND:
			effect->mNext=new CBackGroundEffect;		break;
		case BE_FIELD_COLOR:
			effect->mNext=new CFieldColorEffect;		break;
		default:
			return NULL;
	}

	effect->mNext->SetBlend(blend);
	effect->mNext->SetColor(color);
	effect->mNext->Init(x,y,parama,paramb);
	return effect->mNext;
}

void CBattleEffect::Init(int x,int y,int parama,int paramb)
{
}
bool CBattleEffect::OnTimer(void)
{
	mTimer++;

	return true;
}
void CBattleEffect::OnDraw(void)
{
}

void	CBattleEffect::AllTimer(void)
{
	CBattleEffect* effect=this;
	while(effect->mNext){
		bool timer=effect->mNext->OnTimer();
		if(!timer || effect->mNext->mDelete){
			CBattleEffect* del=effect->mNext;
			effect->mNext=effect->mNext->mNext;
			delete del;
		}
		effect=effect->mNext;
		if(!effect)	break;
	}
}
void	CBattleEffect::AllDraw(void)
{
	CBattleEffect* effect=mNext;
	while(effect){
		effect->OnDraw();
		effect=effect->mNext;
	}

}
void CBattleEffect::AllDelete(void)
{
	CBattleEffect* effect=mNext;

	while(effect){
		CBattleEffect* del=effect;
		effect=effect->mNext;
		delete del;
	}

	mNext=NULL;
}
void CBattleEffect::Destroy(void)
{
	mDelete=true;
}

void	CBattleEffect::LoadTexture(CDTTexture* tex,int no)
{
	char fname[256];
	sprintf(fname,"data\\battleeffect\\battleeffect%02d",no);
	DTest.LoadTexture(*tex,fname,0xff008080);
}

void	CBattleEffect::GetSpeed(int param,double& x,double& y)
{
	x=((param%0x10000)-0x4000)/64.0;
	y=((param/0x10000)-0x4000)/64.0;
}
int	CBattleEffect::SetSpeed(double x,double y)
{
	return (int(x*64)+0x4000)+(int(y*64)+0x4000)*0x10000;
}

////////////////////////////////////////////////////////////////////////////////////////
void CStandardEffect::Init(int x,int y,int parama,int paramb)
{
	mX=x+BATTLE_SQUARE_X;
	mY=y+BATTLE_SQUARE_Y;

	LoadTexture(&mTex,parama);

	mTurn=paramb;
}
bool CStandardEffect::OnTimer(void)
{
	CBattleEffect::OnTimer();
	
	if(mTimer>=ANI_SPEED*4){
		return false;
	}
	return true;
}
void CStandardEffect::OnDraw(void)
{
	int ani=mTimer/ANI_SPEED;
	
	DTest.SetBlend(mBlend);
	DTest.DrawTexture(mTex,mTurn,
						(ani%2)*64,(ani/2)*64,
						64,64,
						(int)mX-64,(int)mY-64,
						128,128,mColor);
	DTest.SetBlend(BLEND_STANDARD);
	CBattleEffect::OnDraw();
}
////////////////////////////////////////////////////////////////////////////////////////
void CAnimationEffect::Init(int x,int y,int parama,int paramb)
{
	mX=x+BATTLE_SQUARE_X;
	mY=y+BATTLE_SQUARE_Y;

	LoadTexture(&mTex,parama);

	mTurn=paramb;
	mAniNo=0;
}
void CAnimationEffect::OnDraw(void)
{
	DTest.SetBlend(mBlend);
	DTest.DrawTexture(mTex,mTurn,
						(mAniNo%4)*64,(mAniNo/4)*64,
						64,64,
						(int)mX-64,(int)mY-64,
						128,128,mColor);
	DTest.SetBlend(BLEND_STANDARD);
	CBattleEffect::OnDraw();
}
////////////////////////////////////////////////////////////////////////////////////////
void CSmallAnimationEffect::OnDraw(void)
{
	DTest.SetBlend(mBlend);
	DTest.DrawTexture(mTex,mTurn,
						(mAniNo%4)*32,(mAniNo/4)*32,
						32,32,
						(int)mX-32,(int)mY-32,
						64,64,mColor);
	DTest.SetBlend(BLEND_STANDARD);
	CBattleEffect::OnDraw();
}
////////////////////////////////////////////////////////////////////////////////////////
void CHaloEffect::Init(int x,int y,int parama,int paramb)
{
	CAnimationEffect::Init(x,y,parama&0xffff,paramb);
	
	mCharacter=parama/0x10000;
}
bool CHaloEffect::OnTimer(void)
{
	mAniNo=mTimer/16%4;
	if(mAniNo>=2)	mAniNo+=2;

	CBattleCharacter* character=BattleMode.GetCharacter(mCharacter);
	mX=character->GetX()+64;
	mY=character->GetY()-character->GetZ()+96;

	return CAnimationEffect::OnTimer();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CLargeEffect::OnDraw(void)
{
	int ani=mTimer/ANI_SPEED;
	
	DTest.SetBlend(mBlend);
	DTest.DrawTexture(mTex,mTurn,
						(ani%2)*128,(ani/2)*128,
						128,128,
						(int)mX-128,(int)mY-128,
						256,256,mColor);
	DTest.SetBlend(BLEND_STANDARD);
	CBattleEffect::OnDraw();
}
////////////////////////////////////////////////////////////////////////////////////////
void CItemEffect::Init(int x,int y,int parama,int paramb)
{
	mX=x+BATTLE_SQUARE_X;
	mY=y+BATTLE_SQUARE_Y;
	mZ=0;

	int gx=parama%0x10000;
	int gy=parama/0x10000;

	mSX=(gx-x)/BM_INTARVAL_TIME;
	mSY=(gy-y)/BM_INTARVAL_TIME;
	mSZ=BM_INTARVAL_TIME*BATTLE_GRAVITY/2;

	mItem=paramb;
}
bool CItemEffect::OnTimer(void)
{
	CBattleEffect::OnTimer();

	mX+=mSX;
	mY+=mSY;
	mZ+=mSZ;

	mSZ-=BATTLE_GRAVITY;
	
	if(mTimer>=BM_INTARVAL_TIME){
		return false;
	}
	return true;
}
void CItemEffect::OnDraw(void)
{
	int icon=AllItem[mItem]->mIcon;
	int tx=icon%8*32;;
	int ty=(icon/8)%8*32;
	int tex=icon/64;
	
	DTest.DrawTexture(ItemTex[tex],0,
						tx,ty,
						32,32,
						int(mX)-32,int(mY-mZ)-32,
						32,32,mColor);
	CBattleEffect::OnDraw();
}
////////////////////////////////////////////////////////////////////////////////////////
void CMoveEffect::Init(int x,int y,int parama,int paramb)
{
	mX=x+BATTLE_SQUARE_X;
	mY=y+BATTLE_SQUARE_Y;

	GetSpeed(parama,mSX,mSY);

	mRad=-atan2(mSX,mSY);
	mTurn=paramb/0x10000;

	LoadTexture(&mTex,paramb&0xffff);
}
bool CMoveEffect::OnTimer(void)
{
	CBattleEffect::OnTimer();

	mX+=mSX;
	mY+=mSY;
	
	if(mX<-1280 || 2000<mX || mY<-1280 || 2000<mY){
		return false;
	}
	return true;
}
void CMoveEffect::OnDraw(void)
{
	int ani=(mTimer/ANI_SPEED)%4;
	
	DTest.SetBlend(mBlend);
	DTest.DrawTextureRotate(mTex,mTurn,
						(ani%2)*64,(ani/2)*64,
						64,64,
						int(mX),int(mY),
						128,128,mColor,mRad);
	DTest.SetBlend(BLEND_STANDARD);
	CBattleEffect::OnDraw();
}

int CMoveEffect::OperationD(double parama,double paramb)
{
	mSX=parama;
	mSY=paramb;

	if(mSX || mSY){
		mRad=-atan2(mSX,mSY);
	}
	return 0;
}
int CMoveEffect::OperationDP(double* parama,double* paramb)
{
	if(parama)	*parama=mSX;
	if(paramb)	*paramb=mSY;

	return 0;
}

int CMoveEffect::OperationA(int parama,int paramb)
{
	mSX=parama;
	mSY=paramb;

	if(mSX || mSY){
		mRad=-atan2(mSX,mSY);
	}
	return 0;
}
int CMoveEffect::OperationP(int* parama,int* paramb)
{
	if(parama)	*parama=(int)mSX;
	if(paramb)	*paramb=(int)mSY;

	return 0;
}
////////////////////////////////////////////////////////////////////////////////////////
void CStretchFadeMoveEffect::Init(int x,int y,int parama,int paramb)
{
	CMoveEffect::Init(x,y,parama,paramb&0xffff);

	mLife=paramb/0x10000;
	mWidth=128;
	mHeight=128;
}
bool CStretchFadeMoveEffect::OnTimer(void)
{
	bool ret=CMoveEffect::OnTimer();
	if(mTimer>mLife){
		return false;
	}
	return ret;
}
void CStretchFadeMoveEffect::OnDraw(void)
{
	int ani=(mTimer/ANI_SPEED)%4;	
	int t;
	if(mTimer>mLife/2){
		t=mLife-mTimer;
	}else{
		t=mTimer;
	}
	D3DCOLOR color=((((mColor&0xff000000)/(mLife/2))*t) & 0xff000000) | (mColor&0xffffff);

	DTest.SetBlend(mBlend);
	DTest.DrawTextureRotate(mTex,0,
						(ani%2)*64,(ani/2)*64,
						64,64,
						int(mX),int(mY),
						mWidth,mHeight,color,mRad);
	DTest.SetBlend(BLEND_STANDARD);
	CBattleEffect::OnDraw();
}
int CStretchFadeMoveEffect::OperationA(int parama,int paramb)
{
	mWidth=parama;
	mHeight=paramb;
	return 0;
}
////////////////////////////////////////////////////////////////////////////////////////

void CMoveNoSpinEffect::OnDraw(void)
{
	int ani=(mTimer/ANI_SPEED)%4;
	
	DTest.SetBlend(mBlend);
	DTest.DrawTexture(mTex,mTurn,
						(ani%2)*64,(ani/2)*64,
						64,64,
						int(mX)-64,int(mY)-64,
						128,128,mColor);
	DTest.SetBlend(BLEND_STANDARD);
	CBattleEffect::OnDraw();
}
////////////////////////////////////////////////////////////////////////////////////////

void CStretchFadeMoveNoSpinEffect::OnDraw(void)
{
	int ani=(mTimer/ANI_SPEED)%4;	
	int t;
	if(mTimer>mLife/2){
		t=mLife-mTimer;
	}else{
		t=mTimer;
	}
	D3DCOLOR color=((((mColor&0xff000000)/(mLife/2))*t) & 0xff000000) | (mColor&0xffffff);

	DTest.SetBlend(mBlend);
	DTest.DrawTextureRotate(mTex,0,
						(ani%2)*64,(ani/2)*64,
						64,64,
						int(mX),int(mY),
						mWidth,mHeight,color);
	DTest.SetBlend(BLEND_STANDARD);
	CBattleEffect::OnDraw();
}
////////////////////////////////////////////////////////////////////////////////////////

void	CTailMoveEffect::Init(int x,int y,int parama,int paramb)
{
	CMoveEffect::Init(x,y,parama,paramb);
	for(int i=0;i<TAIL_NO;i++){
		mTailX[i]=-65535;
		mTailY[i]=-65535;
	}
}
bool	CTailMoveEffect::OnTimer(void)
{
	for(int i=TAIL_NO-1;i>=1;i--){
		mTailX[i]=mTailX[i-1];
		mTailY[i]=mTailY[i-1];
	}
	mTailX[0]=mX;
	mTailY[0]=mY;
	return CMoveEffect::OnTimer();
}
void	CTailMoveEffect::OnDraw(void)
{
	DTest.SetBlend(mBlend);
	for(int i=TAIL_NO-1;i>=0;i--){
		int ani=((mTimer-i)/ANI_SPEED)%4;
		if(ani<0)	ani+=4;
		int size=64-i*8;

		DTest.DrawTextureRotate(mTex,0,
							(ani%2)*64,(ani/2)*64,
							64,64,
							int(mTailX[i]),int(mTailY[i]),
							size,size,mColor,mRad);
	}
	CMoveEffect::OnDraw();
}
////////////////////////////////////////////////////////////////////////////////////////
void CMagicStartEffect::Init(int x,int y,int parama,int paramb)
{
	mX=x+BATTLE_SQUARE_X;
	mY=y+BATTLE_SQUARE_Y;
	mColor=parama;
}

bool CMagicStartEffect::OnTimer(void)
{
	CBattleEffect::OnTimer();

	if(mTimer>=BM_INTARVAL_TIME){
		return false;
	}
	return true;
}
void CMagicStartEffect::OnDraw(void)
{
	int size=(BM_INTARVAL_TIME-mTimer)*16;
	double rad=(mTimer*mTimer)/80.0;
	D3DCOLOR alpha=(mTimer*0xb0/(BM_INTARVAL_TIME))*0x01000000;
	D3DCOLOR color=(mColor&0xffffff);

	DTest.SetBlend(BLEND_STANDARD);
	DTest.DrawBoxRotate((int)mX,(int)mY,size,size,color+((alpha/3)&0xff000000),rad);
	DTest.SetBlend(BLEND_ADD);
	DTest.DrawBoxRotate((int)mX,(int)mY,size,size,color+alpha,rad);
	DTest.SetBlend(BLEND_STANDARD);
	CBattleEffect::OnDraw();
}
////////////////////////////////////////////////////////////////////////////////////////
void CSmallShotEffect::OnDraw(void)
{
	int ani=(mTimer/ANI_SPEED)%4;
	
	DTest.SetBlend(mBlend);
	DTest.DrawTextureRotate(mTex,0,
						(ani%2)*32,(ani/2)*32,
						32,32,
						int(mX),int(mY),
						64,64,mColor,mRad);
	DTest.SetBlend(BLEND_STANDARD);
	CBattleEffect::OnDraw();
}
////////////////////////////////////////////////////////////////////////////////////////
void CSmallMoveSpinEffect::Init(int x,int y,int parama,int paramb)
{
	CMoveEffect::Init(x,y,parama,paramb&0xffff);
	mRad=0;
	mSpin=(paramb/0x10000)*3.141592/180;
}
bool CSmallMoveSpinEffect::OnTimer(void)
{
	mRad+=mSpin;
	return CMoveEffect::OnTimer();
}
void CSmallMoveSpinEffect::OnDraw(void)
{	
	DTest.SetBlend(mBlend);
	DTest.DrawTextureRotate(mTex,0,
						0,0,
						32,32,
						int(mX),int(mY),
						64,64,mColor,mRad);
	DTest.SetBlend(BLEND_STANDARD);
	CBattleEffect::OnDraw();
}
////////////////////////////////////////////////////////////////////////////////////////
void CParticleEffect::Init(int x,int y,int parama,int paramb)
{
	mLife=paramb/0x10000;
	CMoveEffect::Init(x,y,parama,paramb&0xffff);
}

bool CParticleEffect::OnTimer(void)
{
	bool ret= CMoveEffect::OnTimer();
	if(ret){
		if(mTimer>mLife)	return false;
	}
	return ret;
}
void CParticleEffect::OnDraw(void)
{
	D3DCOLOR color=(((0xff000000/mLife)*(mLife-mTimer)) & 0xff000000) | (mColor&0xffffff);

	DTest.SetBlend(mBlend);
	DTest.DrawTextureRotate(mTex,0,
						0,0,
						32,32,
						int(mX),int(mY),
						64,64,color,mRad);
	DTest.SetBlend(BLEND_STANDARD);
	CBattleEffect::OnDraw();
}
////////////////////////////////////////////////////////////////////////////////////////
void CSpinParticleEffect::OnDraw(void)
{
	D3DCOLOR color=(((0xff000000/mLife)*(mLife-mTimer)) & 0xff000000) | (mColor&0xffffff);
	double rad=mRad+(mTimer*0.3);

	DTest.SetBlend(mBlend);
	DTest.DrawTextureRotate(mTex,0,
						0,0,
						32,32,
						int(mX),int(mY),
						64,64,color,rad);
	DTest.SetBlend(BLEND_STANDARD);
	CBattleEffect::OnDraw();
}
////////////////////////////////////////////////////////////////////////////////////////
void CFadeInParticleEffect::OnDraw(void)
{
	int t;
	if(mTimer>mLife/2){
		t=mLife-mTimer;
	}else{
		t=mTimer;
	}
	D3DCOLOR color=(((0xff000000/(mLife/2))*t) & 0xff000000) | (mColor&0xffffff);

	DTest.SetBlend(mBlend);
	DTest.DrawTextureRotate(mTex,0,
						0,0,
						32,32,
						int(mX),int(mY),
						64,64,color,mRad);
	DTest.SetBlend(BLEND_STANDARD);
	CBattleEffect::OnDraw();
}

////////////////////////////////////////////////////////////////////////////////////////
bool CFallParticleEffect::OnTimer(void)
{
	mSY+=0.25;
	return CParticleEffect::OnTimer();
}
////////////////////////////////////////////////////////////////////////////////////////
void CConcentrateEffect::Init(int x,int y,int parama,int paramb)
{
	mLife=paramb/0x10000;

	mRad=(parama&0xffff)*3.141592/180;;
	int length=parama/0x10000;

	mSX=-cos(mRad)*length;
	mSY=sin(mRad)*length;

	mX=x+BATTLE_SQUARE_X-mSX;
	mY=y+BATTLE_SQUARE_Y-mSY;

	mSX/=mLife;
	mSY/=mLife;

	LoadTexture(&mTex,paramb&0xffff);
}

bool CConcentrateEffect::OnTimer(void)
{
	bool ret= CMoveEffect::OnTimer();
	if(ret){
		if(mTimer>=mLife)	return false;
	}
	return ret;
}
void CConcentrateEffect::OnDraw(void)
{
	D3DCOLOR color=(((0xff000000/mLife)*(mTimer)) & 0xff000000) | (mColor&0xffffff);
	int size=128*(mLife-mTimer)/mLife;
	if(size<=0)	return;

	DTest.SetBlend(mBlend);
	DTest.DrawTextureRotate(mTex,0,
						0,0,
						32,32,
						int(mX),int(mY),
						size,size,color,mRad);
	DTest.SetBlend(BLEND_STANDARD);
	CBattleEffect::OnDraw();
}

////////////////////////////////////////////////////////////////////////////////////////
void	CStretchEffect::Init(int x,int y,int parama,int paramb)
{
	mX=x+BATTLE_SQUARE_X;
	mY=y+BATTLE_SQUARE_Y;

	LoadTexture(&mTex,parama&0xffff);

	mTurn=parama/0x10000;

	mWidth=paramb&0xffff;
	mHeight=paramb/0x10000;
}
void	CStretchEffect::OnDraw(void)
{
	int x=(int)mX-mWidth/2;
	int y=(int)mY-mHeight/2;

	DTest.SetBlend(mBlend);
	DTest.DrawTexture(mTex,mTurn,
						0,0,
						64,64,
						x,y,
						mWidth,mHeight,mColor);
	DTest.SetBlend(BLEND_STANDARD);
	CBattleEffect::OnDraw();
}
int		CStretchEffect::OperationA(int parama,int paramb)
{
	mWidth=parama;
	mHeight=paramb;
	return 0;
}
////////////////////////////////////////////////////////////////////////////////////////

void	CLargeStretchEffect::OnDraw(void)
{
	int x=(int)mX-mWidth/2;
	int y=(int)mY-mHeight/2;

	DTest.SetBlend(mBlend);
	DTest.DrawTexture(mTex,mTurn,
						0,0,
						128,128,
						x,y,
						mWidth,mHeight,mColor);
	DTest.SetBlend(BLEND_STANDARD);
	CBattleEffect::OnDraw();
}
////////////////////////////////////////////////////////////////////////////////////////
void	CSpreadEffect::Init(int x,int y,int parama,int paramb)
{
	CStretchEffect::Init(x,y,parama,0);

	mEndSize=paramb&0xffff;
	mEndTime=paramb/0x10000;
}
bool	CSpreadEffect::OnTimer(void)
{
	mWidth	=mEndSize*mTimer/mEndTime;
	mHeight	=mEndSize*mTimer/mEndTime;
	int t=mEndTime-mTimer;
	mColor	=((0xff*t/mEndTime)<<24) | (mColor & 0xffffff);
	if(mTimer>mEndTime)	return false;
	return CStretchEffect::OnTimer();
}
////////////////////////////////////////////////////////////////////////////////////////
void	CLargeSpreadEffect::OnDraw(void)
{
	int x=(int)mX-mWidth/2;
	int y=(int)mY-mHeight/2;

	DTest.SetBlend(mBlend);
	DTest.DrawTexture(mTex,mTurn,
						0,0,
						128,128,
						x,y,
						mWidth,mHeight,mColor);
	DTest.SetBlend(BLEND_STANDARD);
	CBattleEffect::OnDraw();
}

////////////////////////////////////////////////////////////////////////////////////////
void	CTurnSpreadEffect::Init(int x,int y,int parama,int paramb)
{
	CSpreadEffect::Init(x,y,parama&0xffff,paramb);

	mRad=(parama/0x10000)*3.14/180.0;
}
void	CTurnSpreadEffect::OnDraw(void)
{
	DTest.SetBlend(mBlend);
	DTest.DrawTextureRotate(mTex,0,
						0,0,
						64,64,
						(int)mX,(int)mY,
						mWidth,mHeight,mColor,mRad);
	DTest.SetBlend(BLEND_STANDARD);
	CBattleEffect::OnDraw();
}
////////////////////////////////////////////////////////////////////////////////////////
bool	CTurnSpreadSlashEffect::OnTimer(void)
{
	bool ret=CTurnSpreadEffect::OnTimer();
	mWidth=64;

	return ret;
}
////////////////////////////////////////////////////////////////////////////////////////
void	CStretchRotateEffect::Init(int x,int y,int parama,int paramb)
{
	CStretchEffect::Init(x,y,parama,paramb);

	mAngle=parama/0x10000;

}
void	CStretchRotateEffect::OnDraw(void)
{
	DTest.SetBlend(mBlend);
	DTest.DrawTextureRotate(mTex,0,
						0,0,
						64,64,
						int(mX),int(mY),
						mWidth,mHeight,mColor,
						mAngle*3.141592/180);
	DTest.SetBlend(BLEND_STANDARD);
	CBattleEffect::OnDraw();
}

////////////////////////////////////////////////////////////////////////////////////////
void	CRotateEffect::Init(int x,int y,int parama,int paramb)
{
	mX=x+BATTLE_SQUARE_X;
	mY=y+BATTLE_SQUARE_Y;

	LoadTexture(&mTex,parama&0xffff);

	mAngle=parama/0x10000;

	int cx=paramb&0xffff;
	int cy=paramb/0x10000;

	CulcRad(-cx+64,-cy+64);
}
void	CRotateEffect::OnDraw(void)
{
	double rad=mAngle*3.141592/180;
	int x=int(mX+sin(-rad+mRad)*mLength);
	int y=int(mY+cos(-rad+mRad)*mLength);

	DTest.SetBlend(mBlend);
	DTest.DrawTextureRotate(mTex,0,
						0,0,
						64,64,
						x,y,
						128,128,mColor,
						rad);
	DTest.SetBlend(BLEND_STANDARD);
	CBattleEffect::OnDraw();
}
int		CRotateEffect::OperationA(int parama,int paramb)
{
	mAngle=parama;
	return 0;
}
void	CRotateEffect::CulcRad(int x,int y)
{
	mRad=atan2(double(x),double(y));
	mLength=hypot(double(x),double(y));
}
////////////////////////////////////////////////////////////////////////////////////////
void	CCharacterEffect::Init(int x,int y,int parama,int paramb)
{
	mX=x+BATTLE_SQUARE_X;
	mY=y+BATTLE_SQUARE_Y;

	mMove=(paramb!=0);

	mCharacterNo=parama;
}
bool	CCharacterEffect::OnTimer(void)
{
	return CBattleEffect::OnTimer();
}
void	CCharacterEffect::OnDraw(void)
{
	CBattleCharacter* character=BattleMode.GetCharacter(mCharacterNo);

	DTest.SetBlend(mBlend);
	if(mMove){
		character->OnDraw(mColor);
	}else{
		int x=int(mX)-((character->GetSize()-1)*BATTLE_SQUARE_SIZE/2);
		int y=(int)mY-((character->GetSize()-1)*BATTLE_SQUARE_SIZE*3/4);
		character->OnDraw(x,y,mColor);
	}
	DTest.SetBlend(BLEND_STANDARD);

	CBattleEffect::OnDraw();
}
////////////////////////////////////////////////////////////////////////////////////////
void	CFlashCharacterEffect::Init(int x,int y,int parama,int paramb)
{
	mX=0;
	mY=0;

	mMove=true;

	mCharacterNo=parama;
	mEndTime=paramb;
	if(mEndTime<=0){
		mEndTime=DEFAULT_TIME;
	}
}
bool		CFlashCharacterEffect::OnTimer(void)
{
	bool ret=CBattleEffect::OnTimer();
	
	if(mTimer>=mEndTime){
		return false;
	}

	int alpha=(mEndTime-mTimer)*255/mEndTime;
	mColor=(mColor & 0xffffff) + alpha*0x1000000;
	return ret;
}


////////////////////////////////////////////////////////////////////////////////////////
void	CFlashEffect::Init(int x,int y,int parama,int paramb)
{
	mX=0;
	mY=0;

	mEndTime=parama;
	if(mEndTime<=0){
		mEndTime=DEFAULT_TIME;
	}
}
bool		CFlashEffect::OnTimer(void)
{
	CBattleEffect::OnTimer();
	
	if(mTimer>=mEndTime){
		return false;
	}
	return true;
}
void		CFlashEffect::OnDraw(void)
{
	int alpha=(mEndTime-mTimer)*(mColor>>24)/mEndTime;
	D3DCOLOR color=(mColor & 0xffffff) + alpha*0x1000000;
	DTest.SetBlend(mBlend);
	DTest.DrawBox(0,0,640,480,color);
	DTest.SetBlend(BLEND_STANDARD);

	CBattleEffect::OnDraw();
}
////////////////////////////////////////////////////////////////////////////////////////
void	CBoxEffect::Init(int x,int y,int parama,int paramb)
{
	mX=x+BATTLE_SQUARE_X;
	mY=y+BATTLE_SQUARE_Y;

	mWidth=parama;
	mHeight=paramb;
}
void	CBoxEffect::OnDraw(void)
{
	int x=(int)mX-mWidth/2;
	int y=(int)mY-mHeight/2;

	DTest.SetBlend(mBlend);
	DTest.DrawBox(x,y,mWidth+x,mHeight+y,mColor);
	DTest.SetBlend(BLEND_STANDARD);
	CBattleEffect::OnDraw();
}
int		CBoxEffect::OperationA(int parama,int paramb)
{
	mWidth=parama;
	mHeight=paramb;
	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////
void	CGradationVEffect::Init(int x,int y,int parama,int paramb)
{
	mX=x+BATTLE_SQUARE_X;
	mY=y;

	mBottom=parama;
	mBColor=D3DCOLOR(paramb);
}
void	CGradationVEffect::OnDraw(void)
{
	DTest.SetBlend(mBlend);
	DTest.DrawBox(0,int(mY),640,mBottom,mColor,mColor,mBColor,mBColor);
	DTest.SetBlend(BLEND_STANDARD);
	CBattleEffect::OnDraw();
}
int		CGradationVEffect::OperationA(int parama,int paramb)
{
	mY=parama;
	mBottom=paramb;
	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////
void	CLineEffect::Init(int x,int y,int parama,int paramb)
{
	mX=x+BATTLE_SQUARE_X;
	mY=y+BATTLE_SQUARE_Y;

	mX2=parama+BATTLE_SQUARE_X;
	mY2=paramb+BATTLE_SQUARE_Y;
	mWidth=LINE_WIDTH;
}
void	CLineEffect::OnDraw(void)
{
	DTest.SetBlend(mBlend);
	DTest.DrawLine(int(mX),int(mY),mX2,mY2,mWidth,mColor);
	DTest.SetBlend(BLEND_STANDARD);
	CBattleEffect::OnDraw();
}
int	CLineEffect::OperationA(int parama,int paramb)
{
	mWidth=parama/100.0;
	return 0;
}
int	CLineEffect::OperationD(double parama,double paramb)
{
	mX2=parama;
	mY2=paramb;

	return 0;
}
////////////////////////////////////////////////////////////////////////////////////////
void CTailLineEffect::Init(int x,int y,int parama,int paramb)
{
	mX=x+BATTLE_SQUARE_X;
	mY=y+BATTLE_SQUARE_Y;

	GetSpeed(parama,mSX,mSY);

	mCurrentTail=0;
	mTailNo=paramb%0x10000;
	mWidth=paramb/0x10000;

	if(mTailNo>TAIL_MAX)	mTailNo=TAIL_MAX;
	mTailX[0]=mX;
	mTailY[0]=mY;
}
bool CTailLineEffect::OnTimer(void)
{
	CBattleEffect::OnTimer();

	mX+=mSX;
	mY+=mSY;

	OperationA(int(mX),int(mY));

	int tail=mTailNo;
	if(tail>mCurrentTail)	tail=mCurrentTail;

	bool del=true;
	for(int i=0;i<tail;i++){
		if(mTailX[i]>-100 && 800>mTailX[i] && mTailY[i]>-100 && 600>mTailY[i]){
			del=false;
		}
	}
	return !del;
}
void CTailLineEffect::OnDraw(void)
{
	int tail=mTailNo;
	if(tail>mCurrentTail)	tail=mCurrentTail;
	
	DTest.SetBlend(mBlend);

	for(int i=0;i<tail-1;i++){
		int alpha=0xff*(tail-i+1)/tail+1;
		DTest.DrawLine(int(mTailX[i]),int(mTailY[i]),int(mTailX[i+1]),int(mTailY[i+1]),mWidth,alpha*0x01000000 + (mColor&0xffffff));
	}

	DTest.SetBlend(BLEND_STANDARD);
	CBattleEffect::OnDraw();
}
int CTailLineEffect::OperationA(int parama,int paramb)
{
	mCurrentTail++;
	for(int i=mTailNo-1;i>=1;i--){
		mTailX[i]=mTailX[i-1];
		mTailY[i]=mTailY[i-1];
	}
	mTailX[0]=parama;
	mTailY[0]=paramb;

	return 0;
}
int CTailLineEffect::OperationD(double parama,double paramb)
{
	mSX=parama;
	mSY=paramb;
	return 0;
}
int CTailLineEffect::OperationDP(double* parama,double* paramb)
{
	if(parama)	*parama=mSX;
	if(paramb)	*paramb=mSY;
	return 0;
}
////////////////////////////////////////////////////////////////////////////////////////
void	CBreakEffect::Init(int x,int y,int parama,int paramb)
{
	mBreak=paramb/0x10000;
	paramb=(paramb&0xffff) + LIFE*0x10000;
	CFallParticleEffect::Init(x,y,parama,paramb);
}
void	CBreakEffect::OnDraw(void)
{
	D3DCOLOR color=(((0xff000000/mLife)*(mLife-mTimer)) & 0xff000000) | (mColor&0xffffff);

	int tx=mBreak%4;
	int ty=mBreak/4;
	DTest.SetBlend(mBlend);
	DTest.DrawTexture(mTex,0,
						tx*16,ty*16,
						16,16,
						int(mX)+tx*32-64,int(mY)+ty*32-64,
						32,32,color);
	DTest.SetBlend(BLEND_STANDARD);
	CBattleEffect::OnDraw();
}
////////////////////////////////////////////////////////////////////////////////////////
void CTornadeEffect::Init(int x,int y,int parama,int paramb)
{
	mX=x+BATTLE_SQUARE_X;
	mY=y+BATTLE_SQUARE_Y;

	mRad =(parama%0x10000)*3.141592/180;;
	mSize=(parama/0x10000);
}
bool CTornadeEffect::OnTimer(void)
{
	CBattleEffect::OnTimer();

	if(mTimer>TIME){
		return false;
	}

	return true;
}
void CTornadeEffect::OnDraw(void)
{
	const int SIZE=32;

	int x,y;
	D3DCOLOR color;
	GetDrawInfo(&x,&y,&color);
	DTest.SetBlend(mBlend);

	DTest.DrawBox(x,y,x+SIZE,y+SIZE,color);

	DTest.SetBlend(BLEND_STANDARD);
	CBattleEffect::OnDraw();
}
void CTornadeEffect::GetDrawInfo(int* x,int* y,D3DCOLOR* color)
{
	int t=(TIME-mTimer);
	int size=mSize*t*t/(TIME*TIME);
	double rad=mRad+mTimer*mTimer*0.003;
	if(x)		*x=int(mX + cos(rad)*size);
	if(y)		*y=int(mY + sin(rad)*size/2 - mTimer*mTimer/8);
	if(color)	*color=(mTimer*0xff/TIME)*0x01000000 | mColor&0x00ffffff;
}
////////////////////////////////////////////////////////////////////////////////////////
void CTornadeSpinEffect::Init(int x,int y,int parama,int paramb)
{
	CTornadeEffect::Init(x,y,parama,paramb);

	LoadTexture(&mTex,paramb);
}
void CTornadeSpinEffect::OnDraw(void)
{
	int x,y;
	D3DCOLOR color;
	GetDrawInfo(&x,&y,&color);
	double rad=mTimer*0.2;

	DTest.SetBlend(mBlend);
	DTest.DrawTextureRotate(mTex,0,0,0,32,32,x,y,64,64,color,rad);
	DTest.SetBlend(BLEND_STANDARD);
	CBattleEffect::OnDraw();
}
////////////////////////////////////////////////////////////////////////////////////////
void CTornadeLineEffect::Init(int x,int y,int parama,int paramb)
{
	CTornadeEffect::Init(x,y,parama,paramb);

	mTailNo=paramb%0x10000;
	mWidth=paramb/0x10000;

	if(mTailNo>TAIL_MAX)	mTailNo=TAIL_MAX;

	GetDrawInfo(&x,&y,NULL);
	mTailX[0]=x;
	mTailY[0]=y;
}
bool CTornadeLineEffect::OnTimer(void)
{
	bool ret=CTornadeEffect::OnTimer();

	for(int i=mTailNo-1;i>=1;i--){
		mTailX[i]=mTailX[i-1];
		mTailY[i]=mTailY[i-1];
	}
	int x,y;
	GetDrawInfo(&x,&y,NULL);
	mTailX[0]=x;
	mTailY[0]=y;

	return ret;
}
void CTornadeLineEffect::OnDraw(void)
{
	D3DCOLOR color;
	GetDrawInfo(NULL,NULL,&color);
	int tail=mTailNo;
	if(tail>mTimer)	tail=mTimer;

	int alpha=color>>24;
	D3DCOLOR rgb=color& 0xffffff;
	
	DTest.SetBlend(mBlend);

	for(int i=0;i<tail-1;i++){
		int a=alpha*(tail-i+1)/tail+1;
		DTest.DrawLine(int(mTailX[i]),int(mTailY[i]),int(mTailX[i+1]),int(mTailY[i+1]),mWidth,a*0x01000000 + (mColor&0xffffff));
	}
	DTest.SetBlend(BLEND_STANDARD);
	CBattleEffect::OnDraw();
}
////////////////////////////////////////////////////////////////////////////////////////
void	CBlackHoleEffect::Init(int x,int y,int parama,int paramb)
{
	int size=paramb&0xffff;
	CStretchEffect::Init(x,y,parama&0xffff,size+size*0x10000);

	mRad=(parama/0x10000)*3.141592/180;
	mFlashTimer=(paramb/0x10000);
}
bool	CBlackHoleEffect::OnTimer(void)
{
	bool ret=CStretchEffect::OnTimer();
	mFlashTimer++;
	if(mTimer>16)	return false;
	return ret;
}

void	CBlackHoleEffect::OnDraw(void)
{
	DTest.SetBlend(mBlend);
	DTest.DrawTextureRotate(mTex,mTurn,
						0,0,
						256,256,
						int(mX),int(mY),
						mWidth,mHeight,mColor,mRad);
	DTest.SetBlend(BLEND_STANDARD);
	CBattleEffect::OnDraw();
}
////////////////////////////////////////////////////////////////////////////////////////

void CThunderEffect::Init(int x,int y,int parama,int paramb)
{
	CTailLineEffect::Init(x-BATTLE_SQUARE_X,y-BATTLE_SQUARE_Y,0,16+parama*0x10000);
}
bool CThunderEffect::OnTimer(void)
{
	if(mTimer<=mWidth){
		mSX=(pInput->Rand(2000)-1000)*0.1;
		mSY=(pInput->Rand(1000)+200)*0.1;
		if(mTimer>=1){
			SetEffect(BE_THUNDER,
				int(mX),int(mY),
				mWidth-mTimer-1,0,mBlend,mColor);
		}
	}else{
		mSX=0;
		mSY=0;
	}

	if(mTimer>mWidth+16)	Destroy();

	CTailLineEffect::OnTimer();

	return true;
}
void CThunderEffect::OnDraw(void)
{
	int tail=mTailNo-1;
	if(tail>mTimer)	tail=mTimer;
	
	DTest.SetBlend(mBlend);

	for(int i=0;i<tail;i++){
		int alpha=0xff*(tail-i+1)/tail+1;
		int width=mWidth+i-mTimer;
		if(width<=0)	continue;
		DTest.DrawLine(int(mTailX[i]),int(mTailY[i]),int(mTailX[i+1]),int(mTailY[i+1]),width,alpha*0x01000000 + (mColor&0xffffff));
	}
	DTest.SetBlend(BLEND_STANDARD);
	CBattleEffect::OnDraw();
}
////////////////////////////////////////////////////////////////////////////////////////

void CSnowManEffect::Init(int x,int y,int parama,int paramb)
{
	mStart=paramb/0x10000;
	CMoveEffect::Init(x,y,parama,paramb&0xffff);

	mOldSX=mSX;
	mOldSY=mSY;
}

bool CSnowManEffect::OnTimer(void)
{
	bool ret= CMoveEffect::OnTimer();
	if(ret){
		if(mTimer==16)					OperationD(0,0);
		if(mTimer+mStart==DEL_TIME)		OperationD(-mOldSX/2,-mOldSY/2);
		if(mTimer+mStart>DEL_TIME)		mSY+=0.25;
		if(mTimer+mStart>DEL_TIME+32)	return false;
	}
	return ret;
}
void CSnowManEffect::OnDraw(void)
{

	D3DCOLOR color=mColor;
	if(mTimer+mStart>DEL_TIME){
		int t=mTimer+mStart-DEL_TIME;
		color=(((0xff000000/32)*(32-t)) & 0xff000000) | (mColor&0xffffff);
	}
	DTest.SetBlend(mBlend);
	DTest.DrawTextureRotate(mTex,0,
						0,0,
						32,32,
						int(mX),int(mY),
						64,64,color,mRad);
	DTest.SetBlend(BLEND_STANDARD);
	CBattleEffect::OnDraw();
}
////////////////////////////////////////////////////////////////////////////////////////

void CSpotLightEffect::Init(int x,int y,int parama,int paramb)
{
	mX=x+BATTLE_SQUARE_X;
	mY=y+BATTLE_SQUARE_Y;
	mBottom=bool((parama&0xffff0000)!=0);
	if(mBottom)	mY+=32;

	LoadTexture(&mTex,parama&0xffff);

	mAllTime  = paramb&0xffff;
	mStartTime= paramb/0x10000;
}
bool CSpotLightEffect::OnTimer(void)
{
	CBattleEffect::OnTimer();
	
	if(mTimer>=mAllTime){
		return false;
	}
	return true;
}
void CSpotLightEffect::OnDraw(void)
{
	int x=128;
	if(mTimer<mStartTime)			x=128*mTimer/mStartTime;
	if(mTimer>mAllTime-mStartTime)	x=128*(mAllTime-mTimer)/mStartTime;
	
	int ty=32+(mBottom?16:0);
	DTest.SetBlend(mBlend);
	DTest.DrawTexture(mTex,0,
						0,ty,
						64,16,
						(int)mX-x/2,int(mY),
						x,32,mColor);
	DTest.DrawTexture(mTex,0,
						0,0,
						64,32,
						(int)mX-x/2,0,
						x,mY,mColor);
	DTest.SetBlend(BLEND_STANDARD);
	CBattleEffect::OnDraw();
}
////////////////////////////////////////////////////////////////////////////////////////
void CLaserCanonEffect::Init(int x,int y,int parama,int paramb)
{
	mX=x+BATTLE_SQUARE_X;
	mY=y+BATTLE_SQUARE_Y;

	LoadTexture(&mTex,parama&0xffff);

	mAllTime= parama/0x10000;
	int tx  = paramb&0xffff;
	int ty  = paramb/0x10000;

	mRad=atan2(double(tx-x),double(y-ty));

	mLaserX=int(mX+sin(mRad)*532);
	mLaserY=int(mY-cos(mRad)*532);
}
bool CLaserCanonEffect::OnTimer(void)
{
	CBattleEffect::OnTimer();
	
	if(mTimer>=mAllTime){
		return false;
	}
	return true;
}
void CLaserCanonEffect::OnDraw(void)
{
	int alpha=255;
	if(mTimer>=mAllTime/2){
		int t=mAllTime-mTimer;
		alpha=t*255/(mAllTime/2);
	}
	D3DCOLOR color=mColor&0x00ffffff + alpha*0x01000000;
	int w=121+pInput->Rand(15);
	DTest.SetBlend(mBlend);
	DTest.DrawTextureRotate(mTex,0,
							0,32,
							64,32,
							(int)mX,int(mY),
							w,64,color,mRad);
	DTest.DrawTextureRotate(mTex,0,
							0,0,
							64,32,
							mLaserX,mLaserY,
							w,1000,color,mRad);
	DTest.SetBlend(BLEND_STANDARD);
	CBattleEffect::OnDraw();
}
////////////////////////////////////////////////////////////////////////////////////////
void CBackGroundEffect::Init(int x,int y,int parama,int paramb)
{
	mX=x;
	mY=y;
}
void CBackGroundEffect::OnDraw(void)
{
	CDTTexture* tex=BattleMode.GetBackGroundTex();
	DTest.SetBlend(mBlend);
	DTest.DrawTexture(*tex,0,
						0,0,
						640,480,
						(int)mX,int(mY),
						640,480,mColor);
	DTest.SetBlend(BLEND_STANDARD);
	CBattleEffect::OnDraw();
}
////////////////////////////////////////////////////////////////////////////////////////
void CFieldColorEffect::Init(int x,int y,int parama,int paramb)
{
	OperationA(parama,paramb);
}
int CFieldColorEffect::OperationA(int x,int y)
{
	mChangeTime=x;
	mTargetColor=y;

	return 0;
}
bool CFieldColorEffect::OnTimer(void)
{
	CBattleEffect::OnTimer();

	if(mChangeTime>=0){
		if(mChangeTime<=0){
			if(mTargetColor==0){
				return false;
			}
			mColor=mTargetColor;
		}else{
			unsigned char* c=(unsigned char*)&mColor;
			unsigned char* goal=(unsigned char*)&mTargetColor;
			for(int i=0;i<4;i++){
				c[i]+=(goal[i]-c[i])/mChangeTime;
			}
		}
		mChangeTime--;
	}
	return true;
}
void CFieldColorEffect::OnDraw(void)
{
	DTest.SetBlend(mBlend);
	DTest.DrawBox(	0,0,640,480,mColor);
	DTest.SetBlend(BLEND_STANDARD);
	CBattleEffect::OnDraw();
}
////////////////////////////////////////////////////////////////////////////////////////


