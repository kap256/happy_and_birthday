#pragma once

#include "element.h"
//#include "gamewindow.h"
#include "characterdata.h"


const int AT_ELE_NO				=5;
	const int AT_ELE_NONE			=1;
	const int AT_ELE_FIRE			=2;
	const int AT_ELE_ICE			=4;
	const int AT_ELE_WIND			=8;
	const int AT_ELE_EARTH			=16;

const int AT_ATK				=0x03;
	const int AT_ATK_WEAPON			=0;
	const int AT_ATK_MAGIC			=1;
	const int AT_ATK_CONST			=2;
const int AT_DEF				=0x0c;
	const int AT_DEF_WEAPON			=0;
	const int AT_DEF_MAGIC			=4;
	const int AT_DEF_ZERO			=8;

const int AT_AVOID				=0x10;
const int AT_CRITICUL			=0x20;
const int AT_LONG_LANGE			=0x40;
const int AT_SKILL				=0x80;
const int AT_NO_RAND			=0x100;
const int AT_MAGIC				=0x200;

const int AT_TYPE_WEAPON		= AT_SKILL | AT_ATK_WEAPON | AT_DEF_WEAPON | AT_AVOID | AT_CRITICUL ;
const int AT_TYPE_BOW			= AT_TYPE_WEAPON | AT_LONG_LANGE;
const int AT_TYPE_MAGIC			= AT_MAGIC | AT_ATK_MAGIC | AT_DEF_MAGIC | AT_LONG_LANGE;
const int AT_TYPE_CONST			= AT_ATK_CONST | AT_DEF_ZERO;
const int AT_TYPE_NO_RAND		= AT_ATK_CONST | AT_DEF_ZERO | AT_NO_RAND;
const int AT_TYPE_LASER			= AT_SKILL | AT_ATK_WEAPON | AT_DEF_ZERO | AT_NO_RAND | AT_LONG_LANGE;

const int AT_OPT_DEF_BREAK			=0x3;
	const int AT_OPT_DEF_BREAK_0		=0x1;
	const int AT_OPT_DEF_BREAK_1		=0x2;
const int AT_OPT_HALF_HIT			=0x4;
const int AT_OPT_ANTI_AIR			=0x8;
const int AT_OPT_NO_AIR				=0x10;
const int AT_OPT_CRT_UP				=0x20;
const int AT_OPT_NO_EFFECT			=0x40;		//‚Ì‚¯‚¼‚ç‚È‚¢
const int AT_OPT_NO_DEATH			=0x80;
const int AT_OPT_NO_DOUBLE			=0x100;
const int AT_OPT_NO_REFLECT			=0x200;
const int AT_OPT_LIFE_BREAK			=0x400;
const int AT_OPT_ANTI_UNDEAD		=0x800;
const int AT_OPT_NO_TWIN_HIT		=0x1000;
const int AT_OPT_ANTI_AIR_ELEMENT_	=0x2000;

const int FE_POWER_MASK =0xffff0000;

const int FE_TYPE		=0x000f0000;
	const int FE_TYPE_MAGIC		=0x00010000;
const int FE_ELEMENT	=0x00f00000;
	const int FE_ELE_FIRE		=0x00100000;
const int FE_ANI_SHIFT	=24;
const int FE_ANIMATION	=0xf << FE_ANI_SHIFT;
	const int FE_ANI_0			=0x0 << FE_ANI_SHIFT;
const int FE_TIMER_SHIFT=28;
const int FE_TIMER		=0xf << FE_TIMER_SHIFT;
const int FE_TIMER_MASK =0xffffffff ^ FE_TIMER;

const int MIN_ATK_LV_0	=30;
const int MIN_ATK_LV_1	=50;
const int MIN_ATK_LV_2	=100;
const int MIN_ATK_LV_3	=200;
const int MIN_ATK_LV_4	=400;


const int		BATTLE_SQUARE_SIZE	=64;
const int		BATTLE_SQUARE_X		=0;
const int		BATTLE_SQUARE_Y		=32;
const int		BATTLE_SQUARE_WIDTH	=9;
const int		BATTLE_SQUARE_Height=4;
const double	BATTLE_GRAVITY		=0.9;

const int		BATTLE_ACTION_WAIT	=4;
const int		BATTLE_NO_MOVE_WAIT	=BATTLE_ACTION_WAIT+4;

const int		MAX_ENEMY_NUMBER	=16;
const int		MAX_PET_NUMBER		=3;
const int		MAX_WALL_NUMBER		=4;
const int		ENEMY				=MAX_PARTY_NUMBER;
const int		PET					=ENEMY+MAX_ENEMY_NUMBER;
const int		WALL				=PET+MAX_PET_NUMBER;
const int		CHARACTER_NO		=MAX_ENEMY_NUMBER+MAX_PARTY_NUMBER+MAX_PET_NUMBER+MAX_WALL_NUMBER;

const	int		ANI_TYPE	=4;


const int		BM_START				=0;
	const int		BM_START_TIME			=10;
	const int		BM_START_TIME_STONE		=20;

const int		BM_START_MENU			=1;
const int		BM_CHECK_MENU			=2;

const int		BM_PERSONAL_MENU		=64;
	const int		BM_PM_NUMBER			=8;
	const int		BM_PM_START				=0;
	const int		BM_PM_SKILL				=1;
	const int		BM_PM_MAGIC				=2;
	const int		BM_PM_SPECIAL			=3;
	const int		BM_PM_ITEM				=4;
	const int		BM_PM_TARGET			=6;
	const int		BM_PM_MOVE				=7;
	const int		BM_PM_BACK				=128;

const int		BM_BATTLE				=256;
	const int		BM_TURN_START_TIME		=20;
	const int		BM_INTARVAL_TIME		=20;
	const int		BM_INTARVAL_RUN_START	=4;
	const int		BM_TURN_END				=BM_BATTLE+3;

const int		BM_ESCAPE				=512;
	const int		BM_ESCAPE_TIME			=30;
	const int		BM_ESCAPE_FAILED		=BM_ESCAPE+1;
	const int		BM_ESCAPE_FAILED_TIME	=BM_ESCAPE_TIME+10;

const int		BM_WIN					=1024;
	const int		BM_WIN_TIME				=BM_INTARVAL_TIME+20;
	const int		BM_RESULT				=BM_WIN+0x10;
		const int		BM_RESULT_TIME			=16;
		const int		BM_RESULT_EXP			=BM_RESULT+1;

const int		BM_LOSE_START			=2048;
	const int		BM_LOSE_START_TIME		=BM_INTARVAL_TIME+20;
const int		BM_LOSE					=2049;
	const int		BM_LOSE_TIME			=320;
	const int		BM_LOSE_BLACK_TIME		=64;
	const int		BM_LOSE_SE_TIME			=32;


const int BATTLE_WINDOW_X		=205;
const int BATTLE_WINDOW_Y		=480-32-ELEMENT_Height*5-10;
const int BATTLE_WINDOW_Y_UNDER	=415;

const int BATTLE_MENU_X			=5;
const int BATTLE_MENU_X_HIDDEN	=BATTLE_MENU_X-80;
const int BATTLE_MENU_Y			=BATTLE_WINDOW_Y+8;
const int BATTLE_MENU_WIDTH		=250;

const int BOPT_LOSE_OK				=0x00000001;
const int BOPT_NO_ESCAPE			=0x00000002;
const int BOPT_ESCAPE_OK			=0x00000004;
const int BOPT_FIRST_ATTACK			=0x00000008;
const int BOPT_LPUP_x2				=0x00000010;
const int BOPT_2_TURN				=0x00000020;
const int BOPT_KILL_LEADER			=0x00000040;
const int BOPT_SUPER_ENEMY			=0x00000080;
const int BOPT_NO_RESULT			=0x00000100;
const int BOPT_LPUP_x3				=0x00000200;
const int BOPT_LP_HALF				=0x00000400;
const int BOPT_POWUP_x15			=0x00000800;
const int BOPT_INT_x3				=0x00001000;
const int BOPT_ENEMY_FIRST_ATTACK	=0x00002000;
const int BOPT_SHORT_ESCAPE			=0x00004000;
const int BOPT_BGM_BOSS				=0x00008000;
const int BOPT_P_UP_LEADER_ONLY		=0x00010000;
const int BOPT_P_UP_WOUT_BIG		=0x00020000;
const int BOPT_INT_x2				=0x00040000;
const int BOPT_P_UP_WOUT_LEADER		=0x00080000;
const int BOPT_LAST_BATTLE_NORMAL	=0x00100000;
const int BOPT_LAST_BATTLE_HARD		=0x00200000;
const int BOPT_POW_HALF				=0x00400000;
const int BOPT_INT_HALF				=0x00800000;
const int BOPT_POWUP_x40			=0x01000000;
const int BOPT_LP_LOCK_				=0x02000000;
const int BOPT_END_BGM_OFF			=0x04000000;
const int BOPT_BGM_NO_CHANGE		=0x08000000;


const int PARAM_UP_NO			=32;
	const int UP_ATK				=2;
	const int UP_DEF				=4;
	const int UP_MATK				=8;
	const int UP_MDEF				=16;
	const int UP_SPD				=32;

const int STATUS_ANI_TIME		=16;
