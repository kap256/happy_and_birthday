#pragma once

#include "characterdata.h"
#include "skilldata.h"
#include "battleconst.h"

const int THINK_AOPT_HALF_INT		=0x0100;
const int THINK_AOPT_HALF_INT_1TURN	=0x0200;
const int THINK_AOPT_MA_NO_CONTINUE	=0x0400;

const int THINK_ACT_RANDOM			=0x00;
const int THINK_ACT_PRIORITY		=0x01;
const int THINK_ACT_PRI_HALF_INT	=THINK_ACT_PRIORITY | THINK_AOPT_HALF_INT;
const int THINK_ACT_ROTATE_PRI		=0x03;
const int THINK_ACT_MA_ROTATE		=0x04;
const int THINK_ACT_NIGHT_KING		=0x05;
const int THINK_ACT_DETH_MEMBER		=0x06;
const int THINK_ACT_MA_ROTATE_16	=0x07;
const int THINK_ACT_LEFT_HP			=0x08;
const int THINK_ACT_LEFT_HP_ROTATE	=0x09;
const int THINK_ACT_DETH_MEM_ROTATE	=0x0a;
const int THINK_ACT_DREAM_EGG		=0x0b;

const int THINK_TOPT_HALF_INT		=0x01000000;
const int THINK_TOPT_QUARTER_INT	=0x02000000;
const int THINK_TOPT_NO_TARGETING	=0x04000000;

const int THINK_TGT_RANDOM		=0x00000000;
const int THINK_TGT_MOST_DAMAGE	=0x00010000;
const int THINK_TGT_MOST_HP		=0x00020000;
const int THINK_TGT_LOW_HP		=0x00030000;
const int THINK_TGT_HI_LEVEL	=0x00040000;
const int THINK_TGT_MULTI_HIT	=0x00050000;
const int THINK_TGT_MOST_LP		=0x00060000;
const int THINK_TGT_FEW_ST		=0x00070000;
const int THINK_TGT_MOST_ST		=0x00080000;
const int THINK_TGT_HI_ATK		=0x00090000;
const int THINK_TGT_LOW_FEFFECT	=0x000a0000;
const int THINK_TGT_LARGE_SIZE	=0x000b0000;
const int THINK_TGT_HI_DEF		=0x000c0000;
const int THINK_TGT_LEFT		=0x000d0000;	//左のほうのやつをターゲットにする。
const int THINK_TGT_SELF		=0x000e0000;

const int ENEMY_NO			=128;
const int ENEMY_PARTY_NO	=80;
const int DROPITEM_NO		=4;
//const int AS				=MAX_CSKILL_NUMBER;

const int SMALL				=1;
const int LARGE				=2;

class CEnemyBaseData{
public:
	const char* mName;
	int		mExp;
	int		mMoney;

	int		mMoveType;
	int		mMapImage;

	int		mSize;
	int		mThinkType;
	int		mInt;
	int		mLine;

	int		mBaseLP;
	int		mBaseAtk;
	int		mBaseDef;
	int		mBaseMagAtk;
	int		mBaseMagDef;
	int		mBaseSpeed;
	int		mBaseTech;

	double	mGrowLP;
	double	mGrowAtk;
	double	mGrowDef;
	double	mGrowMagAtk;
	double	mGrowMagDef;
	double	mGrowSpeed;
	double	mGrowTech;

	int		mSkillNo;
	int*	mSkill;
	int*	mLearnLv;
	unsigned int*	mSkillParam;


public:
	CEnemyBaseData(void);
	CEnemyBaseData(const char* name,int exp,int money,int line,
				int	movetype,int mapimage,
				int size,int think,int intel,
				int baselv,
				int blp,int batk,int bdef,int bmatk,int bmdef,int bspd,int btech,
				double glp,double gatk,double gdef,double gmatk,double gmdef,double gspd,double gtech,
				int skill, ...);
	~CEnemyBaseData(void);
};
class CEnemyPartyData{
public:

	int mEnemy[MAX_ENEMY_NUMBER];
	int mLv[MAX_ENEMY_NUMBER];
	int mX[MAX_ENEMY_NUMBER];
	int mY[MAX_ENEMY_NUMBER];
	int mField;
	int mOption;

public:
	CEnemyPartyData(void);
	CEnemyPartyData(int field,int option,int data, ...);
};

extern const CEnemyBaseData		EnemyBaseData[ENEMY_NO];
extern const CEnemyPartyData	EnemyPartyData[ENEMY_PARTY_NO];
