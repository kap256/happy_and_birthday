#include "village.h"
#include "input.h"
#include "cursor.h"
#include "menudata.h"
#include "changemode.h"
#include "option.h"
#include "mapmode.h"

CVillageMode::CVillageMode()
{
	mCursor=0;
}

CVillageMode::~CVillageMode()
{
}

void CVillageMode::Init()
{
	DTest.LoadTexture(mBackGround,"data\\otherimg\\village");
	char filename[256];
	sprintf(filename,"data\\mapobject\\mapobject%03d",Flag.mParty[0]);
	DTest.LoadTexture(mPlayer,filename,0xff008080);
	mTimer=0;

	for(int i=ID_UI;i<ID_NSUI;i++){
		Flag.mCloset[i]=0;
	}

	InitMenu();

	strcpy(Flag.mMapName,"フィーレス村");
	Flag.mClosetUse=true;

	const CVillageItem *vi=(CVillageItem*)mList.GetCursorItem();
	mPlayerX=vi->mX;
	mPlayerY=vi->mY;

	PlayBGM(2);		
}
void CVillageMode::InitMenu()
{
/*	char fname[128];
	int bgmonth;
	if(Flag.mCurrentMonth<0){
		bgmonth=Flag.mMonth-1;
	}else{
		bgmonth=pInput->Rand(Flag.mMonth);
	}
	sprintf(fname,"data\\villagebg\\bg%02d",bgmonth);
*/	


const int MENU_NO=12;
	static const CVillageItem ele[MENU_NO]={
		CVillageItem("冒険に出かける"				,0,48	,973,485),
		CVillageItem("準備する"						,9,57	,492,148),
		CVillageItem("メンバー変更"					,10,58	,661,327),
		CVillageItem("ジュエルショップ・リーリン"	,1,49	,659,386),
		CVillageItem("武器屋・ジョーイ工房"			,2,50	,524,46),
		CVillageItem("防具屋・アイアンウォール"		,3,51	,49	,158),
		CVillageItem("雑貨屋・トライアック商店"		,4,52	,727,188),
		CVillageItem("掘り出し物屋・東封"			,5,53	,544,395),
		CVillageItem("アーゲリオ農場"				,6,54	,215,40),
		CVillageItem("日記"							,8,56	,492,148),
		CVillageItem("倉庫"							,7,55	,727,188),
		CVillageItem("初心戻し"						,11,59	,727,188),
	};
	int		no;
	int		list[MENU_NO];
	int		plist[MENU_NO];
	memset(plist,0,sizeof(plist));

	no=0;
	for(int i=0;i<10;i++){
		if(ele[i].mID==10){
			if(Flag.mCurrentMonth<=0)	continue;
			if(Flag.mMonth<=1)			continue;
		}else if(1<=ele[i].mID && ele[i].mID<=6){
#ifdef _NO_SHOP_
			continue;
#endif
			int shop = ele[i].mID-1;
			if(Flag.mShopOK[shop]==0)	continue;
			if(shop<5){
				if(Flag.mShopNew[shop]){
					plist[no] = 1;
				}
			}else{
				for(int i=0;i<MAX_SEED_NUMBER;i++){
					int seed=Flag.GetAllItem(ID_SD+i,false,2);
					if(seed>0){
						plist[no]=1;
						break;
					}
				}
			}
		}
		list[no]=i;
		no++;
	}

	mOriginal=(Flag.SearchFSkill(315,true));
	if(mOriginal){
		list[no-1]	= 11;
		list[no]	= 9;
		no++;
	}

	mList.InitListWindow(10,30,320,ele,list,no,no,sizeof(CVillageItem),plist);
	mList.SetCursor(mCursor);

	mSubList.Destroy();
	mSubList.QuickDestroy();

	mBypathList.Destroy();
	mBypathList.QuickDestroy();

	Flag.mMapBGMNo=0;

//	Flag.AllHeal();
}

void CVillageMode::OnTimer()
{
	if(	mOriginal!=(Flag.SearchFSkill(315,true))){
		InitMenu();
	}
	mTimer++;

	mList.OnTimer(mSubList.GetDestroy());
	mSubList.OnTimer(mBypathList.GetDestroy());
	mBypathList.OnTimer(true);

	const CVillageItem *vi=(CVillageItem*)mList.GetCursorItem();
	double mx=(vi->mX - mPlayerX)/8.0;
	double my=(vi->mY - mPlayerY)/8.0;
	mPlayerX+=mx;
	mPlayerY+=my;

	mCursor=mList.GetCursor();
	if(pInput->GetKey(4)==1){
		PlayWave(SE_YES);
		if(!(mBypathList.GetDestroy())){
			const CElement* ele=mBypathList.GetCursorItem();
			SetBGMFadeOut(10);
			switch(ele->mID){
				case 0:
					ChangeMapInfo.ChangeMap(Flag.mBypathMapNo,
											Flag.mBypathX,
											Flag.mBypathY,
											Flag.mBypathZ,
											Flag.mBypathDir);
					Flag.mMapBGMNo=	Flag.mBypathBGM;
					Flag.mClosetUse=false;
					Flag.mOldMonth=Flag.mCurrentMonth;
					Cursor.Escape();
					Flag.Market();
					Flag.CheckReserve();
					MapMode.SetBGMWait(10);
					break;
				case 1:
					StartMonth(Flag.mCurrentMonth);
					break;
			}
		}else if(!(mSubList.GetDestroy())){
			const CElement* ele=mSubList.GetCursorItem();
			if(ele->mID<32){
				if(ele->mID == Flag.mCurrentMonth && Flag.SearchFSkill(355) && Flag.mBypathMapNo>=0){
					InitBypathList(Cursor.GetY());
				}else{
					SetBGMFadeOut(10);
					StartMonth(ele->mID);
				}
			}else if(ele->mID<34){
				MenuData.SetMenu(DiaryMenu,ele->mID-32);
			}else if(ele->mID==34){
				MenuData.SetMenu(ExitYesNo,0);
			}
		}else{
			const CElement* ele=mList.GetCursorItem();
			switch(ele->mID){
				case 0:
					InitMonthList();						break;
				case 1:
					ChangeMapInfo.ChangeMap(51,3,5,2,1);
					Flag.mShopNew[0]=false;
					Cursor.Escape();
					break;
				case 2:
					ChangeMapInfo.ChangeMap(52,5,3,2,0);
					Flag.mShopNew[1]=false;
					Cursor.Escape();
					break;
				case 3:
					ChangeMapInfo.ChangeMap(53,4,6,2,2);
					Flag.mShopNew[2]=false;
					Cursor.Escape();
					break;
				case 4:
					ChangeMapInfo.ChangeMap(54,6,4,2,0);
					Flag.mShopNew[3]=false;
					Cursor.Escape();
					break;
				case 5:
					ChangeMapInfo.ChangeMap(50,5,6,2,1);
					Cursor.Escape();
					break;
				case 6:
					ChangeMapInfo.ChangeMap(55,6,2,2,3);
					Cursor.Escape();
					break;
				case 7:
					MenuData.SetMenu(ClosetMenu,0);			break;
				case 8:
					InitDiaryList();						break;
				case 9:
					MenuData.SetMenu(StartMenu,0);			break;
				case 10:
					MenuData.SetMenu(ChangeMemberMenu,0);	break;
				case 11:
					MenuData.SetMenu(RemoveBirthStone,0);	break;

			}
		}
	}
	if(pInput->GetKey(5)==1){;
		if((mBypathList.GetDestroy()) && (mSubList.GetDestroy())){
			MenuData.SetMenu(StartMenu,0);
		}
	}
	if(pInput->GetKey(6)==1){
		if(!(mBypathList.GetDestroy())){
			mBypathList.Destroy();
			PlayWave(SE_NO);
		}else if(!(mSubList.GetDestroy())){
			mSubList.Destroy();
			PlayWave(SE_NO);
		}
	}
}

void CVillageMode::OnDraw()
{
	const D3DCOLOR BG=0x00004000;
	
	const int W=BG_W-300;
	const int H=BG_H-480;
	int cx=int((mPlayerX/BG_W)*W);
	int cy=int((mPlayerY/BG_H)*H);

	DTest.DrawTexture(mBackGround,0,0,0,BG_W,BG_H,340-cx,-cy,BG_W,BG_H);
	DTest.DrawBox(0,0,340,480,BG | 0xff000000);
	DTest.DrawBox(340,0,360,480,BG | 0xff000000,BG,BG | 0xff000000,BG);
	int ani;
	switch((mTimer/16)%4){
		case 0:		ani=1;	break;
		case 2:		ani=3;	break;
		default:	ani=2;	break;
	}
	DTest.DrawTexture(mPlayer,0,0,ani*64,64,64,int(mPlayerX)+340-32-cx,int(mPlayerY)-32-cy,64,64);


	mList.OnDraw();
	mSubList.OnDraw();
	mBypathList.OnDraw();

	if(mTimer<16){
		int alpha=(15-mTimer)*16;
		DTest.DrawBox(0,0,640,480,D3DCOLOR_ARGB(alpha,0,0,0));
	}
}

void CVillageMode::InitMonthList()
{
	int h=Flag.mMonth;
	if(h>12)	h=12;
	int slv=Flag.GetDInfo(true);

	mSubList.InitListWindow(180,40,(slv?400:320),
							MonthData,NormalHard[Flag.mHardMode],
							Flag.mMonth,h,sizeof(CMonthData));
	if(Flag.mCurrentMonth>=0){
		for(int i=0;i<Flag.mMonth;i++){
			if(MonthData,NormalHard[Flag.mHardMode][i]==Flag.mCurrentMonth){
				mSubList.SetCursor(i);
				break;
			}
		}
	}else{
		mSubList.SetCursor(Flag.mMonth-1);
	}

	for(int i=0;i<Flag.mMonth;i++){
		mSubList.SetParam(i,slv);
	}
}
void CVillageMode::InitDiaryList()
{
	const int MENU_NO=3;
	static const CElement ele[MENU_NO]={
		CElement("セーブ"			,32),
		CElement("ロード"			,33),
		CElement("タイトルへ"		,34),
	};

	int y =Cursor.GetY();
	if(y>400)	y=400;
	mSubList.InitListWindow(220,y - 30,160,
							ele,NULL,
							MENU_NO,MENU_NO,sizeof(CElement));
}

void CVillageMode::InitBypathList(int y)
{
	const int MENU_NO=2;
	static const CElement ele[MENU_NO]={
		CElement("抜け道をつかう"	,0),
		CElement("はじめから"		,1),
	};
	if(y>400)	y=400;
	mBypathList.InitListWindow(420,y,200,
							ele,NULL,
							MENU_NO,MENU_NO,sizeof(CElement));
}

void  CVillageMode::StartMonth(int month)
{
	ChangeMapInfo.ChangeMap(MonthData[month].mStartMap,
						MonthData[month].mX,
						MonthData[month].mY,
						MonthData[month].mZ,
						0);
	Flag.mClosetUse=false;
	if(Flag.mCurrentMonth!=month){
		Flag.mBypathMapNo=-1;
		for(int i=MAX_PARTY_NUMBER-1;i>=0;i--){
			Flag.ChangeMember(i,-1);
		}
		for(int i=0;i<MAX_PARTY_NUMBER;i++){
			int character=MonthData[month].mParty[i];
			if(0<=character && character<=1 && (Flag.mHardMode&1)){
				character=1-character;
			}
			Flag.ChangeMember(i,character);
		}
	}
	Flag.mOldMonth=Flag.mCurrentMonth;
	Flag.mCurrentMonth=month;
	Cursor.Escape();
	Flag.Market();
	Flag.AllHeal();
}
