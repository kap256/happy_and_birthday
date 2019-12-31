#include "menudata.h"
#include "flag.h"
#include "itemdata.h"


bool	CMenuData::UseItem(int character,int item,int target)
{
	CarryItem useitem;
	if(character>=0){
		useitem=Flag.mPlayerData[Flag.mParty[character]].GetItem(item);
	}else{
		useitem=Flag.mTempItem[item];
	}

	const CUseItemData *ui=&UseItemData[useitem.item-ID_UI];
	if((ui->mType & TYPE_SPEND)  && useitem.param<=0){
		PlayWave(SE_ERROR);
		return false;
	}


	if(ui->mType & TYPE_SELECT_CHARCTER){
		if(target<0){
			SetMenu(UISelectCharacter,character*0x100+item);
			return false;
		}else if(Flag.mParty[target]<0){
			return false;
		}
	}

	switch(ui->mType & 0x0fffffff){
		case TYPE_LP_HEAL:
			UI_LPHeal(useitem.item , target);	break;
		case TYPE_SP_HEAL:
			UI_SPHeal(useitem.item , target);	break;
		case TYPE_BIRTH_STONE:
			UI_BirthStone();			break;
		case TYPE_SAVE:
			UI_Save();					break;
		case TYPE_CLOSET:
			UI_Closet();				break;
		default:
			return false;
	}


	if(character>=0){
		return !Flag.mPlayerData[Flag.mParty[character]].SpendUseItem(item);
	}else{
		if(ui->mType & TYPE_SPEND){
	//TODO ‚ ‚Æ‚Ü‚í‚µ
			return !Flag.mTempItem[item].Spend();
		}
	}
	return true;
}


//////////////////////////////////////////////////////////////////////////////////////////////////
void	CMenuData::UI_LPHeal(int item,int target)
{
	const CUseItemData *ui=&UseItemData[item-ID_UI];

	int pow=ui->mParamA;
	if(Flag.SearchFSkill(271))	pow=(pow * 3 + 1)/2;

	Flag.mPlayerData[Flag.mParty[target]].HealLP(pow);
	PlayWave(SE_HEAL);
}

void	CMenuData::UI_SPHeal(int item,int target)
{
	const CUseItemData *ui=&UseItemData[item-ID_UI];

	int pow=ui->mParamA;
	if(Flag.SearchFSkill(271))	pow=(pow * 3 + 1)/2;

	Flag.mPlayerData[Flag.mParty[target]].HealSP(ui->mParamB,pow);
	PlayWave(SE_HEAL);
}
void	CMenuData::UI_BirthStone(void)
{
	SetMenu(BirthStoneMenu,0);
}
void	CMenuData::UI_Save(void)
{
	SetMenu(DiaryMenu,0);
}
void	CMenuData::UI_Closet(void)
{
	SetMenu(ClosetMenu,0);
}
