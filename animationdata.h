#pragma once

#include "characterdata.h"
#include "battleconst.h"


struct CAnimationData{
	int		mX;
	int		mY;
	double	mRad;
	bool	mTop;
	bool	mView;
	bool	mTurn;

	CAnimationData();
	CAnimationData(int x,int y,int deg,bool top,bool turn);
};

extern const CAnimationData AnimationData[MAX_CHARACTER_NUMBER*2][16*ANI_TYPE];