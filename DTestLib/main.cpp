#include "dtest.h"

const int TEX_NO = 1;
const int LOOP	 = 100;	

CDTTexture Tex[TEX_NO];
int T;

void LoadTex(int tno,int ino)
{
	char fname[256];
	sprintf(fname,"img\\mapobject%03d",ino);
	DTest.LoadTexture(Tex[tno],fname);
}

void StartUp()
{
}
void AfterStartUp()
{
	for(int i=0;i<TEX_NO;i++){
		LoadTex(i,100+LOOP-i);
	}
	DTest.ChangeTextColor(0,0xffffffff);
}
void OnTimer()
{
}
void OnDraw()
{
	const int CHANGE	= 10;
	const int WIDTH		= 222;
	int tex=(T/CHANGE)%TEX_NO;
	if(T%CHANGE==0){
		int img=(T/CHANGE)%LOOP + 100;
		LoadTex((tex+TEX_NO-1)%TEX_NO,img);
	}
	T++;
	DTest.Printf(0,0,0,"%d",T);

	int x;
	if((T/WIDTH)%2==0){
		x=T%WIDTH+100;
	}else{
		x=WIDTH+100-T%WIDTH;
	}
	DTest.DrawTexture(Tex[tex],0,0,0,64,64,x,160);
}
void End()
{
}