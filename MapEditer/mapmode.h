#ifndef _MAPMODE_
#define _MAPMODE_

#include "dtest.h"
#include "../main.h"
#include "../mapconst.h"

class CMapMode{
	int			mMapNo;
	int			mBGColor;
	int			mMapData[MAP_SIZE_X][MAP_SIZE_Y][MAP_SIZE_Z];
	int			mMapSizeX;
	int			mMapSizeY;
	CDTTexture	mMapChipImg[MAP_CHIP_NO];

	int			mUndoData[8][MAP_SIZE_X][MAP_SIZE_Y][MAP_SIZE_Z];
	int			mUndoNo;
	bool		mUndoSaved;

	bool		mPhotoMode;


public:
	CMapMode(void);
	void AfterStartUp(void);
	bool LoadMap(int mapno);
	bool LoadMapData(char* fname);
	void OnTimer(void);
	void OnDraw(void);
	void DrawMap(int camerax,int cameray,int mousex,int mousey,int &cx,int &cy,int &cz);

	void MapShift(int sx,int sy,int sz);
	void Mennuri(int x,int y,int z,int pallete,int zmove=0);
	void Mennuri(int x,int y,int z,int before,int after,int zmove,bool draw[MAP_SIZE_X][MAP_SIZE_Y]);
	void Reset(void);

	void FuncMinimum(int before,int after,int zure);

	void FuncToWB(int pz);
	void FuncFromWB(void);

	int GetChipType(int x,int y,int z){return MapChipParam[mMapData[x][y][z]/20];}

	bool Save(void);

};

#endif