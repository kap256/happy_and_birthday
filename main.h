#pragma once

//#undef _DEBUG
		//Ç´ÇÒÇæÇÒÇÃÇÌÇ¥

#ifndef _EVENT_EDITER_
#include "DTestLib/dtest.h"
#endif

#include "stdio.h"
#include "skilldata.h"

class CInput;
class CCursor;
class CMenuData;
class CMapMode;
class CBattleMode;
class CVillageMode;
class CTitleMode;
class CEndingMode;

class CEncountInfo;
class CReturnMapInfo;
class CChangeMapInfo;


extern	CDTTexture		SystemTex;
extern	CDTTexture		StatusTex[STATUS_TEX_NUMBER];

extern	CInput*			pInput;
extern	CCursor			Cursor;
extern	CMenuData		MenuData;

extern	CMapMode		MapMode;
extern	CBattleMode		BattleMode;
extern	CVillageMode	VillageMode;
extern	CTitleMode		TitleMode;
extern	CEndingMode		EndingMode;

extern	CEncountInfo	EncountInfo;
extern	CReturnMapInfo	ReturnMapInfo;
extern	CChangeMapInfo	ChangeMapInfo;


extern	int	GameMode;
const	int GMODE_MAP		=0;
const	int GMODE_BATTLE	=1;
const	int GMODE_VILLAGE	=2;
const	int GMODE_TITLE		=3;
const	int GMODE_ENDING	=4;
const	int GMODE_TRIAL		=5;

const double LINE_WIDTH = 2.4;

const D3DCOLOR COLOR_WIN		=0xffffffb0;
const D3DCOLOR COLOR_SELECT		=0x8080ffb0;
const D3DCOLOR COLOR_LINE		=0xff808040;
const D3DCOLOR COLOR_GRAY		=0xffb0b0b0;
const D3DCOLOR COLOR_SHADOW		=0x40000000;
const D3DCOLOR COLOR_DARK_WINDOW=0xffb0b080;



void PlayWave(int sound);
void PlayBGM(int sound,bool force=false);
void SetBGMFadeOut(int time);
void SetBGMFade(int time,int vol);

const int SAVE_DATA_NO	=30;
const int NumberOfSound	=64;
const int BGM_MAX		=32;

const int SE_MOVE			=0;
const int SE_YES			=1;
const int SE_EXCHANGE		=1;
const int SE_NO				=2;
const int SE_ERROR			=4;
const int SE_EQUIP			=3;
const int SE_LV_UP			=21;
const int SE_LEARN_SKILL	=3;
const int SE_REMOVE_SKILL	=3;		//ïœÇ¶ÇÊÇ§ÅB
const int SE_BUY_ITEM		=15;
const int SE_GET_ITEM		=3;
const int SE_SELL_ITEM		=15;
const int SE_SAVE			=3;
const int SE_ENCOUNT		=23;
const int SE_OPEN_BOX		=9;
const int SE_TRAP_OK		=6;
const int SE_TRAP_MISS		=10;
const int SE_GET_ALL_ITEM	=33;
const int SE_STEP			=11;
const int SE_MOVE_BLOCK		=12;
const int SE_SEED_THROW		=18;
const int SE_SEED_IN_EARTH	=11;
const int SE_PRODUCE_UP		=6;
const int SE_PROVOKE_1		=32;
const int SE_PROVOKE_2		=33;
const int SE_MESSAGE_START	=44;
const int SE_MESSAGE_END	=45;
const int SE_GAME_OVER		=47;


const int SE_SLASH			=16;
const int SE_HARD_SLASH		=31;
const int SE_HIT			=17;
const int SE_HARD_HIT		=22;//ñ¢íË
const int SE_THRUST			=19;
const int SE_HARD_THRUST	=31;//ñ¢íË
const int SE_CHOP			=19;
const int SE_HARD_CHOP		=31;//ñ¢íË
const int SE_SHOOT			=16;
const int SE_BOW			=20;
const int SE_LASER			=30;
const int SE_HEAL			=21;
const int SE_CURE			=39;
const int SE_FIRE			=22;
const int SE_ICE			=3;
const int SE_HARD_ICE		=37;
const int SE_WIND			=16;
const int SE_STONE			=17;

const int SE_MAGIC			=5;
const int SE_MISS			=18;
const int SE_PARAM_UP		=23;
const int SE_PARAM_DOWN		=25;

const int SE_POISON			=25;//ñ¢íË
const int SE_SLEEP			=25;
const int SE_CHARM			=25;//ñ¢íË
const int SE_PARALYSE		=30;
const int SE_SEAL			=31;
const int SE_BERSERK		=22;
const int SE_ROT			=25;//ñ¢íË
const int SE_DIVE			=25;

const int SE_POO			=25;
const int SE_SWING			=18;
const int SE_GUS			=26;
const int SE_WAVE			=17;//ñ¢íË
const int SE_WARP			=23;//ñ¢íË
const int SE_EXPLOSION		=36;
const int SE_CLOCK			=24;
const int SE_STAR			=0;
const int SE_GRAVITY		=12;
const int SE_ELECTRIC		=30;
const int SE_GUST			=28;
const int SE_BLOOD			=25;//ñ¢íË
const int SE_FLASH			=46;
const int SE_SPINE			=30;//ñ¢íË
const int SE_INK			=25;//ñ¢íË
const int SE_TORNADE		=40;
const int SE_BLACK_HOLE		=40;//í≤ñ°í‡
const int SE_CALL			=46;	//ñ¢íË
const int SE_RIVIVAL		=5; //ñ¢íË
const int SE_EGG_BREAK		=43; 
const int SE_SONG			=48; 

//ï÷óòånä÷êî
int			PriorityRand(int* pri,int no,int and=0xffffffff,int min=0);	
D3DCOLOR	BlendColor(D3DCOLOR color1,D3DCOLOR color2);	
