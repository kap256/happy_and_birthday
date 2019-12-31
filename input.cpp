#ifndef _EVENT_EDITER_
#include "DTestLib/dtest.h"
#endif
#include "input.h"
#include "option.h"
#include <time.h>


CInput::CInput(void)
{
//乱数生成
	dsfmt_init_gen_rand(&mMT,(unsigned)time( NULL ));
	for(int i=0;i<500000;i++){
		dsfmt_genrand_close_open(&mMT);
	}

//キーコンフィグ
	for(int n=0;n<20;n++){
		mKeyCustum[n]=n;
	}
}

void CInput::Reset(void)
{
	const int keybord[20]={
		DIK_LEFT	,DIK_UP		,DIK_RIGHT	,DIK_DOWN	,
		DIK_C		,DIK_X		,DIK_Z		,DIK_ESCAPE	,
		DIK_ESCAPE	,DIK_ESCAPE	,DIK_ESCAPE	,DIK_ESCAPE	,
		DIK_ESCAPE	,DIK_ESCAPE	,DIK_ESCAPE	,DIK_ESCAPE	
	};

	for(int n=0;n<20;n++){
		int k=DTest.GetKey(keybord[n]);
#ifndef _GAME_PAD_OFF_
		int j=DTest.GetJoyButton(mKeyCustum[n]);
		if(j>k)	k=j;
#endif
		mKey[n]=k;
	}
}
int  CInput::GetRepeatKey(int no)
{
	int key=GetKey(no);
	if(key>KEY_REPEAT_START){
		key=key%KEY_REPEAT_SPEED+1;
	}
	return key;
}
int	 CInput::Rand(int size)
{
	return int(dsfmt_genrand_close_open(&mMT)*size);
}
void CInput::SetKeyCustom(int no,int key)
{
	if(no<4)	return;
	mKeyCustum[no]=key;
}