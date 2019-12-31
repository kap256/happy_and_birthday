#include "mapmessage.h"
#include "cursor.h"
#include "input.h"
#include "flag.h"
#include "mapmode.h"

CNumber forDebug;
CMapMessage::CMapMessage(void)
{
	CPlaneWindow::CPlaneWindow();
}
void	CMapMessage::InitMapMessage(const char* mes,CNumber x,CNumber y,CNumber z,int origin,int para)
{
	int j=0;
	for(int i=0;i<256;i++){
		if(mes[j]=='$'){
			j++;
			switch(mes[j]){
				case 'n':
					mMessage[i]='\n';	break;
				case 's':
					mMessage[i]=' ';	break;
				case 'f':
					{
						int flag=GetMapMessageNo(mes,j);

						int& no=Flag.mFlag[flag];
						char notxt[32];
						sprintf(notxt,"%d",no);

						AddMapMessage(mes,i,notxt);
					}
					break;
				case 'm':
					{
						int member;
						if(mes[j+1]=='f'){
							j++;
							int flag=GetMapMessageNo(mes,j);
							member=Flag.mFlag[flag];
						}else{
							member=GetMapMessageNo(mes,j);
						}
						if(0<member && member<MAX_CHARACTER_NUMBER){
							AddMapMessage(mes,i,CharacterBaseData[member].GetName());
						}
					}
					break;
				default:
					mMessage[i]=mes[j];
			}
		}else{
			mMessage[i]=mes[j];
		}
		if(mes[j]=='\0'){
			break;
		}
		j++;
	}
	mMapMesX=x;
	forDebug=x;
	mMapMesY=y;
	mMapMesZ=z;
	mOrigin=origin;
	mMesTimer=0;
	mParam=para;

	int w,h;
	GetMessageSize(0,mMessage,&w,&h);
	CPlaneWindow::InitPlaneWindow(0,-240,w+32+MESSAGE_SPACE,h+32+MESSAGE_SPACE);
}
int 	CMapMessage::GetMapMessageNo(const char* mes,int& n)
{
	char num[32];
	for(int k=0;k<32;k++){
		n++;
		if('0'<=mes[n] && mes[n]<='9'){
			num[k]=mes[n];
		}else{
			num[k]='\0';
			break;
		}
	}
	n--;
	return atoi(num);
}
void	CMapMessage::AddMapMessage(const char* mes,int& n,const char* text)
{
	
	size_t tlen=strlen(text);
	for(size_t k=0;k<tlen;k++){
		mMessage[n]=text[k];
		n++;
	}
	n--;
}

void	CMapMessage::OnTimer(void)
{
	CPlaneWindow::OnTimer();
	if(GetInited()){
		mMesTimer++;
	}
}

void	CMapMessage::OnDraw(int cx,int cy)
{
	if(!mLive)	return;
	DrawWindow(cx,cy);

	char text[256];
	strcpy(text,mMessage);
	int end=GetTextEnd(text,mMesTimer);
	if(end<256)	text[end]='\0';
	int position=DT_LEFT;

	DrawMessage(0,text,MESSAGE_SPACE/2,MESSAGE_SPACE/2,0,position);
}
void	CMapMessage::DrawWindow(int cx,int cy)
{
	if(!mLive)	return;

	int w,h;
	GetWindowSize(&w,&h);
	CulculateXY(w,h,cx,cy);

	CPlaneWindow::DrawWindow((int)mX,(int)mY,w,h);

	int dx,dy,turn;
	turn=0;
	if(mOrigin!=MAP_MES_CENTER){
		if(mOrigin&0x01){
			dy=int(mY+h-16);
		}else{
			turn=TURN_VERTICAL;
			dy=int(mY-16);
		}

		if(mOrigin&0x10){
			dx=int(mX+w-32);
		}else{
			dx=int(mX+16);
		}
		DTest.DrawTexture(SystemTex,turn,0,32,16,32,dx,dy);
	}
}
void	CMapMessage::CulculateXY(int w,int h,int cx,int cy)
{
	int x,y,z;
	x=mMapMesX.GetNumber();
	y=mMapMesY.GetNumber();
	z=mMapMesZ.GetNumber();

	mX=x-y+32;
	mY=(x+y)/2-z+16;

	mX=mX*MAP_CHIP_SIZE/32;
	mY=mY*MAP_CHIP_SIZE/32;

	if(mOrigin!=MAP_MES_CENTER){
		if(mOrigin&0x01){
			mY-=h+MAP_CHIP_SIZE*3/4;
		}else{
			mY+=MAP_CHIP_SIZE/4;
		}

		if(mOrigin&0x10){
			mX-=w-24;
		}else{
			mX-=24;
		}
	}else{
		mX-=w/2;
		mY-=h/2;
	}

	mX-=cx;
	mY-=cy;
	Move(int(mX),int(mY));
}
int		CMapMessage::GetTextEnd(const char* text,int no)
{
	int size=0;
	int i=0;
	while(text[size]!='\0'){
		if(text[size]&0x80)	size+=2;
		else				size++;
		i++;
		if(i>no)	break;
	}
	return size;
}

void	CMapMessage::QuickDestroy()
{
	this->mLive=false;
}
/////////////////////////////////////////////////////

void CMapSelectWindow::InitSelectWindow(const char* mes,CNumber x,CNumber y,CNumber z,int origin,int no,int* go)
{
	InitMapMessage(mes,x,y,z,origin,0);
	mNo=no;
	if(mNo>MAP_SELECT_MAX)	mNo=MAP_SELECT_MAX;

	for(int i=0;i<mNo;i++){
		mGoto[i] = go[i];
	}

	int j=1;
	mMesTop[0]=0;
	for(int i=0;i<256;i++){
		if(mMessage[i]=='\n'){
			mMessage[i]='\0';
			mMesTop[j]=i+1;
			j++;
			if(j>=mNo)	break;
		}
	}

	mHeight=32+mNo*22;
	mWidth+=20;
	mSelect=0;
}
void	CMapSelectWindow::OnTimer(void)
{
	if(!mLive)	return;
	CMapMessage::OnTimer(); 
	if(GetDestroy())	return;

	if(pInput->GetRepeatKey(1)==1){
		PlayWave(SE_MOVE);
		mSelect--;
		if(mSelect<0)	mSelect=mNo-1;
	}
	if(pInput->GetRepeatKey(3)==1){
		PlayWave(SE_MOVE);
		mSelect++;
		mSelect%=mNo;
	}

	Cursor.Move(int(mX-8),int(mY+32+mSelect*22));
}
void	CMapSelectWindow::OnDraw(int cx,int cy)
{
	if(!mLive)	return;
	DrawWindow(cx,cy);

	for(int i=0;i<mNo;i++){
		if(mSelect==i){
			DrawBox(0,i*22+12,mWidth-32,8,COLOR_SELECT);
		}
		DrawText(0,&(mMessage[mMesTop[i]]),10,i*22+4);
	}
}

///////////////////////////////////////////////////////////////////////////////

CMapMiniMessage::CMapMiniMessage()
{
	mMessage=NULL;
	mNext=NULL;
}
CMapMiniMessage::~CMapMiniMessage()
{
	delete[] mMessage;
}

void CMapMiniMessage::SetMessage(int x,int y,int z,const char* mes,D3DCOLOR color)
{
	CMapMiniMessage* minimes=this;

	while(minimes->mNext){
		minimes=minimes->mNext;
	}

	minimes->mNext=new CMapMiniMessage;
	minimes->mNext->Init(x,y,z,mes,color);
}

void CMapMiniMessage::Init(int x,int y,int z,const char* mes,D3DCOLOR color)
{

	mX=x;
	mY=y;
	mZ=z;
	mColor=color;
	if(!mes){
		mes="ERROR!";
	}
	mSize=int(strlen(mes));
	mMessage=new char[mSize+1];
	strcpy(mMessage,mes);

	mTimer=0;
}
void CMapMiniMessage::OnTimer(void)
{
	mTimer++;
	mZ+=MMES_SPEED_Z;

	while(mNext && mNext->mTimer >= MMES_TIME_DELETE){
		CMapMiniMessage* del=mNext;
		mNext=mNext->mNext;
		delete del;
	}

	if(mNext)	mNext->OnTimer();
}
void CMapMiniMessage::OnDraw(int cx,int cy)
{
	int x=mX-mY+32;
	int y=(mX+mY)/2-int(mZ);

	x=int(x*MAP_CHIP_SIZE/32);
	y=int(y*MAP_CHIP_SIZE/32);

	if(mMessage){
		int ty=y-cy;
		for(int i=0;i<mSize;i++){
			int fx=(mMessage[mSize-i-1]%32)*8;
			int fy=52+(mMessage[mSize-i-1]/32)*12;

			int tx=x-cx+(-i+1)*MMES_WIDTH;
			D3DCOLOR color=mColor;

			if(mTimer>MMES_TIME_VANISH){
				color=((0xff*(MMES_TIME_DELETE-mTimer)/(MMES_TIME_DELETE-MMES_TIME_VANISH))<<24) + (mColor % 0x01000000);
			}
			DTest.DrawTexture(	SystemTex,0,
								fx,fy,8,12,
								tx,ty,8,12,
								color);
		}
	}

	if(mNext)	mNext->OnDraw(cx,cy);
}
void CMapMiniMessage::AllDelete(void)
{
	CMapMiniMessage* mes=mNext;

	while(mes){
		CMapMiniMessage* del=mes;
		mes=mes->mNext;
		delete del;
	}

	mNext=NULL;
}

