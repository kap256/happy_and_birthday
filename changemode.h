#pragma once

#include "main.h"
#include "mapobject.h"


const int ENCOUNT_TIME			=20;
const int ENCOUNT_TIME_AFTER	=20;

const int RETURN_MAP_TIME			=20;
const int RETURN_MAP_TIME_AFTER		=20;


class CEncountInfo{
public:
	bool	mEncount;
	int		mTimer;
	int		mPartyNo;

	int		mReader;
	int		mLv;
	int		mMember;
	int		mField;
	int		mOption;

	int		mLvUp;
public:
	CEncountInfo();

	void	Encount(CEnemyObject* object,int lvup,int option);
	void	Encount(int party,int lvup,int option=0);
	void	Encount(int option);
};

class CReturnMapInfo{
public:
	bool	mReturnMap;
	int		mTimer;
	bool	mEscaped;

public:
	CReturnMapInfo();
	void	ReturnMap(bool escape);
};

class CChangeMapInfo{
public:
	bool	mChangeMap;
	int		mTimer;

	int		mNo;
	int		mX;
	int		mY;
	int		mZ;
	int		mDir;

	bool	mLoad;

public:
	CChangeMapInfo();
	void	ChangeMap(int no,int x,int y,int z,int dir);
};