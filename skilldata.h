#pragma once

#include "element.h"


const int MAX_SKILL_NUMBER	=512;  
const int MAX_CSKILL_NUMBER	=256;
const int MAX_STATUS_NUMBER	=32;  
const int STATUS_TEX_NUMBER	=MAX_STATUS_NUMBER/16;  

const int MAX_CSKILL_LEVEL	=4;
const int ITEM_CSKILL		=127;

const int 	SKILL_Height	=26;

enum SkillType{
				SKILL_STATUS,
				SKILL_COMMAND,
				SKILL_AUTO,
				SKILL_FIELD,
				SKILL_NONE};

class CSkillData:public CElement{
public:
	SkillType		mType;
	int				mParam;

public:
	CSkillData(void);
	CSkillData(const char* name,int id,SkillType type,int param);
	int		GetHeight(void)const{return SKILL_Height;};
	void	Draw(CPlaneWindow* window,int x,int y,int param,D3DCOLOR color=0xffffffff) const;
	void	Help(int param) const;
	virtual const char*	GetName()const;
};

const int 	CS_SKILL		=0x001;
const int	CS_SWORD		=0x002;
const int	CS_SPEAR		=0x004;
const int 	CS_AXE			=0x008;
const int 	CS_BOW			=0x010;
const int 	CS_MAGIC		=0x020;
const int 	CS_OTHER		=0x040;
const int 	CS_ITEM			=0x080;
const int 	CS_ENEMY_ITEM	=0x100;
	const int 	CS_EITEM		=CS_ENEMY_ITEM | CS_ITEM;

const int 	CS_MAGIC_FIRE	=CS_MAGIC + 0x01000;
const int 	CS_MAGIC_ICE	=CS_MAGIC + 0x02000;
const int 	CS_MAGIC_WIND	=CS_MAGIC + 0x03000;
const int 	CS_MAGIC_EARTH	=CS_MAGIC + 0x04000;

const int 	CS_OTHER_HEAL	=CS_OTHER + 0x01000;
const int 	CS_OTHER_FIELD	=CS_OTHER + 0x02000;
const int 	CS_OTHER_ST		=CS_OTHER + 0x03000;
const int 	CS_OTHER_PET	=CS_OTHER + 0x04000;

const int 	CS_NO_MOVE		=0x000*0x100000;
const int 	CS_MOVE			=0x001*0x100000;
const int 	CS_QUICK		=0x002*0x100000;
const int 	CS_SLOW			=0x004*0x100000;
const int 	CS_NO_WALK		=0x008*0x100000;
const int 	CS_RIVIVAL		=0x010*0x100000;
const int 	CS_SUPER_SLOW	=0x040*0x100000;
const int 	CS_SUPER_QUICK	=0x080*0x100000;
const int 	CS_CALL_PET		=0x100*0x100000;
const int 	CS_TLOCK_HARD	=0x200*0x100000;
const int 	CS_TLOCK_VHARD	=0x400*0x100000;

const int 	CS_MOVE_NO_WALK	=CS_NO_WALK | CS_MOVE;
const int 	CS_QUICK_MOVE	=CS_QUICK | CS_MOVE;
const int 	CS_QUICK_NO_WALK=CS_QUICK | CS_MOVE | CS_NO_WALK;
const int 	CS_SLOW_MOVE	=CS_SLOW | CS_MOVE;
const int 	CS_SLOW_NO_WALK	=CS_SLOW | CS_MOVE | CS_NO_WALK;

const int 	CR_NONE			=0;
const int 	CR_SELF			=1;
const int 	CR_ONE			=2;
const int 	CR_ONE_M		=4;
const int 	CR_VLINE		=8;
const int 	CR_HLINE		=16;
const int 	CR_CIRCLE		=32;
const int 	CR_BIG_CIRCLE	=64;
const int 	CR_ALL			=128;
const int 	CR_S_ALL		=256;

	const int 	AS_LP_UP			=1*0x10000;
	const int 	AS_TECH_UP			=2*0x10000;
	const int 	AS_ATK_UP			=3*0x10000;
	const int 	AS_DEF_UP			=4*0x10000;
	const int 	AS_MATK_UP			=5*0x10000;
	const int 	AS_MDEF_UP			=6*0x10000;
	const int 	AS_SPD_UP			=7*0x10000;
	const int 	AS_PARAM_UP_OTHER	=8*0x10000;

	const int 	AS_FIRE_GUARD	=10*0x10000;
	const int 	AS_ICE_GUARD	=11*0x10000;
	const int 	AS_WIND_GUARD	=12*0x10000;
	const int 	AS_EARTH_GUARD	=13*0x10000;
	const int 	AS_NO_ELE_GUARD	=14*0x10000;
	const int 	AS_ELE_HEAL		=15*0x10000;
	const int 	AS_ADD_ELEMENT	=16*0x10000;
	const int 	AS_ELE_POW_UP	=17*0x10000;
	const int 	AS_ELE_OTHER	=18*0x10000;

	const int 	AS_STATUS_GUARD	=21*0x10000;
	const int 	AS_SPECIAL_GUARD=22*0x10000;
	const int 	AS_ADD_STATUS	=23*0x10000;
	const int 	AS_ADD_STATUS_OT=24*0x10000;
	const int 	AS_ADD_ST_DOWN	=25*0x10000;
	const int 	AS_ANTI_STATUS	=26*0x10000;
	const int 	AS_ST_OTHER		=27*0x10000;

	const int 	AS_CRT_UP		=30*0x10000;
	const int 	AS_ANTI_ATTACK	=31*0x10000;
	const int 	AS_CRT_OTHER	=32*0x10000;
	const int 	AS_AVOID_UP		=33*0x10000;

	const int 	AS_LP_GUARD		=41*0x10000;
	const int 	AS_MONEY_UP		=42*0x10000;
	const int 	AS_EXP_UP		=43*0x10000;
	const int 	AS_EXP_UP_OTHER	=44*0x10000;
	const int 	AS_DEF_BREAK	=45*0x10000;
	const int 	AS_MDEF_BREAK	=46*0x10000;
	const int 	AS_ELEMENT_BREAK=47*0x10000;
	const int 	AS_ST_GUARD_BREAK=48*0x10000;

	const int 	AS_WEAPON_MASTER=50*0x10000;

	const int 	AS_DMG_BOOST	=66*0x10000;
	const int 	AS_PET_SKILL	=67*0x10000;

	const int 	AS_FREE_SPACE	=128*0x10000;
	const int 	AS_FS_OTHER		=129*0x10000;


	const int 	AS_HP_HEAL_UP	=130*0x10000;
	const int 	AS_SPECIAL_UP	=131*0x10000;
	const int 	AS_ACT_ORDER	=132*0x10000;

	const int 	AS_OPTION		=140*0x10000;

	const int 	AS_MULTI_ACTION	=151*0x10000;
	const int 	AS_MOVE			=152*0x10000;
	const int 	AS_COUNTER		=153*0x10000;

	const int 	AS_OTHER		=256*0x10000;
	const int 	AS_BANSEI		=257*0x10000;

const int MAX_QUICK_LOAD = 4;
class CComSkillData:public CElement{
public:
	int		mType;
	int		mSP;
	bool	mTarget;
	int		mRange;
	bool	mDeadEnd;
	int		mQuickLoad[MAX_QUICK_LOAD];
public:
	CComSkillData(void);
	CComSkillData(const char* name,int id,int type,int sp,bool target,int range,int qload, ...);
	void	Draw(CPlaneWindow* window,int x,int y,int param,D3DCOLOR color=0xffffffff) const;
	void	Help(int param) const;
	unsigned int GetSortNo(void) const;
};

class CStatusData:public CElement{
public:
	const char*	mEnglish;
	int			mParam;
	bool		mBad;
	bool		mShowParam;
	D3DCOLOR	mColor;
	int			mSE;
public:
	CStatusData(void);
	CStatusData(const char* name,int id,const char* english,bool bad,bool show,int param,D3DCOLOR color,int se);

	int		GetHeight(void)const{return SKILL_Height;};
	void	Draw(CPlaneWindow* window,int x,int y,int param,D3DCOLOR color=0xffffffff) const;

	void	Help(int param) const;
};


extern const CSkillData		SkillData[MAX_SKILL_NUMBER];
extern const CComSkillData	ComSkillData[MAX_CSKILL_NUMBER];
extern const CStatusData	StatusData[MAX_STATUS_NUMBER];
extern const CSkillData 	NoneSkillData;

