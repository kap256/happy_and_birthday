#include "menudata.h"
#include "changemode.h"
#include "mapmode.h"

CStartMenu::~CStartMenu()
{
}
void	CStartMenu::Destroy(void)
{
	mWindow.Destroy();
	mInfo.Destroy();
	if(mCDataMove){
		for(int i=0;i<MAX_CHARACTER_NUMBER+MAX_PARTY_NUMBER;i++){
			mCDataWindow[i].Destroy();
		}
	}
	CMenuData::Destroy();
}

////////////////////////////////////////////////////////////////////
CItemMenu::~CItemMenu()
{
}
void	CItemMenu::Destroy(void)
{
	mItemList.Destroy();
	mSWindow.Destroy();
	mHelpWindow.Destroy();

	mCategoryWindow.Destroy();
	mClosetList.Destroy();
	mCompare.Destroy();
	CMenuData::Destroy();
}
////////////////////////////////////////////////////////////////////
CItemSubMenu::~CItemSubMenu()
{
}
void	CItemSubMenu::Destroy(void)
{
	mList.Destroy();
	mTitle.QuickDestroy();
	mCompare.Destroy();
	CMenuData::Destroy();
}
////////////////////////////////////////////////////////////////////
CExchangeItem::~CExchangeItem()
{
}
void	CExchangeItem::Destroy(void)
{
	CItemMenu::Destroy();
	mExchangeInfo.Destroy();
	mCompare.Destroy();
}
////////////////////////////////////////////////////////////////////
CDumpItem::~CDumpItem()
{
}
void	CDumpItem::Destroy(void)
{
	mList.Destroy();
	mTitle.Destroy();
	CMenuData::Destroy();
}
////////////////////////////////////////////////////////////////////
CUIMenuSelectCharacter::~CUIMenuSelectCharacter()
{
}
void	CUIMenuSelectCharacter::Destroy(void)
{
	mTitle.Destroy();
	for(int i=0;i<MAX_CHARACTER_NUMBER+MAX_PARTY_NUMBER;i++){
		mCDataWindow[i].Home();
	}
	CMenuData::Destroy();
}
////////////////////////////////////////////////////////////////////
CBirthStoneMenu::~CBirthStoneMenu()
{
}
void	CBirthStoneMenu::Destroy(void)
{
	mBirthStoneSystem.Destroy();
	CMenuData::Destroy();
}
////////////////////////////////////////////////////////////////////
CLearnSkillMenu::~CLearnSkillMenu()
{
	delete[] mHelp;
	mHelp=NULL;
}
void	CLearnSkillMenu::Destroy(void)
{
	mLearnSkill.Destroy();
	mCompare.Destroy();
	mMessageWindow[0].Destroy();
	mMessageWindow[1].Destroy();
	mHelpWindow.Destroy();
	mBirthStoneWindow.Destroy();
	CMenuData::Destroy();
}
////////////////////////////////////////////////////////////////////
CLearnSkillYesNo::~CLearnSkillYesNo()
{
}
void	CLearnSkillYesNo::Destroy(void)
{
	mList.Destroy();
	mTitle.Destroy();
	CMenuData::Destroy();
}
////////////////////////////////////////////////////////////////////
CStatusMenu::~CStatusMenu()
{
	if(mDelete){
		delete mCharacter;
		mCharacter=NULL;
	}
}
void	CStatusMenu::Destroy(void)
{
	mWindow.Destroy();
	mList.Destroy();
	mListTitle.Destroy();
	mListHelp.Destroy();
	CMenuData::Destroy();
}
////////////////////////////////////////////////////////////////////
CFormationMenu::~CFormationMenu()
{
}
void	CFormationMenu::Destroy(void)
{
	mWindow.Destroy();
	for(int i=0;i<MAX_CHARACTER_NUMBER+MAX_PARTY_NUMBER;i++){
		mCDataWindow[i].Home();
	}
	CMenuData::Destroy();
}
////////////////////////////////////////////////////////////////////
CBoxStartMenu::~CBoxStartMenu()
{
}
void	CBoxStartMenu::Destroy(void)
{
	for(int i=0;i<MAX_CHARACTER_NUMBER+MAX_PARTY_NUMBER;i++){
		mCDataWindow[i].Destroy();
	}
	mList.Destroy();
	mWindow.Destroy();
	CMenuData::Destroy();
}
////////////////////////////////////////////////////////////////////
CBoxSelectMenu::~CBoxSelectMenu()
{
}
void	CBoxSelectMenu::Destroy(void)
{
	for(int i=0;i<MAX_CHARACTER_NUMBER+MAX_PARTY_NUMBER;i++){
		mCDataWindow[i].Home();
	}
	mParamWindow.Destroy();
	CMenuData::Destroy();
}
////////////////////////////////////////////////////////////////////
CBoxOpenMenu::~CBoxOpenMenu()
{
}
void CBoxOpenMenu::Destroy()
{
	mWindow.Destroy();
	CMenuData::Destroy();
}
/////////////////////////////////////////////////////////////////////////////////
COverItemMenu::~COverItemMenu()
{
}
void	COverItemMenu::Destroy(void)
{
	mSystem.Destroy();
	
	if(!mCDataInited){
		for(int i=0;i<MAX_CHARACTER_NUMBER+MAX_PARTY_NUMBER;i++){
			mCDataWindow[i].Destroy();
		}
	}
	CMenuData::Destroy();
}
////////////////////////////////////////////////////////////////////
CItemHelp::~CItemHelp()
{
	delete[] mText;
	mText=NULL;
}
void	CItemHelp::Destroy(void)
{
	mWindow.Destroy();
	CMenuData::Destroy();
}
////////////////////////////////////////////////////////////////////
CSelectHelp::~CSelectHelp()
{
}
void	CSelectHelp::Destroy(void)
{
	mWindow.Destroy();
	CMenuData::Destroy();
}
////////////////////////////////////////////////////////////////////
CEquipmentHelp::~CEquipmentHelp()
{
}
void	CEquipmentHelp::Destroy(void)
{
	CSelectHelp::Destroy();
}
////////////////////////////////////////////////////////////////////
CEventSkipMenu::~CEventSkipMenu()
{
}
void	CEventSkipMenu::Destroy(void)
{
	mList.Destroy();
	mTitle.Destroy();
	CMenuData::Destroy();
}
////////////////////////////////////////////////////////////////////
CSkillHelp::~CSkillHelp()
{
	delete[] mText;
	mText=NULL;
}
void	CSkillHelp::Destroy(void)
{
	mWindow.Destroy();
	CMenuData::Destroy();
}
////////////////////////////////////////////////////////////////////
CShopMenu::~CShopMenu()
{
	delete[] mHelp;
	mHelp=NULL;
}
void	CShopMenu::Destroy(void)
{
	mList.Destroy();
	mTalk.Destroy();
	mSWindow.Destroy();
	mInfo.Destroy();
	mCompare.Destroy();
	mPlayerItem.Destroy();

	CMenuData::Destroy();
}
////////////////////////////////////////////////////////////////////
CFleaMarketMenu::~CFleaMarketMenu()
{
}
////////////////////////////////////////////////////////////////////
CFarmMenu::~CFarmMenu()
{
	delete[] mText;
}
void	CFarmMenu::Destroy(void)
{
	mList.Destroy();
	mHelp.Destroy();
	mSWindow.Destroy();
	mPlayerItem.Destroy();
	
	if(mCDataMove){
		for(int i=0;i<MAX_CHARACTER_NUMBER+MAX_PARTY_NUMBER;i++){
			mCDataWindow[i].Destroy();
		}
	}

	Flag.AllHeal();

	CMenuData::Destroy();
}
////////////////////////////////////////////////////////////////////
CSeedMenu::~CSeedMenu()
{
	delete[] mHelp;
}
void	CSeedMenu::Destroy(void)
{
	mSeedWindow.Destroy();
	mWindow.Destroy();
	CMenuData::Destroy();
}
////////////////////////////////////////////////////////////////////
CSellYesNoMenu::~CSellYesNoMenu()
{
}
void	CSellYesNoMenu::Destroy(void)
{
	mList.Destroy();
	mTitle.Destroy();
	CMenuData::Destroy();
}
////////////////////////////////////////////////////////////////////
CReturnVillage::~CReturnVillage()
{
}
void	CReturnVillage::Destroy(void)
{
	mList.Destroy();
	mTitle.Destroy();
	CMenuData::Destroy();
}
////////////////////////////////////////////////////////////////////
CDiaryMenu::~CDiaryMenu()
{
}
void	CDiaryMenu::Destroy(void)
{
	mList.Destroy();
	mTitle.Destroy();
	mYesNo.Destroy();
	CMenuData::Destroy();
}
////////////////////////////////////////////////////////////////////
CClosetMenu::~CClosetMenu()
{
}
void	CClosetMenu::Destroy(void)
{
	mItemList.Destroy();
	mClosetList.Destroy();
	mEquipWindow.Destroy();
	mCategoryWindow.Destroy();
	mSWindow.Destroy();
	mHelpWindow.Destroy();
	CMenuData::Destroy();
}
////////////////////////////////////////////////////////////////////
CMonthStartMenu::~CMonthStartMenu()
{
}
void	CMonthStartMenu::Destroy(void)
{
	mList.Destroy();
	for(int i=0;i<MAX_CHARACTER_NUMBER+MAX_PARTY_NUMBER;i++){
		mCDataWindow[i].Destroy();
	}
	CMenuData::Destroy();

	Flag.CheckFreeSpace();
	Flag.mClosetUse=false;

	MapMode.SetBGMWait(20);
}
////////////////////////////////////////////////////////////////////
CChangeMemberMenu::~CChangeMemberMenu()
{
}
void	CChangeMemberMenu::Destroy(void)
{
	if(mTop){
		for(int i=0;i<MAX_CHARACTER_NUMBER+MAX_PARTY_NUMBER;i++){
			CMenuData::mCDataWindow[i].Destroy();
		}
	}
	for(int i=0;i<mReserveNo;i++){
		mCDataWindow[i].Destroy();
	}
	
	Flag.CheckFreeSpace();
	CMenuData::Destroy();
}
////////////////////////////////////////////////////////////////////
CConfigMenu::~CConfigMenu()
{
}
void CConfigMenu::Destroy()
{
	mAll.Destroy();
	mHelp.Destroy();
	for(int i=0;i<MENU_NO;i++){
		mSelectWindow[i].Destroy();
	}

	UpData();

	CMenuData::Destroy();
}
////////////////////////////////////////////////////////////////////
CFreeSpaceMenu::~CFreeSpaceMenu()
{
	delete[] mHelp;
	mHelp=NULL;
}
void	CFreeSpaceMenu::Destroy(void)
{
	mLearnSkill.Destroy();
	CMenuData::Destroy();
}
////////////////////////////////////////////////////////////////////

void	CFSSelectSkillMenu::Destroy(void)
{
	CLearnSkillMenu::Destroy();
	mOldSkill.Destroy();
}
////////////////////////////////////////////////////////////////////
CGlobalConfigMenu::~CGlobalConfigMenu()
{
}
void CGlobalConfigMenu::Destroy()
{
	mAll.Destroy();
	GlobalFlag.mFullScreen=(mSelectWindow.GetCursorItem()->mID!=0);
	GlobalFlag.Save();

	mSelectWindow.Destroy();

	CMenuData::Destroy();
}
////////////////////////////////////////////////////////////////////

CKeyConfigMenu::~CKeyConfigMenu()
{
}
void CKeyConfigMenu::Destroy()
{
	mList.Destroy();

	CMenuData::Destroy();
}
////////////////////////////////////////////////////////////////////

CDInfoMenu::~CDInfoMenu()
{
	delete[] mEnemy;
}
void CDInfoMenu::Destroy()
{
	mList.Destroy();
	mWindow.Destroy();

	CMenuData::Destroy();
}
////////////////////////////////////////////////////////////////////
CBypathMenu::~CBypathMenu()
{
}
void	CBypathMenu::Destroy(void)
{
	mList.Destroy();
	mTitle.Destroy();
	CMenuData::Destroy();
}
////////////////////////////////////////////////////////////////////
CStatusHelp::~CStatusHelp()
{
	delete[] mText;
	mText=NULL;
}
void	CStatusHelp::Destroy(void)
{
	mWindow.Destroy();
	CMenuData::Destroy();
}
////////////////////////////////////////////////////////////////////
CRemoveBirthStoneMenu::~CRemoveBirthStoneMenu()
{
}
void	CRemoveBirthStoneMenu::Destroy(void)
{
	mSWindow.Destroy();
	mWindow.Destroy();
	CMenuData::Destroy();
}
////////////////////////////////////////////////////////////////////

CRemoveBSPersonal::~CRemoveBSPersonal()
{
}
void	CRemoveBSPersonal::Destroy(void)
{
	mList.Destroy();
	mMonthList.Destroy();
	mSkillList.Destroy();
	mCDataWin.Destroy();
	CMenuData::Destroy();

}
////////////////////////////////////////////////////////////////////

CSelectDifficult::~CSelectDifficult()
{
}
void	CSelectDifficult::Destroy(void)
{
	mList.Destroy();
	mHelp.Destroy();
	mTitle.Destroy();

	CMenuData::Destroy();
}
////////////////////////////////////////////////////////////////////

CExitYesNo::~CExitYesNo()
{
}
void	CExitYesNo::Destroy(void)
{
	mList.Destroy();
	mTitle.Destroy();
	CMenuData::Destroy();
}
////////////////////////////////////////////////////////////////////

CSuccessSaveDataMenu::~CSuccessSaveDataMenu()
{
}
void	CSuccessSaveDataMenu::Destroy(void)
{
	mHelp.Destroy();
	CDiaryMenu::Destroy();
}
////////////////////////////////////////////////////////////////////
CCallPetHelp::~CCallPetHelp()
{
}
void	CCallPetHelp::Destroy(void)
{
	CSelectHelp::Destroy();
}
////////////////////////////////////////////////////////////////////

CTutorialMenu::~CTutorialMenu()
{
}
void	CTutorialMenu::Destroy(void)
{
	mWindow.Destroy();

	CMenuData::Destroy();
}

////////////////////////////////////////////////////////////////////
CSoundTestMenu::~CSoundTestMenu()
{
}
void	CSoundTestMenu::Destroy(void)
{
	mCurrent.Destroy();
	mBGMWindow.Destroy();
	CMenuData::Destroy();
}
////////////////////////////////////////////////////////////////////
CPreciousItemMenu::~CPreciousItemMenu()
{
}
void	CPreciousItemMenu::Destroy(void)
{
	mHelpWindow.Destroy();
	mCategoryWindow.Destroy();
	mClosetList.Destroy();
	CMenuData::Destroy();
}
////////////////////////////////////////////////////////////////////