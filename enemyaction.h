#pragma once

#include "action.h"

class CAct_146: public CBattleAction{
public:
	int		OnTimer();
};
class CAct_147: public CBattleAction{
public:
	int		OnTimer();
};
class CAct_150: public CBattleAction{
public:
	int		OnTimer();
};
class CAct_151: public CBattleAction{
public:
	int		OnTimer();
};
class CAct_152: public CBattleAction{
public:
	int		OnTimer();
};
class CAct_153: public CBattleAction{
public:
	int		OnTimer();
};
class CAct_154: public CBattleAction{
public:
	int		OnTimer();
};
class CAct_156: public CBattleAction{
public:
	int		OnTimer();
};
class CAct_157: public CBattleAction{
public:
	int		OnTimer();
};
class CAct_158: public CBattleAction{
public:
	int		OnTimer();
};
class CAct_168: public CBattleAction{
	static const int size=80;
	void DrawCircle(int t,int x,int y,CBattleEffect* effect);
public:
	int		OnTimer();
};
class CAct_170: public CBattleAction{
public:
	int		OnTimer();
};
class CAct_171: public CBattleAction{
public:
	int		OnTimer();
};
class CAct_172: public CBattleAction{
public:
	int		OnTimer();
};
class CAct_173: public CBattleAction{
public:
	int		OnTimer();
};
class CAct_174: public CBattleAction{
public:
	int		OnTimer();
};
class CAct_180: public CBattleAction{
public:
	int		OnTimer();
};
class CAct_181: public CBattleAction{
public:
	int		OnTimer();
};
class CAct_182: public CBattleAction{
public:
	int		OnTimer();
};
class CAct_183: public CBattleAction{
public:
	int		OnTimer();
};
class CAct_184: public CBattleAction{
public:
	int		OnTimer();
};
class CAct_185: public CBattleAction{
public:
	int		OnTimer();
};
class CAct_186: public CBattleAction{
public:
	int		OnTimer();
};
class CAct_187: public CBattleAction{
public:
	int		OnTimer();
};
class CAct_188: public CBattleAction{
public:
	int		OnTimer();
};
class CAct_198: public CBattleAction{
public:
	int		OnTimer();
};
class CAct_199: public CBattleAction{
public:
	int		OnTimer();
};
class CAct_200_203: public CBattleAction{
	int		mParty;
	bool	mEnd;
public:
	CAct_200_203(int pt,bool end){mParty=pt;mEnd=end;};
	int		OnTimer();
};
class CAct_204: public CBattleAction{
public:
	int		OnTimer();
};
class CAct_206: public CBattleAction{
public:
	int		OnTimer();
};
class CAct_222: public CBattleAction{
public:
	int		OnTimer();
};
class CAct_223: public CBattleAction{
public:
	int		OnTimer();
};