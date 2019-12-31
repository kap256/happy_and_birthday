#pragma once

//#include "main.h"
#include "gamewindow.h"
#include "month.h"


class CVillageMode {

	static const int BG_W=1024;
	static const int BG_H=512;

	CDTTexture	mBackGround;
	CDTTexture	mPlayer;
	CListWindow	mList;
	CListWindow	mSubList;
	CListWindow	mBypathList;

	int		mTimer;
	int		mCursor;
	bool	mOriginal;

	double	mPlayerX;
	double	mPlayerY;

public:
	CVillageMode(void);
	~CVillageMode(void);

	void Init(void);
	void OnTimer(void);
	void OnDraw(void);
	void ResetCursor(void){mCursor=0;};
private:
	void InitMenu(void);
	void InitMonthList(void);
	void InitDiaryList(void);
	void InitBypathList(int y);
	void StartMonth(int month);

};
