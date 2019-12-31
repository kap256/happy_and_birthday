#pragma once

#include "stdio.h"
#include "element.h"
#include "skilldata.h"
#include "dataconst.h"


const int ID_CS		=0;
const int ID_AS		=1024;

const int		MAX_LV				=60;
const int		LV_UP_EXP			=1000;
const int		LV_UP_EXP_RATIO_LANK	=6;
	const double	LV_UP_EXP_RATIO		[LV_UP_EXP_RATIO_LANK]=
							{1.25	,1.25	,1.15	,1.05	,1.02	,1.05};
	const int		LV_UP_EXP_RATIO_LV	[LV_UP_EXP_RATIO_LANK]=
							{0		,5		,5		,5		,5		,255};	//前の段階とのレベル差を表記。

const double	EXP_DOWN_RATIO			=0.98;
const double	EXP_UP					=0.8;
const int		MAX_EXP = 9999;
const int		MIN_EXP = 1;
const int		TRESURE_EXP		=150;
const int		TRESURE_EXP_MIN	=100;

const int CHAREQ_WEAPON		=1	*	0x100;
const int CHAREQ_ARMOR		=2	*	0x100;
const int CHAREQ_ACCESSORY	=3	*	0x100;

const int CHAREQ_FROM_START	=1	*	0x10000;

enum GROW_PARAM
{
	GROW_STM	,GROW_ATK	,GROW_DEF	,GROW_MATK	,GROW_MDEF	,GROW_TECH	,GROW_SPD
};

struct CarryItem{
	int item;
	int param;

	CarryItem(){};
	CarryItem(int i){item = i; param = 0;}

	bool Spend();
	bool operator !=(int i){return item != i;}
	bool operator ==(int i){return item == i;}
};

class CCharacterBaseData:public CElement{
private :
	char*	mFamilyName;
	char*	mEggName;
public:
	int		mStartLv;
	int		mLP;
	int		mTech;	
	int		mAtk;
	int		mDef;
	int		mMagAtk;
	int		mMagDef;
	int		mSpeed;

public:
	CCharacterBaseData(void);
	CCharacterBaseData(char* name,int id,char* fname,char* ename,int startlv,int lp,int tech,int atk,int def,int matk,int mdef,int spd);

	const char*	GetName()const;
	const char*	GetFamilyName()const;
	void		Help(int param) const;
};


extern const CCharacterBaseData		CharacterBaseData[MAX_CHARACTER_NUMBER];


class CCharacterData:public CElement{
#ifdef _DEBUG
	friend void DebugOutput();
	friend void OutputDBCharaParam(FILE *pFile,int lv,int param[9]);
#endif
protected:
	int		mLv;
	double	mExp;

	int		mLP;
	int		mLostLP;
	double	mMaxSP[MAX_CSKILL_LEVEL];
	int		mLostSP[MAX_CSKILL_LEVEL];

	int		mStm;
	int		mAtk;
	int		mDef;
	int		mMagAtk;
	int		mMagDef;
	int		mSpeed;
	int		mTech;

	int		mSize;

	int		mCSkill[MAX_CSKILL];
	int		mASkill[MAX_ASKILL];

	int		GetCSkillAllPrivate(int type,int* skill=NULL,bool* sp=NULL,int *param=NULL);
public:
//	int		mLv;
	CCharacterData();
			void	Init(int id,const char* name,int lv,int stm,int atk,int def,int matk,int mdef,int spd,int tech,int size=1);
	virtual void	Draw(CPlaneWindow* window,int x,int y,int param,D3DCOLOR color=0xffffffff)const{CElement::Draw(window,x,y,param,color);};

			void	CalculateSP(void);
	virtual	int		CalculateLP(void);
	virtual	void	CalculateSkill(void);
			void	CheckParamMax(void);

	virtual int		GetCSkillAll(int type,int* skill=NULL,bool* sp=NULL,int *param=NULL);
			int		GetCSkill(int no)		{if(no<0 || MAX_CSKILL<=no)	return -1; return mCSkill[no];};
			int		GetASkillAll(int* skill=NULL);
	virtual int		GetSkillParam(int no)	{return 0;};
			bool	SearchASkill(int no);
			int		SearchASkillType(int type,bool add);
	virtual void    ASkillOpen(int no)		{return;};

	int		GetLv()			{return mLv;};
	int		GetExp()		{return int(mExp);};

	int		GetMaxLP()		{return mLP;};
	void	SetMaxLP(int lp){mLP=lp;};
	int		GetLP()			{if(mLP<=mLostLP)return 1;return mLP-mLostLP;};

	int		GetMaxSP(int no){if(no<0||MAX_CSKILL_LEVEL<=no)	return 0;return int(mMaxSP[no]);};
	int		GetSP(int no)	{if(no<0||MAX_CSKILL_LEVEL<=no)	return 0;
							if(mMaxSP[no]<=mLostSP[no])		return 0;
							return int(mMaxSP[no])-mLostSP[no];};

	int		GetStm()		{return mStm;};
	int		GetAtk()		{return mAtk;};
	int		GetDef()		{return mDef;};
	int		GetMagAtk()		{return mMagAtk;};
	int		GetMagDef()		{return mMagDef;};
	int		GetSpeed()		{return mSpeed;};
	int		GetTech()		{return mTech;};
	int		GetSize()		{return mSize;};

	void	SetAtk(int i)	{mAtk=i;};
	void	SetDef(int i)	{mDef=i;};
	void	SetMagAtk(int i){mMagAtk=i;};
	void	SetMagDef(int i){mMagDef=i;};
	void	SetSpeed(int i)	{mSpeed=i;};


	void	LostLP(int lost);
	void	HealLP(int heal);
	bool	LostSP(int lv,int pow);
	void	HealSP(int lv,int heal);

	int		SetCSkill(int skill);
	int		SetASkill(int skill);
	int		ClearASkill(int skill);

	void	Help(int param) const;
};

class CPlayerData:public CCharacterData{
public:
	static double CulcExp(double base,int plv,int elv,bool tresure);
private:
	int		mBaseLP;
	int		mBaseStm;
	int		mBaseAtk;
	int		mBaseDef;
	int		mBaseMagAtk;
	int		mBaseMagDef;
	int		mBaseSpeed;
	int		mBaseTech;

	CarryItem	mItem[MAX_ITEM];
	int		mWeapon;
	int		mSecondWeapon;
	int		mArmor;
	int		mAccessory[MAX_ACCESSORY+1];
	int		mMaxItem;
	int		mMaxUseItem;
	int		mMaxAccessory;

	int		mSkill[MAX_MONTH][MAX_LEARN_MONTH+1];
	int		mFreeSpace[MAX_FREE_SPACE];

public:
	CPlayerData();
	void	Init(int no);
	void	AllHeal(void);
	void	Calculate(void);
	int		CalculateLP(void);
	void	CalculateSkill(void);
	void	SetSkill(int skill);

	void	GetSkillSpace(int sspace[MAX_MONTH]);
	int		GetLearnSkillNo(int month);
	void	GetLearnSkill(int month,int* list);
	bool	CheckLearnSkill(int month,int no);
	bool	CheckLearnSkill(int skill);
	int		LearnSkill(int month,int no);
	bool	ClearAllLearnSkill();
	bool	ClearAllStarSkill();
	bool	ClearMonthLearnSkill(int month);
	bool	ClearLearnSkill(int mmin,int mmax,bool normal);

	int		GetEquipment(int type);
	bool	CheckEquiped(int no);
	bool	Equip(int no,bool force=false);
	bool	TakeOff(int no);
	int		GetCSkillAll(int type,int* skill=NULL,bool* sp=NULL,int *param=NULL);

	const char*	GetName()			{return CharacterBaseData[mID].GetName();};
	const char*	GetFamilyName()		{return CharacterBaseData[mID].GetFamilyName();};
	int		GetBaseLP()		{return mBaseLP;};
	int		GetBaseStm()	{return mBaseStm;};
	int		GetBaseAtk()	{return mBaseAtk;};
	int		GetBaseDef()	{return mBaseDef;};
	int		GetBaseMagAtk()	{return mBaseMagAtk;};
	int		GetBaseMagDef()	{return mBaseMagDef;};
	int		GetBaseSpeed()	{return mBaseSpeed;};
	int		GetBaseTech()	{return mBaseTech;};

	CarryItem	GetItem(int no)		{if(no<0||mMaxItem<=no)	return NO_ITEM;return mItem[no];};
	int		GetItemMax()		{return mMaxItem;};
	int		GetItemCurrent();
	int		GetUseItemMax()		{return mMaxUseItem;};
	int		GetUseItemCurrent();
	int		GetAccessoryMax()	{return mMaxAccessory;};
	int		GetAccessoryCurrent();
	int		GetEquipMax()		{return mMaxAccessory+WEAPON_SPACE+ARMOR_SPACE;};
	int		GetWeapon()			{if(mWeapon>=0)	return mItem[mWeapon].item;	return NO_ITEM;};
	int		GetSecondWeapon()	{if(mSecondWeapon>=0)	return mItem[mSecondWeapon].item;	return NO_ITEM;};
	int		GetSecondWeaponNo()	{return mSecondWeapon;};

	bool	SetExp(double &exp,double get);

	bool	CheckItemSpace(int itme);
	int		SearchItem(int item);
	int		SetItem(int item);
	int		SetItem(CarryItem item);
	void	SetItem(int no,int item);
	void	SetItem(int no,CarryItem item);
	bool	SpendUseItem(int no);

	void	DeleteNoEquipedItem(void);
	void	CollectItem(int no);
	void	OrderItem(void);

	int		GetFSpace(int no)			{return mFreeSpace[no];};
	void	SetFSpace(int no,int skill)	{mFreeSpace[no]=skill;Calculate();};

	void	Help(int param) const;

	void	Save(FILE* pFile);
	void	Load_0(FILE* pFile);
	void	Load_1(FILE* pFile);
	void	Load_2(FILE* pFile);
	void	Load_4(FILE* pFile);
	void	Load_5(FILE* pFile);
	void	Load_8(FILE* pFile);
private:
	void	CalculateEquipment(int no);
};
class CEnemyData:public CCharacterData{
public:
	static double	GetGrowRatio(int lv,GROW_PARAM param);
public:
	int		mNo;

	int		mSkillParam[MAX_CSKILL];
	int		mThinkType;
	int		mInt;
	int		mMoney;

public:
	CEnemyData();
	void	Draw(CPlaneWindow* window,int x,int y,int param,D3DCOLOR color=0xffffffff) const;
	//int		GetCSkillAll(int type,int* skill=NULL,bool* sp=NULL,int *param=NULL);
	int		GetSkillParam(int no){return mSkillParam[no];};
	int		GetMoney()	{return mMoney;};
	void    ASkillOpen(int no);

	void	LoadEnemyData(int no,int lv);
	void	CulculateLvRevision(int oldlv,int newlv,int* param,double grow,GROW_PARAM growratio);
};


class CPetData:public CElement{
public:
	int		mImg;
	int		mSize;
	int		mLine;

	int		mLP;	//全て百分率
	int		mTech;	
	int		mAtk;
	int		mDef;
	int		mMagAtk;
	int		mMagDef;
	int		mSpeed;

	int		mSkillNo;
	int*	mSkill;

public:
	CPetData(void);
	CPetData(char* name,int id,int img,int size,int line,int lp,int atk,int def,int matk,int mdef,int spd,int tech,
			int skill, ...);
	~CPetData(void);

	void		Help(int param) const;
};

extern const int		CharacterSkill[MAX_CHARACTER_NUMBER][MAX_MONTH][MAX_SKILL_MONTH];
extern const int		StartSkill[MAX_CHARACTER_NUMBER][MAX_MONTH][MAX_LEARN_MONTH+1];
extern const int		StartItem[MAX_CHARACTER_NUMBER][MAX_ITEM];
extern const CPetData	PetData[MAX_PET_TYPE];
