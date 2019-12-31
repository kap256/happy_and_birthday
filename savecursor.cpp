#include "savecursor.h"
#include "main.h"
#include "flag.h"


void CSaveCursorPersonal::Save(FILE* pFile)
{
	_putw(mTopMenu,pFile);

	for(int i=0;i<5;i++){
		_putw(mSkill[i],pFile);
	}
	_putw(mMagic,pFile);
	_putw(mSpecial,pFile);
	_putw(mItem,pFile);
}
void CSaveCursorPersonal::Load_0(FILE* pFile)
{
	mTopMenu=_getw(pFile);

	for(int i=0;i<5;i++){
		mSkill[i]=_getw(pFile);
	}
	mMagic=_getw(pFile);
	mSpecial=_getw(pFile);
	mItem=_getw(pFile);
}
int* CSaveCursorPersonal::GetPointer(int id)
{
	switch(id & 0xffffff00){
		case SC_P_TOP_MENU:		return &mTopMenu;
		case SC_P_SKILL_SWORD:	return &mSkill[0];
		case SC_P_SKILL_SPEAR:	return &mSkill[1];
		case SC_P_SKILL_AXE:	return &mSkill[2];
		case SC_P_SKILL_BOW:	return &mSkill[3];
		case SC_P_SKILL_OTHER:	return &mSkill[4];
		case SC_P_MAGIC:		return &mMagic;
		case SC_P_ITEM:			return &mItem;
		case SC_P_PLAYER_TARGET:return &mPlayerTarget;
		case SC_P_ENEMY_TARGET:	return &mEnemyTarget;
	}
	return NULL;
}
/////////////////////////////////////////////////
void CSaveCursor::Save(FILE* pFile)
{
	for(int i=0;i<MAX_CHARACTER_NUMBER;i++){
		mPersonal[i].Save(pFile);
	}	
}
void CSaveCursor::Load_0(FILE* pFile)
{
	for(int i=0;i<MAX_CHARACTER_NUMBER;i++){
		mPersonal[i].Load_0(pFile);
	}		
}
int		CSaveCursor::Get(int id)
{
	if(id==SC_NONE)													return -1;
	if(Flag.mUseSaveCursor<2 && (id & 0xffffff00)!=SC_P_TOP_MENU)	return -1;
	if(Flag.mUseSaveCursor<1)										return -1;

	int* n=GetPointer(id);
	return *n;
}
void	CSaveCursor::Set(int id,int set)
{
	if(id==SC_NONE)	return;
	int* n=GetPointer(id);
	*n=set;
}
int*	CSaveCursor::GetPointer(int id)
{
	if(id & SC_PERSONAL){
		int cno=id&0xff;
		return mPersonal[cno].GetPointer(id);
	}
	return NULL;
}
