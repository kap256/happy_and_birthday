#include "menudata.h"
#include "cursor.h"
#include "input.h"
#include "flag.h"
#include "mapmode.h"
#include "changemode.h"
#include "village.h"
#include "option.h"

#ifndef _NO_USE_GGSOUND_
#include "GGSound.h"
#endif


bool CStartMenu::OnTimer()
{

	bool timer=CMenuData::OnTimer();

	bool c=timer;
	if(mNextMenu>=0)	c=false;
	mWindow.OnTimer(c);
	mInfo.OnTimer();

	if(mCDataMove){
		for(int i=0;i<MAX_CHARACTER_NUMBER+MAX_PARTY_NUMBER;i++){
			mCDataWindow[i].OnTimer();
		}
	}
	if(!timer)	return false;

	if(mNextMenu>=0){		
		Cursor.Move(mCDataWindow[mCharacter].GetX(),
					mCDataWindow[mCharacter].GetY()+CCharaDataWindowMenu::WIN_Height-40);
	//カーソル操作
		//控え用
		if(mCharacter>=MAX_PARTY_NUMBER){
			if(pInput->GetRepeatKey(1)==1){
				mCharacter=(mCharacter-MAX_PARTY_NUMBER)*2/mReserveNo + 2;
				PlayWave(SE_MOVE);
			}
			if(pInput->GetRepeatKey(3)==1){
				mCharacter=(mCharacter-MAX_PARTY_NUMBER)*2/mReserveNo ;
				PlayWave(SE_MOVE);
			}
			if(pInput->GetRepeatKey(0)==1){
				mCharacter--;
				if(mCharacter<MAX_PARTY_NUMBER)	mCharacter=MAX_PARTY_NUMBER+mReserveNo-1;
				PlayWave(SE_MOVE);
			}
			if(pInput->GetRepeatKey(2)==1){
				mCharacter++;
				if(mCharacter>=MAX_PARTY_NUMBER+mReserveNo)	mCharacter=MAX_PARTY_NUMBER;
				PlayWave(SE_MOVE);
			}
		}else{
		//スタメン用
			if(pInput->GetRepeatKey(1)==1){
				mCharacter-=2;
				if(mCharacter<0){
					if(mReserveNo<=0){
						mCharacter+=MAX_PARTY_NUMBER;
					}else{
						mCharacter=MAX_PARTY_NUMBER;
					}
				}
				PlayWave(SE_MOVE);
			}
			if(pInput->GetRepeatKey(3)==1){
				mCharacter+=2;
				if(mCharacter>=MAX_PARTY_NUMBER){
					if(mReserveNo<=0){
						mCharacter-=MAX_PARTY_NUMBER;
					}else{
						mCharacter=MAX_PARTY_NUMBER;
					}
				}
				PlayWave(SE_MOVE);
			}
			if(pInput->GetRepeatKey(0)==1 || pInput->GetRepeatKey(2)==1){
				mCharacter=mCharacter^1;
				PlayWave(SE_MOVE);
			}
		}

		if(pInput->GetKey(4)==1){
			Cursor.Click();
			if(Flag.mParty[mCharacter]>=0){
				for(int i=0;i<MAX_CHARACTER_NUMBER+MAX_PARTY_NUMBER;i++){
					mCDataWindow[i].Home();
				}

				switch(mNextMenu){
					case 0:
						SetMenu(ItemMenu,mCharacter);
						break;
					case 2:
						SetMenu(StatusMenu,mCharacter+64);
						break;
					case 4:
						SetMenu(BirthStoneMenu,mCharacter);
						break;
				}
				mNextMenu=-1;
			}else{
				PlayWave(SE_ERROR);
			}
		}
		if(pInput->GetKey(5)==1){
			if(Flag.mParty[mCharacter]>=0){
				SetMenu(StatusMenu,mCharacter+64);
			}
		}
		if(pInput->GetKey(6)==1){
			PlayWave(SE_NO);
			mNextMenu=-1;
		}
	}else{
		for(int i=0;i<MAX_CHARACTER_NUMBER+MAX_PARTY_NUMBER;i++){
			mCDataWindow[i].Home();
		}
		if(pInput->GetKey(6)==1){
			PlayWave(SE_NO);
			Destroy();
			return true;
		}
		if(pInput->GetKey(4)==1){
			mCharacter=0;
			const CElement* ele=mWindow.GetCursorItem();
			switch(ele->mID){
				case 0:
				case 2:
				case 4:
					PlayWave(SE_YES);
					SetNextMenu(ele->mID);
					break;
				case 3:
					SetMenu(ConfigMenu,mCharacter);
					break;
				case 5:
					SetMenu(FormationMenu,mCharacter);
					break;
				case 6:
					SetMenu(DInfoMenu,Flag.mCurrentMonth);
					break;
				case 7:
					SetMenu(PreciousItemMenu,0);
					break;
#ifdef _DEBUG
					case 255:
						CMapObject* obj;
						obj=MapMode.mObject[0];
						ChangeMapInfo.mX=obj->GetX_32();
						ChangeMapInfo.mY=obj->GetY_32();
						ChangeMapInfo.mZ=obj->GetZ_32();
						ChangeMapInfo.mDir=obj->GetDir();
						MenuData.SetMenu(DiaryMenu,0);
						break;

					case 256:
						MenuData.SetMenu(ChangeMemberMenu,0);
						break;
					case 257:
						MenuData.SetMenu(ClosetMenu,0);
						break;
					case 258:
						MenuData.SetMenu(FarmMenu,0);
						break;
#endif
			}
		}
	}
	return true;
}

void	CStartMenu::SetNextMenu(int next)
{
	mNextMenu=next;
	for(int i=0;i<mReserveNo;i++){
		mCDataWindow[i+MAX_PARTY_NUMBER].Move(	i*(640)/(mReserveNo),
												380);
	}
	for(int i=0;i<MAX_PARTY_NUMBER;i++){
		mCDataWindow[i].Move((i%2)*CCharaDataWindowMenu::WIN_WIDTH,
							(i/2)*(280+(mReserveNo>0?0:100)));
	}
}
//////////////////////////////////////////////////////////////////////////////////
bool CItemMenu::OnTimer()
{
	bool timer=CMenuData::OnTimer();

	int item=mItemList.OnTimer(timer && mMode==0);
	mHelpWindow.OnTimer();
	mCompare.OnTimer();
	int chara=mSWindow.OnTimer(timer && mMode==0);

	int category=mCategoryWindow.OnTimer(timer && mMode==1);
	int closet=mClosetList.OnTimer(timer && mMode==1);
 
	if(!timer)	return false;


	if(mMode==0){
		if(chara>=0){
			ChangeCharacter(chara);
		}
		if(item>=0){
			CheckItemCursor(item);
			ChangeHelp();
		}

		if(pInput->GetKey(6)==1){
			PlayWave(SE_NO);
			Destroy();
			return true;
		}

		if(pInput->GetKey(4)==1){
			CPlayerData& pdata=Flag.mPlayerData[Flag.mParty[mCharacter]];
			int cursor=mItemList.GetCursor();
			int x,y;
			mItemList.GetCursorPos(&x,&y);
			if((1<=cursor && cursor<WEAPON_SPACE) || pdata.GetEquipMax()<=cursor){
				SetMenu(ItemSubMenu,(y-1)*0x10000+mCharacter*0x100+cursor);
			}else{
				OpenCloset();
			}
		}
	}else if (mMode==1){
		if(category>=0){
			mCategory = mCategoryWindow.GetCursorItem()->mID;
			int cat=1<<mCategory;
			if(mCategory<=0)	cat=mCategoryAll;
			UpdataClosetList(&mClosetList,cat); 
			ChangeHelp();
			closet=0;
		}
		if(closet>=0){
			ChangeHelp();
		}

		if(pInput->GetKey(6)==1){
			PlayWave(SE_NO);
			CloseCloset();
		}
		if(pInput->GetKey(4)==1){
			int id=mClosetList.GetCursorItem()->mID;
			int select=mItemList.GetCursor();
			int closet=Flag.GetCloset(id);
			if(closet){
				CPlayerData &cdata=Flag.mPlayerData[Flag.mParty[mCharacter]];
				CarryItem item=cdata.GetItem(select);

				Flag.SetCloset(item);
				cdata.TakeOff(select);
				cdata.SetItem(select,id);
				if(!(select!=0 && select<WEAPON_SPACE)){
					cdata.Equip(select);
				}else{
					cdata.Calculate();
				}
				if(id!=NO_ITEM)	PlayWave(SE_EQUIP);
				else			PlayWave(SE_NO);

				CloseCloset();
			}
		}
	}

	return true;
}
void	CItemMenu::CheckItemCursor(int item){
	if(mCharacter<MAX_PARTY_NUMBER)	return;
	int eqmax=Flag.mPlayerData[Flag.mParty[mCharacter]].GetEquipMax();
	if(item>=eqmax+1){
		mItemList.SetCursor(eqmax-1);
	}else if(item>=eqmax){
		mItemList.SetCursor(0);
	}
	int x,y;
	mItemList.GetCursorPos(&x,&y);
	Cursor.Move(x,y);
}
void CItemMenu::OpenCloset()
{
	CPlayerData& pdata=Flag.mPlayerData[Flag.mParty[mCharacter]];

	PlayWave(SE_YES);
	mClosetList.Move(320,45);
	mClosetList.SetCursor(0);
	mHelpWindow.Move(5,235);
	mCompare.Move(5,20);
	mPData=pdata;
	mCompare.SetBefore(&mPData);

	int cursor=mItemList.GetCursor();
	int clist[CATEGORY_MAX];
	int cnum=1;
	clist[0]=0;
	mCategoryAll=0;

	if(cursor<WEAPON_SPACE){
		for(int i=1;i<6;i++){
			mCategoryAll= mCategoryAll |(1<<i);
		}
		mPData.Equip(cursor);
	}else if (cursor<WEAPON_SPACE+ARMOR_SPACE){
		mCategoryAll=1<<6;
	}else{
		int current=mItemList.GetCursorItem()->mID;
		mCategoryAll=0x1f<<7;
		for(int i=0;i<MAX_ACCESSORY_REAL;i++){
			int ac=pdata.GetEquipment(CHAREQ_ACCESSORY+i);
			if(ac<0)	continue;
			int item=pdata.GetItem(ac).item;
			mCategoryAll = mCategoryAll & (~(AllItem[item]->mType & 0x0000ffff)<<6);
		}

		if(current!=NO_ITEM){
			mCategoryAll = mCategoryAll | (AllItem[current]->mType & 0x0000ffff)<<6;
		}
	}

	for(int i=0;i<CATEGORY_MAX;i++){
		if(mCategoryAll & (1<<(i))){
			clist[cnum]=i;
			cnum++;
		}
	}

	mCategoryWindow.InitSelectWindow(
			650,8,320,
			ItemCategory,clist,
			cnum,sizeof(CItemCategoryData));
	mCategoryWindow.Open();
	mCategoryWindow.Move(320,8);


	UpdataClosetList(&mClosetList,mCategoryAll); 
	mCategory=0;
	mMode=1;
	ChangeHelp();
}
void CItemMenu::CloseCloset()
{
	mClosetList.Home();
	mCategoryWindow.Home();
	mHelpWindow.Home();
	mCompare.Home();
	mMode=0;
	Update();
}
//////////////////////////////////////////////////////////////////////////////////
bool CItemSubMenu::OnTimer()
{
	CPlayerData &cdata=Flag.mPlayerData[Flag.mParty[mCharacter]];
	bool timer=CMenuData::OnTimer();

	mAccessoryTimer--;
	mList.OnTimer(timer);
	mTitle.OnTimer();
	mCompare.OnTimer();

	if(!timer)	return false;

	if(pInput->GetKey(6)==1){
		PlayWave(SE_NO);
		Destroy();
		return true;
	}

	if(pInput->GetKey(4)==1){
		const CElement* ele=mList.GetCursorItem();
		switch(ele->mID){
			case 0:
				if(cdata.Equip(mItem)){
					PlayWave(SE_EQUIP);
					Flag.mPlayerData[Flag.mParty[mCharacter]].OrderItem();
					Destroy();
				}else{
					PlayWave(SE_ERROR);
					mAccessoryTimer=120;
				}
				break;
			case 1:
				ChangeWeapon();
				Destroy();
				break;
			case 2:
				if(UseItem(mCharacter,mItem,-1)){
					Destroy();
				}
				break;
			case 3:
				cdata.CollectItem(mItem);
				Destroy();
				break;
			case 16:
				int y;
				y=mList.GetY();
				y+=mList.GetCursor()*ELEMENT_Height;
				SetMenu(ExchangeItem,y*0x10000+mCharacter*0x100+mItem);
				break;
			case 17:
				SetMenu(DumpItem,Flag.mParty[mCharacter]*0x100+mItem);
				break;
			case 18:
				PlayWave(SE_YES);
				Flag.mPlayerData[Flag.mParty[mCharacter]].OrderItem();
				Destroy();
				break;
		}
	}
	return true;
}
void CItemSubMenu::ChangeWeapon()
{
	CItemMenu* menu=dynamic_cast<CItemMenu*>(MenuData.SearchMenu(typeid( CItemMenu )));
	if(!menu)	return;

	menu->OpenCloset();
}
////////////////////////////////////////////////////////////////////
bool CExchangeItem::OnTimer()
{
	bool timer=CMenuData::OnTimer();

	int item=mItemList.OnTimer(timer);
	mExchangeInfo.OnTimer();
	mCompare.OnTimer();
	int chara=mSWindow.OnTimer(timer);
 
	if(!timer)	return false;


	//UpdataItemList(Flag.mParty[mCharacter],&mItemList);

	if(chara>=0){
		mCharacter=chara;
		ChangeCharacter();
	}
	
	int eqmax=Flag.mPlayerData[Flag.mParty[mCharacter]].GetEquipMax();
	if(item>=0){
		if(item==0){
			mItemList.SetCursor(eqmax);
		}else if(item<eqmax){
			mItemList.SetCursor(100);
		}
		int x,y;
		mItemList.GetCursorPos(&x,&y);
		Cursor.Move(x,y);
	}

	if(pInput->GetKey(6)==1){
		PlayWave(SE_NO);
		Destroy();
		return true;
	}

	if(pInput->GetKey(4)==1){
		PlayWave(SE_EXCHANGE);
		int cursor=mItemList.GetCursor();
		CPlayerData &fromdata=Flag.mPlayerData[mExchangeCharacter];
		CPlayerData &todata=Flag.mPlayerData[Flag.mParty[mCharacter]];

		CarryItem fromitem=fromdata.GetItem(mExchangeItem);
		CarryItem toitem=todata.GetItem(mItemList.GetCursor());

		if(	(fromitem.item == toitem.item) 
			&& (fromitem.item > ID_UI)
			&& (UseItemData[fromitem.item-ID_UI].mType&TYPE_COLLECT))
		{
			if(fromitem.param<1)	fromitem.param=1;
			if(toitem.param<1)		toitem.param=1;
			fromitem.param+=toitem.param;
			fromdata.SetItem(mExchangeItem,fromitem);
			todata.SetItem(cursor,NO_ITEM);
		}else{
			fromdata.SetItem(mExchangeItem,toitem);
			todata.SetItem(cursor,fromitem);
		}

		fromdata.TakeOff(mExchangeItem);
		todata.TakeOff(cursor);

		Destroy();
	}
	return true;
}
////////////////////////////////////////////////////////////////////
bool CDumpItem::OnTimer()
{
	CPlayerData &cdata=Flag.mPlayerData[mCharacter];
	bool timer=CMenuData::OnTimer();

	mList.OnTimer(timer);
	mTitle.OnTimer();

	if(!timer)	return false;

	if(pInput->GetKey(6)==1){
		PlayWave(SE_NO);
		Destroy();
		return true;
	}

	if(pInput->GetKey(4)==1){
		if(mList.GetCursor()==0){
			CarryItem item=cdata.GetItem(mItem);
			cdata.SetItem(mItem,NO_ITEM);
			cdata.TakeOff(mItem);
			Flag.SetCloset(item);
		}else{
			PlayWave(SE_NO);
		}
		Destroy();
	}
	return true;
}
////////////////////////////////////////////////////////////////////
bool CUIMenuSelectCharacter::OnTimer()
{
	bool timer=CMenuData::OnTimer();


	mTitle.OnTimer();

	if(!timer)	return false;

	for(int i=0;i<MAX_PARTY_NUMBER;i++){
		mCDataWindow[i].Move(300-i*50,40+i*CCharaDataWindowMenu::WIN_Height);
	}
	Cursor.Move(mCDataWindow[mCharacter].GetX(),
				mCDataWindow[mCharacter].GetY()+CCharaDataWindowMenu::WIN_Height-40);

	if(mDTime>0){
		mDTime++;
		if(mDTime>20)	Destroy();
		return true;
	}

	if(pInput->GetKey(6)==1){
		PlayWave(SE_NO);
		Destroy();
		return true;
	}

	if(pInput->GetRepeatKey(1)==1){
		PlayWave(SE_MOVE);
		mCharacter=mCharacter--;
		if(mCharacter<0)	mCharacter=MAX_PARTY_NUMBER-1;
	}
	if(pInput->GetRepeatKey(3)==1){
		PlayWave(SE_MOVE);
		mCharacter=mCharacter++;
		mCharacter%=MAX_PARTY_NUMBER;
	}

	if(pInput->GetKey(4)==1){
		Cursor.Click();
		if(UseItem(mUseCharacter,mUseItem,mCharacter)){
			mDTime=1;
			return true;
		}
	}
	if(pInput->GetKey(5)==1){
		SetMenu(StatusMenu,mCharacter);
	}

	return true;
}
//////////////////////////////////////////////////////////////////////////////////
bool CBirthStoneMenu::OnTimer()
{
	bool timer=CMenuData::OnTimer();

	mBirthStoneSystem.OnTimer(timer);


	if(!timer)	return false;

/*	if(pInput->GetKey(1)==1){
		int character=mBirthStoneSystem.GetCharacter();
		int lv=Flag.mPlayerData[Flag.mParty[character]].GetLv();
		lv++;
		lv%=61;
		Flag.mPlayerData[Flag.mParty[character]].mLv=lv;
		Update();
	}
*/	if(pInput->GetKey(6)==1){
		PlayWave(SE_NO);
		Destroy();
		return true;
	}
	if(pInput->GetKey(4)==1){
		int month=mBirthStoneSystem.GetCursorItem()->mID;
		int character=mBirthStoneSystem.GetCharacter();
		if(month<MAX_MONTH){
			SetMenu(LearnSkillMenu,character*0x100+month);
		}else{
			SetMenu(FreeSpaceMenu,character);
		}
	}
	if(pInput->GetKey(5)==1){
		int character=mBirthStoneSystem.GetCharacter();
		SetMenu(StatusMenu,character);
	}
	return true;
}
//////////////////////////////////////////////////////////////////////////////////
bool CLearnSkillMenu::OnTimer()
{
	bool timer=CMenuData::OnTimer();

	int no=mLearnSkill.OnTimer(timer);
	mCompare.OnTimer();
	mMessageWindow[0].OnTimer();
	mMessageWindow[1].OnTimer();
	mHelpWindow.OnTimer();
	mBirthStoneWindow.OnTimer();

	mMessageTimer--;

	if(!timer)	return false;

	if(no>=0){
		MoveCursor(no);
	}

	if(mLearnSkill.GetCursor()>=MAX_SKILL_MONTH-1){
		mActiveWindow=1;
	}else{
		mActiveWindow=0;
	}

	if(pInput->GetKey(6)==1){
		PlayWave(SE_NO);
		Destroy();
		return true;
	}

	if(pInput->GetKey(4)==1){
		Select();
	}

	mMessageWindow[mActiveWindow].Move(mMessageWindow[mActiveWindow].GetX(),Y);
	mMessageWindow[mActiveWindow^1].Move(mMessageWindow[mActiveWindow^1].GetX(),Y+10);
	return true;
}
void	CLearnSkillMenu::SetMessage(const char* mes)
{
	mMessage=mes;
	mMessageTimer=120;
}
void	CLearnSkillMenu::MoveCursor(int no)
{
	mAfter=Flag.mPlayerData[Flag.mParty[mCharacter]];
	mAfter.LearnSkill(mMonth,no);
	mMessageTimer=0;

	delete[] mHelp;

	char fname[256];
	const CElement* ele=mLearnSkill.GetCursorItem();
	if(SkillData[ele->mID].mType==SKILL_COMMAND){
		sprintf(fname,"data\\help\\comskill%02d",(SkillData[ele->mID].mParam)/32);
		mHelp=GetHelpTextShort(fname,SkillData[ele->mID].mParam);
	}else{
		sprintf(fname,"data\\help\\skill%02d",(ele->mID)/32);
		mHelp=GetHelpTextShort(fname,ele->mID);
	}
}
void	CLearnSkillMenu::Select(void)
{
	if(mStoneNo[mActiveWindow]<=0){
		SetMessage("誕生石が足りません！");
		PlayWave(SE_ERROR);
		return;
	}
	mAfter=Flag.mPlayerData[Flag.mParty[mCharacter]];
	int skill=mLearnSkill.GetCursor();
	const int &check=CharacterSkill[Flag.mParty[mCharacter]][mMonth][skill];
	if(SkillData[check].mType==SKILL_NONE){
			SetMessage("このスキルは習得できません！");
			PlayWave(SE_ERROR);
			return;
	}
	int learn=mAfter.LearnSkill(mMonth,skill);
	if(learn<0){
		switch(learn){
			case -2:
				SetMessage("すでに習得しています！");
				PlayWave(SE_ERROR);
				return;
			case -1:
				SetMessage("これ以上習得できません！");
				PlayWave(SE_ERROR);
				return;
		}
	}
	SetMenu(LearnSkillYesNo,skill*0x10000+mCharacter*0x100+mMonth);
}
//////////////////////////////////////////////////////////////////////////////////
bool CLearnSkillYesNo::OnTimer()
{
	CPlayerData &cdata=Flag.mPlayerData[mCharacter];
	bool timer=CMenuData::OnTimer();

	mList.OnTimer(timer);
	mTitle.OnTimer();

	if(!timer)	return false;

	if(pInput->GetKey(6)==1){
		PlayWave(SE_NO);
		Destroy();
		return true;
	}

	if(pInput->GetKey(4)==1){
		if(mList.GetCursor()<=0){
			Flag.mPlayerData[Flag.mParty[mCharacter]].LearnSkill(mMonth,mSkill);
			if(mSkill>=MAX_SKILL_MONTH-1){
				Flag.SpendAllItem(ITEM_BIRTH_STONE+mMonth+MAX_MONTH,true);
			}else{
				Flag.SpendAllItem(ITEM_BIRTH_STONE+mMonth,true);
			}
			PlayWave(SE_LEARN_SKILL);
			Flag.mPlayerData[Flag.mParty[mCharacter]].OrderItem();
		}else{
			PlayWave(SE_NO);
		}

		Destroy();
	}
	return true;
}
////////////////////////////////////////////////////////////////////
bool CStatusMenu::OnTimer()
{
	bool timer=CMenuData::OnTimer();

	mWindow.OnTimer();
	mList.OnTimer(timer);
	mListTitle.OnTimer();
	mListHelp.OnTimer();

	if(!timer)	return false;

	if(pInput->GetKey(6)==1){
		PlayWave(SE_NO);
		Destroy();
		return true;
	}

	if(mChangeable){
		if(pInput->GetKey(0)==1){
			PlayWave(SE_YES);
			Init(mCharacterNum+0x100*2);
		}
		if(pInput->GetKey(2)==1){
			PlayWave(SE_YES);
			Init(mCharacterNum+0x100*1);
		}
	}
	if(pInput->GetKey(4)==1){
		PlayWave(SE_YES);
		ChangeList();
	}
	return true;
}
////////////////////////////////////////////////////////////////////
bool CFormationMenu::OnTimer()
{
	bool timer=CMenuData::OnTimer();

	mWindow.OnTimer();

	if(!timer)	return false;
	mTimer++;

	if(!mSelected){
		Cursor.Move(mCDataWindow[mCharacter].GetX(),
					mCDataWindow[mCharacter].GetY()+CCharaDataWindowMenu::WIN_Height-40);
		if(pInput->GetRepeatKey(1)==1){
			PlayWave(SE_MOVE);
			do{
				mCharacter=mCharacter--;
				if(mCharacter<0)	mCharacter=MAX_PARTY_NUMBER-1;
			}while(Flag.mParty[mCharacter]<0);
		}
		if(pInput->GetRepeatKey(3)==1){
			PlayWave(SE_MOVE);
			do{
				mCharacter=mCharacter++;
				mCharacter%=MAX_PARTY_NUMBER;
			}while(Flag.mParty[mCharacter]<0);
		}

		if(pInput->GetKey(6)==1){
			PlayWave(SE_NO);
			Destroy();
			return true;
		}
		if(pInput->GetKey(4)==1){
			PlayWave(SE_YES);
			mX=Flag.mFormationX[mCharacter];
			mY=Flag.mFormationY[mCharacter];
			mSelected=true;
		}
		if(pInput->GetKey(5)==1){
			SetMenu(StatusMenu,mCharacter);
		}
	}else{
		Cursor.Move(mX*64+32+mWindow.GetX(),
					mY*64+32+mWindow.GetY());
		if(pInput->GetRepeatKey(0)==1){
			PlayWave(SE_MOVE);
			mX--;
			if(mX<0)	mX=3;
		}
		if(pInput->GetRepeatKey(1)==1){
			PlayWave(SE_MOVE);
			mY--;
			if(mY<0)	mY=3;
		}
		if(pInput->GetRepeatKey(2)==1){
			PlayWave(SE_MOVE);
			mX++;
			if(mX>=4)	mX=0;
		}
		if(pInput->GetRepeatKey(3)==1){
			PlayWave(SE_MOVE);
			mY++;
			if(mY>=4)	mY=0;
		}
		if(pInput->GetKey(4)==1){
			for(int i=0;i<MAX_PARTY_NUMBER;i++){
				if(i==mCharacter)	continue;
				if(	Flag.mFormationX[i]==mX
				&&	Flag.mFormationY[i]==mY){
					Flag.mFormationX[i]=Flag.mFormationX[mCharacter];
					Flag.mFormationY[i]=Flag.mFormationY[mCharacter];
					break;
				}
			}
			PlayWave(SE_YES);
			Flag.mFormationX[mCharacter]=mX;
			Flag.mFormationY[mCharacter]=mY;
			mSelected=false;
		}
		if(pInput->GetKey(6)==1){
			PlayWave(SE_NO);
			mSelected=false;
		}
	}
	return true;
}
////////////////////////////////////////////////////////////////////
bool CBoxStartMenu::OnTimer()
{
	bool timer=CMenuData::OnTimer();

	mWindow.OnTimer();
	mList.OnTimer(timer);

	for(int i=0;i<MAX_CHARACTER_NUMBER+MAX_PARTY_NUMBER;i++){
		mCDataWindow[i].OnTimer();
	}

	if(!timer)	return false;

	if(pInput->GetKey(6)==1){
		PlayWave(SE_NO);
		Destroy();
		return true;
	}

	if(pInput->GetKey(4)==1){
		const CElement *ele=mList.GetCursorItem();
		switch(ele->mID){
			case 0:
				SetMenu(BoxSelectMenu,mBox+(mKnowTrap?0x10000:0));
				mList.Move(0,mList.GetY());
				mWindow.Move(0,mWindow.GetY());
				break;
			case 1:
				SetMenu(ItemMenu,0);
				break;
			case 2:
				PlayWave(SE_NO);
				Destroy();
				return true;
		}
	}

	return true;
}
////////////////////////////////////////////////////////////////////
bool CBoxSelectMenu::OnTimer()
{
	bool timer=CMenuData::OnTimer();

	mParamWindow.OnTimer();

	if(!timer)	return false;

	int cx=mCDataWindow[mCharacter].GetX();
	int cy=mCDataWindow[mCharacter].GetY()+CCharaDataWindowMenu::WIN_Height-40;

	Cursor.Move(cx,cy);
//	mParamWindow.Move(cx-110,cy+8);

	if(pInput->GetRepeatKey(1)==1){
		PlayWave(SE_MOVE);
		do{
			mCharacter=mCharacter--;
			if(mCharacter<0)	mCharacter=MAX_PARTY_NUMBER-1;
		}while(Flag.mParty[mCharacter]<0);
	}
	if(pInput->GetRepeatKey(3)==1){
		PlayWave(SE_MOVE);
		do{
			mCharacter=mCharacter++;
			mCharacter%=MAX_PARTY_NUMBER;
		}while(Flag.mParty[mCharacter]<0);
	}

	if(pInput->GetKey(6)==1){
		PlayWave(SE_NO);
		Destroy();
		return true;
	}


	if(pInput->GetKey(4)==1){
		Cursor.Click();
		if(mOpen[mCharacter]>=2){
			PlayWave(SE_ERROR);
			return true;
		}
		for(int i=0;i<MAX_KEY_TYPE;i++){
			if(mUseKey[mCharacter][i]){
				Flag.SpendAllItem(ITEM_KEY + i,true);
			}
		}
		SetMenu(BoxOpenMenu,mBox+mCharacter*0x10000);
	}
	if(pInput->GetKey(5)==1){
		SetMenu(StatusMenu,mCharacter);
	}
	return true;
}
////////////////////////////////////////////////////////////////////
bool COverItemMenu::OnTimer()
{
	bool timer=CMenuData::OnTimer();

	bool ret=mSystem.OnTimer(timer);
	if(!mCDataInited){
		for(int i=0;i<MAX_CHARACTER_NUMBER+MAX_PARTY_NUMBER;i++){
			mCDataWindow[i].OnTimer();
		}
	}

	if(!timer)	return false;

	if(ret){
		PlayWave(SE_NO);
		MenuData.AllDelete();
	}
	return true;
}
////////////////////////////////////////////////////////////////////
bool CItemHelp::OnTimer()
{
	bool timer=CMenuData::OnTimer();

	mWindow.OnTimer();

	if(!timer)	return false;

	if(pInput->GetKey(6)==1){
		PlayWave(SE_NO);
		Destroy();
	}
	return true;
}
////////////////////////////////////////////////////////////////////
bool CSelectHelp::OnTimer()
{
	bool timer=CMenuData::OnTimer();

	mWindow.OnTimer();

	if(!timer)	return false;

	if(pInput->GetKey(6)==1){
		PlayWave(SE_NO);
		Destroy();
		return true;
	}

	if(mListSize>0){
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

		int x=mWindow.GetX();
		int y=mWindow.GetY()+(mCursor+1)*mEleHeight-8+mHeight;
		Cursor.Move(x,y);

		if(pInput->GetKey(4)==1 || pInput->GetKey(5)==1){
			mList[mCursor]->Help(0);
		}
	}
	return true;
}
////////////////////////////////////////////////////////////////////
bool CEquipmentHelp::OnTimer()
{
	bool timer=CSelectHelp::OnTimer();

	if(!timer)	return false;

	return true;
}
////////////////////////////////////////////////////////////////////
bool CEventSkipMenu::OnTimer()
{
	bool timer=CMenuData::OnTimer();

	mList.OnTimer(timer);
	mTitle.OnTimer();

	if(!timer)	return false;

	if(pInput->GetKey(6)==1){
		PlayWave(SE_NO);
		Destroy();
		return true;
	}

	if(pInput->GetKey(4)==1){
		if(mList.GetCursor()==0){
			MapMode.DoEventSkip();
			PlayWave(SE_YES);
		}else{
			PlayWave(SE_NO);
		}
		Destroy();
	}
	return true;
}
////////////////////////////////////////////////////////////////////
bool CSkillHelp::OnTimer()
{
	bool timer=CMenuData::OnTimer();

	mWindow.OnTimer();

	if(!timer)	return false;

	if(pInput->GetKey(6)==1){
		PlayWave(SE_NO);
		Destroy();
	}
	return true;
}
////////////////////////////////////////////////////////////////////
bool CShopMenu::OnTimer()
{
	bool timer=CMenuData::OnTimer();

	int list=mList.OnTimer(timer && (mShopMode==0));
	mTalk.OnTimer();
	mInfo.OnTimer();
	mCompare.OnTimer();
	mPlayerItem.OnTimer(false);

	int chara=mSWindow.OnTimer(mShopMode==1 && timer);

	if(!timer)	return false;

		
	if(mShopMode==0){
		if(list>=0)	GetHelp();
		if(pInput->GetKey(6)==1){
			PlayWave(SE_NO);
			Destroy();
			return true;
		}
		if(pInput->GetKey(4)==1){
			if(CheckMoney()){
				int item=mList.GetCursorItem()->mID;
				if(item<MAX_EQUIPMENT_NUMBER + MAX_SEED_NUMBER){
					PlayWave(SE_YES);
					InitPlayerData();
					mShopMode=1;
				}else{
					Buy();
				}
			}else{
				PlayWave(SE_ERROR);
			}
		}
	}else if(mShopMode==1){
		if(chara>=0){
			mSelectMember=chara;
			ChangePlayerData();
		}
		if(pInput->GetKey(4)==1){
			Cursor.Click();
			if(Buy()){
				ReturnMode();
			}else{
				PlayWave(SE_ERROR);	
			}
		}
		if(pInput->GetKey(6)==1){
			PlayWave(SE_NO);
			ReturnMode();
		}
	}
	return true;
}
bool	CShopMenu::CheckMoney()
{
	int item=mList.GetCursorItem()->mID;
	return AllItem[item]->GetPrice() <= Flag.mMoney;
}
void	CShopMenu::InitPlayerData(void)
{
	if(mSelectMember<ID_CLOSET){
		CPlayerData &cdata=Flag.mPlayerData[mSelectMember];
		int item=mList.GetCursorItem()->mID;
		int y=100;
		if(AllItem[item]->mType & TYPE_EQUIPMENT){
			mCompare.InitWindow(-400,20+32+ITEM_Height*8,item,&cdata);
			mCompare.Open();
			mPlayerItem.InitPlaneWindow(30,660,0,0);
		}
	}

	ChangePlayerData();
}
void	CShopMenu::ChangePlayerData(void)
{
	mCursorTimer=0;
	if(mSelectMember<ID_CLOSET){
		CPlayerData &cdata=Flag.mPlayerData[mSelectMember];

		int item=mList.GetCursorItem()->mID;
		int y=100;
		if(AllItem[item]->mType & TYPE_EQUIPMENT){
			y=20;
			mCompare.Move(30,mCompare.GetY());
			mCompare.SetBefore(&cdata);
			mCompare.SetEquip(item);
		
			int start;
			int maxitem;
			int itemlist[MAX_ITEM];

			if(item<ID_AMR){		//武器
				start=0;						maxitem=WEAPON_SPACE;
			}else if(item<ID_ACC){	//防具
				start=WEAPON_SPACE;				maxitem=ARMOR_SPACE;
			}else{					//アクセサリ
				start=WEAPON_SPACE+ARMOR_SPACE;	maxitem=cdata.GetAccessoryMax();
			}

			for(int i=0;i<maxitem;i++){
				CarryItem citem = cdata.GetItem(i+start);
				itemlist[i]=citem.item;
			}

			mPlayerItem.InitListWindow(	mPlayerItem.GetX(),mPlayerItem.GetY(),ITEM_LIST_WIDTH,
										(const CElement**)AllItem,itemlist,
										maxitem,maxitem);
			mPlayerItem.Move(30,mCompare.GetY()-maxitem*ITEM_Height-31);
			mPlayerItem.Open();
			mPlayerItem.ShowCursor(false);
		}
	}else{
		mCompare.Move(-480,mCompare.GetY());
		mPlayerItem.Move(30,660);
	}
}
void	CShopMenu::ReturnMode(void)
{
	mShopMode=0;
	mCompare.Move(-480,mCompare.GetY());
	mPlayerItem.Move(30,660);
}
bool	CShopMenu::Buy(void)
{
	int item=mList.GetCursorItem()->mID;
	int member=mSelectMember;
	if(item>=MAX_EQUIPMENT_NUMBER)	member=ID_CLOSET;
	if(member>=ID_CLOSET){
		Flag.SetCloset(item);
	}else{
		if(Flag.mPlayerData[member].SetItem(item)<0)	return false;
		Flag.mPlayerData[member].OrderItem();
	}
	PlayWave(SE_BUY_ITEM);
	Flag.mMoney -= AllItem[item]->GetPrice();

	return true;
}
void	CShopMenu::GetHelp(void)
{
	delete[] mHelp;
	mHelp=GetItemTextShort(mList.GetCursorItem()->mID);
}
////////////////////////////////////////////////////////////////////

bool	CFleaMarketMenu::Buy(void)
{
	if(!CShopMenu::Buy())	return false;

	Flag.mMarket[mList.GetCursorItem()->mID]--;
	mList.SetItem(mList.GetCursor(),AllItem[NO_ITEM]);
	Flag.mFleaMarketList[mList.GetCursor()]=NO_ITEM;

	return true;
}

////////////////////////////////////////////////////////////////////
bool	CFarmMenu::OnTimer(void)
{
	bool timer=CMenuData::OnTimer();
	int flist=mList.OnTimer(timer && mMode==0);
	mHelp.OnTimer();
	int smember=mSWindow.OnTimer(timer && mMode==1);
	int plist=mPlayerItem.OnTimer(timer && mMode==1);
	if(mCDataMove){
		for(int i=0;i<MAX_PARTY_NUMBER;i++){
			mCDataWindow[i].OnTimer();
		}
	}
	if(!timer)	return false;

	if(smember>=0){
		mSelectMember=smember;
		InitPlayerItem();
	}
	if(flist>=0 || plist>=0){
		GetHelp();
	}

	switch(mMode){
		case 0:
			SelectFarmItem();		break;
		case 1:
			SelectCharacterItem();	break;
	}
	return true;
}
void	CFarmMenu::SelectFarmItem(void)
{
	if(pInput->GetKey(6)==1){
		PlayWave(SE_NO);
		Destroy();
		return;
	}
	if(pInput->GetKey(4)==1){
		int item=mList.GetCursorItem()->mID;
		int cursor=mList.GetCursor();
		if(mItemNo[cursor]<Flag.GetFarm(item-ID_UI)){
			PlayWave(SE_YES);
			mMode=1;
			SetPlayerItemCursor();
			GetHelp();
		}else{
			PlayWave(SE_ERROR);
		}
	}
}
void	CFarmMenu::SelectCharacterItem(void)
{
	if(pInput->GetKey(6)==1){
		PlayWave(SE_NO);
		mMode=0;
		GetHelp();
		return;
	}
	if(pInput->GetKey(4)==1){
		int item=mList.GetCursorItem()->mID;
		CPlayerData &cdata=Flag.mPlayerData[Flag.mParty[mSelectMember]];		
		int cursor=mPlayerItem.GetCursor();

		cdata.SetItem(cursor+cdata.GetEquipMax(),item);
		PlayWave(SE_GET_ITEM);

		mMode=0;

		UpdataPlayerItem();
		UpdataList();
	}
}
////////////////////////////////////////////////////////////////////
bool	CSeedMenu::OnTimer(void)
{
	bool timer=CMenuData::OnTimer();
	mSeedWindow.OnTimer();
	mWindow.OnTimer();
	if(!timer)	return false;

	Cursor.Escape();
	switch(mTimer){
		case START_TIME:		PlayWave(SE_SEED_THROW);	break;
		case THROW_END_TIME:	PlayWave(SE_SEED_IN_EARTH);	break;
		case NEW_ITEM_TIME:		PlayWave(SE_PRODUCE_UP);	break;
		case END_TIME:			mSeedNo-=mNewItemNo;			break;
	}
	if(THROW_END_TIME<mTimer && mTimer<NEW_ITEM_TIME){
		if(mTimer%SHAKE_TIME==0)	PlayWave(SE_MOVE_BLOCK);
	}

	if(pInput->GetKey(4)==1 || pInput->GetKey(5)==1 || pInput->GetKey(6)==1 ){
		if(mTimer>=END_TIME){
			if(!CheckSeed()){
				Destroy();
				return false;
			}
		}else{
			if(mTimer<NEW_ITEM_TIME)	PlayWave(SE_PRODUCE_UP);
			mTimer=END_TIME;
		}
	}
	mTimer++;
	return true;
}
////////////////////////////////////////////////////////////////////
bool CSellMenu::OnTimer()
{
	bool timer=CMenuData::OnTimer();

	int list=mList.OnTimer(timer && (mShopMode==1));
	mTalk.OnTimer();
	mInfo.OnTimer();

	int chara=mSWindow.OnTimer(mShopMode==0 && timer);

	if(!timer)	return false;

		
	if(mShopMode==1){
		if(list>=0)	GetHelp();
		if(pInput->GetKey(6)==1){
			PlayWave(SE_NO);
			mShopMode=0;
			delete[] mHelp;
			mHelp=NULL;
			return true;
		}
		if(pInput->GetKey(4)==1){
			int character;
			int item;
			int price;
			if(mSelectMember>=ID_CLOSET){
				character=MAX_CHARACTER_NUMBER;
				item=mList.GetCursorItem()->mID;
				price=AllItem[item]->GetSellPrice();
			}else{
				character=mSelectMember;
				item=mList.GetCursor();
				price=AllItem[Flag.mPlayerData[character].GetItem(item).item]->GetSellPrice();
			}

			if(price<=0){
				PlayWave(SE_ERROR);
				return true;
			}else{
				SetMenu(SellYesNoMenu,character*0x10000 + item);
			}
		}
	}else if(mShopMode==0){
		if(chara>=0){
			mSelectMember=chara;
			ChangeCharacter();
		}
		if(pInput->GetKey(4)==1){
			PlayWave(SE_YES);
			mShopMode=1;
			GetHelp();
		}
		if(pInput->GetKey(6)==1){
			PlayWave(SE_NO);
			Destroy();
			return false;
		}
	}
	return true;
}
////////////////////////////////////////////////////////////////////////
bool CSellYesNoMenu::OnTimer()
{
	bool timer=CMenuData::OnTimer();

	mList.OnTimer(timer);
	mTitle.OnTimer();

	if(!timer)	return false;

	mTimer++;
	if(mMoveTimer & 0xff){
		mMoveTimer--;
	}


	if(pInput->GetKey(6)==1){
		PlayWave(SE_NO);
		Destroy();
		return true;
	}
	if(pInput->GetRepeatKey(0)==1){
		if(mSellNo>0){
			mSellNo--;
			if(mSellNo<=0) mSellNo=mSellMax;
			PlayWave(SE_MOVE);
		}
		mMoveTimer=0x100+CURSOR_CLICK_TIME;
	}
	if(pInput->GetRepeatKey(2)==1){
		if(mSellNo>0){
			mSellNo++;
			if(mSellNo>mSellMax)	mSellNo=1;
			PlayWave(SE_MOVE);
		}
		mMoveTimer=0x200+CURSOR_CLICK_TIME;
	}

	if(pInput->GetKey(4)==1){
		if(mList.GetCursor()<=0){
			if(mSellNo<=0)	mSellNo=1;
			Flag.mMoney+=AllItem[mItemID]->GetSellPrice() * mSellNo;
			Flag.mMarket[mItemID]+=mSellNo;
			if(mCharacter<MAX_CHARACTER_NUMBER){
				if(mItemID > ID_UI && (UseItemData[mItemID-ID_UI].mType&TYPE_COLLECT)){
					CarryItem ci=Flag.mPlayerData[mCharacter].GetItem(mItemNo);
					if(ci.param<=1)	ci.param=1;
					ci.param-=mSellNo;
					if(ci.param<=0){
						Flag.mPlayerData[mCharacter].SetItem(mItemNo,NO_ITEM);
					}else{
						Flag.mPlayerData[mCharacter].SetItem(mItemNo,ci);
					}
				}else{
					Flag.mPlayerData[mCharacter].SetItem(mItemNo,NO_ITEM);
				}
				Flag.mPlayerData[mCharacter].TakeOff(mItemNo);
			}else{
				Flag.mCloset[mItemNo]-=mSellNo;
			}
			PlayWave(SE_SELL_ITEM);
		}else{
			PlayWave(SE_NO);
		}
		Destroy();
	}
	return true;
}
////////////////////////////////////////////////////////////////////
bool CReturnVillage::OnTimer()
{
	bool timer=CMenuData::OnTimer();

	mList.OnTimer(timer);
	mTitle.OnTimer();

	if(!timer)	return false;

	if(pInput->GetKey(6)==1){
		PlayWave(SE_NO);
		Destroy();
		return true;
	}

	if(pInput->GetKey(4)==1){
		if(mList.GetCursor()<=0){
			Flag.AllHeal();
			ChangeMapInfo.ChangeMap(-1,0,0,0,0);
			Cursor.Escape();
			PlayWave(SE_YES);
			SetBGMFadeOut(10);
			MapMode.SetBGMWait(-1);
			Flag.mMapBGMNo=0;
		}else{
			PlayWave(SE_NO);
		}
		Destroy();
	}
	return true;
}
////////////////////////////////////////////////////////////////////

bool CDiaryMenu::OnTimer()
{
	bool timer=CMenuData::OnTimer();

	mList.OnTimer(timer && mFileNo<0);
	mYesNo.OnTimer(timer);
	mTitle.OnTimer();

	if(!timer)	return false;

	if(mFileNo<0){
		if(pInput->GetKey(6)==1){
			PlayWave(SE_NO);
			Destroy();
			return true;
		}

		if(pInput->GetKey(4)==1){
			int cursor=mList.GetCursor();
			if(!mData[cursor] && mLoad){
				PlayWave(SE_ERROR);
				return true;
			}
			int y;
			mList.GetCursorPos(NULL,&y);
			if(y>420)	y=420;
			InitYesNoWindow(y);
			mFileNo=mList.GetCursorItem()->mID;
			if(mFileNo<0 && mSuccess){
				Flag.NewGame();
				MenuData.AllDelete();
				Destroy();
				SetBGMFadeOut(20);
			}
			PlayWave(SE_YES);
		}
	}else{
		if(pInput->GetKey(6)==1){
			PlayWave(SE_NO);
			mFileNo=-1;
			mYesNo.Destroy();
		}
		if(pInput->GetKey(4)==1){

			int id=mYesNo.GetCursorItem()->mID;
			switch(id){
				case 0:
					GlobalFlag.mLastSave=mFileNo;
					Flag.Save(mFileNo);
					LoadSaveDataInfo(mList.GetCursor(),mFileNo);
					PlayWave(SE_SAVE);
					break;
				case 1:
					GlobalFlag.mLastSave=mFileNo;
					if(Flag.Load(mFileNo)){
						VillageMode.ResetCursor();
						Cursor.Escape();
						Destroy();
						return true;
					}
					PlayWave(SE_ERROR);
					break;
				case 2:
					if(Flag.LoadSuccessData(mFileNo)){
						SetBGMFadeOut(20);
						VillageMode.ResetCursor();
						Cursor.Escape();
						Flag.NewGame();
						MenuData.AllDelete();
						Destroy();
						return true;
					}
					PlayWave(SE_ERROR);
					break;
				default:
					PlayWave(SE_NO);
					break;
			}
			mFileNo=-1;
			mYesNo.Destroy();
		}
	}
	return true;
}
void CDiaryMenu::InitYesNoWindow(int y)
{
	static const CElement ele[4]={
		CElement("記録する",0),
		CElement("読み込む",1),
		CElement("継承する",2),
		CElement("やめる",3)
	};

	static int list[2]={0,3};
	int x=320;

	if(mSuccess){
		list[0]=2;
		x=440;
	}else if(mLoad){
		list[0]=1;
	}else{
		list[0]=0;
	}

	mYesNo.InitListWindow(	x,y-20,160,
							ele,list,
							2,2,sizeof(CElement));
}
////////////////////////////////////////////////////////////////////
bool CClosetMenu::OnTimer()
{
	bool timer=CMenuData::OnTimer();

	int mode;
	if(mSelect>=0)			mode=2;
	else if(mCharacter>=0)	mode=1;
	else					mode=0;

	int chara	=mSWindow.OnTimer		(timer && mode==0);
	int item	=mItemList.OnTimer		(timer && mode==1);
	int closet	=mClosetList.OnTimer	(timer && mode==2);
	int category=mCategoryWindow.OnTimer(timer && mode==2);
	mEquipWindow.OnTimer();
	mHelpWindow.OnTimer();

	if(!timer)	return false;

	if(mCharacter<0){
		mHelpWindow.Move(HELP_X_1,HELP_Y_1);
	}else{
		int cx=Cursor.GetGoalX();
		if(cx<240){
			mHelpWindow.Move(HELP_X_3,HELP_Y_3);
		}else{
			mHelpWindow.Move(HELP_X_2,HELP_Y_2);
		}
	}
	if(mode==0){//////////////////////////
		
		mClosetList.Home();
		mItemList.Home();
		mCategoryWindow.Home();

		if(chara>=0){
			InitItemList(chara,&mItemList,LIST_X_2,LIST_Y);
		}
		if(pInput->GetKey(6)==1){
			PlayWave(SE_NO);
			Destroy();
			return true;
		}

		if(pInput->GetKey(4)==1){
			PlayWave(SE_YES);
			mCharacter=mSWindow.GetCharacter();
			GetHelp(mItemList.GetCursorItem()->mID);
		}
	}else if(mode==1){////////////////////////
		mEquipWindow.Home();
		if(item>=0)	GetHelp(mItemList.GetCursorItem()->mID);
		if(pInput->GetKey(6)==1){
			PlayWave(SE_NO);
			mCharacter=-1;
			return true;
		}

		if(pInput->GetKey(4)==1){
			PlayWave(SE_YES);
			int cursor=mItemList.GetCursor();
			mSelect=cursor;
			GetHelp(mClosetList.GetCursorItem()->mID);

			mEquipWindow.SetBefore(&Flag.mPlayerData[mCharacter]);
			mEquipWindow.SetEquip(mClosetList.GetCursorItem()->mID);
		}
	}else{///////////////////////////////////////
		mItemList.Move(LIST_X_1,LIST_Y);
		mClosetList.Move(LIST_X_2,45);
		mCategoryWindow.Move(LIST_X_2,8);
		int id=mClosetList.GetCursorItem()->mID;
		if(id < MAX_EQUIPMENT_NUMBER){
			mEquipWindow.Move(HELP_X_2,LIST_Y);
		}else{
			mEquipWindow.Home();
		}

		if(category>=0){
			mCategory = mCategoryWindow.GetCursorItem()->mID;
			UpdataClosetList(&mClosetList,1<<mCategory); 
			closet=0;
		}
		if(closet>=0){
			GetHelp(mClosetList.GetCursorItem()->mID);
			mEquipWindow.SetEquip(mClosetList.GetCursorItem()->mID);
		}
		if(pInput->GetKey(6)==1){
			PlayWave(SE_NO);
			mSelect = -1;
			GetHelp(mItemList.GetCursorItem()->mID);
			return true;
		}
		if(pInput->GetKey(4)==1){
			int closet=Flag.GetCloset(id);
			if(closet){
				PlayWave(SE_YES);
				CPlayerData &cdata=Flag.mPlayerData[mCharacter];
				CarryItem item=cdata.GetItem(mSelect);

				if((id > ID_UI)
					&& (UseItemData[id-ID_UI].mType&TYPE_COLLECT))
				{
					if(item.item == id){
						CarryItem sitem=id;
						sitem.param=closet;
						if(item.param<1)	item.param=1;
						if(sitem.param<1)	sitem.param=1;
						item.param+=sitem.param ;
						cdata.SetItem(mSelect,item);
					}else{
						Flag.SetCloset(item);
						cdata.TakeOff(mSelect);
						CarryItem sitem=id;
						sitem.param=closet;
						cdata.SetItem(mSelect,sitem);
					}
				}else{
					Flag.SetCloset(item);
					cdata.TakeOff(mSelect);
					cdata.SetItem(mSelect,id);
				}
				cdata.TakeOff(mSelect);

				mSelect=-1;
				Update();
			}
			//if(mClosetList.GetCursorItem()==NULL){
			//	mClosetList.SetCursor(mClosetList.GetCursor()-1);
			//}
		}
	}

	return true;
}
void	CClosetMenu::GetHelp(int item)
{
	mHelpWindow.ChangeItem(item);
}
//////////////////////////////////////////////////////////////////////////////////
bool CMonthStartMenu::OnTimer()
{
	bool timer=CMenuData::OnTimer();

	mList.OnTimer(timer);
	
	for(int i=0;i<MAX_CHARACTER_NUMBER+MAX_PARTY_NUMBER;i++){
		mCDataWindow[i].OnTimer();
	}

	if(!timer)	return false;


/*	if(pInput->GetKey(6)==1){
		PlayWave(SE_NO);
		Destroy();
		return true;
	}
*/
	if(pInput->GetKey(4)==1){
		const CElement* ele=mList.GetCursorItem();
		switch(ele->mID){
			case 0:
				PlayWave(SE_YES);
				Destroy();
				Cursor.Escape();
				return true;
			case 1:
				SetMenu(StartMenu,1);
				break;
			case 2:
				SetMenu(ChangeMemberMenu,1);
				break;
			case 3:
				SetMenu(ClosetMenu,256);
				break;
			case 4:
				MenuData.SetMenu(FarmMenu,1);
				break;
			case 5:
				MenuData.SetMenu(DInfoMenu,Flag.mCurrentMonth);
				break;
			case 6:
				mTempMapNo=ChangeMapInfo.mNo;
				ChangeMapInfo.mNo=-1;
				MenuData.SetMenu(DiaryMenu,0);
				break;
		}
	}

	return true;
}
//////////////////////////////////////////////////////////////////////////////////
bool CChangeMemberMenu::OnTimer()
{
	bool timer=CMenuData::OnTimer();


	if(mTop){
		for(int i=0;i<MAX_PARTY_NUMBER;i++){
			CMenuData::mCDataWindow[i].OnTimer();
		}
	}
	for(int i=0;i<mReserveNo;i++){
		mCDataWindow[i].OnTimer();
	}

	if(!timer)	return false;

	if(mChange<0){
		Cursor.Move(CMenuData::mCDataWindow[mCursor].GetX(),
					CMenuData::mCDataWindow[mCursor].GetY()+CCharaDataWindowMenu::WIN_Height-40);

		if(pInput->GetKey(6)==1){
			PlayWave(SE_NO);
			Destroy();
			return true;
		}

		if(pInput->GetKey(4)==1){
			PlayWave(SE_YES);
			mChange=mCursor;
			mCursor=mSelect;
			Cursor.Click();
		}
		if(pInput->GetKey(5)==1){
			SetMenu(StatusMenu,mCursor);
		}
		if(pInput->GetRepeatKey(1)==1){
			PlayWave(SE_MOVE);
			MoveCursor(-1);
		}
		if(pInput->GetRepeatKey(3)==1){
			PlayWave(SE_MOVE);
			MoveCursor(1);
		}
	}else{
		Cursor.Move(mCDataWindow[mCursor].GetX(),
					mCDataWindow[mCursor].GetY()+CCharaDataWindowMenu::WIN_Height-40);
		if(pInput->GetKey(6)==1){
			PlayWave(SE_NO);
			mCursor=mChange;
			mChange=-1;
			return true;
		}
		if(pInput->GetKey(5)==1){
			int character=mReserve[mCursor];
			if(character>=0){
				SetMenu(StatusMenu,Flag.SearchMember(character)+32-MAX_PARTY_NUMBER);
			}
		}
		if(pInput->GetRepeatKey(1)==1){
			PlayWave(SE_MOVE);
			mCursor--;
			if(mCursor<0)	mCursor=mReserveNo-1;
		}
		if(pInput->GetRepeatKey(3)==1){
			PlayWave(SE_MOVE);
			mCursor++;
			if(mCursor>=mReserveNo)	mCursor=0;
		}
		mSelect=mCursor;
		if(pInput->GetKey(4)==1){
			Cursor.Click();
			if(mChangeOk[mCursor]){
				PlayWave(SE_YES);
				Flag.ChangeMember(mChange,mReserve[mCursor]);

				CMenuData::mCDataWindow[mChange].InitWindow(
								CMenuData::mCDataWindow[mChange].GetX(),
								CMenuData::mCDataWindow[mChange].GetY(),
								Flag.mParty[mChange]);
				InitCDWin();
				mCursor=mChange;
				mChange=-1;
			}else{
				PlayWave(SE_ERROR);
			}
		}
	}
	MoveCDWin(mSelect);
	return true;
}
//////////////////////////////////////////////////////////////////////////////////
bool CConfigMenu::OnTimer()
{
	bool timer=CMenuData::OnTimer();

	mAll.OnTimer(timer);
	mHelp.OnTimer();
	int cursor=mAll.GetCursor();
	for(int i=0;i<MENU_NO;i++){
		if (cursor!=i){
			mSelectWindow[i].InitArrow();
		}
		if(mSelectWindow[i].OnTimer(timer && (cursor==i))>=0){
			UpData();
		}
	}

	if(!timer)	return false;

	if(pInput->GetKey(6)==1){
		PlayWave(SE_NO);
		Destroy();
		return true;
	}
	
	if(cursor==MENU_NO){
		if(pInput->GetKey(4)==1){
			SetMenu(KeyConfigMenu,0);
		}
	}

	return true;
}
void	CConfigMenu::UpData(void)
{
	if(Flag.mBGM!=(mSelectWindow[0].GetCursorItem()->mID!=0)){
		Flag.mBGM=(mSelectWindow[0].GetCursorItem()->mID!=0);
		if(Flag.mBGM){
			int b=Flag.mBGMNo;
			Flag.mBGMNo=0;
			PlayBGM(b);
		}else{
#ifndef _NO_USE_GGSOUND_
			GGSound_Stop(0);
#endif
			//PlayBGM(0);		
		}
	}

	Flag.mSE =(mSelectWindow[1].GetCursorItem()->mID!=0);
	Flag.mUseSaveCursor =mSelectWindow[2].GetCursorItem()->mID;
	Flag.mBattleSpeed=(mSelectWindow[3].GetCursorItem()->mID!=0);
	Flag.mMapMoveTurn=(mSelectWindow[4].GetCursorItem()->mID!=0);
	Flag.mAutoDash=(mSelectWindow[5].GetCursorItem()->mID!=0);

	GlobalFlag.mFullScreen=(mSelectWindow[6].GetCursorItem()->mID!=0);
	GlobalFlag.Save();
}
//////////////////////////////////////////////////////////////////////////////////
bool CFreeSpaceMenu::OnTimer()
{
	bool timer=CMenuData::OnTimer();

	int no=mLearnSkill.OnTimer(timer);

	if(!timer)	return false;

	if(no>=0){
		MoveCursor(no);
	}

	if(pInput->GetKey(6)==1){
		PlayWave(SE_NO);
		Destroy();
		return true;
	}

	if(pInput->GetKey(4)==1){
		int cursor=mLearnSkill.GetCursor();
		SetMenu(FSBirthStone,mCharacter+cursor*0x100);
	}
	return true;
}
void	CFreeSpaceMenu::MoveCursor(int no)
{
	delete[] mHelp;
	char fname[256];
	const CElement* ele=mLearnSkill.GetCursorItem();

	if(ele->mID>=0){
		if(SkillData[ele->mID].mType==SKILL_COMMAND){
			sprintf(fname,"data\\help\\comskill%02d",(SkillData[ele->mID].mParam)/32);
			mHelp=GetHelpTextShort(fname,SkillData[ele->mID].mParam);
		}else{
			sprintf(fname,"data\\help\\skill%02d",(ele->mID)/32);
			mHelp=GetHelpTextShort(fname,ele->mID);
		}

		int learn=Flag.CheckLearnSkill(ele->mID);
		for(int i=0;i<MAX_PARTY_NUMBER;i++){
			mLearnCharacter[i]=((learn & (1<<i))!=0);
		}
	}else{
		mHelp=NULL;
		memset(mLearnCharacter,0,sizeof(mLearnCharacter));
	}
}

//////////////////////////////////////////////////////////////////////////////////

bool CFSBirthStoneMenu::OnTimer()
{
	bool timer=CMenuData::OnTimer();

	mBirthStoneSystem.OnTimer(timer);


	if(!timer)	return false;
	if(pInput->GetKey(6)==1){
		PlayWave(SE_NO);
		Destroy();
		return true;
	}
	if(pInput->GetKey(4)==1){
		int month=mBirthStoneSystem.GetCursorItem()->mID;
		int character=mBirthStoneSystem.GetCharacter();
		if(month<MAX_MONTH){
			SetMenu(FSSelectSkill,mFreeSpace*0x01000000+mFreeSpaceCharacter*0x010000+character*0x100+month);
		}else{
			PlayWave(SE_ERROR);
		}
	}
	if(pInput->GetKey(5)==1){
		int character=mBirthStoneSystem.GetCharacter();
		SetMenu(StatusMenu,character);
	}
	return true;
}
//////////////////////////////////////////////////////////////////////////////////

bool CFSSelectSkillMenu::OnTimer()
{
	bool timer=CLearnSkillMenu::OnTimer();

	mOldSkill.OnTimer();

	if(!timer)	return false;
	return true;
}

void	CFSSelectSkillMenu::MoveCursor(int no)
{
	mAfter=Flag.mPlayerData[Flag.mParty[mFreeSpaceCharacter]];
	if(CheckSkill()==0){
		int skill=CharacterSkill[Flag.mParty[mCharacter]][mMonth][mLearnSkill.GetCursor()];
		mAfter.SetFSpace(mFreeSpace,skill);
	}
	mMessageTimer=0;

	delete[] mHelp;

	char fname[256];
	const CElement* ele=mLearnSkill.GetCursorItem();
	if(SkillData[ele->mID].mType==SKILL_COMMAND){
		sprintf(fname,"data\\help\\comskill%02d",(SkillData[ele->mID].mParam)/32);
		mHelp=GetHelpTextShort(fname,SkillData[ele->mID].mParam);
	}else{
		sprintf(fname,"data\\help\\skill%02d",(ele->mID)/32);
		mHelp=GetHelpTextShort(fname,ele->mID);
	}
}
void	CFSSelectSkillMenu::Select(void)
{
	int error=CheckSkill();
	if(error){
		switch(error){
			case -1:
				SetMessage("このスキルは未修得です！");		break;
			case -2:
				SetMessage("このスキルは設定できません！");	break;
		}
		PlayWave(SE_ERROR);
		return;
	}

	int skill=CharacterSkill[Flag.mParty[mCharacter]][mMonth][mLearnSkill.GetCursor()];
	Flag.mPlayerData[Flag.mParty[mFreeSpaceCharacter]].SetFSpace(mFreeSpace,skill);
	PlayWave(SE_LEARN_SKILL);
	MenuData.Delete(typeid(CFSBirthStoneMenu));
}

int	CFSSelectSkillMenu::CheckSkill(void)
{
	CPlayerData test=Flag.mPlayerData[Flag.mParty[mCharacter]];
	int skill=mLearnSkill.GetCursor();

	//星付きの確認
	int smax=MAX_SKILL_MONTH-1;
	if(Flag.mPlayerData[Flag.mParty[mFreeSpaceCharacter]].SearchASkill(312))	smax++;
	if(skill>=smax){
		return -2;
	}

	//アイテムアップ、アクセサリアップを除く例外処理！
	int id=CharacterSkill[Flag.mParty[mCharacter]][mMonth][skill];
	if(45<=id && id<=47){
		return -2;
	}

	//習得しているかの確認
	if(test.CheckLearnSkill(mMonth,skill)){
		return 0;
	}
	return -1;
}
//////////////////////////////////////////////////////////////////////////////////
bool CGlobalConfigMenu::OnTimer()
{
	bool timer=CMenuData::OnTimer();

	mAll.OnTimer(timer);
	int cursor=mAll.GetCursor();
	mSelectWindow.OnTimer(timer && (cursor==0));

	if(!timer)	return false;

	if(pInput->GetKey(6)==1){
		PlayWave(SE_NO);
		Destroy();
		return true;
	}
	if(cursor==1){
		if(pInput->GetKey(4)==1){
			SetMenu(KeyConfigMenu,0);
		}
	}

	return true;
}
//////////////////////////////////////////////////////////////////////////////////
bool CKeyConfigMenu::OnTimer()
{
	bool timer=CMenuData::OnTimer();

	mList.OnTimer(true);

	if(!timer)	return false;
	int cursor=mList.GetCursorItem()->mID;

/*	if(DTest.GetKey(DIK_ESCAPE)==1){
		PlayWave(SE_NO);
		for(int n=0;n<MAX_KEY;n++){
			GlobalFlag.mKeyConfig[n]=pInput->GetKeyCustom(n);
		}
		Destroy();
		return true;
	}
*/
	if(cursor>=0){
		for(int i=4;i<20;i++){
			if(DTest.GetJoyButton(i)==1){
				GlobalFlag.mKeyConfig[cursor]=i;
				GlobalFlag.Save();
				PlayWave(SE_YES);
				break;
			}
		} 
	}else{
		if(pInput->GetRepeatKey(4)){
			PlayWave(SE_LV_UP);
			Destroy();
			return true;
		}
	}

	return true;
}
//////////////////////////////////////////////////////////////////////////////////
bool CDInfoMenu::OnTimer()
{
	bool timer=CMenuData::OnTimer();

	mList.OnTimer(timer);
	mWindow.OnTimer();

	if(!timer)	return false;

	if(pInput->GetKey(6)==1){
		PlayWave(SE_NO);
		Destroy();
		return true;
	}
	if(pInput->GetKey(4)==1){
		mList.GetCursorItem()->Help(0);
	}
	return true;
}
//////////////////////////////////////////////////////////////////////////////////
bool CBypathMenu::OnTimer()
{
	bool timer=CMenuData::OnTimer();

	mList.OnTimer(timer);
	mTitle.OnTimer();

	if(!timer)	return false;

	if(pInput->GetKey(6)==1){
		PlayWave(SE_NO);
		Destroy();
		return true;
	}

	if(pInput->GetKey(4)==1){
		if(mList.GetCursor()<=0){
			MenuData.SetMenu(DiaryMenu,0);
		}else{
			Flag.mBypathMapNo	= ChangeMapInfo.mNo;
			Flag.mBypathX		= ChangeMapInfo.mX;
			Flag.mBypathY		= ChangeMapInfo.mY;
			Flag.mBypathZ		= ChangeMapInfo.mZ;
			Flag.mBypathDir		= ChangeMapInfo.mDir;
			Flag.mBypathBGM		= Flag.mMapBGMNo;
			ChangeMapInfo.ChangeMap(-1,0,0,0,0);
			Flag.AllHeal();
			Cursor.Escape();
			PlayWave(SE_YES);
			Destroy();
		}
		//Destroy();
	}
	return true;
}
////////////////////////////////////////////////////////////////////
bool CStatusHelp::OnTimer()
{
	bool timer=CMenuData::OnTimer();

	mWindow.OnTimer();

	if(!timer)	return false;

	if(pInput->GetKey(6)==1){
		PlayWave(SE_NO);
		Destroy();
	}
	return true;
}
////////////////////////////////////////////////////////////////////
bool CRemoveBirthStoneMenu::OnTimer()
{
	bool timer=CMenuData::OnTimer();
	mErrorTimer--;

	int list=mSWindow.OnTimer(timer);
	mWindow.OnTimer();

	if(!timer)	return false;

	if(list>=0){
		mErrorTimer=0;
	}

	if(pInput->GetKey(6)==1){
		PlayWave(SE_NO);
		Destroy();
	}
	if(pInput->GetKey(4)==1){
		int id=mSWindow.GetCursorItem()->mID;
		SetMenu(RemoveBSPersonal,id);
	}

	return true;
}
////////////////////////////////////////////////////////////////////
bool CRemoveBSPersonal::OnTimer()
{
	bool timer=CMenuData::OnTimer();

	mList.OnTimer(timer && mMode==0);
	int month=mMonthList.OnTimer(timer && mMode==1);
	mSkillList.OnTimer();
	mCDataWin.OnTimer();


	if(!timer)	return false;

	if(mMode==0){
		mSkillList.Home();
		if(pInput->GetKey(6)==1){
			PlayWave(SE_NO);
			Destroy();
			return true;
		}

		if(pInput->GetKey(4)==1){
			CPlayerData &cdata=Flag.mPlayerData[mCharacter];
			switch(mList.GetCursor()){
				case 0:
					for(int i=0;i<MAX_MONTH;i++){
						int bs= cdata.GetLearnSkillNo(i);
						bool star =cdata.CheckLearnSkill(i,MAX_SKILL_MONTH-1);
						if(star){
							Flag.SetCloset(ITEM_BIRTH_STONE + MAX_MONTH + i);
						}
						Flag.SetCloset(ITEM_BIRTH_STONE + i,bs);
					}
					if(cdata.ClearAllLearnSkill()){
						Flag.GetCloset(ITEM_BIRTH_STONE+24,true);	//初心戻し用例外処理
					}
					ResetMonthList();
					mList.SetCursor(3);
					PlayWave(SE_REMOVE_SKILL);
					cdata.OrderItem();
					break;
				case 1:
					for(int i=0;i<MAX_MONTH;i++){
						bool star =cdata.CheckLearnSkill(i,MAX_SKILL_MONTH-1);
						if(star){
							Flag.SetCloset(ITEM_BIRTH_STONE + MAX_MONTH + i);
						}
					}
					if(cdata.ClearAllStarSkill()){
						Flag.GetCloset(ITEM_BIRTH_STONE+24,true);	//初心戻し用例外処理
					}
					ResetMonthList();
					mList.SetCursor(3);
					PlayWave(SE_REMOVE_SKILL);
					cdata.OrderItem();
					break;
				case 2:
					mMode=1;
					PlayWave(SE_YES);
					break;
				case 3:
					PlayWave(SE_NO);
					Destroy();
					break;
			}
		}
	}else if(mMode==1){
		mSkillList.Move(30,MONTH_LIST_Y+100);
		if(month>=0){
			InitSkillList();
		}

		if(pInput->GetKey(6)==1){
			PlayWave(SE_NO);
			mMode=0;
			return false;
		}

		if(pInput->GetKey(4)==1){
			CPlayerData &cdata=Flag.mPlayerData[mCharacter];
			int cursor=mMonthList.GetCursor();
			int bs= cdata.GetLearnSkillNo(cursor);
			bool star =cdata.CheckLearnSkill(cursor,MAX_SKILL_MONTH-1);
			if(star){
				Flag.SetCloset(ITEM_BIRTH_STONE + MAX_MONTH + cursor);
			}
			Flag.SetCloset(ITEM_BIRTH_STONE + cursor,bs);
			if(cdata.ClearMonthLearnSkill(cursor)){
				Flag.GetCloset(ITEM_BIRTH_STONE+24,true);	//初心戻し用例外処理
			}
			ResetMonthList();
			PlayWave(SE_REMOVE_SKILL);
			cdata.OrderItem();
		}
	}
	return true;
}
////////////////////////////////////////////////////////////////////
bool CSelectDifficult::OnTimer()
{
	bool timer=CMenuData::OnTimer();

	mList.OnTimer(timer);
	mTitle.OnTimer();
	mHelp.OnTimer();

	if(!timer)	return false;

	if(pInput->GetKey(6)==1){
		PlayWave(SE_NO);
		Destroy();
		return true;
	}

	if(pInput->GetKey(4)==1){
		int diff=mList.GetCursor();
#ifdef _HARD_LOCK_
		if(diff==1){
			PlayWave(SE_ERROR);
		}else{
#endif
			Flag.mHardMode=diff;
			PlayWave(SE_YES);
//セーブデータがあるときは、継承メニューを呼び出します。
			//Destroy();
			MenuData.SetMenu(SuccessSaveData,0);
				
#ifdef _HARD_LOCK_
		}
#endif
	}
	return true;
}
////////////////////////////////////////////////////////////////////
bool CExitYesNo::OnTimer()
{
	bool timer=CMenuData::OnTimer();

	mList.OnTimer(timer);
	mTitle.OnTimer();

	if(!timer)	return false;

	bool exit=false;

	if(pInput->GetKey(6)==1){
		PlayWave(SE_NO);
		Destroy();
		return true;
	}

	if(pInput->GetKey(4)==1){
		if(mList.GetCursor()<=0){
			exit=true;
		}else{
			PlayWave(SE_NO);
			Destroy();
		}
	}
	
	if(DTest.GetKey(DIK_ESCAPE)==1 || exit){
		SetBGMFadeOut(20);
		ChangeMapInfo.ChangeMap(-2,0,0,0,0);
		Destroy();
		MenuData.AllDelete();
	}
	return true;
}
////////////////////////////////////////////////////////////////////
bool CSuccessSaveDataMenu::OnTimer()
{
	mHelp.OnTimer();
	return CDiaryMenu::OnTimer();
}
////////////////////////////////////////////////////////////////////
bool CCallPetHelp::OnTimer()
{
	bool timer=CSelectHelp::OnTimer();

	if(!timer)	return false;

	return true;
}
////////////////////////////////////////////////////////////////////
bool CTutorialMenu::OnTimer()
{
	bool timer=CMenuData::OnTimer();

	mWindow.OnTimer();

	if(!timer)	return false;

	if(pInput->GetKey(4)==1 || pInput->GetKey(2)==1){
		mPage++;
		if(mPage>=mMaxPage){
			mPage=mMaxPage-1;
			PlayWave(SE_NO);
			Destroy();
			return true;
		}
		PlayWave(SE_MOVE);
		ChangePage(mPage);
	}

	if(pInput->GetKey(6)==1 || pInput->GetKey(0)==1){
		mPage--;
		if(mPage<0){
			mPage=0;
		}
		PlayWave(SE_MOVE);
		ChangePage(mPage);
	}
	return true;
}
////////////////////////////////////////////////////////////////////
bool CSoundTestMenu::OnTimer()
{
	bool timer=CMenuData::OnTimer();

	mBGMWindow.OnTimer(timer);
	mCurrent.OnTimer();
	if(!timer)	return false;

	if(pInput->GetKey(6)==1){
		if(Flag.mBGMNo!=0){
			SetBGMFadeOut(20);
		}else{
			PlayBGM(25);
			PlayWave(SE_NO);
			Destroy();
			return true;
		}
	}
	if(pInput->GetKey(4)==1){
		int id=mBGMWindow.GetCursorItem()->mID;
		PlayBGM(id,true);
	}
	return true;
}
//////////////////////////////////////////////////////////////////////////////////
bool CPreciousItemMenu::OnTimer()
{
	bool timer=CMenuData::OnTimer();

	mHelpWindow.OnTimer();

	int category=mCategoryWindow.OnTimer(timer);
	int closet=mClosetList.OnTimer(timer);
 
	if(!timer)	return false;


	if(category>=0){
		mCategory = mCategoryWindow.GetCursorItem()->mID;
		int cat=1<<mCategory;
		if(mCategory<=0)	cat=mCategoryAll;
		UpdataClosetList(&mClosetList,cat,false); 
		ChangeHelp();
		closet=0;
	}
	if(closet>=0){
		ChangeHelp();
	}

	if(pInput->GetKey(6)==1){
		PlayWave(SE_NO);
		Destroy();
		return true;
	}
	if(pInput->GetKey(4)==1){
		mClosetList.GetCursorItem()->Help(0);
	}

	return true;
}