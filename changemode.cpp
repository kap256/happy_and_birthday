#include "changemode.h"
#include "flag.h"
#include "enemydata.h"
#include "main.h"
#include "battlemode.h"
#include "mapmode.h"
#include "option.h"

CEncountInfo::CEncountInfo()
{
	mEncount=false;
	mTimer=ENCOUNT_TIME+ENCOUNT_TIME_AFTER;
	mPartyNo=-1;
}
void	CEncountInfo::Encount(int option)
{
	mEncount=true;
	mTimer=0;
	if(Flag.mMapBGMNo!=8 && Flag.mMapBGMNo!=16 && !(mOption & BOPT_BGM_NO_CHANGE))	PlayBGM(0);
	PlayWave(SE_ENCOUNT);

	switch(option){
		case 1:
			mOption =mOption | BOPT_FIRST_ATTACK;
			break;
	}
	
	BattleMode.LoadTexture();
}
void	CEncountInfo::Encount(CEnemyObject* object,int lvup,int option)
{
	mPartyNo=-1;

	mReader	=object->mReader;
	mLv		=object->mLv/4*4;
	mMember	=object->mMember;
	mField	=object->mField;
	mOption	=object->mOption;

	if(Flag.mCurrentMonth==14)	mOption=(mOption | BOPT_BGM_NO_CHANGE);

	mLvUp=lvup;
	Encount(option);

}
void	CEncountInfo::Encount(int party,int lvup,int option)
{
	mPartyNo=party;

	mField	=EnemyPartyData[party].mField;
	mOption	=EnemyPartyData[party].mOption;

	mLvUp=lvup;

	Encount(option);
}


//////////////////////////////////////////////////////////////////////////////

CReturnMapInfo::CReturnMapInfo()
{
	mReturnMap=false;
	mEscaped=false;
	mTimer=RETURN_MAP_TIME+RETURN_MAP_TIME_AFTER;
}

void	CReturnMapInfo::ReturnMap(bool escape)
{
	mReturnMap=true;
	mEscaped=escape;
	mTimer=0;
	PlayBGM(Flag.mMapBGMNo);

	for(int i=0;i<MAX_CHARACTER_NUMBER;i++){
		Flag.mPlayerData[i].Equip(0);
	}
#ifdef _BATTLE_FPS_
	DTest.SetFPS(60);
#endif
}

//////////////////////////////////////////////////////////////////////////////

CChangeMapInfo::CChangeMapInfo()
{
	mChangeMap=false;
	mTimer=RETURN_MAP_TIME+RETURN_MAP_TIME_AFTER;
}

void	CChangeMapInfo::ChangeMap(int no,int x,int y,int z,int dir)
{
//-1	村に帰る
//-2	タイトルへ
//-3	体験版デモ

	mChangeMap=true;
	if(mTimer>=RETURN_MAP_TIME+RETURN_MAP_TIME_AFTER || no == -3){
		mTimer=0;
		mLoad=false;
	}else{
		mTimer=RETURN_MAP_TIME-1;
		mLoad=(mNo>=0);
	}

	mNo=no;
	mX=x;
	mY=y;
	mZ=z;
	mDir=dir;

	DTest.DrawSkip(false);

	
/*
	if(mNo>=0){
		MapMode.LoadMap(mNo,
						mX,
						mY,
						mZ,
						mDir);
	}
*/
}