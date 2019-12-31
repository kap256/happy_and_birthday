#include "mapmode.h"
#include "number.h"
#include "changemode.h"
#include "menudata.h"
#include "month.h"
#include "option.h"
#include "ending.h"

void CMapMode::SetMessage(CEvent *event)
{
	int no,ori,para;
	no	=event->mCurrent->mData[0].GetNumber();
	ori	=event->mCurrent->mData[4].GetNumber();
	para=event->mCurrent->mData[5].GetNumber();

	if(no<0 || MAP_MESSAGE_NO<no)	return;
	mMessage[no].InitMapMessage(event->mCurrent->mText,
								event->mCurrent->mData[1],
								event->mCurrent->mData[2],
								event->mCurrent->mData[3],
								ori,para);

	if(!GetEventSkip() && !(para & SMES_NO_SOUND)){
		PlayWave(SE_MESSAGE_START);
	}
}
void CMapMode::DeleteMessage(CEvent *event)
{
	int no;
	no	=event->mCurrent->mData[0].GetNumber();

	if(no<0 || MAP_MESSAGE_NO<no)	return;
	if(GetEventSkip()){
		mMessage[no].QuickDestroy();
	}else{
        mMessage[no].Destroy();
	}
	//PlayWave(SE_MESSAGE_END);
}
void CMapMode::DeleteMessageQuick(CEvent *event)
{
	int no;
	no	=event->mCurrent->mData[0].GetNumber();

	if(no<0 || MAP_MESSAGE_NO<no)	return;
	mMessage[no].QuickDestroy();
}
bool CMapMode::Wait(CEvent *event)
{
	int time;
	if(mEventSkip>=0x80000000+ESKIP_BLACK_OUT){
		time=0;
	}
	time	=event->mCurrent->mData[0].GetNumber();
	if(time<=0)		time=-1;
	event->mWaitTime= time;

	event->mCurrent = event->mCurrent->mNext;

	return true;
}
void CMapMode::Stop(CEvent *event)
{
	int stop=event->mCurrent->mData[0].GetNumber();

	mStop=stop;
	if(mStop==0){
		for(int i=0;i<OBJECT_NO;i++){
			if(mObject[i]){
				mObject[i]->SetStop(false);
			}
		}
	}else if(mStop==1){
		mObject[0]->SetPos((mObject[0]->GetX()+16)/32,(mObject[0]->GetY()+16)/32,mObject[0]->GetZ_32());
	}

}
void CMapMode::PlayWaveEvent(CEvent *event)
{
	if(mEventSkip>=0x80000000+ESKIP_BLACK_OUT)	return;

	int wave=event->mCurrent->mData[0].GetNumber();

	PlayWave(wave);
}
void CMapMode::PlayBGMEvent(CEvent *event)
{
	int bgm=event->mCurrent->mData[0].GetNumber();
	bgm=cPlayBGMMap[bgm];
	if(bgm<0)	return;
	
	if(bgm<BGM_MAX){
		GlobalFlag.mSound[bgm]=true;
	}

	if(Flag.mMapBGMNo==bgm)	return;
	Flag.mMapBGMNo=bgm;
	if(mEventSkip>=0x80000000+ESKIP_BLACK_OUT)	return;

	SetBGMFadeOut(10);
	mBGMWait=10;
}
void CMapMode::SetFadeOutEvent(CEvent *event)
{
	int time=event->mCurrent->mData[0].GetNumber();
	Flag.mMapBGMNo=0;

	if(mEventSkip>=0x80000000+ESKIP_BLACK_OUT)	return;
	SetBGMFadeOut(time);
}
void CMapMode::SetFadeEvent(CEvent *event)
{
	int time=event->mCurrent->mData[0].GetNumber();
	int vol=event->mCurrent->mData[1].GetNumber();
	if(mEventSkip>=0x80000000+ESKIP_BLACK_OUT)	return;

	SetBGMFade(time,vol);
}
void CMapMode::SetCamera(CEvent *event)
{
	int	 x		=event->mCurrent->mData[0].GetNumber();
	int	 y		=event->mCurrent->mData[1].GetNumber();
	bool move	=(event->mCurrent->mData[2].GetNumber()!=0);
	if(mEventSkip>=0x80000000+ESKIP_BLACK_OUT)	move=true;

	SetCamera(x,y,move);
}
void CMapMode::SetCameraSpeed(CEvent *event)
{
	int	 spd	=event->mCurrent->mData[0].GetNumber();

	mCameraSpeed = spd/10.0;
}
void CMapMode::GetMapChip(CEvent *event)
{
	int i;
	int *to=event->mCurrent->mData[0].GetSpecialNumber(&i);
	if(to!=&i){
		int x=event->mCurrent->mData[1].GetNumber();
		int y=event->mCurrent->mData[2].GetNumber();
		int z=event->mCurrent->mData[3].GetNumber();

		*to=mMapData[x][y][z];
	}
}
void CMapMode::SetMapChip(CEvent *event)
{
	int chip=event->mCurrent->mData[0].GetNumber();
	int x=event->mCurrent->mData[1].GetNumber();
	int y=event->mCurrent->mData[2].GetNumber();
	int z=event->mCurrent->mData[3].GetNumber();

	if(x< 0)			return;
	if(x>=MAP_SIZE_X)	return;
	if(y< 0)			return;
	if(y>=MAP_SIZE_Y)	return;
	if(z< 0)			return;
	if(z>=MAP_SIZE_Z)	return;

	mMapData[x][y][z]=chip;
}
void CMapMode::ChangeAllChip(CEvent *event)
{
	int before=event->mCurrent->mData[0].GetNumber();
	int after=event->mCurrent->mData[1].GetNumber();

	for(int x=0;x<MAP_SIZE_X-1;x++){
		for(int y=0;y<MAP_SIZE_Y-1;y++){
			for(int z=1;z<MAP_SIZE_Z-1;z++){
				if(mMapData[x][y][z]==before){
					mMapData[x][y][z]=after;
				}
			}
		}
	}
}	
void CMapMode::MonthTitle(CEvent *event)
{
	mMonthTitleTimer=1;
}	
void CMapMode::ChangeMapTitle(CEvent *event)
{
	strncpy(Flag.mMapName,event->mCurrent->mText,sizeof(Flag.mMapName));
}		
void CMapMode::MapColor(CEvent *event)
{
	int	 a		=event->mCurrent->mData[0].GetNumber();
	int	 r		=event->mCurrent->mData[1].GetNumber();
	int	 g		=event->mCurrent->mData[2].GetNumber();
	int	 b		=event->mCurrent->mData[3].GetNumber();

	mMapColor= D3DCOLOR_ARGB(a,r,g,b);
}
void CMapMode::BGColor(CEvent *event)
{
	int	 no		=event->mCurrent->mData[0].GetNumber();

	mBGColor=no;
	if(no>=MAX_BG_COLOR){
		DTest.LoadTexture(mBGImg,"data\\battlefield\\fieldbg");
	}
}
void CMapMode::Store(CEvent *event)
{
	int i;
	int *to=event->mCurrent->mData[0].GetSpecialNumber(&i);
	int no=event->mCurrent->mData[1].GetNumber();
	if(to!=&i){
		*to=no;
	}
}
void CMapMode::Goto(CEvent *event)
{
	int line=event->mCurrent->mData[0].GetNumber();

	GoToLine(line,event);
}
void CMapMode::NewLine(CEvent *event)
{
	int a	=event->mCurrent->mData[0].GetNumber();
	int b	=event->mCurrent->mData[1].GetNumber();
	int line=event->mCurrent->mData[2].GetNumber();

	CEvent* eve=NewLine(EH_NEW_LINE,a,b,NULL,false);
	if(eve){
		GoToLine(line,eve);
		mEventReturn=eve->mID;
	}else{
		mEventReturn=-1;
	}
}
bool CMapMode::Kill(CEvent *event)
{
	int id	=event->mCurrent->mData[0].GetNumber();

	if(id == event->mID){
		return true;
	}else{
		KillEvent(id);
		return false;
	}
}
void CMapMode::If(CEvent *event)
{
	int a,b,sign,line;
	a	=event->mCurrent->mData[0].GetNumber();
	b	=event->mCurrent->mData[1].GetNumber();
	sign=event->mCurrent->mData[2].GetNumber();
	line=event->mCurrent->mData[3].GetNumber();

	switch(sign){
		case IF_EQUAL:
			if(a==b){
				GoToLine(line,event);
				return;
			}
			break;
		case IF_BIGGER:
			if(a>b){
				GoToLine(line,event);
				return;
			}
			break;
		case IF_SMALLER:
			if(a<b){
				GoToLine(line,event);
				return;
			}
			break;
		case IF_NOT_EQUAL:
			if(a!=b){
				GoToLine(line,event);
				return;
			}
			break;
	}
	event->mCurrent = event->mCurrent->mNext;
}
void CMapMode::IfBox(CEvent *event)
{
	int obj,x1,y1,x2,y2,line;
	bool in;
	obj	=event->mCurrent->mData[0].GetNumber();
	x1	=event->mCurrent->mData[1].GetNumber();
	y1	=event->mCurrent->mData[2].GetNumber();
	x2	=event->mCurrent->mData[3].GetNumber();
	y2	=event->mCurrent->mData[4].GetNumber();
	in	=(event->mCurrent->mData[5].GetNumber()!=0);
	line=event->mCurrent->mData[6].GetNumber();

	if(x1>x2){
		int x=x1;	x1=x2;	x2=x;
	}
	if(y1>y2){
		int y=y1;	y1=y2;	y2=y;
	}

	if(obj<0)			obj=0;
	if(obj>=OBJECT_NO)	obj=OBJECT_NO-1;
	if(mObject[obj]){
		int objx=mObject[obj]->GetX_32();
		int objy=mObject[obj]->GetY_32();
		if((x1<=objx && objx<=x2 && y1<=objy && objy<=y2)==in){
			GoToLine(line,event);
			return;
		}
	}

	event->mCurrent = event->mCurrent->mNext;
}
void CMapMode::IfBox3D(CEvent *event)
{
	int obj,x1,y1,z1,x2,y2,z2,line;
	bool in;
	obj	=event->mCurrent->mData[0].GetNumber();
	x1	=event->mCurrent->mData[1].GetNumber();
	y1	=event->mCurrent->mData[2].GetNumber();
	z1	=event->mCurrent->mData[3].GetNumber();
	x2	=event->mCurrent->mData[4].GetNumber();
	y2	=event->mCurrent->mData[5].GetNumber();
	z2	=event->mCurrent->mData[6].GetNumber();
	in	=(event->mCurrent->mData[7].GetNumber()!=0);
	line=event->mCurrent->mData[8].GetNumber();

	if(x1>x2){	int x=x1;	x1=x2;	x2=x;	}
	if(y1>y2){	int y=y1;	y1=y2;	y2=y;	}
	if(z1>z2){	int z=z1;	z1=z2;	z2=z;	}

	if(obj<0)			obj=0;
	if(obj>=OBJECT_NO)	obj=OBJECT_NO-1;
	if(mObject[obj]){
		int objx=mObject[obj]->GetX_32();
		int objy=mObject[obj]->GetY_32();
		int objz=mObject[obj]->GetZ_32();
		if((x1<=objx && objx<=x2 && y1<=objy && objy<=y2 && z1<=objz && objz<=z2)==in){
			GoToLine(line,event);
			return;
		}
	}

	event->mCurrent = event->mCurrent->mNext;
}
void CMapMode::Select(CEvent *event,int no)
{
	mSelectEvent=event;

	int ori;
	ori	=event->mCurrent->mData[4].GetNumber();

	mSelectDefault=event->mCurrent->mData[0].GetNumber();

	int go[MAP_SELECT_MAX];
	for(int i=0;i<no;i++){
		go[i]=event->mCurrent->mData[5+i].GetNumber();
	}
	mSelectWindow.InitSelectWindow(
								event->mCurrent->mText,
								event->mCurrent->mData[1],
								event->mCurrent->mData[2],
								event->mCurrent->mData[3],
								ori,
								no,
								go);
	PlayWave(SE_YES);
	event->mWaitTime=-1;
}

void CMapMode::StartEventSkip(CEvent *event)
{
	if(mEventSkip>=0x80000000+ESKIP_BLACK_OUT && mBGMWait>=0){
		mBGMWait=4;
	}else if(mBGMWait<0){
		mBGMWait=0;
	}
	mEventSkip=1;
}
void CMapMode::EndEventSkip(void)
{
	if(mEventSkip>=0x80000000+ESKIP_BLACK_OUT && mBGMWait>=0){
		mBGMWait=4;
	}
	mWaitWindow.QuickDestroy();
	mSkipWindow.QuickMove(320,490);
	mEventSkip=0;
}
void CMapMode::ReturnVillageEvent(CEvent *event)
{
	int param	=event->mCurrent->mData[0].GetNumber();
	if(param!=0){
		if(param & 1){
			Flag.AllHeal();
		}
		if(!(param & 2)){
			SetBGMFadeOut(10);
		}
		ChangeMapInfo.ChangeMap(-1,0,0,0,0);
		mBGMWait=-1;
		Flag.mMapBGMNo=2;
	}else{
		MenuData.SetMenu(ReturnVillage,param);
	}

	Flag.Market();

}
void CMapMode::OpenMenu(CEvent *event)
{
	int param	=event->mCurrent->mData[0].GetNumber();
	switch(param){
		case 0:
			if(Flag.mOldMonth!=Flag.mCurrentMonth){
				Flag.mOldMonth=Flag.mCurrentMonth;
				MenuData.SetMenu(MonthStartMenu,0);
			}
			break;
		case 1:
			MenuData.SetMenu(StartMenu,0);
			break;
		case 2:
			MenuData.SetMenu(ChangeMemberMenu,0);
			break;
	}

	if(mEventSkip>=0x80000000+ESKIP_BLACK_OUT){
		//PlayBGM(Flag.mMapBGMNo);
	}
}
void CMapMode::Shop(CEvent *event)
{
	int  shop	=event->mCurrent->mData[0].GetNumber();
	bool param	=(event->mCurrent->mData[1].GetNumber()!=0);
	MenuData.SetMenu(ShopMenu,shop+(param?0:1)*0x100);
}
void CMapMode::ShopMan(CEvent *event)
{
	int shop[FLEA_MARKET_ITEM_MAX];
	bool param	=(event->mCurrent->mData[0].GetNumber()!=0);
	for(int i=0;i<FLEA_MARKET_ITEM_MAX;i++){
		shop[i]=event->mCurrent->mData[i+1].GetNumber();
	}

	Flag.SetMarket(shop);
	MenuData.SetMenu(FleaMarketMenu,(param?0:1)*0x100);
}
void CMapMode::Tutorial(CEvent *event)
{
#ifndef _SHOW_TUTORIAL
	if(mEventSkip>=0x80000000+ESKIP_BLACK_OUT)	return;
#endif
	if(Flag.mHardMode>0)						return;

	int  tu	=event->mCurrent->mData[0].GetNumber();
	MenuData.SetMenu(TutorialMenu,tu);
}
void CMapMode::FleaMarket(CEvent *event)
{
	bool param	=(event->mCurrent->mData[0].GetNumber()!=0);
	MenuData.SetMenu(FleaMarketMenu,(param?0:1)*0x100);
}
void CMapMode::Farm(CEvent *event)
{
	bool param	=(event->mCurrent->mData[0].GetNumber()!=0);
	MenuData.SetMenu(FarmMenu,(param?0:1)*0x100);
}
void CMapMode::Seed(CEvent *event)
{
	bool param	=(event->mCurrent->mData[0].GetNumber()!=0);
	MenuData.SetMenu(SeedMenu,(param?0:1)*0x100);
}
void CMapMode::Sell(CEvent *event)
{
	bool param	=(event->mCurrent->mData[0].GetNumber()!=0);
	MenuData.SetMenu(SellMenu,(param?0:1)*0x100);
}
bool CMapMode::NextMonth(CEvent *event)
{
	bool ret=false;
	if(Flag.mCurrentMonth<0)	return ret;

	int month=MonthData[Flag.mCurrentMonth].mMonth+1;

	if(month>MONTH_LOCK){
		month=MONTH_LOCK;
#ifdef _TRIAL_VERSION_
		EndEventSkip();
		ChangeMapInfo.ChangeMap(-3,0,0,0,0);
		ret=true;
#endif
	}
	Flag.NewMonth(month);

	Flag.mBypathMapNo=-1;
	Flag.mCurrentMonth=-1;
	return ret;
}
void CMapMode::SetFriend(CEvent *event)
{
	int no	=event->mCurrent->mData[0].GetNumber();
	Flag.mFriend[no]=true;
	Flag.CheckReserve();
}
void CMapMode::SetEggFriend(CEvent *event)
{
	int no	=event->mCurrent->mData[0].GetNumber();
	Flag.mEggFriend[no]=true;
	Flag.CheckReserve();
}
void CMapMode::SetParty(CEvent *event)
{
	int no	=event->mCurrent->mData[0].GetNumber();
	Flag.AddMember(no);
}
void CMapMode::LeaveParty(CEvent *event)
{
	int no	=event->mCurrent->mData[0].GetNumber();
	for(int i=0;i<MAX_PARTY_NUMBER;i++){
		if(Flag.mParty[i]==no){
			Flag.ChangeMember(i,-1);
		}
	}
}
void CMapMode::ChangeMap(CEvent *event)
{
	int no	=event->mCurrent->mData[0].GetNumber();
	int x	=event->mCurrent->mData[1].GetNumber();
	int y	=event->mCurrent->mData[2].GetNumber();
	int z	=event->mCurrent->mData[3].GetNumber();
	int dir	=event->mCurrent->mData[4].GetNumber();

	ChangeMapInfo.ChangeMap(no,x,y,z,dir);
}
void CMapMode::Battle(CEvent *event)
{
	int no	=event->mCurrent->mData[0].GetNumber();

	EncountInfo.Encount(no,mEnemyLvUp);
	event->mWaitTime= 0;
	event->mCurrent = event->mCurrent->mNext;
}
void CMapMode::AddEnemyLevel(CEvent *event)
{
	int up	=event->mCurrent->mData[0].GetNumber();

	mEnemyLvUp+=up;
}
void CMapMode::GetItem(CEvent *event)
{
	int item=event->mCurrent->mData[0].GetNumber();

	Flag.DropTempItem();
	Flag.SetTempItem(item);
	MenuData.SetMenu(OverItemMenu,0);
}
void CMapMode::SpendItem(CEvent *event)
{
	int item=event->mCurrent->mData[0].GetNumber();

	bool cuse=Flag.mClosetUse;
	Flag.mClosetUse=true;
	Flag.SpendAllItem(item);
	Flag.mClosetUse=cuse;
}
void CMapMode::AddMarket(CEvent *event)
{
	int item=event->mCurrent->mData[0].GetNumber();

	if(item<0)	return;
	Flag.mMarket[item]++;
	Flag.mMarketNew[item]=true;
}
void CMapMode::HealAll(CEvent *event)
{
	Flag.AllHeal();
}
void CMapMode::SetObject(CEvent *event,int type)
{
	int no	=event->mCurrent->mData[0].GetNumber();
	int x	=event->mCurrent->mData[1].GetNumber();
	int y	=event->mCurrent->mData[2].GetNumber();
	int z	=event->mCurrent->mData[3].GetNumber();
	int dir	=event->mCurrent->mData[4].GetNumber();
	int spd	=event->mCurrent->mData[5].GetNumber();
	int freq=event->mCurrent->mData[6].GetNumber();
	int img	=event->mCurrent->mData[7].GetNumber();
	int hit	=event->mCurrent->mData[8].GetNumber();
	int grav=event->mCurrent->mData[9].GetNumber();
	int etc	=event->mCurrent->mData[10].GetNumber();

	switch(type){
		case 1:
			CreateHighObject(no,x,y,z,dir,spd,freq,img,(hit!=0),grav,etc);
			break;
		case 2:
			CreateAniObject(no,x,y,z,dir,spd,freq,img,(hit!=0),grav,etc);
			break;
		case 3:
			CreateBigObject(no,x,y,z,dir,spd,freq,img,(hit!=0),grav,etc);
			break;
		case 4:
			CreateHighAniObject(no,x,y,z,dir,spd,freq,img,(hit!=0),grav,etc);
			break;
		case 5:
			CreateWoodObject(no,x,y,z,dir,spd,freq,img,(hit!=0),grav,etc);
			break;
		case 6:
			CreateBigAniObject(no,x,y,z,dir,spd,freq,img,(hit!=0),grav,etc);
			break;
		default:
			CreateObject(no,x,y,z,dir,spd,freq,img,(hit!=0),grav,etc);
			break;
	}
}
void CMapMode::DeleteObject(CEvent *event)
{
	int no	=event->mCurrent->mData[0].GetNumber();

	DeleteObject(no);
}
void CMapMode::SetAAObject(CEvent *event)
{
	int no	=event->mCurrent->mData[0].GetNumber();
	int x	=event->mCurrent->mData[1].GetNumber();
	int y	=event->mCurrent->mData[2].GetNumber();
	int z	=event->mCurrent->mData[3].GetNumber();
	int dir	=event->mCurrent->mData[4].GetNumber();
	int spd	=event->mCurrent->mData[5].GetNumber();
	int freq=event->mCurrent->mData[6].GetNumber();
	int img	=event->mCurrent->mData[7].GetNumber();
	int koma	=event->mCurrent->mData[8].GetNumber();
	int anispd	=event->mCurrent->mData[9].GetNumber();
	int hit	=event->mCurrent->mData[10].GetNumber();
	int grav=event->mCurrent->mData[11].GetNumber();
	int etc	=event->mCurrent->mData[12].GetNumber();

	CreateAAObject(no,x,y,z,dir,spd,freq,img,koma,anispd,(hit!=0),grav,etc);
}
void CMapMode::SetLiftObject(CEvent *event)
{
	int no	=event->mCurrent->mData[0].GetNumber();
	int x	=event->mCurrent->mData[1].GetNumber();
	int y	=event->mCurrent->mData[2].GetNumber();
	int z	=event->mCurrent->mData[3].GetNumber();
	int dir	=event->mCurrent->mData[4].GetNumber();
	int spd	=event->mCurrent->mData[5].GetNumber();
	int freq=event->mCurrent->mData[6].GetNumber();
	int img	=event->mCurrent->mData[7].GetNumber();
	int etc	=event->mCurrent->mData[8].GetNumber();

	CreateLiftObject(no,x,y,z,dir,spd,freq,img,etc);
}
void CMapMode::SetNormalTrap(CEvent *event)
{
	int no	=event->mCurrent->mData[0].GetNumber();
	int x	=event->mCurrent->mData[1].GetNumber();
	int y	=event->mCurrent->mData[2].GetNumber();
	int z	=event->mCurrent->mData[3].GetNumber();
	int img	=event->mCurrent->mData[4].GetNumber();
	int cyc	=event->mCurrent->mData[5].GetNumber();
	int time=event->mCurrent->mData[6].GetNumber();
	int pow	=event->mCurrent->mData[7].GetNumber();
	int se	=event->mCurrent->mData[8].GetNumber();
	int etc	=event->mCurrent->mData[9].GetNumber();

	CreateNTObject(no,x,y,z,img,etc,cyc,time,pow,se);
}
void CMapMode::SetPutObject(CEvent *event)
{
	int no	=event->mCurrent->mData[0].GetNumber();
	int x	=event->mCurrent->mData[1].GetNumber();
	int y	=event->mCurrent->mData[2].GetNumber();
	int z	=event->mCurrent->mData[3].GetNumber();
	int spd	=event->mCurrent->mData[4].GetNumber();
	int img	=event->mCurrent->mData[5].GetNumber();
	int etc	=event->mCurrent->mData[6].GetNumber();

	CreatePutObject(no,x,y,z,spd,img,etc);
}


void CMapMode::SetEnemy(CEvent *event)
{
	int no	=event->mCurrent->mData[0].GetNumber();
	int x	=event->mCurrent->mData[1].GetNumber();
	int y	=event->mCurrent->mData[2].GetNumber();
	int z	=event->mCurrent->mData[3].GetNumber();
	int dir	=event->mCurrent->mData[4].GetNumber();
	int spd	=event->mCurrent->mData[5].GetNumber();
	int freq=event->mCurrent->mData[6].GetNumber();

	int movemode	=event->mCurrent->mData[7].GetNumber();
	int search		=event->mCurrent->mData[8].GetNumber();
	int reader		=event->mCurrent->mData[9].GetNumber();
	int lv			=event->mCurrent->mData[10].GetNumber();
	int member		=event->mCurrent->mData[11].GetNumber();
	int field		=event->mCurrent->mData[12].GetNumber();
	int etc			=event->mCurrent->mData[13].GetNumber();

	int img	=EnemyBaseData[reader].mMapImage;
	int grav=EnemyBaseData[reader].mMoveType;

	CreateEnemyObject(no,x,y,z,dir,spd,freq,img,true,grav,0,movemode,search,reader,lv,member,field,etc);
}
void CMapMode::SetEnemyLocked(CEvent *event)
{
	int no	=event->mCurrent->mData[0].GetNumber();
	int x	=event->mCurrent->mData[1].GetNumber();
	int y	=event->mCurrent->mData[2].GetNumber();
	int z	=event->mCurrent->mData[3].GetNumber();
	int dir	=event->mCurrent->mData[4].GetNumber();
	int spd	=event->mCurrent->mData[5].GetNumber();
	int freq=event->mCurrent->mData[6].GetNumber();

	int movemode	=event->mCurrent->mData[7].GetNumber();
	int search		=event->mCurrent->mData[8].GetNumber();
	int party		=event->mCurrent->mData[9].GetNumber();

	int reader=EnemyPartyData[party].mEnemy[0];
	int img	=EnemyBaseData[reader].mMapImage;
	int grav=EnemyBaseData[reader].mMoveType;

	CreateEnemyObject(no,x,y,z,dir,spd,freq,img,true,grav,0,movemode,search,party);
}

void CMapMode::SetBox(CEvent *event)
{
	int no	=event->mCurrent->mData[0].GetNumber();
	int x	=event->mCurrent->mData[1].GetNumber();
	int y	=event->mCurrent->mData[2].GetNumber();
	int z	=event->mCurrent->mData[3].GetNumber();
	int dir	=event->mCurrent->mData[4].GetNumber();
	int box	=event->mCurrent->mData[5].GetNumber();

	CreateBoxObject(no,x,y,z,dir,box);
}
void CMapMode::SetSavePoint(CEvent *event)
{
	int no	=event->mCurrent->mData[0].GetNumber();
	int x	=event->mCurrent->mData[1].GetNumber();
	int y	=event->mCurrent->mData[2].GetNumber();
	int z	=event->mCurrent->mData[3].GetNumber();

	CreateSavePoint(no,x,y,z);
}

void CMapMode::SetObjPos(CEvent *event)
{
	int no	=event->mCurrent->mData[0].GetNumber();
	if(mObject[no]){
		int x	=event->mCurrent->mData[1].GetNumber();
		int y	=event->mCurrent->mData[2].GetNumber();
		int z	=event->mCurrent->mData[3].GetNumber();
		mObject[no]->SetPos(x,y,z);
	}
}
void CMapMode::SetObjGoal(CEvent *event)
{
	int no	=event->mCurrent->mData[0].GetNumber();
	if(mObject[no]){
		int x	=event->mCurrent->mData[1].GetNumber();
		int y	=event->mCurrent->mData[2].GetNumber();
		int z	=event->mCurrent->mData[3].GetNumber();

		if(mEventSkip>=0x80000000+ESKIP_BLACK_OUT){
			mObject[no]->SetPos(x,y,z);
		}else{
			mObject[no]->SetGoal(x,y,z);
		}
	}
}
void CMapMode::SetObjGoalDir(CEvent *event)
{
	int no	=event->mCurrent->mData[0].GetNumber();
	if(mObject[no]){
		int dir	=event->mCurrent->mData[1].GetNumber();
		int l	=event->mCurrent->mData[2].GetNumber();

		if(mEventSkip>=0x80000000+ESKIP_BLACK_OUT){
			int nx,ny;
			mObject[no]->GetComp(dir,&nx,&ny);
			nx*=l;
			ny*=l;
			nx+=mObject[no]->GetX_32();
			ny+=mObject[no]->GetY_32();
			mObject[no]->SetPos(nx,ny,mObject[no]->GetZ_32());
		}else{
			mObject[no]->SetGoalDir(dir,l);
		}
	}
}
void CMapMode::SetObjDir(CEvent *event)
{
	int no	=event->mCurrent->mData[0].GetNumber();
	if(mObject[no]){
		int dir	=event->mCurrent->mData[1].GetNumber();

		mObject[no]->SetDir(dir);
	}
}
void CMapMode::SetObjImg(CEvent *event)
{
	int no	=event->mCurrent->mData[0].GetNumber();
	if(mObject[no]){
		int img	=event->mCurrent->mData[1].GetNumber();

		mObject[no]->LoadImage(img);
	}
}
void CMapMode::SetObjColor(CEvent *event)
{
	int no	=event->mCurrent->mData[0].GetNumber();	

	if(mObject[no]){
		int	 a	=event->mCurrent->mData[1].GetNumber();
		int	 r	=event->mCurrent->mData[2].GetNumber();
		int	 g	=event->mCurrent->mData[3].GetNumber();
		int	 b	=event->mCurrent->mData[4].GetNumber();
		int time=event->mCurrent->mData[5].GetNumber();	
		D3DCOLOR color= D3DCOLOR_ARGB(a,r,g,b);

		mObject[no]->SetColor(color,time);
	}
}
void CMapMode::SetObjSpeed(CEvent *event)
{
	int no	=event->mCurrent->mData[0].GetNumber();
	if(mObject[no]){
		int spd	=event->mCurrent->mData[1].GetNumber();

		mObject[no]->SetSpeed(spd);
	}
}
void CMapMode::SetObjGrav(CEvent *event)
{
	int no	=event->mCurrent->mData[0].GetNumber();
	if(mObject[no]){
		int grav	=event->mCurrent->mData[1].GetNumber();

		mObject[no]->SetGrav(grav);
	}
}
void CMapMode::SetObjParam(CEvent *event)
{
	int no	=event->mCurrent->mData[0].GetNumber();
	if(mObject[no]){
		int param	=event->mCurrent->mData[1].GetNumber();

		mObject[no]->SetParam(param);
	}
}
void CMapMode::SetObjView(CEvent *event)
{
	int no	=event->mCurrent->mData[0].GetNumber();
	if(mObject[no]){
		bool view	=(event->mCurrent->mData[1].GetNumber()!=0);

		mObject[no]->SetView(view);
	}
}
void CMapMode::SetObjZSpeed(CEvent *event)
{
	int no	=event->mCurrent->mData[0].GetNumber();
	if(mObject[no]){
		int zs	=event->mCurrent->mData[1].GetNumber();

		mObject[no]->SetZSpeed(zs/10.0);
	}
}

void CMapMode::KnockBack(CEvent *event)
{
	int no	=event->mCurrent->mData[0].GetNumber();
	int se	=event->mCurrent->mData[1].GetNumber();
	if(mObject[no]){
		mObject[0]->SetEscape(mObject[event->mParamA]);
		PlayWave(se);
	}
}
void CMapMode::ShowImage(CEvent *event)
{
	int no	=event->mCurrent->mData[0].GetNumber();
	if(no<=0)	
	if(mObject[no]){
			mShowImage=false;
			mImage.Release();
			return;
	}
	
	if(mEventSkip>=0x80000000+ESKIP_BLACK_OUT){
		return;
	}

	DTest.DrawSkip(false);
	char fname[256];
	sprintf(fname,"data\\otherimg\\img%02d",no);
	DTest.LoadTexture(mImage,fname);
	mShowImage=true;
}


void CMapMode::OpenBox(CEvent *event)
{
	if(!Flag.mTreasure[event->mParamA]){
		MenuData.SetMenu(BoxStartMenu,event->mParamA);
	}
}
void CMapMode::Encount(CEvent *event)
{
	if(mObject[event->mParamA]){
		CEnemyObject *p = dynamic_cast<CEnemyObject*>(mObject[event->mParamA]);
		if(p){
			if(p->mParty<0){
				EncountInfo.Encount(p,mEnemyLvUp,event->mParamB);
			}else{
				EncountInfo.Encount(p->mParty,mEnemyLvUp,event->mParamB);
			}
			if(event->mParamB==1){
				PlayWave(SE_HIT);
			}
			event->mWaitTime= 0;
			event->mCurrent = event->mCurrent->mNext;
			return;
		}
	}
}

void CMapMode::CheckBattle(CEvent *event)
{
#ifdef _DELETE_ESCAPE_ENEMY_
	DeleteObject(event->mParamA);
	return;
#endif
	if(mObject[event->mParamA]){
		CEnemyObject *p = dynamic_cast<CEnemyObject*>(mObject[event->mParamA]);
		if(p){
			if(!ReturnMapInfo.mEscaped){
				DeleteObject(event->mParamA);
			}else{
				if(!Flag.SearchFSkill(285)){
					mObject[0]->SetEscape(mObject[event->mParamA]);
				}
			}
		}
	}
}


void CMapMode::Ending(CEvent *event)
{
	GlobalFlag.mSoundTest=true;
	GlobalFlag.Save();
	if(Flag.mHardMode){
		Flag.mClearData = Flag.mClearData | 2;
	}else{
		Flag.mClearData = Flag.mClearData | 1;
	}
	NextMonth(event);

	Flag.AllHeal();
	if(mEventSkip<0x80000000+ESKIP_BLACK_OUT){
		PlayBGM(23);
	}
	if(GetEventSkip()){
		EndingMode.EndEnding();
	}else{
		GameMode=GMODE_ENDING;
		EndingMode.Init();
	}
}

void CMapMode::TrueEnd(CEvent *event)
{
	NextMonth(event);
	MapMode.EndEventSkip();

	Flag.AllHeal();
	GameMode=GMODE_ENDING;
	EndingMode.Init(3);
}
