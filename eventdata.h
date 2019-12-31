#pragma once

#include "number.h"

const int MAX_EVENT_DATA	=16;
const int MAX_EVENT_PATTERN	=100;
const int MAX_EVENT_HEADER	=16;

const int EH_NEW_LINE		=0;
const int EH_HIT			=1;
const int EH_TALK			=2;
const int EH_ENCOUNT		=3;
const int EH_LOAD_MAP		=4;
const int EH_BOX			=5;
const int EH_WALK			=6;
const int EH_TIME_FLAME		=7;
const int EH_TIME_SEC		=8;
const int EH_FALL			=9;
const int EH_SAVE_POINT		=10;
const int EH_GOAL			=11;

	const int EVE_END_EVENT				=1;
	const int EVE_SET_MESSAGE			=2;
	const int EVE_DELETE_MESSAGE		=3;
	const int EVE_STOP					=4;
	const int EVE_CHANGE_MAP			=5;
	const int EVE_WAIT					=6;
	const int EVE_SET_OBJECT			=7;
	const int EVE_DELETE_OBJECT			=8;
	const int EVE_SET_ENEMY				=9;
	const int EVE_ENCOUNT				=10;
	const int EVE_IF					=11;
	const int EVE_CHECK_BATTLE			=12;
	const int EVE_SET_BOX				=13;
	const int EVE_OPEN_BOX				=14;
	const int EVE_IF_BOX				=15;
	const int EVE_STORE					=16;
	const int EVE_SET_ANI_OBJECT		=17;
	const int EVE_SET_WOOD_OBJECT		=18;
	const int EVE_SET_OBJ_POS			=19;
	const int EVE_SET_OBJ_GOAL			=20;
	const int EVE_START_EVENT_SKIP		=21;
	const int EVE_END_EVENT_SKIP		=22;
	const int EVE_SET_CAMERA			=23;
	const int EVE_SET_OBJ_DIR			=24;
	const int EVE_GO_TO					=25;
	const int EVE_MAP_TITLE				=26;
	const int EVE_SET_OBJ_IMG			=27;
	const int EVE_SET_OBJ_ZSPEED		=28;
	const int EVE_PLAY_WAVE				=29;
	const int EVE_PLAY_BGM				=30;
	const int EVE_NEW_LINE				=31;
	const int EVE_SET_BIG_OBJECT		=32;
	const int EVE_SET_OBJ_VIEW			=33;
	const int EVE_SET_CAMERA_SPEED		=34;
	const int EVE_BATTLE				=35;
	const int EVE_GET_ITEM				=36;
	const int EVE_IF_BOX_3D				=37;
	const int EVE_SET_AUTO_ANI_OBJECT	=38;
	const int EVE_SET_OBJ_GOAL_DIR		=39;
	const int EVE_RETURN_VILLAGE		=40;
	const int EVE_SET_HIGH_ANI_OBJECT	=41;
	const int EVE_NEXT_MONTH			=42;
	const int EVE_KILL					=43;
	const int EVE_MAP_COLOR				=44;
	const int EVE_OPEN_MENU				=45;
	const int EVE_SET_FRIEND			=46;
	const int EVE_SET_PARTY				=47;
	const int EVE_SHOP					=48;
	const int EVE_SELECT_2				=49;
	const int EVE_SET_PUT_OBJECT		=50;
	const int EVE_SPEND_ITEM			=51;
	const int EVE_SELECT_4				=52;
	const int EVE_SELL					=53;
	const int EVE_SELECT_3				=54;
	const int EVE_FLEA_MARKET			=55;
	const int EVE_FARM					=56;
	const int EVE_GET_MAP_CHIP			=57;
	const int EVE_SET_MAP_CHIP			=58;
	const int EVE_BG_COLOR				=59;
	const int EVE_ADD_ENEMY_LEVEL		=60;
	const int EVE_SET_LIFT_OBJECT		=61;
	const int EVE_SET_OBJ_GRAV			=62;
	const int EVE_SET_OBJ_SPEED			=63;
	const int EVE_SET_NORMAL_TRAP		=64;
	const int EVE_SET_SAVE_POINT		=65;
	const int EVE_HEAL_ALL				=66;
	const int EVE_SET_HIGH_OBJECT		=67;
	const int EVE_SEED					=68;
	const int EVE_SET_ENEMY_LOCKED		=69;
	const int EVE_LEAVE_PARTY			=70;
	const int EVE_KNOCK_BACK			=71;
	const int EVE_SHOP_MAN				=72;
	const int EVE_INST					=73;
	const int EVE_CHANGE_ALL_CHIP		=74;
	const int EVE_SET_BIG_ANI_OBJECT	=75;
	const int EVE_SET_OBJ_PARAM			=76;
	const int EVE_ENDING				=77;
	const int EVE_DEL_MES_QUICK			=78;
	const int EVE_SET_EGG_FRIEND		=79;
	const int EVE_ADD_MARKET			=80;
	const int EVE_SET_OBJ_COLOR			=81;
	const int EVE_TUTORIAL				=82;
	const int EVE_CHANGE_MAP_TITLE		=83;
	const int EVE_SHOW_IMAGE			=84;
	const int EVE_TRUE_ENDING			=85;
	const int EVE_MONTH_TITLE			=86;
	const int EVE_SET_FADE_OUT			=87;
	const int EVE_SET_FADE				=88;

//メッセージ表示関連の定数

const int SMES_NO_SOUND = 0x00000010 ;

struct CEventHeaderInfo{
	int		mEvent;
	char	*mCommandName;
};

struct CEventDataInfo{
	int		mEvent;
	char	*mCommandName;
	bool	mText;
	int		mDataNo;
	char	*mHelp;
};


class CEventData{
public:
	int			mLine;
	int			mEvent;
	CNumber		mData[MAX_EVENT_DATA];
	char*		mText;
	CEventData* mNext;
public:
	CEventData();
	~CEventData();
	void AllDelete();
};

class CEventHeader{
public:
	int				mEvent;
	int				mParamA;
	int				mParamB;
	CEventData*		mCurrent;
	CEventHeader*	mNext;

public:
	CEventHeader();
	void AllDelete();
};

class CEvent :public CEventHeader{
public:
	bool			mKill;
	int				mID;
	int				mStartTime;
	int				mWaitTime;

	int				mEvent;
	int				mParamA;
	int				mParamB;
	CEventData*		mCurrent;
	CEvent*			mNext;

public:
	CEvent();
	void AllDelete();
};


extern const CEventDataInfo		EventDataInfo[MAX_EVENT_PATTERN];
extern const CEventHeaderInfo		EventHeaderInfo[MAX_EVENT_HEADER];
