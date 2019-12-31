#pragma once

#include "element.h"
#include "dataconst.h"

const int MAX_ALL_MONTH	= 19;

class CMonthData:public CElement{
public:
	const char* mTitle;
	int	mMonth;
	int	mBaseLv[2];
	int	mStartMap;
	int	mX;
	int	mY;
	int	mZ;

	int	mParty[MAX_PARTY_NUMBER];
	int mImpossible[4];

	int* mTresure;
	int  mTresureNo;

	int* mEnemy;
	int  mEnemyNo;

public:
	CMonthData();
	CMonthData(const char* name,int id,int month,const char* title,int normallv,int hardlv,int startmap,int x,int y,int z,
				int pa,int pb,int pc,int pd,int ima,int imb,int imc,int imd,int tresure, ...);
	~CMonthData();

	void	Draw(CPlaneWindow* window,int x,int y,int param,D3DCOLOR color=0xffffffff) const;
	void	Help(int param)const;
	int		GetHeight(void)const{return 32;};
	int		GetBaseLv(void);

};

extern	CMonthData	MonthData[MAX_ALL_MONTH];
extern	int			NormalHard[3][MAX_ALL_MONTH];
extern	const char*			MonthEnglish[MAX_MONTH];
