#pragma once

//#include "main.h"
#include "gamewindow.h"

class CTitleMode {
	static const int BLACK_TIME		= 8;
	static const int WHITE_UP_TIME	= 16 + BLACK_TIME;
	static const int WHITE_TIME		= 16 + WHITE_UP_TIME;
	static const int WHITE_DOWN_TIME= 64 + WHITE_TIME;

	static const int CURSOR_START	= WHITE_TIME;

	static const int MAX_ITEM		=5;
	static const int ITEM_X			=480;
	static const int ITEM_Y			=360;
	static const int ITEM_H			=40;
	static const int ITEM_X_MOVE	=-15;

	CDTTexture	mTitleTex;
	int mTimer;
	int mCursor;
	bool mShowCursor;

	int mItemList[MAX_ITEM];
	int mItemNo;
public:
	CTitleMode(void);
	~CTitleMode(void);

	void Init(void);
	void OnTimer(void);
	void OnDraw(void);
	void Destroy(void);

	bool ShowCursor(void){return mShowCursor;}; 
private:
	void GetItemPos(int no,int *x,int *y);
};

