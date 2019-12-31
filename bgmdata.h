#pragma once
#include "main.h"
#include "element.h"

//�ȃJ�e�S���B
const int TOWN		= 1000;
const int FIELD		= 2000;
const int BATTLE	= 3000;
const int EVENT		= 4000;
const int ENDING	= 5000;
const int NOT_PLAY	= 65535;	//����ȏ�̔ԍ��̋Ȃ́A�T�E���h�e�X�g�ōĐ�����Ȃ��B


class CBGMData:public CElement{
public :
	int mLoopStart;	//�}�C�i�X���ƃ��[�v���Ȃ�
	int mSort;	//�T�E���h�e�X�g�̋ȏ��B

	CBGMData();
	CBGMData(char* name,char* subname,int id,int loopstart,int sort);
	virtual int		GetHeight(void)const{return ELEMENT_Height+3;};
};

extern const CBGMData BGMData[BGM_MAX];