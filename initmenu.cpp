#include "menudata.h"
#include "itemdata.h"
#include "flag.h"
#include "cursor.h"
#include "input.h"
#include "month.h"
#include "main.h"
#include "dataconst.h"
#include "battlemode.h"
#include "option.h"
#include "changemode.h"
#include "bgmdata.h"


CStartMenu::CStartMenu(int param)
{
	InitList();

const int INFO_W=280;
const int INFO_H=80;
	mInfo.InitPlaneWindow(	640-INFO_W,
							480-CCharaDataWindowMenu::WIN_Height-INFO_H,
							INFO_W,
							INFO_H);
	mNextMenu=-1;
	mCharacter=0;
	if(param&1){
		mCDataMove=false;
		for(int i=0;i<MAX_PARTY_NUMBER;i++){
			mCDataWindow[i].BaseMove(	(i%2)*CCharaDataWindowMenu::WIN_WIDTH,
										(i/2)*(480-CCharaDataWindowMenu::WIN_Height));
		}
	}else{
		mCDataMove=true;
		for(int i=0;i<MAX_PARTY_NUMBER;i++){
			mCDataWindow[i].InitWindow(	(i%2)*CCharaDataWindowMenu::WIN_WIDTH,
										(i/2)*(480-CCharaDataWindowMenu::WIN_Height),
										Flag.mParty[i]);
		}
		InitReserveCDataWin();
	}
	
	mReserveNo=0;
	for(int i=0;i<MAX_CHARACTER_NUMBER;i++){
		if(Flag.mParty[i+MAX_PARTY_NUMBER]<0)	break;
		mReserveNo++;
	}
}
void	CStartMenu::InitList(void)
{
	static const CItem ele[MENU_NO]={
		CItem("装備・アイテム",0,0),
		CItem("誕生石",4,0),
		CItem("陣形",5,0),
		CItem("ステータス",2,0),
		CItem("設定",3,0),
		CItem("セーブ(_DEBUG)",255,0),
		CItem("ダンジョン情報",6,0),
		CItem("入れ替え(_DEBUG)",256,0),
		CItem("倉庫(_DEBUG)",257,0),
		CItem("農場(_DEBUG)",258,0),
		CItem("貴重品",7,0),
	};

	int list[MENU_NO]={0,1,2,3,10};
	int msize=5;
	if(Flag.GetDInfo(Flag.mCurrentMonth,false)){
		list[msize]=6;
		msize++;
	}
	list[msize]=4;
	msize++;

#ifdef _DEBUG
	list[msize]=9;
	list[msize+1]=7;
	list[msize+2]=5;
	msize+=3;
#endif
	mWindow.InitListWindow(0,CCharaDataWindowMenu::WIN_Height,180,ele,list,msize,msize,sizeof(CItem));
}
//////////////////////////////////////////////////////////////////////////////////////////////////////
CItemMenu::CItemMenu(int param)
{
	mCharacter=param;
	mCategory = 0;
	mCategoryAll=0;
	mMode=0;

	ChangeCharacter(mCharacter);
	mSWindow.Init(300,LIST_Y+20,mCharacter,true);
	int cursor=mItemList.GetCursor();
	int item=Flag.mPlayerData[Flag.mParty[mCharacter]].GetItem(cursor) .item;
	mHelpWindow.InitWindow(300,LIST_Y+118,item);

	CPlayerData &cdata=Flag.mPlayerData[Flag.mParty[mCharacter]];
	mCompare.InitWindow(-320,20,0,&cdata,true);
	InitClosetList(&mClosetList,650,45,mCategory,15,true,false);
	mCategoryWindow.InitSelectWindow(
		650,8,320,
		ItemCategory,NULL,
		CATEGORY_MAX,sizeof(CItemCategoryData));
}

void	CItemMenu::ChangeCharacter(int chara)
{
	mCharacter=chara;
	int cursor=mItemList.GetCursor();
	InitItemList(Flag.mParty[mCharacter],&mItemList,LIST_X,LIST_Y); 
	mItemList.SetCursor(cursor);
	CheckItemCursor(cursor);
	ChangeHelp();
	SetReserveParam();
}

void	CItemMenu::SetReserveParam()
{
	if(mCharacter>=MAX_PARTY_NUMBER){
		CPlayerData& pdata=Flag.mPlayerData[Flag.mParty[mCharacter]];
		for(int i=pdata.GetEquipMax();i<pdata.GetItemMax();i++){
			mItemList.SetParam(i,mItemList.GetParam(i) | 0x800);
		}
	}
}
void CItemMenu::Update()
{
	UpdataItemList(Flag.mParty[mCharacter],&mItemList);
	SetReserveParam();
	ChangeHelp();

	if(mCategory<=0){
		UpdataClosetList(&mClosetList,mCategoryAll); 
	}else{
		UpdataClosetList(&mClosetList,1<<mCategory); 
	}
	mSWindow.Update();
}
void CItemMenu::ChangeHelp()
{
	if(mMode==0){
		int cursor=mItemList.GetCursor();
		int item=Flag.mPlayerData[Flag.mParty[mCharacter]].GetItem(cursor).item;
		mHelpWindow.ChangeItem(item);
	}else if(mMode==1){
		int item=mClosetList.GetCursorItem()->mID;
		int no=mItemList.GetCursor();
		mHelpWindow.ChangeItem(item);
		mCompare.SetEquip(item,no);
	}
}
/////////////////////////////////////////////////////////////////////////////////
CItemSubMenu::CItemSubMenu(int param)
{
	mCharacter=(param/0x100)%0x100;
	mItem=param%0x100;

	CPlayerData &cdata=Flag.mPlayerData[Flag.mParty[mCharacter]];

	mAccessoryTimer=0;
	static const CItem ele[6]={
		CItem("装備する",0,0),
		CItem("変更",1,0),
		CItem("使う",2,0),
		CItem("まとめる",3,0),
		CItem("整頓",18,0),
		CItem("交換",16,0),
	};
	int list[8];
	int size=0;

	int item=cdata.GetItem(mItem).item;
	if(mItem<WEAPON_SPACE){
		if(item==NO_ITEM){
			ChangeWeapon();
			Destroy();
			return;
		}else{
			list[size]=1;
			list[size+1]=0;
			size+=2;
		}
	}else{
		if(item==NO_ITEM){
			size=1;
			list[0]=5;
		}else{
			if(AllItem[item]->mType & TYPE_ON_MENU){
				list[size]=2;
				size++;
			}
			list[size]=5;
			size+=1;
		}
	}

	int y=(param/0x10000)-ITEM_Height;
	
	mTitle.InitPlaneWindow(CItemMenu::LIST_X,y,ITEM_LIST_WIDTH,ITEM_Height+32,true);

	if(y>480-size*ELEMENT_Height-32)	y=480-size*ELEMENT_Height-32;

	mList.InitListWindow(CItemMenu::LIST_X+ITEM_LIST_WIDTH,y,150,ele,list,size,size,sizeof(CItem));

	if(AllItem[item]->mType & TYPE_EQUIPMENT){
		int cy;
		if(y>480-ITEM_Height-32-CCharaDataWindowCompare::WIN_Height){
			cy=y-CCharaDataWindowCompare::WIN_Height;
		}else{
			cy=y+ITEM_Height+32;
		}

		mCompare.InitWindow(CItemMenu::LIST_X,cy,mItem,&cdata,true);
	}
}
void CItemSubMenu::Update()
{
	Destroy();
}
////////////////////////////////////////////////////////////////////
CExchangeItem::CExchangeItem(int param)
:CItemMenu((param%0x10000)/0x100)
{
	mExchangeCharacter=Flag.mParty[(param%0x10000)/0x100];
	mExchangeItem=param%0x100;
	int y=param/0x10000;

	ChangeCharacter();

	mSWindow.SetReserve(false);
	mItemList.SetCursor(mExchangeItem);
	mExchangeInfo.InitPlaneWindow(320,140,320,INFO_Height);
}

void	CExchangeItem::ChangeCharacter(void)
{
	CPlayerData& pdata=Flag.mPlayerData[Flag.mParty[mCharacter]];
	InitItemList(Flag.mParty[mCharacter],&mItemList,LIST_X,LIST_Y);
	mItemList.QuickMove(5,15);

	for(int i=0;i<pdata.GetEquipMax();i++){
		mItemList.SetParam(i,mItemList.GetParam(i) | 0x800);
	}

	mItemList.SetCursor(pdata.GetEquipMax());
}
/////////////////////////////////////////////////////////////////////////////////
CDumpItem::CDumpItem(int param)
{
	mCharacter=param/0x100;
	mItem=param%0x100;
	
	mList.InitYesNoWindow(320-50,250,100);
	mTitle.InitPlaneWindow(320-150,150,300,100);
}
/////////////////////////////////////////////////////////////////////////////////
CUIMenuSelectCharacter::CUIMenuSelectCharacter(int param)
{
	mDTime=0;

	if(param>=0){
		mUseCharacter=param/0x100;
		mUseItem=param%0x100;
	}else{
		mUseCharacter=-1;
		mUseItem=(0x100+param)%0x100;
	}

	mCharacter=0;
	
	mTitle.InitPlaneWindow(20,20,280,ITEM_Height+32);

	for(int i=0;i<MAX_PARTY_NUMBER;i++){
		int x=300-i*50;
		int y=40+i*CCharaDataWindowMenu::WIN_Height;
		mCDataWindow[i].Move(x,y);
	}
}
/////////////////////////////////////////////////////////////////////////////////
CBirthStoneMenu::CBirthStoneMenu(int param)
{
	mBirthStoneSystem.Init(param);
}
void CBirthStoneMenu::Update()
{
	mBirthStoneSystem.Update();
}
/////////////////////////////////////////////////////////////////////////////////
CLearnSkillMenu::CLearnSkillMenu(int param)
{
	mCharacter=param/0x100;
	mMonth=param%0x100;
	mMessage=NULL;
	mMessageTimer=0;
	mActiveWindow=0;
	mHelp=NULL;

	int chara=Flag.mParty[mCharacter];
	int skill[MAX_SKILL_MONTH];

	int i;
	for(i=0;i<MAX_SKILL_MONTH;i++){
		skill[i]=CharacterSkill[chara][mMonth][i];
	}


	mLearnSkill.InitListWindow(	0,20,320,
								SkillData,skill,
								MAX_SKILL_MONTH,MAX_SKILL_MONTH,
								sizeof(CSkillData),NULL);
	mLearnSkill.InitPlaneWindow(300,CBirthStoneSystem::MONTH_LIST_Y+100,
								320,SKILL_Height*MAX_SKILL_MONTH+32,true);
	mLearnSkill.Move(320,Y+45);

	const int LeftY=Y+0;
	mCompare.InitWindow(0,LeftY,&Flag.mPlayerData[chara],&mAfter);
	mMessageWindow[0].InitPlaneWindow(320,Y,240,45);
	mMessageWindow[1].InitPlaneWindow(380,Y+10,240,45);

	mHelpWindow.InitPlaneWindow(0,LeftY+mCompare.GetHeight(),
		mCompare.GetWidth(),45+mLearnSkill.GetHeight()-mCompare.GetHeight());


	static const CBirthStone bstone[MAX_MONTH]={
		CBirthStone("1月",0),
		CBirthStone("2月",1),
		CBirthStone("3月",2),
		CBirthStone("4月",3),
		CBirthStone("5月",4),
		CBirthStone("6月",5),
		CBirthStone("7月",6),
		CBirthStone("8月",7),
		CBirthStone("9月",8),
		CBirthStone("10月",9),
		CBirthStone("11月",10),
		CBirthStone("12月",11)
	};
	mBirthStone=&bstone[mMonth];
	mBirthStoneWindow.InitPlaneWindow(CBirthStoneSystem::MONTH_LIST_X,
								CBirthStoneSystem::MONTH_LIST_Y+mMonth*32+8,
								CBirthStoneSystem::MONTH_LIST_WIDTH,
								48,true);
	mBirthStoneWindow.Move(CBirthStoneSystem::MONTH_LIST_X,LeftY-48);
	Update();
	MoveCursor(0);
	mCDataWindow[mCharacter].Move(300,Y-100);
}
void	CLearnSkillMenu::Update(void)
{
	int chara=Flag.mParty[mCharacter];
	int learn[MAX_LEARN_MONTH];
	int listparam[MAX_SKILL_MONTH];

	Flag.mPlayerData[chara].GetLearnSkill(mMonth,learn);
	memset(listparam,0,sizeof(listparam));
	for(int i=0;i<MAX_LEARN_MONTH;i++){
		if(0<learn[i] && learn[i]<=MAX_SKILL_MONTH){
			listparam[learn[i]-1]=1;
		}
	}
	listparam[MAX_SKILL_MONTH-1]+=2;

	mLearnSkill.SetParam(listparam);
	mStoneNo[0]=Flag.GetAllItem(ITEM_BIRTH_STONE+mMonth,false,2);
	mStoneNo[1]=Flag.GetAllItem(ITEM_BIRTH_STONE+mMonth+MAX_MONTH,false,2);


	int	param[MAX_MONTH];
	CBirthStoneSystem::GetBSParam(Flag.mParty[mCharacter],param);
	mBSParam=param[mMonth];
}

/////////////////////////////////////////////////////////////////////////////////
CLearnSkillYesNo::CLearnSkillYesNo(int param)
{
	mSkill=(param&0xff0000)/0x10000;
	mCharacter=(param&0xff00)/0x100;
	mMonth=param&0xff;

	
	mList.InitYesNoWindow(320-50,250,100);
	mTitle.InitPlaneWindow(320-150,150,300,100);
}
/////////////////////////////////////////////////////////////////////////////////
CStatusMenu::CStatusMenu()
{
	mCharacter=NULL;
	mPlayer=NULL;
	mListType=0;
}
CStatusMenu::CStatusMenu(int param)
{
	//
	mCharacter=NULL;
	mPlayer=NULL;
	mListType=0;
	mReserve=((param&32)!=0);
	mAllMember=((param&64)!=0);
	mChangeable=true;
	mCharacterNum=param&0x1f;
	int y=Init(param);
	if(y>0){
		mListTitle.InitPlaneWindow(300,30,ITEM_LIST_WIDTH,40);
		mListHelp.InitPlaneWindow(300,y,ITEM_LIST_WIDTH,420-y);
	}
}
int CStatusMenu::Init(int param)
{
	int add;
	switch(param/0xff){
		case 1:		add=1;	break;
		case 2:		add=-1;	break;
		default:	add=0;	break;
	}
	if(add){
		int max;
		if(mReserve || mAllMember)	max=MAX_CHARACTER_NUMBER;
		else						max=MAX_PARTY_NUMBER;
		for(int i=0;i<max;i++){
			mCharacterNum+=add;
			if(mCharacterNum>=max)	mCharacterNum=0;
			if(mCharacterNum<0)		mCharacterNum=max-1;

			if(mReserve){
				if(Flag.mParty[mCharacterNum+MAX_PARTY_NUMBER ]>=0)		break;
			}else{
				if(Flag.mParty[mCharacterNum]>=0)	break;
			}
		}
	}

	if(mDelete){
		delete mCharacter;
		mCharacter=NULL;
	}
	mPlayer=new CPlayerCharacter;
	mCharacter=mPlayer;
	if(mReserve){
		mCharacter->LoadCharacter(Flag.mParty[mCharacterNum+MAX_PARTY_NUMBER],0,0,0);
	}else{
		mCharacter->LoadCharacter(Flag.mParty[mCharacterNum],0,0,0);
	}
	mDelete=true;

	if(!mReserve && Flag.mParty[mCharacterNum]<0){
		Destroy();
		return 0;
	}

	mWindow.InitPlaneWindow(20,30,280,390,false);
	mListType--;
	ChangeList();
	return mList.GetY()+mList.GetHeight();
}
void	CStatusMenu::ChangeList(void)
{
	mListType++;
	mListType%=LIST_TYPE;

	if(mDelete  && mListType==3)	mListType=0;
	if(!mPlayer && mListType==0)	mListType=1;

	switch(mListType){
		case 0:
			{
				CPlayerData &cdata=*(mPlayer->GetPData());
				int maxitem;
				int itemlist[MAX_ITEM];
				int param[MAX_ITEM];

				GetItemListInfo(cdata,&maxitem,itemlist,param);

				mList.InitListWindow(	300,70,ITEM_LIST_WIDTH,
										(const CElement**)AllItem,itemlist,
										maxitem,ELEMENT_Height/2,param,false);
			}
			break;
		case 1:
			{
				CCharacterData &cdata=*(mCharacter->GetCData());
				int	skill[MAX_CSKILL];
				int size=cdata.GetCSkillAll(0xffffffff,skill,NULL);
				int param[MAX_CSKILL];
				for(int i=0;i<MAX_CSKILL;i++){
					param[i]=(mCharacter->CheckOpenData(ODATA_CS+i) ? 0 : 4);
				}
				mList.InitListWindow(	300,
										70,
										ITEM_LIST_WIDTH,
										ComSkillData,skill,
										size,ITEM_Height/2,sizeof(CComSkillData),param,false);
			}
			break;
		case 2:
			{
				CCharacterData &cdata=*(mCharacter->GetCData());
				int	skill[MAX_ASKILL];
				int size=cdata.GetASkillAll(skill);
				int param[MAX_ASKILL];
				for(int i=0;i<MAX_ASKILL;i++){
					param[i]=(mCharacter->CheckOpenData(ODATA_AS+i) ? 0 : 4);
				}
				mList.InitListWindow(	300,
										70,
										ITEM_LIST_WIDTH,
										SkillData,skill,
										size,ELEMENT_Height/2,sizeof(CSkillData),param,false);
			}
			break;
		case 3:
			{
				int	st[STATUS_NO],param[STATUS_NO];
				int size=mCharacter->GetStatusAll(st,param);
				if(mCharacter->SearchASkill(305))	size=0;
				mList.InitListWindow(	300,
										70,
										ITEM_LIST_WIDTH,
										StatusData,st,
										size,ELEMENT_Height/2,sizeof(CStatusData),param,false);
			}
			break;
	}
}

/////////////////////////////////////////////////////////////////////////////////
CFormationMenu::CFormationMenu(int param)
{
	mWindow.InitPlaneWindow(320,80,320,320);
	mCharacter=0;
	mSelected=false;
	mTimer=0;

	for(int i=0;i<MAX_PARTY_NUMBER;i++){
		mCDataWindow[i].Move(0,40+i*CCharaDataWindowMenu::WIN_Height);
		mPlayer[i].LoadCharacter(Flag.mParty[i],0,0,0);
	}
}
/////////////////////////////////////////////////////////////////////////////////
CBoxStartMenu::CBoxStartMenu(int param)
{
	mBox=param;

	static const CItem ele[3]={
		CItem("開ける",0,0),
		CItem("アイテムの確認",1,0),
		CItem("立ち去る",2,0)
	};

	int	Height=60;
	int	width=320;
	mKnowTrap=false;
	if(TreasureData[mBox].mKey>0){
		for(int i=0;i<MAX_KEY_TYPE;i++){
			if(TreasureData[mBox].mKey & (1<<i)){
				Height+=20;
			}
		}
	}else{
		Height+=20;
	}

	if(Flag.SearchFSkill(282)){
		mKnowTrap=true;
		Height+=40;
		if(TreasureData[mBox].mTrap>=0){
			Height+=170;
			width+=160;
		}
	}	
	for(int i=0;i<MAX_PARTY_NUMBER;i++){
		mCDataWindow[i].InitWindow(	(i%2)*(640+CCharaDataWindowMenu::WIN_WIDTH)-CCharaDataWindowMenu::WIN_WIDTH,
									(i/2)*(480+CCharaDataWindowMenu::WIN_Height)-CCharaDataWindowMenu::WIN_Height,
									Flag.mParty[i]);
	}
	InitReserveCDataWin();

	mList.InitListWindow(220,180+Height/2,200,ele,NULL,3,3,sizeof(CItem));

	mX=320-width/2;
	mWindow.InitPlaneWindow(mX,180-Height/2,width,Height);
}
void	CBoxStartMenu::Update(void)
{
	mList.Move(220,mList.GetY());
	mWindow.Move(mX,mWindow.GetY());
}

/////////////////////////////////////////////////////////////////////////////////
CBoxSelectMenu::CBoxSelectMenu(int param)
{
	mBox=param & 0xffff;
	mKnowTrap= ((param & 0xff0000)>0);
	mCharacter=0;

	const int &key=TreasureData[mBox].mKey;

	for(int i=0;i<MAX_KEY_TYPE;i++){
		mKey[i]=Flag.GetAllItem(ITEM_KEY+i,false,2);
	}

	for(int i=0;i<MAX_PARTY_NUMBER;i++){
		mCDataWindow[i].Move(300,50+i*CCharaDataWindowMenu::WIN_Height);

		mOpen[i]=0;
		for(int j=0;j<MAX_KEY_TYPE;j++){
			if(TreasureData[mBox].mKey & (1<<j)){
				switch(j){
					case 0:
					case 1:
						mUseKey[i][j]=!(Flag.mPlayerData[Flag.mParty[i]].SearchASkill(278));	break;
					case 2:
					case 3:
					case 4:
						mUseKey[i][j]=!(Flag.mPlayerData[Flag.mParty[i]].SearchASkill(279));	break;
					case 5:
					case 6:
						mUseKey[i][j]=!(Flag.mPlayerData[Flag.mParty[i]].SearchASkill(280));	break;
					case 7:
						mUseKey[i][j]=true;	break;
				}
				if(mUseKey[i][j]){
					if(mOpen[i]<2){
						if(mKey[j]>0){
							mOpen[i]=1;
						}else{
							mOpen[i]=2;
						}
					}
				}
			}else{
				mUseKey[i][j]=false;
			}
		}
	}

	InitParamWin();

/*	if(mKnowTrap){
		mParamWindow.InitPlaneWindow(
			mCDataWindow[0].GetX(),
			mCDataWindow[0].GetY()+CCharaDataWindowMenu::WIN_Height-40,
			120,64);
	}
*/
}
void	CBoxSelectMenu::InitParamWin(void)
{
	int paramh=30;

	for(int i=0;i<MAX_KEY_TYPE;i++){
		if(TreasureData[mBox].mKey & (1<<i)){
			paramh+=30;
		}
	}
	if(mKnowTrap && TreasureData[mBox].mTrap>=0){
		paramh+=50;
	}
	if(paramh<=30)	return;
	mParamWindow.InitPlaneWindow(
		20,
		420-paramh,
		280,paramh);
}

/////////////////////////////////////////////////////////////////////////////////
COverItemMenu::COverItemMenu(int param)
{
	mSystem.Init();
	mCDataInited=mCDataWindow[0].GetInited();
	if(!mCDataInited){
		for(int i=0;i<MAX_PARTY_NUMBER;i++){
			mCDataWindow[i].InitWindow(	(i%2)*(640+CCharaDataWindowMenu::WIN_WIDTH)-CCharaDataWindowMenu::WIN_WIDTH,
										(i/2)*(480+CCharaDataWindowMenu::WIN_Height)-CCharaDataWindowMenu::WIN_Height,
										Flag.mParty[i]);
		}
		InitReserveCDataWin();
	}
}
void COverItemMenu::Update(void)
{
	mSystem.ResetList();
}

/////////////////////////////////////////////////////////////////////////////////
CItemHelp::CItemHelp(int param)
{
	mItemNo=param;
	mText=GetItemText(param);
	mAllNo=Flag.GetAllItem(param);

	int w,h;
	CPlaneWindow::GetMessageSize(0,mText,&w,&h);

	if(w<240)	w=240;
	w+=50;
	h+=110;

	mWindow.InitPlaneWindow(320-w/2,240-h/2,w,h);

	Cursor.Escape();
}
/////////////////////////////////////////////////////////////////////////////////
CSelectHelp::CSelectHelp()
{
	memset(mList,0,sizeof(mList));
	mListSize=0;
}
int	CSelectHelp::InitList(const CElement* list[],int size,int h,int w)
{
	mHeight		=h;
	mWidth		=w;
	mListSize	=size;
	mCursor	=0;

	for(int i=0;i<size;i++){
		mList[i]=list[i];
	}

	if(size>0){
		mEleHeight = mList[0]->GetHeight();
		return size*mEleHeight;
	}
	Cursor.Escape();
	return 0;
}
/////////////////////////////////////////////////////////////////////////////////
CEquipmentHelp::CEquipmentHelp(int param)
{
	mItemNo=param;
	mText=GetItemText(param);
	mAllNo=Flag.GetAllItem(param);

	int w,h;
	CPlaneWindow::GetMessageSize(0,mText,&w,&h);

	if(w<240)	w=240;
	h+=70;

	const CElement* ele[LIST_MAX];
	const CEquipmentData & edata=EquipmentData[param];
	int size=edata.mSkillNo;
	for(int i=0;i<edata.mSkillNo;i++){
		ele[i]=&(SkillData[edata.mSkill[i]]);
	}
	int lh=InitList(ele,size,h+40,w);

	if(lh>0){
		h+=lh+30;
	}else{
		h+=50;
	}
	if(h<170)	h=170;

	w+=190;
	h+=40;
	mWindow.InitPlaneWindow(320-w/2,240-h/2,w,h);
}
/////////////////////////////////////////////////////////////////////////////////
CEventSkipMenu::CEventSkipMenu(int param)
{	
	mList.InitYesNoWindow(320-50,250,100);
	mTitle.InitPlaneWindow(320-100,180,200,70);
}
/////////////////////////////////////////////////////////////////////////////////

CSkillHelp::CSkillHelp(int param)
{
	mSkillNo=param;

	char fname[256];
	sprintf(fname,"data\\help\\skill%02d",mSkillNo/32);
	mText=GetHelpText(fname,mSkillNo);

	int w,h;
	CPlaneWindow::GetMessageSize(0,mText,&w,&h);

	if(w<240)	w=240;
	w+=50;
	h+=90;

	mWindow.InitPlaneWindow(320-w/2,240-h/2,w,h);

	Cursor.Escape();
}
/////////////////////////////////////////////////////////////////////////////////

CComSkillHelp::CComSkillHelp(int param)
{
	mSkillNo=param;

	char fname[256];
	sprintf(fname,"data\\help\\comskill%02d",mSkillNo/32);
	mText=GetHelpText(fname,mSkillNo);

	int w,h;
	CPlaneWindow::GetMessageSize(0,mText,&w,&h);

	if(w<240)	w=240;
	w+=50;
	h+=100;

	mWindow.InitPlaneWindow(320-w/2,240-h/2,w,h);

	Cursor.Escape();
}
/////////////////////////////////////////////////////////////////////////////////
CShopMenu::CShopMenu()
{
}
CShopMenu::CShopMenu(int param)
{
	Init(param);
	InitList();
	GetHelp();
}
void	CShopMenu::Init(int param)
{
	mShopNo=param&0xff;
	mShopMode=0;
	mHelp=NULL;

	mTalk.InitPlaneWindow(40,30,560,50);
	mInfo.InitPlaneWindow(40,80+ITEM_Height*8+32,320,ITEM_Height*5);

	mSWindow.Init(360,80,(!(param&0x100)),(!(param&0x200)));
	mSelectMember=mSWindow.GetCharacter(0);
}
void	CShopMenu::InitList(void)
{
	int itemno=0;
	int list[MAX_MONTH * MAX_SHOP_ITEM];

	for(int j=0;j<MAX_SHOP_ITEM;j++){
		if(ShopData[mShopNo][Flag.mMonth-1][j]>=0){
			list[itemno]=ShopData[mShopNo][Flag.mMonth-1][j];
			itemno++;
		}else{
			break;
		}
	}
	int newitem=itemno;
	for(int i=Flag.mMonth-2;i>=0;i--){
		for(int j=0;j<MAX_SHOP_ITEM;j++){
			if(ShopData[mShopNo][i][j]>=0){
				list[itemno]=ShopData[mShopNo][i][j];
				itemno++;
			}else{
				break;
			}
		}
	}

	bool change=true;

	while(change){
		change=false;
		for(int i=newitem;i<itemno-1;i++){
			if(list[i]>list[i+1]){
				int n=list[i];
				list[i]=list[i+1];
				list[i+1]=n;
				change=true;
			}
		}
	}

	mList.InitListWindow(	40,80,320,
							(const CElement**)AllItem,
							list,
							itemno,
							8);
		
	for(int i=0;i<newitem;i++){
		mList.SetParam(i,8+512);
	}
	for(int i=newitem;i<itemno;i++){
		mList.SetParam(i,8);
	}
}
/////////////////////////////////////////////////////////////////////////////////
CFleaMarketMenu::CFleaMarketMenu(int param)
{
	Init(param);
	InitList();
	GetHelp();
}
void CFleaMarketMenu::InitList()
{
	static int list[FLEA_MARKET_ITEM_MAX];
	int itemno=FLEA_MARKET_ITEM_MAX;

	for(int i=FLEA_MARKET_ITEM_MAX-1;i>=0;i--){
		list[i]=Flag.mFleaMarketList[i];
		if(Flag.mFleaMarketList[i]!=NO_ITEM)	Flag.mMarketNew[Flag.mFleaMarketList[i]]=false;
	}


	mList.InitListWindow(	40,80,320,
							(const CElement**)AllItem,
							list,
							itemno,
							8);
		
	for(int i=0;i<itemno;i++){
		if(Flag.mFleaMarketNew[i]){
			mList.SetParam(i,8+512);
		}else{
			mList.SetParam(i,8);
		}
	}
}
/////////////////////////////////////////////////////////////////////////////////
CFarmMenu::CFarmMenu(int param)
{
	mText=NULL;
	mSelectMember=0;
	mMode=0;
	
	mCDataMove=!(param&1);
	if(mCDataMove){
		for(int i=0;i<MAX_PARTY_NUMBER;i++){
			CMenuData::mCDataWindow[i].InitWindow(	650,40,
										Flag.mParty[i]);
		}
	}

	mList.InitListWindow(5,30,320,(const CElement**)AllItem,NULL,0,12);
	mHelp.InitPlaneWindow(5,62+ITEM_Height*12,320,70);
	mSWindow.Init(315,40,0,false);
	InitPlayerItem();

//草の初期化
	for(int i=0;i<MAX_CHARACTER_NUMBER;i++){
		bool b=false;
		for(int j=0;j<MAX_PARTY_NUMBER;j++){
			if(i==Flag.mParty[j]){
				b=true;
				break;
			}
		}
		if(b)	continue;

		int itemno=Flag.mPlayerData[i].GetUseItemMax();
		int start=Flag.mPlayerData[i].GetEquipMax();
		for(int j=0;j<itemno;j++){
			Flag.mPlayerData[i].SetItem(j+start,NO_ITEM);
		}
	}
	for(int i=ID_UI;i<ID_NSUI;i++){
		Flag.mCloset[i]=0;
	}

	UpdataList();
	GetHelp();
}
void CFarmMenu::SetPlayerItemCursor()
{
	for(int i=0;i<mPlayerItem.GetListSize();i++){
		if(mPlayerItem.GetItem(i)->mID==NO_ITEM){
			mPlayerItem.SetCursor(i);
			break;
		}
	}
}
void CFarmMenu::InitPlayerItem()
{
	int maxitem=Flag.mPlayerData[Flag.mParty[mSelectMember]].GetUseItemMax();
	mPlayerItem.InitListWindow(	315,140,ITEM_LIST_WIDTH,
								(const CElement**)AllItem,NULL,
								0,maxitem);
	UpdataPlayerItem();
	SetPlayerItemCursor();
}
void CFarmMenu::UpdataPlayerItem(void)
{
	CPlayerData &cdata=Flag.mPlayerData[Flag.mParty[mSelectMember]];		
	int start=cdata.GetEquipMax();
	int maxitem=cdata.GetUseItemMax();
	int itemlist[MAX_ITEM];

	for(int i=0;i<maxitem;i++){
		CarryItem citem = cdata.GetItem(i+start);
		itemlist[i]=citem.item;
	}
	
	mPlayerItem.ResetList((const CElement**)AllItem,itemlist,maxitem);
	GetHelp();
}
void CFarmMenu::UpdataList()
{
	static int list[MAX_SEED_NUMBER];
	int itemno=0;

	for(int i=0;i<MAX_SEED_NUMBER;i++){
		if(Flag.mFarm[i]>0){
			list[itemno]=i+ID_UI;
			itemno++;
		}
	}

	mList.ResetList((const CElement**)AllItem,list,itemno);
		
	for(int i=0;i<itemno;i++){
		mItemNo[i]=Flag.GetUsedFarmItem(list[i]);
		mList.SetParam(i,16+0x1000000*mItemNo[i]);
	}
}
void CFarmMenu::GetHelp()
{
	int item=NO_ITEM;
	if(mMode==0){
		item=mList.GetCursorItem()->mID;
	}else if(mMode==1){
		item=mPlayerItem.GetCursorItem()->mID;
	}
	delete[] mText;
	mText=CMenuData::GetItemTextShort(item,true);
}
/////////////////////////////////////////////////////////////////////////////////
CSeedMenu::CSeedMenu(int param)
{
	mSeedWindow.InitPlaneWindow(80,100,240,40);
	mWindow.InitPlaneWindow(80,140,480,240);
	mHelp=NULL;
	if(!CheckSeed())	PlayWave(SE_ERROR);
}
bool CSeedMenu::CheckSeed(void)
{
	mSeedNo=0;
	mNewItem=-1;
	for(int i=0;i<MAX_SEED_NUMBER;i++){
		int no=Flag.GetAllItem(ID_SD+i,false,2);
		if(no>0){
			mSeedNo+=no;
			if(mNewItem<0){
				Flag.mFarm[i]+=no;
				mNewItem=i;
				mNewItemNo=no;
				Flag.GetAllItem(ID_SD+i,true,2);
			}
		}
	}
	if(mNewItem>=0){
		mTimer=0;
		delete[] mHelp;
		mHelp=GetItemTextShort(mNewItem+ID_UI,true);
		return true;
	}
	mTimer=65535;
	return false;
}
/////////////////////////////////////////////////////////////////////////////////
CSellMenu::CSellMenu(int param)
{
	Init(param);
	InitList();
//	GetHelp();
}
void CSellMenu::Update(void)
{
	UpdataList();
	GetHelp();
}
void CSellMenu::InitList()
{
	InitItemList(mSWindow.GetCharacter(0),&mList,40,80,8);

	int itemno=mList.GetListSize();
	for(int i=0;i<itemno;i++){
		int p=mList.GetParam(i);
		mList.SetParam(i,64+(p&1));
	}
}

void	CSellMenu::ChangeCharacter(void)
{
	UpdataList();
	mList.SetCursor(0);
}
void	CSellMenu::UpdataList(void)
{
	if(mSelectMember>=ID_CLOSET){
		UpdataClosetList(&mList,1,false);
	}else{
		UpdataItemList(mSelectMember,&mList);
	}

	int itemno=mList.GetListSize();
	for(int i=0;i<itemno;i++){
		int p=mList.GetParam(i);
		mList.SetParam(i,64+(p&1));
	}
}

/////////////////////////////////////////////////////////////////////////////////
CSellYesNoMenu::CSellYesNoMenu(int param)
{
	mCharacter=param/0x10000;
	mItemNo=param&0xffff;
	mTimer=0;
	mMoveTimer=0;

	int h;
	if(mCharacter<MAX_CHARACTER_NUMBER){
		mItemID=Flag.mPlayerData[mCharacter].GetItem(mItemNo).item;
		mSellNo=0;		
		if(mItemID > ID_UI && (UseItemData[mItemID-ID_UI].mType&TYPE_COLLECT)){
			mSellMax=Flag.mPlayerData[mCharacter].GetItem(mItemNo).param;
			if(mSellMax>1){
				mSellNo=1;
			}
		}
	}else{
		mItemID=mItemNo;
		mSellNo=1;
		mSellMax=Flag.mCloset[mItemNo];
	}

	if(mSellNo>0){
		h=160;
	}else{
		h=100;
	}
	if(mItemID==NO_ITEM){
		PlayWave(SE_ERROR);
		Destroy();
		return;
	}

	mList.InitYesNoWindow(320-50,280,100);
	mTitle.InitPlaneWindow(320-180,280-h,340,h);
}
/////////////////////////////////////////////////////////////////////////////////

CReturnVillage::CReturnVillage(int param)
{
	mList.InitYesNoWindow(320-50,250,100);
	mTitle.InitPlaneWindow(320-80,180,160,70);
}
/////////////////////////////////////////////////////////////////////////////////

CDiaryMenu::CDiaryMenu(int param)
{
	mLoad=(param&0x01);
	mSuccess=((param & 0x02)!=0);
	mFileNo=-1;

	if(mSuccess){
		mSaveList[0].Init(-1,-1,0,0,0);
		mData[0]=true;
	}
	for(int i=0;i<SAVE_DATA_NO;i++){
		LoadSaveDataInfo(i+(mSuccess?1:0),i);
	}


	mList.InitListWindow(	60,80,400,
							mSaveList,NULL,
							SAVE_DATA_NO+(mSuccess?1:0),7,sizeof(CSaveDataItem));


	if(!(param & 0x02)){
		mList.SetCursor(GlobalFlag.mLastSave+4);
		mList.SetCursor(GlobalFlag.mLastSave);
	}

	mTitle.InitPlaneWindow(40,40,200,40);
}
void CDiaryMenu::LoadSaveDataInfo(int no,int fno)
{
	FILE* pFile;
	static char fname[256];
	sprintf(fname,"save\\save%02d",fno);
	if(pFile=fopen(fname,"rb")){
		_getw(pFile);
		int month=_getw(pFile);
		int time=_getw(pFile)/60;
		int mode=_getw(pFile);
		int cl=_getw(pFile);
		mSaveList[no].Init(fno,month,mode,time,cl);
		fclose(pFile);
	}else{
		mSaveList[no].Init(fno,0,0,0,0);
	}
	mData[no]=(mSaveList[no].mMonth!=0);
}
/////////////////////////////////////////////////////////////////////////////////
CClosetMenu::CClosetMenu(int param)
{
	mCharacter=-1;
	mCategory = 0;

	InitItemList(Flag.mParty[0],&mItemList,LIST_X_2-10,LIST_Y); 
	InitClosetList(&mClosetList,630,45,mCategory,15,true,false);
	mCategoryWindow.InitSelectWindow(
		630,8,320,
		ItemCategory,NULL,
		CATEGORY_MAX,sizeof(CItemCategoryData));
	mSWindow.Init(LIST_X_1+10,LIST_Y-10,true,false);
	mHelpWindow.InitWindow(HELP_X_1,HELP_Y_1,NO_ITEM);
	mEquipWindow.InitWindow(HELP_X_1,LIST_Y,0,&Flag.mPlayerData[0]);

	mSelect=-1;
}
void CClosetMenu::Update()
{
	if(mCharacter>=0){
		UpdataItemList(mCharacter,&mItemList);
	}
	UpdataClosetList(&mClosetList,1<<mCategory); 
}
/////////////////////////////////////////////////////////////////////////////////

CMonthStartMenu::CMonthStartMenu(int param)
{
	static const CItem ele[MENU_NO]={
		CItem("準備",1,0),
		CItem("メンバー変更",2,0),
		CItem("倉庫",3,0),
		CItem("農場",4,0),
		CItem("出発する！",0,0),
		CItem("ダンジョン情報",5,0),
		CItem("セーブ",6,0),
	};
	int list[MENU_NO]={0,1};
	int msize=2;
	if(Flag.mShopOK[5]){
		list[msize]=3;
		msize++;
	}
	if(Flag.GetDInfo(Flag.mCurrentMonth,true)){
		list[msize]=5;
		msize++;
	}
	list[msize]=6;
	list[msize+1]=4;
	msize+=2;

	mList.InitListWindow(80,120,160,ele,list,msize,msize,sizeof(CItem));

	for(int i=0;i<MAX_PARTY_NUMBER;i++){
		mCDataWindow[i].InitWindow(	280,
									i*CCharaDataWindowMenu::WIN_Height+40,
									Flag.mParty[i]);
	}
	InitReserveCDataWin();

	Flag.CheckRareItem();
	Flag.mClosetUse=true;
	mTempMapNo=-1;
}
void CMonthStartMenu::Update(void)
{
	for(int i=0;i<MAX_PARTY_NUMBER;i++){
		mCDataWindow[i].BaseMove(	280,
									i*CCharaDataWindowMenu::WIN_Height+40);
	}
	InitReserveCDataWin();
	Flag.mClosetUse=true;
	if(mTempMapNo>=0){
		ChangeMapInfo.mNo=mTempMapNo;
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////
CChangeMemberMenu::CChangeMemberMenu(int param)
{
	Flag.CheckReserve();
	mTop=!(param&1);
	mReserveNo=0;
	bool change=false;
	for(int i=0;i<MAX_PARTY_NUMBER;i++){
		if(MonthData[Flag.mCurrentMonth].mParty[i]==-1){
			change=true;
			break;
		}
	}

	if(!change){
		PlayWave(SE_ERROR);
		Destroy();
		return;
	}

	for(int i=0;i<MAX_PARTY_NUMBER;i++){
		if(mTop){
			CMenuData::mCDataWindow[i].InitWindow(0,0,Flag.mParty[i]);
		}
		int x;
		if(MonthData[Flag.mCurrentMonth].mParty[i]!=-1){
			x=-80;
		}else{
			x=0;
			change=true;
		}
		CMenuData::mCDataWindow[i].Move(x,i*CCharaDataWindowMenu::WIN_Height+40);
	}
	InitCDWin();
	mChange=-1;
	mCursor=-1;
	MoveCursor(1);
}
void	CChangeMemberMenu::InitCDWin(void)
{
	mReserveNo=0;

	mCDataWindow[mReserveNo].InitWindow(640,0,-1);
	mReserve[mReserveNo]=-1;
	mReserveNo++;
	for(int i=0;i<MAX_CHARACTER_NUMBER;i++){
		int character=Flag.mParty[ i + MAX_PARTY_NUMBER ];
		if(character<0)	continue;
		mChangeOk[mReserveNo]=true;
		for(int j=0;j<4;j++){
			if(MonthData[Flag.mCurrentMonth].mImpossible[j]==character){
				mChangeOk[mReserveNo]=false;
				break;
			}
		}
		mCDataWindow[mReserveNo].InitWindow(640,0,character);
		mReserve[mReserveNo]=character;
		mReserveNo++;
	}

	if(mReserveNo>1)	mSelect=1;
	else				mSelect=0;
}
void	CChangeMemberMenu::MoveCDWin(int cursor)
{
	for(int i=0;i<mReserveNo;i++){
		double rad=(i-cursor)*0.25;
		double s=sin(rad);
		double c=cos(rad);
		mCDataWindow[i].Move(int(720-c*400),int(180+s*400));
	}
}
void	CChangeMemberMenu::MoveCursor(int add)
{
	do{
		mCursor+=add;
		mCursor%=MAX_PARTY_NUMBER;
		if(mCursor<0)	mCursor+=MAX_PARTY_NUMBER;
	}while(MonthData[Flag.mCurrentMonth].mParty[mCursor]!=-1);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////
CBattleStatusMenu::CBattleStatusMenu(int param)
{
	if(param & 0x100){
		mChangeable=false;
		mCharacterNum=0;
	}else{
		mChangeable=true;
		mCharacterNum=param&0xff;
	}
	mListType=3;
	int y=Init(mCharacterNum);
	if(y>0){
		mListTitle.InitPlaneWindow(300,30,ITEM_LIST_WIDTH,40);
		mListHelp.InitPlaneWindow(300,y,ITEM_LIST_WIDTH,420-y);
	}
}
int CBattleStatusMenu::Init(int param)
{
	int add;
	switch(param/0x100){
		case 1:		add=1;	break;
		case 2:		add=-1;	break;
		default:	add=0;	break;
	}

	if(!mChangeable){
		mCharacter=CBattleCharacter::mHelpTarget;
	}else{
		if(add){
			int max=CHARACTER_NO;
			for(int i=0;i<max;i++){
				mCharacterNum+=add;
				if(mCharacterNum>=max)	mCharacterNum=0;
				if(mCharacterNum<0)		mCharacterNum=max-1;

				mCharacter=BattleMode.GetCharacter(mCharacterNum);
				if(mCharacter->GetCData() && !(mCharacter->GetDead())){
					break;
				}
			}
		}
		mCharacter=BattleMode.GetCharacter(mCharacterNum);
	}

	mPlayer=NULL;
	mDelete=false;

	mWindow.InitPlaneWindow(20,30,280,390,false);
	mListType--;
	ChangeList();
	return mList.GetY()+mList.GetHeight();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////
CConfigMenu::CConfigMenu(int param)
{
	static const CLargeItem ele[MENU_NO+1]={
		CLargeItem("ＢＧＭの再生",0,0),
		CLargeItem("効果音の再生",1,0),
		CLargeItem("カーソルの記憶(戦闘中)",2,0),
		CLargeItem("戦闘スピード",6,0),
		CLargeItem("マップ移動の角度",4,0),
		CLargeItem("通常の移動方法",7,0),
		CLargeItem("フルスクリーン",5,0),
		CLargeItem("ゲームパッドの設定",3,0),
	};
	mAll.InitListWindow(WIN_X,WIN_Y,WIN_W,ele,NULL,MENU_NO+1,MENU_NO+1,sizeof(CLargeItem));
	mHelp.InitPlaneWindow(WIN_X,WIN_Y + mAll.GetHeight(),WIN_W,HELP_H);


	static const CElement select[MENU_NO][4]={
		{
			CElement("再生する",1),
			CElement("再生しない",0),
		},
		{
			CElement("再生する",1),
			CElement("再生しない",0),
		},
		{
			CElement("全て記憶",2),
			CElement("カテゴリのみ記憶",1),
			CElement("記憶しない",0),
		},
		{
			CElement("テキパキ",1),
			CElement("まったり",0),
		},
		{
			CElement("左に45°",1),
			CElement("右に45°",0),
		},
		{
			CElement("ダッシュ",1),
			CElement("歩き",0),
		},
		{
			CElement("フルスクリーン",1),
			CElement("ウインドウ",0),
		},
	};

	static const int selectno[MENU_NO]={2,2,3,2,2,2,2};
	for(int i=0;i<MENU_NO;i++){
		mSelectWindow[i].InitSelectWindow(
						WIN_X+250,
						WIN_Y+20+ele[0].GetHeight()*i,
						WIN_W-300,
						select[i],NULL,selectno[i],sizeof(CElement));
	}
	mSelectWindow[0].SetCursor((Flag.mBGM?0:1));
	mSelectWindow[1].SetCursor((Flag.mSE?0:1));
	mSelectWindow[2].SetCursor(2-Flag.mUseSaveCursor);
	mSelectWindow[3].SetCursor((Flag.mBattleSpeed?0:1));
	mSelectWindow[4].SetCursor((Flag.mMapMoveTurn?0:1));
	mSelectWindow[5].SetCursor((Flag.mAutoDash?0:1));
	mSelectWindow[6].SetCursor((GlobalFlag.mFullScreen?0:1));
}
//////////////////////////////////////////////////////////////////////////////////////////////////////
CFreeSpaceMenu::CFreeSpaceMenu(int param)
{
	mCharacter=param;
	mHelp=NULL;

	int chara=Flag.mParty[mCharacter];
	const CElement* skill[MAX_FREE_SPACE];
	mFreeSpace=Flag.mPlayerData[chara].SearchASkillType(AS_FREE_SPACE,true);

	int i;
	for(i=0;i<mFreeSpace;i++){
		int s=Flag.mPlayerData[chara].GetFSpace(i);
		if(s>=0){
			skill[i]=&SkillData[s];
		}else{
			skill[i]=&NoneSkillData;
		}
	}

	mLearnSkill.InitListWindow(	0,20,320,
								skill,NULL,
								mFreeSpace,MAX_SKILL_MONTH,
								NULL);
	mLearnSkill.InitPlaneWindow(300,CBirthStoneSystem::MONTH_LIST_Y+100,
								320,SKILL_Height*MAX_SKILL_MONTH+32,true);
	mLearnSkill.Move(160,CBirthStoneSystem::MONTH_LIST_Y+100);

	for(int i=0;i<MAX_PARTY_NUMBER;i++){
		char filename[256];
		sprintf(filename,"data\\mapobject\\mapobject%03d",Flag.mParty[i]);
		DTest.LoadTexture(mTex[i],filename,0xff008080);
	}

	MoveCursor(0);
}
void	CFreeSpaceMenu::Update(void)
{
	int chara=Flag.mParty[mCharacter];
	const CElement* skill[MAX_FREE_SPACE];
	mFreeSpace=Flag.mPlayerData[chara].SearchASkillType(AS_FREE_SPACE,true);

	int i;
	for(i=0;i<mFreeSpace;i++){
		int s=Flag.mPlayerData[chara].GetFSpace(i);
		if(s>=0){
			skill[i]=&SkillData[s];
		}else{
			skill[i]=&NoneSkillData;
		}
	}
	mLearnSkill.ResetList(skill,NULL,mFreeSpace);

	MoveCursor(mLearnSkill.GetCursor());
}
//////////////////////////////////////////////////////////////////////////////////////////////////////
CFSBirthStoneMenu::CFSBirthStoneMenu(int param)
{
	mFreeSpaceCharacter	=param&0x00ff;
	mFreeSpace			=param/0x0100;
	mBirthStoneSystem.Init(mFreeSpaceCharacter,mFreeSpace);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
CFSSelectSkillMenu::CFSSelectSkillMenu(int param)
:CLearnSkillMenu(param&0xffff)
{
	param=param>>16;
	mFreeSpaceCharacter	=param&0x00ff;
	mFreeSpace			=param/0x0100;

	mCompare.InitWindow(0,Y,&Flag.mPlayerData[Flag.mParty[mFreeSpaceCharacter]],&mAfter);
	mMessageWindow[0].QuickDestroy();
	mMessageWindow[1].QuickDestroy();

	mOldSkill.InitPlaneWindow(320,Y,320,45);

	MoveCursor(0);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////
CGlobalConfigMenu::CGlobalConfigMenu(int param)
{
	static const CLargeItem ele[2]={
		CLargeItem("フルスクリーン",0,0),
		CLargeItem("ゲームパッドの設定",1,0),
	};
	mAll.InitListWindow(WIN_X,WIN_Y,WIN_W,ele,NULL,2,2,sizeof(CLargeItem));


	static const CElement select[2]={
		CElement("する",1),
		CElement("しない",0),
	};

	mSelectWindow.InitSelectWindow(
					WIN_X+220,
					WIN_Y+20,
					WIN_W-250,
					select,NULL,2,sizeof(CElement));
	mSelectWindow.SetCursor((GlobalFlag.mFullScreen?0:1));
}
//////////////////////////////////////////////////////////////////////////////////////////////////////
CKeyConfigMenu::CKeyConfigMenu(int param)
{
	static const CKeyConfigItem ele[MAX_KEY-4]={
		CKeyConfigItem("決定・調べる"			,4,0),
		CKeyConfigItem("メニュー・ヘルプ"		,5,0),
		CKeyConfigItem("キャンセル・ダッシュ"	,6,0),
	};
	static const CElement ret("終了",-1);
	static const CElement *list[MAX_KEY-4+1];
	for(int i=0;i<MAX_KEY-4;i++){
		list[i]=&ele[i];
	}
	list[MAX_KEY-4]=&ret;
	mList.InitListWindow(WIN_X,WIN_Y,WIN_W,list,NULL,MAX_KEY-3,MAX_KEY-3);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////
CDInfoMenu::CDInfoMenu(int param)
{
	mMonth=param;

	if(mMonth<0 || mMonth>=MAX_ALL_MONTH) {
		Destroy();
		return;
	}

	CMonthData& mdata=MonthData[mMonth];

	mEnemy=new CEnemyCharacter[mdata.mEnemyNo];
	for(int i=0;i<mdata.mEnemyNo;i++){
		mEnemy[i].LoadCharacter(mdata.mEnemy[i],mdata.GetBaseLv(),0,0,0);
		mEnemy[i].SetID(-1);
	}

	memset(mKey,0,sizeof(mKey));
	mOpenTresure=0;
	for(int i=0;i<mdata.mTresureNo;i++){
		int& t=mdata.mTresure[i];
		if(Flag.mTreasure[t]){
			mOpenTresure++;
		}else{
			for(int j=0;j<MAX_KEY_TYPE;j++){
				if(TreasureData[t].mKey & (1<<j)){
					mKey[j]++;
				}
			}
		}
	}

	mWindow.InitPlaneWindow(20,20,610,420);
	if(mdata.mEnemyNo>0){
		mList.InitListWindow(280,70,340,mEnemy,NULL,mdata.mEnemyNo,14,sizeof(CEnemyCharacter));
		for(int i=0;i<mdata.mEnemyNo;i++){
			mList.SetParam(i,2);
		}
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////

CBypathMenu::CBypathMenu(int param)
{
	mList.InitYesNoWindow(30,50,200);
	mTitle.InitPlaneWindow(20,10,160,40);

	
	static const CItem ele[MENU_NO]={
		CItem("セーブ"			,0,0),
		CItem("抜け道で村に帰る",1,0)
	};


	mList.InitListWindow(30,50,220,ele,NULL,MENU_NO,MENU_NO,sizeof(CItem));
}
/////////////////////////////////////////////////////////////////////////////////
CStatusHelp::CStatusHelp(int param)
{
	mStatusNo=param;
	mText=GetHelpText("data\\help\\status",mStatusNo);

	int w,h;
	CPlaneWindow::GetMessageSize(0,mText,&w,&h);

	if(w<240)	w=240;
	w+=50;
	h+=90;

	mWindow.InitPlaneWindow(320-w/2,240-h/2,w,h);

	Cursor.Escape();
}
/////////////////////////////////////////////////////////////////////////////////
CRemoveBirthStoneMenu::CRemoveBirthStoneMenu(int param)
{
	mErrorTimer=0;

	static const CRemoveBirthStoneEle ele[MAX_CHARACTER_NUMBER]=
	{
		0,1,2,3,4,5,6,7,8,9,10,11,12,13,14
	};
	int no=0;
	int member[MAX_CHARACTER_NUMBER];

	for(int i=0;i<MAX_CHARACTER_NUMBER;i++){
		if(Flag.mParty[i]>=0){
			member[no]=Flag.mParty[i];
			no++;
		}
	}

	int h=no;
	if(h>8)	h=8;
	mSWindow.InitListWindow(40,60,540,ele,member,no,h,sizeof(CRemoveBirthStoneEle));

	mWindow.InitPlaneWindow(20,20,320,40);
}
/////////////////////////////////////////////////////////////////////////////////
CRemoveBSPersonal::CRemoveBSPersonal(int param)
{
	static const CElement ele[4]={
		CElement("全て取り外す",0),
		CElement("「★」のみ取り外す",1),
		CElement("月を選んで取り外す",2),
		CElement("終了",3)
	};
	mCharacter=param;
	mMode=0;
	
	mList.InitListWindow(	40,MONTH_LIST_Y+100,300,
								ele,NULL,
								4,4,
								sizeof(CElement),NULL);

	mCDataWin.InitWindow(30,MONTH_LIST_Y,mCharacter);
	
	InitMonthList();
	mSkillList.InitPlaneWindow(	10-320,MONTH_LIST_Y+100,
								320,SKILL_Height*MAX_SKILL_MONTH+32);
}

void CRemoveBSPersonal::InitMonthList(void)
{
	static const CBirthStone bstone[MAX_MONTH]={
		CBirthStone("1月",0),
		CBirthStone("2月",1),
		CBirthStone("3月",2),
		CBirthStone("4月",3),
		CBirthStone("5月",4),
		CBirthStone("6月",5),
		CBirthStone("7月",6),
		CBirthStone("8月",7),
		CBirthStone("9月",8),
		CBirthStone("10月",9),
		CBirthStone("11月",10),
		CBirthStone("12月",11),
	};
	int list[MAX_MONTH];
	for(int i=0;i<MAX_MONTH;i++){
		list[i]=i;
	}
	int	skillspace[MAX_MONTH];
	Flag.mPlayerData[mCharacter].GetSkillSpace(skillspace);

	int length=1;
	if(Flag.SearchFSkill(313)){
		length=MAX_MONTH-1;
	}else{
		for(;length<MAX_MONTH;length++){
			if(skillspace[length]<=0)	break;
		}
	}

	mMonthList.InitListWindow(	MONTH_LIST_X,MONTH_LIST_Y,CBirthStoneSystem::MONTH_LIST_WIDTH,
								bstone,list,
								length,length,
								sizeof(CBirthStone),NULL);
	ResetMonthList();
}
void CRemoveBSPersonal::GetBSParam(int character,int* param)
{
	CPlayerData &cdata=Flag.mPlayerData[character];
	memset(param,0,sizeof(int)*MAX_MONTH);

	int	skillspace[MAX_MONTH];
	cdata.GetSkillSpace(skillspace);

	for(int i=0;i<MAX_MONTH-1;i++){
		int learn=cdata.GetLearnSkillNo(i);
		int star =(cdata.CheckLearnSkill(i,MAX_SKILL_MONTH-1)?1:0);
		param[i]=skillspace[i]+learn*8+star*128;
		if(skillspace[i]<=0){
			break;
		}
	}
}
void CRemoveBSPersonal::ResetMonthList(void)
{
	CPlayerData &cdata=Flag.mPlayerData[mCharacter];
	int param[MAX_MONTH];
	GetBSParam(mCharacter,param);

	mMonthList.SetParam(param);
	InitSkillList();
}

void CRemoveBSPersonal::InitSkillList()
{
	int month=mMonthList.GetCursorItem()->mID;
	int chara=mCharacter;
	int learn[MAX_LEARN_MONTH];

	int i;
	for(i=0;i<MAX_SKILL_MONTH;i++){
		mSkill[i]=CharacterSkill[chara][month][i];
	}

	Flag.mPlayerData[chara].GetLearnSkill(month,learn);
	memset(mLearn,0,sizeof(mLearn));
	for(i=0;i<MAX_LEARN_MONTH;i++){
		if(0<learn[i] && learn[i]<=MAX_SKILL_MONTH){
			mLearn[learn[i]-1]=1;
		}
	}
	mLearn[MAX_SKILL_MONTH-1]+=2;
	
}
/////////////////////////////////////////////////////////////////////////////////
CSelectDifficult::CSelectDifficult(int param)
{
	static const CSelectDifficultItem ele[2]={
		CSelectDifficultItem("Normal",0,0),
		CSelectDifficultItem("Hard",1,0)
	};

	mList.InitListWindow(80,80,480,ele,NULL,2,2,sizeof(CSelectDifficultItem));

	mTitle.InitPlaneWindow(80,30,320,50);
	mHelp.InitPlaneWindow(80,80+mList.GetHeight(),480,160);
}
/////////////////////////////////////////////////////////////////////////////////
CExitYesNo::CExitYesNo(int param)
{
	mList.InitYesNoWindow(320-50,250,100);
	mTitle.InitPlaneWindow(320-120,170,240,80);

	mList.SetCursor(1);
}
/////////////////////////////////////////////////////////////////////////////////
CSuccessSaveDataMenu::CSuccessSaveDataMenu(int param)
:CDiaryMenu(3)
{
	mHelp.InitPlaneWindow(20,10,480,130);

	mList.Move(160,110);
	mTitle.QuickDestroy();
}
/////////////////////////////////////////////////////////////////////////////////
CCallPetHelp::CCallPetHelp(int param)
{
	mSkillNo=param;
	mPetNo=param-117;	//マジックナンバー！


	char fname[256];
	sprintf(fname,"data\\help\\comskill%02d",mSkillNo/32);
	mText=GetHelpText(fname,mSkillNo);

	int w,h;
	CPlaneWindow::GetMessageSize(0,mText,&w,&h);

	if(w<240)	w=240;
	h+=70;

	const CElement* ele[LIST_MAX];
	const CPetData & pdata=PetData[mPetNo];
	int size=pdata.mSkillNo;
	for(int i=0;i<pdata.mSkillNo;i++){
		int &skill=pdata.mSkill[i];
		ele[i]=&(SkillData[pdata.mSkill[i]]);
	}
	int lh=InitList(ele,size,h,w);

	if(lh>0){
		h+=lh+30;
	}else{
		h+=50;
	}
	if(h<210)	h=210;

	w+=190;
	mWindow.InitPlaneWindow(320-w/2,240-h/2,w,h);
}
/////////////////////////////////////////////////////////////////////////////////
CTutorialMenu::CTutorialMenu(int param)
{
	const int w=600;
	const int h=320;

	mNo=param;
	
	char fname[256];
	sprintf(fname,"data\\tutorial\\tutorial%02d",mNo);
	FILE *pFile=fopen(fname,"r");

	if(pFile){
		fgets(mTitle,sizeof(mTitle),pFile);
		char no[8];
		fgets(no,sizeof(no),pFile);
		mMaxPage=atoi(no);

		fclose(pFile);
	}else{
		PlayWave(SE_ERROR);
		Destroy();
		return;
	}

	ChangePage(0);

	mWindow.InitPlaneWindow(320-w/2,240-h/2,w,h);
}
void CTutorialMenu::ChangePage(int page)
{
	mPage=page;

	char fname[256];
	sprintf(fname,"data\\tutorial\\tutorial%02d",mNo);
	FILE *pFile=fopen(fname,"r");
	if(pFile){
		char str[256];
		int page=-1;
		while(page<mPage){
			fgets(str,sizeof(str),pFile);
			if(str[0]=='*')	page++;
		}

		fgets(mSubTitle,sizeof(mTitle),pFile);
		fgets(str,sizeof(str),pFile);
		sprintf(fname,"data\\tutorial\\%02d",atoi(str));
		DTest.LoadTexture(mTex,fname,0);

		strcpy(mText,"");
		while(true){
			fgets(str,sizeof(str),pFile);
			if(str[0]=='*')	break;
			strcat(mText,str);
		}

		fclose(pFile);
	}else{
		strcpy(mSubTitle,"エラー：ファイルが存在しません。");
		mTex.Release();
	}
}
/////////////////////////////////////////////////////////////////////////////////
CSoundTestMenu::CSoundTestMenu(int param)
{
	const int LIST_X = 120;
	const int LIST_Y = 70;

	int itemno=0;
	int itemlist[BGM_MAX];

	for(int i=0;i<BGM_MAX;i++){
#ifndef _ALL_SOUND_TEST_
		if(GlobalFlag.mSound[i]){
#endif
			itemlist[itemno]=i;
			itemno++;
#ifndef _ALL_SOUND_TEST_
		}
#endif
	}

	bool turn=true;
	while(turn){
		turn=false;
		for(int i=1;i<itemno;i++){
			if(BGMData[itemlist[i]].mSort < BGMData[itemlist[i-1]].mSort){
				turn=true;
				int item=itemlist[i];
				itemlist[i]=itemlist[i-1];
				itemlist[i-1]=item;
			}
		}
	}

	for(int i=0;i<itemno;i++){
		if(BGMData[itemlist[i]].mSort >= NOT_PLAY){
			itemno=i;
			break;
		}
	}
	
	mBGMWindow.InitListWindow(	LIST_X,LIST_Y,300,
								(const CElement*)BGMData,itemlist,
								itemno,14,sizeof(CBGMData));
	mCurrent.InitPlaneWindow(LIST_X-80,LIST_Y-50,360,50);

	SetBGMFadeOut(20);
}
/////////////////////////////////////////////////////////////////////////////////

CPreciousItemMenu::CPreciousItemMenu(int param)
{
	mCategory = 0;
	mCategoryAll=0x3000;

	mHelpWindow.InitWindow(0,120,0);

	int clist[CATEGORY_MAX];
	int cnum=1;
	clist[0]=0;
	for(int i=0;i<CATEGORY_MAX;i++){
		if(mCategoryAll & (1<<(i))){
			clist[cnum]=i;
			cnum++;
		}
	}

	InitClosetList(&mClosetList,320,45,0,15,false,false);
	UpdataClosetList(&mClosetList,mCategoryAll,false); 

	mCategoryWindow.InitSelectWindow(
			320,8,320,
			ItemCategory,clist,
			cnum,sizeof(CItemCategoryData));

	ChangeHelp();
}

void CPreciousItemMenu::ChangeHelp()
{
	int item=mClosetList.GetCursorItem()->mID;
	mHelpWindow.ChangeItem(item);
}