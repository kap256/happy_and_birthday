#pragma once

#include "stdio.h"

const int MAX_SNUM_NUMBER	= 64;

const int IF_EQUAL			= 0;
const int IF_BIGGER			= 1;
const int IF_SMALLER		= 2;
const int IF_NOT_EQUAL		= 3;

const int SNUM_RANDOM		= -2;
const int SNUM_MONEY		= -3;
const int SNUM_PLAYER_IMAGE	= -4;
const int SNUM_PARTY_NUMBER	= -5;
const int SNUM_CURRENT		= -6;
const int SNUM_RETURN		= -7;
const int SNUM_CURRENT_MONTH= -8;
const int SNUM_MONTH		= -9;
const int SNUM_PARAMA		= -10;
const int SNUM_PARAMB		= -11;
const int SNUM_MAP_X		= -12;
const int SNUM_MAP_Y		= -13;
const int SNUM_EVENT_ID		= -14;
const int SNUM_BUSINESS		= -15;
const int SNUM_MAP_NO		= -16;
const int SNUM_START_X		= -17;
const int SNUM_START_Y		= -18;
const int SNUM_START_Z		= -19;
const int SNUM_START_DIR	= -20;
const int SNUM_HARD_MODE	= -21;
const int SNUM_EVENT_SKIP	= -22;
const int SNUM_ESCAPE		= -64;

const int SNUM_KAKKO		= -100;
const int SNUM_FLAG			= -101;
const int SNUM_OBJECT_X		= -102;
const int SNUM_OBJECT_Y		= -103;
const int SNUM_OBJECT_Z		= -104;
const int SNUM_OBJECT_DIR	= -105;
const int SNUM_MAP_FLAG		= -106;
const int SNUM_OBJECT_X_32	= -107;
const int SNUM_OBJECT_Y_32	= -108;
const int SNUM_OBJECT_Z_32	= -109;
const int SNUM_FIX			= -110;
const int SNUM_ITEM_NO		= -111;
const int SNUM_MEMBER		= -112;
const int SNUM_OBJECT_GOAL	= -113;
const int SNUM_SHOP_OK		= -114;
const int SNUM_BOX			= -115;
const int SNUM_IS_MEMBER	= -116;
const int SNUM_OBJECT_CAM_X	= -117;
const int SNUM_OBJECT_CAM_Y	= -118;
const int SNUM_ROOT			= -119;

struct CSpecialNumberInfo{
	int		mSNumber;
	char*	mName;
	bool	mArray;
	int		mMax;
};

enum NumberSign{END,ADD,SUB,MUL,DIV,RES};

class CNumber{
public:
	static int	mCurrent;
private:
	bool		mSNum;
	int			mNumber;
	NumberSign	mSign;
	CNumber*	mIndex;
	CNumber*	mNext;

public:
	CNumber();
	CNumber(int no){mNumber=no;};
	~CNumber();
	void	LoadNumber(FILE* pFile);
	void	InitNumber(int no){mNumber=no;};
	int		GetNumber(void);
	int*	GetSpecialNumber(int *num);
	CNumber	Copy(void);

	void	AllDelete(void);
	void	AllDelete(CNumber* num);
};

extern const CSpecialNumberInfo		SpecialNumberInfo[MAX_SNUM_NUMBER];
