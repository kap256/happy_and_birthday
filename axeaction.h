#pragma once

#include "action.h"

class CAct_29: public CBattleAction{
public:
	int		OnTimer();
};
class CAct_30: public CBattleAction{
public:
	int		OnTimer();
};
class CAct_31: public CBattleAction{
public:
	int		OnTimer();
};
class CAct_32: public CBattleAction{
public:
	int		OnTimer();
};
class CAct_33_34: public CBattleAction{
	int mParam;
	int mImg;
public:
	CAct_33_34(int param,int img);
	int		OnTimer();
};
class CAct_35: public CBattleAction{
public:
	int		OnTimer();
};
class CAct_36: public CBattleAction{
public:
	int		OnTimer();
};
class CAct_37: public CBattleAction{
public:
	int		OnTimer();
};