#pragma once

#include "dataconst.h"

const int TREASURE_NO	=512;
const int TRAP_NO		=32;

const int TRAP_LP		=0;
const int TRAP_ATK		=1;
const int TRAP_DEF		=2;
const int TRAP_MATK		=3;
const int TRAP_MDEF		=4;
const int TRAP_SPD		=5;
const int TRAP_TECH		=6;
const int TRAP_IMP		=7;



class CTreasureData{
public:
	int		mItemNo;
	int		mKey;
	int		mTrap;
	int		mTrapLv;
	int		mExpLv;
	int*	mItem;

public:
	CTreasureData(void);
	CTreasureData(int key,int trap,int traplv,int explv,int item,...);
	~CTreasureData(void);
	int		GetTrapLv(void)const;

};

class CTrapData{
public:
	char*	mName;
	char*	mHelp;
	int		mParam;
	bool	mGetItem;

public:
	CTrapData(void);
	CTrapData(char* name,int param,bool item,char* help);
};

extern const CTreasureData	TreasureData[TREASURE_NO];
extern const CTrapData		TrapData[TRAP_NO];
