#pragma once

#include "action.h"

class CAct_128: public CBattleAction{

public:
	int		OnTimer();
};
class CAct_129: public CBattleAction{
public:
	int		OnTimer();
};
class CAct_130: public CBattleAction{
	int mPow;
public:
	CAct_130(int pow){mPow=pow;};
	int		OnTimer();
};
class CAct_131: public CBattleAction{
	int mPow;
public:
	CAct_131(int pow){mPow=pow;};
	int		OnTimer();
};
class CAct_132: public CBattleAction{
	int mPow;
public:
	CAct_132(int pow){mPow=pow;};
	int		OnTimer();
};
class CAct_136: public CBattleAction{
	int mPow;
public:
	CAct_136(int pow){mPow=pow;};
	int		OnTimer();
};
class CAct_137: public CBattleAction{
	int mPow;
public:
	CAct_137(int pow){mPow=pow;};
	int		OnTimer();
};
class CAct_139: public CBattleAction{
public:
	int		OnTimer();
};
class CAct_140: public CBattleAction{
public:
	int		OnTimer();
};