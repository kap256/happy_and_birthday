#pragma once
#include "stdio.h"
#include "characterdata.h"

const int SC_NONE		=0;
const int SC_PERSONAL	= 0x80000000;
	const int SC_P_TOP_MENU		= 0x000 | SC_PERSONAL;
	const int SC_P_SKILL_SWORD	= 0x100 | SC_PERSONAL;
	const int SC_P_SKILL_SPEAR	= 0x200 | SC_PERSONAL;
	const int SC_P_SKILL_AXE	= 0x300 | SC_PERSONAL;
	const int SC_P_SKILL_BOW	= 0x400 | SC_PERSONAL;
	const int SC_P_SKILL_OTHER	= 0x500 | SC_PERSONAL;
	const int SC_P_MAGIC		= 0x600 | SC_PERSONAL;
	const int SC_P_ITEM			= 0x700 | SC_PERSONAL;
	const int SC_P_PLAYER_TARGET= 0x800 | SC_PERSONAL;
	const int SC_P_ENEMY_TARGET	= 0x900 | SC_PERSONAL;

class CSaveCursorPersonal{
private:
	int mTopMenu;

	int mSkill[5];
	int mMagic;
	int mSpecial;
	int mItem;

	int mPlayerTarget;
	int mEnemyTarget;
public:
	void	Save(FILE* pFile);
	void	Load_0(FILE* pFile);

	int*	GetPointer(int id);
};


class CSaveCursor{
private:
	CSaveCursorPersonal mPersonal[MAX_CHARACTER_NUMBER];
public:
	void	Save(FILE* pFile);
	void	Load_0(FILE* pFile);

	int		Get(int id);
	void	Set(int id,int set);
private:
	int*	GetPointer(int id);
};