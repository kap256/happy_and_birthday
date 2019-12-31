#pragma once
#include "stdio.h"
#include "characterdata.h"

	const int ODATA_LP		=	0;
	const int ODATA_SP		=	1;
	const int ODATA_ATK		=	2;
	const int ODATA_DEF		=	3;
	const int ODATA_MATK	=	4;
	const int ODATA_MDEF	=	5;
	const int ODATA_SPD		=	6;

	const int ODATA_CS		=	7;
	const int ODATA_AS		=	7+MAX_CSKILL;
const int MAX_ODATA		=ODATA_AS+MAX_ASKILL;


class CEnemyOpenData{
private:
	int		mKillNo; 

	bool	mOpenData[MAX_ODATA];
public:
	CEnemyOpenData();
	void	Save(FILE* pFile);
	void	Load_0(FILE* pFile);

	bool	Get(int no);
	void	Set(int no);
	int		GetKill(void){return mKillNo;};
	void	SetKill(void);
};
