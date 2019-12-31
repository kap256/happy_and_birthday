#include "enemyopendata.h"


CEnemyOpenData::CEnemyOpenData()
{
	memset(mOpenData,0,sizeof(mOpenData));
	mKillNo=0;
}
void	CEnemyOpenData::Save(FILE* pFile)
{
	fwrite(mOpenData,1,sizeof(mOpenData),pFile);
	_putw(mKillNo,pFile);
}
void	CEnemyOpenData::Load_0(FILE* pFile)
{
	fread(mOpenData,1,sizeof(mOpenData),pFile);
	mKillNo=_getw(pFile);
}

bool	CEnemyOpenData::Get(int no)
{
	if(no<0 || MAX_ODATA<=no)	return false;
	return mOpenData[no];
}
void	CEnemyOpenData::Set(int no)
{
	if(no<0 || MAX_ODATA<=no)	return;
	mOpenData[no]=true;
}
void	CEnemyOpenData::SetKill(void)
{
	if(mKillNo>=5){
		mOpenData[ODATA_LP]=true;
	}
	if(mKillNo>=10){
		mOpenData[ODATA_SP]=true;
	}
	if(mKillNo>=15){
		mOpenData[ODATA_SPD]=true;
	}
	if(mKillNo>=30){
		mOpenData[ODATA_ATK]=true;
		mOpenData[ODATA_MATK]=true;
		mOpenData[ODATA_DEF]=true;
		mOpenData[ODATA_MDEF]=true;	
	}
	if(mKillNo>=100){
		memset(mOpenData,1,sizeof(mOpenData));
	}
	mKillNo++;
}

