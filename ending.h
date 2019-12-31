#pragma once
#include "DTestLib/dtest.h"

class CEndingMode {
	int			mTimer;
	int			mMode;
	CDTTexture	mTex;

public:
	CEndingMode(void);
	~CEndingMode(void);

	void Init(int mode=0);
	void OnTimer(void);
	void OnDraw(void);
	void EndEnding(bool force=false);
	void ChangeMode(int mode=0);

	bool DrawTitle(void);
	bool DrawStaff(void);
	bool DrawEnd(void);
private:
};
struct CStaffInfo{
	const char* job;
	const char* name;
};
struct CStaffViewPos{
		int tx,ty;
		int jx,jy;
		int nx,ny;
};