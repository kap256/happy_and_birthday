#pragma once

//#include "main.h"
#include "gamewindow.h"
#include "battlecharacter.h"
#include "characterdata.h"
#include "battleconst.h"
#include "battlemenu.h"
#include "battlemessage.h"
#include "battleeffect.h"
#include "skilldata.h"
#include <list>


const int FIELD_TYPE	=7;
	const int FIELD_NORMAL	=0;
	const int FIELD_FIRE	=1;
	const int FIELD_ICE		=2;
	const int FIELD_WIND	=3;
	const int FIELD_EARTH	=4;
	const int FIELD_POW		=5;
	const int FIELD_MAG		=6;



class CBattleMode {
	
//	bool	mDebugFlag;
protected:
	friend			class	CBattleMenu;
	friend			class	CTurnStart;
	friend			class	CPersonal;
	friend			class	CSelectSkill;
	friend			class	CSetTarget;
	friend			class	CMoveCharcter;
	friend			class	CResultExp;
	friend			class	CUseItem;
	friend			class	CCheck;
	friend			class	CEnemyCharacter;

	friend			class	CBattleAction;
		friend			class	CActCallPet;
		friend			class	CActMagicField;
		friend			class	CAct_82;		//グランドウォール
		friend			class	CAct_139;		//エスケープ
		friend			class	CActCallEnemy;
		friend			class	CAct_151;		//まやかしの魔術師
		friend			class	CAct_156;		//マグマフィールド
		friend			class	CAct_200_203;	//以下、夢の卵系
		friend			class	CAct_204;
		friend			class	CAct_206;

	int		mBattleOption;

	int		mTurn;

	int		mMode;

	int		mTimer;
	int		mModeTimer;
	int		mEscapeCount;

	int		mFieldShakeX;
	int		mFieldShakeY;
	int		mFieldFlash[BATTLE_SQUARE_WIDTH][BATTLE_SQUARE_Height];
	int		mFieldEffect[BATTLE_SQUARE_WIDTH][BATTLE_SQUARE_Height];
	int		mTargetLineFrom;
	int		mTargetLineTo;

	D3DCOLOR	mFieldColor;
	D3DCOLOR	mColorChangeGoal;
	int			mColorChangeTimer;

	int		mMagicField;

	CDTTexture	mField;
	CDTTexture	mFieldEffectTex;
	CDTTexture	mFieldBG;
	CDTTexture	mBackGround;
	bool		mDrawBG;

	CPlayerCharacter	mPlayer[MAX_PARTY_NUMBER];
	CEnemyCharacter		mEnemy[MAX_ENEMY_NUMBER];
	CPetCharacter		mPet[MAX_PET_NUMBER];
	CWallCharacter		mWall[MAX_WALL_NUMBER];
	CBattleCharacter* 	mCharacter[CHARACTER_NO];

	std::list<CBattleAction*>			mActionList;
	std::list<CBattleActionNameWindow>	mActionNameList;

	bool	mMoveCharacter[MAX_PARTY_NUMBER];
	int		mMoveX[MAX_PARTY_NUMBER];
	int		mMoveY[MAX_PARTY_NUMBER];

	int		mIntervalTimer;
	int		mIntervalType;

	int					mDMesID;
	CBattleMessage		mMessage;
	CBattleEffect		mTopEffect;
	CBattleEffect		mBottomEffect;

	CPlaneWindow		mWindow;
	CBattleMenu			mBattleMenu;
public:
	CBattleMode();
	void	AfterStartUp(void);
	void	LoadTexture(void);
	void	LoadBattle(void);

	void	OnTimer(void);
	void	OnDraw(void);

	int		GetPossibleTarget(int player,int act,int* target,int* possible=NULL);
	int		GetPossibleTarget(CBattleCharacter* character,int *target,int* possible=NULL);

	int					GetCharacterNo(CBattleCharacter* character);
	CBattleCharacter*	GetCharacter(int no){return mCharacter[no];};


	bool	SetCharacterMove(int x,int y,int character);
	void	ClearCharacterMove(int character);

	int		GetTurn(){return mTurn;};
	int		GetFieldEffect(int x,int y){return mFieldEffect[x][y];};
	CDTTexture* GetBackGroundTex(){return &mBackGround;};

private:
	int	    SetEnemy(int no,int enemy,int lv,int enemyfield[BATTLE_SQUARE_Height][BATTLE_SQUARE_Height],int param,int xrand=2);
	int	    SetPet(int no,int type,int master,int field[BATTLE_SQUARE_WIDTH][BATTLE_SQUARE_Height],int param,int xrand=1);

	void	DrawBattle(void);
	void	FlashField(int x,int y,D3DCOLOR color);

	void	ChangeMode(int mode);
	void	CheckBiology(void);

	void	SetInterval(int time,int type);
	void	TurnStart(void);
	void	Interval(void);
	void	SetActionList(int action,int acharacter);
	bool	AfterAction();
	bool	CheckBattleEnd(void);
	void	Action(void);
	void	EndAction(void);
	void	CancelAction(int target);
	void	TurnEnd(void);
	void	AfterTurnEnd(void);

	void	SetMessage(int target,const char* text,D3DCOLOR color){SetMessage(target,text,color,color);};
	void	SetMessage(int target,const char* text,D3DCOLOR color1,D3DCOLOR color2);
	void	SetNumberMessage(int target,int dmg,int type,const char* text,D3DCOLOR color,bool crt,int id);
	void	SetTotalMessage(int id);


	int*	Attack(int player,int* target,int targetno,int power,int minatk,int type,int element,int option,int sound,CBattleAction* act);
	void	ChargeBreak(int player);
	int		Attack(int player,int target,int power,int minatk,int type,int element,int option,int sound,int ratio,CBattleAction* act);
	int		GetElementPower(int player,int target,int element,int mainele,int option,int* selected=NULL);
	bool	CheckHit(int player,int target,int type,int option=0);
	void	Miss(int target);
//	int		GetDamage(int player,int target,int power,int type,int element,int option,int sound,int ratio=100);
	int		HealHP(int player,int target,int power,int dmesid,bool revival=false);
	int		HealLP(int player,int target,int power);
	int		LostLP(int player,int target,int power,int dmesid);
	int		HealSP(int player,int target,int power,int lv);
	int		LostSP(int player,int target,int power,int lv);
	int		ParamChange(int player,int target,int param,int up);
	int		SetStatus(int player,int target,int st,int hit,int param=-1,bool showmiss=true);
	bool	CureStatus(int player,int target);
	bool	DispelStatus(int player,int target);
	bool	SetCancel(int player,int target,int base);
	void	MoveCharacter(int target,int x,int y,bool move=true);
	bool	MoveNear(int target,int left,int right,int	field[BATTLE_SQUARE_WIDTH][BATTLE_SQUARE_Height],bool move);

	int*	GetTarget(int player,int center,int act,int *no,bool liveonly=false);
	int*	GetTarget(int player,int field[BATTLE_SQUARE_WIDTH][BATTLE_SQUARE_Height],int center,int *no,bool liveonly=false);
	int		SetRandomTarget(int player,int act);
	int		SetRandomAction(int player);

	bool	CheckEnemy(int player,int target=-1);	//実行者が敵かどうかを判定します。
	int		CheckTarget(CBattleCharacter* player,int target,int skill,int* fno=NULL,int *fc=NULL);
	void	CreateCharacterField(bool* character,int	field[BATTLE_SQUARE_WIDTH][BATTLE_SQUARE_Height]);
	int		CheckField(CBattleCharacter* character,int	field[BATTLE_SQUARE_WIDTH][BATTLE_SQUARE_Height]);
	int		CheckField(int x,int y,int size,int	field[BATTLE_SQUARE_WIDTH][BATTLE_SQUARE_Height]);
	void	GetAttackArea(int player,int x,int y,int size,int type,int	field[BATTLE_SQUARE_WIDTH][BATTLE_SQUARE_Height],int ratio=100);
	int*	GetTargetArea(int player,int target,int skill,int	field[BATTLE_SQUARE_WIDTH][BATTLE_SQUARE_Height],int* fno=NULL);

	void	ChangeFieldColor(D3DCOLOR color,int time);
public:
	double	GetShotSpeed(int player,int target,int spd,double *x,double *y);
	double	GetShotSpeed(int px,int py,int tx,int ty,int spd,double *x,double *y);
	double	GetShotSpeedByTime(int player,int target,int time,double *x,double *y);
	double	GetShotSpeedByTime(int px,int py,int tx,int ty,int time,double *x,double *y);
	int		GetRandomTarget(int player,int action);

	void	SetFieldEffect(int x,int y,int pow,int option,int time);

	/////////////////////// Action ////////////////

	void	Action_NormalAttack(int sound,int effect,int pow=100);
	void	Action_Thrust(void);
	void	Action_Shoot(int pow);
	void	Action_Laser(void);
	void	Action_NoSP(void);

	void	Action_SpinAttack(int pow,int min);

	void	Action_ParamUp(int param,D3DCOLOR color);
	void	Action_ParamDown(int param,D3DCOLOR color);
	void	Action_Cure(int size,int no,bool down,bool heal);
	void	Action_Gus(int st);
	void	Action_MagicField(int field);
	void	Action_Reflection(int pow);
	void	Action_Barrier(int pow);
	void	Action_Revival(int pow,bool live);
	void	Action_SkillDrain(int lv,int pow,D3DCOLOR color);
	void	Action_CallPet(int no);

	void	Action_DeathWord(int st,D3DCOLOR color);

//剣技
	void	Action_8(void);
	void	Action_9(void);
	void	Action_10(void);
	void	Action_12(void);
	void	Action_13(void);
	void	Action_14(void);
	void	Action_15(void);
	void	Action_16_17(int pow,int min,int hit,int opt,int img,int once,int se);

//槍技
	void	Action_19(void);
	void	Action_20(void);
	void	Action_21(void);
	void	Action_22(void);
	void	Action_23(void);
	void	Action_24(void);
	void	Action_25(void);
	void	Action_26(void);
	void	Action_27(void);

//斧技
	void	Action_28(int effect,int sound,int pow);
	void	Action_29(void);
	void	Action_30(void);
	void	Action_31(void);
	void	Action_32(void);
	void	Action_33_34(int param,int img);
	void	Action_35(void);
	void	Action_36(void);
	void	Action_37(void);

//弓技
	void	Action_39(void);
	void	Action_40(void);
	void	Action_41(void);
	void	Action_42(void);
	void	Action_43(void);
	void	Action_44(void);
	void	Action_45(void);
	void	Action_46(void);
	void	Action_47(void);
//格闘技
	void	Action_50(void);

//魔法
	void	Action_53(void);

//炎魔法
	void	Action_54(void);
	void	Action_55(void);
	void	Action_57(void);
	void	Action_58(void);
	void	Action_59(void);
	void	Action_60(void);
	void	Action_61(void);

//氷魔法
	void	Action_62(void);
	void	Action_63(void);
	void	Action_65(void);
	void	Action_66(void);
	void	Action_67(void);
	void	Action_68(void);
	void	Action_69(void);

//風魔法
	void	Action_70(void);
	void	Action_71(void);
	void	Action_73(void);
	void	Action_74(void);
	void	Action_75(void);
	void	Action_76(void);
	void	Action_77(void);

//土魔法
	void	Action_78(void);
	void	Action_79(void);
	void	Action_81(void);
	void	Action_82(void);
	void	Action_83(void);
	void	Action_84(void);
	void	Action_85(void);

//特殊魔法
	void	Action_86(void);
	void	Action_87(void);
	void	Action_88(void);

	void	Action_91(void);

//特殊行動
//回復系
	void	Action_92(void);
	void	Action_93(void);
	void	Action_95(void);

//ステータス異常系
	void	Action_103(void);
	void	Action_104(void);
	void	Action_105(void);
	void	Action_106(void);
	void	Action_107(void);

//キャラ専用
	void	Action_111(void);
	void	Action_112(void);

	void	Action_114(void);
	void	Action_115(void);
	void	Action_116(void);

//アイテム
	void	Action_128(void);
	void	Action_129(void);
	void	Action_130(int pow);
	void	Action_131(int pow);
	void	Action_132(int pow);
	void	Action_136(int pow);
	void	Action_137(int pow);
	void	Action_139(void);
	void	Action_140(void);

//敵専用
	void	Action_146(void);
	void	Action_147(void);

	void	Action_150(void);
	void	Action_151(void);
	void	Action_152(void);
	void	Action_153(void);
	void	Action_154(void);
	void	Action_156(void);
	void	Action_157(void);
	void	Action_158(void);
	void	Action_CallEnemy(int min,int max,int lv);
	void	Action_168(void);
	void	Action_170(void);
	void	Action_171(void);
	void	Action_172(void);
	void	Action_173(void);
	void	Action_174(void);
	void	Action_Song(int st,D3DCOLOR color);
	void	Action_180(void);
	void	Action_181(void);
	void	Action_182(void);
	void	Action_183(void);
	void	Action_184(void);
	void	Action_185(void);
	void	Action_186(void);
	void	Action_187(void);
	void	Action_188(void);
	void	Action_198(void);
	void	Action_199(void);

//夢の卵専用
	void	Action_200_203(int eneparty,bool endbattle);
	void	Action_204(void);
	void	Action_206(void);
};

extern D3DCOLOR BattleFieldColor[FIELD_TYPE];