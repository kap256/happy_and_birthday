#include "mapobject.h"
#include "mapmode.h"
#include "input.h"
#include "flag.h"
#include "menudata.h"
#include "changemode.h"
#include "option.h"

void CMapObject::GetComp(int dir,int* x,int* y)
{
	if(dir<0){
		dir=dir%4+4;
	}else{
		dir=dir%4;
	}
	switch(dir){
		case 0:
			*x=-1;	*y=0;	return;
		case 1:
			*x=0;	*y=-1;	return;
		case 2:
			*x=1;	*y=0;	return;
		case 3:
			*x=0;	*y=1;	return;
	}
}

CMapObject::CMapObject(void){
	mID=-1;
	mView=true;
}
CMapObject::~CMapObject(void){
}

void CMapObject::InitObject(int id,int x,int y,int z,int dir,int speed,int movefreq
							,int img,bool hit,int gravity,int param)
{
	mID=id;
	mSpeed=speed;
	mMoveFreq=movefreq;
	mShowNo=param%64;
	mGravity=gravity;
	mHit=hit;
	mParam=param;
	LoadImage(img);

	mX=x*32;
	mY=y*32;
	mZ=z*16;

	if(mMoveFreq>0)		mWalkWait=(pInput->Rand(512))/mMoveFreq;

	mSpeedZ=0;
	mGround=true;
	mGoalX=x;
	mGoalY=y;
	mGoalZ=z;
	mWalkTimer=0;
	mFloatTimer=0;
	mSki=0;

	mNextDir=-1;
	mDir=dir%GetMaxDir();

	mChangeAni=true;
	mAniNo=0;

	mStop=false;
}
void CMapObject::LoadImage(int no)
{
	DTest.DrawSkip(false);

	char filename[256];
	sprintf(filename,"data\\mapobject\\mapobject%03d.bmp",no);
	DTest.LoadTexture(mImg,filename,0xff008080);
#ifdef _SHOW_OBJECT_IMG_NO_
	mDebugImageNo = no;
#endif
}
void CMapObject::GetDrawInfo(int &x,int &y,int &tx,int &ty,int &turn)
{
	GetDrawXY(x,y);

	bool t=false;
	turn=0;
	tx;
	ty;
	if(mDir==1 || mDir==2)			t=!t;
	if(mParam & OBJOPT_SHOW_TURN)	t=!t;
	if(t)	turn=TURN_HORIZONTAL;

	ty=mAniNo%4;
	tx=mAniNo/4*2;
	if(mDir<=1)		tx++;
}
void CMapObject::GetDrawXY(int &drawx,int &drawy)
{
	int x,y;
	if(mParam & OBJOPT_WALL){
		int dir=(mParam&OBJOPT_WALL)/OBJOPT_WALL_0-1;
		GetComp(dir,&x,&y);
		x*=16;
		y*=16;
	}else{
		x=0;
		y=0;
	}
	x+=(int)mX;
	y+=(int)mY;

	drawx=x-y;
	drawy=(x+y)/2-GetZ()-16;
	drawx=int((drawx*MAP_CHIP_SIZE)/32);
	drawy=int((drawy*MAP_CHIP_SIZE)/32);
}
void CMapObject::OnDraw(int cx,int cy)
{
	if(mID<0)	return;
	if(!mView)	return;
	int drawx;
	int drawy;

	int turn;
	int tx;
	int ty;

	GetDrawInfo(drawx,drawy,tx,ty,turn);
	drawx-=cx;
	drawy-=cy;

	DTest.DrawTexture(mImg,turn,tx*64,ty*64,64,64,drawx,drawy,MAP_CHIP_SIZE*2,MAP_CHIP_SIZE*2);
	if(0<mFloatTimer && mFloatTimer<GRAV_FLOAT_TIME){
		DTest.SetBlend(BLEND_ADD);
		const D3DCOLOR COLOR=0xff0000ff;
		DTest.DrawTexture(mImg,turn,tx*64,ty*64,64,64,drawx,drawy,MAP_CHIP_SIZE*2,MAP_CHIP_SIZE*2,COLOR);
		DTest.SetBlend(BLEND_STANDARD);
	}

#ifdef _DEBUG
	DrawDebugData(drawx,drawy);
#endif
}
void CMapObject::DrawShadow(int cx,int cy,int z)
{
	if(mParam & OBJOPT_NO_SHADOW)	return;
	if(mID<0)	return;
	if(!mView)	return;
	int drawx;
	int drawy;
	GetDrawXY(drawx,drawy);
	z=z-2;
	drawx-=cx;
	drawy=int(drawy-cy+((mZ-z*16)*MAP_CHIP_SIZE)/32);

	if(mParam & OBJOPT_BLOCK_SHADOW){
		DTest.DrawBox(
				int(drawx+MAP_CHIP_SIZE)	,int(drawy+MAP_CHIP_SIZE),
				int(drawx)					,int(drawy+MAP_CHIP_SIZE/2),
				int(drawx+MAP_CHIP_SIZE*2)	,int(drawy+MAP_CHIP_SIZE/2),
				int(drawx+MAP_CHIP_SIZE)	,int(drawy)					,
				COLOR_SHADOW,COLOR_SHADOW,COLOR_SHADOW,COLOR_SHADOW);
	}else{
		int size;
		D3DCOLOR color;
		if(mParam & OBJOPT_LIGHT_SHADOW){
			size=pInput->Rand(15)-7;
			color=COLOR_SHADOW|0x00ff8000;
		}else if(mParam & OBJOPT_BLUE_SHADOW){
			color=0x808080ff;
			size=8;
		}else if(mParam & OBJOPT_SMALL_SHADOW){
			color=COLOR_SHADOW;
			size=-30;
		}else{
			color=COLOR_SHADOW;
			size=0;
		}
		DTest.DrawTexture(SystemTex,0,
							32,32,
							32,32,
							drawx+(MAP_CHIP_SIZE/2)-size/2,drawy-size/2,
							MAP_CHIP_SIZE+size,MAP_CHIP_SIZE+size,
							color);
	}
}
int CMapObject::OnTimer(int mapsizex,int mapsizey,CMapMode *map,CMapObject* object[OBJECT_NO])
{
	if(mID<0)	return 0;
	int ret=0;

	bool ground=false;
	int bgrav=mGravity&BASE_GRAV;

	if(bgrav==GRAV_NONE || bgrav==GRAV_AIR){
		double spd;
		if(bgrav==GRAV_NONE){
			spd=3.2;
		}else{
			spd=-32.0/mSpeed;
		}
		if(mWalkTimer<=0){
			mSpeedZ+=spd/16;
			mGround=false;
			if(mSpeedZ>spd)		mSpeedZ=spd;
		}else{
			mGround=false;
			if(mGoalZ*16+15>mZ && bgrav==GRAV_AIR ){
				mSpeedZ-=spd/16;
				if(mSpeedZ>-spd)		mSpeedZ=-spd;
			}else if(mGoalZ*16+30<mZ && bgrav==GRAV_AIR ){
				mSpeedZ+=spd/16;
				if(mSpeedZ<spd)		mSpeedZ=spd;
			}else{
				mSpeedZ-=0.20;
				if(mSpeedZ<0){
					mSpeedZ=0;
				}
			}
		}
	}else if(bgrav==GRAV_FALL_NONE){
		mGround=false;
		mSpeedZ-=0.10;
		if(mSpeedZ<-8)		mSpeedZ=-8;
	}else if(bgrav==GRAV_LOCK){
		mGround=true;
		mZ+=mSpeedZ;
		mSpeedZ=0;
	}else{
		mSpeedZ-=0.50;
		if(mSpeed<32){
			mSpeedZ-=2.0/mSpeed;
		}
		mGround=false;
		if(mSpeedZ<-8)		mSpeedZ=-8;
	}

	mZ+=mSpeedZ;

	if(mZ<0){
		MapMode.SendMessage(EH_FALL,mID,-1);
	}
	if(!mGround){
		int gravityx=GetX_32();
		int gravityy=GetY_32();
		if(mZ>=0){
			if(	(map->GetChipType(gravityx,gravityy,int(mZ)/16) & MCHIP_GROUND) &&mSpeedZ<0){
				mGround=true;
			}
			if(map->GetChipType(int(mX+16)/32,int(mY+16)/32,int(mZ)/16) & MCHIP_GROUND){
				mGround=true;
			}
		}
		if (!mGround && mHit){
			for(int ObjNo=0;ObjNo<OBJECT_NO;ObjNo++){
				if (ObjNo!=mID && object[ObjNo] && object[ObjNo]->CheckHit(this,false)>0){
					mGround=true;
					ret=ObjNo+1;
				}
			}
		}
		if(mGravity & GRAV_FLOAT){
			if(mGround){
				mFloatTimer=GRAV_FLOAT_TIME;
			}else if(mFloatTimer>0 && mSpeedZ<0){
				if(mFloatTimer%3==GRAV_FLOAT_TIME%3){
					PlayWave(SE_STAR);
				}
				mFloatTimer--;
				mGround=true;
			}
		}
		if(mGround){
			mZ=int(mZ)/16*16+16;
			mSpeedZ=0;
			ground=true;
		}
	}

	if(bgrav==GRAV_NONE || bgrav==GRAV_AIR || bgrav==GRAV_FALL_NONE)	mGround=true;


	if (mWalkTimer>0){
		int goalx=GetX_32()*32;
		int goaly=GetY_32()*32;

		mX+=(goalx-mX)/mWalkTimer;
		mY+=(goaly-mY)/mWalkTimer;
/*
		int x,y;
		GetComp(mDir,&x,&y);
		mX+=x*(32.0/mSpeed);
		mY+=y*(32.0/mSpeed);
/**/
		if(mChangeAni){
			if(mSki==1){
				mAniNo=1;
			}else if(mWalkTimer>=(mSpeed+1)/2){
				int n=GetX_32()+GetY_32();
				if(n%2==0)		mAniNo=1;
				else			mAniNo=3;
			}else{
				mAniNo=2;
			}
		}
		if(mWalkTimer==2)	MapMode.SendMessage(EH_WALK,mID,-1);
	}
	mWalkTimer--;
	if(mWalkTimer==0){	
		FixPos();
	}
	if (mWalkTimer<=0){
		if(mChangeAni){
			if(mSpeedZ<0){
				mAniNo=4+((unsigned int)(mZ)/16)%2;
			}else if(mWalkTimer<0){
				mAniNo=0;
			}
		}

		int x=mGoalX-GetX_32();
		int y=mGoalY-GetY_32();

		bool ski=false;
		int skix=GetX_32();
		int skiy=GetY_32();
		int skiz=GetZ_32();



		if(((x!=0)||(y!=0))&&(mGround)){

			if((x!=0)&&(y!=0)){
				int n=rand()%2;
				if(n==1)	x=0;
				else		y=0;
			}
			if (x!=0){
				if (x<0)	mDir=0;
				else		mDir=2;
			} else {
				if (y<0)	mDir=1;
				else		mDir=3;
			}
			int nx,ny,nz;
			GetComp(mDir,&nx,&ny);
			nx+=GetX_32();
			ny+=GetY_32();
			nz=int(mZ/16);

			if ((nx<=0)||(nx>=mapsizex)){
				mGoalX=GetX_32();
				mGoalY=GetY_32();
				return -8;
			} else	if ((ny<=0)||(ny>=mapsizey)){
				mGoalX=GetX_32();
				mGoalY=GetY_32();
				return -8;
			}

			int jump=0;
			if(bgrav==GRAV_JUMP || bgrav==GRAV_NONE || bgrav==GRAV_AIR)	jump=1;
			if(mGravity & GRAV_HI_JUMP)									jump*=2;
#ifdef _SUPER_JUMP_
			jump*=_SUPER_JUMP_;
#endif

			int space=0;
			int height=0;
			for(int i=0;i<3+jump;i++){
				if(map->GetChipType(GetX_32(),GetY_32(),nz+i) & MCHIP_FENCE){
					int chip=map->GetChip(GetX_32(),GetY_32(),nz+i);
					if(chip%4==mDir){
						space=0;
						break;
					}
				}
				if(map->GetChipType(nx,ny,nz+i) & MCHIP_FENCE){
					int chip=map->GetChip(nx,ny,nz+i);
					if((chip+2)%4==mDir){
						space=0;
						break;
					}
				}
				if(map->GetChipType(nx,ny,nz+i) & MCHIP_GROUND){
					space=0;
					height=i+1;
				}else{
					space++;
					if(space>=3)	break;
				}
			}
			if(space<3){
				mGoalX=GetX_32();
				mGoalY=GetY_32();
				return -4;
			}else if( height>0){
				mFloatTimer=0;
				if(bgrav==GRAV_JUMP){
					mSpeedZ=(3.5+mSpeed/16.0)*sqrt(double(height));
				}else{
					mSpeedZ=3.0;
				}
				nz+=height;
			}

			if (mHit){
				for(int ObjNo=0;ObjNo<OBJECT_NO;ObjNo++){
					if (object[ObjNo] && object[ObjNo]->CheckHit(this,true)>0){
						mGoalX=GetX_32();
						mGoalY=GetY_32();
						mSpeedZ=0.0;

						return ObjNo+1;
					}
				}
			}

			int h;
			for(h=nz;h>=0;h--){
				int chip=map->GetChip(nx,ny,h);
				int nomove;
				if(bgrav==GRAV_LOCK || bgrav==GRAV_NONE || bgrav==GRAV_AIR){
					nomove=MCHIP_NO_MOVE ^ MCHIP_NO_WALK;
				}else if(mGravity & GRAV_WATER){
					nomove=MCHIP_NO_MOVE ^ MCHIP_WATER;
				}else{
					nomove=MCHIP_NO_MOVE;
				}
				if(chip==1 || (MapChipParam[chip/20] & nomove)){
					mGoalX=GetX_32();
					mGoalY=GetY_32();
					mSpeedZ=0;

					return -4;
				}else if(MapChipParam[chip/20] & MCHIP_GROUND){
					if((mGravity & GRAV_WATER_SKI)==GRAV_WATER_SKI){
						if((MapChipParam[chip/20] & MCHIP_WATER)){
							ski=true;
							SetGoalDir(mDir,2);
						}
					}
					break;
				}
			}


			if(ground && (bgrav==GRAV_NONE || bgrav==GRAV_AIR)){
				mSpeedZ=3.0;
			}

			mWalkTimer=mSpeed;
			if(mChangeAni){
				if(mSki==1){
					mAniNo=1;
				}else{
					mAniNo=2;
				}
			}
		} else {
			ski=(mSki>0);
			if(!mStop){
				if(mWalkTimer==-1){
					MapMode.SendMessage(EH_GOAL,mID,-1);
					if(mSki){
						SetGoalDir(mDir,1);
					}
				}
				if(mMoveFreq>0){
					if(mWalkTimer==0){
						mWalkWait=(pInput->Rand(512))/mMoveFreq;
						mWalkTimer=-1;
					}
					mWalkWait--;
					if (mWalkWait<=0){
						SetGoalDir(pInput->Rand(4),1);
						x=mGoalX-GetX_32();
						y=mGoalY-GetY_32();
					}
				}
				mWalkTimer--;
			}

			if(mNextDir>=0){
				mDir=mNextDir;
				mNextDir=-1;
			}
		}
		int chip=map->GetChip(GetX_32(),GetY_32(),GetZ_32()-1);
		if((MapChipParam[chip/20] & MCHIP_CONVEYER)){
			ski=true;
			int x,y;
			GetComp(((chip%20)/4)%4,&x,&y);
			x+=GetX_32();
			y+=GetY_32();
			SetGoal(x,y,GetZ_32());
		}

		if(ski){
			if(mSki<=0){
				mSki=1;
				mSkiStartX=skix;
				mSkiStartY=skiy;
				mSkiStartZ=skiz;
			}
		}else{
			if(mSki<=1){
				mSki=0;
			}
		}
	}
	if (mHit){
		for(int ObjNo=0;ObjNo<OBJECT_NO;ObjNo++){
			object[ObjNo] && object[ObjNo]->CheckHit(this,false);
		}
	}

	return ret;
}
bool CMapObject::Move(double x,double y)
{
	bool oldgoal=(mGoalX==int(mX+16)/32 && mGoalY==int(mY+16)/32 );
	mX+=x;
	mY+=y;
	if(mWalkTimer<=0 && oldgoal){
		mGoalX=int(mX+16)/32;
		mGoalY=int(mY+16)/32;
	}
	return true;
}

void CMapObject::SetPos(int x,int y,int z)
{
	mX=x*32;
	mY=y*32;
	mZ=z*16;
	mWalkTimer=0;
	SetGoal(x,y,z);
}
void CMapObject::SetDir(int dir)
{
	if(CheckGoal()){
		mDir=dir%GetMaxDir();
	}else{
		mNextDir=dir%GetMaxDir();
	}
}
void	CMapObject::SetSpeed(int speed)
{
	double ratio=double(speed)/mSpeed;
	mSpeed=speed;
	mWalkTimer=int(mWalkTimer*ratio);
}

void	CMapObject::SetZSpeed(double zspeed)
{
	mSpeedZ=zspeed;
}
int		CMapObject::SetGoal(int x,int y,int z)
{
	mGoalX=x;
	mGoalY=y;
	mGoalZ=z;
	return 0;
}
int		CMapObject::SetGoalDir(int dir,int l)
{
	int nx,ny;
	GetComp(dir,&nx,&ny);
	nx*=l;
	ny*=l;
	nx+=GetX_32();
	ny+=GetY_32();
	return SetGoal(nx,ny,GetZ_32());
}
void	CMapObject::SetStop(bool stop)
{
	mStop=stop;
	if(stop){
		SetGoal(GetX_32(),GetY_32(),GetZ_32());
	}
}

bool CMapObject::CheckGoal(void)
{
	if(mWalkTimer>1)		return false;
	if(mGoalX!=GetX_32())	return false;
	if(mGoalY!=GetY_32())	return false;
	if(!mGround)			return false;
	return true;
}

int CMapObject::CheckHit(CMapObject* obj,bool walk)
{
	int ret=-1;
	int x,y;
	int z=obj->GetZ();
	if(mHit){
		x=obj->GetX_32();
		y=obj->GetY_32();
		ret=1;
		if(walk){
			int dx,dy;
			GetComp(obj->GetDir(),&dx,&dy);
			x+=dx;
			y+=dy;
		}
	}else{
		x=(obj->GetX()+16)/32;
		y=(obj->GetY()+16)/32;
	}
	bool hit=true;

	if(x!=GetX_32())	return 0;
	if(y!=GetY_32())	return 0;

	if(z<=mZ-32)		return 0;
	if(z>=mZ+32)		return 0;

	int i=mID;
	int j=obj->GetID();
	if(i>j)	{int k=i;i=j;j=k;}
	if(i!=j){
		MapMode.SendMessage(EH_HIT,i,j);
	}
	return ret;

}
bool CMapObject::CheckTalk(CMapObject* obj)
{
	if(CheckHit(obj,true)!=0){
		return (MapMode.SendMessage(EH_TALK,mID,-1)>0);
	}
	return false;

}
unsigned int CMapObject::GetDrawZ(void)
{
	return (int(mX)+int(mY)+31)/32*0x01000000+(int(mZ))/16*0x010000+ 64-(int)(mX)%32-(int)(mY)%32+mShowNo*64;
}

unsigned int CMapObject::GetShadowZ(CMapMode *mmode)
{
	int z=int(mZ/16);
	int mapx=int(mX+16)/32;
	int mapy=int(mY+16)/32;

	while(z>0){
		if(!(mmode->GetChipType(mapx,mapy,z) & MCHIP_THRU_SHADOW)){
			break;
		}
		z--;
	}
	return (int(mX)+int(mY)+31)/32*0x01000000+(z+1)*0x010000;
}
int  CMapObject::GetX_32(void)
{
	double nx=mX;

	if (mWalkTimer>0){
		int x,y;
		GetComp(mDir,&x,&y);
		nx+=x*(32.0/mSpeed)*mWalkTimer;
	}
	return int(nx+16)/32;
}
int  CMapObject::GetY_32(void)
{
	double ny=mY;
	if (mWalkTimer>0){
		int x,y;
		GetComp(mDir,&x,&y);
		ny+=y*(32.0/mSpeed)*mWalkTimer;
	}
	return int(ny+16)/32;
}
int  CMapObject::GetZ_32(void)
{
	return int(mZ+8)/16;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
CPlayerObject::CPlayerObject(void)
{
	mDash=false;
	mEscape=0;
	mActionTime=0;
	mActionType=0;
	mBaseSpeed=-1;
}
void CPlayerObject::SetDash(bool dash){
const double DUSH_SPEED=16.0/11.0;
const double SDUSH_SPEED=16.0/6.0;

	if(Flag.SearchFSkill(293))	dash=false;
	if(mWalkTimer<0)	mWalkTimer=0;

	if(dash && !mDash){
		if(Flag.SearchFSkill(277)){
			int oldtime=mWalkTimer;
			SetSpeed(int(mBaseSpeed/SDUSH_SPEED));
			if(oldtime>1 && mWalkTimer<=1)	mWalkTimer=2;
		}else{
			mSpeed=int(mBaseSpeed/DUSH_SPEED);
			if(mWalkTimer>0){
				if( 1<mWalkTimer && mWalkTimer<4){
					mWalkTimer=4;
				}
				mWalkTimer=int(mWalkTimer/DUSH_SPEED);
			}
		}
		mDash=dash;
	}
	if(!dash && mDash && mEscape<=0){
		mSpeed=mBaseSpeed;
		mWalkTimer=int(mWalkTimer*DUSH_SPEED);
		mDash=dash;
	}
}
void CPlayerObject::SetEscape(CMapObject* obj)
{
	mEscape=ESCAPE_TIME;
	mWalkTimer=-1;
	mNextDir=-1;

	int x,y;
	if(obj){
		x=int(mX)-obj->GetX();
		y=int(mY)-obj->GetY();
	}else{
		GetComp(mDir,&x,&y);
		x=-x;
		y=-y;
	}
	if(x!=0 || y!=0){
		double rad=atan2(double(x),double(y));
		mDir=int((-rad-0.5)/3.141592*2+8)%4;
	}else{
		mDir=(mDir+2)%4;
	}

	if(mSki==1)	mSki=2;
	SetGoalDir(mDir,1);
}
void CPlayerObject::SetBaseSpeed(int speed)
{
	double ratio;
	if(mBaseSpeed>=0){
		ratio=double(mBaseSpeed)/speed;
	}else{
		ratio=1;
	}
	mBaseSpeed=speed;
	SetSpeed(int(mSpeed*ratio));
}
bool CPlayerObject::SetAction(int type)
{
	if(mActionTime>0)	return false;

	mActionTime=ACTION_TIME;
	mActionType=type;
	return true;
}
int		CPlayerObject::GetAction()
{
	if(mActionTime==ACTION_AFTER_TIME){
		return mActionType;
	}else if(mActionTime>0){
		return -mActionTime;
	}
	return 0;
}

int	CPlayerObject::OnTimer(int mapsizex,int mapsizey,CMapMode *map,CMapObject* object[OBJECT_NO])
{
	if(0<mEscape){
		SetDash(true);
		if( mEscape<ESCAPE_TIME ){
			SetGoal(GetX_32(),GetY_32(),GetZ_32());
		}
		mEscape--;
		if(mEscape==0){
			mDir=(mDir+2)%4;
		}
	}

	mActionTime--;
	mGravity=mGravity&BASE_GRAV;
	if(Flag.SearchFSkill(281)){
		mGravity=mGravity | GRAV_FLOAT;
	}else{
		mFloatTimer=0;
	}

	if(Flag.SearchFSkill(276)){
		mGravity=mGravity | GRAV_HI_JUMP;
	}
	if(Flag.SearchFSkill(341)){
		mGravity=mGravity | GRAV_WATER_SKI;
	}
	int ret=CMapObject::OnTimer(mapsizex,mapsizey,map,object);

	if(mSki==1){
		if(ret<0){
			PlayWave(SE_HIT);
			SetEscape(NULL);
		}
	}else if(mSki>=2){
		mSki++;
		if(mSki>ESCAPE_TIME){
			SetPos(mSkiStartX,mSkiStartY,mSkiStartZ);
			mSki=0;
		}
	}
	return ret;
}
void CPlayerObject::OnDraw(int cx,int cy)
{
	if(mSki%3==2)	return;
	CMapObject::OnDraw(cx,cy);
}
void CPlayerObject::GetDrawInfo(int &x,int &y,int &tx,int &ty,int &turn)
{
	if(mActionTime>ACTION_AFTER_TIME){
		mAniNo=6;
	}else if(mActionTime>0){
		mAniNo=7;
	}

	CMapObject::GetDrawInfo(x,y,tx,ty,turn);
	if(0<mEscape){
		tx=tx ^ 1;
		turn=turn^TURN_HORIZONTAL;
	}
}
int CPlayerObject::CheckHit(CMapObject* obj,bool walk)
{
	bool enemy=(obj->GetObjType()==MapObjEnemy);

	if(enemy && mWalkTimer>0)
		walk=false;
	return CMapObject::CheckHit(obj,walk);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

void CBoxObject::InitBoxObject(int id,int x,int y,int z,int dir,int no)
{
	InitObject(id,x,y,z,dir,32,0,100,true,GRAV_NO_JUMP,1);

	mBoxNo=no;
}

void CBoxObject::OnDraw(int cx,int cy)
{
	mAniNo=(Flag.mTreasure[mBoxNo]?1:0);
	CMapObject::OnDraw(cx,cy);
}

bool CBoxObject::CheckTalk(CMapObject* obj)
{
	if(CheckHit(obj,true)!=0){
		MapMode.SendMessage(EH_BOX,mBoxNo,-1);
		return true;
	}
	return false;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////

void CSavePoint::InitSavePoint(int id,int x,int y,int z)
{
	InitObject(id,x,y,z,0,32,0,99,true,GRAV_NO_JUMP,1+OBJOPT_BLUE_SHADOW);

	for(int i=0;i<STAR_NO;i++){
		mXRad[i]=360/STAR_NO*i;
		mYRad[i]=pInput->Rand(360);
		mYSpeed[i]=pInput->Rand(32);
		mColor[i]=pInput->Rand(384);
	}
	mTimer=0;
}
int	 CSavePoint::OnTimer(int mapsizex,int mapsizey,CMapMode *map,CMapObject* object[OBJECT_NO])
{
	mTimer++;
	return CMapObject::OnTimer(mapsizex,mapsizey,map,object);
}
void CSavePoint::OnDraw(int cx,int cy)
{
	if(mID<0)	return;
	if(!mView)	return;
	int drawx;
	int drawy;

	int turn;
	int tx;
	int ty;

	GetDrawInfo(drawx,drawy,tx,ty,turn);
	drawx-=cx;
	drawy-=cy;
	drawy+=int(sin(mTimer*0.05)*MAP_CHIP_SIZE/16);

	int t=mTimer%128/2;
	if(t>32)	t=64-t;
	D3DCOLOR c=0x010101*t+0xffdfdfdf;

	DrawStar(drawx,drawy,false);
	DTest.DrawTexture(mImg,turn,tx*64,ty*64,64,64,drawx,drawy,MAP_CHIP_SIZE*2,MAP_CHIP_SIZE*2,c);
	DrawStar(drawx,drawy,true);
}

bool CSavePoint::CheckTalk(CMapObject* obj)
{
	if(CheckHit(obj,true)!=0){
		if(obj){
			ChangeMapInfo.mX=obj->GetX_32();
			ChangeMapInfo.mY=obj->GetY_32();
			ChangeMapInfo.mZ=obj->GetZ_32();
			ChangeMapInfo.mDir=obj->GetDir();
		}

		if(Flag.SearchFSkill(355)){
			MenuData.SetMenu(BypathMenu,0);
		}else{
			MenuData.SetMenu(DiaryMenu,0);
		}
		return true;
	}
	return false;
}
void	CSavePoint::DrawStar(int x,int y,bool top)
{
	for(int i=0;i<STAR_NO;i++){
		int xrad=mXRad[i]+mTimer*3;
		if(((xrad+90)%360<180) == top)	continue;
		double mx=(sin(xrad*3.141592/180))*MAP_CHIP_SIZE*0.7;

		int yrad=mYRad[i]+mTimer*(mYSpeed[i]+5)/10;
		double my=(sin(yrad*3.141592/180))*MAP_CHIP_SIZE*0.40;

		mx+=MAP_CHIP_SIZE*0.5;
		my+=MAP_CHIP_SIZE*0.5;
		D3DCOLOR c=GetColor(mColor[i]+mTimer);
		DTest.DrawTexture(mImg,0,64,64,32,32,x+mx,y+my,MAP_CHIP_SIZE,MAP_CHIP_SIZE,c);
	}
}

D3DCOLOR	CSavePoint::GetColor(int t)
{
	t%=384;
	if(t<64){
		return D3DCOLOR_ARGB(255,255,t*2+127,127);
	}else if(t<128){
		t-=64;
		return D3DCOLOR_ARGB(255,255-t*2,255,127);
	}else if(t<192){
		t-=128;
		return D3DCOLOR_ARGB(255,127,255,127+t*2);
	}else if(t<256){
		t-=192;
		return D3DCOLOR_ARGB(255,127,255-t*2,255);
	}else if(t<320){
		t-=256;
		return D3DCOLOR_ARGB(255,127+t*2,127,255);
	}else if(t<384){
		t-=320;
		return D3DCOLOR_ARGB(255,255,127,255-t*2);
	}
	return 0xffffffff;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
void CHighObject::OnDraw(int cx,int cy)
{
	if(mID<0)	return;
	if(!mView)	return;
	int drawx;
	int drawy;
	int turn;
	int tx;
	int ty;

	GetDrawInfo(drawx,drawy,tx,ty,turn);
	drawx-=cx;
	drawy-=cy;

	DTest.DrawTexture(mImg,turn,tx*64,ty*129,64,128,drawx,drawy-MAP_CHIP_SIZE*2,MAP_CHIP_SIZE*2,MAP_CHIP_SIZE*4);

#ifdef _DEBUG
	DrawDebugData(drawx,drawy);
#endif
}
int CHighObject::CheckHit(CMapObject* obj,bool walk)
{
	int ret=-1;
	int x=obj->GetX_32();
	int y=obj->GetY_32();
	int z=obj->GetZ();
	if(mHit){
		ret=1;
		if(walk){
			int dx,dy;
			GetComp(obj->GetDir(),&dx,&dy);
			x+=dx;
			y+=dy;
		}
	}

	if(x!=GetX_32())						return 0;
	if(y!=GetY_32())						return 0;
	if(z<=mZ-64)							return 0;
	if(z>=mZ+64)							return 0;

	int i=mID;
	int j=obj->GetID();
	if(i>j)	{int k=i;i=j;j=k;}
	MapMode.SendMessage(EH_HIT,i,j);
	return ret;

}
///////////////////////////////////////////////////////////////////////////////////////////////////////

CWoodObject::CWoodObject()
{
	mDeleteTimer=0;
}
int	 CWoodObject::OnTimer(int mapsizex,int mapsizey,CMapMode *map,CMapObject* object[OBJECT_NO])
{
	if(mDeleteTimer>0){
		mDeleteTimer--;
		if(mDeleteTimer==ACTION_AFTER_TIME){
			PlayWave(SE_CHOP);
		}else if(mDeleteTimer<=0){
			LoadImage(97);
			mHit=false;
		}
	}
	return CMapObject::OnTimer(mapsizex,mapsizey,map,object);
}
void CWoodObject::GetDrawInfo(int &x,int &y,int &tx,int &ty,int &turn)
{
	CHighObject::GetDrawInfo(x,y,tx,ty,turn);
	if(0<mDeleteTimer && mDeleteTimer<ACTION_AFTER_TIME){
		x+=mDeleteTimer%2*6-3;
	}
}
int CWoodObject::CheckHit(CMapObject* obj,bool walk)
{
	bool player=(obj->GetID()==0);
	int ret=CHighObject::CheckHit(obj,walk);

	if(ret!=0 && player){
		int z=obj->GetZ_32()-GetZ_32();
		if(abs(z)>=2)	return ret;
		Cut(obj);
	}
	return ret;
}
void CWoodObject::Cut(CMapObject* obj)
{
	if(mDeleteTimer!=0)			return;
	if(!mHit)					return;
	if(!Flag.SearchFSkill(302))	return;

	mDeleteTimer=ACTION_TIME;
	obj->SetAction(ACTION_NONE);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////
void CAniObject::InitObject(int id,int x,int y,int z,int dir,int speed,int movefreq,
										int img,bool hit,int gravity,int param)
{
	CMapObject::InitObject(id,x,y,z,dir,speed,movefreq,img,hit,gravity,param);
	mAnimation=dir % 16;
}
void CAniObject::GetDrawInfo(int &x,int &y,int &tx,int &ty,int &turn)
{
	GetDrawXY(x,y);

	turn=0;
	ty=mAnimation%4;
	tx=mAnimation/4;
	if(mParam & OBJOPT_SHOW_TURN)	turn=TURN_HORIZONTAL;
}
void CAniObject::SetDir(int dir)
{
	mAnimation=dir % 16;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
void CHighAniObject::InitObject(int id,int x,int y,int z,int dir,int speed,int movefreq,
										int img,bool hit,int gravity,int param)
{
	CMapObject::InitObject(id,x,y,z,dir,speed,movefreq,img,hit,gravity,param);
	mAnimation=dir % 8;
}
void CHighAniObject::GetDrawInfo(int &x,int &y,int &tx,int &ty,int &turn)
{
	GetDrawXY(x,y);

	turn=0;
	ty=mAnimation%2;
	tx=mAnimation/2;
	if(mParam & OBJOPT_SHOW_TURN)	turn=TURN_HORIZONTAL;
}
void CHighAniObject::SetDir(int dir)
{
	mAnimation=dir % 8;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
void CBigObject::OnDraw(int cx,int cy)
{
	if(mID<0)	return;
	if(!mView)	return;
	int drawx;
	int drawy;
	int turn;
	int tx;
	int ty;

	GetDrawInfo(drawx,drawy,tx,ty,turn);
	drawx-=cx;
	drawy-=cy;

	DTest.DrawTexture(mImg,turn,tx*128,ty*128,128,128,drawx-MAP_CHIP_SIZE/2,drawy-MAP_CHIP_SIZE*2,MAP_CHIP_SIZE*4,MAP_CHIP_SIZE*4);

#ifdef _DEBUG
	DrawDebugData(drawx,drawy);
#endif
}
void CBigObject::DrawShadow(int cx,int cy,int z)
{
	if(mID<0)	return;
	if(mParam & OBJOPT_NO_SHADOW)	return;
	if(!mView)	return;
	int drawx;
	int drawy;
	GetDrawXY(drawx,drawy);
	drawx-=cx;
	drawy=int(drawy-cy+((mZ-z*16)*MAP_CHIP_SIZE)/32-MAP_CHIP_SIZE/2);

	DTest.DrawTexture(SystemTex,0,0,144,64,48,drawx+MAP_CHIP_SIZE/2,drawy+MAP_CHIP_SIZE/2,MAP_CHIP_SIZE*3,MAP_CHIP_SIZE*3*5/8,COLOR_SHADOW);
}
int CBigObject::CheckHit(CMapObject* obj,bool walk)
{
	int ret=-1;
	int x=obj->GetX_32();
	int y=obj->GetY_32();
	int z=obj->GetZ();
	if(mHit){
		ret=1;
		if(walk){
			int dx,dy;
			GetComp(obj->GetDir(),&dx,&dy);
			x+=dx;
			y+=dy;
		}
	}

	if(x>GetX_32())							return 0;
	if(x<=GetX_32()-3)						return 0;
	if(y>GetY_32())							return 0;
	if(y<=GetY_32()-3)						return 0;
	if(z<=mZ-64)							return 0;
	if(z>=mZ+64)							return 0;

	int i=mID;
	int j=obj->GetID();
	if(i>j)	{int k=i;i=j;j=k;}
	MapMode.SendMessage(EH_HIT,i,j);
	return ret;

}
///////////////////////////////////////////////////////////////////////////////////////////////////////
void CBigAniObject::GetDrawInfo(int &x,int &y,int &tx,int &ty,int &turn)
{
	GetDrawXY(x,y);

	turn=0;
	ty=mDir%2;
	tx=mDir/2;
	if(mParam & OBJOPT_SHOW_TURN)	turn=TURN_HORIZONTAL;
}
//////////////////////////////////////////////////////////////
void CAutoAniObject::InitAutoAniObject(int id,int x,int y,int z,int dir,int speed,int movefreq
							,int img,int koma,int anispd,bool hit,int gravity,int param)
{
	InitObject(id,x,y,z,dir,speed,movefreq,img,hit,gravity,param);

	mAutoAniTimer=0;
	mKoma=koma;
	mAniSpeed=anispd;
}
int	 CAutoAniObject::OnTimer(int mapsizex,int mapsizey,CMapMode *map,CMapObject* object[OBJECT_NO])
{
	int ret=CMapObject::OnTimer(mapsizex,mapsizey,map,object);
	mAutoAniTimer++;

	int dir=(mAutoAniTimer/mAniSpeed)%mKoma;
	if(mKoma<=4 && dir>=2) dir+=2;
	SetDir(dir);

	return ret;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////

CPutObject::CPutObject()
{
	mHeavy=0;
}
int		CPutObject::OnTimer(int mapsizex,int mapsizey,CMapMode *map,CMapObject* object[OBJECT_NO])
{
	if(mWalkTimer==mSpeed){
		PlayWave(SE_MOVE_BLOCK);
	}

	int ret=CLiftObject::OnTimer(mapsizex,mapsizey,map,object);
	
	if(mHeavy>0){
		int x,y;
		GetComp(GetDir(),&x,&y);
		if(mHeavy>8){
			x=-x;
			y=-y;
		}
		mX+=x*0.5;
		mY+=y*0.5;
		if(mHeavy>16){
			mHeavy=0;
			FixPos();
		}else{
			mHeavy++;
		}
	}

	return ret;
}
void CPutObject::GetDrawInfo(int &x,int &y,int &tx,int &ty,int &turn)
{
	GetDrawXY(x,y);

	turn=0;
	ty=0;
	tx=0;
}
int CPutObject::CheckHit(CMapObject* obj,bool walk)
{
	bool player=(obj->GetID()==0);

	int hit=CLiftObject::CheckHit(obj,walk,1);

	if(hit>0 && walk && mWalkTimer<=0 && player && mHeavy<=0){
		bool move=true;
		if(mParam & OBJOPT_HEAVY_1){
			if(Flag.SearchFSkill(296)<=0)	move=false;
		}
		if(mParam & OBJOPT_HEAVY_2){
			if(Flag.SearchFSkill(297)<=0)	move=false;
		}

		if(move){
			SetGoalDir(obj->GetDir(),1);
		}else{
			SetDir(obj->GetDir());
			mHeavy=1;
			PlayWave(SE_MOVE_BLOCK);
		}
	}
	return hit;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////

CLiftObject::CLiftObject()
{
	mMoveX=0;
	mMoveY=0;
}
int		CLiftObject::OnTimer(int mapsizex,int mapsizey,CMapMode *map,CMapObject* object[OBJECT_NO])
{
	double oldx=mX;
	double oldy=mY;

	int ret=CMapObject::OnTimer(mapsizex,mapsizey,map,object);

	mMoveX=mX-oldx;
	mMoveY=mY-oldy;

	return ret;
}
int		CLiftObject::CheckHit(CMapObject* obj,bool walk)
{
	return CheckHit(obj,walk,0);
}

int		CLiftObject::CheckHit(CMapObject* obj,bool walk,int bottomhit)
{
	int ret=-1;
	int x=obj->GetX();
	int y=obj->GetY();
	int z=obj->GetZ();
	if(mHit){
		ret=1;
		if(walk){
			int dx,dy;
			GetComp(obj->GetDir(),&dx,&dy);
			x+=dx*32;
			y+=dy*32;
		}
	}
	bool hit=true;

	if(z<=mZ && bottomhit){
		ret=CMapObject::CheckHit(obj,walk);
		if(ret<=0)	return ret;
	}else{
		if(x<=mX-16)		return 0;
		if(x>mX+16)			return 0;
		if(y<=mY-16)		return 0;
		if(y>mY+16)			return 0;

		if(z<=mZ)			return 0;
		if(z>=mZ+32)		return 0;
	}

	int i=mID;
	int j=obj->GetID();
	if(i>j)	{int k=i;i=j;j=k;}
	if(i!=j){
		MapMode.SendMessage(EH_HIT,i,j);
	}

	if(ret>0 && obj!=this && z>mZ ){
		obj->Move(mMoveX,mMoveY);
	}
	return hit;
}
unsigned int CLiftObject::GetDrawZ(void)
{
	return (int(mX)+int(mY))/32*0x01000000+(int(mZ))/16*0x010000+ 64-(int)(mX)%32-(int)(mY)%32+mShowNo*64;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////
void	CNormalTrapObject::InitNTObject(int objno,int x,int y,int z,int img,int showno,
									int cycle,int timer,int pow,int se)
{
	InitObject(objno,x,y,z,0,32,0,img,0,GRAV_LOCK,showno);

	mCycle=cycle;
	mPow=pow;
	mSE=se;

	mTimer=timer;
	mDmg=false;
}
int		CNormalTrapObject::CheckHit(CMapObject* obj,bool walk)
{
	bool player=(obj->GetID()==0);

	int hit=CMapObject::CheckHit(obj,walk);
	if(hit!=0 && player && mDmg){
		MapMode.SetDamage(this,mPow);
		mDmg=false;
		return 2;
	}
	return hit;
}
int		CNormalTrapObject::OnTimer(int mapsizex,int mapsizey,CMapMode *map,CMapObject* object[OBJECT_NO])
{
	int t=mTimer%mCycle;
	if(t==0){
		int x,y,px,py;
		object[0]->GetDrawXY(px,py);
		GetDrawXY(x,y);
		py-=40;
		if(abs(px-x)<320 && abs(py-y)<240){
			PlayWave(mSE);
		}
	}
	if(t==0){
		mDmg=true;
		SetDir(4);
	}else if(t==4){
		SetDir(1);
	}else if(t==8){
		SetDir(5);
	}else if(t>=12){
		mDmg=false;
		SetDir(0);
	}

	mTimer++;

	int ret=CMapObject::OnTimer(mapsizex,mapsizey,map,object);
	return ret;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
void	CStoneObject::InitStoneObject(int objno,int x,int y,int z,int dir)
{
	InitObject(objno,x,y,z+2,0,8,0,98,1,GRAV_FALL_NONE,OBJOPT_SMALL_SHADOW+1);
    mSpeedZ=-0.01;
	SetGoalDir(dir,255);
}
int		CStoneObject::OnTimer(int mapsizex,int mapsizey,CMapMode *map,CMapObject* object[OBJECT_NO])
{
	int ret=CMapObject::OnTimer(mapsizex,mapsizey,map,object);

	if(ret>0){
		MapMode.SendMessage(EH_ENCOUNT,ret-1,1);
	}

	if(ret>1 || ( mSpeedZ>=0 && ret!=1)){
		object[mID]=NULL;
		delete this;
	}

	return ret;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

void CEnemyObject::InitEnemyObject(int id,int x,int y,int z,int dir,int speed,int movefreq,
									int img,bool hit,int gravity,int showno,
									int movemode,int search,int reader,int lv,int member,int field,int option)
{
	InitObject(id,x,y,z,dir,speed,movefreq,img,hit,gravity,showno|1);

	mMoveMode=movemode;
	mSearch=search;

	mParty=-1;
	mReader=reader;
	mLv=lv;
	mMember=member;
	mField=field;
	mOption=option;

	mEncountTimer=0;
	mStartX=x;
	mStartY=y;

	mParamA=0;
	mParamB=0;
}
void CEnemyObject::InitEnemyObject(int id,int x,int y,int z,int dir,int speed,int movefreq,
									int img,bool hit,int gravity,int showno,
									int movemode,int search,int party)
{
	InitObject(id,x,y,z,dir,speed,movefreq,img,hit,gravity,showno|1);

	mMoveMode=movemode;
	mSearch=search;

	mParty=party;

	mEncountTimer=0;
	mStartX=x;
	mStartY=y;

	mParamA=0;
	mParamB=0;
}
int	 CEnemyObject::OnTimer(int mapsizex,int mapsizey,CMapMode *map,CMapObject* object[OBJECT_NO])
{
	if(mEncountTimer>0){
		SetGoalDir(0,0);
	}

	int ret=CMapObject::OnTimer(mapsizex,mapsizey,map,object);

	if(mEncountTimer>0){
		mEncountTimer--;
		SetGoalDir(0,0);
		return ret;
	}

	if(ret==1){
		MapMode.SendMessage(EH_ENCOUNT,mID,-1);
		mEncountTimer=ESCAPE_TIME;
	}else{
		if(object[0]){
			int x=object[0]->GetX()-(int)mX;
			int y=object[0]->GetY()-(int)mY;
			int z=object[0]->GetZ()-(int)mZ;
			if(x*x+y*y+z*z < 32){
				MapMode.SendMessage(EH_ENCOUNT,mID,-1);
				mEncountTimer=ESCAPE_TIME;
			}
		}
	}
	if(mSki)	return ret;

	switch(mMoveMode){
		case 0:	Search_0(object);		break;
		case 1:	Search_1(object);		break;
		case 2:	Search_2(object);		break;
		case 3:	Search_3(1);			break;
		case 4:	Search_3(-1);			break;
		case 5:	Search_5(object);		break;
		case 6:	Search_6(object);		break;
		case 7:	Search_7(map);			break;
		case 8:	Search_8(1);			break;
		case 9:	Search_8(-1);			break;
		case 10:Search_8(2);			break;
		case 11:Search_3(2);			break;

		case 100:	Search_100();		break;
	}

	return ret;
}

void CEnemyObject::OnDraw(int cx,int cy)
{
	if(mEncountTimer>0 && mEncountTimer%8<4){
		return;
	}

	CMapObject::OnDraw(cx,cy);
}

int CEnemyObject::CheckHit(CMapObject* obj,bool walk)
{
	bool player=(obj->GetID()==0);

	if(player && mWalkTimer>0 && mEncountTimer<=0) walk=false;
	int hit=CMapObject::CheckHit(obj,walk);

	if(hit!=0 && player && obj->GetWalkTimer()<=0){
		if(mEncountTimer<=0){
			MapMode.SendMessage(EH_ENCOUNT,mID,-1);
			mEncountTimer=ESCAPE_TIME;
			return 2;
		}else {
			int pz=obj->GetZ_32();
			int z=GetZ_32();
			if(pz<z-1 || z+1<pz)	return 2;
			if(Flag.SearchFSkill(285))	return 0;
			return 2;
		}
	}
	return hit;
}

void	CEnemyObject::Search_0(CMapObject* object[OBJECT_NO])
{
	if(Flag.mPlayTime%8)		return;
	if(Flag.SearchFSkill(284))	return;
	int tx=object[0]->GetX_32();
	int ty=object[0]->GetY_32();
	int lx=tx-mStartX;
	int ly=ty-mStartY;
	int l=mSearch*mSearch;
	if(lx*lx<=l && ly*ly<=l){
		SetGoal(tx,ty,object[0]->GetZ_32());
		mParamA=1;
	}else if(mParamA!=0){
		SetGoal(GetX_32(),GetY_32(),GetZ_32());
		mParamA=0;
	}

	int x=GetX_32()-mStartX;
	int y=GetY_32()-mStartY;
	if(x*x>l || y*y>l)	SetGoalDir(mDir+2,1);
}

void	CEnemyObject::Search_1(CMapObject* object[OBJECT_NO])
{
	int tz=object[0]->GetZ_32();
	if(tz!=GetZ_32())	return;

	Search_0(object);
}
void	CEnemyObject::Search_2(CMapObject* object[OBJECT_NO])
{
	if(Flag.mPlayTime%8)		return;
	if(Flag.SearchFSkill(284))	return;
	int lx=object[0]->GetX_32()-GetX_32();
	int ly=object[0]->GetY_32()-GetY_32();
	int l=mSearch*mSearch;
	if(lx*lx<=l && ly*ly<=l){
		int dir;
		if(lx*lx>ly*ly){
			if(lx>0)	dir=0;
			else		dir=2;
		}else{
			if(ly>0)	dir=1;
			else		dir=3;
		}
		SetGoalDir(dir,1);
	}
}
void	CEnemyObject::Search_3(int dir)
{
	if(mWalkTimer<=0){
		SetGoalDir(dir+mDir,255);
	}
}
void	CEnemyObject::Search_5(CMapObject* object[OBJECT_NO])
{
	if(Flag.SearchFSkill(284))	return;
	SetGoal(object[0]->GetX_32(),GetY_32(),object[0]->GetZ_32());
	SetDir(mSearch);
}
void	CEnemyObject::Search_6(CMapObject* object[OBJECT_NO])
{
	if(Flag.SearchFSkill(284))	return;
	SetGoal(GetX_32(),object[0]->GetY_32(),object[0]->GetZ_32());
	SetDir(mSearch);
}
void	CEnemyObject::Search_7(CMapMode* map)
{
	if(Flag.mPlayTime%2)	return;
	int x=GetX_32();
	int y=GetY_32();
	int z=GetZ_32();
	int h=map->GetMapHeight(x,y,z);

	int newh[4];
	for(int i=0;i<4;i++){
		int dir=(i+mDir)%4;
		int newx,newy;
		GetComp(dir,&newx,&newy);
		newx+=x;
		newy+=y;
		newh[dir]=map->GetMapHeight(newx,newy,z);
		if(newh[dir]==h-1){
			SetGoal(newx,newy,z);
			return;
		}
	}


	for(int i=0;i<4;i++){
		int dir=(i+mDir)%4;
		if(newh[dir]==h){
			SetGoalDir(dir,1);
			return;
		}
	}
}

void	CEnemyObject::Search_8(int dir)
{
	int x=GetX_32()-mStartX;
	int y=GetY_32()-mStartY;
	int l=mSearch*mSearch;
	if(x*x>=l || y*y>=l){
		SetGoalDir(dir+mDir,255);
		mStartX=GetX_32();
		mStartY=GetY_32();
	}else if(mWalkTimer<=0){
		SetGoalDir(mDir,255);
	}
}



///////////////////デバッグ情報の描画///////////////////////////////////////////////////////////////
#ifdef _DEBUG
void CMapObject::DrawDebugData(int x,int y)
{
	x+=16;
	#ifdef _SHOW_OBJECT_NO_
		y+=16;
		DTest.DrawBox(x,y,x+40,y+10,0x80ffffff);
		DTest.Printf(1,x,y,"no.%d",mID);
	#endif
	#ifdef _SHOW_OBJECT_IMG_NO_
		if(MapMode.mDebugShowImgNo){
			y+=16;
			DTest.DrawBox(x,y,x+40,y+10,0x80ffffff);
			DTest.Printf(1,x,y,"Img.%03d",mDebugImageNo);
		}
	#endif
}
void CBoxObject::DrawDebugData(int x,int y)
{
	x+=16;

	#ifdef _SHOW_TRESURE_NO_
		y+=16;
		DTest.DrawBox(x,y,x+40,y+10,0x80ffffff);
		DTest.Printf(1,x,y,"Box.%d",mBoxNo);
	#endif
	CMapObject::DrawDebugData(x-16,y);

}
#endif