#pragma once

#include "main.h"

const int CURSOR_START_X	=640;
const int CURSOR_START_Y	=480;
const int CURSOR_SPEED		=4;
const int CURSOR_CLICK_TIME	=4;
const int CURSOR_ANI_TIME	=20;
const int CURSOR_MAX_STAR	=16;

class CCursor{
	double	mX;
	double	mY;
	int		mGoalX;
	int		mGoalY;
	int		mTimer;
	int		mStarX[CURSOR_MAX_STAR];
	int		mStarY[CURSOR_MAX_STAR];
	int		mStar;

public:
	CCursor();
	void	OnTimer();
	void	OnDraw();

	void	Move(int x,int y);
	void	Click(void);
	void	Escape(void);

	int		GetX(void)		{return int(mX);};
	int		GetY(void)		{return int(mY);};
	int		GetGoalX(void)	{return int(mGoalX);};
	int		GetGoalY(void)	{return int(mGoalY);};
};