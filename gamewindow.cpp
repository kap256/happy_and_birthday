#include "gamewindow.h"
#include "input.h"
#include "cursor.h"
#include "battlecharacter.h"
#include "menudata.h"
#include "skilldata.h"
#include "itemdata.h"
#include "action.h"

CPlaneWindow::CPlaneWindow()
{
	mLive=false;
}
CPlaneWindow::~CPlaneWindow()
{
}
void	CPlaneWindow::InitPlaneWindow(int x,int y,int w,int h,bool opened)
{
	mX=x;
	mY=y;
	mWidth=w;
	mHeight=h;

	mGoalX=x;
	mBaseX=x;
	mGoalY=y;
	mBaseY=y;

	mLive=true;
	if(opened){
		mTimer=WINDOW_OPEN_TIME;
	}else{
		mTimer=0;
	}
}
void	CPlaneWindow::QuickDestroy(void)
{
	mLive=false;
	mTimer=0;
}
void	CPlaneWindow::Destroy(void)
{
	if(mTimer<=WINDOW_OPEN_TIME)
		mTimer=WINDOW_OPEN_TIME*2-mTimer+1;
}
void	CPlaneWindow::OnTimer(void)
{
	if(!mLive)	return;
	if(mTimer!=WINDOW_OPEN_TIME)	mTimer++;
	if(mTimer>WINDOW_OPEN_TIME*2+1)	mLive=false;

	mX+=(mGoalX-mX)/8.0;
	mY+=(mGoalY-mY)/8.0;
}
void	CPlaneWindow::OnDraw(D3DCOLOR color)
{
	if(!mLive)	return;
	int w,h;
	GetWindowSize(&w,&h);
	DrawWindow((int)mX,(int)mY,w,h,color);
}


void	CPlaneWindow::DrawWindow(int x,int y,int w,int h,D3DCOLOR color)
{
	DTest.DrawTexture(	SystemTex,0,
						0,0,16,16,
						x,y,16,16,color);
	DTest.DrawTexture(	SystemTex,0,
						0,16	,16,16,
						x,y+16	,16,h-32,color);
	DTest.DrawTexture(	SystemTex,TURN_VERTICAL,
						0,0		,16,16,
						x,y+h-16,16,16,color);
	DTest.DrawTexture(	SystemTex,TURN_VERTICAL,
						16,0		,16,16,
						x+16,y+h-16	,w-32,16,color);
	DTest.DrawTexture(	SystemTex,TURN_VERTICAL|TURN_HORIZONTAL,
						0,0				,16,16,
						x+w-16,y+h-16	,16,16,color);
	DTest.DrawTexture(	SystemTex,TURN_HORIZONTAL,
						0,16		,16,16,
						x+w-16,y+16	,16,h-32,color);
	DTest.DrawTexture(	SystemTex,TURN_HORIZONTAL,
						0,0		,16,16,
						x+w-16,y,16,16,color);
	DTest.DrawTexture(	SystemTex,0,
						16,0	,16,16,
						x+16,y	,w-32,16,color);
	DTest.DrawTexture(	SystemTex,0,
						16,16,16,16,
						x+16,y+16,w-32,h-32,color);
}
void	CPlaneWindow::GetWindowSize(int* w,int *h)
{
	int time=mTimer;
	if(mTimer>WINDOW_OPEN_TIME){
		time=WINDOW_OPEN_TIME*2-mTimer+1;
	}
	if(time<0)	time=0;
	
	*w=(mWidth-32)*time/WINDOW_OPEN_TIME+32;
	*h=(mHeight-32)*time/WINDOW_OPEN_TIME+32;
}

void	CPlaneWindow::DrawText(int fontnum,const char *text,int x,int y,D3DCOLOR color,int position)
{
	if(!mLive)	return;
	if(mTimer>WINDOW_OPEN_TIME+1)	return;
	if(mTimer<WINDOW_OPEN_TIME)	color=0x00ffffff;

	int w,h;
	w=mWidth;
	h=mHeight;

	if(position==DT_RIGHT){
		w-=x;
		x=0;
	}

	w-=24+x;
	h-=24+y;
	if(w<0)	return;

	if(y<-8){
		position=position|DT_BOTTOM;
		int i,j;
		DTest.GetTextSize(0,text,&i,&j);
		h=j+y+8;
		y=-8;
	}
	if(h<0)	return;
	DTest.DrawText(fontnum,text,x+(int)mX+16,y+(int)mY+16,color,position,w,h);
}
void	CPlaneWindow::DrawMessage(int fontnum,const char *text,int x,int y,D3DCOLOR color,int position)
{
	if(!mLive)	return;
	if(!text)	return;
	if(mTimer>WINDOW_OPEN_TIME+1)	return;
	if(mTimer<WINDOW_OPEN_TIME)	color=0x00ffffff;

	char mes[512];
	int i=0;
	int old=0;
	int line=0;
	for(int n=0;n<512;n++){
		//mes[i]=text[n];
		if(text[n]=='\n' || text[n]=='\0'){
			strcpy(mes,&text[old]);
			mes[i]='\0';
			DrawText(
				fontnum,mes,
				x,
				y+(DTest.GetFontSize(fontnum)+MESSAGE_SPACE)*line,
				color,position);
			if(text[n]=='\0')	return;
			line++;
			old=n+1;
			i=0;
		}else{
			i++;
		}
	}
}
void	CPlaneWindow::GetMessageSize(int fontnum,const char *text,int *x,int *y)
{
	if(!text)	return;
	int line=0;
	for(int n=0;n<512;n++){
		if(text[n]=='\n'){
			line++;
		}else if(text[n]=='\0'){
			break;
		}
	}
	DTest.GetTextSize(fontnum,text,x,y);
	if(*y)	*y+=line*MESSAGE_SPACE;
}
void	CPlaneWindow::DrawBox(int x,int y,int w,int h,D3DCOLOR color)
{
	if(!mLive)	return;
	int time=mTimer;
	if(mTimer>=WINDOW_OPEN_TIME)
		time=WINDOW_OPEN_TIME*2-mTimer;

	if(x<-8){
		w-=-8-x;
		x=-8;
	}
	if(y<-8){
		h-=-8-y;
		y=-8;
	}
	if(x+w>mWidth-24)	w=mWidth-24-x;
	if(y+h>mHeight-24)	h=mHeight-24-y;

	if(w<0)	return;
	if(h<0)	return;

	x=x*time/WINDOW_OPEN_TIME;
	y=y*time/WINDOW_OPEN_TIME;
	w=w*time/WINDOW_OPEN_TIME;
	h=h*time/WINDOW_OPEN_TIME;

	x=(int)mX+x+16;
	y=(int)mY+y+16;
	DTest.DrawBox(x,y,x+w,y+h,color);
}

void	CPlaneWindow::DrawTexture(CDTTexture* tex,int turn,int fx,int fy,int fsx,int fsy,int tx,int ty,int tsx,int tsy,D3DCOLOR color)
{
	if(!mLive)	return;
	if(mTimer>WINDOW_OPEN_TIME+1)	return;
	if(mTimer<WINDOW_OPEN_TIME)	color=0x00ffffff;

	DTest.DrawTexture(*tex,turn,fx,fy,fsx,fsy,tx+(int)mX+16,ty+(int)mY+16,tsx,tsy,color);
}

void	CPlaneWindow::DrawTexture(CDTTexture* tex,int turn,int fx,int fy,int fsx,int fsy,int tx,int ty,D3DCOLOR color)
{
	if(!mLive)	return;
	if(mTimer>WINDOW_OPEN_TIME+1)	return;
	if(mTimer<WINDOW_OPEN_TIME)	color=0x00ffffff;

	if(tx<-8){
		fx+=-8-tx;
		fsx-=-8-tx;
		tx=-8;
	}
	if(ty<-8){
		fy+=-8-ty;
		fsy-=-8-ty;
		ty=-8;
	}
	if(tx+fsx>mWidth-24)	fsx=mWidth-24-tx;
	if(ty+fsy>mHeight-24)	fsy=mHeight-24-ty;

	DTest.DrawTexture(*tex,turn,fx,fy,fsx,fsy,tx+(int)mX+16,ty+(int)mY+16,fsx,fsy,color);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

CListWindow::CListWindow()
{
	mList=NULL;
	mParam=NULL;
	mShowCursor=true;
}
CListWindow::~CListWindow()
{
	delete[] mList;
	delete[] mParam;
}
void CListWindow::InitYesNoWindow(int x,int y,int w)
{
	static const CElement ele[2]=
	{
			CElement("ÇÕÇ¢"		,0),
			CElement("Ç¢Ç¢Ç¶"	,1)
	};

	InitListWindow(	x,y,w,
					ele,NULL,
					2,2,sizeof(CElement));

}

void CListWindow::InitListWindow(int x,int y,int w,const CElement *listdata,int* list,int listsize,int h,int size,int *param,bool pagemove)
{
	mElementHeight=listdata[0].GetHeight();
	InitPlaneWindow(x,y,w,h*mElementHeight+32);
	mListHeight=h;
	mCursor=0;
	mPage=0;
	mPageY=-10;
	mPageMove=pagemove;

	ResetList(listdata,list,listsize,size,param);
}

void CListWindow::InitListWindow(int x,int y,int w,const CElement **listdata,int* list,int listsize,int h,int *param,bool pagemove)
{
	mElementHeight=listdata[0]->GetHeight();
	InitPlaneWindow(x,y,w,h*mElementHeight+32);
	mListHeight=h;
	mCursor=0;
	mPage=0;
	mPageY=-10;
	mPageMove=pagemove;

	ResetList(listdata,list,listsize,param);
}
void CListWindow::ResetList(const CElement **listdata,int* list,int listsize,int *param)
{
	if(listsize<=0){
		ResetList(NULL,NULL,0,NULL);
		return;
	}
	delete[] mList;
	delete[] mParam;
	mList=new const CElement*[listsize];
	mParam=new int[listsize];
	mListSize=listsize;

	if(list){
		for(int i=0;i<listsize;i++){
			mList[i]=listdata[list[i]];
		}
	}else{
		for(int i=0;i<listsize;i++){
			mList[i]=listdata[i];
		}
	}
	SetParam(param);
	if(mCursor>=mListSize)	mCursor=mListSize-1;
}

void CListWindow::ResetList(const CElement *listdata,int* list,int listsize,int size,int *param)
{
	if(listsize<=0){
		listsize=1;
		static const CElement ele("çÄñ⁄Ç™Ç†ÇËÇ‹ÇπÇÒÅB",0);
		list=NULL;
		listdata=&ele;
	}
	delete[] mList;
	delete[] mParam;
	mList=new const CElement*[listsize];
	mParam=new int[listsize];
	mListSize=listsize;

	if(list){
		for(int i=0;i<listsize;i++){
			mList[i]=(const CElement*)(long long(listdata)+(list[i]*size));
		}
	}else{
		for(int i=0;i<listsize;i++){
			mList[i]=(const CElement*)(long long(listdata)+(i*size));
		}
	}
	SetParam(param);
	
	if(mCursor>=mListSize)	mCursor=mListSize-1;
}
void CListWindow::SetParam(int* param)
{
	if(!param){
		memset(mParam,0,sizeof(int)*mListSize);
		return;
	}
	memcpy(mParam,param,sizeof(int)*mListSize);
}

void CListWindow::SetParam(int no,int param)
{
	if(no<0){
		for(int i=0;i<mListSize;i++){
			mParam[i]=param;
		}
		return;
	}
	mParam[no]=param;
}


int CListWindow::OnTimer(bool cursor)
{
	if(!mLive)	return -1;
	CPlaneWindow::OnTimer();

	int oldcursor=mCursor;
	if(cursor){
		if(pInput->GetRepeatKey(1)==1){
			mCursor--;
			if(mCursor<0)	mCursor=mListSize-1;
			PlayWave(SE_MOVE);
		}
		if(pInput->GetRepeatKey(3)==1){
			mCursor++;
			if(mCursor>mListSize-1)	mCursor=0;
			PlayWave(SE_MOVE);
		}

		if(mListSize>mListHeight && mPageMove){
			if(pInput->GetRepeatKey(0)==1){
				mCursor-=mListHeight;
				mPage-=mListHeight;
				if(mCursor<0)	mCursor=0;
				PlayWave(SE_MOVE);
			}
			if(pInput->GetRepeatKey(2)==1){
				mCursor+=mListHeight;
				mPage+=mListHeight;
				if(mCursor>mListSize-1)				mCursor=mListSize-1;
				PlayWave(SE_MOVE);
			}
		}
		if(mPage>mListSize-mListHeight-1)	mPage=mListSize-mListHeight;
		if(mPage<0)							mPage=0;
		if(mCursor>=mPage+mListHeight)		mPage=mCursor-mListHeight+1;
		if(mCursor<mPage)					mPage=mCursor;

		if(!GetDestroy()){
			int x,y;
			GetCursorPos(&x,&y);
			Cursor.Move(x,y);
			if(pInput->GetKey(4)==1){
				Cursor.Click();
			}
			if(pInput->GetKey(5)==1){
				mList[mCursor]->Help(mParam[mCursor]);
			}
		}
	}

	mPageY-=(mPageY-mPage*mElementHeight+10)/4;

	if(mCursor!=oldcursor)	return mCursor;
	return -1;
}
void	 CListWindow::GetCursorPos(int *x,int *y)
{
	int cy=int((mCursor+1)*mElementHeight-mPageY-8);
	if(cy<0)		cy=0;
	if(cy>mHeight)	cy=mHeight;

	if(x)	*x=int(mX);
	if(y)	*y=(int)mY+cy;
}

void CListWindow::OnDraw(D3DCOLOR color)
{
	if(!mLive)	return;
	CPlaneWindow::OnDraw(color);

	if(mList){
		int y=int(mPageY/mElementHeight);
		for(int i=y;i<mListHeight+y+2;i++){
			if(i==mCursor && mShowCursor){
				DrawBox(0,int((i+1)*mElementHeight-int(mPageY)-22),mWidth-32,8,COLOR_SELECT);
			}
			if(i<mListSize)
				mList[i]->Draw(this,20,int(i*mElementHeight-int(mPageY)-8),mParam[i],color);
		}
		if(mListSize>mListHeight){
			DrawBox(mWidth-30,0,2,mHeight-32,COLOR_LINE);
			int y=int((mPageY+10)*(mHeight-32)/((mListSize-mListHeight)*mElementHeight)-8);
			DrawTexture(&SystemTex,0,16,32,16,16,mWidth-37,y,color);
		}
	}
}
void CListWindow::SetCursor(int cursor)
{
	mCursor=cursor;
	if(mCursor<0)			mCursor=0;
	if(mCursor>mListSize-1)	mCursor=mListSize-1;

	if(mPage>mListSize-mListHeight-1)	mPage=mListSize-mListHeight;
	if(mPage<0)							mPage=0;
	if(mCursor>=mPage+mListHeight)		mPage=mCursor-mListHeight+1;
	if(mCursor<mPage)					mPage=mCursor;
}
void CListWindow::SetItem(int no,const CElement* element)
{
	if(no<0)			return;
	if(no>=mListSize)	return;
	mList[no]=element;
}



/////////////////////////////////////////////////////////////////////////////////////////////////////
void	CItemListWindow::SetPData(CPlayerData &pdata)
{
	mPData=&pdata;
}
void	CItemListWindow::InitListWindow(int x,int y,int w,const CElement **listdata,int* list,int listsize,int h,int *param,bool pagemove)
{
	CListWindow::InitListWindow(x,y,w,listdata,list,listsize,h,param,pagemove);
	InitPlaneWindow(x,y,w,GetHeight()+38);
}
void	CItemListWindow::InitListWindow(int x,int y,int w,const CElement *listdata,int* list,int listsize,int h,int size,int *param,bool pagemove)
{
	CListWindow::InitListWindow(x,y,w,listdata,list,listsize,h,size,param,pagemove);
	InitPlaneWindow(x,y,w,GetHeight()+38);
}

void	 CItemListWindow::GetCursorPos(int *x,int *y)
{
	CListWindow::GetCursorPos(x,y);
	*y += 6;
	if(mCursor>=WEAPON_SPACE)						*y += 12;
	if(mCursor>=WEAPON_SPACE+ARMOR_SPACE)			*y += 12;
	if(mPData && mCursor>=mPData->GetEquipMax())	*y += 12;
}

void CItemListWindow::OnDraw(D3DCOLOR color)
{
	if(!mLive)	return;
	CPlaneWindow::OnDraw(color);

	if(mList){
		int y=-6;
		for(int i=0;i<mListHeight+2;i++){
			if(i==0)
				{	DrawSeparate("Weapon",i,y,0xffd00000);		y+=12;}
			if(i==WEAPON_SPACE)	
				{	DrawSeparate("Armor",i,y,0xff0000d0);		y+=12;}
			if(i==WEAPON_SPACE+ARMOR_SPACE)
				{	DrawSeparate("Accessory",i,y,0xff00b000);	y+=12;}
			if(mPData && i==mPData->GetEquipMax())
				{	DrawSeparate("Use Item",i,y,0xff0080d0);	y+=12;}

			if(i==mCursor && mShowCursor){
				DrawBox(0,int((i+1)*mElementHeight-mPageY-22)+y,mWidth-32,8,COLOR_SELECT);
			}
			if(i<mListSize){
				mList[i]->Draw(this,20,int(i*mElementHeight-mPageY-8+y),mParam[i],color);
			}
		}
		if(mListSize>mListHeight){
			DrawBox(mWidth-30,0,2,mHeight-32,COLOR_LINE);
			int y=int((mPageY+10)*(mHeight-32)/((mListSize-mListHeight)*mElementHeight)-8);
			DrawTexture(&SystemTex,0,16,32,16,16,mWidth-37,y,color);
		}
	}
}
void CItemListWindow::DrawSeparate(const char* name,int no,int y,D3DCOLOR textcolor)
{
	DrawBox(0,int((no)*mElementHeight-mPageY)+y+2,mWidth/3,2,0xff808040);
	DrawText(2,name,-8,int((no)*mElementHeight-mPageY)+y-9,textcolor);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
CSelectWindow::CSelectWindow()
{
	mList=NULL;
}
CSelectWindow::~CSelectWindow()
{
	delete[] mList;
}
void CSelectWindow::InitSelectWindow(int x,int y,int w,const CElement **listdata,int* list,int listsize)
{
	InitPlaneWindow(x,y,w,ELEMENT_Height+16);

	mCursor=0;
	mCursorTimer=0;
	mX=0;
	mSpeedX=0;
	mMoveTimer=0;
	mInWidth=(mWidth-16);

	if(mList)	delete[] mList;
	mList=new const CElement*[listsize];
	mListSize=listsize;

	if(list){
		for(int i=0;i<listsize;i++){
			mList[i]=listdata[list[i]];
		}
	}else{
		for(int i=0;i<listsize;i++){
			mList[i]=listdata[i];
		}
	}
}
void	CSelectWindow::InitSelectWindow(int x,int y,int w,const CElement *listdata,int* list,int listsize,int size)
{
	InitPlaneWindow(x,y,w,ELEMENT_Height+16);

	mCursor=0;
	mCursorTimer=0;
	mX=0;
	mSpeedX=0;
	mMoveTimer=0;
	mInWidth=(mWidth-16);

	if(mList)	delete[] mList;
	mList=new const CElement*[listsize];
	mListSize=listsize;

	if(list){
		for(int i=0;i<listsize;i++){
			mList[i]=(const CElement*)(long long(listdata)+(list[i]*size));
		}
	}else{
		for(int i=0;i<listsize;i++){
			mList[i]=(const CElement*)(long long(listdata)+(i*size));
		}
	}
}

int CSelectWindow::OnTimer(bool cursor)
{
	if(!mLive)	return -1;
	CPlaneWindow::OnTimer();

	if(mMoveTimer){
		mX+=mSpeedX*mInWidth/SWIN_MOVETIME;
		mMoveTimer--;
		if(mX<0)						mX+=(mListSize)*mInWidth-1;
		if(mX>=(mListSize)*mInWidth)	mX=0;
	}else{
		mX=mCursor*mInWidth;
	}

	if(cursor){
		mCursorTimer++;
		bool move=false;
		int	 turn=0;
		if(pInput->GetRepeatKey(0)==1){
			PlayWave(SE_MOVE);
			turn=-1;;
			move=true;
		}
		if(pInput->GetRepeatKey(2)==1){
			PlayWave(SE_MOVE);
			turn=1;;
			move=true;
		}

		if(move){
			mX=mCursor*mInWidth;
			mSpeedX=turn;
			mMoveTimer=SWIN_MOVETIME;

			mCursor+=turn;
			if(mCursor<0)	mCursor=mListSize-1;
			if(mCursor>mListSize-1)				mCursor=0;
			return mCursor;
		}
	}

	return -1;
}

void CSelectWindow::OnDraw(D3DCOLOR color)
{
	if(!mLive)	return;
	CPlaneWindow::OnDraw(color);

	if(mList){
		int x=mX/mInWidth;
		const CElement* e1,*e2;
		e1=mList[x];
		if(x>=mListSize-1){
			e2=mList[0];
		}else{
			e2=mList[x+1];
		}

		int w,h;
		DTest.GetTextSize(0,e1->GetName(),&w,&h);
		DrawText(0,e1->GetName(),mX%mInWidth+(mWidth-32)-w+8-16,-8,0,DT_RIGHT);
		DrawText(0,e2->GetName(),-mX%mInWidth+(mWidth-16)+16,-8,0,DT_LEFT);

		int texturex;
		switch(((mCursorTimer+CURSOR_ANI_TIME-1)/CURSOR_ANI_TIME)%4){
			case 1:
				texturex=1;
				break;
			case 3:
				texturex=2;
				break;
			default:
				texturex=0;
		}
		if(mMoveTimer && mSpeedX<0){
			DrawTexture(&SystemTex,TURN_HORIZONTAL,160,32,32,16,-20,-4,32,16);
		}else{
			DrawTexture(&SystemTex,TURN_HORIZONTAL,64+texturex*32,32,32,16,-20,-4,32,16,color);
		}

		if(mMoveTimer && mSpeedX>0){
			DrawTexture(&SystemTex,0,160,32,32,16,(mWidth-32)-12,-4,32,16);
		}else{
			DrawTexture(&SystemTex,0,64+texturex*32,32,32,16,(mWidth-32)-12,-4,32,16,color);
		}
	}
}
void CSelectWindow::InitArrow()
{
	mCursorTimer=0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

CCharaDataWindowMini::CCharaDataWindowMini()
{
	mCData=NULL;
}
void CCharaDataWindowMini::InitWindow(CBattleCharacter* cdata)
{
	mCData=cdata;
	int x=cdata->GetX()-100;
	int y=cdata->GetY()+50;
	InitPlaneWindow(x,y,WIN_WIDTH,WIN_Height);
}
void CCharaDataWindowMini::OnTimer(void)
{
	if(!mLive)	return;
	int x=mCData->GetX()-100;
	int y=mCData->GetY()+50;
	if(y>200)	y=200;
	Move(x,y);
	CPlaneWindow::OnTimer();
}
void CCharaDataWindowMini::OnDraw(D3DCOLOR color)
{
	if(!mLive)	return;
	CPlaneWindow::OnDraw(color);

	int w,h;
	GetWindowSize(&w,&h);

	DTest.DrawLine(	(int)mX+w,(int)mY+20,
					mCData->GetX()+50,mCData->GetY()+90,LINE_WIDTH,
					0xff404020);
	static char text[128];
	if(mCData){
		DrawText(1,mCData->GetName(),-8,-8+TEXT_Height*0);

		DrawText(1,"HP",-8,-8+TEXT_Height*1);
		sprintf(text,"%d/%d",mCData->GetHP(),mCData->GetMaxHP());
		DrawText(1,text,0,-8+TEXT_Height*1,0,DT_RIGHT);

		DrawText(1,"SP",-8,-8+TEXT_Height*2);
		sprintf(text,"%d/%d/%d/%d",mCData->GetSP(3),mCData->GetSP(2),mCData->GetSP(1),mCData->GetSP(0));
		DrawText(1,text,0,-8+TEXT_Height*2,0,DT_RIGHT);

		DrawText(1,"ATK",-8,-8+TEXT_Height*3);
		sprintf(text,"%d",mCData->GetAtk());
		DrawText(1,text,0,-8+TEXT_Height*3,0,DT_RIGHT);

		DrawText(1,"DEF",-8,-8+TEXT_Height*4);
		sprintf(text,"%d",mCData->GetDef());
		DrawText(1,text,0,-8+TEXT_Height*4,0,DT_RIGHT);

		DrawText(1,"MATK",-8,-8+TEXT_Height*5);
		sprintf(text,"%d",mCData->GetMagAtk());
		DrawText(1,text,0,-8+TEXT_Height*5,0,DT_RIGHT);

		DrawText(1,"MDEF",-8,-8+TEXT_Height*6);
		sprintf(text,"%d",mCData->GetMagDef());
		DrawText(1,text,0,-8+TEXT_Height*6,0,DT_RIGHT);

		DrawText(1,"SPD",-8,-8+TEXT_Height*7);
		sprintf(text,"%d",mCData->GetSpeed());
		DrawText(1,text,0,-8+TEXT_Height*7,0,DT_RIGHT);

	}
}
///////////////////////////////////////////////////////////////////////////
CCharaDataWindowMenu::CCharaDataWindowMenu()
{
}
void CCharaDataWindowMenu::InitWindow(int x,int y,int no,bool reserve)
{
	InitPlaneWindow(x,y,WIN_WIDTH,WIN_Height);

	mNo=no;
	mReserve=reserve;

	char filename[256];
	sprintf(filename,"data\\mapobject\\mapobject%03d",no);
	DTest.LoadTexture(mTex,filename,0xff008080);
}
void CCharaDataWindowMenu::OnDraw(bool select,D3DCOLOR color)
{
	if(!mLive)	return;
	static char text[128];

	D3DCOLOR wincolor=color;
	if(mReserve){
		wincolor=0xffd0d0ff;
	}
	CPlaneWindow::OnDraw(wincolor);
	if(select){
		DrawBox(0,mHeight/2-16,mWidth-32,mHeight/2-16,COLOR_SELECT);
	}
	if(mNo<0){
		DrawText(0,"- Empty -",0,25,0,DT_CENTER);
		return;
	}

	CPlayerData& cdata=Flag.mPlayerData[mNo];
	DrawText(0,cdata.GetName(),64,0);
	D3DCOLOR textcolor=0;
	D3DCOLOR imgcolor=color;
	if(mReserve){
		DrawText(0,"Reserve",200,0,0xff0000ff);
		textcolor=0xff808080;
		imgcolor=0x80ffffff;
	}
	DrawTexture(&mTex,0,0,0,64,64,-32,-32,128,128,imgcolor);

	sprintf(text,"Lv.%d",cdata.GetLv());
	DrawText(0,text,64,25,textcolor);
	sprintf(text,"Exp.%d",cdata.GetExp());
	DrawText(0,text,64,50,textcolor);

	const int RIGHT=10;
	DrawText(0,"LP",150,25,textcolor);
	sprintf(text,"%d",cdata.GetLP());
	DrawText(0,text,RIGHT+60,25,textcolor,DT_RIGHT);
	DrawText(0,"/",RIGHT+40,25,textcolor,DT_RIGHT);
	sprintf(text,"%d",cdata.GetMaxLP());
	DrawText(0,text,RIGHT+0,25,textcolor,DT_RIGHT);

	DrawText(0,"SP",150,50,textcolor);
	int j=0;
	while(1){
		sprintf(text,"%d",cdata.GetSP(j));
		DrawText(0,text,RIGHT+j*30,50,textcolor,DT_RIGHT);
		j++;
		if(j>=MAX_CSKILL_LEVEL)	break;
		DrawText(0,"/",RIGHT+j*30-8,50,textcolor,DT_RIGHT);
	}
}

///////////////////////////////////////////////////////////////////////////
/*CCharaDataWindowHalf::CCharaDataWindowHalf()
{
}
void CCharaDataWindowHalf::InitWindow(int x,int y,int no)
{
	InitPlaneWindow(x,y,WIN_WIDTH,WIN_Height);
	mNo=no;
}
void CCharaDataWindowHalf::OnDraw(D3DCOLOR color)
{
	if(!mLive)	return;
	static char text[128];

	CPlaneWindow::OnDraw(color);

	CPlayerData& cdata=Flag.mPlayerData[mNo];
	int y=0;

	const int RIGHT=15;
	DrawText(0,"LP",0,y);
	sprintf(text,"%d",cdata.GetLP());
	DrawText(0,text,RIGHT+60,y,0,DT_RIGHT);
	DrawText(0,"/",RIGHT+40,y,0,DT_RIGHT);
	sprintf(text,"%d",cdata.GetMaxLP());
	DrawText(0,text,RIGHT+0,y,0,DT_RIGHT);

	y+=25;
	DrawText(0,"SP",0,y);
	int j=0;
	while(1){
		sprintf(text,"%d",cdata.GetSP(j));
		DrawText(0,text,RIGHT+j*35,y,0,DT_RIGHT);
		j++;
		if(j>=MAX_CSKILL_LEVEL)	break;
		DrawText(0,"/",RIGHT+j*35-10,y,0,DT_RIGHT);
	}

	y+=20;
	DrawText(0,"ç≈ëÂSP",0,y);
	j=0;
	while(1){
		sprintf(text,"%d",cdata.GetMaxSP(j));
		DrawText(0,text,RIGHT+j*35,y,0,DT_RIGHT);
		j++;
		if(j>=MAX_CSKILL_LEVEL)	break;
		DrawText(0,"/",RIGHT+j*35-10,y,0,DT_RIGHT);
	}

	y+=30;
	DrawText(0,"çUåÇóÕ",0,y);
	sprintf(text,"%d",cdata.GetAtk());
	DrawText(0,text,RIGHT+60,y,0,DT_RIGHT);
	DrawText(0,"(",RIGHT+40,y,0,DT_RIGHT);
	sprintf(text,"%+d)",cdata.GetAtk()-cdata.GetBaseAtk());
	DrawText(0,text,RIGHT-15,y,0,DT_RIGHT);

	y+=25;
	DrawText(0,"ñhå‰óÕ",0,y);
	sprintf(text,"%d",cdata.GetDef());
	DrawText(0,text,RIGHT+60,y,0,DT_RIGHT);
	DrawText(0,"(",RIGHT+40,y,0,DT_RIGHT);
	sprintf(text,"%+d)",cdata.GetDef()-cdata.GetBaseDef());
	DrawText(0,text,RIGHT-15,y,0,DT_RIGHT);

	y+=30;
	DrawText(0,"ñÇñ@çUåÇ",0,y);
	sprintf(text,"%d",cdata.GetMagAtk());
	DrawText(0,text,RIGHT+60,y,0,DT_RIGHT);
	DrawText(0,"(",RIGHT+40,y,0,DT_RIGHT);
	sprintf(text,"%+d)",cdata.GetMagAtk()-cdata.GetBaseMagAtk());
	DrawText(0,text,RIGHT-15,y,0,DT_RIGHT);

	y+=25;
	DrawText(0,"ñÇñ@ñhå‰",0,y);
	sprintf(text,"%d",cdata.GetMagDef());
	DrawText(0,text,RIGHT+60,y,0,DT_RIGHT);
	DrawText(0,"(",RIGHT+40,y,0,DT_RIGHT);
	sprintf(text,"%+d)",cdata.GetMagDef()-cdata.GetBaseMagDef());
	DrawText(0,text,RIGHT-15,y,0,DT_RIGHT);

	y+=30;
	DrawText(0,"Ç∑ÇŒÇ‚Ç≥",0,y);
	sprintf(text,"%d",cdata.GetSpeed());
	DrawText(0,text,RIGHT+60,y,0,DT_RIGHT);
	DrawText(0,"(",RIGHT+40,y,0,DT_RIGHT);
	sprintf(text,"%+d)",cdata.GetSpeed()-cdata.GetBaseSpeed());
	DrawText(0,text,RIGHT-15,y,0,DT_RIGHT);

	y+=30;
	DrawText(0,"ãZèp",0,y);
	sprintf(text,"%d",cdata.GetTech());
	DrawText(0,text,RIGHT+60,y,0,DT_RIGHT);
	DrawText(0,"(",RIGHT+40,y,0,DT_RIGHT);
	sprintf(text,"%+d)",cdata.GetTech()-cdata.GetBaseTech());
	DrawText(0,text,RIGHT-15,y,0,DT_RIGHT);

}
*/

///////////////////////////////////////////////////////////////////////////
CCharaDataWindowCompare::CCharaDataWindowCompare()
{
	mBefore=NULL;
	mAfter=NULL;
}
void CCharaDataWindowCompare::InitWindow(int x,int y,CPlayerData* before,CPlayerData* after)
{
	InitPlaneWindow(x,y,WIN_WIDTH,WIN_Height);

	mBefore=before;
	mAfter=after;
}
void CCharaDataWindowCompare::OnDraw(D3DCOLOR color)
{
	if(!mLive)		return;
	if(!mBefore)	return;
	if(!mAfter)		return;
	static char text[128];

	CPlaneWindow::OnDraw(color);

	DrawLine(0,"LP"			,mBefore->GetMaxLP(),mAfter->GetMaxLP());
	DrawLine(4,"çUåÇóÕ"		,mBefore->GetAtk(),mAfter->GetAtk());
	DrawLine(5,"ñhå‰óÕ"		,mBefore->GetDef(),mAfter->GetDef());
	DrawLine(6,"ñÇñ@çUåÇ"	,mBefore->GetMagAtk(),mAfter->GetMagAtk());
	DrawLine(7,"ñÇñ@ñhå‰"	,mBefore->GetMagDef(),mAfter->GetMagDef());
	DrawLine(8,"Ç∑ÇŒÇ‚Ç≥"	,mBefore->GetSpeed(),mAfter->GetSpeed());

	int y=int(1.5*ELEMENT_Height-4);
	DrawText(0,"SP",0,y);
	int x=100;
	int j=0;
	int sp[MAX_CSKILL_LEVEL];
	while(1){
		sp[j]=mBefore->GetMaxSP(j);
		sprintf(text,"%d",sp[j]);
		DrawText(0,text,x+j*30,y,0,DT_RIGHT);
		j++;
		if(j>=MAX_CSKILL_LEVEL)	break;
		DrawText(0,"/",x+j*30-8,y,0,DT_RIGHT);
	}

	y+=ELEMENT_Height;
	x=40;
	DrawText(0,"Å®",150,y,0,DT_RIGHT);

	j=0;
	while(1){
		int aftersp=mAfter->GetMaxSP(j);
		sprintf(text,"%d",aftersp);
		D3DCOLOR color= 0xff000000;
		if(sp[j]>aftersp)	color=0xff0000ff;
		if(sp[j]<aftersp)	color=0xffff0000;
		DrawText(0,text,x+j*30,y,color,DT_RIGHT);
		j++;
		if(j>=MAX_CSKILL_LEVEL)	break;
		DrawText(0,"/",x+j*30-8,y,0,DT_RIGHT);
	}
}
void	CCharaDataWindowCompare::DrawLine(int line,const char* title,int before,int after)
{
	int y=line*ELEMENT_Height-4;
	char text[128];

	DrawText(0,title,0,y);

	sprintf(text,"%d",before);
	DrawText(0,text,160,y,0,DT_RIGHT);

	DrawText(0,"Å®",130,y,0,DT_RIGHT);

	sprintf(text,"%d",after);
	DrawText(0,text,80,y,0,DT_RIGHT);

	if(before>after){
		sprintf(text,"Å´%d",before-after);
		DrawText(0,text,WIN_WIDTH-80,y,0xff0000ff);
	}else if(after>before){
		sprintf(text,"Å™%d",after-before);
		DrawText(0,text,WIN_WIDTH-80,y,0xffff0000);
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////
void CCharaDataWindowEquip::InitWindow(int x,int y,int equip,CPlayerData* before,bool carry)
{
	CCharaDataWindowCompare::InitWindow(x,y,before,&mAfter);
	mCarry=carry;
	if(mCarry){
		SetEquipCarry(equip);
	}else{
		SetEquip(equip);
	}
}
void CCharaDataWindowEquip::SetEquip(int equip,int no)
{
	mEquip=equip;

	mAfter=*mBefore;
	if(equip==NO_ITEM){
		mAfter.TakeOff(no);
	}else{
		if(no<0)	no=mAfter.GetEquipMax()+1;
		mAfter.SetItem(no,equip);
		mAfter.Equip(no,true);
	}
}
void CCharaDataWindowEquip::SetEquipCarry(int no)
{
	mEquip=no;

	mAfter=*mBefore;
	if(mAfter.CheckEquiped(no)){
		mAfter.TakeOff(no);
	}else{
		mAfter.Equip(no,true);
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////void CCharaDataWindowEquip::InitWindow(int x,int y,int equip,CPlayerData* before,bool carry)

CItemHelpWindow::CItemHelpWindow()
{
	mHelp=NULL;
}
CItemHelpWindow::~CItemHelpWindow()
{
	delete[] mHelp;
}
void CItemHelpWindow::InitWindow(int x,int y,int item)
{
	InitPlaneWindow(x,y,WIN_WIDTH,WIN_Height+125);
	ChangeItem(item);
}
void CItemHelpWindow::OnDraw(D3DCOLOR color)
{
	if(!mLive)		return;

	CPlaneWindow::OnDraw(color);

	DrawMessage(0,mHelp,0,24);
	AllItem[mItemNo]->Draw(this,0,0,0);

	DrawBox(-32,88,999,2,COLOR_LINE);
	DrawText(0,AllItem[mItemNo]->GetTypeName(),0,95);
	if(AllItem[mItemNo]->mType & TYPE_EQUIPMENT){
		DrawParam(this,10,120,mItemNo,0,true);
		for(int i=0;i<EquipmentData[mItemNo].mSkillNo;i++){
			int skill = EquipmentData[mItemNo].mSkill[i];
			SkillData[skill].Draw(this,140,90+i*15,16,color);
		}
	}

}
void CItemHelpWindow::ChangeItem(int item)
{
	mItemNo=item;

	delete[] mHelp;
	mHelp=CMenuData::GetItemTextShort(item,true);

	if(AllItem[item]->mType & TYPE_EQUIPMENT){
	}
}

void CItemHelpWindow::DrawParam(CPlaneWindow* win,int x,int y,int item,int max,bool force)
{
	int i=0;
	if(EquipmentData[item].mLP		|| force)	{DrawParam(win,x,y,max,"LP",EquipmentData[item].mLP,i,0.15);			i++;};
	if(EquipmentData[item].mAtk		|| force)	{DrawParam(win,x,y,max,"çUåÇóÕ",EquipmentData[item].mAtk,i);		i++;};
	if(EquipmentData[item].mDef		|| force)	{DrawParam(win,x,y,max,"ñhå‰óÕ",EquipmentData[item].mDef,i);		i++;};
	if(EquipmentData[item].mMagAtk	|| force)	{DrawParam(win,x,y,max,"ñÇñ@çUåÇ",EquipmentData[item].mMagAtk,i);	i++;};
	if(EquipmentData[item].mMagDef	|| force)	{DrawParam(win,x,y,max,"ñÇñ@ñhå‰",EquipmentData[item].mMagDef,i);	i++;};
	if(EquipmentData[item].mSpeed	|| force)	{DrawParam(win,x,y,max,"Ç∑ÇŒÇ‚Ç≥",EquipmentData[item].mSpeed,i,2);	i++;};
}
void CItemHelpWindow::DrawParam(CPlaneWindow* win,int x,int y,int max,const char* name,int param,int i,double ratio)
{
const int Height=16;
const int font=2;

	if(i>=max && max>0){
		i=max-1;
		win->DrawBox(x,i*Height+y,1000,1000,COLOR_WIN);
		win->DrawText(font,"...etc",x+50,i*Height+y);
		return;
	}

	int bar=int(pow(double(abs(param)),0.7)*ratio);
	D3DCOLOR fcolor=0xffb0b0b0;
	if(param!=0){
		bar++;
		if(param>0){
			win->DrawBox(x+60,i*Height+y+8,bar,6, 0xffffb0b0);
			fcolor=0xffb00000;
		}else{
			win->DrawBox(x+60-bar,i*Height+y+8,bar,6,0xffb0b0ff);
			fcolor=0xff0000b0;
		}
	}
	win->DrawText(font,name,x+0,i*Height+y);
	char text[64];
	sprintf(text,"%+d",param);
	win->DrawText(font,text,x+60,i*Height+y,fcolor);
}
///////////////////////////////////////////////////////////////////////////////////////////////////
CBattleActionNameWindow::CBattleActionNameWindow(CBattleAction* act)
{
	mAction=act;
	mNo=act->GetAction();
}

void	CBattleActionNameWindow::OnDraw()
{
	if(!mLive)	return;

	const char* text=ComSkillData[mNo].GetName();
	if(GetDestroy()){
		int y=int(mY)+ (GetTimer()-WINDOW_OPEN_TIME)*2;
		int alpha=(WINDOW_OPEN_TIME*2-GetTimer()+1)*255/WINDOW_OPEN_TIME;
		CPlaneWindow::DrawWindow(int(mX),y,mWidth,mHeight,D3DCOLOR_ARGB(alpha,255,255,255));

		int w,h;
		w=mWidth-24;
		h=mHeight-16;

		DTest.DrawText(6,text,(int)mX+16,y+8,D3DCOLOR_ARGB(alpha,1,1,1),DT_CENTER,w,h);
	}else{
		int w,h;
		GetWindowSize(&w,&h);
		int x=int(mX+(mWidth-w)/2);
		int y=int(mY+(mHeight-h)/2);
		CPlaneWindow::DrawWindow(x,y,w,h);
		DrawText(6,text,0,-8,0,DT_CENTER);
	}
}