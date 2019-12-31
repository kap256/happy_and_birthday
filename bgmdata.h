#pragma once
#include "main.h"
#include "element.h"

//曲カテゴリ。
const int TOWN		= 1000;
const int FIELD		= 2000;
const int BATTLE	= 3000;
const int EVENT		= 4000;
const int ENDING	= 5000;
const int NOT_PLAY	= 65535;	//これ以上の番号の曲は、サウンドテストで再生されない。


class CBGMData:public CElement{
public :
	int mLoopStart;	//マイナスだとループしない
	int mSort;	//サウンドテストの曲順。

	CBGMData();
	CBGMData(char* name,char* subname,int id,int loopstart,int sort);
	virtual int		GetHeight(void)const{return ELEMENT_Height+3;};
};

extern const CBGMData BGMData[BGM_MAX];