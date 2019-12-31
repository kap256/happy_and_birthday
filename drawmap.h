
bool CMapMode::LoadMapData(char* fname)
{
#ifdef DRAW_MAP_RPG
	const int z_start=0;
	const int z_end=MAP_SIZE_Z-1;
#endif
#ifdef DRAW_MAP_EDITER
	const int z_start=1;
	const int z_end=MAP_SIZE_Z;
#endif
	FILE *pFile;
	pFile=fopen(fname,"rb");
	if(pFile){
		int version;

		fread(&version,1,sizeof(int),pFile);
		if(version>=0){
//旧形式
			mMapSizeX=version;
			fread(&mMapSizeY,1,sizeof(int),pFile);
			fread(&mBGColor,1,sizeof(int),pFile);

			for(int x=1;x<mMapSizeX;x++){
				for(int y=1;y<mMapSizeY;y++){
					for(int z=z_start;z<z_end;z++){
						fread(&mMapData[x][y][z],1,sizeof(int),pFile);
					}
				}
			}
		}else{
//新形式
			fread(&mMapSizeX,1,sizeof(int),pFile);
			fread(&mMapSizeY,1,sizeof(int),pFile);
			fread(&mBGColor,1,sizeof(int),pFile);

			short chip=-1;
			short len=0;
			for(int z=z_start;z<z_end;z++){
				for(int y=1;y<mMapSizeY;y++){
					for(int x=1;x<mMapSizeX;x++){
						while(len<=0){
							fread(&len,1,sizeof(short),pFile);
							fread(&chip,1,sizeof(short),pFile);
							if(feof(pFile)){
								chip=20;
								len=10000;
							}
						}
						mMapData[x][y][z]=int(chip);
						len--;
					}
				}
			}
		}
		fclose(pFile);
		return true;
	}
	return false;
}



#ifdef DRAW_MAP_RPG
void CMapMode::DrawMap(int camerax,int cameray)
#endif

#ifdef DRAW_MAP_EDITER
void CMapMode::DrawMap(int camerax,int cameray,int mousex,int mousey,int &cx,int &cy,int &cz)
#endif
{
	int SYSTEM_CHIP = 20;
#ifdef DRAW_MAP_RPG
#ifndef _OUTPUT_MAP_BMP_
	if(mBGColor>=MAX_BG_COLOR){
		DTest.GetDeveice()->SetSamplerState(0,D3DSAMP_ADDRESSU,D3DTADDRESS_MIRROR);
		DTest.GetDeveice()->SetSamplerState(0,D3DSAMP_ADDRESSV,D3DTADDRESS_MIRROR);
		DTest.DrawTexture(mBGImg,0,Flag.mPlayTime/4,Flag.mPlayTime/4,320,240,0,0,640,480);
		DTest.GetDeveice()->SetSamplerState(0,D3DSAMP_ADDRESSU,D3DTADDRESS_WRAP);
		DTest.GetDeveice()->SetSamplerState(0,D3DSAMP_ADDRESSV,D3DTADDRESS_WRAP);
	}else{
		DTest.DrawBox(0,0,640,480,
			cMapBGColor[mBGColor][0],cMapBGColor[mBGColor][0],cMapBGColor[mBGColor][1],cMapBGColor[mBGColor][1]);
	}
#endif
	int draw_obj_start=0;
#ifdef _HIDDEN_PLAYER_
	draw_obj_start=1;
#endif
#ifdef _OUTPUT_MAP_BMP_
	draw_obj_start=1;
#endif

	int allobject=0;
	CMapObject*		obj[OBJECT_NO];
	unsigned int	objz[OBJECT_NO];
	memset(obj,0,sizeof(obj));
	for(int n=draw_obj_start;n<OBJECT_NO;n++){
		if(mObject[n]){
			obj[allobject]=mObject[n];
			objz[allobject]=obj[allobject]->GetDrawZ();
			allobject++;
		}
	}
	bool turn=true;
	while(turn){
		turn=false;
		for(int n=1;n<allobject;n++){
			if(objz[n]<objz[n-1]){
				turn=true;
				unsigned int z=objz[n];
				objz[n]=objz[n-1];
				objz[n-1]=z;
				CMapObject* o=obj[n];
				obj[n]=obj[n-1];
				obj[n-1]=o;
			}
		}
	}


	CMapObject*		sobj[OBJECT_NO];
	unsigned int	sobjz[OBJECT_NO];
	int				sobjno=0;
	memset(sobj,0,sizeof(sobj));
	for(int n=draw_obj_start;n<OBJECT_NO;n++){
		if(mObject[n]){
			sobj[sobjno]=mObject[n];
			sobjz[sobjno]=sobj[sobjno]->GetShadowZ(this);
			sobjno++;
		}
	}
	turn=true;
	while(turn){
		turn=false;
		for(int n=1;n<allobject;n++){
			if(sobjz[n]<sobjz[n-1]){
				turn=true;
				unsigned int z=sobjz[n];
				sobjz[n]=sobjz[n-1];
				sobjz[n-1]=z;
				CMapObject* o=sobj[n];
				sobj[n]=sobj[n-1];
				sobj[n-1]=o;
			}
		}
	}



	int objno=0;
		sobjno=0;
#endif
		
#ifdef DRAW_MAP_EDITER
	if(!mPhotoMode){
		SYSTEM_CHIP = 1;
		DTest.DrawLine((2-mMapSizeY)*gMapChipSize-gCamerax,
						(mMapSizeY)*gMapChipSize/2-gCameray,
						(mMapSizeX-mMapSizeY+1)*gMapChipSize-gCamerax,
						(mMapSizeX+mMapSizeY-1)*(gMapChipSize/2)-gCameray,2,0xff800000);
		DTest.DrawLine((mMapSizeX)*gMapChipSize-gCamerax,
						(mMapSizeX)*gMapChipSize/2-gCameray,
						(mMapSizeX-mMapSizeY+1)*gMapChipSize-gCamerax,
						(mMapSizeX+mMapSizeY-1)*(gMapChipSize/2)-gCameray,2,0xff800000);
		DTest.DrawLine(	-gCamerax+gMapChipSize,
						gMapChipSize/2-gCameray,
						(mMapSizeX)*gMapChipSize-gCamerax,
						(mMapSizeX)*gMapChipSize/2-gCameray,2,0xff800000);
		DTest.DrawLine(	-gCamerax+gMapChipSize,
						gMapChipSize/2-gCameray,
						(2-mMapSizeY)*gMapChipSize-gCamerax,
						(mMapSizeY)*gMapChipSize/2-gCameray,2,0xff800000);
	}

	int MAP_CHIP_SIZE=gMapChipSize;
#endif

	int i,j;
	int x,y,z;
	for(i=0;i<MAP_SIZE_Y+MAP_SIZE_X-2;i++){
			
#ifdef DRAW_MAP_RPG
		for(z=1;z<MAP_SIZE_Z-1;z++){
			while((unsigned(i*0x010000+z*0x0100)>=sobjz[sobjno]/0x100)&&(sobjno<allobject)){
				if(z!=1)	sobj[sobjno]->DrawShadow(camerax,cameray,z);
				sobjno++;
			}
			while((unsigned(i*0x010000+z*0x0100)>=objz[objno]/0x100)&&(objno<allobject)){
				//DTest.SetFilter( D3DTEXF_LINEAR );
				obj[objno]->OnDraw(camerax,cameray);
				objno++;
				//DTest.SetFilter( D3DTEXF_POINT );
			}
#endif
#ifdef DRAW_MAP_EDITER
		for(z=1;z<MAP_SIZE_Z;z++){
#endif
			for(j=0;j<i;j++){
				x=j;
				y=i-j;
				if(x>mMapSizeX-1)	continue;
				if(y>mMapSizeY-1)	continue;
				if(x<1)				continue;
				if(y<1)				continue;
				bool t=false;
				bool top=false;
				double dx,dy;
				dx=(x-y)*MAP_CHIP_SIZE-camerax;
				dy=(x+y-z)*(MAP_CHIP_SIZE/2)-cameray;
				if(mMapData[x][y][z]>=SYSTEM_CHIP){
					int chip=mMapData[x][y][z]%20;
					int page=mMapData[x][y][z]/20;
#ifdef DRAW_MAP_RPG
					if(MapChipParam[page] & MCHIP_ANIMATION_FAST){
						int n=(chip+(mTime/8))%4;
						chip=chip/4*4+n;
					}else	if(MapChipParam[page] & MCHIP_ANIMATION){
						int n=(chip+(mTime/20))%4;
						chip=chip/4*4+n;
					}
#endif
#ifdef _OUTPUT_MAP_BMP_
					{
#else
					if(-MAP_CHIP_SIZE*2<=dx&&dx<=640&&-MAP_CHIP_SIZE*2<=dy&&dy<=480){
#endif
						if((GetChipType(x,y,z+1) & MCHIP_NO_VIEW_OBST)){
							t=true;
							top=true;
						}else	if((GetChipType(x,y+1,z) & MCHIP_NO_VIEW_OBST)||(GetChipType(x+1,y,z) & MCHIP_NO_VIEW_OBST)){
							t=true;
						}
						if(t){
#ifdef DRAW_MAP_RPG
							DTest.DrawTexture(mMapChipImg[page-1],0,(chip%4)*64,(chip/4)*48,64,48,dx,dy,MAP_CHIP_SIZE*2,MAP_CHIP_SIZE*3/2);
#endif
#ifdef DRAW_MAP_EDITER
							D3DCOLOR color=0xffffffff;
							if(mMapData[x][y][z]<20)	color=0x80ffffff;
							DTest.DrawTexture(mMapChipImg[page],0,(chip%4)*64,(chip/4)*48,64,48,dx,dy,MAP_CHIP_SIZE*2,MAP_CHIP_SIZE*3/2,color);
#endif
							if(top && (!(MapChipParam[page] & MCHIP_NO_BORDER) || (MapChipParam[page] & MCHIP_GET_SHADOW))){
								for(int i=z+1;i<MAP_SIZE_Z-1;i++){
									if(!(GetChipType(x,y,i) & MCHIP_NO_SHADOW)){
										DTest.DrawBox(
														int(dx+MAP_CHIP_SIZE)	,int(dy+MAP_CHIP_SIZE),
														int(dx)					,int(dy+MAP_CHIP_SIZE/2),
														int(dx+MAP_CHIP_SIZE*2)	,int(dy+MAP_CHIP_SIZE/2),
														int(dx+MAP_CHIP_SIZE)	,int(dy)				,
														COLOR_SHADOW,COLOR_SHADOW,COLOR_SHADOW,COLOR_SHADOW);
										break;
									}
								}
							}

#ifdef DRAW_MAP_RPG
							if(!(MapChipParam[page] & MCHIP_NO_BORDER)){
#endif
#ifdef DRAW_MAP_EDITER
							if(!(MapChipParam[page] & MCHIP_NO_BORDER) && !mPhotoMode){							
#endif
								const D3DCOLOR LINE_COLOR = 0x80000000;
								
								if(top){
									if(GetChipType(x-1,y,z)&MCHIP_NO_BORDER){
										//上面左上
										DTest.DrawLine(dx+MAP_CHIP_SIZE,dy,dx,dy+MAP_CHIP_SIZE/2,LINE_WIDTH,LINE_COLOR);
									}
									if(GetChipType(x,y-1,z)&MCHIP_NO_BORDER){
										//上面右上
										DTest.DrawLine(dx+MAP_CHIP_SIZE,dy,dx+MAP_CHIP_SIZE*2,dy+MAP_CHIP_SIZE/2,LINE_WIDTH,LINE_COLOR);
									}
								}
								if(GetChipType(x,y+1,z)&MCHIP_NO_BORDER){
									if(GetChipType(x,y,z+1)&MCHIP_NO_BORDER){
										//上面左下
										DTest.DrawLine(dx,dy+MAP_CHIP_SIZE/2,dx+MAP_CHIP_SIZE,dy+MAP_CHIP_SIZE,LINE_WIDTH,LINE_COLOR);
									}
									if(!(GetChipType(x-1,y+1,z)&MCHIP_NO_BORDER) | (GetChipType(x-1,y,z)&MCHIP_NO_BORDER)){
										//側面左
										DTest.DrawLine(dx,dy+MAP_CHIP_SIZE/2,dx,dy+MAP_CHIP_SIZE+2,LINE_WIDTH,LINE_COLOR);
									}
									if(GetChipType(x,y,z-1)&MCHIP_NO_BORDER){
										//底面左
										DTest.DrawLine(dx,dy+MAP_CHIP_SIZE,dx+MAP_CHIP_SIZE,dy+MAP_CHIP_SIZE*3/2,LINE_WIDTH,0xff000000);
									}
								}
								if(GetChipType(x+1,y,z)&MCHIP_NO_BORDER){
									if(GetChipType(x,y,z+1)&MCHIP_NO_BORDER){
										//上面右下
										DTest.DrawLine(dx+MAP_CHIP_SIZE,dy+MAP_CHIP_SIZE,dx+MAP_CHIP_SIZE*2,dy+MAP_CHIP_SIZE/2,LINE_WIDTH,LINE_COLOR);
									}
									if(!(GetChipType(x+1,y-1,z)&MCHIP_NO_BORDER) | (GetChipType(x,y-1,z)&MCHIP_NO_BORDER)){
										//側面右
										DTest.DrawLine(dx+MAP_CHIP_SIZE*2,dy+MAP_CHIP_SIZE/2,dx+MAP_CHIP_SIZE*2,dy+MAP_CHIP_SIZE,LINE_WIDTH,LINE_COLOR);
									}
									if(GetChipType(x,y,z-1)&MCHIP_NO_BORDER){
										//底面右
										DTest.DrawLine(dx+MAP_CHIP_SIZE,dy+MAP_CHIP_SIZE*3/2,dx+MAP_CHIP_SIZE*2,dy+MAP_CHIP_SIZE,LINE_WIDTH,LINE_COLOR);
									}
								}
								if((GetChipType(x,y+1,z)&MCHIP_NO_BORDER)&&(GetChipType(x+1,y,z)&MCHIP_NO_BORDER)){
									//側面中央
									DTest.DrawLine(dx+MAP_CHIP_SIZE,dy+MAP_CHIP_SIZE,dx+MAP_CHIP_SIZE,dy+MAP_CHIP_SIZE*3/2,LINE_WIDTH,LINE_COLOR);
								}
							}
						}
					}
				}
#ifdef DRAW_MAP_EDITER
				if(gZRock>=0){
					if(z==gZRock){
						t=true;
						top=true;
					}else{
						t=false;
						top=false;
					}
				}
				if(	t
				&&	dx+dy*2+gMapChipSize<=mousex+mousey*2
				&&	dx-dy*2+gMapChipSize>mousex-mousey*2
				&&	dx+dy*2+gMapChipSize*3>mousex+mousey*2
				&&	dx-dy*2-gMapChipSize<=mousex-mousey*2){
					if(top){
						cx=x;
						cy=y;
						cz=z;
					}else{
						cx=-1;
					}
				}
#endif
			}
		}
	}
}