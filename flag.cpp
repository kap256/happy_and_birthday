#include "flag.h"
#include "string.h"
#include "input.h"
#include "main.h"
#include "changemode.h"
#include "month.h"
#include "option.h"
#include "itemdata.h"
#include "cursor.h"
#include "mapmode.h"

///////////////////////////////////////////////////////////////////////////////////
CGlobalFlag::CGlobalFlag()
{
	mFullScreen=false;
	mLastSave=0;
	mSoundTest=false;
	memset(mSound,0,sizeof(mSound));
	for(int n=0;n<MAX_KEY;n++){
		mKeyConfig[n]=n;
	}
}
void CGlobalFlag::Save()
{
	FILE* pFile=fopen("save\\gsave","wb");
	if(pFile){
		_putw((mFullScreen?1:0),pFile);	
		fwrite(mKeyConfig,1,sizeof(mKeyConfig),pFile);
		_putw(mLastSave,pFile);	
		_putw((mSoundTest?1:0),pFile);	
		fwrite(mSound,1,sizeof(mSound),pFile);
		fclose(pFile);
	}

	Update();
}
void CGlobalFlag::Load()
{
	FILE* pFile=fopen("save\\gsave","rb");
	if(pFile){
		mFullScreen=(_getw(pFile)!=0);
		fread(mKeyConfig,1,sizeof(mKeyConfig),pFile);
		mLastSave=_getw(pFile);
		mSoundTest=(_getw(pFile)!=0);
		fread(mSound,1,sizeof(mSound),pFile);
		fclose(pFile);
	}

	Update();
}
void CGlobalFlag::Update(void)
{
	for(int n=0;n<MAX_KEY;n++){
		pInput->SetKeyCustom(n,mKeyConfig[n]);
	}
#ifndef	_LOCK_FULL_SCREEN_
	DTest.FullScreen(mFullScreen);
	DTest.ShowCursor(!mFullScreen);
#endif
}
///////////////////////////////////////////////////////////////////////////////////
void CFlag::Reset()
{
	memset(mFlag,0,sizeof(mFlag));
	memset(mTreasure,0,sizeof(mTreasure));
	memset(mFriend,0,sizeof(mFriend));
	memset(mEggFriend,0,sizeof(mEggFriend));
	memset(mParty,-1,sizeof(mParty));
	memset(mMarket,0,sizeof(mMarket));
	memset(mCloset,0,sizeof(mCloset));
	memset(mFarm,0,sizeof(mFarm));
	memset(mShopNew,0,sizeof(mShopNew));
	memset(mMarketNew,0,sizeof(mMarketNew));
	memset(mShopOK,0,sizeof(mShopOK));

	mPlayTime=0;
	mBiology=false;

	if(!mChangeOptionTitle){
		mSE=true;
		mBGM=true;
		mUseSaveCursor=1;
		mMapMoveTurn=false;
		mDropUsedItem=false;
		mAutoOrderItem=true;
		mBattleSpeed=true;
		mAutoDash=false;
	}

	mBypathMapNo=-1;
	mMonth=1;
	mCurrentMonth=-1;
	mHardMode=0;

	mClearData=0;

	mMoney=300;
	int i;
	for(i=0;i<MAX_CHARACTER_NUMBER;i++){
		mPlayerData[i].Init(i);
	}

	for(i=0;i<TEMP_ITEM_MAX;i++){
		mTempItem[i]=NO_ITEM;
	}
	mFriend[0]=true;
	mFriend[1]=true;
	mFriend[2]=true;
	mFriend[3]=true;


	mParty[0]=0;
	mParty[1]=1;
	mParty[2]=2;
	mParty[3]=3;

	mMarket[ITEM_BIRTH_STONE]=256;
	mMarket[ITEM_BIRTH_STONE+13]=1;

	mFarm[0]=30;
	mCloset[ID_BS+0]=2;

	mShopOK[1]=1;
	mShopOK[2]=1;

	mClosetUse=false;

/*	for(i=0;i<MAX_PARTY_NUMBER;i++){
		mFormationX[i]=i;
//		mFormationY[i]=(i*2+1)%5;
	}
*/	mFormationY[0]=1;
	mFormationX[0]=1;
	mFormationY[1]=2;
	mFormationX[1]=1;
	mFormationY[2]=0;
	mFormationX[2]=2;
	mFormationY[3]=3;
	mFormationX[3]=2;

	Market();

#ifdef _ALL_ITEM_
	for(int i=0;i<MAX_CHARACTER_NUMBER;i++){
		mFriend[i]=true;
	}
	mMonth=12;
	memset(mCloset,1,sizeof(mCloset));
	for(int i=0;i<VILLAGE_SHOP_MAX;i++){
		mShopOK[i]=1;
	}

#endif
	AllHeal();
}
void CFlag::NewGame()
{
	ChangeMapInfo.ChangeMap(MonthData[0].mStartMap,
							MonthData[0].mX,
							MonthData[0].mY,
							MonthData[0].mZ,
							0);
	mClosetUse=false;
	for(int i=0;i<MAX_PARTY_NUMBER;i++){
		mParty[i]=MonthData[0].mParty[i];
		if(mParty[i]<2 && (mHardMode&1)){
			mParty[i]=1-mParty[i];
		}
	}
	mCurrentMonth=0;
	Cursor.Escape();
}
void	CFlag::NewMonth(int month)
{
	if(mMonth>=month)	return ;
	mMonth=month;

	for(int i=0;i<MAX_SHOP_ITEM;i++){
		int item=ShopData[mHardMode][month-1][i];
		if(item<0)	break;
		mMarket[item]++;
		mMarketNew[item]=true;
	}

	for(int i=0;i<3;i++){
		mShopNew[i]=(ShopData[i+2][month-1][0]>=0);
	}
	mShopNew[3]=(ShopData[5+GetBusinessAbility()][month-1][0]>=0);
}

void	CFlag::GetShopItem(int shop,bool item[MAX_ITEM_NUMBER])
{
	for(int i=Flag.mMonth-1;i>=0;i--){
		for(int j=0;j<MAX_SHOP_ITEM;j++){
			if(ShopData[shop][i][j]>=0){
				item[ShopData[shop][i][j]]=true;
			}else{
				break;
			}
		}
	}
}
void	CFlag::Market(void)
{
//�s�v�i�̏���
	int i;
	for(i=ID_UI;i<ID_NSUI;i++){
		mMarket[i]=0;
	}
//�o�W����Ȃ��A�C�e���̌���
	bool mask[MAX_ITEM_NUMBER];
	memset(mask,0,sizeof(mask));

	for(i=2;i<5;i++){
		GetShopItem(i,mask);
	}
	GetShopItem(5+GetBusinessAbility(),mask);
	GetShopItem(MAX_SHOP_NO-1,mask);

//���i�̌���
	int market[MAX_ITEM_NUMBER];
	int itemno=0;
	int marketstart=0;
	for(i=0;i<MAX_ITEM_NUMBER;i++){
		if(mask[i])	continue;
		if(mMarket[i]>0){
			if((AllItem[i]->mType & TYPE_RARE) && (AllItem[i]->mType & TYPE_USE_ITEM)){
				mFleaMarketList[marketstart]=i;
				marketstart++;
			}else{
				market[itemno]=i;
				itemno++;
			}
		}
	}
	int marketno=pInput->Rand(FLEA_MARKET_ITEM_MAX-2)+2;
	if(itemno<=0){
		marketno=marketstart;
	}

	mShopNew[4]=false;
	memset(mFleaMarketNew,0,sizeof(mFleaMarketNew));
	for(i=marketstart;i<marketno;i++){
		int rnd=pInput->Rand(itemno);
		mFleaMarketList[i]=market[rnd];
		if(market[rnd]==NO_ITEM){
			marketno=i;
			break;
		}
		if(mMarketNew[market[rnd]]){
			mShopNew[4]=true;
			mFleaMarketNew[i]=true;
		}
		market[rnd]=NO_ITEM;
	}
	for(i=marketno;i<FLEA_MARKET_ITEM_MAX;i++){
		mFleaMarketList[i]=NO_ITEM;
	}
}

void	CFlag::SetMarket(int* item)
{
	char market[MAX_ITEM_NUMBER];
	memset(market,0,sizeof(market));

	int marketno=0;
	for(int i=0;i<FLEA_MARKET_ITEM_MAX;i++){
		if(item[i]>=NO_ITEM){
			continue;
		}
		if(mMarket[item[i]]-market[item[i]]>0){
			mFleaMarketList[marketno]=item[i];
			market[item[i]]++;
			marketno++;
		}
	}
	for(int i=marketno;i<FLEA_MARKET_ITEM_MAX;i++){
		mFleaMarketList[i]=NO_ITEM;
	}
}

int		CFlag::GetAllItem(int item,bool spend,int closet)
//closet 0�Ȃ畕��A1�Ȃ�ꍇ�ɂ��A�Q�Ȃ��Ύg�p�B
{
	int no=0;
	for(int i=0;i<MAX_PARTY_NUMBER+MAX_CHARACTER_NUMBER;i++){
		for(int j=0;j<MAX_ITEM;j++){
				//TODO ���Ƃ܂킵
			if(mPlayerData[mParty[i]].GetItem(j) == item){
				if(item > ID_UI && (UseItemData[item-ID_UI].mType&TYPE_COLLECT)){
					int count=mPlayerData[mParty[i]].GetItem(j).param;
					if(count<=0)	count=1;
					no+=count;
				}else{
					no++;
				}
				if(spend){
					mPlayerData[mParty[i]].SetItem(j,NO_ITEM);
					mPlayerData[mParty[i]].TakeOff(j);
				}
			}
		}
	}

	for(int i=0;i<TEMP_ITEM_MAX;i++){
		if(mTempItem[i] == item){
			if(item > ID_UI && (UseItemData[item-ID_UI].mType&TYPE_COLLECT)){
				int count=mTempItem[i].param;
				if(count<=0)	count=1;
				no+=count;
			}else{
				no++;
			}
			if(spend){
				mTempItem[i]=NO_ITEM;
			}
		}
	}

	if((mClosetUse?1:0) + closet>=2){
		no+=mCloset[item];
		if(spend){
			mCloset[item]=0;
		}
	}

//	return 50;
	return no;
}
int	CFlag::GetUsedFarmItem(int item)
{
	int ret=GetAllItem(item,false,2);
	if(item>=ID_UI && item<ID_UI+MAX_SEED_NUMBER){
		ret+=mUsedFarm[item-ID_UI];
	}
	return ret;
}
void	CFlag::SpendAllItem(int item,bool closet)
{
	//TODO ���Ƃ܂킵
	for(int i=0;i<TEMP_ITEM_MAX;i++){
		if(mTempItem[i] == item){
			CarryItem &ci=mTempItem[i];
			if(ci.param<=1){
				mTempItem[i]=NO_ITEM;
			}else{
				ci.param--;
			}
			return ;
		}
	}
	if(mClosetUse || closet){
		if(mCloset[item]>0){
			mCloset[item]--;
			return ;
		}
	}

	for(int i=0;i<MAX_PARTY_NUMBER;i++){
		for(int j=0;j<MAX_ITEM;j++){
			if(mPlayerData[mParty[i]].GetItem(j) == item){
				if(item > ID_UI && (UseItemData[item-ID_UI].mType&TYPE_COLLECT)){
					CarryItem ci=mPlayerData[mParty[i]].GetItem(j);
					if(ci.param<=1){
						mPlayerData[mParty[i]].SetItem(j,NO_ITEM);
					}else{
						ci.param--;
						mPlayerData[mParty[i]].SetItem(j,ci);
					}
				}else{
					mPlayerData[mParty[i]].SetItem(j,NO_ITEM);
				}
				mPlayerData[mParty[i]].TakeOff(j);
				return;
			}
		}
	}
}
int		CFlag::SetTempItem(int item)
{
	//TODO ���Ƃ܂킵
	for(int i=0;i<TEMP_ITEM_MAX;i++){
		if(mTempItem[i] == NO_ITEM){
			mTempItem[i]=item;
			
			int type=AllItem[item]->mType;
			if((type & TYPE_USE_ITEM) && (type & TYPE_SPEND)){
				mTempItem[i].param=UseItemData[item-ID_UI].mUseCount;
			}
			return i;
		}
	}
	return -1;
}
int	CFlag::GetTempItem(int no)
{
	if(mTempItem[no]!=NO_ITEM){
		int &item=mTempItem[no].item;
		if(item > ID_UI && ((AllItem[item]->mType)&TYPE_SEND_CLOSET)){
			SetCloset(mTempItem[no]);
			mTempItem[no]=NO_ITEM;
			return 4;
		}
		if(item > ID_UI && ((AllItem[item]->mType)&TYPE_COLLECT)){
			for(int i=0;i<MAX_PARTY_NUMBER;i++){
				if(mParty[i]<0)	continue;
				if(mPlayerData[mParty[i]].SearchItem(item)>=0){
					mPlayerData[mParty[i]].SetItem(mTempItem[no]);
					mTempItem[no]=NO_ITEM;
					return i;
				}
			}
		}
		for(int i=0;i<MAX_PARTY_NUMBER;i++){
			if(mParty[i]<0)	continue;
			if(mPlayerData[mParty[i]].SetItem(mTempItem[no])>=0){
				mTempItem[no]=NO_ITEM;
				return i;
			}
		}
		return -1;
	}
	return -2;
}
void	CFlag::DropTempItem(bool closet)
{
	for(int i=0;i<TEMP_ITEM_MAX;i++){
		if(mTempItem[i]!=NO_ITEM){
			if(closet){
				SetCloset(mTempItem[i]);
			}else{
				mMarket[mTempItem[i].item]++;
			}
			mTempItem[i]=NO_ITEM;
		}
	}
}
void	CFlag::SetCloset(CarryItem item,CPlayerData* pdata)
{
	for(int i=0;i<MAX_CHARACTER_NUMBER;i++){
		if(&mPlayerData[i]==pdata){
			SetCloset(item);
			return;
		}
	}
}
void	CFlag::SetCloset(CarryItem item)
{
	int i=item.item;
	if(i<0		|| MAX_ITEM_NUMBER<=i)		return;
	if(ID_UI<=i	&& i<ID_UI+MAX_SEED_NUMBER)	return;
	
	if(ID_UI<=i && i< ID_SD && (UseItemData[i-ID_UI].mType&TYPE_COLLECT)){
		if(item.param<=0)	item.param=1;
		mCloset[i]+=item.param;
	}else if(ID_UI<=i && i< ID_SD &&(UseItemData[i-ID_UI].mType&TYPE_MONEY)){
		mMoney+=UseItemData[i-ID_UI].GetPrice();
	}else{
		mCloset[i]+=1;
	}
	if(mCloset[i]>=100)	mCloset[i]=99;
}
void	CFlag::SetCloset(int item,int no)
{
	if(item<0		|| MAX_ITEM_NUMBER<=item)		return;
	if(ID_UI<=item	&& item<ID_UI+MAX_SEED_NUMBER)	return;
	mCloset[item]+=no;
	if(mCloset[item]>=100)	mCloset[item]=99;
}
int	CFlag::GetCloset(int item,bool one)
{
	if(item<0 || MAX_ITEM_NUMBER<=item)	return true;
	if(mCloset[item]>0){
		if((!one) && (item > ID_UI)&& (UseItemData[item-ID_UI].mType&TYPE_COLLECT)){
			int ret=mCloset[item];
			mCloset[item]=0;
			return ret;
		}else{
			mCloset[item]--;
			return 1;
		}
	}
	return 0;
}
int		CFlag::GetFarm(int no)
{
	int farm=mFarm[no];
	if(SearchFSkill(320,true)){
		farm=(farm*3+1)/2;
	}
	return farm;
}
bool	CFlag::SearchFSkill(int no,bool reserve)
{
	for(int i=0;i<MAX_PARTY_NUMBER;i++){
		if(mParty[i]<0)	continue;
		if(mPlayerData[mParty[i]].SearchASkill(no))	return true;
	}
	if(reserve){
		for(int i=0;i<MAX_CHARACTER_NUMBER;i++){
			if(mFriend[i]){
				if(mPlayerData[i].SearchASkill(no))	return true;
			}
		}
	}
	return false;
}
int		CFlag::GetBusinessAbility(void)
{
	int ba=0;
	if(SearchFSkill(317,true))	ba+=1;
	if(SearchFSkill(318,true))	ba+=2;
	return ba;
}
int		CFlag::CheckLearnSkill(int skill)
{
	int ret=0;
	for(int i=0;i<MAX_PARTY_NUMBER;i++){
		if(mPlayerData[mParty[i]].CheckLearnSkill(skill))	ret+=1<<i;
	}
	return ret;
}
void	CFlag::CheckFreeSpace(void)
{
	for(int i=0;i<MAX_PARTY_NUMBER;i++){
		if(mParty[i]<0) continue ;
		CPlayerData* pdata=&mPlayerData[mParty[i]];
		int fs=pdata->SearchASkillType(AS_FREE_SPACE,true);
		for(int j=0;j<fs;j++){
			int skill=pdata->GetFSpace(j);
			if(CheckLearnSkill(skill)<=0){
				pdata->SetFSpace(j,-1);
			}
		}
	}
}
void	CFlag::CheckRareItem(void)
{
	//����e�X�g���ĂȂ���
	bool fri[MAX_CHARACTER_NUMBER];
	memset(fri,0,sizeof(fri));
	for(int i=0;i<MAX_CHARACTER_NUMBER+MAX_PARTY_NUMBER;i++){
		if(mParty[i]<0)						continue ;
		if(mParty[i]>=MAX_CHARACTER_NUMBER) continue ;
		fri[mParty[i]]=true;
	}
	for(int i=0;i<MAX_CHARACTER_NUMBER;i++){
		if(fri[i])	continue;
		CPlayerData* pdata=&mPlayerData[i];
		int ino=pdata->GetItemMax();
		for(int j=0;j<ino;j++){
			CarryItem item=pdata->GetItem(j);
			if(AllItem[item.item]->GetRare()){
				SetCloset(item);
				pdata->SetItem(j,NO_ITEM);
			}
		}
	}
}
int	CFlag::GetDInfo(bool reserve)
{
	int know=0;
	if(Flag.SearchFSkill(319,reserve)){
		know+=4+8+16;
	}
	if(Flag.SearchFSkill(338,reserve)){
		know+=32+64+128;
	}
	if(Flag.SearchFSkill(339,reserve)){
		know+=256+512+1024;
	}
	if(Flag.SearchFSkill(340,reserve)){
		know+=2048+1+2;
	}
	return know*2;
}
bool CFlag::GetDInfo(int month,bool reserve)
{
	if(month<0)	return false;
	int m=MonthData[month].mMonth;
	int know=GetDInfo(reserve);
	return ((1<<m)&know)!=0;
}
void	CFlag::AllHeal(void)
{
	for(int i=0;i<MAX_CHARACTER_NUMBER;i++){
		mPlayerData[i].AllHeal();
	}
	memset(mUsedFarm,0,sizeof(mUsedFarm));
//	mBypathMapNo=-1;
}

void	CFlag::CheckReserve(void)
{
//����̍쐬
	static bool friends[MAX_CHARACTER_NUMBER];
	memcpy(friends,mFriend,sizeof(friends));
	if(mCurrentMonth>=17){
		for(int i=0;i<MAX_CHARACTER_NUMBER;i++){
			if(mEggFriend[i])	friends[i]=true;
		}
	}

//����ɂȂ������o�[�A�d�������o�[�̍폜
	for(int i=0;i<MAX_CHARACTER_NUMBER+MAX_PARTY_NUMBER;i++){
		if(i>=MAX_PARTY_NUMBER){
			if(!friends[mParty[i]]){
				mParty[i] = -1;
			}
		}
		if(mParty[i]>=0){
			friends[mParty[i]]=false;
		}
	}

//����Ȃ������o�[�̕�[
	int reserve=MAX_PARTY_NUMBER;
	for(int i=0;i<MAX_CHARACTER_NUMBER;i++){
		if(friends[i]){
			for(;reserve<MAX_CHARACTER_NUMBER+MAX_PARTY_NUMBER;reserve++){
				if(mParty[reserve]<0)	break;
			}
			if(reserve>=MAX_CHARACTER_NUMBER+MAX_PARTY_NUMBER)	break;
			mParty[reserve]=i;
		}
	}

//�󂫃X�y�[�X�̉���
	memset(friends,0,sizeof(friends));
	for(int i=MAX_PARTY_NUMBER;i<MAX_CHARACTER_NUMBER+MAX_PARTY_NUMBER;i++){
		if(mParty[i]<0){
			for(int j=i;j<MAX_PARTY_NUMBER+MAX_CHARACTER_NUMBER-1;j++){
				mParty[j]=mParty[j+1];
			}
			mParty[MAX_PARTY_NUMBER+MAX_CHARACTER_NUMBER-1]=-1;
		}
	}
}

void	CFlag::AddMember(int character)
{
	for(int i=0;i<MAX_PARTY_NUMBER;i++){
		if(mParty[i]==character){
			return;
		}
	}

	for(int i=0;i<MAX_PARTY_NUMBER;i++){
		if(MonthData[mCurrentMonth].mParty[i]!=-1){
			if(mParty[i]<0){
				ChangeMember(i,character);
				return;
			}
		}
	}
	for(int i=0;i<MAX_PARTY_NUMBER;i++){
		if(mParty[i]<0){
			ChangeMember(i,character);
			return;
		}
	}
}
void	CFlag::ChangeMember(int no,int character)
{
	int old=mParty[no];
	mParty[no]=character;
	if(!mFriend[old]){
		CheckReserve();
		return;
	}

	if(mParty[MAX_PARTY_NUMBER]>=0 && mParty[MAX_PARTY_NUMBER]!=character){
		for(int i=MAX_PARTY_NUMBER+MAX_CHARACTER_NUMBER-1;i>MAX_PARTY_NUMBER;i--){
			mParty[i]=mParty[i-1];
		}
	}
	mParty[MAX_PARTY_NUMBER]=old;
	CheckReserve();
}

int		CFlag::SearchMember(int character,int max)
{
	for(int i=0;i<max;i++){
		if(mParty[i]==character)	return  i;
	}
	return -1;
}
//////////////////////////////////////
void	CFlag::Save(int no)
{
	FILE* pFile;
	static char fname[256];
	sprintf(fname,"save\\save%02d",no);
	if(pFile=fopen(fname,"wb")){
		int i;

		_putw(11,pFile);			//�o�[�W��������
		_putw(mMonth,pFile);		//�Q�[���i�s�x
		_putw(mPlayTime,pFile);		//�v���C����
		_putw(mHardMode,pFile);		//�Q�[�����[�h
		_putw(mClearData,pFile);	//�N���A���

		long pos=ftell(pFile);
		fwrite(&pos,sizeof(long),1,pFile);
							//�ȏ�A�Z�[�u���[�h�̃V�X�e���Ŏg�p�B

		_putw(ENEMY_NO,pFile);				//ENEMY_NO	****����͐�΂Ɉ�ԏ�B****
		_putw(MAX_CHARACTER_NUMBER,pFile);	//MAX_CHARACTER_NUMBER
		_putw(MAX_PARTY_NUMBER,pFile);		//MAX_PARTY_NUMBER
		_putw(MAX_ITEM_NUMBER,pFile);		//MAX_ITEM_NUMBER
		_putw(MAX_SEED_NUMBER,pFile);		//MAX_SEED_NUMBER
		_putw(FLAG_MAX,pFile);				//FLAG_MAX
		_putw(TREASURE_NO,pFile);			//TREASURE_NO
							//�ȏ�A�z��̓ǂݍ��݂Ŏg�p�B

		_putw((mSE?1:0),pFile);		//���ʉ��Đ�
		_putw((mBGM?1:0),pFile);	//BGM�Đ�
		_putw(mUseSaveCursor,pFile);	//�J�[�\���L�����g�����ǂ���
		_putw((mMapMoveTurn?1:0),pFile);	
		_putw((mDropUsedItem?1:0),pFile);	
		_putw((mAutoOrderItem?1:0),pFile);	
		_putw((mBattleSpeed?1:0),pFile);	
		_putw((mAutoDash?1:0),pFile);	
							//�ȏ�A�ݒ�f�[�^

		_putw(mMoney,pFile);		//������
		for(i=0;i<MAX_CHARACTER_NUMBER;i++){
			_putw((mFriend[i]?1:0),pFile);		//���ԂɂȂ������ǂ���
			_putw((mEggFriend[i]?1:0),pFile);		//���ԂɂȂ������ǂ���
			mPlayerData[i].Save(pFile);			//�X�̃L�����N�^�[�ɃZ�[�u���܂铊��
		}

		for(i=0;i<MAX_CHARACTER_NUMBER+MAX_PARTY_NUMBER;i++){
			_putw(mParty[i],pFile);	//���ԃL����
		}
		for(i=0;i<MAX_PARTY_NUMBER;i++){				
			_putw(mFormationX[i],pFile);
			_putw(mFormationY[i],pFile);		//�w�`���
		}
							//�ȏ�A�v���C���[�f�[�^

		for(i=0;i<MAX_ITEM_NUMBER;i++){
			_putw(mMarket[i],pFile);		//�@��o������
			_putw(mCloset[i],pFile);		//�q��
		}
		for(i=0;i<MAX_SEED_NUMBER;i++){
			_putw(mFarm[i],pFile);			//�앨�̐��Y��
		}
							//�ȏ�A�A�C�e���f�[�^

		for(i=0;i<FLAG_MAX;i++){
			_putw(mFlag[i],pFile);			//�ʏ�t���O
		}
		for(i=0;i<TREASURE_NO;i++){
			_putw((mTreasure[i]?1:0),pFile);//�󔠃t���O
		}
							//�ȏ�A�t���O
		_putw(mCurrentMonth,pFile);

		mSaveCursor.Save(pFile);

		for(i=0;i<4;i++){
			_putw((mShopNew[i]?1:0),pFile);//�V���i���׏��
		}
		if(ChangeMapInfo.mNo<=-2){
			_putw(-1,pFile);
		}else{
			_putw(ChangeMapInfo.mNo,pFile);
		}
		_putw(ChangeMapInfo.mX,pFile);
		_putw(ChangeMapInfo.mY,pFile);
		_putw(ChangeMapInfo.mZ,pFile);
		_putw(ChangeMapInfo.mDir,pFile);
	
		_putw(mMapBGMNo,pFile);			//BGM

		long enepos=ftell(pFile);
		fseek(pFile,pos,SEEK_SET);
		fwrite(&enepos,sizeof(long),1,pFile);
		fseek(pFile,enepos,SEEK_SET);

		for(i=0;i<ENEMY_NO;i++){		//�G�̏��
			mEnemyOpenData[i].Save(pFile);
		}
		fwrite(mMarketNew,1,sizeof(mMarketNew),pFile);//�V���i���
		fwrite(mUsedFarm,1,sizeof(mUsedFarm),pFile);//�g�p�ςݍ앨

		_putw(mBypathMapNo,pFile);
		_putw(mBypathX,pFile);
		_putw(mBypathY,pFile);
		_putw(mBypathZ,pFile);
		_putw(mBypathDir,pFile);
		_putw(mBypathBGM,pFile);

		fwrite(mShopOK,1,sizeof(mShopOK),pFile);		//���̎{�݂��g���邩�ǂ����B

		fclose(pFile);
	}
}
bool	CFlag::Load(int no)
{
	Reset();
	FILE* pFile;
	static char fname[256];
	sprintf(fname,"save\\save%02d",no);
	if(pFile=fopen(fname,"rb")){
		int ver=_getw(pFile);
		switch(ver){
			case 0:
			case 1:
			case 2:
				Load_0(pFile,ver);	break;
			case 3:
			case 4:
			case 5:
				Load_3(pFile,ver);	break;
			case 6:
				Load_6(pFile,ver);	break;
			case 7:
			case 8:
				Load_7(pFile,ver);	break;
			case 9:
			case 10:
			case 11:
				Load_9(pFile,ver);	break;
			default:
				return false;
		}
		if(ver<=10){
			for(int i=0;i<MAX_CHARACTER_NUMBER;i++){
				CPlayerData &cdata=mPlayerData[i];
				for(int j=0;j<16;j++){
					CarryItem item=cdata.GetItem(j);

					SetCloset(item);
					cdata.TakeOff(j);
					cdata.SetItem(j,NO_ITEM);
				}
			}
		}
		fclose(pFile);
#ifdef _OUTPUT_GET_TRESURE_
	if(pFile=fopen("data\\ �󔠊l����.txt","w")){
		for(int i=0;i<TREASURE_NO;i++){
			fprintf(pFile,"%04d�ԁF%s�l��\n",i,(mTreasure[i]?"":"��"));
		}
		fclose(pFile);
	}
#endif
		Market();
//		mPlayerData[6].Init(6);
#ifdef _SET_FLAG_
		mFlag[_SET_FLAG_ID_]=_SET_FLAG_NO_;
#endif
		return true;
	}
	return false;
}

bool	CFlag::LoadSuccessData(int no)
{
	
	FILE* pFile;
	static char fname[256];
	sprintf(fname,"save\\save%02d",no);
	if(pFile=fopen(fname,"rb")){
		int ver=_getw(pFile);
		switch(ver){
			case 0:
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:
				LoadSuccessData_0(pFile,ver);	break;
			case 9:
			case 10:
			case 11:
				LoadSuccessData_9(pFile,ver);	break;
			default:
				return false;
		}
		fclose(pFile);
		return true;
	}
	return false;
}

/////���[�h/////////////////////////////////////////////////////////////

bool	CFlag::Load_0(FILE* pFile,int ver)
{
	int i;

	mMonth=_getw(pFile);	//�Q�[���i�s�x
	mPlayTime=_getw(pFile);
	mHardMode=_getw(pFile);
	_getw(pFile);
						//�ȏ�A�Z�[�u���[�h�̃V�X�e���Ŏg�p�B

	mSE=(_getw(pFile)!=0);
	mBGM=(_getw(pFile)!=0);
	mUseSaveCursor=(_getw(pFile)!=0);
	mMapMoveTurn=(_getw(pFile)!=0);
	_getw(pFile);
	_getw(pFile);
	_getw(pFile);
	_getw(pFile);
						//�ȏ�A�ݒ�f�[�^

	mMoney=_getw(pFile);
	for(i=0;i<MAX_CHARACTER_NUMBER;i++){
		mFriend[i]=(_getw(pFile)!=0);
		switch(ver){
			case 2:
				mPlayerData[i].Load_2(pFile);	break;
			case 1:
				mPlayerData[i].Load_1(pFile);	break;
			default:
				mPlayerData[i].Load_0(pFile);	break;
		}	//�X�̃L�����N�^�[�Ƀ��[�h���܂铊��
	}
	for(i=0;i<MAX_PARTY_NUMBER;i++){
		mParty[i]=_getw(pFile);
		mFormationX[i]=_getw(pFile);
		mFormationY[i]=_getw(pFile);
	}
						//�ȏ�A�v���C���[�f�[�^

	for(i=0;i<MAX_ITEM_NUMBER;i++){
		mMarket[i]=_getw(pFile);
		mCloset[i]=_getw(pFile);
	}
	for(i=0;i<MAX_SEED_NUMBER;i++){
		mFarm[i]=_getw(pFile);
	}						//�ȏ�A�A�C�e���f�[�^

	for(i=0;i<FLAG_MAX;i++){
		mFlag[i]=_getw(pFile);
	}
	for(i=0;i<256;i++){
		mTreasure[i]=(_getw(pFile)!=0);
	}
	mCurrentMonth=_getw(pFile);
						//�ȏ�A�t���O

	mSaveCursor.Load_0(pFile);	//�J�[�\���L��

	for(i=0;i<4;i++){
		mShopNew[i]=(_getw(pFile)!=0);	//�V���i�ɂ���
	}

	int mapno=_getw(pFile);
	int x=_getw(pFile);
	int y=_getw(pFile);
	int z=_getw(pFile);
	int dir=_getw(pFile);
	ChangeMapInfo.ChangeMap(mapno,x,y,z,dir);

	mMapBGMNo=_getw(pFile);
	PlayBGM(mMapBGMNo);
	for(i=0;i<ENEMY_NO;i++){		//�G�̏��
		mEnemyOpenData[i].Load_0(pFile);
	}


	return true;
}

bool	CFlag::Load_3(FILE* pFile,int ver)
{
	int i;

	mMonth=_getw(pFile);	//�Q�[���i�s�x
	mPlayTime=_getw(pFile);
	mHardMode=_getw(pFile);
	mClearData=_getw(pFile);
						//�ȏ�A�Z�[�u���[�h�̃V�X�e���Ŏg�p�B

	mSE=(_getw(pFile)!=0);
	mBGM=(_getw(pFile)!=0);
	mUseSaveCursor=(_getw(pFile)!=0);
	mMapMoveTurn=(_getw(pFile)!=0);
	mDropUsedItem=(_getw(pFile)!=0);
	_getw(pFile);
	_getw(pFile);
	_getw(pFile);
						//�ȏ�A�ݒ�f�[�^
	if(!mBGM){
		PlayBGM(0);
	}

	mMoney=_getw(pFile);
	for(i=0;i<15;i++){
		mFriend[i]=(_getw(pFile)!=0);
		switch(ver){
			case 3:
				mPlayerData[i].Load_2(pFile);	break;
			case 4:
				mPlayerData[i].Load_4(pFile);	break;
			case 5:
			default:
				mPlayerData[i].Load_5(pFile);	break;
		}	//�X�̃L�����N�^�[�Ƀ��[�h���܂铊��
	}
	for(i=0;i<19;i++){
		mParty[i]=_getw(pFile);
	}
	for(i=0;i<4;i++){
		mFormationX[i]=_getw(pFile);
		mFormationY[i]=_getw(pFile);
	}
						//�ȏ�A�v���C���[�f�[�^

	for(i=0;i<416;i++){
		mMarket[i]=_getw(pFile);
		mCloset[i]=_getw(pFile);
	}
	for(i=0;i<32;i++){
		mFarm[i]=_getw(pFile);
	}						//�ȏ�A�A�C�e���f�[�^

	for(i=0;i<1024;i++){
		int f=_getw(pFile);
		if(i<FLAG_MAX)		mFlag[i]=f;
	}
	for(i=0;i<256;i++){
		mTreasure[i]=(_getw(pFile)!=0);
	}
	mCurrentMonth=_getw(pFile);
	mOldMonth=mCurrentMonth;
						//�ȏ�A�t���O

	mSaveCursor.Load_0(pFile);	//�J�[�\���L��

	for(i=0;i<4;i++){
		mShopNew[i]=(_getw(pFile)!=0);	//�V���i�ɂ���
	}

	int mapno=_getw(pFile);
	int x=_getw(pFile);
	int y=_getw(pFile);
	int z=_getw(pFile);
	int dir=_getw(pFile);
	ChangeMapInfo.ChangeMap(mapno,x,y,z,dir);

	mMapBGMNo=_getw(pFile);
	if(mapno>=0){
		PlayBGM(mMapBGMNo);
	}
	for(i=0;i<128;i++){		//�G�̏��
		mEnemyOpenData[i].Load_0(pFile);
	}

	fread(mMarketNew,1,sizeof(mMarketNew[0])*416,pFile);//�V���i���
	fread(mUsedFarm,1,sizeof(mUsedFarm[0])*32,pFile);	//�g�p�ςݍ앨

	mBypathMapNo=_getw(pFile);
	mBypathX	=_getw(pFile);
	mBypathY	=_getw(pFile);
	mBypathZ	=_getw(pFile);
	mBypathDir	=_getw(pFile);

	fread(mShopOK,1,sizeof(mShopOK),pFile);		//���̎{�݂��g���邩�ǂ����B

	return true;
}



bool	CFlag::Load_6(FILE* pFile,int ver)
{
	int i;

	mMonth=_getw(pFile);	//�Q�[���i�s�x
	mPlayTime=_getw(pFile);
	mHardMode=_getw(pFile);
	mClearData=_getw(pFile);
						//�ȏ�A�Z�[�u���[�h�̃V�X�e���Ŏg�p�B

	int max_character_number=_getw(pFile);
	int max_party_number	=_getw(pFile);
	int max_item_number		=_getw(pFile);
	int max_seed_number		=_getw(pFile);
	int flag_max			=_getw(pFile);
	int tresure_no			=_getw(pFile);
	int enemy_no			=_getw(pFile);
							//�ȏ�A�z��̓ǂݍ��݂Ŏg�p�B

	mSE=(_getw(pFile)!=0);
	mBGM=(_getw(pFile)!=0);
	mUseSaveCursor=(_getw(pFile)!=0);
	mMapMoveTurn=(_getw(pFile)!=0);
	mDropUsedItem=(_getw(pFile)!=0);
	_getw(pFile);
	_getw(pFile);
	_getw(pFile);
						//�ȏ�A�ݒ�f�[�^
	if(!mBGM){
		PlayBGM(0);
	}

	mMoney=_getw(pFile);
	for(i=0;i<max_character_number;i++){
		mFriend[i]=(_getw(pFile)!=0);
		mPlayerData[i].Load_5(pFile);//�X�̃L�����N�^�[�Ƀ��[�h���܂铊��
	}
	for(i=0;i<max_character_number+max_party_number;i++){
		mParty[i]=_getw(pFile);
	}
	for(i=0;i<max_party_number;i++){
		mFormationX[i]=_getw(pFile);
		mFormationY[i]=_getw(pFile);
	}
						//�ȏ�A�v���C���[�f�[�^

	for(i=0;i<max_item_number;i++){
		mMarket[i]=_getw(pFile);
		mCloset[i]=_getw(pFile);
	}
	for(i=0;i<max_seed_number;i++){
		mFarm[i]=_getw(pFile);
	}						//�ȏ�A�A�C�e���f�[�^

	for(i=0;i<flag_max;i++){
		int f=_getw(pFile);
		if(i<FLAG_MAX)		mFlag[i]=f;
	}
	for(i=0;i<tresure_no;i++){
		mTreasure[i]=(_getw(pFile)!=0);
	}
	mCurrentMonth=_getw(pFile);
	mOldMonth=mCurrentMonth;
						//�ȏ�A�t���O

	mSaveCursor.Load_0(pFile);	//�J�[�\���L��

	for(i=0;i<4;i++){
		mShopNew[i]=(_getw(pFile)!=0);	//�V���i�ɂ���
	}

	int mapno=_getw(pFile);
	int x=_getw(pFile);
	int y=_getw(pFile);
	int z=_getw(pFile);
	int dir=_getw(pFile);
	ChangeMapInfo.ChangeMap(mapno,x,y,z,dir);

	mMapBGMNo=_getw(pFile);
	if(mapno>=0){
		PlayBGM(mMapBGMNo);
	}
	for(i=0;i<enemy_no;i++){		//�G�̏��
		mEnemyOpenData[i].Load_0(pFile);
	}

	fread(mMarketNew,1,sizeof(mMarketNew[0])*max_item_number,pFile);//�V���i���
	fread(mUsedFarm,1,sizeof(mUsedFarm[0])*max_seed_number,pFile);	//�g�p�ςݍ앨

	mBypathMapNo=_getw(pFile);
	mBypathX	=_getw(pFile);
	mBypathY	=_getw(pFile);
	mBypathZ	=_getw(pFile);
	mBypathDir	=_getw(pFile);

	fread(mShopOK,1,sizeof(mShopOK),pFile);		//���̎{�݂��g���邩�ǂ����B

	return true;
}



bool	CFlag::Load_7(FILE* pFile,int ver)
{
	int i;

	mMonth=_getw(pFile);	//�Q�[���i�s�x
	mPlayTime=_getw(pFile);
	mHardMode=_getw(pFile);
	mClearData=_getw(pFile);
						//�ȏ�A�Z�[�u���[�h�̃V�X�e���Ŏg�p�B

	int max_character_number=_getw(pFile);
	int max_party_number	=_getw(pFile);
	int max_item_number		=_getw(pFile);
	int max_seed_number		=_getw(pFile);
	int flag_max			=_getw(pFile);
	int tresure_no			=_getw(pFile);
	int enemy_no			=_getw(pFile);
							//�ȏ�A�z��̓ǂݍ��݂Ŏg�p�B

	mSE=(_getw(pFile)!=0);
	mBGM=(_getw(pFile)!=0);
	mUseSaveCursor=(_getw(pFile)!=0);
	mMapMoveTurn=(_getw(pFile)!=0);
	mDropUsedItem=(_getw(pFile)!=0);
	_getw(pFile);
	_getw(pFile);
	_getw(pFile);
						//�ȏ�A�ݒ�f�[�^
	if(!mBGM){
		PlayBGM(0);
	}

	mMoney=_getw(pFile);
	for(i=0;i<max_character_number;i++){
		mFriend[i]=(_getw(pFile)!=0);
		mEggFriend[i]=(_getw(pFile)!=0);
		switch(ver){
			case 8:
				mPlayerData[i].Load_8(pFile);	break;
			default:
				mPlayerData[i].Load_5(pFile);	break;
		}	//�X�̃L�����N�^�[�Ƀ��[�h���܂铊��
	}
	for(i=0;i<max_character_number+max_party_number;i++){
		mParty[i]=_getw(pFile);
	}
	for(i=0;i<max_party_number;i++){
		mFormationX[i]=_getw(pFile);
		mFormationY[i]=_getw(pFile);
	}
						//�ȏ�A�v���C���[�f�[�^

	for(i=0;i<max_item_number;i++){
		mMarket[i]=_getw(pFile);
		mCloset[i]=_getw(pFile);
	}
	for(i=0;i<max_seed_number;i++){
		mFarm[i]=_getw(pFile);
	}						//�ȏ�A�A�C�e���f�[�^

	for(i=0;i<flag_max;i++){
		int f=_getw(pFile);
		if(i<FLAG_MAX)		mFlag[i]=f;
	}
	for(i=0;i<tresure_no;i++){
		mTreasure[i]=(_getw(pFile)!=0);
	}
	mCurrentMonth=_getw(pFile);
	mOldMonth=mCurrentMonth;
						//�ȏ�A�t���O

	mSaveCursor.Load_0(pFile);	//�J�[�\���L��

	for(i=0;i<4;i++){
		mShopNew[i]=(_getw(pFile)!=0);	//�V���i�ɂ���
	}

	int mapno=_getw(pFile);
	int x=_getw(pFile);
	int y=_getw(pFile);
	int z=_getw(pFile);
	int dir=_getw(pFile);
	ChangeMapInfo.ChangeMap(mapno,x,y,z,dir);

	mMapBGMNo=_getw(pFile);
	if(mapno>=0){
		PlayBGM(mMapBGMNo);
	}
	for(i=0;i<enemy_no;i++){		//�G�̏��
		mEnemyOpenData[i].Load_0(pFile);
	}

	fread(mMarketNew,1,sizeof(mMarketNew[0])*max_item_number,pFile);//�V���i���
	fread(mUsedFarm,1,sizeof(mUsedFarm[0])*max_seed_number,pFile);	//�g�p�ςݍ앨

	mBypathMapNo=_getw(pFile);
	mBypathX	=_getw(pFile);
	mBypathY	=_getw(pFile);
	mBypathZ	=_getw(pFile);
	mBypathDir	=_getw(pFile);

	fread(mShopOK,1,sizeof(mShopOK),pFile);		//���̎{�݂��g���邩�ǂ����B

	return true;
}
bool	CFlag::Load_9(FILE* pFile,int ver)
{
	int i;

	mMonth=_getw(pFile);	//�Q�[���i�s�x
	mPlayTime=_getw(pFile);
	mHardMode=_getw(pFile);
	mClearData=_getw(pFile);
	long l;
	fread(&l,sizeof(long),1,pFile);
						//�ȏ�A�Z�[�u���[�h�̃V�X�e���Ŏg�p�B

	int enemy_no			=_getw(pFile);
	int max_character_number=_getw(pFile);
	int max_party_number	=_getw(pFile);
	int max_item_number		=_getw(pFile);
	int max_seed_number		=_getw(pFile);
	int flag_max			=_getw(pFile);
	int tresure_no			=_getw(pFile);
							//�ȏ�A�z��̓ǂݍ��݂Ŏg�p�B

	if(!mChangeOptionTitle){
		mSE=(_getw(pFile)!=0);
		mBGM=(_getw(pFile)!=0);
		mUseSaveCursor=_getw(pFile);
		mMapMoveTurn=(_getw(pFile)!=0);
		mDropUsedItem=(_getw(pFile)!=0);
		mAutoOrderItem=(_getw(pFile)!=0);
		mBattleSpeed=(_getw(pFile)!=0);
		mAutoDash=(_getw(pFile)!=0);
	}else{
		_getw(pFile);
		_getw(pFile);
		_getw(pFile);
		_getw(pFile);
		_getw(pFile);
		_getw(pFile);
		_getw(pFile);
		_getw(pFile);
	}
	mChangeOptionTitle=false;
						//�ȏ�A�ݒ�f�[�^
	if(!mBGM){
		PlayBGM(0);
	}

	mMoney=_getw(pFile);
	for(i=0;i<max_character_number;i++){
		mFriend[i]=(_getw(pFile)!=0);
		mEggFriend[i]=(_getw(pFile)!=0);
		switch(ver){
			case 9:
			default:
				mPlayerData[i].Load_8(pFile);	break;
		}	//�X�̃L�����N�^�[�Ƀ��[�h���܂铊��
	}
	for(i=0;i<max_character_number+max_party_number;i++){
		mParty[i]=_getw(pFile);
	}
	for(i=0;i<max_party_number;i++){
		mFormationX[i]=_getw(pFile);
		mFormationY[i]=_getw(pFile);
	}
						//�ȏ�A�v���C���[�f�[�^

	for(i=0;i<max_item_number;i++){
		mMarket[i]=_getw(pFile);
		mCloset[i]=_getw(pFile);
	}
	for(i=0;i<max_seed_number;i++){
		mFarm[i]=_getw(pFile);
	}						//�ȏ�A�A�C�e���f�[�^

	for(i=0;i<flag_max;i++){
		int f=_getw(pFile);
		if(i<FLAG_MAX)		mFlag[i]=f;
	}
	for(i=0;i<tresure_no;i++){
		mTreasure[i]=(_getw(pFile)!=0);
	}
	mCurrentMonth=_getw(pFile);
	mOldMonth=mCurrentMonth;
						//�ȏ�A�t���O

	mSaveCursor.Load_0(pFile);	//�J�[�\���L��

	for(i=0;i<4;i++){
		mShopNew[i]=(_getw(pFile)!=0);	//�V���i�ɂ���
	}

	int mapno=_getw(pFile);
	int x=_getw(pFile);
	int y=_getw(pFile);
	int z=_getw(pFile);
	int dir=_getw(pFile);
	ChangeMapInfo.ChangeMap(mapno,x,y,z,dir);

	mMapBGMNo=_getw(pFile);
	SetBGMFadeOut(20);
	if(mapno>=0){
		MapMode.SetBGMWait(30);
	}
	for(i=0;i<enemy_no;i++){		//�G�̏��
		mEnemyOpenData[i].Load_0(pFile);
	}

	fread(mMarketNew,1,sizeof(mMarketNew[0])*max_item_number,pFile);//�V���i���
	fread(mUsedFarm,1,sizeof(mUsedFarm[0])*max_seed_number,pFile);	//�g�p�ςݍ앨

	mBypathMapNo=_getw(pFile);
	mBypathX	=_getw(pFile);
	mBypathY	=_getw(pFile);
	mBypathZ	=_getw(pFile);
	mBypathDir	=_getw(pFile);
	if(ver>=10)	mBypathBGM	=_getw(pFile);

	fread(mShopOK,1,sizeof(mShopOK),pFile);		//���̎{�݂��g���邩�ǂ����B

	return true;
}


void	CFlag::LoadSuccessData_0(FILE* pFile,int ver)
{
					_getw(pFile);
	mPlayTime	=	_getw(pFile);
					_getw(pFile);
	mClearData	=	_getw(pFile);
}
void	CFlag::LoadSuccessData_9(FILE* pFile,int ver)
{
					_getw(pFile);
	mPlayTime	=	_getw(pFile);
					_getw(pFile);
	mClearData	=	_getw(pFile);

	long pos;
	fread(&pos,sizeof(long),1,pFile);
	int enemy_no	=_getw(pFile);
	fseek(pFile,pos,SEEK_SET);

	for(int i=0;i<enemy_no;i++){		//�G�̏��
		mEnemyOpenData[i].Load_0(pFile);
	}
}
