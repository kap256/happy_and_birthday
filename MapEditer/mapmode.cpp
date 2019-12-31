#include "mapmode.h"
#include "editerbutton.h"
#include "stdio.h"


const int BUTTON_NO		=35;
const int WIN_SIZE_X	=640;
const int WIN_SIZE_Y	=480;

int gMapChipSize=32;
int gCamerax=-320;
int gCameray=-240;
int gPalette=20;
int gPalettePage=1;


int gZRock=-1;
CDTTexture gButtonTex;

#define DRAW_MAP_EDITER
#include "../drawmap.h"

CEditerButton gButton[BUTTON_NO];


CMapMode::CMapMode(void)
{
}
void CMapMode::AfterStartUp(void)
{
	for(int n=0;n<MAP_CHIP_NO;n++){
		char filename[256];
		sprintf(filename,"..\\..\\data\\mapchip\\mapchip%02d.bmp",n);
		DTest.LoadTexture(mMapChipImg[n],filename,0xff008080);
	}
	DTest.LoadTexture(gButtonTex,"..\\button.bmp",0xff008080);
	gButton[0].Init(WIN_SIZE_X-300,WIN_SIZE_Y+50,0,0xffffb0b0);
	gButton[1].Init(WIN_SIZE_X-300,WIN_SIZE_Y+10,2,0xffffb0b0);
	gButton[2].Init(WIN_SIZE_X-300,WIN_SIZE_Y+160,1,0xffb0b0ff);
	gButton[3].Init(WIN_SIZE_X-300,WIN_SIZE_Y+200,3,0xffb0b0ff);
	gButton[4].Init(90,WIN_SIZE_Y+100,0,0xffffb0b0);
	gButton[5].Init(90,WIN_SIZE_Y+60,2,0xffffb0b0);
	gButton[6].Init(130,WIN_SIZE_Y+100,1,0xffb0b0ff);
	gButton[7].Init(130,WIN_SIZE_Y+60,3,0xffb0b0ff);
	gButton[8].Init(180,WIN_SIZE_Y+100,0,0xffffb0b0);
	gButton[9].Init(180,WIN_SIZE_Y+60,2,0xffffb0b0);
	gButton[10].Init(220,WIN_SIZE_Y+100,1,0xffb0b0ff);
	gButton[11].Init(220,WIN_SIZE_Y+60,3,0xffb0b0ff);
	gButton[12].Init(10,WIN_SIZE_Y+160,1,0xffb0b0ff);
	gButton[13].Init(50,WIN_SIZE_Y+160,3,0xffb0b0ff);
	gButton[14].Init(30,WIN_SIZE_Y+200,5,0xffb0b0ff);
	gButton[15].Init(70,WIN_SIZE_Y+200,7,0xffb0b0ff);
	gButton[16].Init(180,WIN_SIZE_Y+160,0,0xffffb0b0);
	gButton[17].Init(220,WIN_SIZE_Y+160,2,0xffffb0b0);
	gButton[18].Init(160,WIN_SIZE_Y+200,4,0xffffb0b0);
	gButton[19].Init(200,WIN_SIZE_Y+200,6,0xffffb0b0);
	gButton[20].Init(115,WIN_SIZE_Y+200,9,0xff80ff80);
	gButton[21].Init(30,WIN_SIZE_Y+30,8,0xffff0000);
	gButton[22].Init(260,WIN_SIZE_Y+200,0,0xffffb0b0);
	gButton[23].Init(300,WIN_SIZE_Y+200,1,0xffb0b0ff);

	gButton[24].Init(260,WIN_SIZE_Y+40 ,10,0xffb0ffb0);
	gButton[25].Init(300,WIN_SIZE_Y+40 ,11,0xffb0ffb0);
	gButton[26].Init(260,WIN_SIZE_Y+80,12,0xffb0ffb0);
	gButton[27].Init(300,WIN_SIZE_Y+80,13,0xffb0ffb0);
	gButton[28].Init(260,WIN_SIZE_Y+120 ,14,0xffb0ffb0);
	gButton[29].Init(300,WIN_SIZE_Y+120,15,0xffb0ffb0);
	gButton[30].Init(680,30,16,0xffb0ffb0);

	gButton[31].Init(645,120,18,0xffb0ffb0);
	gButton[32].Init(675,80,17,0xffb0ffb0);
	gButton[33].Init(685,120,19,0xffb0ffb0);
	gButton[34].Init(675,160,20,0xffb0ffb0);
	mMapNo=0;
	mBGColor=0;

//全体のマップに対する処理をするとき、ここを使おう！
/**
	{
		FILE* log=fopen("..\\..\\data\\  マップデータ変更ログ.txt","w");
		for(mMapNo=0;mMapNo<1000;mMapNo++){
			bool output_log=false;
			if(LoadMap(mMapNo)){
				//ここに個別の処理を書き込む
				for(int x=0;x<mMapSizeX;x++){
					for(int y=0;y<mMapSizeY;y++){
						for(int z=0;z<MAP_SIZE_Z;z++){
							int& chip=mMapData[x][y][z];
							if(260<=chip && chip<=277 && chip%4<2){
								chip=0;
								output_log=true;
							}
						}
					}
				}
				//ここまで
				Save();
				if(output_log) fprintf(log,"%d\n",mMapNo);
			}
		}
		fclose(log);
		mMapNo=0;
	}
/**/
//ここまで！

	FILE* pFile=fopen("dat","rb");
	if(pFile){
		mMapNo=_getw(pFile);
		fclose(pFile);
	}
	LoadMap(mMapNo);

	mPhotoMode=false;
}
bool CMapMode::LoadMap(int mapno)
{
	mMapSizeX=MIN_MAP_SIZE;
	mMapSizeY=MIN_MAP_SIZE;

	Reset();

	char filename[256];
	sprintf(filename,"..\\..\\data\\mapdata\\mapdata%04d",mMapNo);
	bool ret=LoadMapData(filename);

	mUndoSaved=false;
	mUndoNo=0;
	for(int n=0;n<8;n++){
		memcpy(mUndoData[n],mMapData,sizeof(mMapData));
	}

	return ret;
}
void CMapMode::Reset(void)
{
	gCamerax=-320;
	gCameray=-240;
	memset(mMapData,0,sizeof(mMapData));
	int x,y;
	for(x=1;x<mMapSizeX;x++){
		for(y=1;y<mMapSizeY;y++){
			mMapData[x][y][2]=20;
		}
	}
	for(x=0;x<MAP_SIZE_X;x++){
		for(y=0;y<MAP_SIZE_Y;y++){
			mMapData[x][y][0]=20;
			mMapData[x][y][1]=1;
		}
	}
}

void CMapMode::OnTimer(void)
{
}
void CMapMode::OnDraw(void)
{
	int mousex,mousey;
	DTest.GetMousePos(&mousex,&mousey);
	int leftbutton=DTest.GetMouseButton(0);
	int rightbutton=DTest.GetMouseButton(1);

	if(DTest.GetKey(DIK_P)==1){
		mPhotoMode=!mPhotoMode;
	}
	if(DTest.GetKey(DIK_W)==1){
		gMapChipSize/=2;
		gCamerax=(gCamerax+mousex)/2-mousex;
		gCameray=(gCameray+mousey)/2-mousey;
	}
	if(DTest.GetKey(DIK_Q)==1){
		gMapChipSize*=2;
		gCamerax=(gCamerax+mousex)*2-mousex;
		gCameray=(gCameray+mousey)*2-mousey;
	}

	if(DTest.GetKey(DIK_Z)==1 && DTest.GetKey(DIK_LCONTROL)){
		memcpy(mUndoData[mUndoNo],mMapData,sizeof(mMapData));
		if(DTest.GetKey(DIK_LSHIFT)){
			mUndoNo=(mUndoNo+1)%8;
		}else{
			mUndoNo--;
			if(mUndoNo<0)	mUndoNo+=8;
		}
		memcpy(mMapData,mUndoData[mUndoNo],sizeof(mMapData));
	}


	if(mPhotoMode){
		DTest.DrawBox(0,0,WIN_SIZE_X,WIN_SIZE_Y,
			0xff008080);
	}else{
		DTest.DrawBox(0,0,WIN_SIZE_X,WIN_SIZE_Y,
			cMapBGColor[mBGColor][0],cMapBGColor[mBGColor][0],cMapBGColor[mBGColor][1],cMapBGColor[mBGColor][1]);
	}

	int i;
	int cursorx=-1;
	int cursory=0;
	int cursorz=0;
	DrawMap(gCamerax,gCameray,mousex,mousey,cursorx,cursory,cursorz);

	if(DTest.GetKey(DIK_C)==1){
		if(gZRock>=0){
			gZRock=-1;
		}else if(cursorx>=0){
			gZRock=cursorz;
		}
	}

	DTest.DrawBox(0,WIN_SIZE_Y,WIN_SIZE_X,1280,0xffffff80);
	DTest.DrawBox(WIN_SIZE_X,0,1280,1280,0xffffff80);
	int chip=gPalette%20;
	int page=gPalette/20;
	D3DCOLOR color=0xff008080;
	DTest.DrawBox(10,WIN_SIZE_Y+75,64+10,WIN_SIZE_Y+80+50,color);
	DTest.DrawTexture(mMapChipImg[page],0,(chip%4)*64,(chip/4)*48,64,48,10,WIN_SIZE_Y+80);
	DTest.Printf(0,10,WIN_SIZE_Y+130,"Chip:%d",gPalette);
	DTest.Printf(0,650,10,"Func",gPalette);

	DTest.DrawBox(WIN_SIZE_X-260,WIN_SIZE_Y,WIN_SIZE_X-4,WIN_SIZE_Y+48*5,color);
	if(WIN_SIZE_X-260<mousex&&mousex<WIN_SIZE_X-4 && WIN_SIZE_Y<mousey && mousey<WIN_SIZE_Y+48*5){
		int x=(mousex-(WIN_SIZE_X-260))/64;
		int y=(mousey-WIN_SIZE_Y)/48;
		DTest.DrawBox(WIN_SIZE_X-260+x*64,WIN_SIZE_Y+y*48,WIN_SIZE_X-260+x*64+64,WIN_SIZE_Y+y*48+48,0xffff4040);
		if(leftbutton==1){
			gPalette=gPalettePage*20+y*4+x;
		}
	}


	DTest.DrawTexture(mMapChipImg[gPalettePage],0,0,0,256,48*5,WIN_SIZE_X-260,WIN_SIZE_Y);

	for(i=0;i<BUTTON_NO;i++){
		if(gButton[i].OnTimer(gButtonTex,mousex,mousey)){
			if(DTest.GetMouseButton(0)==1){
				int *mapsize=&mMapSizeY;
				const int *maxsize=&MAP_SIZE_Y;
				switch(i){
					case 0:
					case 1:
					case 2:
					case 3:
						int add;
						if(i%2==1)	add=5;	else add=1;
						if(i>=2)	add*=-1;
						gPalettePage+=add;
						gPalettePage%=MAP_CHIP_NO;
						if(gPalettePage<0)	gPalettePage+=MAP_CHIP_NO;
						break;
					case 4:
					case 5:
					case 6:
					case 7:
						mapsize=&mMapSizeX;
						maxsize=&MAP_SIZE_X;
					case 8:
					case 9:
					case 10:
					case 11:
						if(i%2==1)	add=5;	else add=1;
						if(i%4>=2)	add*=-1;
						*mapsize+=add;
						if(*mapsize>*maxsize-2)		*mapsize=*maxsize-2;
						if(*mapsize<MIN_MAP_SIZE)	*mapsize=MIN_MAP_SIZE;
						break;
					case 12:
					case 13:
					case 14:
					case 15:
					case 16:
					case 17:
					case 18:
					case 19:
						if(Save()){
							switch(i%4){
								case 1:
									add=5;		break;
								case 2:
									add=20;		break;
								case 3:
									add=100;	break;
								default:
									add=1;
							}
							if(i<16)	add*=-1;
							mMapNo+=add;
							if(mMapNo<0)	mMapNo=0;
							LoadMap(mMapNo);
						}
						break;
					case 20:
						Save();
						break;
					case 21:
						memcpy(mUndoData[mUndoNo],mMapData,sizeof(mMapData));
						mUndoNo=(mUndoNo+1)%8;
						Reset();
						break;
					case 22:
						mBGColor++;
						mBGColor%=MAX_BG_COLOR;
						break;
					case 23:
						mBGColor--;
						if(mBGColor<0)	mBGColor+=MAX_BG_COLOR;
						break;
					case 24:
						MapShift(-1,0,0);	break;
					case 25:
						MapShift(0,-1,0);	break;
					case 26:
						MapShift(0,1,0);	break;
					case 27:
						MapShift(1,0,0);	break;
					case 28:
						MapShift(0,0,1);	break;
					case 29:
						MapShift(0,0,-1);	break;
					case 30:
						FuncMinimum(3,2,0);	break;
					case 31:
						FuncFromWB();		break;
					case 32:
						FuncToWB(1);		break;
					case 33:
						FuncToWB(2);		break;
					case 34:
						FuncToWB(100);		break;
				}
			}
		}
	}


	if(mPhotoMode){
		DTest.Printf(0,240,WIN_SIZE_Y,"フォトモード");
	}
	if(gZRock>=0){
		DTest.Printf(0,140,WIN_SIZE_Y,"高さロック:%d",gZRock-1);
		if(DTest.GetKey(DIK_S)==1&&cursorz<MAP_SIZE_Z-2){
			gZRock++;
			gCameray-=gMapChipSize/2;
		}else if(DTest.GetKey(DIK_A)==1&&gZRock>1){
			gZRock--;
			gCameray+=gMapChipSize/2;
		}
	}
	DTest.Printf(0,90,WIN_SIZE_Y+32,"SizeX:%d",mMapSizeX-1);
	DTest.Printf(0,180,WIN_SIZE_Y+32,"SizeY:%d",mMapSizeY-1);
	DTest.Printf(0,270,WIN_SIZE_Y+170,"背景:%d",mBGColor);
	DTest.Printf(0,WIN_SIZE_X-300,WIN_SIZE_Y+100,"page\n %d",gPalettePage);

	char text[128];
	sprintf(text,"MapNo:%d",mMapNo);
	DTest.DrawText(0,text,82,WIN_SIZE_Y+170,0xff000000,DT_CENTER,98);


	if(!(0<=mousex && mousex<=WIN_SIZE_X && 0<=mousey && mousey<=WIN_SIZE_Y))return;
	int cameraspeed=8;
	if(DTest.GetKey(DIK_SPACE))	cameraspeed=4;
	if(mousex+96>WIN_SIZE_X)	gCamerax+=(mousex+96-WIN_SIZE_X)/cameraspeed;
	if(mousex-96<0)				gCamerax+=(mousex-96)/cameraspeed;
	if(mousey+96>WIN_SIZE_Y)	gCameray+=(mousey+96-WIN_SIZE_Y)/cameraspeed;
	if(mousey-96<0)				gCameray+=(mousey-96)/cameraspeed;

	if(cursorx<0){
		DTest.Printf(0,0,WIN_SIZE_Y,"ｶﾒﾗ x:%d y:%d",gCamerax,gCameray);
		return;
	}
	if(cursorz<=0)	cursorz=1;


	int dx,dy;
	dx=(cursorx-cursory)*gMapChipSize-gCamerax;
	dy=(cursorx+cursory-cursorz)*(gMapChipSize/2)-gCameray;
	DTest.DrawLine(dx+gMapChipSize,dy,dx,dy+gMapChipSize/2,2,0xffff0000);
	DTest.DrawLine(dx+gMapChipSize,dy,dx+gMapChipSize*2,dy+gMapChipSize/2,2,0xffff0000);
	DTest.DrawLine(dx,dy+gMapChipSize/2,dx+gMapChipSize,dy+gMapChipSize,2,0xffff0000);
	DTest.DrawLine(dx+gMapChipSize,dy+gMapChipSize,dx+gMapChipSize*2,dy+gMapChipSize/2,2,0xffff0000);

	DTest.Printf(0,0,WIN_SIZE_Y,"x:%d y:%d z:%d",cursorx,cursory,cursorz);

	if(leftbutton==1 ||((leftbutton>=16||gZRock>=0) && leftbutton%4==1)){
		if(!mUndoSaved){
			memcpy(mUndoData[mUndoNo],mMapData,sizeof(mMapData));
			mUndoNo=(mUndoNo+1)%8;
			mUndoSaved=true;
		}
		int palette=gPalette;
		int z=cursorz;
		int zmove=0;
		if(DTest.GetKey(DIK_X)&&cursorz<MAP_SIZE_Z-2){
			z++;
			zmove=-1;
		}else if(DTest.GetKey(DIK_Z)&&cursorz>=1){
			palette=0;
		}

		if(DTest.GetKey(DIK_LSHIFT)||DTest.GetKey(DIK_RSHIFT)){
			Mennuri(cursorx,cursory,z,palette,zmove);
		}else{
			mMapData[cursorx][cursory][z]=palette;
		}
	}else if(leftbutton<0){
		mUndoSaved=false;
	}
	if(rightbutton==1){
		gPalette=mMapData[cursorx][cursory][cursorz];
	}
}
void CMapMode::Mennuri(int x,int y,int z,int pallete,int zmove)
{
	bool draw[MAP_SIZE_X][MAP_SIZE_Y];
	memset(draw,0,sizeof(draw));

	Mennuri(x,y,z,mMapData[x][y][z+zmove],pallete,zmove,draw);
}
void CMapMode::Mennuri(int x,int y,int z,int before,int after,int zmove,bool draw[MAP_SIZE_X][MAP_SIZE_Y])
{
	if(x>mMapSizeX-1)					return;
	if(x<=0)							return;
	if(y>mMapSizeY-1)					return;
	if(y<=0)							return;
	if(draw[x][y])						return;
	if(mMapData[x][y][z+zmove]!=before)	return;

	mMapData[x][y][z]=after;
	draw[x][y]=true;
	Mennuri(x+1,y,z,before,after,zmove,draw);
	Mennuri(x-1,y,z,before,after,zmove,draw);
	Mennuri(x,y+1,z,before,after,zmove,draw);
	Mennuri(x,y-1,z,before,after,zmove,draw);
}
void CMapMode::MapShift(int sx,int sy,int sz)
{
	memcpy(mUndoData[mUndoNo],mMapData,sizeof(mMapData));
	mUndoNo=(mUndoNo+1)%8;

	static int data[MAP_SIZE_X][MAP_SIZE_Y][MAP_SIZE_Z];
	memcpy(data,mMapData,sizeof(data));

	for(int x=0;x<mMapSizeX;x++){
		for(int y=0;y<mMapSizeY;y++){
			for(int z=0;z<MAP_SIZE_Z;z++){
				mMapData[x][y][z]=data[x-sx][y-sy][z-sz];
			}
		}
	}
}
void CMapMode::FuncMinimum(int before,int after,int zure)
{
	memcpy(mUndoData[mUndoNo],mMapData,sizeof(mMapData));
	mUndoNo=(mUndoNo+1)%8;

	static int data[MAP_SIZE_X][MAP_SIZE_Y][MAP_SIZE_Z];
	memcpy(data,mMapData,sizeof(data));

	int nx=0;
	for(int x=0;x<mMapSizeX;x++){
		int ny=0;
		for(int y=0;y<mMapSizeY;y++){
			for(int z=0;z<MAP_SIZE_Z;z++){
				mMapData[x][y][z]=data[(x+zure)*before/after-zure][(y+zure)*before/after-zure][z];
			}
		}
	}
}
void CMapMode::FuncToWB(int pz)
{
	memcpy(mUndoData[mUndoNo],mMapData,sizeof(mMapData));
	mUndoNo=(mUndoNo+1)%8;

	static int data[MAP_SIZE_X][MAP_SIZE_Y][MAP_SIZE_Z];
	memcpy(data,mMapData,sizeof(data));

	for(int x=0;x<mMapSizeX;x++){
		for(int y=0;y<mMapSizeY;y++){
			for(int z=0;z<MAP_SIZE_Z;z++){
				if(mMapData[x][y][z]==500){
					mMapData[x][y][z]=501+((x+y+(z)/pz)%2);
				}
			}
		}
	}
}
void CMapMode::FuncFromWB(void)
{
	memcpy(mUndoData[mUndoNo],mMapData,sizeof(mMapData));
	mUndoNo=(mUndoNo+1)%8;

	static int data[MAP_SIZE_X][MAP_SIZE_Y][MAP_SIZE_Z];
	memcpy(data,mMapData,sizeof(data));

	for(int x=0;x<mMapSizeX;x++){
		for(int y=0;y<mMapSizeY;y++){
			for(int z=0;z<MAP_SIZE_Z;z++){
				if(mMapData[x][y][z]==501 || mMapData[x][y][z]==502){
					mMapData[x][y][z]=500;
				}
			}
		}
	}
}
bool CMapMode::Save(void)
{
	FILE *pFile;
	char filename[256];
	sprintf(filename,"..\\..\\data\\mapdata\\mapdata%04d",mMapNo);
	pFile=fopen(filename,"wb");
	if(!pFile){
		PlaySound("..\\error.wav",NULL,SND_ASYNC);
		return false;
	}

//新形式
	int ver=-1;
	fwrite(&ver,1,sizeof(int),pFile);
	fwrite(&mMapSizeX,1,sizeof(int),pFile);
	fwrite(&mMapSizeY,1,sizeof(int),pFile);
	fwrite(&mBGColor,1,sizeof(int),pFile);

	short chip=-1;
	short len=0;
	for(int z=1;z<MAP_SIZE_Z;z++){
		for(int y=1;y<mMapSizeY;y++){
			for(int x=1;x<mMapSizeX;x++){
				bool write=false;
				if(chip!=short(mMapData[x][y][z])){
					if(len>0){
						write=true;
					}else{
						len=1;
						chip=short(mMapData[x][y][z]);
					}
				}else{
					len++;
				}
				if(len>10000)	write=true;
				if(write){
					fwrite(&len,1,sizeof(short),pFile);
					fwrite(&chip,1,sizeof(short),pFile);
					len=1;
					chip=short(mMapData[x][y][z]);
				}
			}
		}
	}
	fwrite(&len,1,sizeof(short),pFile);
	fwrite(&chip,1,sizeof(short),pFile);
//ここまで
	fclose(pFile);

	pFile=fopen("dat","wb");
	if(pFile){
		_putw(mMapNo,pFile);
		fclose(pFile);
	}
	return true;
}
