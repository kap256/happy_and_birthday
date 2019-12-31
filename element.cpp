#include "element.h"
#include "main.h"
#include "gamewindow.h"
#include "month.h"
#include "flag.h"
#include "input.h"


CElement::CElement(void)
{
	mName="";
	mID=0;
}
CElement::CElement(const char* name,int id)
{
	mName=name;
	mID=id;
}
CElement::~CElement()
{
}
void	CElement::Draw(CPlaneWindow* window,int x,int y,int param,D3DCOLOR color) const
{
	window->DrawText(0,GetName(),x,y,param);
}
void	CElement::Help(int param)const
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
CItem::CItem(void)
{
}
CItem::CItem(const char* name,int id,int help)
{
	mName=name;
	mID=id;
	mHelpID=help;
}
void CItem::Help(int param)const
{
}
///////////////////////////////////////////////////////////////////////////////////////////////////
void	CKeyConfigItem::Draw(CPlaneWindow* window,int x,int y,int param,D3DCOLOR color) const
{
	window->DrawText(0,mName,x+20,y,param);
	
	int key=GlobalFlag.mKeyConfig[mID];
	char keyname[16];
	switch(key){
		case 0:	strcpy(keyname,"←");	break;
		case 1:	strcpy(keyname,"↑");	break;
		case 2:	strcpy(keyname,"→");	break;
		case 3:	strcpy(keyname,"↓");	break;
		default:
			sprintf(keyname,"ボタン %d",key-3);	break;
	}
	window->DrawText(0,keyname,260,y,param);
}
///////////////////////////////////////////////////////////////////////////////////////////////////
CIconItem::CIconItem(void)
{
	mIcon=-1;
}
CIconItem::CIconItem(const char* name,int id,int icon)
{
	mName=name;
	mID=id;
	mIcon=icon;
}

void CIconItem::Draw(CPlaneWindow* window,int x,int y,int param,D3DCOLOR color) const
{
	y-=4;

	if(mIcon>=0){
		int tx=mIcon%8*32;
		int ty=(mIcon/8)%8*32;
		window->DrawTexture(&SystemTex,0,tx,ty,32,32,x,y);
	}
	if(param & 1){
		int img;
		switch((Flag.mPlayTime/10)%4){
			case 0:	img=1;	break;
			case 2:	img=2;	break;
			default:img=0;	break;
		}
		window->DrawTexture(&SystemTex,0,64,144+img*16,48,16,x-20,y+24);
	}

	window->DrawText(0,mName,x+40,y+14);
}
///////////////////////////////////////////////////////////////////////////////////////////////////
CVillageItem::CVillageItem(void)
{
}
CVillageItem::CVillageItem(const char* name,int id,int icon,int x,int y):CIconItem(name,id,icon)
{
	mX=x;
	mY=y;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
CSaveDataItem::CSaveDataItem(void)
{
}
void CSaveDataItem::Init(int id,int month,int mode,int playtime,int cleardata)
{
	mID=id;
	mMonth=month;
	mMode=mode;
	mPlayTime=playtime;
	mClearData=cleardata;
}
void CSaveDataItem::Draw(CPlaneWindow* window,int x,int y,int param,D3DCOLOR color) const
{
	window->DrawBox(x,y,320,2,COLOR_LINE);
	y+=4;

	if(mMonth==-1){
		window->DrawText(0,"継承しない！",x+20,y+18,0xffb00000);
		return;
	}

	static char text[256];
	D3DCOLOR textcolor;

	sprintf(text,"%d page",mID+1);
	window->DrawText(2,text,x+10,y);

	if(mMonth==0){
		window->DrawText(0,"--- NO DATA ---",x+60,y+20,COLOR_GRAY);
		return;
	}else if(mMonth<MAX_ALL_MONTH){
		int month=NormalHard[mMode][mMonth-1];
		MonthData[month].Draw(window,x+40,y+14,0);
	}

	switch(mMode){
		case 0:	strcpy(text,"Normal");	textcolor=0xff008000;	break;
		case 1:	strcpy(text,"Hard");	textcolor=0xffb00000;	break;
		default:strcpy(text,"Extra");	textcolor=0xffff00ff;	break;
	}
	window->DrawText(2,text,x+70,y,textcolor);

	int sec=mPlayTime%60;
	int min=(mPlayTime/60)%60;
	int hour=(mPlayTime/60/60);
	sprintf(text,"%d : %02d : %02d",hour,min,sec);
	window->DrawText(2,text,50,y,0,DT_RIGHT);

	for(int i=0;i<2;i++){
		D3DCOLOR c;
		if(mClearData & (1<<i)){
			c=0xffffffff;
		}else{
			c=0x40000000;
		}
		window->DrawTexture(&SystemTex,false,112,144+i*16,16,16,x+280+i*20,y+20,c);
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////
CSelectDifficultItem::CSelectDifficultItem(const char* name,int id,int help) : CItem(name,id,help)
{
	char filename[256];
	sprintf(filename,"data\\mapobject\\mapobject%03d",id);
	DTest.LoadTexture(mTex,filename,0xff008080);
}
void CSelectDifficultItem::Draw(CPlaneWindow* window,int x,int y,int param,D3DCOLOR color) const
{
	window->DrawTexture((CDTTexture*)&mTex,0,0,0,64,64,x-48,y-28,128,128);
	window->DrawText(6,mName,x+50,y+24);

	char text[128];
	sprintf(text,"主人公：%s",CharacterBaseData[mID].GetName());
	window->DrawText(0,text,x+110,y+55);
}