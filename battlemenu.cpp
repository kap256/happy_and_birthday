#include "battlemenu.h"
#include "math.h"
#include "input.h"
#include "battlemode.h"
#include "itemdata.h"
#include "skilldata.h"
#include "cursor.h"
#include "changemode.h"
#include "month.h"
#include "menudata.h"

CBattleMenu::CBattleMenu(void)
{
	mNext=NULL;
	mDeleteTimer=0;
}

CBattleMenu::~CBattleMenu(void)
{
}
void	CBattleMenu::Init(int param)
{
}
bool	CBattleMenu::OnTimer(void)
{
	bool top=CheckTop();
	bool cursor=top;

	if(mDeleteTimer>0)					cursor=false;
	mList.OnTimer(cursor);

	if(mDeleteTimer>0){
		mDeleteTimer++;
		if(mNext)	mNext->OnTimer();
		mUpdate=false;
		return false;
	}
	while(mNext){
		if(mNext->mDeleteTimer>WINDOW_OPEN_TIME){
			CBattleMenu* del=mNext;
			mNext=mNext->mNext;
			delete del;
		}else{
			mNext->OnTimer();
			if(mNext->mDeleteTimer==1){
				Update();
				mUpdate=true;
			}
			return top;
		}
	}

	if(!mUpdate){
		Update();
		mUpdate=true;
	}
	return true;
}
void	CBattleMenu::DrawAll(void)
{
	CBattleMenu* menu=mNext;
	while(menu){
		menu->OnDraw();
		menu=menu->mNext;
	}
}
void	CBattleMenu::OnDraw(void)
{
	D3DCOLOR color=GetColor();
	mList.OnDraw(color);
}
void	CBattleMenu::Destroy(void)
{
	if(mDeleteTimer<=0){
		mDeleteTimer=1;
		mList.Destroy();
	}
}
void	CBattleMenu::SetMenu(BattleMenuType type,int param)
{
	PlayWave(SE_YES);

	CBattleMenu* mdata=this;
	while(mdata->mNext){
		mdata=mdata->mNext;
	}

	switch(type){
		case BMenuTurnStart:
			mdata->mNext= new CTurnStart;		break;
		case BMenuPersonal:
			mdata->mNext= new CPersonal;		break;
		case BMenuSelectSkill:
			mdata->mNext= new CSelectSkill;		break;
		case BMenuSetTarget:
			mdata->mNext= new CSetTarget;		break;
		case BMenuMoveCharacter:
			mdata->mNext= new CMoveCharcter;	break;
		case BMenuResultExp:
			mdata->mNext= new CResultExp;		break;
		case BMenuUseItem:
			mdata->mNext= new CUseItem;			break;
		case BMenuCheck:
			mdata->mNext= new CCheck;			break;
		
	}
	mdata->mNext->Init(param);
}

void	CBattleMenu::AllDelete(int n)
{
	CBattleMenu* mdata=mNext;

	for(int i=0;i<n;i++){
		mdata=mdata->mNext;
		if(!mdata)	return;
	}
	while(mdata){
		mdata->Destroy();
		mdata=mdata->mNext;
	}
}
bool CBattleMenu::CheckTop()
{
	CBattleMenu* bm=mNext;
	while(bm){
		if(bm->mDeleteTimer<=0)	return false;
		bm=bm->mNext;
	}
	return true;
}
D3DCOLOR	CBattleMenu::GetColor()
{
	if(CheckTop()){
		return 0xffffffff;
	}
	return COLOR_DARK_WINDOW;
}
void	CBattleMenu::FlashCharacter(int chara,bool reset)
{
	if(reset)	memset(BattleMode.mFieldFlash,0,sizeof(BattleMode.mFieldFlash));
	int x=BattleMode.mPlayer[chara].GetSX();
	int y=BattleMode.mPlayer[chara].GetSY();
	if(BattleMode.mFieldFlash[x][y]==0)	BattleMode.mFieldFlash[x][y]=200;
}
/////////////////////////////////////////////////////////////////////////////////////////////
void CTurnStart::Init(int param)
{
	static const CItem ele[MENU_NO]=
	{
			CItem("戦う"			,0,0),
			CItem("戦況確認"		,1,0),
			CItem("逃げる"			,2,0),
	};
	int no=MENU_NO;
	if(param & BOPT_NO_ESCAPE)	no--;

	mList.InitListWindow(	BATTLE_MENU_X,
							BATTLE_WINDOW_Y,
							BATTLE_WINDOW_X-BATTLE_MENU_X,
							ele,NULL,
							no,no,sizeof(CItem));
}
bool CTurnStart::OnTimer(void)
{
	if(!CBattleMenu::OnTimer())
		return false;

	if(pInput->GetRepeatKey(4)==1){
		const CElement* ele = mList.GetCursorItem();
		switch(ele->mID){
			case 0:
				BattleMode.ChangeMode(BM_PERSONAL_MENU);
				break;
			case 1:
				BattleMode.ChangeMode(BM_CHECK_MENU);
				break;
			case 2:
				BattleMode.ChangeMode(BM_ESCAPE);
				Destroy();
				break;
		}
	}

	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////
void CPersonal::Init(int param)
{
	mPlayerNum=param;

	bool	noweapon=false;
	int		list[MAX_ITEM+2];
	int		weaponno=-1;
	mWeaponNo=0;
	CPlayerData* cdata=BattleMode.mPlayer[mPlayerNum].GetPData();

	int w=cdata->GetEquipment(CHAREQ_WEAPON);
	for(int i=0;i<=MAX_ITEM;i++){
		mWeapon[mWeaponNo]=cdata->GetEquipment(CHAREQ_FROM_START+CHAREQ_WEAPON+i);
		if(mWeapon[mWeaponNo]<0){
			if(i==0){
				noweapon=true;
				list[0]=NO_ITEM;
				mWeaponNo++;
				continue;
			}
			break;
		}
		if(mWeapon[mWeaponNo]==w){
			weaponno=mWeaponNo;
		}
		list[mWeaponNo]=cdata->GetItem(mWeapon[mWeaponNo]).item;
		mWeaponNo++;
	}
	if(!noweapon){
		list[mWeaponNo]=NO_ITEM;
		mWeaponNo++;
	}

	static CItem ele[MENU_NO+1]={
			CItem("技"			,0,0),
			CItem("魔法"		,1,0),
			CItem("特殊"		,2,0),
			CItem("所持品"		,3,0),
			CItem("移動"		,4,0),
			CItem("行動不能！"	,5,0),
	};

	int menulist[MENU_NO];
	for(int i=0;i<MENU_NO;i++){
		menulist[i]=i;
	}
	if(BattleMode.mPlayer[mPlayerNum].SearchStatus(3)>0){
		menulist[0]=MENU_NO;
	}
	if(BattleMode.mPlayer[mPlayerNum].SearchStatus(4)>0){
		menulist[1]=MENU_NO;
	}
	mList.InitListWindow(	BATTLE_MENU_X,
							BATTLE_MENU_Y,
							BATTLE_MENU_WIDTH,
							ele,menulist,
							MENU_NO,MENU_NO,
							sizeof(CItem));

	int cursor=Flag.mSaveCursor.Get(SC_P_TOP_MENU + cdata->mID);
	mList.SetCursor(cursor);

	mSWindow.InitSelectWindow(	0,
								BATTLE_MENU_Y-24,
								BATTLE_MENU_WIDTH,
								(const CElement**)AllItem,list,
								mWeaponNo);
	mSWindow.SetCursor(weaponno);
	ChangeWeapon(weaponno);
	BattleMode.mPlayer[mPlayerNum].SetAction(-1);
	BattleMode.mPlayer[mPlayerNum].SetTarget(-1);
	BattleMode.ClearCharacterMove(mPlayerNum);

}
void	CPersonal::ChangeWeapon(int w)
{
	if(w<0)	w=mWeaponNo-1;
	CPlayerData* cdata=BattleMode.mPlayer[mPlayerNum].GetPData();
	if(mWeapon[w]<0){
		cdata->TakeOff(CHAREQ_WEAPON);
	}else{
		cdata->Equip(mWeapon[w]);
	}
	mCWindow.InitWindow(&BattleMode.mPlayer[mPlayerNum]);

	if(BattleMode.mPlayer[mPlayerNum].SearchStatus(3)<=0){
		mSkill[0]=(cdata->GetCSkillAll(CS_SKILL)>0);
	}else{
		mSkill[0]=false;
	}
	if(BattleMode.mPlayer[mPlayerNum].SearchStatus(4)<=0){
		mSkill[1]=(cdata->GetCSkillAll(CS_MAGIC)>0);
	}else{
		mSkill[1]=false;
	}
	mSkill[2]=(cdata->GetCSkillAll(CS_OTHER)>0);

	for(int i=0;i<3;i++){
		if(mSkill[i]){
			mList.SetParam(i,0);
		}else{
			mList.SetParam(i,COLOR_GRAY);
		}
	}

	BattleMode.mPlayer[mPlayerNum].LoadWeaponImg();
}
bool CPersonal::OnTimer(void)
{
	bool time=CBattleMenu::OnTimer();

	int weapon=mSWindow.OnTimer(time);
	mCWindow.OnTimer();

	if(!time){
		return false;
	}else{
		if(!mCWindow.GetLive())	mCWindow.InitWindow(&BattleMode.mPlayer[mPlayerNum]);
	}

	FlashCharacter(mPlayerNum);

	if(pInput->GetRepeatKey(6)==1){
		//PlayWave(SE_NO);
		BattleMode.ChangeMode(BM_PERSONAL_MENU+mPlayerNum*BM_PM_NUMBER+BM_PM_BACK);
		return false;
	}
	if(weapon>=0){
		ChangeWeapon(weapon);
	}



	if(pInput->GetRepeatKey(4)==1){
		const CElement* ele = mList.GetCursorItem();
		if(ele->mID >=3 || mSkill[ele->mID]){
			switch(ele->mID){
				case 0:
					BattleMode.ChangeMode(BM_PERSONAL_MENU + BM_PM_SKILL + BM_PM_NUMBER*mPlayerNum);
					break;
				case 1:
					BattleMode.ChangeMode(BM_PERSONAL_MENU + BM_PM_MAGIC + BM_PM_NUMBER*mPlayerNum);
					break;
				case 2:
					BattleMode.ChangeMode(BM_PERSONAL_MENU + BM_PM_SPECIAL + BM_PM_NUMBER*mPlayerNum);
					break;
				case 3:
					BattleMode.ChangeMode(BM_PERSONAL_MENU + BM_PM_ITEM + BM_PM_NUMBER*mPlayerNum);
					break;
				case 4:
					BattleMode.ChangeMode(BM_PERSONAL_MENU + BM_PM_MOVE + BM_PM_NUMBER*mPlayerNum);
					break;
				case 5:
					BattleMode.ChangeMode(BM_PERSONAL_MENU + BM_PM_NUMBER * (mPlayerNum+1));
					break;
			}
			//memset(BattleMode.mFieldFlash,0,sizeof(BattleMode.mFieldFlash));
			CPlayerData* cdata=BattleMode.mPlayer[mPlayerNum].GetPData();
			Flag.mSaveCursor.Set(SC_P_TOP_MENU + cdata->mID,mList.GetCursor());
			mCWindow.Destroy();
			int minus=BATTLE_WINDOW_X-BATTLE_MENU_WIDTH;
			mSWindow.Move(minus,BATTLE_MENU_Y);
			mList.Move(minus,BATTLE_MENU_Y);
		}else{
			PlayWave(SE_ERROR);
		}
	}


	return true;
}
void	CPersonal::Update(void)
{
	mList.Move(	BATTLE_MENU_X,BATTLE_MENU_Y);
	mSWindow.Move(0,BATTLE_MENU_Y-24);
}
void	CPersonal::OnDraw(void)
{
	D3DCOLOR color=GetColor();
	mList.OnDraw(color);
	mSWindow.OnDraw(color);
	mCWindow.OnDraw(color);
}
void	CPersonal::Destroy(void)
{
	CBattleMenu::Destroy();
	mSWindow.Destroy();
	mCWindow.Destroy();
	memset(BattleMode.mFieldFlash,0,sizeof(BattleMode.mFieldFlash));
}


/////////////////////////////////////////////////////////////////////////////
CSelectSkill::CSelectSkill()
{
	mSaveCursor=SC_NONE;
}
void CSelectSkill::Init(int param)
{
	mPlayerNum=param/BM_PM_NUMBER;
	CPlayerData* pdata=BattleMode.mPlayer[mPlayerNum].GetPData();

	switch(param%BM_PM_NUMBER){
		case BM_PM_SKILL:
			mSkillType = CS_SKILL;
			mSaveCursor=SC_P_SKILL_OTHER;
			int weapon;
			weapon=pdata->GetWeapon();
			if(weapon<MAX_EQUIPMENT_NUMBER){
				if(AllItem[weapon]->mType == TYPE_SWORD)mSaveCursor=SC_P_SKILL_SWORD;
				if(AllItem[weapon]->mType == TYPE_SPEAR)mSaveCursor=SC_P_SKILL_SPEAR;
				if(AllItem[weapon]->mType == TYPE_AXE)	mSaveCursor=SC_P_SKILL_AXE;
				if(AllItem[weapon]->mType == TYPE_BOW)	mSaveCursor=SC_P_SKILL_BOW;
			}
			break;
		case BM_PM_MAGIC:
			mSkillType = CS_MAGIC;
			mSaveCursor=SC_P_MAGIC;
			break;
		case BM_PM_SPECIAL:
			mSkillType = CS_OTHER;
			mSaveCursor=SC_P_ITEM;
			break;
	}
	mSaveCursor+=pdata->mID;

	int size=pdata->GetCSkillAll(mSkillType,mSkill,mSP);
	mList.InitListWindow(	BATTLE_MENU_X,
							BATTLE_MENU_Y,
							320,
							ComSkillData,mSkill,
							size,5,sizeof(CComSkillData));

	int cursor=Flag.mSaveCursor.Get(mSaveCursor);
	for(int i=0;i<size;i++){
		if(!mSP[i]){
			mList.SetParam(i,1);
		}
		if(mSkill[i] == cursor){
			mList.SetCursor(i);
		}
 	}
	mSPWin.InitPlaneWindow(	0,
							BATTLE_MENU_Y-24,
							280,
							ELEMENT_Height+16);
	mSPX=0;
	mSPGoal=0;
}
bool CSelectSkill::OnTimer(void)
{
	bool time=CBattleMenu::OnTimer();

	mSPWin.OnTimer();

	if(!time){
		return false;
	}

	FlashCharacter(mPlayerNum);
	if(pInput->GetRepeatKey(6)==1){
		PlayWave(SE_NO);
		Destroy();
		BattleMode.mPlayer[mPlayerNum].SetAction(-1);
		return false;
	}

	if(pInput->GetRepeatKey(4)==1){
		if(mSP[mList.GetCursor()]){
			const CElement* ele = mList.GetCursorItem();
			BattleMode.mPlayer[mPlayerNum].SetAction(ele->mID);
			BattleMode.ChangeMode(BM_PERSONAL_MENU + BM_PM_TARGET + BM_PM_NUMBER*mPlayerNum);
			Flag.mSaveCursor.Set(mSaveCursor,ele->mID);

			mList.Move(BATTLE_WINDOW_X-320,BATTLE_MENU_Y);
			mSPWin.Move(BATTLE_WINDOW_X-280,BATTLE_MENU_Y);
		}else{
			PlayWave(SE_ERROR);
		}
	}

	const CElement* ele = mList.GetCursorItem();
	const CComSkillData &sdata=ComSkillData[ele->mID];
	mSPGoal=sdata.mSP;
	mSPX+=(mSPGoal*SP_WIDTH-mSPX)/4.0;

	return true;
}
void	CSelectSkill::OnDraw(void)
{
	D3DCOLOR color=GetColor();
	mList.OnDraw(color);
	mSPWin.OnDraw(color);

	CPlayerData* pdata=BattleMode.mPlayer[mPlayerNum].GetPData();

	

	const int x=20;
	const int y=-6;
	bool zerosp=(pdata->GetSP(mSPGoal-1)<=0 && mSPGoal>0);

	color = 0x80ff8040;
	if(zerosp)	color = 0xffff0000;
	mSPWin.DrawBox(int(280-65-mSPX),y,22,16,color);
	mSPWin.DrawText(0,"残りSP",0,y);
	int j=0;
	static char text[32];
	while(1){
		int sp=pdata->GetSP(j);
		sprintf(text,"%d",sp);

		color=0xff000000;
		if(j+1==mSPGoal && zerosp){
			color=0xffffffff;
		}
		mSPWin.DrawText(0,text,x+(j+1)*SP_WIDTH,y,color,DT_RIGHT);
		j++;
		if(j>=MAX_CSKILL_LEVEL)	break;
		mSPWin.DrawText(0,"/",x+(j+1)*SP_WIDTH-10,y,0,DT_RIGHT);
	}
	mSPWin.DrawText(0,"∞",x+2,y,0,DT_RIGHT);
}
void	CSelectSkill::Destroy(void)
{
	CBattleMenu::Destroy();
	mSPWin.Destroy();
	memset(BattleMode.mFieldFlash,0,sizeof(BattleMode.mFieldFlash));
}
void	CSelectSkill::Update(void)
{
	mList.Move(BATTLE_MENU_X,BATTLE_MENU_Y);
	mSPWin.Move(0,BATTLE_MENU_Y-24);
}

/////////////////////////////////////////////////////////////////////////////
CSetTarget::CSetTarget()
{
	mSaveCursor=SC_NONE;
}
void CSetTarget::Init(int param)
{
	mPlayerNum=param;
	mSkill=BattleMode.mPlayer[mPlayerNum].GetAction();
	CPlayerData* pdata=BattleMode.mPlayer[mPlayerNum].GetPData();

	bool enemy=BattleMode.mPlayer[mPlayerNum].Enemy();
	if(ComSkillData[mSkill].mTarget)	enemy=!enemy;
	CElement *ele[CHARACTER_NO];

	if(!enemy){
		mTarget[0]=0;
		mSaveCursor=SC_P_PLAYER_TARGET+pdata->mID;
	}else{
		mTarget[0]=MAX_PARTY_NUMBER;
		mSaveCursor=SC_P_ENEMY_TARGET+pdata->mID;
	}


	int allparam[CHARACTER_NO];
	int no=0;

	memset(allparam,0,sizeof(allparam));

	int cursor=0;
	if(ComSkillData[mSkill].mRange==CR_ALL || ComSkillData[mSkill].mRange==CR_S_ALL){
		static  CItem all("全体",0,0);
		ele[0]=&all;
		no=1;
		if(BattleMode.mPlayer[mPlayerNum].SearchASkill(382)){
			if(!enemy){
				mTarget[0]=MAX_PARTY_NUMBER;
			}else{
				mTarget[0]=0;
			}
		}
	}else if(ComSkillData[mSkill].mRange==CR_NONE || ComSkillData[mSkill].mRange==CR_SELF){
		static  CItem play("実行",0,0);
		ele[0]=&play;
		no=1;
	}else{
		int possible;
		no=BattleMode.GetPossibleTarget(mPlayerNum,mSkill,mTarget,&possible);
		for(int i=0;i<no;i++){
			ele[i]=BattleMode.mCharacter[mTarget[i]];
		}
		for(int i=possible;i<no;i++){
			allparam[i]=1;
		}
	}
	if(no<=0){
		static  CItem none("エラー：対象がいません。",-1,0);
		ele[0]=&none;
		mTarget[0]=0;
		no=1;
	}

	int size=5;
	if(no<size)	size=no;
	mList.InitListWindow(	BATTLE_MENU_X,
							BATTLE_MENU_Y,
							BATTLE_MENU_WIDTH,
							(const CElement**)ele,NULL,
							no,size,allparam);
	mList.SetCursor(cursor);
	mRealTarget=0;
	mTimer=0;
}
bool CSetTarget::OnTimer(void)
{
	if(!CBattleMenu::OnTimer())
		return false;

	mTimer++;
	const CElement *ele=mList.GetCursorItem();
	int cursor=mList.GetCursor();
	if(cursor!=mOldCursor){
		mTimer=0;
	}
	if(ele->mID>=0 && ComSkillData[mSkill].mRange!=CR_NONE  && ComSkillData[mSkill].mRange!=CR_SELF){
		int fno;
		int *allfriend;
		mRealTarget=BattleMode.CheckTarget(BattleMode.mCharacter[mPlayerNum],mTarget[cursor],mSkill);

		allfriend=BattleMode.GetTargetArea(	mPlayerNum,
											mRealTarget,
											mSkill,
											BattleMode.mFieldFlash,
											&fno);

		FlashCharacter(mPlayerNum,false);

		BattleMode.mTargetLineFrom=mPlayerNum;
		BattleMode.mTargetLineTo=mTarget[cursor];

		if(mRealTarget!=mTarget[cursor]){
			CBattleCharacter* character=BattleMode.mCharacter[mRealTarget];
			character->ChangeAni(1*ANI_TYPE,1,true);
		}
		if(mTimer%60==0){
			for(int i=0;i<fno;i++){
				if(allfriend[i]!=mPlayerNum){
					CBattleCharacter* character=BattleMode.mCharacter[allfriend[i]];
					character->SetAvoid();
				}
			}
		}
	}else{
		BattleMode.mTargetLineFrom=-1;
	}

	if(pInput->GetRepeatKey(6)==1){
		PlayWave(SE_NO);
		Destroy();
		BattleMode.mPlayer[mPlayerNum].SetTarget(-1);
		return false;
	}

	if(pInput->GetRepeatKey(4)==1 && ele->mID>=0){
		BattleMode.mPlayer[mPlayerNum].SetTarget(mTarget[cursor]);
		BattleMode.ChangeMode(BM_PERSONAL_MENU+(mPlayerNum+1)*BM_PM_NUMBER);
		Flag.mSaveCursor.Set(mSaveCursor,mTarget[cursor]);
	}

	mOldCursor=cursor;
	return true;
}

void CSetTarget::Destroy(void)
{
	CBattleMenu::Destroy();
	memset(BattleMode.mFieldFlash,0,sizeof(BattleMode.mFieldFlash));
	BattleMode.mTargetLineFrom=-1;
}
/////////////////////////////////////////////////////////////////////////////
void CMoveCharcter::Init(int param)
{
	mPlayerNum=param;
	mX=BattleMode.mPlayer[mPlayerNum].GetSX();
	mY=BattleMode.mPlayer[mPlayerNum].GetSY();
	mTimer=0;
	memset(BattleMode.mFieldFlash,0,sizeof(BattleMode.mFieldFlash));
}
bool CMoveCharcter::OnTimer(void)
{
	mTimer++;
	if(!CBattleMenu::OnTimer())
		return false;

	if(pInput->GetRepeatKey(6)==1){
		PlayWave(SE_NO);
		BattleMode.ClearCharacterMove(mPlayerNum);
		Destroy();
		return false;
	}
	if(pInput->GetRepeatKey(4)==1){
		if(BattleMode.SetCharacterMove(mX,mY,mPlayerNum)){
			PlayWave(SE_YES);
			BattleMode.ChangeMode(BM_PERSONAL_MENU+(mPlayerNum+1)*BM_PM_NUMBER);
			return false;
		}else{
			PlayWave(SE_ERROR);
		}
	}

	int &x=mX;
	int &y=mY;
	if(pInput->GetRepeatKey(0)==1){
		PlayWave(SE_MOVE);
		x--;
		if(x<5)	x=8;
	}
	if(pInput->GetRepeatKey(1)==1){
		PlayWave(SE_MOVE);
		y--;
		if(y<0)	y=3;
	}
	if(pInput->GetRepeatKey(2)==1){
		PlayWave(SE_MOVE);
		x++;
		if(x>=9)	x=5;
	}
	if(pInput->GetRepeatKey(3)==1){
		PlayWave(SE_MOVE);
		y++;
		if(y>=4)	y=0;
	}
	Cursor.Move(mX*BATTLE_SQUARE_SIZE+BATTLE_SQUARE_X+32,
				mY*BATTLE_SQUARE_SIZE+BATTLE_SQUARE_Y+64);
	return true;
}
void CMoveCharcter::OnDraw(void)
{
	CBattleMenu::OnDraw();

	if(mDeleteTimer>0)	return;

	D3DCOLOR color=(int(sin(mTimer/16.0+24)*127)+128)*0x01000000+0x8080ff;
	DTest.SetBlend(BLEND_ADD);
	for(int x=5;x<=9;x++){
		int l=BATTLE_SQUARE_X+x*BATTLE_SQUARE_SIZE+32;
		int t=BATTLE_SQUARE_Y+64;
		DTest.DrawBox(	l,t,
						l+2,t+258,color);
	}
	for(int y=0;y<=4;y++){
		int l=BATTLE_SQUARE_X+5*BATTLE_SQUARE_SIZE+32;
		int t=BATTLE_SQUARE_Y+y*BATTLE_SQUARE_SIZE+64;
		DTest.DrawBox(	l,t,
						l+258,t+2,color);
	}
	DTest.SetBlend(BLEND_STANDARD);

	for(int i=0;i<MAX_PARTY_NUMBER;i++){
		if(BattleMode.mMoveCharacter[i]){
			BattleMode.mPlayer[i].OnDraw(	(BattleMode.mMoveX[i])*BATTLE_SQUARE_SIZE+BATTLE_SQUARE_X,
											(BattleMode.mMoveY[i])*BATTLE_SQUARE_SIZE+BATTLE_SQUARE_Y,
											0xb0ffffff);
		}
	}

	BattleMode.mPlayer[mPlayerNum].OnDraw(	(mX)*BATTLE_SQUARE_SIZE+BATTLE_SQUARE_X,
											(mY)*BATTLE_SQUARE_SIZE+BATTLE_SQUARE_Y-16);
	DTest.SetBlend(BLEND_SOLID_ADD);
	BattleMode.mPlayer[mPlayerNum].OnDraw(	(mX)*BATTLE_SQUARE_SIZE+BATTLE_SQUARE_X,
											(mY)*BATTLE_SQUARE_SIZE+BATTLE_SQUARE_Y-16,
											0xff000080);
	DTest.SetBlend(BLEND_STANDARD);
}
void CMoveCharcter::Destroy(void)
{
	CBattleMenu::Destroy();
}
/////////////////////////////////////////////////////////////////////////////
void CResultExp::Init(int param)
{
	mTimer=0;
	mWindow.InitPlaneWindow(20,20,300,420);
	mMoneyWindow.InitPlaneWindow(320,360,300,80);

	int moneyup=0;
	int expup=0;
	bool miyage=false;
	for(int i=0;i<MAX_PARTY_NUMBER;i++){
		if(BattleMode.mPlayer[i].GetLive()){
			int plv=BattleMode.mPlayer[i].GetLv();
			mExp[i]=0;
			for(int j=0;j<MAX_ENEMY_NUMBER;j++){
				if(BattleMode.mEnemy[j].GetLive()){
					int elv=BattleMode.mEnemy[j].GetLv();
//					if(Flag.mHardMode)	elv-=MonthData[Flag.mCurrentMonth].mHardLv/2;
					int exp=BattleMode.mEnemy[j].GetExp();
					mExp[i]+=CPlayerData::CulcExp(exp,plv,elv,false);
				}
			}

			int m=BattleMode.mPlayer[i].GetCData()->SearchASkillType(AS_MONEY_UP,true);
			if(m>moneyup)	moneyup=m;
			int e=BattleMode.mPlayer[i].GetCData()->SearchASkillType(AS_EXP_UP,true);
			if(e>expup)	expup=e;
			if(BattleMode.mPlayer[i].GetCData()->SearchASkill(314))	miyage=true;
		}
	}

	if(miyage){
		for(int i=0;i<MAX_CHARACTER_NUMBER;i++){
			int character=Flag.mParty[i+MAX_PARTY_NUMBER];
			if(character<0)	continue;

			double exp=MIN_EXP;
			int plv=Flag.mPlayerData[character].GetLv();
			for(int j=0;j<MAX_ENEMY_NUMBER;j++){
				if(BattleMode.mEnemy[j].GetLive()){
					int elv=BattleMode.mEnemy[j].GetLv();
	//				if(Flag.mHardMode)	elv-=MonthData[Flag.mCurrentMonth].mHardLv/2;
					int e=BattleMode.mEnemy[j].GetExp();
					exp+=CPlayerData::CulcExp(e,plv,elv,false);
				}
			}
			exp*=1.0+expup/100.0;
			if(exp>MAX_EXP)	exp=MAX_EXP;
			if(exp<MIN_EXP)	exp=MIN_EXP;
			exp=exp*2/3;
			Flag.mPlayerData[character].SetExp(exp,exp);
		}
	}

	mMoney=0;
	for(int j=0;j<MAX_ENEMY_NUMBER;j++){
		if(BattleMode.mEnemy[j].GetLive()){
			int elv=BattleMode.mEnemy[j].GetLv();
		//	if(Flag.mHardMode){
		//		elv=elv-MonthData[Flag.mCurrentMonth].mBaseLv[1]+MonthData[Flag.mCurrentMonth].mBaseLv[0];
		//	}
			int money=BattleMode.mEnemy[j].GetMoney();
			money   = int( money*(elv+10)/10 );
			mMoney += money;
		}
	}
	mMoney=mMoney*(100+moneyup)/100;

	for(int i=0;i<MAX_PARTY_NUMBER;i++){
		mExp[i]*=1.0+expup/100.0;
		if(mExp[i]>MAX_EXP)	mExp[i]=MAX_EXP;
		if(mExp[i]<MIN_EXP)	mExp[i]=MIN_EXP;
		mExp[i]+=0.4999;
		if(BattleMode.mPlayer[i].GetLv()>=MAX_LV)		mExp[i]=0;
	}
	memset(mLvUp,0,sizeof(mLvUp));
}
bool CResultExp::OnTimer(void)
{
	mTimer++;
	bool timer=CBattleMenu::OnTimer();

	mWindow.OnTimer();
	mMoneyWindow.OnTimer();
	mSubWindow.OnTimer();

	if(!timer)
		return false;

	if(		pInput->GetKey(4)==1
		||	pInput->GetKey(5)==1
		||	pInput->GetKey(6)==1){
		if(mTimer<TIMER_END_ADD-1){
			mTimer=TIMER_END_ADD-1;
		}else{
			ReturnMapInfo.ReturnMap(false);
			Destroy();
		}
	}
	for(int i=0;i<MAX_PARTY_NUMBER;i++){
		if(mLvUp[i]>0){
			mLvUp[i]--;
		}
	}

	if(TIMER_START_ADD<=mTimer && mTimer<TIMER_END_ADD){
		int time=TIMER_END_ADD-mTimer;
		for(int i=0;i<MAX_PARTY_NUMBER;i++){
			if(BattleMode.mPlayer[i].GetLive()){
				double exp=mExp[i]/time;
				CPlayerData *pdata=BattleMode.mPlayer[i].GetPData();
				if(pdata->SetExp(mExp[i],exp)){
					mLvUp[i]=LV_UP_TIME;
					PlayWave(SE_LV_UP);
				}
			}
		}
		int money=mMoney/time;
		Flag.mMoney+=money;
		mMoney-=money;
	}else if(mTimer==TIMER_END_ADD){
		mSubWindow.InitPlaneWindow(430,430,200,32);
	}


	return true;
}
void CResultExp::OnDraw(void)
{
	D3DCOLOR color=GetColor();

	char text[256];
	mWindow.OnDraw(color);
	if(mWindow.GetInited()){
		for(int i=0;i<MAX_PARTY_NUMBER;i++){
			if(BattleMode.mPlayer[i].GetLive()){
				BattleMode.mPlayer[i].OnDraw(0,i*100+10);
				mWindow.DrawText(0,BattleMode.mPlayer[i].GetName(),80,i*100);

				mWindow.DrawText(0,"Lv.",50,i*100,0,DT_RIGHT);
				sprintf(text,"%d",BattleMode.mPlayer[i].GetLv());
				mWindow.DrawText(0,text,10,i*100,0,DT_RIGHT);

				int exp=BattleMode.mPlayer[i].GetExp();
				mWindow.DrawText(0,"次のレベルまで",80,i*100+25);
				sprintf(text,"%d",LV_UP_EXP-exp);
				mWindow.DrawText(0,text,10,i*100+25,0,DT_RIGHT);

				int get=int(mExp[i]);
				mWindow.DrawText(0,"獲得経験値",80,i*100+45);
				sprintf(text,"%d",get);
				mWindow.DrawText(0,text,10,i*100+45,0,DT_RIGHT);

				const int BAR_W = 190;
				const int BAR_H = 4;
				mWindow.DrawBox(79,i*100+69,BAR_W+2,BAR_H+2,0xff000000);
				mWindow.DrawBox(80,i*100+70,BAR_W,BAR_H,0xff606060);
				get+=exp;
				if(get>LV_UP_EXP)	get=LV_UP_EXP;
				mWindow.DrawBox(80,i*100+70,BAR_W*get/LV_UP_EXP,BAR_H,0xffffff00);
				mWindow.DrawBox(80,i*100+70,BAR_W*exp/LV_UP_EXP,BAR_H,0xff00ff80);
				if(mLvUp[i]>0){
					int y=i*100+40;
					if(mLvUp[i]>50){
						y+=int(sin((50-mLvUp[i])*0.3141592)*32);
					}
					mWindow.DrawTexture(&SystemTex,0,0,112,64,32,200,y);
				}
			}
		}
	}
	mMoneyWindow.OnDraw();
	mMoneyWindow.DrawText(0,"獲得金額：",0,0);
	sprintf(text,"%d M",mMoney);
	mMoneyWindow.DrawText(0,text,10,0,0,DT_RIGHT);
	mMoneyWindow.DrawText(0,"所持金：",0,30);
	sprintf(text,"%d M",Flag.mMoney);
	mMoneyWindow.DrawText(0,text,10,30,0,DT_RIGHT);
	mSubWindow.OnDraw();
	mSubWindow.DrawText(0,"ボタンを押してください。",0,-8);

	if(mNext){
		mNext->OnDraw();
	}
}
void CResultExp::Destroy(void)
{
	mWindow.QuickDestroy();
	mMoneyWindow.QuickDestroy();
	mSubWindow.QuickDestroy();
	CBattleMenu::Destroy();
}
/////////////////////////////////////////////////////////////////////////////
void CUseItem::Init(int param)
{
	mPlayerNum=param;

	CPlayerData* pdata=BattleMode.mPlayer[mPlayerNum].GetPData();
	int itemlist[MAX_ITEM];
	int paramlist[MAX_ITEM];
	int min=pdata->GetEquipMax();
	int size=pdata->GetItemMax()-min;
	int i;
	for(i=0;i<size;i++){
		CarryItem item = pdata->GetItem(i+min);
		itemlist[i]		= item.item;
		paramlist[i]	= item.param*0x10000 + 4 + 1024;
	}

	mList.InitListWindow(	BATTLE_MENU_X+20,
							BATTLE_MENU_Y,
							280,
							(const CElement**)AllItem,itemlist,
							size,4,paramlist);

	int cursor=Flag.mSaveCursor.Get(SC_P_ITEM+pdata->mID);
	mList.SetCursor(cursor);
}
bool CUseItem::OnTimer(void)
{

	if(!CBattleMenu::OnTimer())
		return false;

	FlashCharacter(mPlayerNum);
	if(pInput->GetRepeatKey(6)==1){
		PlayWave(SE_NO);
		Destroy();
		BattleMode.mPlayer[mPlayerNum].SetAction(-1);
		return false;
	}

	if(pInput->GetRepeatKey(4)==1){
		int cursor= mList.GetCursor();
		CPlayerData* pdata=BattleMode.mPlayer[mPlayerNum].GetPData();
		int min=pdata->GetEquipMax();
		if(BattleMode.mPlayer[mPlayerNum].SetUseItem(cursor+min)){
			BattleMode.ChangeMode(BM_PERSONAL_MENU + BM_PM_TARGET + BM_PM_NUMBER*mPlayerNum);
			CPlayerData* pdata=BattleMode.mPlayer[mPlayerNum].GetPData();
			Flag.mSaveCursor.Set(SC_P_ITEM+pdata->mID,cursor);

			mList.Move(BATTLE_WINDOW_X-280,BATTLE_MENU_Y);
		}else{
			PlayWave(SE_ERROR);
		}
	}


	return true;
}
void	CUseItem::Destroy(void)
{
	memset(BattleMode.mFieldFlash,0,sizeof(BattleMode.mFieldFlash));
	CBattleMenu::Destroy();
}
void	CUseItem::Update(void)
{
	mList.Move(BATTLE_MENU_X+20,BATTLE_MENU_Y);
}

/////////////////////////////////////////////////////////////////////////////
void CCheck::Init(int param)
{
	CElement	*character[CHARACTER_NO];
	int	max=0;

	for(int i=0;i<CHARACTER_NO;i++){
		if(BattleMode.mCharacter[i]->GetCData() && !(BattleMode.mCharacter[i]->GetDead())){
			character[max]=BattleMode.mCharacter[i];
			max++;
		}
	}

	int size=5;
	if(max<size)	size=max;
	mList.InitListWindow(	BATTLE_MENU_X+40,
							BATTLE_MENU_Y,
							250,
							(const CElement**)character,NULL,
							max,size);
}
bool CCheck::OnTimer(void)
{
	if(!CBattleMenu::OnTimer())
		return false;

	if(pInput->GetRepeatKey(6)==1){
		PlayWave(SE_NO);
		Destroy();
		return false;
	}

	if(pInput->GetRepeatKey(4)==1){
		const CElement* ele=mList.GetCursorItem();
		ele->Help(0);
	}

	return true;
}

void CCheck::Destroy(void)
{
	CBattleMenu::Destroy();
}
/////////////////////////////////////////////////////////////////////////////