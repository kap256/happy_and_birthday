
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
						//A�ctex B�cturn
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
						//A�ctex B�cturn
	virtual bool		OnTimer(void){return CBattleEffect::OnTimer();};
	virtual void		OnDraw(void);
			int			OperationA(int parama,int paramb){return (mAniNo=(parama%16));}
						//A����A�j���[�V�����ԍ�
};
class CSmallAnimationEffect:public CAnimationEffect{
public:
	virtual void		OnDraw(void);
};
class CHaloEffect:public CAnimationEffect{
	int mCharacter;
public:
	void		Init(int x,int y,int parama,int paramb);
						//A�ctex+�ǐ����郆�j�b�g*0x10000 B�cturn
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
						//A�c(sx+256)+(sy+256)*0x10000 B�ctex+turn*0x10000
	virtual bool	OnTimer(void);
	virtual void	OnDraw(void);

	virtual int		OperationD(double parama,double paramb);
						//���x�ύX
	virtual int		OperationDP(double* parama,double* paramb);
						//���x�擾
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
						//A�c(sx+256)+(sy+256)*0x10000 B�ctex+����*0x10000
	virtual bool	OnTimer(void);
	virtual void	OnDraw(void);
	virtual int		OperationA(int parama,int paramb);
						//�T�C�Y�ύX
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
						//A�ccolor
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
					//A�c(sx+256)+(sy+256)*0x10000 B�ctex+�p�x*0x10000
			bool	OnTimer(void);
			void	OnDraw(void);
};
class CParticleEffect:public CMoveEffect{
protected:
	int	mLife;
public:
	virtual	void	Init(int x,int y,int parama,int paramb);
						//A�c(sx+256)+(sy+256)*0x10000 B�ctex+����*0x10000
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
						//A�c�p�x+����*0x10000 B�ctex+����*0x10000
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
						//A�ctex+turn*0x10000 B�c(128���W��)����+�c��*0x10000
	virtual bool		OnTimer(void){return CBattleEffect::OnTimer();};
	virtual void		OnDraw(void);
	virtual int			OperationA(int parama,int paramb);
						//�T�C�Y�ύX(128���W��)
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
						//A�ctex+turn*0x10000 B�c(128���W��)�ŏI�T�C�Y+�I������*0x10000
	virtual bool	OnTimer(void);
	virtual void	OnDraw(void){return CStretchEffect::OnDraw();};
};
class CTurnSpreadEffect:public CSpreadEffect{
protected:
	double mRad;
public:
	void	Init(int x,int y,int parama,int paramb);
						//A�ctex+�p�x*0x10000 B�c(128���W��)�ŏI�T�C�Y+�I������*0x10000
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
						//A�ctex+�p�x*0x10000 B�c��_(x+y*0x10000)
	void		OnDraw(void);
	int			OperationA(int parama,int paramb);
						//�p�x�ύX(parama�̂݁B)
protected:
	void		CulcRad(int x,int y);
};
class CStretchRotateEffect:public CStretchEffect{
protected:
	int			mAngle;

public:
	void		Init(int x,int y,int parama,int paramb);
						//A�ctex+�p�x*0x10000 B�c(128���W��)����+�c��*0x10000
	void		OnDraw(void);
};

class CCharacterEffect:public CBattleEffect{
protected:
	int		mCharacterNo;
	bool	mMove;

public:
	virtual void		Init(int x,int y,int parama,int paramb);
						//A�cno B�c�f�t�H���g�̈ʒu���g��(�P)�@�����Ŏw��i�O�j
	virtual bool		OnTimer(void);
	void				OnDraw(void);
};
class CFlashCharacterEffect:public CCharacterEffect{
protected:
	static const int DEFAULT_TIME	= 16;
	int		 mEndTime;

public:
	void		Init(int x,int y,int parama,int paramb);
						//A�cno  B�ctime
	bool		OnTimer(void);
};
class CFlashEffect:public CBattleEffect{
protected:
	static const int DEFAULT_TIME	= 16;

	int		 mEndTime;

public:
	void		Init(int x,int y,int parama,int paramb);
						//A�ctime
	bool		OnTimer(void);
	void		OnDraw(void);
};
class CBoxEffect:public CBattleEffect{
protected:
	int			mWidth;
	int			mHeight;


public:
	void		Init(int x,int y,int parama,int paramb);
						//A�cw B�ch
	void		OnDraw(void);
	int			OperationA(int parama,int paramb);
						//�T�C�Y�ύX
};
class CGradationVEffect:public CBattleEffect{
protected:
	int			mBottom;
	D3DCOLOR	mBColor;
public:
	void		Init(int x,int y,int parama,int paramb);
						//A�cbottom B�ccolor
	void		OnDraw(void);
	int			OperationA(int parama,int paramb);
						//�ʒu�ύX�@A�����@B�����
};
class CLineEffect:public CBattleEffect{
protected:
	double		mX2;
	double		mY2;
	double		mWidth;
public:
	void		Init(int x,int y,int parama,int paramb);
						//A�cx2 B�cy2
	void		OnDraw(void);
	int			OperationA(int parama,int paramb);
						//������ύX�@A���width*100;
	int			OperationD(double parama,double paramb);
						//���W2��ύX�@A���x�@B���y
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
						//A�c(sx+256)+(sy+256)*0x10000 B�cTailNo+Width*0x10000
	virtual bool	OnTimer(void);
	virtual void	OnDraw(void);

	virtual int		OperationA(int parama,int paramb);
						//���x�ύX
	virtual int		OperationD(double parama,double paramb);
						//���x�ύX
	virtual int		OperationDP(double* parama,double* paramb);
						//���x�擾
};
class CBreakEffect:public CFallParticleEffect{
	static const int LIFE = 32;
	int mBreak;
public:
	void	Init(int x,int y,int parama,int paramb);
			//A�c(sx+256)+(sy+256)*0x10000 B�ctex+����*0x10000
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
						//A�c�����p(�x)+�T�C�Y*0x10000 B�c
	virtual void	OnDraw(void);
	virtual bool	OnTimer(void);
};

class CTornadeSpinEffect:public CTornadeEffect{
protected:
	CDTTexture	mTex;

public:
	virtual void	Init(int x,int y,int parama,int paramb);
						//A�c�����p(�x)+�T�C�Y*0x10000 B�c�e�N�X�`��
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
						//A�c�����p(�x)+�T�C�Y*0x10000 B�cTailNo+Width*0x10000
	virtual void	OnDraw(void);
	virtual bool	OnTimer(void);
};
class CBlackHoleEffect:public CStretchEffect{
protected:
	double		mRad;
	int			mFlashTimer;
public:
	void		Init(int x,int y,int parama,int paramb);
						//A�ctex+�p�x*0x10000 B�c(128���W��)����+�c��*0x10000
	bool		OnTimer(void);
	void		OnDraw(void);
};
class CThunderEffect:public CTailLineEffect{
protected:
	double		mRad;
	int			mFlashTimer;
public:
	void		Init(int x,int y,int parama,int paramb);
						//A�cwidth B�c�\��
	bool		OnTimer(void);
	void		OnDraw(void);
};

class CSnowManEffect:public CMoveEffect{
protected:
	
	static const int DEL_TIME=120;	//�Z�̒萔�ƘA��
	int	mStart;
	double mOldSX;
	double mOldSY;
public:
	virtual	void	Init(int x,int y,int parama,int paramb);
						//A�c(sx+256)+(sy+256)*0x10000 B�ctex+�J�n����*0x10000
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
						//A�ctex+���\*0x10000  B�c�S�̎���+�J�n�E�I������*0x10000
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
						//A�ctex+�S�̎���  B�ctx+ty*0x10000
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
						//A�c�ύX���ԁ@B�c�F(0���w�肷��ƁA�ύX��ɍ폜)

	virtual bool		OnTimer(void);
	virtual void		OnDraw(void);
};