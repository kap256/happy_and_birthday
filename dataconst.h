#pragma once

const int MAX_KEY_TYPE	=8;

const int MAX_EQUIPMENT_NUMBER	=256;
const int CATEGORY_MAX			=14; 
const int ID_EQ					=0;
	const int ID_WEP					=ID_EQ	+ 0;
		const int WLV_0						=0;
		const int WLV_1						=WLV_0+2;
		const int WLV_2						=WLV_1+4;
		const int WLV_3						=WLV_2+4;
		const int WLV_4						=WLV_3+4;
		const int WLV_5						=WLV_4+4;
		const int ALL_WEP					=WLV_5+0;

		const int ID_SWD					=ID_WEP	+ 0;
		const int ID_SPR					=ID_SWD + ALL_WEP;
		const int ID_AXE					=ID_SPR + ALL_WEP;
		const int ID_BOW					=ID_AXE + ALL_WEP;
		const int ID_WND					=ID_BOW + ALL_WEP;
		const int ID_SWP					=ID_WND + ALL_WEP;

	const int ID_AMR					=ID_SWP	+ 4;
		const int ALV_0						=0;
		const int ALV_1						=ALV_0+4;
		const int ALV_2						=ALV_1+4;
		const int ALV_3						=ALV_2+6;
		const int ALV_4						=ALV_3+8;
		const int ALV_5						=ALV_4+6;
		const int ALL_AMR					=ALV_5+0;

	const int ID_ACC					=ID_AMR	+ ALL_AMR;
		const int ID_SLD					=ID_ACC	+ 0;
		const int ID_HED					=ID_SLD	+ 16;
		const int ID_GLV					=ID_HED	+ 16;
		const int ID_SHE					=ID_GLV	+ 16;
		const int ID_AME					=ID_SHE	+ 16;
		const int ID_AOT					=ID_AME	+ 22;

const int MAX_USE_ITEM_NUMBER	=128;
const int ID_UI					=ID_EQ+MAX_EQUIPMENT_NUMBER;

const int MAX_SEED_NUMBER		=32;
const int ID_NSUI				=ID_UI+MAX_SEED_NUMBER;
const int ID_BS					=ID_NSUI+0;
const int ID_KEY				=ID_BS+25;
const int ID_MED				=ID_KEY+MAX_KEY_TYPE;
const int ID_OTH				=ID_MED+4;
const int ID_SD					=ID_UI+MAX_USE_ITEM_NUMBER;


const int MAX_ITEM_NUMBER	=	MAX_EQUIPMENT_NUMBER+MAX_USE_ITEM_NUMBER+MAX_SEED_NUMBER;
const int ITEM_TEX_NUMBER	=	5;

const int NO_ITEM				=	MAX_ITEM_NUMBER;
const int ITEM_BIRTH_STONE		=	ID_BS;
const int ITEM_HI_BIRTH_STONE	=	ID_NSUI+0;
const int ITEM_KEY				=	ID_KEY;

const int MAX_SHOP_NO			=	18;
const int MAX_SHOP_ITEM			=	16;

const int MAX_CHARACTER_NUMBER	=15;
const int MAX_PET_TYPE			=10;
const int MAX_PARTY_NUMBER		=4;

const int WEAPON_SPACE			=3;
const int ARMOR_SPACE			=1;
const int MAX_ACCESSORY_REAL	=4;
const int MAX_ACCESSORY			=8;
const int MIN_ACCESSORY			=2;
const int MIN_USE_ITEM			=2;
const int MAX_USE_ITEM			=8;
const int MAX_ITEM				=16;
const int MIN_ITEM				=8;

const int MAX_SKILL_MONTH		=8;
const int MAX_LEARN_MONTH		=6;
const int MAX_MONTH				=13;
const int MAX_CSKILL			=64;
const int MAX_ASKILL			=64;
const int MAX_FREE_SPACE		=4;
