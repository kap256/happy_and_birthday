#pragma once

#ifndef _EVENT_EDITER_
#include "DTestLib/dtest.h"
#endif

#include "element.h"
#include "dataconst.h"

const int				RARE_ITEM	= -1;


class CPlaneWindow;

const int ITEM_Height			=26;


const int TYPE_EQUIPMENT	=0x10000000;
	const int TYPE_WEAPON		=0x01000000 | TYPE_EQUIPMENT;
		const int TYPE_SWORD		=2    | TYPE_WEAPON;
		const int TYPE_SPEAR		=4    | TYPE_WEAPON;
		const int TYPE_AXE			=8    | TYPE_WEAPON;
		const int TYPE_BOW			=16   | TYPE_WEAPON;
		const int TYPE_LASER		=128  | TYPE_WEAPON;	//32とかにすると、魔法などと衝突するので、大きいままで。
	const int TYPE_ARMOR		=0x02000000 | TYPE_EQUIPMENT;
	const int TYPE_ACCESSORY	=0x04000000 | TYPE_EQUIPMENT;
		const int TYPE_SHIELD		=2    | TYPE_ACCESSORY;
		const int TYPE_HEAD			=4    | TYPE_ACCESSORY;
		const int TYPE_GLOVE		=8    | TYPE_ACCESSORY;
		const int TYPE_SHOES		=16   | TYPE_ACCESSORY;
		const int TYPE_TAILSMAN		=32   | TYPE_ACCESSORY;
		const int TYPE_ACC_OTHER	=0    | TYPE_ACCESSORY;

const int TYPE_USE_ITEM		=0x20000000;

	const int TYPE_ON_MENU				=0x08000000;
	const int TYPE_ON_BATTLE			=0x04000000;
	const int TYPE_SELECT_CHARCTER		=0x02000000;
	const int TYPE_SPEND				=0x01000000;
	const int TYPE_COLLECT				=0x00800000;
	const int TYPE_RARE					=0x00400000;
	const int TYPE_SEND_CLOSET			=0x00200000;

	const int TYPE_LP_HEAL				=1 | TYPE_ON_MENU | TYPE_ON_BATTLE | TYPE_SELECT_CHARCTER | TYPE_SPEND;
	const int TYPE_SP_HEAL				=2 | TYPE_ON_MENU | TYPE_ON_BATTLE | TYPE_SELECT_CHARCTER | TYPE_SPEND;
	const int TYPE_SMALL_BOMB			=3 | TYPE_ON_BATTLE | TYPE_SELECT_CHARCTER | TYPE_SPEND;
	const int TYPE_BIG_BOMB				=4 | TYPE_ON_BATTLE | TYPE_SELECT_CHARCTER | TYPE_SPEND;
	const int TYPE_HP_HEAL				=5 | TYPE_ON_BATTLE | TYPE_SELECT_CHARCTER | TYPE_SPEND;
	const int TYPE_STATUS_HEAL			=6 | TYPE_ON_BATTLE | TYPE_SELECT_CHARCTER | TYPE_SPEND;
	const int TYPE_REVIVAL				=7 | TYPE_ON_BATTLE | TYPE_SELECT_CHARCTER | TYPE_SPEND;
	const int TYPE_INSTANT_BARRIER		=8 | TYPE_ON_BATTLE | TYPE_SELECT_CHARCTER | TYPE_SPEND;
	const int TYPE_SABOTEN				=9 | TYPE_ON_BATTLE | TYPE_SELECT_CHARCTER | TYPE_SPEND;
	const int TYPE_WAKAME				=10 | TYPE_ON_BATTLE | TYPE_SELECT_CHARCTER | TYPE_SPEND;
	const int TYPE_GLASS				=11 | TYPE_ON_BATTLE | TYPE_SELECT_CHARCTER | TYPE_SPEND;
	const int TYPE_ESCAPE				=12 | TYPE_ON_BATTLE | TYPE_SELECT_CHARCTER | TYPE_SPEND;
	const int TYPE_GUARD_BREAK			=13 | TYPE_ON_BATTLE | TYPE_SELECT_CHARCTER | TYPE_SPEND;
	const int TYPE_DMG_BOOST			=14 | TYPE_ON_BATTLE | TYPE_SELECT_CHARCTER | TYPE_SPEND;

	const int TYPE_SAVE					=32 | TYPE_ON_MENU | TYPE_SPEND;
	const int TYPE_CLOSET				=33 | TYPE_ON_MENU | TYPE_SPEND;

	const int TYPE_BIRTH_STONE			=100 | TYPE_ON_MENU | TYPE_COLLECT | TYPE_SEND_CLOSET;
	const int TYPE_KEY					=101 | TYPE_COLLECT;
	const int TYPE_NO_USE				=256;
	const int TYPE_MONEY				=512;


const int TYPE_SEED			=0x40000000;


class CItemData:public CElement{
protected:
	int				mPrice;
public:
	unsigned int	mType;
	int				mIcon;

public:
	CItemData(void);
	CItemData(const char* name,int id,int icon,int price,int type);
	void	Draw(CPlaneWindow* window,int x,int y,int param,D3DCOLOR color=0xffffffff) const;
	int		GetHeight(void)const{return ITEM_Height;};
	virtual void	Help(int param) const;
	int		GetSellPrice(void) const;
	int		GetPrice(void) const;
	bool	GetRare(void) const{return mPrice==RARE_ITEM;};
	const char* GetTypeName(void) const;
	const int	GetCategory(void) const;
};

const int MAX_WEAPON_SKILL	=8;


class CEquipmentData:public CItemData{
public:
	int		mLP;
	int		mAtk;
	int		mDef;
	int		mMagAtk;
	int		mMagDef;
	int		mSpeed;
	int		mSkill[MAX_WEAPON_SKILL];
	int		mSkillNo;
public:
	CEquipmentData(void);
	CEquipmentData(const char* name,int id,int icon,int price,int type,
				int lp,int atk,int def,int matk,int mdef,int speed,
				int skill, ...);
	void	Help(int param) const;
};

class CUseItemData:public CItemData{
public:
	int		mParamA;
	int		mParamB;
	int		mUseCount;
public:
	CUseItemData(void);
	CUseItemData(const char* name,int id,int icon,int price,
				int type,int parama,int paramb,int count = 0);
};

class CSeedData:public CItemData{
public:
	CSeedData(void);
	CSeedData(int id,int type);
};

class CItemCategoryData:public CElement{
public:
	int	mMin;
	int	mMax;

public:
	CItemCategoryData(const char* name,int id,int min,int max);
	//void	Draw(CPlaneWindow* window,int x,int y,int param,D3DCOLOR color=0xffffffff) const;
};

extern const CItemData 		NoneItemData;
extern const CItemData *	AllItem[MAX_ITEM_NUMBER+1];
extern const CEquipmentData	EquipmentData[MAX_EQUIPMENT_NUMBER];
extern const CUseItemData	UseItemData[MAX_USE_ITEM_NUMBER];
extern const CSeedData		SeedData[MAX_SEED_NUMBER];

extern const CItemCategoryData 		ItemCategory[CATEGORY_MAX];

extern const int			ShopData[MAX_SHOP_NO][MAX_MONTH][MAX_SHOP_ITEM];

extern CDTTexture	ItemTex[ITEM_TEX_NUMBER];