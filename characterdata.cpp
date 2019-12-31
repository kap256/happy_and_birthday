#include "characterdata.h"
#include "main.h"
#include "itemdata.h"
#include "skilldata.h"
#include "enemydata.h"
#include "gamewindow.h"
#include "flag.h"
#include "option.h"
#include "month.h"
///////////////////////////////////////////////////////////////////////////
//成長率がらみ
double	CEnemyData::GetGrowRatio(int lv,GROW_PARAM param)
{
	switch(param){
	//	case GROW_STM:
	//		return lv*(lv+30)*(lv+100)/3000		/2;
		case GROW_TECH:
			return lv*(lv+10)/10;
		case GROW_SPD:
			return lv;
		default:
			//if(lv<20){
				return lv*(lv+15)/30.0;
			//}else{
			//	return lv*(lv+15)/30.0 + (lv-20);
			//}
	}
}
void	CCharacterData::CalculateSP(void)
{
#define CHANGE(a,b,c)	{if(mMaxSP[(a)] > (b)){mMaxSP[(a)]=(mMaxSP[(a)]-(b))*(c)+(b);}}
	int t=mTech;
	mMaxSP[0]=t/1.0;
	mMaxSP[1]=t/4.0;
	mMaxSP[2]=t/19.0;
	mMaxSP[3]=t/44.0;

	mMaxSP[0]-=int(mMaxSP[1])+int(mMaxSP[2]);
	mMaxSP[1]-=int(mMaxSP[3]);

//Lv.1の成長率の偏り
	CHANGE(0,10,1.3333);
	CHANGE(0,25,0.75);
	CHANGE(0,40,0.5);

//Lv.2の成長率の偏り
	CHANGE(1,3,0.7);
	CHANGE(1,5,0.7);
	CHANGE(1,10,0.7);
	CHANGE(1,15,0.7);

//Lv.3の成長率の偏り
	CHANGE(2,4,0.8);
	CHANGE(2,7,0.8);
	CHANGE(2,10,0.5);

//Lv.4の成長率の偏り
	CHANGE(3,3,0.8);
	CHANGE(3,4,0.8);
	CHANGE(3,5,0.5);

//カンスト処理
	for(int i=0;i<MAX_CSKILL_LEVEL;i++){
		if(mMaxSP[i]>99)	mMaxSP[i]=99;
	}
}
int	CCharacterData::CalculateLP(void)
{
	mLP=mStm*(20+mLv)/20;
	if(mLP<=0)	mLP=1;
	return mLP;
}

int	CPlayerData::CalculateLP(void)
{
	mLP=mStm*(20+mLv)/20;
	if(mLP<=0)	mLP=1;
	return mLP;
}


//////////////////////////////////////////////////////////////////////////////////
CCharacterData::CCharacterData()
{
}
void	CCharacterData::Init(int id,const char* name,int lv,int stm,int atk,int def,int matk,int mdef,int spd,int tech,int size)
{
	mID=id;
	mName=name;
	mLostLP=0;
	for(int i=0;i<MAX_CSKILL_LEVEL;i++){
		mLostSP[i]=0;
	}

	mLv=lv;
	mExp=0;
	memset(mASkill,-1,sizeof(mASkill));
	memset(mCSkill,-1,sizeof(mCSkill));



	mStm	=	stm;
	mAtk	=	atk;
	mDef	=	def;
	mMagAtk	=	matk;
	mMagDef	=	mdef;
	mSpeed	=	spd;
	mTech	=	tech;
	mSize	=	size;
	
	CalculateSkill();
	CalculateLP();
	CalculateSP();
	CheckParamMax();
}

void	CCharacterData::CheckParamMax(void)
{
	if(mLP<0)			mLP=1;
	if(mLP>999999)		mLP=999999;

	if(mAtk>9999)		mAtk=9999;
	if(mDef>9999)		mDef=9999;
	if(mMagAtk>9999)	mMagAtk=9999;
	if(mMagDef>9999)	mMagDef=9999;
	if(mSpeed>999)		mSpeed=999;
}
void	CCharacterData::CalculateSkill(void)
{
	bool maghart=SearchASkill(310);
	bool sklhart=SearchASkill(311);
	char up;
	up=SearchASkillType(AS_LP_UP,true);
	mLP		= (up+100) * mLP/100;

	up=SearchASkillType(AS_ATK_UP,true);
	if(maghart)	up-=30;
	if(sklhart)	up+=20;
	mAtk	= (up+100) * mAtk/100;

	up=SearchASkillType(AS_DEF_UP,true);
	if(maghart)	up-=30;
	if(sklhart)	up+=20;
	mDef	= (up+100) * mDef/100;

	up=SearchASkillType(AS_MATK_UP,true);
	if(maghart)	up+=20;
	if(sklhart)	up-=30;
	mMagAtk = (up+100) * mMagAtk/100;

	up=SearchASkillType(AS_MDEF_UP,true);
	if(maghart)	up+=20;
	if(sklhart)	up-=30;
	mMagDef = (up+100) * mMagDef/100;

	up=SearchASkillType(AS_SPD_UP,true);
	mSpeed	= (up+100) * mSpeed/100;
	up=SearchASkillType(AS_TECH_UP,true);
	for(int i=0;i<MAX_CSKILL_LEVEL;i++){
		mMaxSP[i]	= (up+100) * mMaxSP[i]/100.0;
		if(mMaxSP[i]>99)	mMaxSP[i]=99;
	}

	if(SearchASkill(253))		mAtk+=mMagAtk;
	if(SearchASkill(254))		mDef+=mMagDef/2;
	if(SearchASkill(423)){	//ゴールデンパワー
		mAtk=9999;
		mLP=999999;
	}

	if(mAtk<=0)		mAtk=1;
	if(mDef<=0)		mDef=1;
	if(mMagAtk<=0)	mMagAtk=1;
	if(mMagDef<=0)	mMagDef=1;
	if(mSpeed<=0)	mSpeed=1;
	if(mTech<=0)	mTech=1;
}

void	CCharacterData::LostLP(int lost)
{
	int oldlp=mLostLP;
	mLostLP+=lost;
	if(mLostLP>=mLP){
		mLostLP=mLP-1;
		if(mLostLP<oldlp)	mLostLP=oldlp;
	}
}
void CCharacterData::HealLP(int heal)
{
	mLostLP-=heal;
	if(mLostLP<0)	mLostLP=0;
}

bool CCharacterData::LostSP(int lv,int pow)
{
#ifdef _FREE_SKILL_
	return true;
#endif
	if(SearchASkill(392))		return true;
	if(lv>=MAX_CSKILL_LEVEL)	return false;

	if(GetSP(lv)>=pow){
		mLostSP[lv]+=pow;
		return true;
	}

	int max=GetMaxSP(lv);
	if(mLostSP[lv]<max){
		mLostSP[lv]=max;
	}
	return false;
}
void	CCharacterData::HealSP(int lv,int heal)
{
	if(lv>=MAX_CSKILL_LEVEL)	return;
	mLostSP[lv]-=heal;
	if(mLostSP[lv]<0)	mLostSP[lv]=0;
}

int CCharacterData::SetCSkill(int skill)
{
	for(int i=0;i<MAX_CSKILL;i++){
		if(mCSkill[i]<0){
			mCSkill[i]=skill;
			return i;
		}else if (mCSkill[i]==skill){
			return i;
		}
	}
	return -1;
}
int CCharacterData::SetASkill(int skill)
{
	for(int i=0;i<MAX_ASKILL;i++){
		if(mASkill[i]<0){
			mASkill[i]=skill;
			return i;
		}else if (mASkill[i]==skill){
			return i;
		}
	}
	return -1;
}
int CCharacterData::ClearASkill(int skill)
{
	for(int i=0;i<MAX_ASKILL;i++){
		if (mASkill[i]==skill){
			mASkill[i]=-1;
			return i;
		}
	}
	return -1;
}


void CCharacterData::Help(int param)const
{
}
int	CCharacterData::GetCSkillAll(int type,int* skill,bool* sp,int *param)
{
	if(type==CS_SKILL){
		type=CS_SKILL | CS_SWORD | CS_SPEAR | CS_AXE | CS_BOW ;
	}
	return GetCSkillAllPrivate(type,skill,sp,param);
}
int	CCharacterData::GetCSkillAllPrivate(int type,int* skill,bool* sp,int *param){

	int skillno=0;
	for(int i=0;i<MAX_CSKILL;i++){
		if(mCSkill[i]>=0 && (type & (ComSkillData[mCSkill[i]].mType))){
			if(skill)	skill[skillno]=mCSkill[i];
			if(param)	param[skillno]=GetSkillParam(i);
			if(sp){
				int slv=ComSkillData[mCSkill[i]].mSP;
				if(slv<=0){
					sp[skillno]=true;
				}else{
					sp[skillno]=(GetSP(slv-1)>0);
				}
			}
			skillno++;
		}
	}
	return skillno;
}
int	CCharacterData::GetASkillAll(int* skill)
{
	int skillno=0;
	for(int i=0;i<MAX_ASKILL;i++){
		if(mASkill[i]>=0){
			if(skill)	skill[skillno]=mASkill[i];
			skillno++;
		}
	}
	return skillno;
}
bool	CCharacterData::SearchASkill(int no)
{
	for(int i=0;i<MAX_ASKILL;i++){
		if(mASkill[i]==no){
			ASkillOpen(i);
			return true;
		}
	}
	return false;
}
int	CCharacterData::SearchASkillType(int type,bool add)
{
	int total=0;
	int max=0;
	for(int i=0;i<MAX_ASKILL;i++){
		if(mASkill[i]>=0){
			if((SkillData[mASkill[i]].mParam & 0xffff0000) == type){
				ASkillOpen(i);
				int p=SkillData[mASkill[i]].mParam & 0x0000ffff;
				total+=p;
				if(p>max)	max=p;
			}
		}
	}

	if(add)	return total;
	return max;
}

///////////////////////////////////////////////////////////////////////////////////

CPlayerData::CPlayerData()
{
}


double CPlayerData::CulcExp(double base,int plv,int elv,bool tresure)
{
	if(plv>=MAX_LV)	return 0;
	double ratio=1;
	int lv=elv-plv;

	for(int i=0;i<LV_UP_EXP_RATIO_LANK;i++){
		if(lv<LV_UP_EXP_RATIO_LV[i]){
			ratio=ratio*pow(LV_UP_EXP_RATIO[i],lv);
			break;
		}else{
			ratio=ratio*pow(LV_UP_EXP_RATIO[i],LV_UP_EXP_RATIO_LV[i]);
			lv-=LV_UP_EXP_RATIO_LV[i];
		}
	}

	double ret=base*ratio;

	if(!tresure){
		ret*=pow(EXP_DOWN_RATIO,plv);
		ret+=(elv-plv+60)*base/512;
		if(plv<10){	//レベル１０までの経験値ブースト
			ret=ret*(10-plv+3)/3;
		}
	}
#ifdef _GET_EXP_
		ret*=_GET_EXP_;
#endif
	return ret;
}
void CPlayerData::Init(int no)
{
	mID=no;
	mName=CharacterBaseData[mID].GetName();
	mLostLP=0;
	mSize=1;

	mLv=CharacterBaseData[mID].mStartLv;
	mExp=0;
	memcpy(mSkill,StartSkill[mID],sizeof(mSkill));
	memset(mFreeSpace,-1,sizeof(mFreeSpace));
#ifdef _LEVEL_LOCK_
	mLv=_LEVEL_LOCK_;
#endif

#ifdef _AUTO_LEARN_
	for(int i=0;i<12;i++){
		mSkill[i][0]=1;
		mSkill[i][1]=2;
		mSkill[i][2]=3;
		mSkill[i][3]=4;
		mSkill[i][4]=5;
		mSkill[i][5]=8;
	}
#endif
	mWeapon=-1;
	mArmor=-1;
	for(int i=0;i<MAX_ACCESSORY+1;i++){
		mAccessory[i]=-1;
	}

//	Calculate();


	bool item=true;
	for(int i=0;i<MAX_ITEM;i++){
		if(item){
			SetItem(i,StartItem[mID][i]);
			if(StartItem[mID][i]<0){
				mItem[i]=NO_ITEM;
				item=false;
			}
		}else{
			mItem[i]=NO_ITEM;
		}
	}

	for(int i=MAX_ITEM-1;i>=0;i--){
		Equip(i);
	}
	for(int i=0;i<MAX_CSKILL_LEVEL;i++){
		mLostSP[i]=0;
	}
	Calculate();

}
void	CPlayerData::AllHeal(void)
{
	mLostLP = 0;
	for(int i=0;i<MAX_CSKILL_LEVEL;i++){
		mLostSP[i]=0;
	}

	for(int i=0;i<mMaxItem;i++){
		int item = mItem[i].item - ID_UI;
		if(item>=0 && item<MAX_SEED_NUMBER){
			mItem[i].param=UseItemData[item].mUseCount;
		}
	}
}
bool	CPlayerData::SetExp(double &exp,double get)
{
	bool ret=false;
	mExp+=get;
	exp-=get;
	while(mExp>=LV_UP_EXP){
		mExp-=LV_UP_EXP;
		ret=true;
		mLv++;
		if(mLv>MAX_LV)	break;
		mExp=mExp/LV_UP_EXP_RATIO[0];
		exp=exp/LV_UP_EXP_RATIO[0];
		Calculate();
	}

	if(mLv>=MAX_LV){
		mLv=MAX_LV;
		mExp=0;
		exp=0;
	}
	return ret;
}

void CPlayerData::SetSkill(int skill)
{
	if(skill<0)	return;
	if(SkillData[skill].mType==SKILL_STATUS){
		const int &param=SkillData[skill].mParam;
		int	up=param&0xffff;
		if(param & 0x010000)	mBaseStm+=up;
		if(param & 0x020000)	mBaseTech+=up;
		if(param & 0x040000)	mBaseAtk+=up;
		if(param & 0x080000)	mBaseDef+=up;
		if(param & 0x100000)	mBaseMagAtk+=up;
		if(param & 0x200000)	mBaseMagDef+=up;
		if(param & 0x400000)	mBaseSpeed+=up;
		if(param & 0x800000)	mMaxUseItem+=up;
		if(param & 0x1000000)	mMaxAccessory+=up;
	}else if(SkillData[skill].mType==SKILL_COMMAND){
		SetCSkill(SkillData[skill].mParam);
	}else{
		SetASkill(skill);
	}
}
void CPlayerData::Calculate(void)
{
//初期化
	int oldacc = GetAccessoryMax();

	memset(mASkill,-1,sizeof(mASkill));
	memset(mCSkill,-1,sizeof(mCSkill));

	mBaseStm	=	CharacterBaseData[mID].mLP;
	mBaseAtk	=	CharacterBaseData[mID].mAtk;
	mBaseDef	=	CharacterBaseData[mID].mDef;
	mBaseMagAtk	=	CharacterBaseData[mID].mMagAtk;
	mBaseMagDef	=	CharacterBaseData[mID].mMagDef;
	mBaseSpeed	=	CharacterBaseData[mID].mSpeed;
	mBaseTech	=	CharacterBaseData[mID].mTech;
	mMaxUseItem	=	MIN_USE_ITEM;
	mMaxAccessory = MIN_ACCESSORY;

	mLP		=	0;
	mAtk	=	0;
	mDef	=	0;
	mMagAtk	=	0;
	mMagDef	=	0;
	mSpeed	=	0;
	mTech	=	0;


//スキルのセットアップ
	for(int m=0;m<MAX_MONTH;m++){
		for(int i=0;i<MAX_LEARN_MONTH;i++){
			if(mSkill[m][i]>0){
				const int &skill=CharacterSkill[mID][m][mSkill[m][i]-1];
				SetSkill(skill);
			}
		}
	}

	if(mMaxUseItem>MAX_USE_ITEM)			mMaxUseItem=MAX_USE_ITEM ;
	if(mMaxAccessory>MAX_ACCESSORY_REAL)	mMaxAccessory=MAX_ACCESSORY_REAL ;
	mMaxItem	=	mMaxUseItem+mMaxAccessory+WEAPON_SPACE+ARMOR_SPACE;

	CalculateEquipment(mWeapon);
	CalculateEquipment(mArmor);
	//フリースペースは武器のあと、アクセサリの前
	int freespace = SearchASkillType(AS_FREE_SPACE,true);
	for(int i=0;i<freespace;i++){
		SetSkill(mFreeSpace[i]);
	}
	//装備不能アクセサリの排除
	int acc= GetAccessoryMax();
	if( oldacc > acc){
		for(int i=acc;i<MAX_ACCESSORY+1;i++){
			mAccessory[i]=-1;
		}
	}
	for(int i=0;i<MAX_ACCESSORY+1;i++){
		CalculateEquipment(mAccessory[i]);
	}
	bool at=false;
	int weptype=TYPE_WEAPON;
	if(mWeapon>=0)		weptype=EquipmentData[mItem[mWeapon].item].mType;

	mSecondWeapon=-1;
	if(weptype==TYPE_SWORD){
		if(SearchASkill(289)){
			int maxitem=GetItemMax();
			for(int i=0;i<maxitem;i++){
				if(mWeapon==i)	continue;
				int item=GetItem(i) .item;
				if(AllItem[item]->mType ==TYPE_SWORD){
					mSecondWeapon=i;
					CalculateEquipment(i);
					break;
				}
			}
		}
	}

//パラメータ算出
	mStm	=	mBaseStm;

	int templp = mLP;
	CalculateLP();
	mBaseLP = mLP;
	mLP+=templp ;
	if(SearchASkillType(AS_BANSEI,true)>=28){
		const double bansei=4.5;
		mLP			=int(mBaseLP	*3);
		mBaseAtk	=int(mBaseAtk	*bansei);
		mBaseDef	=int(mBaseDef	*bansei);
		mBaseMagAtk	=int(mBaseMagAtk*bansei);
		mBaseMagDef	=int(mBaseMagDef*bansei);
		//mSpeed	=int(mBaseSpeed	*bansei);
		mBaseTech	=int(mBaseTech	*3);
	}
	mAtk	+=	mBaseAtk;
	mDef	+=	mBaseDef;
	mMagAtk	+=	mBaseMagAtk;
	mMagDef	+=	mBaseMagDef;
	mSpeed	+=	mBaseSpeed;
	mTech	+=	mBaseTech;


	if((weptype & TYPE_SWORD) == TYPE_SWORD){SetCSkill(1);	at=true;}
	if((weptype & TYPE_SPEAR) == TYPE_SPEAR){SetCSkill(2);	at=true;}
	if((weptype & TYPE_AXE) == TYPE_AXE)	{SetCSkill(3);	at=true;}
	if((weptype & TYPE_BOW) == TYPE_BOW)	{SetCSkill(4);	at=true;}
	if((weptype & TYPE_LASER) == TYPE_LASER){SetCSkill(5);	at=true;}
	if(!at)									{SetCSkill(0);}


//スキルのソート
	bool swap=true;
	while(swap){
		swap=false;
		for(int i=1;i<MAX_CSKILL;i++){
			if(mCSkill[i]<0)	break;
			//if(mCSkill[i]<mCSkill[i-1]){
			if(ComSkillData[mCSkill[i]].GetSortNo()<ComSkillData[mCSkill[i-1]].GetSortNo()){
				int cs=mCSkill[i];
				mCSkill[i]=mCSkill[i-1];
				mCSkill[i-1]=cs;
				swap=true;
			}
		}
	}

	int	as[MAX_ASKILL];
	int asno=0;
	memset(as,-1,sizeof(as));
	for(int i=0;i<MAX_ASKILL;i++){
		if(	mASkill[i]<0)	break; 
		if( SkillData[mASkill[i]].mType == SKILL_AUTO){
			as[asno]=mASkill[i];
			asno++;
		}
	}
	swap=true;
	while(swap){
		swap=false;
		for(int i=1;i<asno;i++){
			if(SkillData[as[i]].mParam<SkillData[as[i-1]].mParam){
				int s=as[i];
				as[i]=as[i-1];
				as[i-1]=s;
				swap=true;
			}
		}
	}

	int fsno=0;
	for(int i=0;i<MAX_ASKILL;i++){
		if(	mASkill[i]<0)	break; 
		if( SkillData[mASkill[i]].mType != SKILL_AUTO){
			as[asno+fsno]=mASkill[i];
			fsno++;
		}
	}
	
	swap=true;
	while(swap){
		swap=false;
		for(int i=asno+1;i<fsno+asno;i++){
			if(SkillData[as[i]].mParam<SkillData[as[i-1]].mParam){
				int s=as[i];
				as[i]=as[i-1];
				as[i-1]=s;
				swap=true;
			}
		}
	}

	memcpy(mASkill,as,sizeof(mASkill));
	
//最終調整
	CalculateSP();
	CalculateSkill();

	if(mLP>9999)			mLP=9999;
	if(mMaxItem>MAX_ITEM)	mMaxItem=MAX_ITEM;
	CheckParamMax();
	for(int i=mMaxItem;i<MAX_ITEM;i++){
		if(mItem[i]!=NO_ITEM){
			for(int j=0;j<mMaxItem;j++){
				if(mItem[j]==NO_ITEM){
					mItem[j]=mItem[i];
					mItem[i]=NO_ITEM;
					break;
				}
			}

			if(mItem[i]!=NO_ITEM){
				Flag.SetCloset(mItem[i]);
				mItem[i]=NO_ITEM;
				TakeOff(i);
			}
		}
	}

#ifdef _ALL_SKILL_
	int add=0;
	if(mID%2==1)	add=MAX_CSKILL;
	for(int i=0;i<MAX_CSKILL;i++){
		mCSkill[i]=i+add;
	}
#endif
#ifdef _ALL_SKILL_NO_
	for(int i=0;i<MAX_CSKILL;i++){
		mCSkill[i]=i+_ALL_SKILL_NO_;
	}
#endif

}
void	CPlayerData::CalculateSkill(void)
{
	CCharacterData::CalculateSkill();
	if(SearchASkill(303)){
		mAtk=1;
		if(mWeapon>=0){
			const CEquipmentData & edata=EquipmentData[mItem[mWeapon].item];
			if(edata.mType & TYPE_EQUIPMENT){
				mAtk	=edata.mAtk;
			}
		}
	}
	if(SearchASkill(328)){
		mMagAtk=1;
		if(mWeapon>=0){
			const CEquipmentData & edata=EquipmentData[mItem[mWeapon].item];
			if(edata.mType & TYPE_EQUIPMENT){
				mMagAtk	=edata.mMagAtk;
			}
		}
	}
	if(SearchASkill(309)){
		if(mAtk>mMagAtk)	mMagAtk=mAtk;
		else				mAtk=mMagAtk;
	}
}
void	CPlayerData::GetSkillSpace(int sspace[MAX_MONTH])
{
	static const int lv[MAX_LV]={
		0,	0,	1,	0,	1,	2,	0,	1,	2,	3,
		1,	2,	3,	4,	0,	2,	3,	4,	5,	1,	
		3,	4,	5,	6,	2,	7,	4,	5,	6,	3,	
		8,	7,	6,	5,	4,	9,	8,	7,	6,	10,
		9,	8,	5,	7,	11,	10,	9,	8,	6,	11,
		7,	10,	9,	11,	8,	10,	9,	11,	10,	11
	};

	memset(sspace,0,sizeof(int)*MAX_MONTH);
	if(mLv>MAX_LV)	mLv=MAX_LV;
	for(int i=0;i<mLv;i++){
		sspace[lv[i]]++;
	}
}
int	CPlayerData::GetLearnSkillNo(int month)
{
	int i;
	for(i=0;i<MAX_LEARN_MONTH-1;i++){
		if(mSkill[month][i]<=0)	return i;
	}
	return i;
}
void CPlayerData::GetLearnSkill(int month,int* list)
{
	memcpy(list,mSkill[month],sizeof(int)*MAX_LEARN_MONTH);
}

bool CPlayerData::CheckLearnSkill(int skill)
{
	int sspace[MAX_MONTH];
	GetSkillSpace(sspace);

	for(int month=0;month<MAX_MONTH;month++){
		for(int i=0;i<sspace[month];i++){
			int no=mSkill[month][i]-1;
			if(no<0)	continue;
			if(CharacterSkill[mID][month][no]==skill)	return true;
		}
		if(mSkill[month][MAX_LEARN_MONTH-1]>0){
			if(CharacterSkill[mID][month][MAX_SKILL_MONTH-1]==skill)
				return true;
		}
	}
	return false;
}
bool CPlayerData::CheckLearnSkill(int month,int no)
{
	int sspace[MAX_MONTH];
	GetSkillSpace(sspace);
	no++;
	if(sspace[month]<=0)	return false;

	for(int i=0;i<sspace[month];i++){
		if(mSkill[month][i]==no)				return true;
	}
	if(mSkill[month][MAX_LEARN_MONTH-1]==no)	return true;

	return false;
}
int	CPlayerData::LearnSkill(int month,int no)
{
	int sspace[MAX_MONTH];
	GetSkillSpace(sspace);
	no++;
	if(no==MAX_SKILL_MONTH){	//スペシャル専用例外処理
		if(sspace[month]<=0)						return -1;
		if(mSkill[month][MAX_LEARN_MONTH-1]==no)	return -2;
		mSkill[month][MAX_LEARN_MONTH-1]=no;
		Calculate();
		return MAX_LEARN_MONTH-1;
	}
	for(int i=0;i<sspace[month];i++){
		if(mSkill[month][i]==no)	return -2;
		if(mSkill[month][i]<=0){
			mSkill[month][i]=no;
			Calculate();
			return i;
		}
	}
	return -1;
}
bool	CPlayerData::ClearAllLearnSkill()
{
	return ClearLearnSkill(0,MAX_MONTH-1,true);
}
bool	CPlayerData::ClearAllStarSkill()
{
	return ClearLearnSkill(0,MAX_MONTH-1,false);
}
bool	CPlayerData::ClearMonthLearnSkill(int month)
{
	return ClearLearnSkill(month,month,true);
}
bool	CPlayerData::ClearLearnSkill(int mmin,int mmax,bool normal)
{
	bool ret=false;
	for(int month=mmin;month<=mmax;month++){
		if(mSkill[month][MAX_LEARN_MONTH-1] && CharacterSkill[mID][month][MAX_SKILL_MONTH-1] == 315){
			ret=true;
		}else{
			mSkill[month][MAX_LEARN_MONTH-1]=0;
		}
		if(normal){
			for(int i=0;i<MAX_LEARN_MONTH-1;i++){
				mSkill[month][i]=0;
			}
		}
	}
	Calculate();
	return ret;
}

int	CPlayerData::GetEquipment(int type)
{
	int start;
	int itemtype;

	int repeat = type & 0x00ff;

	switch(type & 0xff00){
		case CHAREQ_WEAPON:
			itemtype=TYPE_WEAPON;
			start=mWeapon;
			break;
		case CHAREQ_ARMOR:
			itemtype=TYPE_ARMOR;
			start=mArmor;
			break;
		case CHAREQ_ACCESSORY:
			if(repeat<GetAccessoryMax()){
				return mAccessory[repeat];
			}else{
				return -1;
			}
			break;
		default:
			return -1;
	}
	if(type & CHAREQ_FROM_START){
		start=0;
	}


	if(start<0){
		start=0;
		repeat--;
		if(repeat<0)	return -1;
	}
	int i=start;

	do{
		if((AllItem[mItem[i].item]->mType&0xffff0000) == itemtype){
			if(repeat<=0)	return i;
			repeat--;
		}
		i++;
		if(i>=mMaxItem)	i=0;
	}while(start != i);

	return -1;
}
bool CPlayerData::CheckEquiped(int no)
{
	if(mWeapon==no)	return true;
	if(mArmor==no)	return true;
	for(int i=0;i<GetAccessoryMax();i++){
		if(mAccessory[i]==no)	return true;
	}
	return false;
}
bool CPlayerData::Equip(int no,bool force)
{
	bool ret=false;
	const CItemData* item = AllItem[mItem[no].item];
	if((item->mType&0xffff0000) == TYPE_WEAPON){
		mWeapon=no;
		ret=true;
	}
	if((item->mType&0xffff0000) == TYPE_ARMOR){
		mArmor=no;
		ret=true;
	}
	if((item->mType&0xffff0000) == TYPE_ACCESSORY){
		int max=GetAccessoryMax();
		if(force)	max++;
		for(int i=0;i<max;i++){
			if(mAccessory[i]>=0){
				int item=GetItem(mAccessory[i]).item;
				int acctype = (AllItem[item]->mType&0xffff);
				if(acctype == (AllItem[mItem[no].item]->mType&0xffff)){
					mAccessory[i]=no;
					ret=true;
					goto equiped ;
				}
			}
		}
		for(int i=0;i<max;i++){
			if(mAccessory[i]<0){
				mAccessory[i]=no;
				ret=true;
				break;
			}
		}
	}

equiped:

	if(ret){
		Calculate();
	}
	return ret;
}
bool	CPlayerData::TakeOff(int no)
{
	bool ret=false;
	switch(no){
		case CHAREQ_WEAPON:
			mWeapon=-1;	ret=true;	break;
		default:
			if(no==mWeapon){
				mWeapon=-1;
				ret=true;
			}
			if(no==mSecondWeapon){
				mSecondWeapon=-1;
				ret=true;
			}
			if(no==mArmor){
				mArmor=-1;
				ret=true;
			}
			int i;
			for(i=0;i<MAX_ACCESSORY;i++){
				if(mAccessory[i]==no){
					mAccessory[i]=-1;
					ret=true;
				}
			}
			break;
	}

	if(ret){
		Calculate();
	}
	return ret;
}

int	CPlayerData::GetCSkillAll(int type,int* skill,bool *sp,int *param)
{
	
#ifdef _ALL_SKILL_
	if(type==CS_SKILL){
		type=CS_SKILL | CS_SWORD | CS_SPEAR | CS_AXE | CS_BOW ;
	}
#else
	if(type==CS_SKILL){
		if(mWeapon>=0){
			int wt=AllItem[mItem[mWeapon].item]->mType;
			type=type | (wt&0xffff) ;
			if( (type&CS_SWORD) | (type&CS_SPEAR) | (type&CS_AXE) ){
				type= type | SearchASkillType(AS_WEAPON_MASTER,true);
			}
		}
	}
#endif
	return GetCSkillAllPrivate(type,skill,sp,param);
}
int	CPlayerData::GetItemCurrent()
{
	int item=0;
	for(int i=GetEquipMax();i<mMaxItem;i++){
		if(mItem[i]!=NO_ITEM)	item++;
	}
	return item;
}
int	CPlayerData::GetUseItemCurrent()
{
	int item=0;
	for(int i=0;i<mMaxItem;i++){
		if(mItem[i]!=NO_ITEM)	item++;
	}
	return item;
}
int	CPlayerData::GetAccessoryCurrent()
{
	int acc=0;
	for(int i=0;i<mMaxAccessory;i++){
		if(mAccessory[i]>=0)	acc++;
	}
	return acc;
}

int	CPlayerData::SearchItem(int item)
{
	for(int i=0;i<mMaxItem;i++){
		if(mItem[i] == item){
			return i;
		}
	}
	return -1;
}

int	CPlayerData::SetItem(int item)
{
	CarryItem ci=item;
	item -= ID_UI;
	if(item>=0 && item<MAX_SEED_NUMBER){
		ci.param=UseItemData[item].mUseCount;
	}
	return SetItem(ci);
}
int	CPlayerData::SetItem(CarryItem item)
{
	int &id=item.item;
//まとめられるアイテムに対する処理
	if(id > ID_UI && (UseItemData[id-ID_UI].mType&TYPE_COLLECT)){
		int itemno=SearchItem(id);
		if(itemno>=0){
			if(mItem[itemno].param<=0)	mItem[itemno].param=1;
			if(item.param<=0)			item.param=1;
			mItem[itemno].param+=item.param;
			if(mItem[itemno].param>99)	mItem[itemno].param=99;
			return itemno;
		}
	}
//武器・防具に対する処理
	if(id<ID_ACC){
		int min,max;
		if(id<ID_AMR){
			min=0;				max=WEAPON_SPACE;
		}else{
			min=WEAPON_SPACE;	max=WEAPON_SPACE+ARMOR_SPACE;
		}
		//仮置き
		/*for(int i=min;i<max;i++){
			if(mItem[i] == NO_ITEM){
				mItem[i]=item;
				Equip(i);
				return i;
			}
		}*/
		
		Flag.SetCloset(mItem[max-1],this);
		for(int i=max-1;i>min;i--){
			mItem[i] =mItem[i-1];
		}
		mItem[min]=item;
		Equip(min);
		return min;
		
//アクセサリに対する処理
	}else if(id<MAX_EQUIPMENT_NUMBER){
		//すでに同じジャンルのものを装備しているかどうかの確認
		for(int i=WEAPON_SPACE+ARMOR_SPACE;i<GetEquipMax();i++){
			if(mItem[i] == NO_ITEM)	continue;
			if(AllItem[mItem[i].item]->mType == AllItem[id]->mType){
				//仮置き
				//return -1;
				Flag.SetCloset(mItem[i],this);
				mItem[i]=item;
				Equip(i);
				return i;
			}
		}
		
		for(int i=WEAPON_SPACE+ARMOR_SPACE;i<GetEquipMax();i++){
			if(mItem[i] == NO_ITEM){
				mItem[i]=item;
				Equip(i);
				return i;
			}
		}
//それ以外のアイテムに対する処理
	}else{
		for(int i=GetEquipMax();i<mMaxItem;i++){
			if(mItem[i] == NO_ITEM){
				mItem[i]=item;
				return i;
			}
		}
	}
	return -1;
}

void	CPlayerData::SetItem(int no,int item)	
{
	if(no<0||mMaxItem<=no)	return;
	mItem[no].item=item;
	mItem[no].param=0;
}
void	CPlayerData::SetItem(int no,CarryItem item)	
{	
	if(no<0||mMaxItem<=no)	return;
	mItem[no]=item;
}
bool	CPlayerData::SpendUseItem(int no)
{
	//TODO あとまわし
	int item=mItem[no].item-ID_UI;

	if(item>=0 && item<MAX_SEED_NUMBER){
		if(UseItemData[item].mType & TYPE_SPEND){
			return mItem[no].Spend();
		}
	}
	return true;
}
void CPlayerData::DeleteNoEquipedItem(void)
{
	bool equiped[MAX_ITEM];
	memset(equiped,0,sizeof(equiped));

	if(mWeapon>=0)			equiped[mWeapon]=true;
	if(mSecondWeapon>=0)	equiped[mSecondWeapon]=true;
	if(mArmor>=0)			equiped[mArmor]=true;
	for(int i=0;i<GetAccessoryMax();i++){
		if(mAccessory[i]>=0)	equiped[mAccessory[i]]=true;
	}

	for(int i=0;i<MAX_ITEM;i++){
		if(!equiped[i]){
			mItem[i]=NO_ITEM;
		}
	}
}
void CPlayerData::CollectItem(int no)
{
	if(no<0||mMaxItem<=no)	return;
	int item=mItem[no].item;
	if(! ((item > ID_UI) && (UseItemData[item-ID_UI].mType&TYPE_COLLECT))){
		return;
	}
	if(mItem[no].param<1)	mItem[no].param=1;

	for(int i=0;i<mMaxItem;i++){
		if(i==no)	continue;
		if(mItem[i].item == item){
			if(mItem[i].param<1)	mItem[i].param=1;
			mItem[no].param += mItem[i].param;
			mItem[i] = NO_ITEM ;
		}
	}
}
void CPlayerData::OrderItem()
{
	for(int i=0;i<mMaxItem;i++){
		CollectItem(i);
	}

	CarryItem item[MAX_ITEM];
	memcpy(item , mItem , sizeof(CarryItem) * (MAX_ITEM));


	int wepno=0;
	CarryItem weapon[WEAPON_SPACE];
	weapon[wepno]=GetItem(mWeapon);
	if(weapon[wepno].item<ID_AMR){
		item[mWeapon]=NO_ITEM;
		wepno++;
	}

	weapon[wepno]=GetItem(mSecondWeapon);
	if(weapon[wepno].item<ID_AMR){
		item[mSecondWeapon]=NO_ITEM;
		wepno++;
	}

	for(int i=0;i<WEAPON_SPACE;i++){
		if(item[i].item<ID_AMR){
			weapon[wepno]=item[i];
			item[i]=NO_ITEM;
			wepno++;
			if(wepno>=WEAPON_SPACE)	break;
		}
	}
	for(int i=wepno;i<WEAPON_SPACE;i++){
		weapon[i]=NO_ITEM;
	}

	CarryItem armer=GetItem(mArmor);
	if(armer!=NO_ITEM)	item[mArmor]=NO_ITEM;

	CarryItem acc[MAX_ACCESSORY];
	for(int i=0;i<GetAccessoryMax();i++){
		acc[i]=GetItem(mAccessory[i]);
		if(acc[i]!=NO_ITEM)	item[mAccessory[i]]=NO_ITEM;
	}


	for(int i=0;i<mMaxItem;i++){
		if(item[i].item < MAX_EQUIPMENT_NUMBER){
			Flag.SetCloset(item[i]);
			item[i]=NO_ITEM;
		}
	}
	int ui_start;
	for(int i=0;i<mMaxItem;i++){
		ui_start=i;
		if(item[i].item==NO_ITEM)	continue;
		break;
	}

/*	for(int i=0;i<mMaxItem;){
		if(item[i].item==NO_ITEM){
			bool noitem=true;
			for(int j=i;j<mMaxItem-1;j++){
				item[j]=item[j+1];
				if(item[j]!=NO_ITEM)	noitem=false;
			}
			item[mMaxItem-1]=NO_ITEM;
			if(noitem)	break;
		}else{
			i++;
		}
	}
*/
	for(int i=0;i<MAX_ITEM;i++){
		mItem[i]=NO_ITEM;
	}
	mWeapon=0;
	memcpy(mItem , weapon , sizeof(CarryItem) * (WEAPON_SPACE));

	if(armer!=NO_ITEM){
		mItem[WEAPON_SPACE]=armer;
		mArmor=WEAPON_SPACE;
	}

	int start=ARMOR_SPACE+WEAPON_SPACE;
	for(int i=0;i<GetAccessoryMax();i++){
		mItem[start+i]=acc[i];
		if(acc[i]!=NO_ITEM){
			mAccessory[i]=start+i;
		}
	}
	start=GetEquipMax();
	int cp=mMaxItem-start;
	if(mMaxItem-ui_start < cp) cp=mMaxItem-ui_start;
	memcpy(&mItem[start] , &item[ui_start] , sizeof(CarryItem) * (cp));

	Calculate();
}

void CPlayerData::Help(int param)const
{
}
void CPlayerData::CalculateEquipment(int no)
{
	if(no<0)								return;
	if(mItem[no].item>MAX_EQUIPMENT_NUMBER)	return;

	const CEquipmentData & edata=EquipmentData[mItem[no].item];

	if(!(edata.mType & TYPE_EQUIPMENT))	return;
	mLP		+=edata.mLP;
	mAtk	+=edata.mAtk;
	mDef	+=edata.mDef;
	mMagAtk	+=edata.mMagAtk;
	mMagDef	+=edata.mMagDef;
	int spd=edata.mSpeed;
	if(spd<0){
		if(SearchASkill(369)){
			spd/=8;
		}
	}
	mSpeed	+=spd;

	int i;
	for(i=0;i<edata.mSkillNo;i++){
		const int &skill=edata.mSkill[i];

		if(SkillData[skill].mType==SKILL_COMMAND){
			SetCSkill(SkillData[skill].mParam);
		}else if(SkillData[skill].mType==SKILL_AUTO || SkillData[skill].mType==SKILL_FIELD ){
			SetASkill(skill);
		}
	}
}

void	 CPlayerData::Save(FILE* pFile)
{
	_putw(mLv,pFile);		//レベル
	_putw((int)mExp,pFile);	//経験値
	_putw(mLostLP,pFile);	//現在ＬＰ

	int i;
	for(i=0;i<MAX_CSKILL_LEVEL;i++){
		_putw(mLostSP[i],pFile);	//現在ＳＰ
	}

	fwrite(mItem,1,sizeof(mItem),pFile);

	_putw(mWeapon,pFile);
	_putw(mArmor,pFile);
	for(i=0;i<MAX_ACCESSORY;i++){
		_putw(mAccessory[i],pFile);
	}							//装備情報

	for(i=0;i<MAX_MONTH;i++){
		for(int j=0;j<MAX_LEARN_MONTH;j++){
			_putw(mSkill[i][j],pFile);
		}
	}							//習得済みスキル
	for(i=0;i<MAX_FREE_SPACE;i++){
		_putw(mFreeSpace[i],pFile);
	}							//フリースペース
}

void	 CPlayerData::Load_8(FILE* pFile)
{
	mLv=_getw(pFile);
	mExp=_getw(pFile);
	mLostLP=_getw(pFile);			//現在ＬＰ

	int i;
	for(i=0;i<MAX_CSKILL_LEVEL;i++){
		mLostSP[i]=_getw(pFile);
	}
	fread(mItem,1,sizeof(mItem),pFile);

	mWeapon=_getw(pFile);
	mArmor=_getw(pFile);
	for(i=0;i<MAX_ACCESSORY;i++){
		mAccessory[i]=_getw(pFile);
	}							//装備情報

	for(i=0;i<MAX_MONTH;i++){
		for(int j=0;j<MAX_LEARN_MONTH;j++){
			mSkill[i][j]=_getw(pFile);
		}
	}							//習得済みスキル
	for(i=0;i<MAX_FREE_SPACE;i++){
		mFreeSpace[i]=_getw(pFile);
	}							//フリースペース

	Calculate();
}
void	 CPlayerData::Load_5(FILE* pFile)
{
	mLv=_getw(pFile);
	mExp=_getw(pFile);
	mLostLP=_getw(pFile);			//現在ＬＰ

	int i;
	for(i=0;i<4;i++){
		mLostSP[i]=_getw(pFile);
	}
	fread(mItem,1,sizeof(mItem),pFile);

	mWeapon=_getw(pFile);
	mArmor=_getw(pFile);
	for(i=0;i<8;i++){
		mAccessory[i]=_getw(pFile);
	}							//装備情報

	for(i=0;i<MAX_MONTH;i++){
		for(int j=0;j<MAX_LEARN_MONTH;j++){
			mSkill[i][j]=_getw(pFile);
		}
	}							//習得済みスキル
	for(i=0;i<2;i++){
		mFreeSpace[i]=_getw(pFile);
	}							//フリースペース

	Calculate();
}
void	 CPlayerData::Load_4(FILE* pFile)
{
	mLv=_getw(pFile);
	mExp=_getw(pFile);
	mLostLP=_getw(pFile);			//現在ＬＰ

	int i;
	for(i=0;i<MAX_CSKILL_LEVEL;i++){
		mLostSP[i]=_getw(pFile);
	}
	fread(mItem,1,sizeof(mItem),pFile);

	mWeapon=_getw(pFile);
	mArmor=_getw(pFile);
	for(i=0;i<MAX_ACCESSORY;i++){
		mAccessory[i]=_getw(pFile);
	}							//装備情報

	for(i=0;i<MAX_MONTH;i++){
		for(int j=0;j<5;j++){
			mSkill[i][j]=_getw(pFile);
		}
	}							//習得済みスキル
	for(i=0;i<2;i++){
		mFreeSpace[i]=_getw(pFile);
	}							//フリースペース

	Calculate();
}
void	 CPlayerData::Load_2(FILE* pFile)
{
	mLv=_getw(pFile);
	mExp=_getw(pFile);
	mLostLP=_getw(pFile);			//現在ＬＰ

	int i;
	for(i=0;i<MAX_CSKILL_LEVEL;i++){
		mLostSP[i]=_getw(pFile);
	}

	for(i=0;i<MAX_ITEM;i++){
		mItem[i].item=_getw(pFile);
		mItem[i].param=0;
	}

	mWeapon=_getw(pFile);
	mArmor=_getw(pFile);
	for(i=0;i<4;i++){
		mAccessory[i]=_getw(pFile);
	}							//装備情報

	for(i=0;i<MAX_MONTH;i++){
		for(int j=0;j<5;j++){
			mSkill[i][j]=_getw(pFile);
		}
	}							//習得済みスキル
	for(i=0;i<2;i++){
		mFreeSpace[i]=_getw(pFile);
	}							//フリースペース

	Calculate();
}
void	 CPlayerData::Load_1(FILE* pFile)
{
	mLv=_getw(pFile);
	mExp=_getw(pFile);
	mLostLP=_getw(pFile);			//現在ＬＰ

	int i;
	for(i=0;i<MAX_CSKILL_LEVEL;i++){
		mLostSP[i]=_getw(pFile);
	}

	for(i=0;i<MAX_ITEM;i++){
		mItem[i].item=_getw(pFile);
		mItem[i].param=0;
	}

	mWeapon=_getw(pFile);
	mArmor=_getw(pFile);
	for(i=0;i<4;i++){
		mAccessory[i]=_getw(pFile);
	}							//装備情報

	for(i=0;i<MAX_MONTH;i++){
		for(int j=0;j<5;j++){
			mSkill[i][j]=_getw(pFile);
		}
	}							//習得済みスキル

	Calculate();
}
void	 CPlayerData::Load_0(FILE* pFile)
{
	mLv=_getw(pFile);
	mExp=_getw(pFile);
	int i;
	for(i=0;i<MAX_ITEM;i++){
		mItem[i].item=_getw(pFile);
		mItem[i].param=0;
	}

	mWeapon=_getw(pFile);
	mArmor=_getw(pFile);
	for(i=0;i<4;i++){
		mAccessory[i]=_getw(pFile);
	}							//装備情報

	for(i=0;i<MAX_MONTH;i++){
		for(int j=0;j<5;j++){
			mSkill[i][j]=_getw(pFile);
		}
	}							//習得済みスキル

	Calculate();
	AllHeal();
}

////////////////////////////////////////////////////////////////////////////////////
CEnemyData::CEnemyData()
{
	mSize=1;
}

void	CEnemyData::Draw(CPlaneWindow* window,int x,int y,int param,D3DCOLOR color)const
{
	D3DCOLOR textcolor=0xff000000;
	if(param & 1){
		textcolor=COLOR_GRAY;
	}
	window->DrawText(0,mName,x,y,textcolor);
	static char text[32];
	sprintf(text,"Lv.%d",mLv);
	window->DrawText(0,text,20,y,textcolor,DT_RIGHT);
}
void	CEnemyData::LoadEnemyData(int no,int lv)
{
	memset(mASkill,-1,sizeof(mASkill));
	memset(mCSkill,-1,sizeof(mCSkill));

	mNo=no;
	if(lv>=MAX_LV)	lv=MAX_LV;

	const CEnemyBaseData& edata=EnemyBaseData[no];

	mName=edata.mName;
	mThinkType=edata.mThinkType;
	mInt=edata.mInt;
	mLv=lv;
	mExp=edata.mExp;
	mMoney=edata.mMoney;
	mSize=edata.mSize;
	if(mSize<=0)	mSize=1;

	mStm	=edata.mBaseLP		+int(edata.mGrowLP		*GetGrowRatio(lv,GROW_STM));
	mAtk	=edata.mBaseAtk		+int(edata.mGrowAtk		*GetGrowRatio(lv,GROW_ATK));
	mDef	=edata.mBaseDef		+int(edata.mGrowDef		*GetGrowRatio(lv,GROW_DEF));
	mMagAtk	=edata.mBaseMagAtk	+int(edata.mGrowMagAtk	*GetGrowRatio(lv,GROW_MATK));
	mMagDef	=edata.mBaseMagDef	+int(edata.mGrowMagDef	*GetGrowRatio(lv,GROW_MDEF));
	mTech	=edata.mBaseTech	+int(edata.mGrowTech	*GetGrowRatio(lv,GROW_TECH));
	mSpeed	=edata.mBaseSpeed	+int(edata.mGrowSpeed	*GetGrowRatio(lv,GROW_SPD));

	CalculateLP();

	mLostLP=0;

	for(int i=0;i<edata.mSkillNo;i++){
		if(lv>=edata.mLearnLv[i]){
			int &skill=edata.mSkill[i];
			if(skill<ID_AS){
				int no=SetCSkill(skill);
				if(no>=0){
					mSkillParam[no]=edata.mSkillParam[i];
				}
			}else{
				int as=skill-ID_AS;
				if(SearchASkill(as)){
					ClearASkill(as);
				}else{
					SetASkill(as);
				}
			}
		}
	}

#ifdef _ENEMY_SKILL_LOCK_
	memset(mCSkill,-1,sizeof(mCSkill));
	mCSkill[0]=_ENEMY_SKILL_LOCK_;
#endif

/*	if(Flag.mMonth>MonthData[Flag.mCurrentMonth].mMonth){
		//プレイヤーとのレベル差が大きい場合の修正
		int reader=Flag.mParty[0];
		int plv=0;
		if(reader>=0)	plv=Flag.mPlayerData[reader].GetLv();
		if(plv-4>lv){
			mLv=(plv-lv)*3/4+lv;
			if(mLv%2==1)	mLv--;
			if(mLv>lv*2)	mLv=lv*2;
			int maxlv=MonthData[NormalHard[Flag.mHardMode][Flag.mMonth-1]].mBaseLv[Flag.mHardMode];
			if(mLv>maxlv)	mLv=maxlv;
			if(mLv<lv)		mLv=lv;
			CulculateLvRevision(lv,mLv,&mStm	,edata.mGrowLP		,GROW_STM);
			CulculateLvRevision(lv,mLv,&mAtk	,edata.mGrowAtk		,GROW_ATK);
			CulculateLvRevision(lv,mLv,&mDef	,edata.mGrowDef		,GROW_DEF);
			CulculateLvRevision(lv,mLv,&mMagAtk	,edata.mGrowMagAtk	,GROW_MATK);
			CulculateLvRevision(lv,mLv,&mMagDef	,edata.mGrowMagDef	,GROW_MDEF);
			CulculateLvRevision(lv,mLv,&mTech	,edata.mGrowTech	,GROW_TECH);
			CulculateLvRevision(lv,mLv,&mSpeed	,edata.mGrowSpeed	,GROW_SPD);

			CalculateLP();
		}
	}
*/
	CalculateSkill();
	CalculateSP();
	CheckParamMax();

	for(int i=0;i<MAX_CSKILL_LEVEL;i++){
		mLostSP[i]=0;
	}
}

void CEnemyData::CulculateLvRevision(int oldlv,int newlv,int* param,double grow,GROW_PARAM growratio)
{
	double oldratio=GetGrowRatio(oldlv,growratio);
	double grow2=(*param)/oldratio;
	if(grow2<grow)	grow=grow2;
	if(grow<0)		grow=0;

	double newratio=GetGrowRatio(newlv,growratio);
	*param +=int(grow*(newratio-oldratio));
}
void CEnemyData::ASkillOpen(int no)
{
	Flag.mEnemyOpenData[mNo].Set(ODATA_AS+no);
}

////////////////////////////////////////////////////////////////////////////////////

CCharacterBaseData::CCharacterBaseData(void)
{
	mFamilyName=NULL;
}
CCharacterBaseData::CCharacterBaseData(char* name,int id,char* fname,char* ename,int startlv,int lp,int tech,int atk,int def,int matk,int mdef,int spd)
{
	mName=name;
	mID=id;
	mFamilyName=fname;
	mEggName=ename;
	mStartLv=startlv;
	mLP=lp;
	mTech=tech;
	mAtk=atk;
	mDef=def;
	mMagAtk=matk;
	mMagDef=mdef;
	mSpeed=spd;
}
void CCharacterBaseData::Help(int param)const
{
}

const char*	CCharacterBaseData::GetName()const
{
	if(Flag.mCurrentMonth==17 && mEggName){
		return mEggName;
	}
#ifdef _DEBUG
	if(mID==12)	return "レーナ（若）";
#endif
	return mName;
}
const char*	CCharacterBaseData::GetFamilyName()const
{
	if(Flag.mCurrentMonth==17 && mEggName){
		return "";
	}
	return mFamilyName;
}

////////////////////////////////////////////////////////////////////////////////////

CPetData::CPetData(void)
{
	mSkill=NULL;
}
CPetData::CPetData(char* name,int id,int img,int size,int line,int lp,int atk,int def,int matk,int mdef,int spd,int tech,int skill, ...)
{
	mName=name;
	mID=id;

	mImg=img;
	mSize=size;
	mLine=line;

	mLP=lp;
	mTech=tech;
	mAtk=atk;
	mDef=def;
	mMagAtk=matk;
	mMagDef=mdef;
	mSpeed=spd;

	int i=0;
	static int allskill[MAX_CSKILL+MAX_ASKILL];
	va_list marker;

	va_start( marker, skill ); 
	while(skill>=0 && i<MAX_CSKILL+MAX_ASKILL)
	{
		allskill[i]=skill;
		i++;
		skill = va_arg( marker, int);
	}
	va_end( marker );

	mSkillNo=i;
	mSkill		= new int[i];

	memcpy(mSkill,allskill,sizeof(int)*i);
}
CPetData::~CPetData(void)
{
	delete[] mSkill;
}

void CPetData::Help(int param)const
{
}

////////////////////////////////////////////////////////////////////////////////////
const CCharacterBaseData CharacterBaseData[MAX_CHARACTER_NUMBER]={
	CCharacterBaseData(
		"カリス",0,
		"・トライアック・ファオーブ",
		"案内人",
		6,80,8,15,10,30,10,50
		),
	CCharacterBaseData(
		"レーナ",1,
		"・ミリス・ルミアル",
		NULL,
		6,5,10,20,0,50,15,60
		),
	CCharacterBaseData(
		"ホリィ",2,
		"・ウィル・カーニッシュ",
		NULL,
		0,30,4,10,5,10,5,100
		),
	CCharacterBaseData(
		"トルガン",3,
		"・グライデン・ハーレック",
		NULL,
		20,40,12,15,5,40,40,45
		),
	CCharacterBaseData(
		"ボルテ",4,
		"・トライアック・グライデン",
		"壮年",
		30,120,8,20,15,20,10,35
		),
	CCharacterBaseData(
		"ミゼリア",5,
		"・エイリーナ・オーリウェン",
		"人魚",
		18,30,12,5,0,25,5,55
		),
	CCharacterBaseData(
		"ルウェイン",6,
		"・ラーリア・ラーリア",
		NULL,
		32,30,5,5,0,5,0,35
		),
	CCharacterBaseData(
		"ラニー",7,
		"・パントン・ステリアス",
		NULL,
		5,60,10,10,5,25,15,55
		),
	CCharacterBaseData(
		"リティス",8,
		" ",
		"管理者",
		7,30,40,0,0,20,50,70
		),
	CCharacterBaseData(
		"ディアノ",9,
		"・ズィーレン・クリーザー",
		"剣士",
		40,200,30,15,15,200,15,70
		),
	CCharacterBaseData(
		"クロット",10,
		"・アーゲリオ・ヨークル",
		NULL,
		4,70,5,20,10,5,0,30
		),
	CCharacterBaseData(
		"フィルミナ",11,
		"・ラーリア・ラーリア",
		NULL,
		8,50,15,15,0,15,20,60
		),
	CCharacterBaseData(
		"レーナ",12,
		"・ミリス・ルミアル",
		"村娘",
		0,5,10,5,0,20,100,30
		),
	CCharacterBaseData(
		"",13,
		" ",
		" ",
		1,50,8,30,30,20,20,40
		),
	CCharacterBaseData(
		"",14,
		" ",
		" ",
		1,50,8,30,30,20,20,40
		),
};
const int CharacterSkill[MAX_CHARACTER_NUMBER][MAX_MONTH][MAX_SKILL_MONTH]={
	//カリス
	{
		{59	,93	,132,1	,8	,13	,27	,369},
		{60	,69 ,94	,45	,41	,21	,35	,252},
		{49	,102,96	,2	,9	,9	,14	,357},
		{79	,95	,46	,3	,9	,22	,36	,358},

		{61	,103,41	,3	,10	,15	,29	,356},
		{63	,97	,98	,4	,10	,23	,37	,309},
		{52	,64	,105,4	,11	,16	,30	,168},
		{72	,82	,11	,17	,24	,31	,38	,180},

		{106,45	,5	,17	,24	,31	,38	,172},
		{65	,99	,6	,18	,25	,32	,39	,273},
		{75	,54	,85	,107,44	,18	,32	,193},
		{43	,6	,44	,19	,26	,33	,40	,312},
	},
	//レーナ
	{
		{49	,41	,8	,13	,28	,28	,34	,93	},
		{79	,229,0	,13	,28	,28	,35	,48	},
		{50	,51	,45	,9	,9	,14	,29	,289},
		{52	,250,41	,9	,14	,29	,36	,337},

		{53	,82	,0	,10	,14	,30	,30	,102},
		{54	,1	,14	,14	,30	,30	,37	,180},
		{55	,11	,15	,15	,31	,37	,37	,105},
		{230,41	,1	,11	,31	,31	,38	,253},

		{85	,244,45	,12	,15	,32	,38	,181},
		{57	,290,1	,12	,32	,32	,38	,368},
		{1	,1	,44	,15	,33	,39	,39	,107},
		{43	,2	,44	,33	,33	,39	,39	,363},
	},
	//ホリィ
	{
		{234,46	,41	,0	,13	,20	,27	,132},
		{50 ,61 ,274,0	,8	,13	,27	,292},
		{80	,143,45	,8	,8	,14	,28	,288},
		{112,235,41	,1	,9	,14	,21	,272},

		{62 ,286,1	,1	,9	,29	,36	,268},
		{110,236,45 ,1	,15	,22	,36	,276},
		{144,41	,41	,2	,2	,10	,10	,114},
		{83 ,232,275,3	,11	,16	,30	,133},

		{113,237,46	,11 ,24	,31	,38	,284},
		{53	,64	,43	,3	,17	,24	,38	,300},
		{115,86	,43 ,3	,12	,32	,39	,116},
		{55	,66	,43	,4	,44	,18	,32	,233},
	},
	//トルガン 
	{
		{118,132,340,1	,8	,28	,37	,313},
		{110,134,238,319,8	,28	,37	,291},
		{111,120,133,46	,1	,8	,22	,314},
		{119,112,239,1	,8	,28	,37	,355},

		{113,121,338,1	,28	,38	,38	,301},
		{146,45	,1	,8	,21	,28	,38	,184},
		{122,240,9	,28	,28	,38	,38	,299},
		{114,135,339,41	,2	,9	,23	,185},

		{123,241,2	,29	,39	,39	,39	,383},
		{115,116,2	,9	,9	,29	,39	,180},
		{125,117,2	,9	,29	,39	,39	,129},
		{2	,2	,2	,9	,9	,40	,40	,315},
	},
	//ボルテ
	{
		{154,369,1	,1	,13	,21	,34	,58	},
		{59	,60	,1	,8	,14	,21	,34	,177},
		{62	,42	,2	,2	,8	,14	,35	,134},
		{49	,61	,286,42	,2	,9	,22	,317},

		{155,287,3	,9	,15	,23	,36	,267},
		{63 ,3	,3	,9	,16	,23	,36	,334},
		{64	,46	,4	,10	,24	,24	,37	,318},
		{52	,4	,4	,10	,17	,24	,37	,343},

		{65	,42	,5	,11	,17	,25	,38	,169},
		{54	,66	,5	,12	,18	,25	,38	,185},
		{67	,42	,6	,6	,12	,18	,26	,384},
		{42	,6	,6	,44	,26	,26	,39	,251},
	},
	/*旧ボルテ
	{
		{59	,1	,7	,13	,20	,27	,34	,58	},
		{1	,1	,8	,14	,21	,28	,35	,169},
		{41	,2	,8	,14	,21	,28	,35	,60	},
		{61	,2	,9	,15	,22	,29	,36	,118},

		{62	,3	,9	,15	,22	,29	,36	,173},
		{3	,3	,10	,16	,23	,30	,37	,181},
		{63	,41	,4	,10	,10	,16	,23	,121},
		{4	,4	,11	,17	,24	,31	,38	,177},

		{64	,5	,11	,17	,24	,31	,38	,185},
		{5	,12	,12	,18	,25	,32	,39	,123},
		{41	,6	,12	,18	,25	,32	,39	,65	},
		{66	,6	,44	,19	,26	,33	,40	,125},
	},
	*/
	//ミゼリア
	{
		{93	,110,42	,0	,7	,28	,34	,184},
		{94	,102,348,0	,20	,28	,34	,126},
		{118,229,42	,0	,8	,21	,34	,300},
		{344,345,1	,9	,9	,29	,35	,249},

		{113,45	,9	,30	,30	,35	,35	,189},
		{97	,105,2	,10	,10	,22	,22	,127},
		{121,2	,10	,10	,31	,31	,36	,246},
		{46	,41	,3	,11	,11	,31	,37	,193},

		{115,3	,11	,11	,23	,32	,37	,181},
		{99	,107,123,41	,4	,38	,38	,128},
		{117,4	,12	,12	,23	,33	,38	,230},
		{101,109,41	,44	,33	,33	,39	,346},
	},
	/*ガント
	{
		{69	,278,0	,7	,14	,20	,34	,68	},
		{59	,156,46	,1	,14	,21	,34	,172},
		{70	,71	,1	,1	,15	,15	,21	,330},
		{157,279,41	,2	,9	,15	,22	,332},

		{72	,60	,2	,16	,22	,22	,35	,333},
		{73	,74	,3	,16	,16	,23	,36	,316},
		{62	,280,46	,3	,10	,17	,23	,331},
		{63	,4	,4	,17	,17	,24	,37	,329},

		{75	,287,41	,4	,11	,18	,24	,158},
		{64	,76	,5	,12	,18	,25	,38	,283},
		{77	,5	,5	,18	,25	,25	,38	,173},
		{6	,6	,44	,19	,19	,26	,39	,169},
	},*/
	//ルウェイン
	{
		{157,0	,7	,13	,20	,27	,34	,78	},
		{158,159,46	,14	,21	,28	,35	,404},
		{96	,104,112,120,42,1	,8	,51	},
		{160,161,45	,1	,8	,15	,29	,405},

		{103,111,42	,2	,2	,9	,9	,406},
		{427,428,9	,16	,23	,30	,37	,64	},
		{98	,122,3	,10	,10	,16	,30	,162},
		{430,431,42	,3	,10	,24	,38	,407},

		{163,164,4	,4	,11	,17	,31	,76	},
		{165,429,4	,18	,25	,32	,39	,408},
		{432,43	,5	,12	,12	,18	,32	,166},
		{45	,5	,12	,19	,26	,33	,40	,309},
	},
	//ラニー
	{
		{79	,142,41	,0	,7	,13	,20	,78	},
		{49	,278,1	,8	,13	,21	,35	,220},
		{80	,143,282,1	,1	,8	,14	,378},
		{145,146,283,41	,9	,14	,21	,81 },

		{82	,96	,134,2	,9	,29	,36	,221},
		{83	,147,45	,3	,15	,29	,37	,223},
		{52	,112,279,46	,23	,30	,37	,224},
		{144,148,250,41	,11	,16	,30	,84},

		{85	,104,4	,12	,12	,17	,31	,222},
		{54	,280,41	,17	,25	,31	,39	,189},
		{86	,120,5	,12	,18	,25	,39	,173},
		{87	,43	,6	,44	,44	,18	,32	,227},
	},
	//リティス
	{
		{136,137,138,139,0	,8	,35	,184},
		{93	,111,119,46	,8	,28	,35	,301},
		{95	,103,41	,1	,9	,29	,36	,300},
		{96	,104,1	,9	,22	,29	,36	,299},

		{112,120,41	,2	,10	,30	,37	,298},
		{98	,114,46	,2	,30	,30	,37	,185},
		{106,122,41	,3	,11	,23	,38	,193},
		{108,124,45	,31	,31	,38	,38	,99	},

		{100,116,41	,4	,12	,23	,32	,107},
		{140,141,4	,12	,12	,32	,39	,115},
		{5	,5	,44	,44	,23	,33	,40	,123},
		{131,43	,44	,44	,33	,40	,40	,181},
	},
	//ディアノ(Hard５月)
	{
		{49	,241,319,0	,7	,13	,20 ,48},
		{51	,228,0	,7	,13	,27	,34	,356},
		{50	,282,1	,8	,14	,21	,35	,189},
		{52	,250,45	,1	,8	,14	,29	,369},

		{53	,231,46	,2	,9	,27	,36	,292},
		{94 ,229,10	,9	,15	,22	,29	,168},
		{54	,3	,17	,17	,23	,27	,29	,382},
		{55	,97	,3	,10	,16	,23	,37	,180},

		{244,45	,4	,11	,17	,27	,38	,172},
		{56	,232,4	,11	,17	,24	,38	,193},
		{99	,230,4	,17	,24	,27	,38	,298},
		{43	,4	,11	,17	,24	,27	,38	,352},
	},
	//クロット
	{

		{69	,374,46	,0	,13	,20	,34	,271},
		{70	,71 ,46	,1	,7	,14	,20	,68	},
		{59	,60	,156,42	,1	,8	,21	,316},
		{61	,228,2	,2	,8	,15	,21	,290},

		{72	,62	,42	,2	,15	,22	,35	,336},
		{73	,74	,42	,3	,9	,16	,36	,320},
		{63	,45	,3	,3	,10	,10	,23	,230},
		{64	,229,4	,10	,17	,17	,37	,330},

		{75	,42	,4	,4	,11	,17	,24	,332},
		{76	,65	,46	,5	,11	,18	,38	,372},
		{77	,66	,42	,5	,12	,18	,18	,426},
		{41	,6	,6	,12	,19	,19	,25	,173},
	},
	/*旧クロゼード
	{
		{46	,0	,0	,7	,13	,20	,34	,321},
		{49	,46	,41	,0	,7	,13	,20	,271},
		{69	,0	,7	,13	,13	,20	,20	,322},
		{51	,0	,0	,7	,13	,20	,34	,323},

		{70	,41	,1	,8	,14	,21	,35	,231},
		{52	,46	,1	,14	,14	,21	,21	,324},
		{72	,2	,2	,2	,9	,9	,9	,320},
		{53	,15	,15	,22	,22	,36	,36	,325},

		{46	,42	,3	,3	,10	,16	,23	,232},
		{54	,3	,10	,10	,16	,23	,37	,326},
		{74	,4	,4	,17	,17	,24	,24	,327},
		{56	,76	,4	,11	,17	,24	,38	,230},
	},
	*/
	/*フィルミア
	{
		{93	,132,0	,8	,20	,27	,35	,192},
		{102,104,260,1	,8	,21	,35	,168},
		{110,112,41	,1	,9	,14	,28	,184},
		{103,111,134,9	,9	,22	,36	,189},

		{133,261,46	,2	,10	,29	,37	,299},
		{105,106,3	,10	,10	,15	,23	,308},
		{113,114,135,11	,23	,30	,38	,300},
		{262,4	,4	,11	,11	,38	,38	,151},

		{108,116,12	,16	,24	,31	,39	,193},
		{107,42	,5	,12	,32	,39	,39	,185},
		{115,46	,5	,17	,25	,32	,40	,152},
		{109,6	,44	,44	,33	,40	,40	,128},
	},
	*/
	
	//フィルミナ
	{
		{93	,132,0	,8	,20	,27	,35	,184},
		{110,133,134,1	,8	,14	,28	,193},
		{94	,102,135,41	,1	,9	,14	,126},
		{95	,111,112,9	,9	,22	,36	,261},

		{45	,41	,2	,2	,10	,15	,29	,151},
		{97	,113,46	,3	,10	,16	,30	,189},
		{105,114,11	,16	,30	,38	,38	,298},
		{106,41	,11	,11	,17	,31	,38	,127},

		{108,100,4	,17	,24	,31	,39	,307},
		{107,115,5	,12	,18	,32	,39	,152},
		{101,116,5	,12	,18	,25	,32	,169},
		{43	,6	,44	,44	,32	,40	,40	,251},
	},
	//レーナ(Hard５月)
	{
		{132,41	,8	,13	,28	,28	,34	,49	},
		{93	,102,0	,13	,28	,28	,35	,48	},
		{95	,111,45	,9	,9	,14	,29	,250},
		{105,348,41	,9	,14	,29	,36	,337},

		{106,122,0	,10	,14	,30	,30	,52},
		{107,1	,14	,14	,30	,30	,37	,180},
		{108,11	,15	,15	,31	,37	,37	,54},
		{230,41	,1	,11	,31	,31	,38	,245},

		{133,354,45	,12	,15	,32	,38	,181},
		{109,346,1	,12	,32	,32	,38	,368},
		{1	,1	,44	,15	,33	,39	,39	,57},
		{43	,2	,44	,33	,33	,39	,39	,363},
	},
		{},
		{},
/*
	//テンプレート
	{
		{	,	,	,	,	,	,	,	},
		{	,	,	,	,	,	,	,	},
		{	,	,	,	,	,	,	,	},
		{	,	,	,	,	,	,	,	},

		{	,	,	,	,	,	,	,	},
		{	,	,	,	,	,	,	,	},
		{	,	,	,	,	,	,	,	},
		{	,	,	,	,	,	,	,	},

		{	,	,	,	,	,	,	,	},
		{	,	,	,	,	,	,	,	},
		{	,	,	,	,	,	,	,	},
		{	,	,	,	,	,	,	,	},
	},
*/
};
const int StartSkill[MAX_CHARACTER_NUMBER][MAX_MONTH][MAX_LEARN_MONTH+1]={
//カリス
	{
		{1,3}
	},
//レーナ
	{
		{1,5}
	},
//ホリィ
	{
	},
//トルガン
	{
		{1,2,6}
	},
//ボルテ
	{
		{1,2,3},
		{1,2,3},
		{1,3,5},
		{5,7},
	},
//ミゼリア
	{
		{1,2},
	},
//ルウェイン
	{
		{1,2,3},
		{1,2,3},
		{5,6,7},
		{1,2,3},

		{3,4,6},
		{3,5,7},
		{6,7},
	},
//ラニー
	{
		{1,2},
		{2},
	},
//リティス
	{
		{1,2,3,4},
	},
//ディアノ（Hard５月）
	{
		{1,2,3,6,7},
		{1,2,4,6,7},
		{1,2,3,4,5},
		{1,2,4,6,7},
		{1,2,3,4,5},
		{1,2,5,7},
		{1,2,3,5},
		{1,2,4},
		{1,3},
		{1},
		{2}
	},
//クロット
	{
		{1,2,3},
	},
//フィルミア
	{
		{1,2,4},
		{2,3},
	},
//レーナ（Hard５月）
	{
	},

	{
	},
	{
	},
};
const int	StartItem[MAX_CHARACTER_NUMBER][MAX_ITEM]={
	//カリス
	{ID_SPR+WLV_0+0	,ID_AXE+WLV_0+0,NO_ITEM	,ID_AMR+ALV_0+0	,ID_SLD+0	,-1},
	//レーナ
	{ID_SWD+WLV_0+0	,NO_ITEM,NO_ITEM		,ID_AMR+ALV_0+0	,ID_GLV+0	,-1},
	//ホリィ
	{ID_BOW+WLV_0+0	,NO_ITEM,NO_ITEM		,ID_AMR+ALV_0+0	,ID_SHE+1	,NO_ITEM,ID_UI	,-1},
	//トルガン
	{ID_WND+WLV_0+0	,NO_ITEM,NO_ITEM		,ID_AMR+ALV_0+2	,ID_SHE+0	,NO_ITEM,ID_UI	,-1},
	//ボルテ
	{ID_SPR+WLV_1+3	,NO_ITEM,NO_ITEM		,ID_AMR+ALV_1+2	,ID_SLD+4,ID_SHE+3	,-1},
	//ミゼリア
	{ID_WND+WLV_2+2	,NO_ITEM,NO_ITEM		,ID_AMR+ALV_2+2	,ID_HED+3,ID_SHE+5	,-1},
	//ルウェイン
	{ID_SWD+WLV_2+3	,NO_ITEM,NO_ITEM		,ID_AMR+ALV_3+3	,ID_HED+6,ID_AME+7	,-1},
	//ラニー
	{ID_BOW+WLV_1+2	,NO_ITEM,NO_ITEM		,ID_AMR+ALV_1+0	,ID_GLV+5,ID_HED+2	,-1},
	//リティス
	{ID_SWP+2		,NO_ITEM,NO_ITEM		,ID_AMR+ALV_0+2	,-1},
	//ディアノ(Hard5月)
	{ID_SWD+WLV_4+3	,NO_ITEM,NO_ITEM		,ID_AMR+ALV_2+0	,ID_GLV+12,ID_SHE+9	,ID_UI+7,ID_UI+1,-1},
	//クロ
	{ID_AXE+WLV_0+0	,NO_ITEM,NO_ITEM		,ID_AMR+ALV_0+1	,ID_GLV+0	,NO_ITEM,ID_UI,ID_UI,-1},
	//フィルミア
	{ID_WND+WLV_1+0	,NO_ITEM,NO_ITEM		,ID_AMR+ALV_1+3	,ID_AME+17	,NO_ITEM,ID_UI,ID_UI,-1},
	//レーナ(Hard5月)
	{ID_WND+WLV_1+1	,NO_ITEM,NO_ITEM		,ID_AMR+ALV_2+2	,ID_AME+8	,NO_ITEM,ID_UI+1,-1},
	//
	{-1},
	//
	{-1}
};
const CPetData	PetData[MAX_PET_TYPE]={
	CPetData(	"★クロたん" ,0,		//名称、ID
				100,SMALL,2,		//画像、サイズ、配置(左が0)
				40		,75		,50		,75		,50		,100	,0,		//各種パラメータ（百分率）
				//スキル
				418,93,				
				-1),
	CPetData(	"★マボ太" ,1,		//名称、ID
				101,SMALL,3,		//画像、サイズ、配置
				30		,10		,10		,90		,200	,120	,50,	//各種パラメータ（百分率）
				//スキル
				94,102,110,118,
				-1),
	CPetData(	"★ガン九郎" ,2,		//名称、ID
				105,SMALL,0,		//画像、サイズ、配置
				60		,40		,200	,10		,10		,20		,50,	//各種パラメータ（百分率）
				//スキル
				69,70,71,207,208,
				-1),
	CPetData(	"★キノ子" ,3,		//名称、ID
				108,SMALL,2,		//画像、サイズ、配置
				50		,10		,10		,10		,10		,100	,50,	//各種パラメータ（百分率）
				//スキル
				132,134,
				-1),
	CPetData(	"★爆次" ,4,		//名称、ID
				107,SMALL,2,		//画像、サイズ、配置
				10		,10		,10		,60		,10		,250	,50,	//各種パラメータ（百分率）
				//スキル
				417,396,247,
				-1),
	CPetData(	"★クマクマ" ,5,		//名称、ID
				111,LARGE,1,		//画像、サイズ、配置
				80		,100	,100	,10		,10		,80		,100,	//各種パラメータ（百分率）
				//スキル
				53,63,64,372,
				-1),
	CPetData(	"★樹太郎" ,6,		//名称、ID
				120,SMALL,2,		//画像、サイズ、配置
				60		,100	,50		,10		,50		,30		,100,	//各種パラメータ（百分率）
				//スキル
				414,411,412,372,199,
				-1),
	CPetData(	"★フェア美" ,7,		//名称、ID
				121,SMALL,3,		//画像、サイズ、配置
				50		,90		,50		,90		,50		,200	,100,	//各種パラメータ（百分率）
				//スキル
				83,113,144,372,242,
				-1),
	CPetData(	"★ハチ" ,8,			//名称、ID
				138,SMALL,1,		//画像、サイズ、配置
				150		,60		,50		,10		,50		,80		,100,	//各種パラメータ（百分率）
				//スキル
				73,74,415,416,371,199,200,
				-1),
	CPetData(	"★バーン隊長" ,9,	//名称、ID
				119,LARGE,2,		//画像、サイズ、配置
				100		,120	,100	,120	,100	,100	,100,	//各種パラメータ（百分率）
				//スキル
				65,75,409,410,373,242,247,
				-1),
};