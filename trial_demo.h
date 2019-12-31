#pragma once
#include "option.h"
#include "DTestLib/dtest.h"

#ifdef _TRIAL_VERSION_

class CTrialDemo {
	static const int BLACK_TIME		= 8;
	static const int WHITE_UP_TIME	= 16 + BLACK_TIME;
	static const int WHITE_TIME		= 16 + WHITE_UP_TIME;
	static const int WHITE_DOWN_TIME= 64 + WHITE_TIME;

	int			mTimer;
	CDTTexture	mTex;

public:
	CTrialDemo(void);
	~CTrialDemo(void);

	void Init(void);
	void OnTimer(void);
	void OnDraw(void);
};
#endif