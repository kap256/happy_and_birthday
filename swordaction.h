#pragma once

#include "action.h"

class CAct_8: public CBattleAction{
public:
	int		OnTimer();
};
class CAct_9: public CBattleAction{
public:
	int		OnTimer();
};
class CAct_10: public CBattleAction{
public:
	int		OnTimer();
};
class CAct_12: public CBattleAction{
public:
	int		OnTimer();
};
class CAct_13: public CBattleAction{
public:
	int		OnTimer();
};
class CAct_14: public CBattleAction{
public:
	int		OnTimer();
};
class CAct_15: public CBattleAction{
public:
	int		OnTimer();
};
class CAct_SevenStars: public CBattleAction{
	int mPow;
	int mMin;
	int mHit;
	int mOpt;
	int mImg;
	int mOnce;
	int mSE;
public:
	CAct_SevenStars(int pow,int min,int hit,int opt,int img,int once,int se);
	int		OnTimer();
};