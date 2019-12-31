#include "menusupport.h"
#include "flag.h"
#include "menudata.h"
#include "input.h"
#include "cursor.h"


CSelectCharacterSystem::CSelectCharacterSystem()
{
	mLive=false;
}
void CSelectCharacterSystem::Init(int x,int y,int character,bool reserve)
{
	mLive=true;

	mX=x;
	mY=y;
	mCharacter=character;
	mCursorTimer=0;
	mMoveTimer=0;

	SetReserve(reserve);
	CMenuData::mCDataWindow[mCharacter].Move(mX,mY);

}
void	CSelectCharacterSystem::Update()
{
	CMenuData::mCDataWindow[mCharacter].Move(mX,mY);
}
void	CSelectCharacterSystem::OnDraw()
{
	if(!mLive)	return;
	CCharaDataWindowMenu &win=CMenuData::mCDataWindow[mCharacter];
	win.OnDraw(true);

	int texturex;
	switch((mCursorTimer/CURSOR_ANI_TIME)%4){
		case 1:
			texturex=1;
			break;
		case 3:
			texturex=2;
			break;
		default:
			texturex=0;
	}

	int drawy=CCharaDataWindowMenu::WIN_Height*2/3-8+mY;
	if(mMoveTimer>0 && mLeft){
		DTest.DrawTexture(SystemTex,TURN_HORIZONTAL,160,32,32,16,mX-16,drawy);
	}else{
		DTest.DrawTexture(SystemTex,TURN_HORIZONTAL,64+texturex*32,32,32,16,mX-16,drawy);
	}

	if(mMoveTimer>0 && !mLeft){
		DTest.DrawTexture(SystemTex,0,160,32,32,16,mX+CCharaDataWindowMenu::WIN_WIDTH-16,drawy);
	}else{
		DTest.DrawTexture(SystemTex,0,64+texturex*32,32,32,16,mX+CCharaDataWindowMenu::WIN_WIDTH-16,drawy);
	}
}
int		CSelectCharacterSystem::OnTimer(bool cursor)
{
	if(!mLive)	return -1;
	mCursorTimer++;
	mMoveTimer--;

	if(cursor){
		if(pInput->GetKey(0)==1){
			PlayWave(SE_MOVE);
			mLeft=true;
			mMoveTimer=SWIN_MOVETIME;
			return ChangeCharacter(-1);
		}
		if(pInput->GetKey(2)==1){
			PlayWave(SE_MOVE);
			mLeft=false;
			mMoveTimer=SWIN_MOVETIME;
			return ChangeCharacter(1);
		}
	}
	return -1;
}

void	CSelectCharacterSystem::Destroy(void)
{
	if(!mLive)	return;
	CMenuData::mCDataWindow[mCharacter].Home();
	mLive=false;
}

void	CSelectCharacterSystem::SetReserve(bool rev)
{
	mReserve=rev;
	if(!mReserve && mCharacter>=MAX_PARTY_NUMBER)	ChangeCharacter(1);
}

int	CSelectCharacterSystem::ChangeCharacter(int change)
{
	CMenuData::mCDataWindow[mCharacter].Home();
	mCharacter=GetCharacter(change,1);
	CMenuData::mCDataWindow[mCharacter].Move(mX,mY);
	return mCharacter;
}
int		CSelectCharacterSystem::GetCharacter(int vector,int length)
{
	int i=mCharacter;
	int repeat=length;

	int max;
	if(mReserve){
		max=MAX_PARTY_NUMBER+MAX_CHARACTER_NUMBER;
	}else{
		max=MAX_PARTY_NUMBER;
	}
	do{
		if(Flag.mParty[i]>=0){
			if(repeat<=0)	return i;
			repeat--;
		}
		i+=vector;
		if(i>=max)	i=0;
		if(i<0)		i+=max;
	}while(mCharacter != i);

	return i;
}

///////////////////////////////////////////////////////////////////////////////////////////
CSelectCharacterShopSystem::CSelectCharacterShopSystem()
{
	mLive=false;
}
void CSelectCharacterShopSystem::Init(int x,int y,bool allmember,bool closet)
{
	mWindow.InitPlaneWindow(x,y,240,ITEM_Height*13+32);
	mLive=true;
	mCharacter=0;

	int i;

	mMemberNo=0;
	for(i=0;i<MAX_PARTY_NUMBER;i++){
		if(Flag.mParty[i]>=0){
			mMember[mMemberNo]=Flag.mParty[i];
			char filename[256];
			sprintf(filename,"data\\mapobject\\mapobject%03d",Flag.mParty[i]);
			DTest.LoadTexture(mMemberTex[mMemberNo],filename,0xff008080);

			mMemberNo++;
		}
	}

	if(allmember){
		for(i=0;i<MAX_CHARACTER_NUMBER;i++){
			int character=Flag.mParty[i+MAX_PARTY_NUMBER];
			if(character<0)	continue;
			mMember[mMemberNo]=character;
			char filename[256];
			sprintf(filename,"data\\mapobject\\mapobject%03d",character);
			DTest.LoadTexture(mMemberTex[mMemberNo],filename,0xff008080);

			mMemberNo++;
			if(mMemberNo>=MAX_CHARACTER_NUMBER)	break;
		}
	}
	if(closet){
		mMember[mMemberNo]=ID_CLOSET;
		mMemberNo++;
	}
}
int		CSelectCharacterShopSystem::OnTimer(bool cursor)
{
	mWindow.OnTimer();
	if(!cursor)	return -1;

	bool ret=false;
	int mnum=mMemberNo-1;

	int x=(mCharacter/8)*96+14 + mWindow.GetX();
	int y=(mCharacter%8)*44+34 + mWindow.GetY();
	Cursor.Move(x,y);

	int cmove=0;
	if(pInput->GetRepeatKey(1)==1){
		cmove--;
		PlayWave(SE_MOVE);
	}
	if(pInput->GetRepeatKey(3)==1){
		cmove++;
		PlayWave(SE_MOVE);
	}
	if(cmove){
		int n=mCharacter & 0x7;
		n+=cmove;
		if(n<0)	n=7;
		if(n>7)	n=0;
		mCharacter=(mCharacter & 0x8) + n;
		if(mCharacter>mnum){
			if(cmove>0){
				mCharacter=mCharacter & 0x8;
			}else{
				mCharacter=mnum;
			}
		}
		ret=true;
	}
	if(mnum>=8){
		if(pInput->GetRepeatKey(0)==1 || pInput->GetRepeatKey(2)==1){
			mCharacter=mCharacter^0x8;
			if(mCharacter>mnum)	mCharacter=mnum;
			PlayWave(SE_MOVE);
			ret=true;
		}
	}

	if(pInput->GetRepeatKey(5)==1){
		//ヘルプ呼び出し
		if(mMember[mCharacter]!=ID_CLOSET){
			MenuData.SetMenu(StatusMenu,mCharacter+64);
		}
	}

	if(ret){
		return mMember[mCharacter];
	}

	return -1;
}
void	CSelectCharacterShopSystem::OnDraw()
{
	mWindow.OnDraw();
	int x=(mCharacter/8)*96+16;
	int y=(mCharacter%8)*44+16;
	mWindow.DrawBox(x,y,96,16,COLOR_SELECT);

	int i;
	for(i=0;i<mMemberNo;i++){
		int x=(i/8)*96;
		int y=(i%8)*44-16;
		if(mMember[i]>=ID_CLOSET){
			int x=(i/8)*96;
			int y=(i%8)*44-16;
			mWindow.DrawTexture(&SystemTex,0,
								224,192,32,32,x+16,y+16,32,32);
			mWindow.DrawText(0,"倉庫",x+50,y+30);
			continue;
		}
		mWindow.DrawTexture(&mMemberTex[i],0,
							0,0,64,64,x,y,64,64);
		/*
		int max = Flag.mPlayerData[mMember[i]].GetItemMax();
		int cur = Flag.mPlayerData[mMember[i]].GetItemCurrent();
		D3DCOLOR color=0xff000000;
		if(max==cur)	color=0xffff0000;
		char text[64];
		sprintf(text,"%d",cur);
		mWindow.DrawText(0,text,x+50,y+30,color);
		sprintf(text,"/ %d",max);
		mWindow.DrawText(0,text,x+70,y+30);
		*/
	}
}
void	CSelectCharacterShopSystem::Destroy(void)
{
	mWindow.Destroy();

}

///////////////////////////////////////////////////////////////////////////////////////////

CBirthStone::CBirthStone(void)
{
}
CBirthStone::CBirthStone(const char* name,int id)
:CElement(name,id)
{
}
void	CBirthStone::Draw(CPlaneWindow* window,int x,int y,int param,D3DCOLOR color) const
{
	const int stonew=27;
	int icon;
	int stonex=70;
	int stonemax=MAX_LEARN_MONTH;
	int space=param%8;
	int learn=param%128/8;
	bool star=(param/128)>0;

	if(mID>=256){//フリースペース（未設定）
		icon=300;
		stonex=105;
		stonemax=space;
	}else{
		int item=ITEM_BIRTH_STONE+mID;
		icon=AllItem[item]->mIcon;
		char text[64];
		int no;
		D3DCOLOR color;
		window->DrawText(1,"/",180,y+14,0,DT_RIGHT);

		no=Flag.GetAllItem(item,false,2);
		color=(no==0 ? COLOR_GRAY : 0);
		sprintf(text,"%d",no);
		window->DrawText(1,text,187,y+12,color,DT_RIGHT);

		no=Flag.GetAllItem(item+13,false,2);
		color=(no==0 ? COLOR_GRAY : 0);
		sprintf(text,"%d",no);
		window->DrawText(1,text,172,y+16,color,DT_RIGHT);
	}

	window->DrawText(0,mName,x-8,y+10);

	int tx=icon%8*32;
	int ty=(icon/8)%8*32;

	for(int i=0;i<stonemax;i++){
		if(i<MAX_LEARN_MONTH-1){
			int size=0;
			if(i>=space){
				if(mID>=12)	break;
				size=12;
			}
			window->DrawTexture(&ItemTex[icon/64],0,tx,ty,32,32,x+stonex+i*stonew+size,y+size,32-size*2,32-size*2,0xd0404040);
		}else{
			int size=0;
			if(space<=0){
				size=12;
			}
			window->DrawTexture(&SystemTex,0,160,112,32,32,x+stonex+i*stonew+size,y+size,32-size*2,32-size*2,0xd0404040);
		}
	}
	for(int i=0;i<learn;i++){
		DrawStone(window,ItemTex[icon/64],x+stonex+i*stonew+1,y-1,tx,ty);
	}
	if(star){
		DrawStone(window,SystemTex,x+stonex+(MAX_LEARN_MONTH-1)*stonew,y,160,112);
	}
}
void CBirthStone::DrawStone(CPlaneWindow* window,CDTTexture& tex,int x,int y,int tx,int ty) const
{
	window->DrawTexture(&tex,0,tx,ty,32,32,x,y,32,32);
	int time=Flag.mPlayTime;
	D3DCOLOR color=(int)(0x18000000*(sin(time/16.0)+1))| 0x00ffffff;
	DTest.SetBlend(BLEND_SOLID_ADD);
	window->DrawTexture(&tex,0,tx,ty,32,32,x,y,32,32,color);
	DTest.SetBlend(BLEND_STANDARD);
}
////////////////////////////////////////////////////////////////////////////////////////////////

CBirthStoneSystem::CBirthStoneSystem()
{
	mLive=false;
}
void CBirthStoneSystem::Init(int character,int fs)
{
	Flag.CheckFreeSpace();

	mLive=true;
	mDeleteTimer=0;
	mCharacter=character;
	mSelectFS=fs;

	InitMonthList();
	mSWindow.Init(300,MONTH_LIST_Y,mCharacter,(fs<0));
	mSkillList.InitPlaneWindow(	300,MONTH_LIST_Y+100,
								320,SKILL_Height*MAX_SKILL_MONTH+32);
}
void CBirthStoneSystem::InitMonthList(void)
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
		CBirthStone("フリースペース",256),
	};
	int list[MAX_MONTH];
	for(int i=0;i<MAX_MONTH;i++){
		list[i]=i;
	}
	int	skillspace[MAX_MONTH];
	Flag.mPlayerData[Flag.mParty[mCharacter]].GetSkillSpace(skillspace);

	int length=1;
	mSenken=Flag.SearchFSkill(313,true);
	if(mSenken){
		length=MAX_MONTH-1;
	}else{
		for(;length<MAX_MONTH;length++){
			if(skillspace[length]<=0)	break;
		}
	}

	mFreeSpace=Flag.mPlayerData[Flag.mParty[mCharacter]].SearchASkillType(AS_FREE_SPACE,true);
	if(mFreeSpace>0){
		list[length]=12;
		length++;
	}
	mMonthList.InitListWindow(	MONTH_LIST_X,MONTH_LIST_Y,MONTH_LIST_WIDTH,
								bstone,list,
								length,length,
								sizeof(CBirthStone),NULL);
	ResetMonthList();
}
void CBirthStoneSystem::GetBSParam(int character,int* param)
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
void CBirthStoneSystem::ResetMonthList(void)
{
	if(Flag.mPlayerData[Flag.mParty[mCharacter]].SearchASkillType(AS_FREE_SPACE,true)!=mFreeSpace){
		InitMonthList();
		return;
	}
	if(!mSenken && Flag.SearchFSkill(313,true)){
		InitMonthList();
		return;
	}

	CPlayerData &cdata=Flag.mPlayerData[Flag.mParty[mCharacter]];
	int param[MAX_MONTH];
	GetBSParam(Flag.mParty[mCharacter],param);

	mMonthList.SetParam(param);

	//フリースペースがセットされているかどうか
	if(mFreeSpace>0){
		int fsset=0;
		for(int i=0;i<mFreeSpace;i++){
			if(Flag.mPlayerData[Flag.mParty[mCharacter]].GetFSpace(i)>=0){
				fsset++;
			}
		}
		mMonthList.SetParam(mMonthList.GetListSize()-1,mFreeSpace+fsset*8);
	}
	

	InitSkillList();
}
void CBirthStoneSystem::InitSkillList()
{
	int month=mMonthList.GetCursorItem()->mID;
	int chara=Flag.mParty[mCharacter];
	int learn[MAX_LEARN_MONTH];

	if(month>=256){	//フリースペース
		memset(mSkill,-2,sizeof(mSkill));
		memset(mLearn,0,sizeof(mLearn));
		for(int i=0;i<mFreeSpace;i++){
			mSkill[i]=Flag.mPlayerData[Flag.mParty[mCharacter]].GetFSpace(i);
		}
		return;
	}

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
void CBirthStoneSystem::OnDraw()
{
	if(!mLive)	return;

	mMonthList.OnDraw();
	mMonthList.DrawText(1,"所持数",55,-8);
	mSWindow.OnDraw();

	mSkillList.OnDraw();
	for(int i=0;i<MAX_SKILL_MONTH;i++){
		int& skill=mSkill[i];
		if(skill<0){
			if(skill==-1){
				NoneSkillData.Draw(&mSkillList,20,i*SKILL_Height,0);
			}
		}else{
			SkillData[mSkill[i]].Draw(&mSkillList,20,i*SKILL_Height,mLearn[i]);
		}
	}

}
int CBirthStoneSystem::OnTimer(bool timer)
{
	if(!mLive)	return -1;
	int month=mMonthList.OnTimer(timer);
	mSkillList.OnTimer();
	int chara=mSWindow.OnTimer(timer);

	if(mDeleteTimer>0){
		mDeleteTimer++;
		if(mDeleteTimer>WINDOW_OPEN_TIME){
			mLive=false;
		}
		return -1;
	}
	if(chara>=0){
		int cursor=mMonthList.GetCursor();
		mCharacter=chara;
		InitMonthList();
		mMonthList.SetCursor(cursor);
		InitSkillList();
	}
	if(month>=0){
		InitSkillList();
	}
	return -1;
}
void CBirthStoneSystem::Destroy(void)
{
	if(!mLive)	return;
	mDeleteTimer=1;
	mMonthList.Destroy();
	mSkillList.Destroy();
	mSkillTitle.Destroy();
	mSWindow.Destroy();
}

void	CBirthStoneSystem::Update(void)
{
	mSWindow.Update();
	ResetMonthList();
}

int		CBirthStoneSystem::GetCursor(void)
{
	return mMonthList.GetCursor();
}
const CElement*	CBirthStoneSystem::GetCursorItem(void)
{
	return mMonthList.GetCursorItem();
}

////////////////////////////////////////////////////////////////////////////////////////////////
const int COverItemSystem::SELECT_X[5]={160,230,300};

COverItemSystem::COverItemSystem()
{
	mLive=false;
}
COverItemSystem::~COverItemSystem()
{
}
void COverItemSystem::Init()
{
	mLive=true;
	mDeleteTimer=0;

	mCursor=1;
	mMode=0;
	mCharacter=0;
	mMoveTimer=0;
	mTimer=0;
	mNoItem=false;


	mWindow.InitPlaneWindow(10,60,320,60);

	int item[TEMP_ITEM_MAX];
	int param[TEMP_ITEM_MAX];
	for(int i=0;i<TEMP_ITEM_MAX;i++){
		item[i]=Flag.mTempItem[i].item;
		param[i]=Flag.mTempItem[i].param * 0x10000 + 1024;
	}
	mTempItem.InitListWindow(	310,70,ITEM_LIST_WIDTH,
								(const CElement**)AllItem,item,
								TEMP_ITEM_MAX,TEMP_ITEM_MAX,param);
	
	mHelpWindow.InitWindow(10,120,Flag.mTempItem[mTempItem.GetCursor()].item);

	GetHelp();
}
bool COverItemSystem::OnTimer(bool timer)
{
	if(!mLive)	return false;
	mTimer++;
	mMoveTimer--;

	bool pi,ti;
	if((!timer) || mMode==0){
		pi=false;		ti=false;
	}else if(mMode==2){
		pi=true;;		ti=false;
	}else{
		pi=false;;		ti=true;
	}
	int titem=mTempItem.OnTimer(ti);
	mWindow.OnTimer();
	mHelpWindow.OnTimer();

	if(!timer)	return false;

	if(titem>=0){
		GetHelp();
	}

	if(mMode==0){
		Cursor.Move(SELECT_X[mCursor]+mWindow.GetX(),mWindow.GetY()+35);
		if(pInput->GetRepeatKey(0)==1){
			PlayWave(SE_MOVE);
			mCursor--;
			if(mCursor<0)	mCursor=1;
		}
		if(pInput->GetRepeatKey(2)==1){
			PlayWave(SE_MOVE);
			mCursor++;
			if(mCursor>1)	mCursor=0;
		}
		if(pInput->GetKey(4)==1){
			Cursor.Click();
			switch(mCursor){
				case 0:
					PlayWave(SE_YES);
					mMode=1;
					break;
				case 1:
					PlayWave(SE_YES);
					Flag.DropTempItem();
					return true;
			}
		}
	}else	if(mMode==1){
		if(pInput->GetKey(4)==1){
			int id=mTempItem.GetCursorItem()->mID;
			if(AllItem[id]->mType & TYPE_ON_MENU){
				MenuData.UseItem(-1,mTempItem.GetCursor(),-1);
				ResetList();
			}else{
				AllItem[id]->Help(0);
			}
		}
		if(pInput->GetKey(6)==1){
			PlayWave(SE_NO);
			if(mCursor==1){
				for(int i=0;i<TEMP_ITEM_MAX;i++){
					int param = mTempItem.GetParam(i);
					mTempItem.SetParam(i, param & 0xffff0000);
				}
			}
			mMode=0;
		}
	}

	return false;
}
void COverItemSystem::OnDraw()
{
	if(!mLive)	return;

	mTempItem.OnDraw();

	mHelpWindow.OnDraw();

	mWindow.OnDraw();

	mWindow.DrawText(0,"アイテム入手！",0,0);

	mWindow.DrawBox(SELECT_X[mCursor],20,SELECT_X[mCursor+1]-SELECT_X[mCursor]-20,10,COLOR_SELECT);
	mWindow.DrawText(0,"確認"		,SELECT_X[0],10);
	mWindow.DrawText(0,"入手"		,SELECT_X[1],10);
	
}
void	COverItemSystem::ResetList(void)
{
	int item[TEMP_ITEM_MAX];
	int param[TEMP_ITEM_MAX];
	for(int i=0;i<TEMP_ITEM_MAX;i++){
		item[i]=Flag.mTempItem[i].item;
		param[i]=Flag.mTempItem[i].param * 0x10000 + 1024;
	}
	mTempItem.ResetList((const CElement**)AllItem,item,
							TEMP_ITEM_MAX,param);

	mNoItem=true;
	for(int i=0;i<TEMP_ITEM_MAX;i++){
		if(Flag.mTempItem[i]!=NO_ITEM){
			mNoItem=false;
			break;
		}
	}
	GetHelp();
}
void COverItemSystem::GetHelp(void)
{
	int item;
	item=Flag.mTempItem[mTempItem.GetCursor()].item;
	mHelpWindow.ChangeItem(item);
}

void COverItemSystem::Destroy()
{
	mTempItem.Destroy();
	mWindow.Destroy();
	mHelpWindow.Destroy();
}
///////////////////////////////////////////////////////////////////////////////////////////

CRemoveBirthStoneEle::CRemoveBirthStoneEle(void)
{
}
CRemoveBirthStoneEle::CRemoveBirthStoneEle(int id)
:CElement("",id)
{
	char filename[256];
	sprintf(filename,"data\\mapobject\\mapobject%03d",id);
	DTest.LoadTexture(mTex,filename,0xff008080);
}
void	CRemoveBirthStoneEle::Draw(CPlaneWindow* window,int x,int y,int param,D3DCOLOR color) const
{
	window->DrawTexture(&mTex,0,
						0,0,64,64,x-32,y-12);
	window->DrawText(0,CharacterBaseData[mID].GetName(),x+16,y+16);

	char text[64];
	sprintf(text,"Lv.%d",Flag.mPlayerData[mID].GetLv());
	window->DrawText(0,text,x+110,y+16);
	window->DrawText(0,"誕生石：",x+180,y+4);

	int bs=0;
	for(int i=0;i<MAX_MONTH;i++){
		bs+=Flag.mPlayerData[mID].GetLearnSkillNo(i);
	}
	sprintf(text,"%3d個",bs);
	window->DrawText(0,text,x+220,y+24,0,DT_RIGHT);
	window->DrawText(0,"★:",x+280,y+16);

	for(int i=0;i<MAX_MONTH-1;i++){
		int icon=AllItem[ITEM_BIRTH_STONE+i]->mIcon;
		int tx=icon%8*32;
		int ty=(icon/8)%8*32;
		int my =(i/6)*20-4;
		int mx =(i%6)*24+300;

		if(Flag.mPlayerData[mID].CheckLearnSkill(i,MAX_SKILL_MONTH-1)){
			window->DrawTexture(&ItemTex[icon/64],0,tx,ty,32,32,
						x+mx,y+my);
			
			int time=Flag.mPlayTime;
			D3DCOLOR color=(int)(0x18000000*(sin(time/16.0)+1))| 0x00ffffff;
			DTest.SetBlend(BLEND_SOLID_ADD);
			window->DrawTexture(&ItemTex[icon/64],0,tx,ty,32,32,
						x+mx,y+my,color);
			DTest.SetBlend(BLEND_STANDARD);
		}else{
			window->DrawBox(x+mx+15,y+my+15,2,2,0xd0000000);
		}
	}
}