
#pragma once

#ifndef _EVENT_EDITER_
#include "DTestLib/dtest.h"
#endif

#include "battleconst.h"

#define BE_SPEED(sx,sy)	(CBattleEffect::SetSpeed((sx),(sy)))

enum BATTLE_EFFECT_TYPE{
	BE_STANDARD ,
	BE_ANIMATION ,
	BE_SMALL_ANIMATION ,
	BE_HALO ,
	BE_LARGE ,
	BE_ITEM ,
	BE_MOVE ,
	BE_MOVE_NO_SPIN ,
	BE_STRETCH_FADE_MOVE,
	BE_STRETCH_FADE_MOVE_NO_SPIN,
	BE_TAIL_MOVE ,
	BE_MAGIC_START ,
	BE_SMALL_SHOT ,
	BE_SMALL_MOVE_SPIN ,
	BE_PARTICUL,
	BE_FADE_IN_PARTICUL,
	BE_SPIN_PARTICUL,
	BE_FALL_PARTICUL,
	BE_CONCENTRATE,
	BE_STRETCH,
	BE_LARGE_STRETCH,
	BE_SPREAD,
	BE_TURN_SPREAD,
	BE_TURN_SPREAD_SLASH,
	BE_LARGE_SPREAD,
	BE_ROTATE ,
	BE_STRETCH_ROTATE,
	BE_CHARACTER,
	BE_FLASH_CHARACTER,
	BE_FLASH,
	BE_BOX,
	BE_GRADATION_V,
	BE_LINE,
	BE_TAIL_LINE,
	BE_BREAK,
	BE_TORNADE,
	BE_TORNADE_SPIN,
	BE_TORNADE_LINE,
	BE_BLACK_HOLE,
	BE_THUNDER,
	BE_SNOW_MAN,
	BE_SPOT_LIGHT,
	BE_LASER_CANON,
	BE_BACK_GROUND,
	BE_FIELD_COLOR,
};

class CBattleEffect{
protected:
	CBattleEffect*	mNext;
	double			mX;
	double			mY;
	int				mBlend;
	D3DCOLOR		mColor;

	int		mTimer;
	bool	mDelete;

public:
					CBattleEffect();
	virtual			~CBattleEffect();
	CBattleEffect*	SetEffect(BATTLE_EFFECT_TYPE type,int x,int y,int parama,int paramb,int blend=BLEND_STANDARD,D3DCOLOR color=0xffffffff);
	double	GetX(void)					{return mX;};
	double	GetY(void)					{return mY;};
	int		GetTimer(void)				{return mTimer;};
	void	SetPos(double x,double y)	{mX=x;mY=y;};
	void	SetColor(D3DCOLOR color)	{mColor=color;};
	D3DCOLOR GetColor()					{return mColor;};
	void	SetBlend(int blend)			{mBlend=blend;};

	virtual void	Init(int x,int y,int parama,int paramb);
	virtual bool	OnTimer(void);
	virtual void	OnDraw(void);
	virtual	void	Destroy(void);

			void	AllTimer(void);
			void	AllDraw(void);
			void	AllDelete(void);

	virtual int		OperationA(int parama,int paramb){return 0;};
	virtual int		OperationP(int* parama,int* paramb){return 0;};
	virtual int		OperationD(double parama,double paramb){return 0;};
	virtual int		OperationDP(double* parama,double* paramb){return 0;};

	static	void	LoadTexture(CDTTexture* tex,int no);
			void	GetSpeed(int param,double& x,double& y);
	static	int		SetSpeed(double x,double y);
};

class CStandardEffect:public CBattleEffect{
protected:
	static const int ANI_SPEED=2;

	CDTTexture	mTex;
	int			mTurn;

public:
	virtual void		Init(int x,int y,int parama,int paramb);
						//A…tex B…turn
	virtual bool		OnTimer(void);
	virtual void		OnDraw(void);
};
class CAnimationEffect:public CBattleEffect{
protected:
	CDTTexture	mTex;
	int			mTurn;
	int			mAniNo;

public:
	virtual void		Init(int x,int y,int parama,int paramb);
						//A…tex B…turn
	virtual bool		OnTimer(void){return CBattleEffect::OnTimer();};
	virtual void		OnDraw(void);
			int			OperationA(int parama,int paramb){return (mAniNo=(parama%16));}
						//A･･･アニメーション番号
};
class CSmallAnimationEffect:public CAnimationEffect{
public:
	virtual void		OnDraw(void);
};
class CHaloEffect:public CAnimationEffect{
	int mCharacter;
public:
	void		Init(int x,int y,int parama,int paramb);
						//A…tex+追随するユニット*0x10000 B…turn
	bool		OnTimer(void);
};
class CLargeEffect:public CStandardEffect{
public:
	virtual void		OnDraw(void);
};
class CItemEffect:public CBattleEffect{
	double	mZ;

	double	mSX;
	double	mSY;
	double	mSZ;

	int		mItem;
public:
	void		Init(int x,int y,int parama,int paramb);
	bool		OnTimer(void);
	void		OnDraw(void);
};
class CMoveEffect:public CBattleEffect{
protected:
	static const int ANI_SPEED=1;

	CDTTexture	mTex;
	int		mTurn;

	double	mRad;

	double	mSX;
	double	mSY;

public:
	virtual void	Init(int x,int y,int parama,int paramb);
						//A…(sx+256)+(sy+256)*0x10000 B…tex+turn*0x10000
	virtual bool	OnTimer(void);
	virtual void	OnDraw(void);

	virtual int		OperationD(double parama,double paramb);
						//速度変更
	virtual int		OperationDP(double* parama,double* paramb);
						//速度取得
	virtual int		OperationA(int parama,int paramb);
	virtual int		OperationP(int* parama,int* paramb);
};
class CMoveNoSpinEffect:public CMoveEffect{
public:
	virtual void	OnDraw(void);
};
class CStretchFadeMoveEffect:public CMoveEffect{
protected:
	int	mLife;
	int	mWidth;
	int mHeight;
public:
	virtual void	Init(int x,int y,int parama,int paramb);
						//A…(sx+256)+(sy+256)*0x10000 B…tex+寿命*0x10000
	virtual bool	OnTimer(void);
	virtual void	OnDraw(void);
	virtual int		OperationA(int parama,int paramb);
						//サイズ変更
};
class CStretchFadeMoveNoSpinEffect:public CStretchFadeMoveEffect{
public:
	virtual void	OnDraw(void);
};

class CTailMoveEffect:public CMoveEffect{
	static const int TAIL_NO=6;
	double	mTailX[TAIL_NO];
	double	mTailY[TAIL_NO];
public:
	virtual void	Init(int x,int y,int parama,int paramb);
	virtual bool	OnTimer(void);
	virtual void	OnDraw(void);
};

class CMagicStartEffect:public CBattleEffect{
	D3DCOLOR	mColor;

public:
	void		Init(int x,int y,int parama,int paramb);
						//A…color
	bool		OnTimer(void);
	void		OnDraw(void);
};
class CSmallShotEffect:public CMoveEffect{
public:
	void	OnDraw(void);
};
class CSmallMoveSpinEffect:public CMoveEffect{
	double  mRad;
	double	mSpin;
public:
	virtual void	Init(int x,int y,int parama,int paramb);
					//A…(sx+256)+(sy+256)*0x10000 B…tex+角度*0x10000
			bool	OnTimer(void);
			void	OnDraw(void);
};
class CParticleEffect:public CMoveEffect{
protected:
	int	mLife;
public:
	virtual	void	Init(int x,int y,int parama,int paramb);
						//A…(sx+256)+(sy+256)*0x10000 B…tex+寿命*0x10000
	virtual bool	OnTimer(void);
	virtual void	OnDraw(void);
};

class CSpinParticleEffect:public CParticleEffect{
public:
	virtual void	OnDraw(void);
};
class CFadeInParticleEffect:public CParticleEffect{
public:
	virtual	void	OnDraw(void);
};

class CFallParticleEffect:public CParticleEffect{
public:
	virtual	void	Init(int x,int y,int parama,int paramb){CParticleEffect::Init(x,y,parama,paramb);};
	virtual	void	OnDraw(void){CParticleEffect::OnDraw();};
			bool	OnTimer(void);
};

class CConcentrateEffect:public CMoveEffect{
protected:
	int	mLife;
public:
			void	Init(int x,int y,int parama,int paramb);
						//A…角度+距離*0x10000 B…tex+寿命*0x10000
	virtual bool	OnTimer(void);
	virtual void	OnDraw(void);
};

class CStretchEffect:public CBattleEffect{
protected:
	int			mWidth;
	int			mHeight;

	CDTTexture	mTex;
	int			mTurn;

public:
	virtual void		Init(int x,int y,int parama,int paramb);
						//A…tex+turn*0x10000 B…(128が標準)横幅+縦幅*0x10000
	virtual bool		OnTimer(void){return CBattleEffect::OnTimer();};
	virtual void		OnDraw(void);
	virtual int			OperationA(int parama,int paramb);
						//サイズ変更(128が標準)
};
class CLargeStretchEffect:public CStretchEffect{
protected:
public:
	void		OnDraw(void);
};
class CSpreadEffect:public CStretchEffect{
protected:
	int	mEndTime;
	int mEndSize;
public:
	virtual void	Init(int x,int y,int parama,int paramb);
						//A…tex+turn*0x10000 B…(128が標準)最終サイズ+終了時刻*0x10000
	virtual bool	OnTimer(void);
	virtual void	OnDraw(void){return CStretchEffect::OnDraw();};
};
class CTurnSpreadEffect:public CSpreadEffect{
protected:
	double mRad;
public:
	void	Init(int x,int y,int parama,int paramb);
						//A…tex+角度*0x10000 B…(128が標準)最終サイズ+終了時刻*0x10000
	virtual bool	OnTimer(void){return CSpreadEffect::OnTimer();};
	virtual void	OnDraw(void);
};
class CTurnSpreadSlashEffect:public CTurnSpreadEffect{
public:
	bool	OnTimer(void);
};
class CLargeSpreadEffect:public CSpreadEffect{
public:
	void		OnDraw(void);
};
class CRotateEffect:public CBattleEffect{
protected:
	double		mRad;
	double		mLength;
	int			mAngle;

	CDTTexture	mTex;

public:
	void		Init(int x,int y,int parama,int paramb);
						//A…tex+角度*0x10000 B…基準点(x+y*0x10000)
	void		OnDraw(void);
	int			OperationA(int parama,int paramb);
						//角度変更(paramaのみ。)
protected:
	void		CulcRad(int x,int y);
};
class CStretchRotateEffect:public CStretchEffect{
protected:
	int			mAngle;

public:
	void		Init(int x,int y,int parama,int paramb);
						//A…tex+角度*0x10000 B…(128が標準)横幅+縦幅*0x10000
	void		OnDraw(void);
};

class CCharacterEffect:public CBattleEffect{
protected:
	int		mCharacterNo;
	bool	mMove;

public:
	virtual void		Init(int x,int y,int parama,int paramb);
						//A…no B…デフォルトの位置を使う(１)　自分で指定（０）
	virtual bool		OnTimer(void);
	void				OnDraw(void);
};
class CFlashCharacterEffect:public CCharacterEffect{
protected:
	static const int DEFAULT_TIME	= 16;
	int		 mEndTime;

public:
	void		Init(int x,int y,int parama,int paramb);
						//A…no  B…time
	bool		OnTimer(void);
};
class CFlashEffect:public CBattleEffect{
protected:
	static const int DEFAULT_TIME	= 16;

	int		 mEndTime;

public:
	void		Init(int x,int y,int parama,int paramb);
						//A…time
	bool		OnTimer(void);
	void		OnDraw(void);
};
class CBoxEffect:public CBattleEffect{
protected:
	int			mWidth;
	int			mHeight;


public:
	void		Init(int x,int y,int parama,int paramb);
						//A…w B…h
	void		OnDraw(void);
	int			OperationA(int parama,int paramb);
						//サイズ変更
};
class CGradationVEffect:public CBattleEffect{
protected:
	int			mBottom;
	D3DCOLOR	mBColor;
public:
	void		Init(int x,int y,int parama,int paramb);
						//A…bottom B…color
	void		OnDraw(void);
	int			OperationA(int parama,int paramb);
						//位置変更　A･･･上　B･･･下
};
class CLineEffect:public CBattleEffect{
protected:
	double		mX2;
	double		mY2;
	double		mWidth;
public:
	void		Init(int x,int y,int parama,int paramb);
						//A…x2 B…y2
	void		OnDraw(void);
	int			OperationA(int parama,int paramb);
						//太さを変更　A･･･width*100;
	int			OperationD(double parama,double paramb);
						//座標2を変更　A･･･x　B･･･y
};
class CTailLineEffect:public CBattleEffect{
protected:
	static const int TAIL_MAX=64;
	double	mTailX[TAIL_MAX];
	double	mTailY[TAIL_MAX];
	double	mSX;
	double	mSY;
	int		mTailNo;
	int		mCurrentTail;
	int		mWidth;

public:
	virtual void	Init(int x,int y,int parama,int paramb);
						//A…(sx+256)+(sy+256)*0x10000 B…TailNo+Width*0x10000
	virtual bool	OnTimer(void);
	virtual void	OnDraw(void);

	virtual int		OperationA(int parama,int paramb);
						//速度変更
	virtual int		OperationD(double parama,double paramb);
						//速度変更
	virtual int		OperationDP(double* parama,double* paramb);
						//速度取得
};
class CBreakEffect:public CFallParticleEffect{
	static const int LIFE = 32;
	int mBreak;
public:
	void	Init(int x,int y,int parama,int paramb);
			//A…(sx+256)+(sy+256)*0x10000 B…tex+分割*0x10000
	void	OnDraw(void);
};

class CTornadeEffect:public CBattleEffect{
protected:
	static const int TIME		=	80;

	double	mRad;
	int		mSize;

	void	GetDrawInfo(int* x,int* y,D3DCOLOR* color);
public:
	virtual void	Init(int x,int y,int parama,int paramb);
						//A…初期角(度)+サイズ*0x10000 B…
	virtual void	OnDraw(void);
	virtual bool	OnTimer(void);
};

class CTornadeSpinEffect:public CTornadeEffect{
protected:
	CDTTexture	mTex;

public:
	virtual void	Init(int x,int y,int parama,int paramb);
						//A…初期角(度)+サイズ*0x10000 B…テクスチャ
	virtual void	OnDraw(void);
};
class CTornadeLineEffect:public CTornadeEffect{
protected:
	
	static const int TAIL_MAX=64;
	double	mTailX[TAIL_MAX];
	double	mTailY[TAIL_MAX];
	int		mTailNo;
	int		mWidth;
public:
	virtual void	Init(int x,int y,int parama,int paramb);
						//A…初期角(度)+サイズ*0x10000 B…TailNo+Width*0x10000
	virtual void	OnDraw(void);
	virtual bool	OnTimer(void);
};
class CBlackHoleEffect:public CStretchEffect{
protected:
	double		mRad;
	int			mFlashTimer;
public:
	void		Init(int x,int y,int parama,int paramb);
						//A…tex+角度*0x10000 B…(128が標準)横幅+縦幅*0x10000
	bool		OnTimer(void);
	void		OnDraw(void);
};
class CThunderEffect:public CTailLineEffect{
protected:
	double		mRad;
	int			mFlashTimer;
public:
	void		Init(int x,int y,int parama,int paramb);
						//A…width B…予備
	bool		OnTimer(void);
	void		OnDraw(void);
};

class CSnowManEffect:public CMoveEffect{
protected:
	
	static const int DEL_TIME=120;	//技の定数と連動
	int	mStart;
	double mOldSX;
	double mOldSY;
public:
	virtual	void	Init(int x,int y,int parama,int paramb);
						//A…(sx+256)+(sy+256)*0x10000 B…tex+開始時刻*0x10000
	virtual bool	OnTimer(void);
	virtual void	OnDraw(void);
};
class CSpotLightEffect:public CBattleEffect{
protected:
	CDTTexture	mTex;

	bool mBottom;
	int	mAllTime;
	int mStartTime;

public:
	virtual void		Init(int x,int y,int parama,int paramb);
						//A…tex+裏表*0x10000  B…全体時間+開始・終了時間*0x10000
	virtual bool		OnTimer(void);
	virtual void		OnDraw(void);
};
class CLaserCanonEffect:public CBattleEffect{
protected:
	CDTTexture	mTex;

	int mAllTime;
	double mRad;

	int mLaserX;
	int mLaserY;

public:
	virtual void		Init(int x,int y,int parama,int paramb);
						//A…tex+全体時間  B…tx+ty*0x10000
	virtual bool		OnTimer(void);
	virtual void		OnDraw(void);
};
class CBackGroundEffect:public CBattleEffect{
protected:
public:
	virtual void		Init(int x,int y,int parama,int paramb);
						//
	virtual void		OnDraw(void);
};
class CFieldColorEffect:public CBattleEffect{
protected:
	int			mChangeTime;
	D3DCOLOR	mTargetColor;

public:
	virtual void		Init(int x,int y,int parama,int paramb);
	virtual int			OperationA(int parama,int paramb);
						//A…変更時間　B…色(0を指定すると、変更後に削除)

	virtual bool		OnTimer(void);
	virtual void		OnDraw(void);
};