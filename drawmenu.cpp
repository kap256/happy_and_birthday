#include "menudata.h"
#include "flag.h"
#include "itemdata.h"
#include "math.h"
#include "month.h"
#include "cursor.h"
#include "option.h"
#include "bgmdata.h"


void CStartMenu::OnDraw()
{
	mWindow.OnDraw();

	mInfo.OnDraw();

	static char text[256];
	mInfo.DrawText(0,Flag.mMapName,10,-5,0,DT_RIGHT);
	mInfo.DrawText(0,"�������F",160,15,0,DT_RIGHT);
	sprintf(text,"%d M",Flag.mMoney);
	mInfo.DrawText(0,text,10,15,0,DT_RIGHT);
	mInfo.DrawText(0,"�v���C���ԁF",160,35,0,DT_RIGHT);
	int sec=(Flag.mPlayTime/(60))%60;
	int min=(Flag.mPlayTime/(60*60))%60;
	int hour=(Flag.mPlayTime/(60*60*60));
	sprintf(text,"%d : %02d : %02d",hour,min,sec);
	mInfo.DrawText(0,text,10,35,0,DT_RIGHT);

	for(int i=0;i<MAX_CHARACTER_NUMBER+MAX_PARTY_NUMBER;i++){
		mCDataWindow[i].OnDraw(mNextMenu>=0 && mCharacter==i);
	}
}
////////////////////////////////////////////////////////////////////

void CItemMenu::OnDraw()
{
	mItemList.OnDraw();
	mSWindow.OnDraw();

	mCategoryWindow.OnDraw();
	mClosetList.OnDraw();

	mCompare.OnDraw();
	mHelpWindow.OnDraw();
}
////////////////////////////////////////////////////////////////////

void CItemSubMenu::OnDraw()
{
	mTitle.OnDraw();

	if(mAccessoryTimer>0){
		char text[64];
		sprintf(text,"�A�N�Z�T���[�� %d �܂łł��I",Flag.mPlayerData[Flag.mParty[mCharacter]].GetAccessoryMax());
		mTitle.DrawText(0,text,0,8,0xffff0000);
	}else{
		mTitle.DrawBox(0,ITEM_Height-12,ITEM_LIST_WIDTH-32,8,COLOR_SELECT);
		CPlayerData &cdata=Flag.mPlayerData[Flag.mParty[mCharacter]];
		int item=cdata.GetItem(mItem).item;
		AllItem[item]->Draw(&mTitle,20,1,0);
	}

	mCompare.OnDraw();
	mList.OnDraw();
}
////////////////////////////////////////////////////////////////////
void CExchangeItem::OnDraw()
{
	CPlayerData &cdata=Flag.mPlayerData[mExchangeCharacter];


	mItemList.OnDraw();
	mSWindow.OnDraw();
	mCompare.OnDraw();

	mExchangeInfo.OnDraw();
	mExchangeInfo.DrawText(0,cdata.GetName(),0,0);
	mExchangeInfo.DrawBox(0,20,320-32,2,COLOR_LINE);
	int item=cdata.GetItem(mExchangeItem).item;
	AllItem[item]->Draw(&mExchangeInfo,20,25,0);
}
////////////////////////////////////////////////////////////////////

void CDumpItem::OnDraw()
{
	mTitle.OnDraw();
	CPlayerData &cdata=Flag.mPlayerData[mCharacter];
	int item=cdata.GetItem(mItem).item;
	AllItem[item]->Draw(&mTitle,0,0,0);
	
	if(ID_UI<=AllItem[item]->mID && AllItem[item]->mID <ID_NSUI){
		mTitle.DrawText(0,"���̂Ă܂����H",0,40,0,DT_RIGHT);
	}else{
		mTitle.DrawText(0,"��q�ɂɑ���܂����H",0,40,0,DT_RIGHT);
	}

	mList.OnDraw();
}
////////////////////////////////////////////////////////////////////
void CUIMenuSelectCharacter::OnDraw()
{
	mTitle.OnDraw();
	CarryItem citem;
	if(mUseCharacter>=0){
		citem=Flag.mPlayerData[Flag.mParty[mUseCharacter]].GetItem(mUseItem);
	}else{
		citem=Flag.mTempItem[mUseItem];
	}

	AllItem[citem.item]->Draw(&mTitle,0,0,citem.param * 0x10000 + 1024);

	for(int i=0;i<MAX_CHARACTER_NUMBER+MAX_PARTY_NUMBER;i++){
		bool select=false;
		if(mCharacter==i)	select=true;
		mCDataWindow[i].OnDraw(select);
	}
}
////////////////////////////////////////////////////////////////////
void CBirthStoneMenu::OnDraw()
{
	mBirthStoneSystem.OnDraw();
}
////////////////////////////////////////////////////////////////////
void CLearnSkillMenu::OnDraw()
{
	mCDataWindow[mCharacter].OnDraw();
	int noactive=mActiveWindow^1;
	mMessageWindow[noactive].OnDraw( );
	AllItem[ITEM_BIRTH_STONE+mMonth+noactive*MAX_MONTH]->Draw(&mMessageWindow[noactive],0,-4,0);
	char text[64];
	sprintf(text,"�~%d",mStoneNo[noactive]);
	mMessageWindow[noactive].DrawText(0,text,10,0,0,DT_RIGHT);

	mMessageWindow[mActiveWindow].OnDraw();
	if(mMessageTimer>0){
		mMessageWindow[mActiveWindow].DrawText(0,mMessage,0,0,0xffff0000);
	}else{
		AllItem[ITEM_BIRTH_STONE+mMonth+mActiveWindow*MAX_MONTH]->Draw(&mMessageWindow[mActiveWindow],0,-4,0);
		char text[64];
		sprintf(text,"�~%d",mStoneNo[mActiveWindow]);
		mMessageWindow[mActiveWindow].DrawText(0,text,10,0,0,DT_RIGHT);
	}

	mLearnSkill.OnDraw();
	mCompare.OnDraw();

	mHelpWindow.OnDraw();
	mHelpWindow.DrawMessage(0,mHelp,0,-20);

	mBirthStoneWindow.OnDraw();
	mBirthStone->Draw(&mBirthStoneWindow,20,-8,mBSParam);
}
////////////////////////////////////////////////////////////////////

void CLearnSkillYesNo::OnDraw()
{
	mTitle.OnDraw();
	int skill=CharacterSkill[Flag.mParty[mCharacter]][mMonth][mSkill];
	SkillData[skill].Draw(&mTitle,0,0,0);
	mTitle.DrawText(0,"���K�����܂����H",0,40,0,DT_RIGHT);

	mList.OnDraw();
}
////////////////////////////////////////////////////////////////////
void CStatusMenu::OnDraw()
{
	mList.OnDraw();
	mListTitle.OnDraw();
	switch(mListType){
		case 0:
			mListTitle.DrawText(0,"�A�C�e��",0,-4);			break;
		case 1:
			mListTitle.DrawText(0,"�Z�E���@�E����",0,-4);	break;
		case 2:
			mListTitle.DrawText(0,"�I�[�g�E�t�B�[���h�X�L��",0,-4);		break;
		case 3:
			mListTitle.DrawText(0,"�X�e�[�^�X�ُ�",0,-4);	break;
	}
	mListHelp.OnDraw();
	if(Flag.mPlayTime %60<40){
		if(mChangeable){
			mListHelp.DrawText(2,"���E�ŃL�����N�^�[�A����L�[�Ń��X�g�؂�ւ�",0,-8,0,DT_RIGHT);
		}else{
			mListHelp.DrawText(2,"����L�[�Ń��X�g�؂�ւ�",0,-8,0,DT_RIGHT);
		}
	}
	mWindow.OnDraw();
	DrawStatus();
}
void	CStatusMenu::DrawStatus()
{
	if(mWindow.GetInited()){
		int size=mCharacter->GetSize();
		mCharacter->OnDraw(mWindow.GetX()-64*size+32,mWindow.GetY()-64*size+32);
	}

	char text[256];
	CPlayerData* pdata=mPlayer->GetPData();
	sprintf(text,"%s\n   %s",pdata->GetName(),pdata->GetFamilyName());
	mWindow.DrawText(0,text,64,0);	

	sprintf(text,"Lv.%d",mCharacter->GetLv());
	mWindow.DrawText(0,text,100,40);	
	sprintf(text,"Exp.%d",mCharacter->GetExp());
	mWindow.DrawText(0,text,180,40);	

	mWindow.DrawText(0,"LP",0,80);
	sprintf(text,"%d",pdata->GetLP());
	mWindow.DrawText(0,text,TOTAL_X-30,80,0,DT_RIGHT);	
	mWindow.DrawText(0,"/",TOTAL_X-40,80,0,DT_RIGHT);	
	sprintf(text,"%d",pdata->GetMaxLP());
	mWindow.DrawText(0,text,TOTAL_X-80,80,0,DT_RIGHT);	

	mWindow.DrawText(0,"SP",0,110);
	static const int SP_X=TOTAL_X-80;
	int j=0;
	while(1){
		sprintf(text,"%d",pdata->GetSP(j));
		mWindow.DrawText(0,text,SP_X+j*30,110,0,DT_RIGHT);
		j++; 
		if(j>=MAX_CSKILL_LEVEL)	break;
		mWindow.DrawText(0,"/",SP_X+j*30-8,110,0,DT_RIGHT);
	}
	mWindow.DrawText(0,"�ő�SP",0,135);
	j=0;
	while(1){
		sprintf(text,"%d",pdata->GetMaxSP(j));
		mWindow.DrawText(0,text,SP_X+j*30,135,0,DT_RIGHT);
		j++;
		if(j>=MAX_CSKILL_LEVEL)	break;
		mWindow.DrawText(0,"/",SP_X+j*30-8,135,0,DT_RIGHT);
	}

	DrawParam("�U����"	,0,pdata->GetAtk()		,pdata->GetBaseAtk());
	DrawParam("�h���"	,1,pdata->GetDef()		,pdata->GetBaseDef());
	DrawParam("���@�U��",2,pdata->GetMagAtk()	,pdata->GetBaseMagAtk());
	DrawParam("���@�h��",3,pdata->GetMagDef()	,pdata->GetBaseMagDef());
	DrawParam("���΂₳",5,pdata->GetSpeed()	,pdata->GetBaseSpeed());
	
	int y=START_Y+int(LINE_Y*6.5);
	mWindow.DrawText(0,"���Օi -",0,y);
	sprintf(text,"%2d/%2d",pdata->GetItemCurrent(),pdata->GetUseItemMax());
	mWindow.DrawText(0,text,140,y,0,DT_RIGHT);	
	mWindow.DrawText(0,"�����i -",150,y);
	sprintf(text,"%d/%d",pdata->GetAccessoryCurrent(),pdata->GetAccessoryMax());
	mWindow.DrawText(0,text,5,y,0,DT_RIGHT);	

}
void	CStatusMenu::DrawParam(const char* name,int no,int param,int base,bool open)
{
	char text[256];
	int y=START_Y+LINE_Y*no;
	mWindow.DrawText(0,name,0,y);

	mWindow.DrawText(0,"(",BASE_X+40,y,0,DT_RIGHT);	
	mWindow.DrawText(0,")",0,y,0,DT_RIGHT);	

	if(open){
		sprintf(text,"%d",base);
		mWindow.DrawText(0,text,BASE_X,y,0,DT_RIGHT);	
		sprintf(text,"%+d",param-base);
		mWindow.DrawText(0,text,ADD_X,y,0,DT_RIGHT);	
		sprintf(text,"%d",param);
		mWindow.DrawText(0,text,TOTAL_X,y,0,DT_RIGHT);	
	}else{
		mWindow.DrawText(0,"???",BASE_X,y,0,DT_RIGHT);	
		sprintf(text,"%+d",param-base);
		mWindow.DrawText(0,text,ADD_X,y,0,DT_RIGHT);	
		mWindow.DrawText(0,"???",TOTAL_X,y,0,DT_RIGHT);	
	}
}

////////////////////////////////////////////////////////////////////

void CFormationMenu::OnDraw()
{
	int i;
	mWindow.OnDraw();
	for(int x=0;x<=4;x++){
		mWindow.DrawBox(16+x*64,16,2,258,COLOR_LINE);
	}
	for(int y=0;y<=4;y++){
		mWindow.DrawBox(16,16+y*64,258,2,COLOR_LINE);
	}


	for(i=0;i<MAX_CHARACTER_NUMBER+MAX_PARTY_NUMBER;i++){
		bool select=false;
		if(mCharacter==i)	select=true;
		mCDataWindow[i].OnDraw(select);
	}

	if(mWindow.GetInited()){
			int x=mWindow.GetX()-20;//-(mTimer/4)%8-10;
			int y=mWindow.GetY();
			D3DCOLOR color=0x80ff4000;

			DTest.DrawBox(	x	,y+160,
							x+40,y+80,
							x+40,y+240,
							x+40,y+240,
							color,color,color,color);
		int	drawz[MAX_PARTY_NUMBER];
		int	drawcharacter[CHARACTER_NO];
		for(i=0;i<MAX_PARTY_NUMBER;i++){
			drawcharacter[i]=i;
			drawz[i]=Flag.mFormationY[i];
		}

		bool turn=true;
		while(turn){
			turn=false;
			for(int n=1;n<MAX_PARTY_NUMBER;n++){
				if(drawz[n] < drawz[n-1]){
					turn=true;
					int c				=drawcharacter[n];
					drawcharacter[n]	=drawcharacter[n-1];
					drawcharacter[n-1]	=c;
					int z		=drawz[n];
					drawz[n]	=drawz[n-1];
					drawz[n-1]	=z;
				}
			}
		}
		for(i=0;i<MAX_PARTY_NUMBER;i++){
			int &no=drawcharacter[i];
			int &x=Flag.mFormationX[no];
			int &y=Flag.mFormationY[no];
			D3DCOLOR color=0xffffffff;
			if(no==mCharacter){
				if(mSelected){
					color=0x80ffffff;
				}else{
					mWindow.DrawBox(16+x*64+2,16+y*64+2,62,62,COLOR_SELECT);
				}
			}
			mPlayer[no].OnDraw(	x*64+mWindow.GetX(),
								y*64-16+mWindow.GetY(),color);
		}
		if(mSelected){
			mPlayer[mCharacter].OnDraw(	(mX)*64+mWindow.GetX(),
										(mY)*64-32+mWindow.GetY());
			DTest.SetBlend(BLEND_SOLID_ADD);
			mPlayer[mCharacter].OnDraw(	(mX)*64+mWindow.GetX(),
										(mY)*64-32+mWindow.GetY(),0xff000080);
			DTest.SetBlend(BLEND_STANDARD);
		}
	}
}
////////////////////////////////////////////////////////////////////

void CBoxStartMenu::OnDraw()
{
	mList.OnDraw();

	mWindow.OnDraw();
	mWindow.DrawText(0,"�󔠂������܂����B",0,0);

	int y=30;
	if(TreasureData[mBox].mKey>0){
		for(int i=0;i<MAX_KEY_TYPE;i++){
			if(TreasureData[mBox].mKey & (1<<i)){
				mWindow.DrawText(0,"�E",10,y);
				const D3DCOLOR KeyColor[MAX_KEY_TYPE]=
				{
					0xff8000ff,
					0xff0000b0,
					0xff0080ff,
					0xff00b000,
					0xffb0b000,
					0xffff8000,
					0xffff0000,
					0xff404040
				};
				mWindow.DrawText(0,AllItem[ID_KEY+i]->GetName(),20,y,KeyColor[i]);
				mWindow.DrawText(0,"���|�����Ă��܂��B",100,y);
				y+=20;
			}
		}
		y+=10;
	}else{
		mWindow.DrawText(0,"�E",10,y);
		mWindow.DrawText(0,"�J�M�͊|�����Ă��܂���B",20,y);
		y+=30;
	}
	mWindow.DrawText(0,"�E",10,y);
	if(mKnowTrap){
		if(TreasureData[mBox].mTrap>=0){
			mWindow.DrawText(0,TrapData[TreasureData[mBox].mTrap].mName,20,y,0xffff0000);
			mWindow.DrawText(0,"��㩂��d�|�����Ă��܂��B",70,y+20);
			mWindow.DrawText(0,"�E����",30,y+50);
			mWindow.DrawText(0,TrapData[TreasureData[mBox].mTrap].mHelp,50,y+70);
			mWindow.DrawText(0,"�E�����ɕK�v�ȃp�����[�^�[",30,y+110);
			const char *param=NULL;;
			switch(TrapData[TreasureData[mBox].mTrap].mParam){
				case TRAP_LP:	param="�ő�LP";		break;
				case TRAP_ATK:	param="�U����";		break;
				case TRAP_DEF:	param="�h���";		break;
				case TRAP_MATK:	param="���@�U��";	break;
				case TRAP_MDEF:	param="���@�h��";	break;
				case TRAP_SPD:	param="���΂₳";	break;
				case TRAP_TECH:	param="�ő�SP";		break;
				case TRAP_IMP:	param="�����s�\";	break;
			}
			mWindow.DrawText(0,param,50,y+130);
			y+=155;
			mWindow.DrawText(0,"�E������x",30,y);
			if(TrapData[TreasureData[mBox].mTrap].mParam==TRAP_IMP){
				mWindow.DrawText(0,"---",50,y+20);
			}else{
				int star=TreasureData[mBox].GetTrapLv();
				D3DCOLOR color=0xff400000;
				for(int i=0;i<star;i++){
					mWindow.DrawText(0,"��",50+i*15,y+20,color);
					color+=0x00100000;
				}
				char text[32];
				sprintf(text,"%d",star);
				mWindow.DrawText(0,text,55+star*15,y+20,color);
			}
		}else{
			mWindow.DrawText(0,"㩂͎d�|�����Ă��܂���B",20,y);
		}
	}

	for(int i=0;i<MAX_CHARACTER_NUMBER+MAX_PARTY_NUMBER;i++){
		mCDataWindow[i].OnDraw();
	}
}
////////////////////////////////////////////////////////////////////
void CBoxSelectMenu::OnDraw()
{
	for(int i=0;i<MAX_PARTY_NUMBER;i++){
		bool s=false;
		if(i==mCharacter)	s=true;
		mCDataWindow[i].OnDraw(s);

		if(Flag.mParty[i]>=0){
			switch(mOpen[i]){
				case 0:
					mCDataWindow[i].DrawText(0,"�J�M�J����",200,0,0xff0000ff);	break;
				case 1:
					mCDataWindow[i].DrawText(0,"�J�M������",200,0,0xffff8000);	break;
				case 2:
					mCDataWindow[i].DrawText(0,"�J�M�J���s��",200,0,0xffff0000);break;
			}
		}
	}

	mParamWindow.OnDraw();
	int y=0;
	for(int i=0;i<MAX_KEY_TYPE;i++){
		if(TreasureData[mBox].mKey & (1<<i)){
			if(mUseKey[mCharacter][i]){
				if(mKey[i]>0){
					mParamWindow.DrawBox(10,y+16,1000,8,0x4040ff00);
					mParamWindow.DrawText(0,"����",120,y+5,0xffff8000);
				}else{
					mParamWindow.DrawBox(10,y+16,1000,8,0x40ff8000);
					mParamWindow.DrawText(0,"�s���I",120,y+5,0xffff0000);
				}
			}else{
				mParamWindow.DrawBox(10,y+16,1000,8,0x4000b0ff);
				mParamWindow.DrawText(0,"�J�M�J����",120,y+5,0xff0000ff);
			}
			
			AllItem[ITEM_KEY+i]->Draw(&mParamWindow,0,y,0);
			mParamWindow.DrawText(0,"�~",40,y+5,0,DT_RIGHT);
			char text[32];
			sprintf(text,"%d",mKey[i]);
			mParamWindow.DrawText(0,text,10,y+5,0,DT_RIGHT);

			y+=30;
		}
	}
	mParamWindow.DrawBox(10,y+8,1000,2,COLOR_LINE);
	if(mKnowTrap && TreasureData[mBox].mTrap>=0){
		y+=20;
		const char *param=NULL;
		int			num=0;
		switch(TrapData[TreasureData[mBox].mTrap].mParam){
			case TRAP_LP:	param="�ő�LP";		num=Flag.mPlayerData[Flag.mParty[mCharacter]].GetMaxLP();	break;
			case TRAP_ATK:	param="�U����";		num=Flag.mPlayerData[Flag.mParty[mCharacter]].GetAtk();		break;
			case TRAP_DEF:	param="�h���";		num=Flag.mPlayerData[Flag.mParty[mCharacter]].GetDef();		break;
			case TRAP_MATK:	param="���@�U��";	num=Flag.mPlayerData[Flag.mParty[mCharacter]].GetMagAtk();	break;
			case TRAP_MDEF:	param="���@�h��";	num=Flag.mPlayerData[Flag.mParty[mCharacter]].GetMagDef();	break;
			case TRAP_SPD:	param="���΂₳";	num=Flag.mPlayerData[Flag.mParty[mCharacter]].GetSpeed();	break;
			case TRAP_TECH:	param="�ő�SP";		num=Flag.mPlayerData[Flag.mParty[mCharacter]].GetTech();	break;
			case TRAP_IMP:	param="�����s�\";	break;
		}
		mParamWindow.DrawText(0,param,20,y);
		if(TrapData[TreasureData[mBox].mTrap].mParam!=TRAP_IMP){
			D3DCOLOR c=0xff000000;
			if(Flag.mPlayerData[Flag.mParty[mCharacter]].SearchASkill(283)){
				c=0xffff0000;
				num*=2;
			}
			if(Flag.mPlayerData[Flag.mParty[mCharacter]].SearchASkill(360)){
				c=0xff9000ff;
				num=1;
			}
			char text[32];
			sprintf(text,"%d",num);
			mParamWindow.DrawText(0,text,10,y,c,DT_RIGHT);
		}
		y+=30;
	}
}
////////////////////////////////////////////////////////////////////

void COverItemMenu::OnDraw()
{
	mSystem.OnDraw();
	
	if(!mCDataInited){
		for(int i=0;i<MAX_CHARACTER_NUMBER+MAX_PARTY_NUMBER;i++){
			mCDataWindow[i].OnDraw();
		}
	}
}
////////////////////////////////////////////////////////////////////
void CItemHelp::OnDraw()
{
	mWindow.OnDraw();
	AllItem[mItemNo]->Draw(&mWindow,0,0,0);
	if(mItemNo!=ID_SD){
		char text[256];
		sprintf(text,"���������F %d �R",mAllNo);
		mWindow.DrawText(0,text,10,40,0,DT_RIGHT);
	}
	mWindow.DrawText(0,AllItem[mItemNo]->GetTypeName(),10,20,0,DT_RIGHT);

	mWindow.DrawMessage(0,mText,0,60);
}
////////////////////////////////////////////////////////////////////
void CSelectHelp::OnDraw()
{
	mWindow.OnDraw();

	for(int i=0;i<mListSize;i++){
		if(i==mCursor){
			mWindow.DrawBox(0,(i+1)*mEleHeight-22+mHeight,mWidth-32,8,COLOR_SELECT);
		}
		mList[i]->Draw(&mWindow,20,i*mEleHeight-8+mHeight,0);
	}
}
////////////////////////////////////////////////////////////////////
void CEquipmentHelp::OnDraw()
{
	CSelectHelp::OnDraw();
	AllItem[mItemNo]->Draw(&mWindow,0,0,0);
	char text[256];
	sprintf(text,"���������F %d �R",mAllNo);
	mWindow.DrawText(0,text,mWindow.GetWidth()-mWidth-16,40,0,DT_RIGHT);
	mWindow.DrawText(0,AllItem[mItemNo]->GetTypeName(),mWindow.GetWidth()-mWidth-16,20,0,DT_RIGHT);
	mWindow.DrawMessage(0,mText,0,60);

	mWindow.DrawText(0,"-- ������� --",0,mHeight-30);
	if(mListSize<=0){
		mWindow.DrawText(0,"���ɂȂ�",16,mHeight-8,COLOR_GRAY);
	}
	mWindow.DrawBox(mWidth,0,2,1000,COLOR_LINE);

	mWindow.DrawText(0,"-- �\�͒l --",mWidth+10,0);

	const CEquipmentData & edata=EquipmentData[mItemNo];
	DrawParam("LP",			edata.mLP,30);
	DrawParam("�U����",		edata.mAtk,60);
	DrawParam("�h���",		edata.mDef,80);
	DrawParam("���@�U��",	edata.mMagAtk,110);
	DrawParam("���@�h��",	edata.mMagDef,130);
	DrawParam("���΂₳",	edata.mSpeed,160);
}
void	CEquipmentHelp::DrawParam(const char* name,int param,int y)
{
	mWindow.DrawText(0,name,mWidth+24,y);
	char text[64];
	sprintf(text,"%+d",param);
	D3DCOLOR color=0xff80d080;
	if(param>0)			color=0xffff0000;
	else if(param<0)	color=0xff0000ff;
	mWindow.DrawText(0,text,8,y,color,DT_RIGHT);
}

////////////////////////////////////////////////////////////////////

void CEventSkipMenu::OnDraw()
{
	mTitle.OnDraw();
	mTitle.DrawText(0,"�C�x���g�X�L�b�v���܂��B\n��낵���ł����H",-8,0,0,DT_CENTER);

	mList.OnDraw();
}
////////////////////////////////////////////////////////////////////
void CSkillHelp::OnDraw()
{
	mWindow.OnDraw();
	SkillData[mSkillNo].Draw(&mWindow,0,0,0);
	mWindow.DrawMessage(0,mText,0,40);
}
////////////////////////////////////////////////////////////////////
void CComSkillHelp::OnDraw()
{
	mWindow.OnDraw();
	const CComSkillData &data=ComSkillData[mSkillNo];
	data.Draw(&mWindow,0,0,0);
	int type=7;
	for(int i=0;i<7;i++){
		if(data.mType & (1<<i)){
			type=i;
		}
	}

	const char *stname[8] = {
		"�Z",
		"���Z",
		"���Z",
		"���Z",
		"�|�Z",
		"���@",
		"����",
		"�A�C�e���g�p"
	};
	static char text[128];
	sprintf(text,"�n�� �F %s",stname[type]);
	mWindow.DrawText(0,text,20,25,0,DT_RIGHT);

	int area=0;
	for(int i=0;i<8;i++){
		if(data.mRange & (1<<i)){
			area=i+1;
		}
	}
	const char *areaname[10] = {
		"---",
		"�������g",
		"�P��",
		"�P��",
		"�c����",
		"������",
		"�͈�(��)",
		"�͈�(��)",
		"�S��",
		"�G�����S��",
	};

	if(data.mRange==CR_S_ALL){
		sprintf(text,"�Ώ� �F �G�����S��");
	}else if(data.mRange==CR_NONE){
		sprintf(text,"�Ώ� �F ---");
	}else{
		sprintf(text,"�Ώ� �F %s�E%s",(data.mTarget?"�G":"����"),areaname[area]);
	}
	mWindow.DrawText(0,text,20,45,0,DT_RIGHT);

	mWindow.DrawMessage(0,mText,0,50);
}
////////////////////////////////////////////////////////////////////
void CShopMenu::OnDraw()
{
	mList.OnDraw();

	static char text[128];
	mTalk.OnDraw();
	mTalk.DrawText(0,mHelp,0,0);

	mSWindow.OnDraw();

	int item=mList.GetCursorItem()->mID;
	mInfo.OnDraw();

	mInfo.DrawText(0,AllItem[item]->GetTypeName(),0,-4);

	if(AllItem[item]->mType & TYPE_EQUIPMENT){
		CItemHelpWindow::DrawParam(&mInfo,20,16,item,4);

		if(EquipmentData[item].mSkillNo){
			sprintf(text,"������ʁF %d ��",EquipmentData[item].mSkillNo);
			for(int i=0;i<EquipmentData[item].mSkillNo;i++){
				mInfo.DrawTexture(&SystemTex,0,16,48,16,16,i*10+30,90,16,16,0xffffffff);
			}
			mInfo.DrawText(2,text,10,80,0xff000000);
		}
	}

	if(item<NO_ITEM){
		mInfo.DrawText(0,"��������:",160,0);
		sprintf(text,"%d �R",Flag.GetAllItem(item,false,2));
		mInfo.DrawText(0,text,10,10,0,DT_RIGHT);

/*		mInfo.DrawText(0,"�q��:",160,30);
		sprintf(text,"%d �R",Flag.mCloset[item]);
		mInfo.DrawText(0,text,10,40,0,DT_RIGHT);
*/	}

	mInfo.DrawText(0,"������:",160,70);
	sprintf(text,"%d M",Flag.mMoney);
	mInfo.DrawText(0,text,10,90,0,DT_RIGHT);

	mPlayerItem.OnDraw();
	mPlayerItem.DrawText(2,"���݂̑���",0,-8,0xff800000);

	mCompare.OnDraw();
}
////////////////////////////////////////////////////////////////////
void CFarmMenu::OnDraw()
{
	mList.OnDraw();
	mHelp.OnDraw();
	mHelp.DrawMessage(0,mText,0,-20);

	mSWindow.OnDraw();
	mPlayerItem.OnDraw();
	
}
////////////////////////////////////////////////////////////////////
void CSeedMenu::OnDraw()
{
	mSeedWindow.OnDraw();
	AllItem[ID_SD]->Draw(&mSeedWindow,0,-8,0);
	char text[64];
	sprintf(text,"�~%d",mSeedNo);
	mSeedWindow.DrawText(0,text,10,-4,0,DT_RIGHT);

	mWindow.OnDraw();
	int x=200;
	if(THROW_END_TIME<mTimer && mTimer<NEW_ITEM_TIME){
		if(mTimer%SHAKE_TIME==0)	x+=4;
	}
	mWindow.DrawTexture(&SystemTex,0,128,112,32,32,x,140,64,64);

	if(mTimer<START_TIME){
	}else if(mTimer<THROW_END_TIME){
		int x=215+(THROW_END_TIME-mTimer)*8;
		int y=(mTimer-THROW_TIME/2-START_TIME);
		y=60+y*y/3;
		mWindow.DrawTexture(&ItemTex[4],0,224,96,32,32,x,y,32,32);
	}else if(mTimer<NEW_ITEM_TIME){
	}else if(mNewItem>=0){
		int t=mTimer;
		if(mTimer>END_TIME)	t=END_TIME+1;
		int x=215-(t-NEW_ITEM_TIME)*4;
		int y=(t-JUMP_TIME/2-NEW_ITEM_TIME);
		y=60+y*y/3;
		
		int icon=AllItem[mNewItem+ID_UI]->mIcon;
		int tx=icon%8*32;
		int ty=(icon/8)%8*32;
		mWindow.DrawTexture(&ItemTex[icon/64],0,tx,ty,32,32,x-1,y+2,0x80000000);
		mWindow.DrawTexture(&ItemTex[icon/64],0,tx,ty,32,32,x,y);
	}

	if(mTimer>END_TIME){
		if(mNewItem>=0){
			mWindow.DrawText(0,"���Y�ʂ��オ��܂����I",10,10,0xffff0000);
			AllItem[mNewItem+ID_UI]->Draw(&mWindow,30,40,0);
			char text[64];
			sprintf(text,"�~%d",mNewItemNo);
			mWindow.DrawText(0,text,240,44);
			mWindow.DrawMessage(0,mHelp,50,60);
		}else{
			mWindow.DrawText(0,"�^�l������܂���I",10,10,0xffff0000);
		}
	}
}
////////////////////////////////////////////////////////////////////
void CSellYesNoMenu::OnDraw()
{
	mTitle.OnDraw();
	mTitle.DrawText(0,"���̃A�C�e���𔄂�܂����H",0,0);
	AllItem[mItemID]->Draw(&mTitle,30,35,64);

	if(mSellNo>0){
		char text[64];
		if(mCharacter>=MAX_CHARACTER_NUMBER){
			mTitle.DrawText(0,"�q��:",130,80);
		}else{
			mTitle.DrawText(0,"����:",130,80);
		}
		sprintf(text,"%d �R",mSellMax);
		mTitle.DrawText(0,text,45,80,0,DT_RIGHT);

		mTitle.DrawText(0,"���p:",130,110);
		sprintf(text,"%d �R",mSellNo);
		mTitle.DrawText(0,text,45,110,0,DT_RIGHT);

		int texturex;
		switch((mTimer/CURSOR_ANI_TIME)%4){
			case 1:		texturex=1;	break;
			case 3:		texturex=2;	break;
			default:	texturex=0;
		}

		if((mMoveTimer&0xff) && (mMoveTimer&0x100)){
			mTitle.DrawTexture(&SystemTex,TURN_HORIZONTAL,160,32,32,16,180,110);
		}else{
			mTitle.DrawTexture(&SystemTex,TURN_HORIZONTAL,64+texturex*32,32,32,16,180,110);
		}

		if((mMoveTimer&0xff) && (mMoveTimer&0x200)){
			mTitle.DrawTexture(&SystemTex,0,160,32,32,16,300,110,32,16);
		}else{
			mTitle.DrawTexture(&SystemTex,0,64+texturex*32,32,32,16,300,110,32,16);
		}

	}

	mList.OnDraw();
}
////////////////////////////////////////////////////////////////////
void CReturnVillage::OnDraw()
{
	mTitle.OnDraw();
	mTitle.DrawText(0,"���ɋA��܂����H",0,10);

	mList.OnDraw();
}
////////////////////////////////////////////////////////////////////
void CDiaryMenu::OnDraw()
{
	mTitle.OnDraw();
	if(mLoad){
		mTitle.DrawText(0,"���L��ǂށ@�i���[�h�j",0,-4);
	}else{
		mTitle.DrawText(0,"���L�������@�i�Z�[�u�j",0,-4);
	}

	mList.OnDraw();
	mYesNo.OnDraw();
}
////////////////////////////////////////////////////////////////////

void CClosetMenu::OnDraw()
{
	mSWindow.OnDraw();
	mItemList.OnDraw();
	mCategoryWindow.OnDraw();
	mClosetList.OnDraw();

	mEquipWindow.OnDraw();
	mHelpWindow.OnDraw();
}
////////////////////////////////////////////////////////////////////

void CMonthStartMenu::OnDraw()
{
	mList.OnDraw();
	for(int i=0;i<MAX_CHARACTER_NUMBER+MAX_PARTY_NUMBER;i++){
		bool select=false;
		mCDataWindow[i].OnDraw(select);
	}

/*
	static char text[256];
	mInfo.DrawText(0,Flag.mMapName,10,-5,0,DT_RIGHT);
	mInfo.DrawText(0,"�������F",160,15,0,DT_RIGHT);
	sprintf(text,"%d M",Flag.mMoney);
	mInfo.DrawText(0,text,10,15,0,DT_RIGHT);
	mInfo.DrawText(0,"�v���C���ԁF",160,35,0,DT_RIGHT);
	int sec=(Flag.mPlayTime/(60))%60;
	int min=(Flag.mPlayTime/(60*60))%60;
	int hour=(Flag.mPlayTime/(60*60*60));
	sprintf(text,"%d : %02d : %02d",hour,min,sec);
	mInfo.DrawText(0,text,10,35,0,DT_RIGHT);
*/}
////////////////////////////////////////////////////////////////////

void CChangeMemberMenu::OnDraw()
{
	for(int i=0;i<MAX_PARTY_NUMBER;i++){
	//	if(MonthData[Flag.mCurrentMonth].mParty[i]==-1 || mTop){
		bool select=false;
		if(mChange<0)	select=(mCursor==i);
		else			select=(mChange==i);
		CMenuData::mCDataWindow[i].OnDraw(select);
	//	}
		if(Flag.mCurrentMonth<0 || MonthData[Flag.mCurrentMonth].mParty[i]!=-1){
			CMenuData::mCDataWindow[i].DrawBox(-20,15,320,40,0x80ff8080);
			CMenuData::mCDataWindow[i].DrawText(0,"�ύX�s�I",0,0,0xffff0000,DT_RIGHT);
		}
	}

	for(int i=0;i<mReserveNo;i++){
		bool select=(mChange>=0 && i==mCursor);
		mCDataWindow[i].OnDraw(select);
		if(!mChangeOk[i]){
			mCDataWindow[i].DrawBox(-20,15,320,40,0x80ff8080);
			mCDataWindow[i].DrawText(0,"�Q���s�I",0,0,0xffff0000,DT_RIGHT);
		}
	}
}
////////////////////////////////////////////////////////////////////
void	CBattleStatusMenu::DrawStatus()
{
	if(mWindow.GetInited()){
		int size=mCharacter->GetSize();
		mCharacter->OnDraw(mWindow.GetX()-64*size+32,mWindow.GetY()-64*size+32);
	}

	char text[256];
	CCharacterData* pdata=mCharacter->GetCData();
	sprintf(text,"%s",mCharacter->GetName());
	mWindow.DrawBox(120,0,1000,16,COLOR_WIN&0xb0ffffff);
	mWindow.DrawText(0,text,120,0);	

	sprintf(text,"Lv.%d",mCharacter->GetLv());
	mWindow.DrawText(0,text,20,20,0,DT_RIGHT);	

	int kill=mCharacter->GetKillNo();
	if(kill>=0){
		sprintf(text,"���j�� - %d",kill);
		mWindow.DrawText(0,text,20,40,0,DT_RIGHT);	
	}
	if(mCharacter->CheckOpenData(ODATA_LP)){
		mWindow.DrawBox(0,70,1000,16,COLOR_WIN&0xb0ffffff);
		mWindow.DrawBox(40,90,1000,16,COLOR_WIN&0xb0ffffff);
		mWindow.DrawText(0,"HP / LP / MLP",0,70);
		int hp=mCharacter->GetHP();
		int mhp=mCharacter->GetMaxHP();
		int mlp=mCharacter->GetMaxLP();
		if(mCharacter->SearchASkill(403)){
			hp-=mlp/4;
			mhp-=mlp/4;
			mlp-=mlp/4;
			if(hp<0)	hp=0;
		}

		sprintf(text,"%d",hp);
		mWindow.DrawText(0,text,TOTAL_X+30,90,0,DT_RIGHT);	
		mWindow.DrawText(0,"/",TOTAL_X+20,90,0,DT_RIGHT);	

		sprintf(text,"%d",mhp);
		mWindow.DrawText(0,text,TOTAL_X-40,90,0,DT_RIGHT);	
		mWindow.DrawText(0,"/",TOTAL_X-50,90,0,DT_RIGHT);

		sprintf(text,"%d",mlp);
		mWindow.DrawText(0,text,TOTAL_X-110,90,0,DT_RIGHT);	
	}else if(!mCharacter->SearchASkill(305)){
		mWindow.DrawBox(0,90,1000,16,COLOR_WIN&0xb0ffffff);
		mWindow.DrawText(0,"�^�����_���[�W",0,90);
		sprintf(text,"%d",mCharacter->GetMaxLP()-mCharacter->GetHP());
		mWindow.DrawText(0,text,TOTAL_X-80,90,0,DT_RIGHT);	

	}else{
		mWindow.DrawText(0,"�s��",0,90);
	}

	static const int SP_X=TOTAL_X-80;
	int j=0;
	if(mCharacter->CheckOpenData(ODATA_SP)){
		mWindow.DrawBox(0,120,1000,16,COLOR_WIN&0xb0ffffff);
		mWindow.DrawText(0,"SP",0,120);
		while(1){
			sprintf(text,"%d",pdata->GetSP(j));
			mWindow.DrawText(0,text,SP_X+j*30,120,0,DT_RIGHT);
			j++; 
			if(j>=MAX_CSKILL_LEVEL)	break;
			mWindow.DrawText(0,"/",SP_X+j*30-8,120,0,DT_RIGHT);
		}
		mWindow.DrawText(0,"�ő�SP",0,145);
		j=0;
		while(1){
			sprintf(text,"%d",pdata->GetMaxSP(j));
			mWindow.DrawText(0,text,SP_X+j*30,145,0,DT_RIGHT);
			j++;
			if(j>=MAX_CSKILL_LEVEL)	break;
			mWindow.DrawText(0,"/",SP_X+j*30-8,145,0,DT_RIGHT);
		}
	}else if(!mCharacter->SearchASkill(305)){
		mWindow.DrawBox(0,130,1000,16,COLOR_WIN&0xb0ffffff);
		mWindow.DrawText(0,"����SP",0,130);
		while(1){
			sprintf(text,"%d",pdata->GetMaxSP(j)-pdata->GetSP(j));
			mWindow.DrawText(0,text,SP_X+j*30,130,0,DT_RIGHT);
			j++; 
			if(j>=MAX_CSKILL_LEVEL)	break;
			mWindow.DrawText(0,"/",SP_X+j*30-8,130,0,DT_RIGHT);
		}
	}else{
		mWindow.DrawText(0,"�s��",0,130);
	}

	DrawParam("�U����"	,1,mCharacter->GetAtk()		,mCharacter->GetBaseAtk()	,mCharacter->CheckOpenData(ODATA_ATK));
	DrawParam("�h���"	,2,mCharacter->GetDef()		,mCharacter->GetBaseDef()	,mCharacter->CheckOpenData(ODATA_DEF));
	DrawParam("���@�U��",3,mCharacter->GetMagAtk()	,mCharacter->GetBaseMagAtk(),mCharacter->CheckOpenData(ODATA_MATK));
	DrawParam("���@�h��",4,mCharacter->GetMagDef()	,mCharacter->GetBaseMagDef(),mCharacter->CheckOpenData(ODATA_MDEF));
	DrawParam("���΂₳",6,mCharacter->GetSpeed()	,mCharacter->GetBaseSpeed()	,mCharacter->CheckOpenData(ODATA_SPD));

}
////////////////////////////////////////////////////////////////////////
void CConfigMenu::OnDraw()
{
	mAll.OnDraw();
	for(int i=0;i<MENU_NO;i++){
		mSelectWindow[i].OnDraw();
	}
	mHelp.OnDraw();
	const char* config_help[MENU_NO+1]={
		"�a�f�l���Đ����邩�ǂ��������߂܂��B",

		"���ʉ����Đ����邩�ǂ��������߂܂��B",

		"�퓬���ɁA�ȑO�ɑI���������ڂ�\n"
			"�����ŃJ�[�\�������킹�邩�ǂ��������߂܂��B",

		"�퓬�X�s�[�h��ݒ肵�܂��B\n\n"
			"�@�i���j�ݒ�ɂ��A�퓬���ʂɑ����e��������܂��B",

		"�}�b�v��ʂŖ��L�[���������Ƃ��ɁA�ǂ̕����ɕ����������߂܂��B\n"
			"�@�E�Ɂ`�@�̏ꍇ�A��L�[�������ΉE��ɁA\n"
			"�@���Ɂ`�@�̏ꍇ�A��L�[�������΍���Ɍ������ĕ����܂��B",

		"�}�b�v��ʂŃL�����Z���{�^�����������Ɉړ������Ƃ���\n�ړ����@�����߂܂��B\n"
			"�L�����Z���{�^���������ƁA�����Е��̈ړ����@�ňړ��ł��܂��B",

		"�t���X�N���[���ŗV�Ԃ��A�E�C���h�E���[�h�ŗV�Ԃ������߂܂��B\n"
			"�S�Z�[�u�f�[�^�ŋ��p�ł��B",

		"�Q�[���p�b�h�̐ݒ���s���܂��B\n"
			"�S�Z�[�u�f�[�^�ŋ��p�ł��B"
	};
	mHelp.DrawMessage(0,config_help[mAll.GetCursor()],0,0);
}
////////////////////////////////////////////////////////////////////
void CFreeSpaceMenu::OnDraw()
{
	mLearnSkill.OnDraw();

	mLearnSkill.DrawText(0,"�K�����Ă���l",0,90);
	for(int i=0;i<MAX_PARTY_NUMBER;i++){
		if(mLearnCharacter[i]){
			mLearnSkill.DrawTexture(&mTex[i],0,0,0,64,64,i*55+20,100);
		}
	}
	mLearnSkill.DrawMessage(0,mHelp,0,150);
}
////////////////////////////////////////////////////////////////////
void CFSSelectSkillMenu::OnDraw()
{
	CLearnSkillMenu::OnDraw();
	mOldSkill.OnDraw();
	if(mMessageTimer>0){
		mOldSkill.DrawText(0,mMessage,0,0,0xffff0000);
	}else{
		int skill=	Flag.mPlayerData[Flag.mParty[mFreeSpaceCharacter]].GetFSpace(mFreeSpace);
		mOldSkill.DrawText(0,"�ύX�O:",0,0);
		const CSkillData* sdata;
		if(skill<0){
			sdata=&NoneSkillData;
		}else{
			sdata=&SkillData[skill];
		}
		sdata->Draw(&mOldSkill,60,-6,0);
	}
}
////////////////////////////////////////////////////////////////////
void CGlobalConfigMenu::OnDraw()
{
	mAll.OnDraw();
	mSelectWindow.OnDraw();
}
////////////////////////////////////////////////////////////////////
void CKeyConfigMenu::OnDraw()
{
	mList.OnDraw();
}
////////////////////////////////////////////////////////////////////
void CDInfoMenu::OnDraw()
{
	if(mMonth<0)	return;
	CMonthData& mdata=MonthData[mMonth];

	mWindow.OnDraw();

	char text[64];
	sprintf(text,"%d��",mdata.mMonth);
	mWindow.DrawText(0,text,0,0,0);
	mWindow.DrawText(5,mdata.GetName(),30,20,0);

	mWindow.DrawText(0,"��������",15,70,0);
	mWindow.DrawTexture(&SystemTex,0,96,112,32,32,30,92);
	mWindow.DrawText(0,"/",120,100);
	sprintf(text,"%d",mOpenTresure);
	D3DCOLOR color=0xff000000;
	if(mOpenTresure>=mdata.mTresureNo){
		color=0xffff0000;
	}
	mWindow.DrawText(0,text,100,100,color);
	sprintf(text,"%d",mdata.mTresureNo);
	mWindow.DrawText(0,text,150,100);
	
	mWindow.DrawText(0,"�J�M",15,130,0);
	mWindow.DrawText(1,"����",395,135,0,DT_RIGHT);
	mWindow.DrawText(1,"�c��",365,135,0,DT_RIGHT);
	int y=150;
	for(int i=0;i<MAX_KEY_TYPE;i++){
		if(mKey[i]<=0)	continue;
		AllItem[ITEM_KEY+i]->Draw(&mWindow,30,y,0);
		char text[32];
		sprintf(text,"%d",mKey[i]);
		mWindow.DrawText(0,text,370,y+3,0,DT_RIGHT);
		mWindow.DrawText(0,"/",390,y+3,0,DT_RIGHT);
		sprintf(text,"%d",Flag.mCloset[ITEM_KEY+i]);
		mWindow.DrawText(0,text,400,y+3,0,DT_RIGHT);

		y+=30;
	}
	mWindow.DrawText(0,"�o������G",250,10);
	mList.OnDraw();
}
////////////////////////////////////////////////////////////////////
void CBypathMenu::OnDraw()
{
	mTitle.OnDraw();
	mTitle.DrawText(0,"�Z�[�u�|�C���g",0,-4);

	mList.OnDraw();
}
////////////////////////////////////////////////////////////////////
void CStatusHelp::OnDraw()
{
	mWindow.OnDraw();
	StatusData[mStatusNo].Draw(&mWindow,0,0,0);
	mWindow.DrawMessage(0,mText,0,40);
}
////////////////////////////////////////////////////////////////////
void CRemoveBirthStoneMenu::OnDraw()
{
	mSWindow.OnDraw();
	mWindow.OnDraw();
	if(mErrorTimer<=0){
		mWindow.DrawText(0,"���S�߂�",0,-4);
	}else{
		mWindow.DrawText(0,"���̃L�����N�^�[�͎��O���܂���I",0,-4,0xffff0000);
	}
}
////////////////////////////////////////////////////////////////////

void CRemoveBSPersonal::OnDraw()
{
	mList.OnDraw();
	mMonthList.OnDraw();
	mSkillList.OnDraw();
	mCDataWin.OnDraw();
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
////////////////////////////////////////////////////////////////////

void CSelectDifficult::OnDraw()
{
	mTitle.OnDraw();
	mTitle.DrawText(0,"��Փx�I��",0,0);
	
	mHelp.OnDraw();
	int cursor=mList.GetCursor();
	switch(cursor){
		case 0:
			mHelp.DrawText(6,"Normal",0,0,0xff008000);
			mHelp.DrawText(0,"�W���I�ȓ�Փx�ł��B���߂Ă̕��ɃI�X�X���ł��B",20,30);
			mHelp.DrawText(0,"�E�G�̃��x�����T���߂ł��B",30,65);
			mHelp.DrawText(0,"�EHard�Ƃ͈ꕔ�̃V�i���I���Ⴂ�܂��B",30,90);
			break;
		case 1:
			mHelp.DrawText(6,"Hard",0,0,0xffb00000);
			mHelp.DrawText(0,"���ɍ�����Փx�ł��B���v���C�ł̑I���͐������܂���B",20,30);
#ifdef _HARD_LOCK_
			mHelp.DrawText(0,"���̃o�[�W�����ł̓v���C���邱�Ƃ��o���܂���B",20,80,0xffff0000);
#else
			mHelp.DrawText(0,"�E�G�̃��x���������Ȃ�܂��B",30,65);
			mHelp.DrawText(0,"�ENormal�Ƃ͈ꕔ�̃V�i���I���Ⴂ�܂��B",30,90);
#endif
			break;

	}

	mList.OnDraw();
}
////////////////////////////////////////////////////////////////////

void CExitYesNo::OnDraw()
{
	mTitle.OnDraw();
	mTitle.DrawText(0,"�I�����ă^�C�g���ɖ߂�܂��B\n��낵���ł����H",0,10,0,DT_CENTER);

	mList.OnDraw();
}
////////////////////////////////////////////////////////////////////

void CSuccessSaveDataMenu::OnDraw()
{
	mHelp.OnDraw();
	mHelp.DrawText(0,"���Z�[�u�f�[�^�̌p��",0,0);
	mHelp.DrawText(0,"�I�������Z�[�u�f�[�^�́A���̃f�[�^�������p���Ńv���C�ł��܂��B",15,20);
	mHelp.DrawText(0,"�E�N���A������Փx",20,50);
	mHelp.DrawText(0,"�E�v���C����",20,65);
	mHelp.DrawText(0,"�E�G���",20,80);

	CDiaryMenu::OnDraw();
}
////////////////////////////////////////////////////////////////////


void CCallPetHelp::OnDraw()
{
	CSelectHelp::OnDraw();
	ComSkillData[mSkillNo].Draw(&mWindow,0,0,8);
	static char text[32];
	sprintf(text,"SLv: %d",ComSkillData[mSkillNo].mSP);
	mWindow.DrawText(0,text,mWidth-60,0);

	mWindow.DrawMessage(0,mText,0,25);

	mWindow.DrawText(0,"-- �����X�L�� --",0,mHeight-30);
	if(mListSize<=0){
		mWindow.DrawText(0,"�Ȃ�",16,mHeight-8,COLOR_GRAY);
	}
	mWindow.DrawBox(mWidth,0,2,1000,COLOR_LINE);

	mWindow.DrawText(0,"-- �\�͔{�� --",mWidth+10,0);


	const CPetData & pdata=PetData[mPetNo];
	DrawParam("LP",			pdata.mLP,30);
	DrawParam("SP",			pdata.mTech,50);
	DrawParam("�U����",		pdata.mAtk,80);
	DrawParam("�h���",		pdata.mDef,100);
	DrawParam("���@�U��",	pdata.mMagAtk,130);
	DrawParam("���@�h��",	pdata.mMagDef,150);
	DrawParam("���΂₳",	pdata.mSpeed,190);
}
void	CCallPetHelp::DrawParam(const char* name,int param,int y)
{
	char text[64];
	sprintf(text,"%d ��",param);
	
	int left=mWindow.GetWidth()-32;
	mWindow.DrawBox(left-param/2,y+10,param/2,6,0x80ff8080);
	mWindow.DrawText(0,name,mWidth+24,y);
	mWindow.DrawText(0,text,8,y,0,DT_RIGHT);
}



////////////////////////////////////////////////////////////////////
void CTutorialMenu::OnDraw()
{
	mWindow.OnDraw();

	char str[128];
	mWindow.DrawBox(-8,16,320,8,COLOR_SELECT);
	mWindow.DrawText(6,mTitle,0,0);
	sprintf(str,"%d.%s",mPage+1,mSubTitle);
	mWindow.DrawText(10,str,10,35,0xffb00000);
	mWindow.DrawBox(10,55,320,2,COLOR_DARK_WINDOW);
	mWindow.DrawMessage(0,mText,20,65);

	mWindow.DrawBox(350-2,10-2,204,244,0xffb08020);
	mWindow.DrawTexture(&mTex,0,0,0,200,240,350,10);

	sprintf(str,"Page %d / %d",mPage+1,mMaxPage);
	mWindow.DrawText(0,str,330,270,0,DT_CENTER);
}
////////////////////////////////////////////////////////////////////
void CSoundTestMenu::OnDraw()
{
	mCurrent.OnDraw();
	mCurrent.DrawText(2,"�Đ����̋ȁF",0,3);
	BGMData[Flag.mBGMNo].Draw(&mCurrent,80,0,0);
	mBGMWindow.OnDraw();
}
////////////////////////////////////////////////////////////////////

void CPreciousItemMenu::OnDraw()
{
	mCategoryWindow.OnDraw();
	mClosetList.OnDraw();
	mHelpWindow.OnDraw();
}
////////////////////////////////////////////////////////////////////