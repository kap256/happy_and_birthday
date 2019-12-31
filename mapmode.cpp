#include "mapmode.h"
#include "input.h"
#include "number.h"
#include "menudata.h" 
#include "cursor.h" 
#include "option.h"
#include "month.h"

#define DRAW_MAP_RPG
#include "drawmap.h"

double MAP_CHIP_SIZE=64;

int CMapMode::GetMapHeight(int x,int y,int z)
{
	if(x<0)				return 0;
	if(y<0)				return 0;
	if(z<0)				return 0;
	if(x>=MAP_SIZE_X)	return 0;
	if(y>=MAP_SIZE_Y)	return 0;
	if(z>=MAP_SIZE_Z)	return 0;

	for(int i=z;i>=0;i--){
		int &chip=mMapData[x][y][i];
		if(GetChipType(x,y,i) & MCHIP_GROUND){
			return i;
		}
	}
	return 0;
}
////////////////////////////////////////////////////////
CMapMode::CMapMode(void)
{
	memset(mObject,0,sizeof(mObject));

	mSkipWindow.InitPlaneWindow(320,490,300,80,true);

	mHawkEye = false;

//ザコ戦用イベントデータ
	CEventData *enc=&mEncountEvent;
	enc->mEvent=EVE_ENCOUNT;
	enc->mNext= new CEventData;
	enc=enc->mNext;
	enc->mEvent=EVE_CHECK_BATTLE;
//宝箱用イベントデータ
	mBoxEvent.mEvent=EVE_OPEN_BOX;
	mEventSkip=0;

	mBGMWait=-1;

#ifdef _SHOW_OBJECT_IMG_NO_
	mDebugShowImgNo=false;
#endif
}
CMapMode::~CMapMode(void)
{
	mEventHeader.AllDelete();
	mEventData.AllDelete();
	mEncountEvent.AllDelete();
	mEvent.AllDelete();
}
void CMapMode::AfterStartUp(void)
{
	for(int n=0;n<MAP_CHIP_NO-1;n++){
		char filename[256];
		sprintf(filename,"data\\mapchip\\mapchip%02d",n+1);
		DTest.LoadTexture(mMapChipImg[n],filename,0xff008080);
	}

	mMapNo=-1;
	LoadMap(mMapNo,5,15,2,1);
}
void CMapMode::LoadMap(int mapno,int x,int y,int z,int dir)
{
	MAP_CHIP_SIZE=64;

	mMapSizeX=MIN_MAP_SIZE; 
	mMapSizeY=MIN_MAP_SIZE;

	mBGImg.Release();
	mMapNo=mapno;
	CreatePlayerObject(0,x,y,z,dir);

	int i;
	for(i=1;i<OBJECT_NO;i++){
		DeleteObject(i);
	}
	for(i=0;i<MAP_MESSAGE_NO;i++){
		mMessage[i].QuickDestroy();
	}
	mMiniMessage.AllDelete();
	mSelectWindow.QuickDestroy();
	mSelectEvent=NULL;

	memset(mMapFlag,0,sizeof(mMapFlag));
	memset(mMapData,0,sizeof(mMapData));

	mStop=0;

	mTrapReverse=false;
	mTrapStop=0;
	mEnemyLvUp=0;

	mWaitWindow.QuickDestroy();

	

	char filename[256];
	sprintf(filename,"data\\mapdata\\mapdata%04d",mMapNo);
	LoadMapData(filename);

	memset(mGotoData,0,sizeof(mGotoData));
	memset(mGotoLine,-1,sizeof(mGotoLine));
	mGotoCurrent=0;
	SetCamera(0,0,true);
	mCameraSpeed=1;

	mEventID=0;
	mTime=0;
	mNoWalkTimer=0;
	mCancelKeyTimer=0;
	mDecideKeyTimer=0;
	mNoTalk=false;

	mEvent.AllDelete();
	mRuningEvent=NULL;

	mEventHeader.AllDelete();
	CEventHeader *eh=&mEventHeader;

	mEventData.AllDelete();
	CEventData *edata=&mEventData;


	sprintf(filename,"data\\event\\event%04d",mMapNo);
	FILE* pFile=fopen(filename,"rb");
	if(pFile){
		int tlen;
		for(tlen=0;tlen<256;tlen++){
			Flag.mMapName[tlen]=getc(pFile)-64;
			if(Flag.mMapName[tlen]=='\0')	break;
			if(Flag.mMapName[tlen]=='\n')
				Flag.mMapName[tlen]=' ';
		}
		int i;
		for(i=0;i<MAX_ENEMY_TYPE;i++){
			Flag.mEnemyType[i]=_getw(pFile);
			if(Flag.mEnemyType[i]<0)	break;
		}
		Flag.mEnemyTypeNo=i;
		int	headstart=ftell(pFile);
				
		while(_getw(pFile)>0){
			fseek(pFile,4*3,SEEK_CUR );
		}

		int event;
		int line=0;
		while((event=_getw(pFile))>0){
			edata->mNext=new CEventData;
			edata=edata->mNext;
			edata->mEvent=event;
			int einfo;
			for(einfo=0;einfo<MAX_EVENT_PATTERN;einfo++){
				if(EventDataInfo[einfo].mEvent==event)	break;
			}
			if(EventDataInfo[einfo].mText){
				char text[1024];
				int tlen=0;
				for(tlen=0;tlen<1024;tlen++){
					text[tlen]=getc(pFile)-64;
					if(text[tlen]=='\0')	break;
				}
				edata->mText=new char[tlen+2];
				strcpy(edata->mText,text);
			}
			for(int i=0;i<EventDataInfo[einfo].mDataNo;i++){
				edata->mData[i].LoadNumber(pFile);
			}
			edata->mLine=line;
			line++;
		}

		fseek(pFile,headstart,SEEK_SET);
		while((event=_getw(pFile))>0){
			eh->mNext=new CEventHeader;
			eh=eh->mNext;
			eh->mEvent=event;
			eh->mParamA=_getw(pFile);
			eh->mParamB=_getw(pFile);
			int line=_getw(pFile);
			eh->mCurrent=GetEventData(line);
		}

		fclose(pFile);
//ザコ戦呼び出し用イベントヘッダ
		eh->mNext=new CEventHeader;
		eh=eh->mNext;
		eh->mEvent=EH_ENCOUNT;
		eh->mParamA=-1;
		eh->mParamB=-1;
		eh->mCurrent=&mEncountEvent;
//宝箱呼び出し用イベントヘッダ
		eh->mNext=new CEventHeader;
		eh=eh->mNext;
		eh->mEvent=EH_BOX;
		eh->mParamA=-1;
		eh->mParamB=-1;
		eh->mCurrent=&mBoxEvent;
	}

	mMapNameTimer=0;
	mMonthTitleTimer=0;
	int x,y;
	DTest.GetTextSize(4,Flag.mMapName,&x,&y);
	mMapNameSize=x;

	mMapColor=0;

	mHawkEye = false;
	Flag.mClosetUse=false;

	mShowImage=false;
	mImage.Release();
	SendMessage(EH_LOAD_MAP,0,0);
	OnTimer();
#ifdef _OUTPUT_MAP_BMP_
	int zure=(5*32/_OUTPUT_MAP_BMP_SIZE_);
	mObject[0]->SetPos(-mMapSizeY/2+zure,mMapSizeY/2-zure,13);
#endif
}
void CMapMode::OnTimer(void)
{
#ifdef _SHOW_OBJECT_IMG_NO_
	if(DTest.GetKey(DIK_SPACE)==1){
		mDebugShowImgNo=!mDebugShowImgNo;
	}
#endif
	mTime++;
	mTrapStop--;
	SendMessage(EH_TIME_FLAME,-1,-1);
	if(mTime%60==0)	SendMessage(EH_TIME_SEC,-1,-1);
	if(mMapNameTimer>=4 || mStop==0){
		mMapNameTimer++;
	}
	if(mMonthTitleTimer>0){
		mMonthTitleTimer++;
	}
	mWaitWindow.OnTimer();

	switch(mStop){
		case 2:
			int x,y,dir;
			dir=mObject[0]->GetDir();
			mObject[0]->GetComp(dir,&x,&y);
			x+=mObject[0]->GetX_32();
			y+=mObject[0]->GetY_32();
			for(int i=0;i<OBJECT_NO;i++){
				if(mObject[i]){
					int tx=mObject[i]->GetX_32();
					int ty=mObject[i]->GetY_32();
					mObject[i]->SetStop(x==tx && y==ty);
				}
			}
			break;
		case 3:
			for(int i=0;i<OBJECT_NO;i++){
				if(mObject[i]){
					mObject[i]->SetStop(true);
				}
			}
			break;
		case 4:
			for(int i=0;i<OBJECT_NO;i++){
				if(mObject[i] && dynamic_cast<CEnemyObject *>(mObject[i])){
					mObject[i]->SetStop(true);
				}
			}
	}


	int action=mObject[0]->GetAction();
	switch(action){
		case ACTION_THROW_STONE:
		{
			PlayWave(SE_MISS);
			int x=mObject[0]->GetX_32();
			int y=mObject[0]->GetY_32();
			int z=mObject[0]->GetZ_32();
			int dir=mObject[0]->GetDir();
			CreateStoneObject(OBJECT_NO-1,x,y,z,dir);
			break;
		}
	}

	for(int i=0;i<OBJECT_NO;i++){
		if(mObject[i])	mObject[i]->OnTimer(mMapSizeX,mMapSizeY,this,mObject);
	}

	if(mStop<=0 && !(mSelectWindow.GetLive()) && action==0){
		int dir=-1;
		int time=0x7fffffff;
		if(mObject[0]->CheckGoal()){
			for(int n=0;n<4;n++){
				int k=pInput->GetKey(n);
				if(k>0 && k<=time){
					dir=n;
					time=k;
				}
			}
		}
		if(dir>=0){
			if(mTrapStop >0){
				int olddir=mObject[0]->GetDir();
				mObject[0]->SetDir(dir);
				if(olddir!=dir)	PlayWave(SE_HIT);
			}else{
				if(Flag.mMapMoveTurn){
					dir=dir+3%4;
				}
				if(mTrapReverse){
					dir=dir+2%4;
				}
				mObject[0]->SetGoalDir(dir,1);
			}
			mNoWalkTimer=0;
		}

		
		if(pInput->GetKey(4)>0){
			mNoWalkTimer=0;
			if(pInput->GetKey(4)==1){
				mNoTalk=true;
				for(int ObjNo=1;ObjNo<OBJECT_NO;ObjNo++){
					if (mObject[ObjNo]){
						if(mObject[ObjNo]->CheckTalk(mObject[0])){
							mNoTalk=false;
						}
					}
				}
				if(mNoTalk){
					if(mDecideKeyTimer<=0)	mDecideKeyTimer=1;
				}
			}else{
				if(mDecideKeyTimer>0){
					mDecideKeyTimer++;
					if(mDecideKeyTimer>=30 && mNoTalk){
						mHawkEye=!mHawkEye;
						mNoTalk=false;
					}
				}
			}

		}else{
			if(mNoTalk){
				SendMessage(EH_TALK,0,-1);
				mNoTalk=false;
			}
			mDecideKeyTimer=0;
		}
		if(pInput->GetKey(5)==1){
			MenuData.SetMenu(StartMenu,0);
		}

		bool dash=(pInput->GetKey(6)>0);
		if(Flag.mAutoDash)	dash= !dash;
		mObject[0]->SetDash(dash);

		if(pInput->GetKey(6)>0){
			mNoWalkTimer=0;
			if(mCancelKeyTimer<=0){
				if(pInput->GetKey(6)==1)	mCancelKeyTimer=1;
			}else {
				mCancelKeyTimer++;
			}
		}else{
			if(mCancelKeyTimer>0 && mCancelKeyTimer<16 && mObject[0]->CheckGoal()&&  Flag.SearchFSkill(288)){
				mObject[0]->SetAction(ACTION_THROW_STONE);
			}

			mCancelKeyTimer=0;
		}
		mNoWalkTimer++;
	}else{
		mNoWalkTimer=0;
		mObject[0]->SetDash(false);
	}

	bool hawkeye=(Flag.SearchFSkill(274));
#ifdef _HAWK_EYE_
	hawkeye=true;
#endif

	if(mHawkEye && hawkeye){
		if(MAP_CHIP_SIZE>32){
			MAP_CHIP_SIZE-=2.0;
			if(MAP_CHIP_SIZE<32)	MAP_CHIP_SIZE=32;
		}
	}else{
		if(MAP_CHIP_SIZE<64){
			MAP_CHIP_SIZE+=3.0;
			if(MAP_CHIP_SIZE>64)	MAP_CHIP_SIZE=64;
		}
	}

	mSelectWindow.OnTimer();
	if(!mSelectWindow.GetDestroy()){
		if(pInput->GetKey(4)==1){
			int	line=mSelectWindow.GetGotoLine();
			GoToLine(line,mSelectEvent);

			mSelectWindow.Destroy();
			WaitBreak(true);
			PlayWave(SE_YES);
		}
		if(pInput->GetKey(6)==1){
			int	line=mSelectWindow.GetGotoLine(mSelectDefault);
			GoToLine(line,mSelectEvent);

			mSelectWindow.Destroy();
			WaitBreak(true);
			PlayWave(SE_NO);
		}
	}else{
		if(pInput->GetKey(4)==1 || pInput->GetKey(6)==1){
			WaitBreak(false);
		}
	}

	for(int i=0;i<MAP_MESSAGE_NO;i++){
		mMessage[i].OnTimer();
	}
	mMiniMessage.OnTimer();


	if(mEventSkip>=0x80000000+ESKIP_BLACK_OUT-1){
		mCameraX = mCameraGoalX;
		mCameraY = mCameraGoalY;
	}else{
		double cx = mCameraX - mCameraGoalX;
		double cy = mCameraY - mCameraGoalY;
		double cl = hypot(cx,cy);
		if(cl<mCameraSpeed){
			mCameraX = mCameraGoalX;
			mCameraY = mCameraGoalY;
		}else{
			cl/=mCameraSpeed;
			mCameraX -= cx / cl;
			mCameraY -= cy / cl;
		}
	}

	
	if(mEventSkip>0){
		mEventSkip++;
		if(mEventSkip<0x7f000000){
			if(mEventSkip<40){
				mSkipWindow.QuickMove(320,490-mEventSkip);
			}else if(mEventSkip>180){
				mSkipWindow.QuickMove(320,480-180-30+mEventSkip);
			}
			if(pInput->GetKey(5)==1 && !(mSelectWindow.GetLive())){
				MenuData.SetMenu(EventSkip,0);
			}
		}else{
			PlayBGM(0);
		}
		if(mEventSkip>=0x80000000+ESKIP_BLACK_OUT){
			mMapNameTimer=1000;
			CEvent *eve=mEvent.mNext;
			if(!(mSelectWindow.GetLive())){
				while(eve){
					eve->mWaitTime=0;
					eve=eve->mNext;
				}
				for(int i=0;i<OBJECT_NO;i++){
					if(mObject[i])	mObject[i]->GoToGoal();
				}
			}
		}
	}

	RunAll();
}
void CMapMode::OnDraw(void)
{
#ifdef _HAWK_EYE_EVERYTIME_
	MAP_CHIP_SIZE=32;
#endif
#ifdef _OUTPUT_MAP_BMP_
	MAP_CHIP_SIZE=_OUTPUT_MAP_BMP_SIZE_;
#endif
//	int x,y,z;
	int camerax,cameray;
	camerax=(mObject[0]->GetX()-mObject[0]->GetY())+32;
	cameray=(mObject[0]->GetX()+mObject[0]->GetY())/2-mObject[0]->GetZ();
	camerax=int(camerax*MAP_CHIP_SIZE/32-320);
	cameray=int(cameray*MAP_CHIP_SIZE/32-240);

#ifndef _OUTPUT_MAP_BMP_
	camerax+=int(mCameraX);
	cameray+=int(mCameraY);
#endif
	
	DrawMap(camerax,cameray);

	mMiniMessage.OnDraw(camerax,cameray);
	
#ifndef _OUTPUT_MAP_BMP_
	if(mShowImage){
#ifndef _HIDDEN_MESSAGE_
		for(int i=0;i<MAP_MESSAGE_NO;i++){
			mMessage[i].OnDraw(camerax,cameray);
		}
#endif
		DTest.DrawTexture(mImage,0,0,0,320,240,0,0,640,480);
		DTest.DrawBox(0,0,640,480,mMapColor);
	}else{
		DTest.DrawBox(0,0,640,480,mMapColor);
#ifndef _HIDDEN_MESSAGE_
		for(int i=0;i<MAP_MESSAGE_NO;i++){
			mMessage[i].OnDraw(camerax,cameray);
		}
#endif
	}


	if(mEventSkip>0){
		const int H=24;
		DTest.DrawBox(0,0,640,H,0xff000000);
		DTest.DrawBox(0,480-H,640,480,0xff000000);
	}
#endif


	if(mWaitWindow.GetLive()){
		mWaitWindow.OnDraw();
		mWaitWindow.DrawText(0,"Next",40,-8);

		int texturex;
		switch((mTime/CURSOR_ANI_TIME)%4){
			case 1:		texturex=1;	break;
			case 3:		texturex=2;	break;
			default:	texturex=0;
		}
		mWaitWindow.DrawTexture(&SystemTex,0,64+texturex*32,32,32,16,80,-8,32,16);
	}


	if(mMapNameTimer>4){
		DrawMapTitle();
	}

	if(mMonthTitleTimer>0){
		DrawMonthTitle();
	}

	if(mEventSkip>=0x80000000){
		int t=mEventSkip&0x00ffffff;
		if(t<ESKIP_BLACK_OUT){
			DTest.DrawBox(0,0,640,480,0x08000000*t);
		}else{
			DTest.DrawBox(0,0,640,480,0xff000000);
		}
	}

	mSelectWindow.OnDraw(camerax,cameray);
#ifdef _OUTPUT_MAP_BMP_
	for(int i=0;i<OBJECT_NO;i++){
		if(mObject[i])	mObject[i]->DrawDebugMapBMP(camerax,cameray);
	}
#else
#ifdef _DEBUG
	DrawDebug();
#endif
#endif
}
#ifdef _DEBUG
void CMapMode::DrawDebug()
{
	#ifdef _SHOW_MAP_NO_
		DTest.DrawBox(0,460,120,480,0x80ffffff);
		DTest.Printf(0,0,460,"MapNo=%d",mMapNo);
	#endif
	#ifdef _SHOW_PLAYER_POS_
		DTest.DrawBox(120,460,240,480,0x80ffffff);
		DTest.Printf(0,120,460,"X=%d Y=%d Z=%d",mObject[0]->GetX_32(),mObject[0]->GetY_32(),mObject[0]->GetZ_32());
	#endif

	#ifdef _SHOW_FLAG_
		for(int i=0;i<=_SHOW_FLAG_MAX_-_SHOW_FLAG_MIN_;i++){
			DTest.DrawBox(0,i*15,100,(i+1)*15,0xffffffff);
			DTest.Printf(1,0,i*15,"Flag:%d = %d",i+_SHOW_FLAG_MIN_ ,Flag.mFlag[i+_SHOW_FLAG_MIN_]);
		}
	#endif
	#ifdef _SHOW_ENEMY_
		for(int i=0;i<MAX_ENEMY_TYPE;i++){
			const int H = 18;
			int enemy=Flag.mEnemyType[i];
			if(enemy<0)	break;
			DTest.DrawBox(480,i*H,640,(i+1)*H,0x80ffffff);
			DTest.Printf(0,485,i*H,EnemyBaseData[enemy].mName);
		}
	#endif
	#ifdef _CHECK_MAPCHIP_
		for(int i=0;i<MAP_CHIP_NO-1;i++){
			DTest.DrawTexture(
				mMapChipImg[i],0,0,0,256,256,(i%8)*64,(i/8)*64,64,64);
		}
	#endif
		
	#ifdef _DRAW_DATA_
		#ifdef _DRAW_DATA_MIN_
			for(int i=_DRAW_DATA_MIN_;i<=_DRAW_DATA_MAX_;i++){
				int j=i-_DRAW_DATA_MIN_;
				int x=(j/8)*64;
				int y=(j%8)*20;
				DTest.DrawBox(x,y,x+64,y+20,0x80ffffff);
				DTest.Printf(0,x,y,"%d:%d",i,_DRAW_DATA_NAME_[i]);
			}
		#else
			DTest.DrawBox(0,0,64,20,0x80ffffff);
			DTest.Printf(0,0,0,"param:%d",_DRAW_DATA_NAME_);
		#endif
	#endif
}
#endif
void CMapMode::DrawMapTitle()
{
	int timer=mMapNameTimer-4;
	static char name[128];
	sprintf(name,"%s",Flag.mMapName);

	const int		x			=0;
	const int		y			=20;
	const int		textmax		=24;
	const int		boxtop		=20;
	const int		boxbottom	=28;
	const int		boxtime		=8;
	const D3DCOLOR	boxcolor	=0x8000ff80;

	const int		texttime	=8;
	const int		showtime	=128;
	const int		deletetime	=8;

	if(timer<=boxtime){
		DTest.DrawBox(x,y+boxtop,x+mMapNameSize+40,y+boxbottom,((boxcolor&0xff000000)/boxtime*timer)&0xff000000 | (boxcolor&0x00ffffff));
	}else if(timer<boxtime+texttime){
		DTest.DrawBox(x,y+boxtop,x+(mMapNameSize+40),y+boxbottom,boxcolor);
		D3DCOLOR color=(0xff000000/texttime*(timer-boxtime))&0xff000000;
		DTest.DrawText(4,name,x+1,y+1,color);
		DTest.DrawText(4,name,x,y,color | 0x00ffffff);
	}else if(timer<boxtime+texttime+showtime){
		DTest.DrawBox(x,y+boxtop,x+(mMapNameSize+40),y+boxbottom,boxcolor);
		DTest.DrawText(4,name,x+1,y+1,0xff000000);
		DTest.DrawText(4,name,x,y,0xffffffff);
	}else if(timer<boxtime+texttime+showtime+deletetime){
		int t=boxtime+texttime+showtime+deletetime-timer;
		DTest.DrawBox(x,y+boxtop,x+(mMapNameSize+40),y+boxbottom,((boxcolor&0xff000000)/deletetime*t)&0xff000000 | (boxcolor&0x00ffffff));
		D3DCOLOR color=(0xff000000/deletetime*t)&0xff000000;
		DTest.DrawText(4,name,x+1,y+1,color);
		DTest.DrawText(4,name,x,y,color|0x00ffffff);
	}
}
void CMapMode::DrawMonthTitle()
{
	if(mMonthTitleTimer<300){
		D3DCOLOR numalpha=0;
		D3DCOLOR engalpha=0;
		D3DCOLOR titlealpha=0;
		if(mMonthTitleTimer<32){
			numalpha=mMonthTitleTimer*0x08000000;
		}else{
			numalpha=0xff000000;
		}

		if(mMonthTitleTimer<16){
		}else if(mMonthTitleTimer<48){
			engalpha=(mMonthTitleTimer-16)*0x08000000;
		}else{
			engalpha=0xff000000;
		}

		if(mMonthTitleTimer<96){
		}else if(mMonthTitleTimer<128){
			titlealpha=(mMonthTitleTimer-96)*0x08000000;
		}else{
			titlealpha=0xff000000;
		}

		if(mMonthTitleTimer>300-32){
			numalpha=(300-mMonthTitleTimer)*0x08000000;
			engalpha=(300-mMonthTitleTimer)*0x08000000;
			titlealpha=(300-mMonthTitleTimer)*0x08000000;
		}

		const CMonthData &mdata=MonthData[Flag.mCurrentMonth];
		char text[64];
		sprintf(text,"%d",mdata.mMonth);
		numalpha=(numalpha/2)&0xff000000;
		DTest.DrawText(11,text,30,30,numalpha | 0xffffff);

		DTest.DrawText(9,MonthEnglish[mdata.mMonth-1],100,110,engalpha|0x404040);
		DTest.DrawText(9,MonthEnglish[mdata.mMonth-1],98,108,engalpha|0xffffff);
		
		DTest.DrawText(12,mdata.mTitle,3,120,titlealpha|0x404040,DT_RIGHT | DT_VCENTER ,600,320);
		DTest.DrawText(12,mdata.mTitle,0,117,titlealpha|0xffffff,DT_RIGHT | DT_VCENTER ,600,320);
	}
}
int CMapMode::SendMessage(int event,int parama,int paramb)
{
	CEventHeader *eh=mEventHeader.mNext;
	while(eh){
		if(	(eh->mEvent==event)
		&&	(eh->mParamA<0 || parama<0 || eh->mParamA==parama)
		&&	(eh->mParamB<0 || paramb<0 || eh->mParamB==paramb)){
			CEvent* eve=NewLine(event,parama,paramb,eh->mCurrent);
			if(eve){
				return eve->mID;
			}
			return -1;
		}
		eh=eh->mNext;
	}
	return -1;
}
void CMapMode::DoEventSkip(void)
{
	if(mEventSkip>0){
		if(mEventSkip<0x7f000000){
			mEventSkip=0x80000000;
		}
	}
}
CEvent* CMapMode::NewLine(int event,int parama,int paramb,CEventData* current,bool check)
{
	if(!current)	current=mEventData.mNext;
	CEvent *nexeve,*eve;
	eve=mEvent.mNext;
	while(eve){
		if(	check && eve->mEvent==event
		&&	(eve->mParamA<0 || eve->mParamA==parama)
		&&	(eve->mParamB<0 || eve->mParamB==paramb)
		&&	eve->mStartTime==mTime){
			return NULL;
		}
		if(eve->mKill)	break;
		eve=eve->mNext;
	}
	if(eve){
		eve->mKill=false;
	}else{
		nexeve = mEvent.mNext;
		eve = new CEvent;
		mEvent.mNext=eve;
		eve->mNext=nexeve;
	}
	eve->mCurrent	= current;
	eve->mEvent		= event;
	eve->mParamA	= parama;
	eve->mParamB	= paramb;
	eve->mID		= mEventID;
	eve->mStartTime	= mTime;
	eve->mWaitTime  = 0;
	mEventID++;

	return eve;
}

void CMapMode::SetDamage(CMapObject* object,int dmg)
{
	if(!Flag.SearchFSkill(287)){
		mObject[0]->SetEscape(object);
	}

	if(Flag.SearchFSkill(286)){
		dmg/=2;
	}

	for(int i=0;i<MAX_PARTY_NUMBER;i++){
		Flag.mPlayerData[Flag.mParty[i]].LostLP(dmg);
	}

	char text[64];
	sprintf(text,"LP -%d",dmg);
	mMiniMessage.SetMessage(
		mObject[0]->GetX(),
		mObject[0]->GetY(),
		mObject[0]->GetZ(),
		text,0xffff0000);
	PlayWave(SE_HIT);
}
int  CMapMode::GetChip(int x,int y,int z)
{
	if(x<0 || MAP_SIZE_X<=x)	return 0;
	if(y<0 || MAP_SIZE_Y<=y)	return 0;
	if(z<0 || MAP_SIZE_Z<=z)	return 0;
	return mMapData[x][y][z];
}

int  CMapMode::GetChipType(int x,int y,int z)
{
	if(x<0 || MAP_SIZE_X<=x)	return 0;
	if(y<0 || MAP_SIZE_Y<=y)	return 0;
	if(z<0 || MAP_SIZE_Z<=z)	return 0;

	int md=mMapData[x][y][z]/20;
	if(md<0||MAP_CHIP_NO<md)md=0;
	return MapChipParam[md];
}
bool CMapMode::BGMWait()
{
	if(mBGMWait>0){
		mBGMWait--;
		if(mBGMWait==0){
			PlayBGM(Flag.mMapBGMNo);
			return true;
		}
	}
	return false;
}
CMapObject*	CMapMode::CreateObject(int objno,int x,int y,int z,int dir,int speed,int movefreq,
									int img,bool hit,int gravity,int showno)
{
	delete mObject[objno];
	mObject[objno]=new CMapObject;
	mObject[objno]->InitObject(objno,x,y,z,dir,speed,movefreq,
								img,hit,gravity,showno);
	return mObject[objno];
}
CMapObject*	CMapMode::CreatePlayerObject(int objno,int x,int y,int z,int dir)
{
	delete mObject[objno];
	int img=Flag.mParty[0];

#ifdef _PLAYER_IMAGE_
	img=_PLAYER_IMAGE_;
#endif
	CPlayerObject *pobj=new CPlayerObject;

#ifdef _PLAYER_SPEED_
	pobj->InitObject(objno,x,y,z,dir,_PLAYER_SPEED_,0,img,true,GRAV_JUMP,1);
	pobj->SetBaseSpeed(_PLAYER_SPEED_);
#else
	pobj->InitObject(objno,x,y,z,dir,16,0,img,true,GRAV_JUMP,1);
	pobj->SetBaseSpeed(16);
#endif

	mObject[objno]=pobj;
	return mObject[objno];
}
CMapObject* CMapMode::CreateHighObject(int objno,int x,int y,int z,int dir,int speed,int movefreq,
									int img,bool hit,int gravity,int showno)
{
	delete mObject[objno];
	mObject[objno]=new CHighObject;
	mObject[objno]->InitObject(objno,x,y,z,dir,speed,movefreq,
								img,hit,gravity,showno);
	return mObject[objno];
}
CMapObject* CMapMode::CreateWoodObject(int objno,int x,int y,int z,int dir,int speed,int movefreq,
									int img,bool hit,int gravity,int showno)
{
	showno=showno | OBJOPT_NO_SHADOW;

	delete mObject[objno];
	mObject[objno]=new CWoodObject;
	mObject[objno]->InitObject(objno,x,y,z,dir,speed,movefreq,
								img,hit,gravity,showno);
	return mObject[objno];
}
CMapObject* CMapMode::CreateHighAniObject(int objno,int x,int y,int z,int dir,int speed,int movefreq,
									int img,bool hit,int gravity,int showno)
{
	delete mObject[objno];
	mObject[objno]=new CHighAniObject;
	mObject[objno]->InitObject(objno,x,y,z,dir,speed,movefreq,
								img,hit,gravity,showno);
	return mObject[objno];
}
CMapObject* CMapMode::CreateAniObject(int objno,int x,int y,int z,int dir,int speed,int movefreq,
									int img,bool hit,int gravity,int showno)
{
	delete mObject[objno];
	mObject[objno]=new CAniObject;
	mObject[objno]->InitObject(objno,x,y,z,dir,speed,movefreq,
								img,hit,gravity,showno);
	return mObject[objno];
}
CMapObject* CMapMode::CreateAAObject(int objno,int x,int y,int z,int dir,int speed,int movefreq,
									int img,int koma,int anispd,bool hit,int gravity,int showno)
{
	delete mObject[objno];
	CAutoAniObject* obj=new CAutoAniObject;
	obj->InitAutoAniObject(objno,x,y,z,dir,speed,movefreq,
								img,koma,anispd,hit,gravity,showno);
	mObject[objno]=obj;

	return mObject[objno];
}
CMapObject* CMapMode::CreateBigObject(int objno,int x,int y,int z,int dir,int speed,int movefreq,
									int img,bool hit,int gravity,int showno)
{
	delete mObject[objno];
	mObject[objno]=new CBigObject;
	mObject[objno]->InitObject(objno,x,y,z,dir,speed,movefreq,
								img,hit,gravity,showno);
	return mObject[objno];
}
CMapObject* CMapMode::CreateBigAniObject(int objno,int x,int y,int z,int dir,int speed,int movefreq,
									int img,bool hit,int gravity,int showno)
{
	delete mObject[objno];
	mObject[objno]=new CBigAniObject;
	mObject[objno]->InitObject(objno,x,y,z,dir,speed,movefreq,
								img,hit,gravity,showno);
	return mObject[objno];
}
CMapObject*	CMapMode::CreatePutObject(int objno,int x,int y,int z,int speed,int img,int showno)
{
	showno=showno | OBJOPT_BLOCK_SHADOW;

	delete mObject[objno];
	mObject[objno]=new CPutObject;
	mObject[objno]->InitObject(objno,x,y,z,0,speed,0,
								img,true,GRAV_NO_JUMP,showno);
	return mObject[objno];
}
CMapObject*	CMapMode::CreateLiftObject(int objno,int x,int y,int z,int dir,int speed,int movefreq,int img,int showno)
{
	delete mObject[objno];
	mObject[objno]=new CLiftObject;
	mObject[objno]->InitObject(objno,x,y,z,dir,speed,movefreq,
								img,true,GRAV_LOCK,showno);
	return mObject[objno];
}
CMapObject*	CMapMode::CreateNTObject(int objno,int x,int y,int z,int img,int showno,
									int cycle,int timer,int pow,int se)
{
	delete mObject[objno];
	CNormalTrapObject* obj=new CNormalTrapObject;
	obj->InitNTObject(objno,x,y,z,img,showno,
						cycle,timer,pow,se);
	mObject[objno]=obj;
	return mObject[objno];
}
CMapObject*	CMapMode::CreateStoneObject(int objno,int x,int y,int z,int dir)
{
	delete mObject[objno];
	CStoneObject* obj=new CStoneObject;
	obj->InitStoneObject(objno,x,y,z,dir);
	mObject[objno]=obj;
	return mObject[objno];
}


CMapObject*	CMapMode::CreateEnemyObject(int objno,int x,int y,int z,int dir,int speed,int movefreq,
										int img,bool hit,int gravity,int showno,
										int movemode,int search,int reader,int lv,int member,int field,int option)
{
	delete mObject[objno];
	CEnemyObject* obj=new CEnemyObject;
	if(Flag.SearchFSkill(275)){
		speed=speed*3/2;
	}
	obj->InitEnemyObject(objno,x,y,z,dir,speed,movefreq,
						img,hit,gravity,showno,
						movemode,search,reader,lv,member,field,option);
	mObject[objno]=obj;
	return mObject[objno];
}
CMapObject*	CMapMode::CreateEnemyObject(int objno,int x,int y,int z,int dir,int speed,int movefreq,
										int img,bool hit,int gravity,int showno,
										int movemode,int search,int party)
{
	delete mObject[objno];
	CEnemyObject* obj=new CEnemyObject;
	if(Flag.SearchFSkill(275)){
		speed=speed*3/2;
	}
	obj->InitEnemyObject(objno,x,y,z,dir,speed,movefreq,
						img,hit,gravity,showno,
						movemode,search,party);
	mObject[objno]=obj;
	return mObject[objno];
}
CMapObject*	CMapMode::CreateBoxObject(int objno,int x,int y,int z,int dir,int box)
{
	delete mObject[objno];
	CBoxObject* obj=new CBoxObject;
	obj->InitBoxObject(objno,x,y,z,dir,box);
	mObject[objno]=obj;
	return mObject[objno];
}
CMapObject*	CMapMode::CreateSavePoint(int objno,int x,int y,int z)
{
	delete mObject[objno];
	CSavePoint* obj=new CSavePoint;
	obj->InitSavePoint(objno,x,y,z);
	mObject[objno]=obj;
	return mObject[objno];
}

bool CMapMode::DeleteObject(int no)
{
	if(no<0)			return false;
	if(no>=OBJECT_NO)	return false;
	if(mObject[no]){
		delete mObject[no];
		mObject[no]=NULL;
		return true;
	}
	return false;
}
void	CMapMode::SetCamera(int x,int y,bool move)
{
	mCameraGoalX=x;
	mCameraGoalY=y;
	if(move){
		mCameraX=x;
		mCameraY=y;
	}
}
void	CMapMode::WaitBreak(bool force)
{
	bool del=false;
	CEvent *eve=mEvent.mNext;
	while(eve){
		if(eve->mWaitTime<-20 || (force && (eve->mWaitTime<0))){
			del=true;
			eve->mWaitTime=0;
		}
		eve=eve->mNext;
	}
	if(del)	mWaitWindow.Destroy();
}

CEventData* CMapMode::GetEventData(int line)
{
	CEventData *edata=mEventData.mNext;
	for(int i=0;i<line;i++){
		if(!edata)	return NULL;
		edata=edata->mNext;
	}
	return edata;
}

void CMapMode::KillEvent(CEvent *event,CEvent *old)
{
	event->mKill=true;

//	old->mNext=event->mNext;
//	delete event;
}
void CMapMode::KillEvent(int id)
{
	CEvent *eve=&mEvent;
	if(!eve->mNext)	return;

	while(eve->mNext){
		if(eve->mNext->mID==id){
			KillEvent(eve->mNext,eve);
			return;
		}
		eve=eve->mNext;
	}
}
void CMapMode::GoToLine(int line,CEvent *event)
{
	if(line<0){
		event->mCurrent=NULL;
		return;
	}

	int i;
	for(i=0;i<MAX_GOTO;i++){
		if(mGotoLine[i]<0)	break;
		if(mGotoLine[i]==line){
			event->mCurrent=mGotoData[i];
			return;
		}
	}

	CEventData* edata=mEventData.mNext;
	int repeat=line;
	if(line > event->mCurrent->mLine){
		edata=event->mCurrent;
		repeat-=event->mCurrent->mLine;
	}
	for(i=0;i<repeat;i++){
		if(!edata)	break;
		edata=edata->mNext;
	}
	event->mCurrent=edata;
	mGotoLine[mGotoCurrent]=line;
	mGotoData[mGotoCurrent]=edata;
	mGotoCurrent=(mGotoCurrent+1)%MAX_GOTO;
}

void CMapMode::RunAll(void)
{
	do{
		CEvent *eve=&mEvent;
		if(!eve->mNext)	break;

		bool escape=mSelectWindow.GetLive();
		while(eve->mNext){
			int run=0;
			if(!(eve->mNext->mKill)){
				run=Run(eve->mNext,eve);
			}
//			if(run&1){
				eve=eve->mNext;
//			}
			if(run&2){
				escape=true;
			}
			mRuningEvent=NULL;
		}
		if(escape)	break;
	}while(mEventSkip>=0x80000000+ESKIP_BLACK_OUT);
}
int CMapMode::Run(CEvent *event,CEvent *old)
{
	mRuningEvent=event;

	if(event->mWaitTime!=0){
		if(mEventSkip<0x80000000+ESKIP_BLACK_OUT || mSelectWindow.GetLive()){
			event->mWaitTime--;
			if((event->mWaitTime<-WINDOW_OPEN_TIME) && (mWaitWindow.GetDestroy())){
				mWaitWindow.InitPlaneWindow(-50,440,120,32);
			}
			return 1;
		}
	}

	while(1){
		if(!event->mCurrent){
			KillEvent(event,old);
			return 2;
		}
		CNumber::mCurrent=event->mCurrent->mLine;
		switch(event->mCurrent->mEvent){
			case EVE_END_EVENT:
				KillEvent(event,old);		return 2;
			case EVE_SET_MESSAGE:
				SetMessage(event);			break;
			case EVE_DELETE_MESSAGE:
				DeleteMessage(event);		break;
			case EVE_STOP:
				Stop(event);				break;
			case EVE_WAIT:
				if( Wait(event)){
											return 1;
				}
				break;
			case EVE_ENCOUNT:
				Encount(event);				return 3;
			case EVE_IF:
				If(event);					continue;
			case EVE_CHECK_BATTLE:
				CheckBattle(event);
				KillEvent(event,old);		return 2;
			case EVE_SET_OBJECT:
				SetObject(event,0);			break;
			case EVE_SET_ENEMY:
				SetEnemy(event);			break;
			case EVE_DELETE_OBJECT:
				DeleteObject(event);		break;
			case EVE_CHANGE_MAP:
				ChangeMap(event);
				KillEvent(event,old);		return 2;
			case EVE_SET_BOX:
				SetBox(event);				break;
			case EVE_OPEN_BOX:
				OpenBox(event);
				KillEvent(event,old);		return 2;
			case EVE_IF_BOX:
				IfBox(event);				continue;
			case EVE_STORE:
				Store(event);				break;
			case EVE_SET_ANI_OBJECT:
				SetObject(event,2);			break;
			case EVE_SET_HIGH_OBJECT:
				SetObject(event,1);			break;
			case EVE_SET_WOOD_OBJECT:
				SetObject(event,5);			break;
			case EVE_SET_OBJ_POS:
				SetObjPos(event);			break;
			case EVE_SET_OBJ_GOAL:
				SetObjGoal(event);			break;
			case EVE_START_EVENT_SKIP:
				StartEventSkip(event);		break;
			case EVE_END_EVENT_SKIP:
				EndEventSkip();				break;
			case EVE_SET_CAMERA:
				SetCamera(event);			break;
			case EVE_SET_OBJ_DIR:
				SetObjDir(event);			break;
			case EVE_GO_TO:
				Goto(event);				continue;
			case EVE_NEW_LINE:
				NewLine(event);				break;
			case EVE_MAP_TITLE:
				if(mMapNameTimer<4){
					mMapNameTimer=4;
				}
				break;
			case EVE_SET_OBJ_IMG:
				SetObjImg(event);			break;
			case EVE_SET_OBJ_ZSPEED:
				SetObjZSpeed(event);		break;
			case EVE_PLAY_WAVE:
				PlayWaveEvent(event);		break;
			case EVE_PLAY_BGM:
				PlayBGMEvent(event);		break;
			case EVE_SET_BIG_OBJECT:
				SetObject(event,3);			break;
			case EVE_SET_OBJ_VIEW:
				SetObjView(event);			break;
			case EVE_SET_CAMERA_SPEED:
				SetCameraSpeed(event);		break;
			case EVE_BATTLE:
				Battle(event);				return 3;
			case EVE_GET_ITEM:
				GetItem(event);				break;
			case EVE_IF_BOX_3D:
				IfBox3D(event);				continue;
			case EVE_SET_AUTO_ANI_OBJECT:
				SetAAObject(event);			break;				
			case EVE_SET_OBJ_GOAL_DIR:
				SetObjGoalDir(event);		break;
			case EVE_RETURN_VILLAGE:
				ReturnVillageEvent(event);	break;
			case EVE_SET_HIGH_ANI_OBJECT:
				SetObject(event,4);			break;
			case EVE_NEXT_MONTH:
				if(NextMonth(event)){
					KillEvent(event,old);
					return 2;
				}
				break;
			case EVE_KILL:
				if(Kill(event)){
					KillEvent(event,old);
					return 2;
				}
				break;
			case EVE_MAP_COLOR:
				MapColor(event);			break;
			case EVE_OPEN_MENU:
				OpenMenu(event);			break;
			case EVE_SET_FRIEND:
				SetFriend(event);			break;
			case EVE_SET_PARTY:
				SetParty(event);			break;
			case EVE_SHOP:
				Shop(event);				break;
			case EVE_SELECT_2:
				Select(event,2);			return 3;
			case EVE_SET_PUT_OBJECT:
				SetPutObject(event);		break;
			case EVE_SPEND_ITEM:
				SpendItem(event);			break;
			case EVE_SELECT_4:
				Select(event,4);			return 3;
			case EVE_SELL:
				Sell(event);				break;
			case EVE_SELECT_3:
				Select(event,3);			return 3;
			case EVE_FLEA_MARKET:
				FleaMarket(event);			break;
			case EVE_FARM:
				Farm(event);				break;
			case EVE_GET_MAP_CHIP:
				GetMapChip(event);			break;
			case EVE_SET_MAP_CHIP:
				SetMapChip(event);			break;
			case EVE_BG_COLOR:
				BGColor(event);				break;
			case EVE_ADD_ENEMY_LEVEL:
				AddEnemyLevel(event);		break;
			case EVE_SET_LIFT_OBJECT:
				SetLiftObject(event);		break;
			case EVE_SET_OBJ_GRAV:
				SetObjGrav(event);			break;
			case EVE_SET_OBJ_SPEED:
				SetObjSpeed(event);			break;
			case EVE_SET_NORMAL_TRAP:
				SetNormalTrap(event);		break;
			case EVE_SET_SAVE_POINT:
				SetSavePoint(event);		break;
			case EVE_HEAL_ALL:
				HealAll(event);				break;
			case EVE_SEED:
				Seed(event);				break;
			case EVE_SET_ENEMY_LOCKED:
				SetEnemyLocked(event);		break;
			case EVE_LEAVE_PARTY:
				LeaveParty(event);			break;
			case EVE_KNOCK_BACK:
				KnockBack(event);			break;
			case EVE_SHOP_MAN:
				ShopMan(event);				break;
			case EVE_CHANGE_ALL_CHIP:
				ChangeAllChip(event);		break;
			case EVE_SET_BIG_ANI_OBJECT:
				SetObject(event,6);			break;
			case EVE_SET_OBJ_PARAM:
				SetObjParam(event);			break;
			case EVE_ENDING:
				Ending(event);				break;
			case EVE_DEL_MES_QUICK:
				DeleteMessageQuick(event);	break;
			case EVE_SET_EGG_FRIEND:
				SetEggFriend(event);		break;
			case EVE_ADD_MARKET:
				AddMarket(event);			break;
			case EVE_SET_OBJ_COLOR:
				SetObjColor(event);			break;
			case EVE_TUTORIAL:
				Tutorial(event);			break;
			case EVE_CHANGE_MAP_TITLE:
				ChangeMapTitle(event);		break;
			case EVE_SHOW_IMAGE:
				ShowImage(event);			break;
			case EVE_TRUE_ENDING:
				TrueEnd(event);				break;
			case EVE_MONTH_TITLE:
				MonthTitle(event);			break;
			case EVE_SET_FADE_OUT:
				SetFadeOutEvent(event);		break;
			case EVE_SET_FADE:
				SetFadeEvent(event);		break;
		}
		event->mCurrent = event->mCurrent->mNext;
	}
	return 2;
}
