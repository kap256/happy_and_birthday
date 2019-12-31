#pragma once

#include "stdlib.h"
#include "dSFMT-src-2.1/dSFMT.h"

const int KEY_REPEAT_START	=20;
const int KEY_REPEAT_SPEED	=3;

class CInput{
	int mKey[20];
	int mKeyCustum[20];
	dsfmt_t mMT;

public:
	CInput(void);
	void Reset(void);
	int  GetKey(int no)		{return mKey[no];};
	int  GetRepeatKey(int no);
	int	 Rand(int size);	

	int  GetKeyCustom(int no){return mKeyCustum[no];};
	void SetKeyCustom(int no,int key);
};
