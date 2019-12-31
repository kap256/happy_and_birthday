#include "number.h"
#include "input.h"
#include "mapmode.h"
#include "flag.h"
#include "changemode.h"

int CNumber::mCurrent=0;

////////////////////////////////////////////////////////////////////////////////

int* CNumber::GetSpecialNumber(int *num)
{
	if(!num)	return NULL;
	if(!mSNum)	return num;

	int index=0;
	if(SpecialNumberInfo[mNumber].mArray){
		index=mIndex->GetNumber();
		if(index<0 && (SpecialNumberInfo[mNumber].mSNumber!=SNUM_KAKKO && SpecialNumberInfo[mNumber].mSNumber!=SNUM_FIX)) 
			index=0;
		else if(index>=SpecialNumberInfo[mNumber].mMax){
			index=SpecialNumberInfo[mNumber].mMax-1;
		}
	}

	*num=0;
	switch(SpecialNumberInfo[mNumber].mSNumber){
		case SNUM_RANDOM:
			*num=pInput->Rand(RAND_MAX);		return num;

		case SNUM_MONEY:
			*num=Flag.mMoney;					return num;

		case SNUM_PLAYER_IMAGE:
			*num=Flag.mParty[0];				return num;

		case SNUM_CURRENT:
			*num=mCurrent;						return num;
		
		case SNUM_CURRENT_MONTH:
			*num=Flag.mCurrentMonth;			return num;

		case SNUM_MONTH:
			*num=Flag.mMonth;					return num;

		case SNUM_MAP_X:
			*num=MapMode.mMapSizeX;				return num;

		case SNUM_MAP_Y:
			*num=MapMode.mMapSizeY;				return num;


		case SNUM_ESCAPE:
			*num=(ReturnMapInfo.mEscaped?1:0);	return num;

		case SNUM_KAKKO:
			*num=index;							return num;

		case SNUM_ROOT:
			*num=int(sqrt(double(index)));		return num;

		case SNUM_FIX:
			mNumber=index;
			mSNum=false;
			*num=index;
			return num;

		case SNUM_PARAMA:
			if(MapMode.mRuningEvent){
				*num=MapMode.mRuningEvent->mParamA;	
				return &(MapMode.mRuningEvent->mParamA);
			}
			*num=-1;	
			return num;

		case SNUM_PARAMB:
			if(MapMode.mRuningEvent){
				*num=MapMode.mRuningEvent->mParamB;	
				return &(MapMode.mRuningEvent->mParamB);
			}
			*num=-1;	
			return num;

		case SNUM_EVENT_ID:
			if(MapMode.mRuningEvent){
				*num=MapMode.mRuningEvent->mID;
			}else{
				*num=-1;	
			}
			return num;

		case SNUM_BUSINESS:
			*num=Flag.GetBusinessAbility();		return num;

		case SNUM_MAP_NO:
			*num=ChangeMapInfo.mNo;				return num;

		case SNUM_START_X:
			*num=ChangeMapInfo.mX;				return num;
		case SNUM_START_Y:
			*num=ChangeMapInfo.mY;				return num;
		case SNUM_START_Z:
			*num=ChangeMapInfo.mZ;				return num;
		case SNUM_START_DIR:
			*num=ChangeMapInfo.mDir;			return num;

		case SNUM_HARD_MODE:
			*num=Flag.mHardMode;				return num;

		case SNUM_EVENT_SKIP:
			*num=(MapMode.mEventSkip>=0x80000000+ESKIP_BLACK_OUT?1:0);
			return num;

		case SNUM_RETURN:
			*num=MapMode.mEventReturn;			return num;

		case SNUM_OBJECT_X:
			if(MapMode.mObject[index])
				*num=MapMode.mObject[index]->GetX();
			return num;

		case SNUM_OBJECT_Y:
			if(MapMode.mObject[index])
				*num=MapMode.mObject[index]->GetY();
			return num;

		case SNUM_OBJECT_Z:
			if(MapMode.mObject[index])
				*num=MapMode.mObject[index]->GetZ();
			return num;

		case SNUM_OBJECT_DIR:
			if(MapMode.mObject[index])
				*num=MapMode.mObject[index]->GetDir();
			return num;

		case SNUM_OBJECT_GOAL:
			if(MapMode.mObject[index])
				*num=(MapMode.mObject[index]->CheckGoal()?1:0);
			return num;

		case SNUM_FLAG:
			*num=Flag.mFlag[index];
			return &(Flag.mFlag[index]);

		case SNUM_MAP_FLAG:
			*num=MapMode.mMapFlag[index];
			return &(MapMode.mMapFlag[index]);
		case SNUM_OBJECT_X_32:
			if(MapMode.mObject[index])
				*num=MapMode.mObject[index]->GetX_32();
			return num;
		case SNUM_OBJECT_Y_32:
			if(MapMode.mObject[index])
				*num=MapMode.mObject[index]->GetY_32();
			return num;
		case SNUM_OBJECT_Z_32:
			if(MapMode.mObject[index])
				*num=MapMode.mObject[index]->GetZ_32();
			return num;

		case SNUM_OBJECT_CAM_X:
			if(MapMode.mObject[index]){
				CMapObject* obj=MapMode.mObject[index];
				*num=obj->GetX()-obj->GetY()+32;
			}
			return num;
		case SNUM_OBJECT_CAM_Y:
			if(MapMode.mObject[index]){
				CMapObject* obj=MapMode.mObject[index];
				*num=(obj->GetX() + obj->GetY())/2 - obj->GetZ();
			}
			return num;

		case SNUM_ITEM_NO:
			*num=Flag.GetAllItem(index,false,2);	return num;

		case SNUM_MEMBER:
			*num=Flag.mParty[index];				return num;

		case SNUM_SHOP_OK:
			*num=Flag.mShopOK[index];				return &(Flag.mShopOK[index]);

		case SNUM_BOX:
			*num=(Flag.mTreasure[index]?1:0);		return num;

		case SNUM_IS_MEMBER:
			*num=Flag.SearchMember(index,MAX_PARTY_NUMBER);		return num;

		default:
			return num;
	}
}


CNumber::CNumber()
{
	mNumber=0;
	mSign=END;
	mIndex=NULL;
	mNext=NULL;
	mSNum=false;
}

CNumber::~CNumber()
{
}

void	CNumber::LoadNumber(FILE *pFile)
{
	mNumber=_getw(pFile);
	if(mNumber<0){
		for(int i=0;i<MAX_SNUM_NUMBER;i++){
			if(SpecialNumberInfo[i].mSNumber==mNumber){
				if(SpecialNumberInfo[i].mArray){
					mIndex= new CNumber;
					mIndex->LoadNumber(pFile);
				}
				mSNum=true;
				mNumber=i;
				break;
			}
		}
	}
	mSign=NumberSign(getc(pFile));
	if(!mSign==END){
		mNext= new CNumber;
		mNext->LoadNumber(pFile);
	}	
}
int		CNumber::GetNumber(void)
{
	CNumber *cnumber=this;
	int ret=0;
	int sign=ADD;
	while(cnumber){
		int num=cnumber->mNumber;
		if(cnumber->mSNum){
			cnumber->GetSpecialNumber(&num);
		}

		switch(sign){
			case ADD:
				ret+=num;	break;
			case SUB:
				ret-=num;	break;
			case MUL:
				ret*=num;	break;
			case DIV:
				ret/=num;	break;
			case RES:
				ret%=num;	break;
			default:
				return ret;
		}
		sign=cnumber->mSign;
		cnumber=cnumber->mNext;
	}
	return ret;
}

CNumber	CNumber::Copy(void)
{
	return *this;
}
void	CNumber::AllDelete(void)
{
	AllDelete(mNext);
	AllDelete(mIndex);
	mNext=NULL;
	mIndex=NULL;
}
void	CNumber::AllDelete(CNumber* num)
{
	CNumber *del;
	if(num){
		del=num;
		num=num->mNext;
		del->AllDelete(del->mIndex);
		delete del;
	}
}

