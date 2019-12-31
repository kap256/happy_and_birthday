#include "battlecharacter.h"
#include "input.h"
#include "animationdata.h"
#include "itemdata.h"
#include "menudata.h"
#include "option.h"
#include "battlemode.h"

CBattleCharacter* CBattleCharacter::mHelpTarget=NULL;

CBattleCharacter::CBattleCharacter()
{
	mLive=false;
	mCData=NULL;
	mID=0;
}

CBattleCharacter::~CBattleCharacter()
{
}
void	CBattleCharacter::Draw(CPlaneWindow* window,int x,int y,int param,D3DCOLOR color) const
{
//param & 1  選択不可 
//param & 2  レベルの代わりに撃破数
	D3DCOLOR textcolor=0xff000000;
	if(param & 1){
		textcolor=COLOR_GRAY;
	}
	window->DrawText(0,GetName(),x,y,textcolor);

	static char text[32];
	if(param & 2){
		int kill=GetKillNo();
		if(kill>=0){
			window->DrawText(0,"撃破数 -",60,y,textcolor,DT_RIGHT);
			sprintf(text,"%d",kill);
			window->DrawText(0,text,20,y,textcolor,DT_RIGHT);
		}
	}else{
		window->DrawText(0,"Lv.",40,y,textcolor,DT_RIGHT);
		sprintf(text,"%d",GetLv());
		window->DrawText(0,text,20,y,textcolor,DT_RIGHT);
	}
}
void	CBattleCharacter::Help(int param)const
{
	if(mID<0){
		mHelpTarget=(CBattleCharacter*)this;
		MenuData.SetMenu(BattleStatusMenu,0x100);
	}else{
		MenuData.SetMenu(BattleStatusMenu,mID);
	}
}
void	CBattleCharacter::LoadCharacter(int no,int x,int y,int param)
{
	if(no<0){
		mLive=false;
		return;
	}
	mLive=true;
	mTimer=0;
	mAirCycle=(pInput->Rand(4000)+3000)/100000.0;//0.05
	mNo=no;
	mZ=0;
	mAni=0;
	mAniTimer=0;
	mTurn=false;
	mJump=false;
	mActionCount=0;
	mAction=-1;
	mTarget=0;
	mColor=0xffffffff;

	mSX=x;
	mSY=y;

	mX		=mSX*BATTLE_SQUARE_SIZE;
	mY		=mSY*BATTLE_SQUARE_SIZE;
	mGoalX	=mSX*BATTLE_SQUARE_SIZE;
	mGoalY	=mSY*BATTLE_SQUARE_SIZE;

	mSpeedX=0;
	mSpeedY=0;
	mSpeedZ=0;
	mMoveTime=0;
	mDamageTimer=0;
	mAvoidTimer=0;
	mCrtFlash=false;

	mHP=GetMaxHP();
#ifdef	_LOCK_HP_
	mHP=_LOCK_HP_;
#endif

	mAtkUp=0;
	mDefUp=0;
	mMagAtkUp=0;
	mMagDefUp=0;
	mSpdUp=0;

	ClearAllStatus();
#ifdef	_SET_STATUS_
	SetStatus(_SET_STATUS_,100);
#endif

	LoadImage(no);
}
void	CBattleCharacter::LoadImage(int no)
{
	char filename[256];
#ifdef _BATTLE_IMAGE_
	no=_BATTLE_IMAGE_;
#endif
	sprintf(filename,"data\\battlecharacter\\battlecharacter%03d",no);
	DTest.LoadTexture(mImg,filename,0xff008080);
}
void	CBattleCharacter::OnTimer(int mode,int timer)
{
	if(!mLive)	return;
	mDamageTimer--;
	mAvoidTimer--;
	mAniTimer--;
	mTimer++;
	int db=GetDeathBlack();
	if(db>0){
		db+=16;
		if(db>96)	db=96;
		SetDeathBlack(db);
	}

	bool air=false;
	if(mZ>0 || mSpeedZ>0){
		mSpeedZ-=BATTLE_GRAVITY;
		air=true;
	}

	mX+=mSpeedX;
	mY+=mSpeedY;
	mZ+=mSpeedZ;
	if(mMoveTime>0){
		mMoveTime--;
		if(mMoveTime<=0){
			mSpeedX=0;
			mSpeedY=0;
			mX=mGoalX;
			mY=mGoalY;
			//mSpeedZ=0;
			mJump=false;
		}
	}

	if(mZ<=0){
		if(air){
			mSpeedZ=0;
			if(mMoveTime>0 && mJump){
				mMoveTime=0;
				mSpeedX=0;
				mSpeedY=0;
				mX=mGoalX;
				mY=mGoalY;
				mJump=false;
			}
			if(!GetDead()){
				if(ChangeAni(6*ANI_TYPE,8,false)){
					mTurn=false;
				}
			}
		}
		mZ=0;
	}

	if(SearchASkill(349)){
		D3DCOLOR color;
		switch(BattleMode.GetTurn()%4){
			case 0:		color=0xffffff00;	break;
			case 1:		color=0xff00ff00;	break;
			case 2:		color=0xff0080ff;	break;
			case 3:		color=0xffff0000;	break;
		}
		unsigned char* c=(unsigned char*)&color;
		unsigned char* old=(unsigned char*)&mColor;
		for(int i=0;i<4;i++){
			old[i]+=(c[i]-old[i])/20;
		}
	}
}

void	CBattleCharacter::OnDraw(D3DCOLOR color,bool weapon)
{
	if(!mLive)				return;
	if(SearchStatus(14)>0)	return;
	int x,y,z;
	GetDrawPositon(x,y,z);
	y-=z;

	if(CheckAir()){
		if(!GetDead()){
			y-=int(24-sin(mTimer*mAirCycle)*6);
		}
	}
	if(color==0){
		color=mColor;
	}
	if(SearchStatus(19)>0 && Flag.mPlayTime%2==0){
		int dx,dy;
		bool turn;
		GetDrawData(dx,dy,turn);
		int add;
		const int ADD=16;
		if(turn){
			add=-ADD;
		}else{
			add= ADD;
		}
		OnDraw(x+add,y-4,color,weapon);
		//OnDraw(x+add,y,(((color&0xff000000)/2)&0xff000000) | (color&0x00ffffff) );
	}

	if(mDamageTimer%3==1 && mCrtFlash){
		DTest.SetBlend(BLEND_SOLID);
		OnDraw(x,y,0xffffffff,weapon);
		DTest.SetBlend(BLEND_STANDARD);
	}else{
		OnDraw(x,y,color,weapon);
	}
}
void	CBattleCharacter::OnDraw(int x,int y,D3DCOLOR color,bool weapon)
{
	if(!mLive)				return;
	int dx,dy;
	bool turn;
	GetDrawData(dx,dy,turn);

	int size=64*GetSize();
	if(GetDead() && color==0xffffffff){
		int db=GetDeathBlack();
		if(db>255)	db=255;		//もっともくらい色は、OnTimerで定義
		color=0xffffffff-(db<<16)-(db<<8)-(db<<0);
	}

	DTest.DrawTexture(mImg,(turn?TURN_HORIZONTAL:0),
						dx*size,dy*size,
						size,size,
//dx*64,dy*64,64,64,
						x,y,size*2,size*2,color);
}
	
void	CBattleCharacter::GetDrawData(int &dx,int &dy,bool &turn)
{
	dx=0;
	dy=0;
	int ani=0;
	if(mAction>=0 && mTarget>=0)				ani=1*ANI_TYPE;
	if(mHP<(GetMaxLP()+GetMaxHP())*0.15)		ani=6*ANI_TYPE;
	if(SearchStatus(15)>0)						ani=1*ANI_TYPE;
	if(SearchStatus(5)>0)						ani=1*ANI_TYPE;
	if(SearchStatus(1)>0)						ani=6*ANI_TYPE;
	if(SearchStatus(9)>0)						ani=6*ANI_TYPE;
	if(SearchStatus(26)>0)						ani=6*ANI_TYPE;


	if(mMoveTime>0){
		if(!mJump){
			ani=(2+(mMoveTime/2)%2)*ANI_TYPE;
		}else{
			ani=(4+(mMoveTime/2)%2)*ANI_TYPE;
		}
	}
	if(GetDead())		ani=7*ANI_TYPE;
	if(mAniTimer>=0)	ani=mAni;

	if(SearchASkill(403)){	//夢の卵専用
		ani=(mTimer/20)%4;
		int mlp=GetMaxLP();
		ani+=((mlp-mHP)*4/mlp)*4;
		ani*=ANI_TYPE;
	}

	dx=(ani/4)%4;
	dy=(ani/4)/4;
	mAni=ani;

	turn=false;
	
	if(mSpeedX!=0 && mAniTimer<=0 && mMoveTime>0 && mJump){
		turn=!turn;
	}

	if(mSpeedX>0){
		turn=!turn;
	}
	if(mSpeedX==0 && Enemy()){
		turn=!turn;
	}
	if(mTurn){
		turn=!turn;
	}
}
void	CBattleCharacter::GetDrawPositon(int &x,int &y,int &z)
{
	int size=(GetSize()-1)*BATTLE_SQUARE_SIZE/2;
	x=(int)mX+BATTLE_SQUARE_X-((GetSize()-1)*BATTLE_SQUARE_SIZE/2);
	y=(int)mY+BATTLE_SQUARE_Y-((GetSize()-1)*BATTLE_SQUARE_SIZE*3/4);
	z=(int)mZ;

	if(mDamageTimer>0)	x-=(mDamageTimer%2)*4;

	if(mAvoidTimer>0){
		double s=sin((mAvoidTimer*3.141592)/AVOID_TIMER);
		x-=int(s*AVOID_X);
		y-=int(s*AVOID_Y);
		z+=int(s*AVOID_Z);
	}
}

void	CBattleCharacter::DrawStatus(D3DCOLOR color)
{
	if(!mLive)	return;
	int	draw=GetDrawStatus();
	if(draw<0)	return;

	int x=GetCX();
	int y=GetCY();

	int dx=(draw/8)%2 * 128;
	int dy=draw%8 * 32;

	if(CheckAir())	y-=16;

	dx+=(Flag.mPlayTime/STATUS_ANI_TIME)%4*32;
	DTest.DrawTexture(StatusTex[draw/16],0,dx,dy,32,32,x-16+2,y-32*GetSize()+2,32,32,0x80000000);
	DTest.DrawTexture(StatusTex[draw/16],0,dx,dy,32,32,x-16,y-32*GetSize(),32,32,color);

}
void	CBattleCharacter::DrawShadow()
{
	if(!mLive)	return;
	int x,y,z;

	GetDrawPositon(x,y,z);

	int size=16*GetSize();

	DTest.DrawTexture(SystemTex,0,0,144,64,48,x+size*2,y+size*3+(GetSize()-1)*16+32,size*4,size*3,COLOR_SHADOW);

}
bool	CBattleCharacter::TurnEnd(CBattleMode* bm)
{
	mActionCount=0;
	SetAction(-1);

	double STATUS_DOWN = 0.70;
	if(SearchASkill(426))	STATUS_DOWN = 0.90;

	mAtkUp=		int(mAtkUp*STATUS_DOWN);
	mDefUp=		int(mDefUp*STATUS_DOWN);
	mMagAtkUp=	int(mMagAtkUp*STATUS_DOWN);
	mMagDefUp=	int(mMagDefUp*STATUS_DOWN);
	mSpdUp=		int(mSpdUp*STATUS_DOWN);

	if(SearchStatus(0)>0)					return false;
	if(SearchStatus(6)>0)					return false;
	if(SearchStatus(12)>0)					return false;
	if(SearchASkill(251) && !(GetDead()))	return false;
	if(bm->GetFieldEffect(GetSX(),GetSY()))	return false;
	return true;
}
bool	CBattleCharacter::TurnStart(void)
{
	CheckLPOver();
	mCharm=(SearchStatus(2)>0);
	mBerserk=(SearchStatus(5)>0);
	return true;
}

void	CBattleCharacter::Move(int x,int y,int time,bool move)
{
	if(mSX==x && mSY==y)	return;
	mSX=x;
	mSY=y;
	if(move)	SetGoalSquare(x,y,time,GetSize(),false,true);
}
void	CBattleCharacter::Return(int time,bool jump,bool force)
{
	if(mSX*BATTLE_SQUARE_SIZE == int(mX) && mSY*BATTLE_SQUARE_SIZE == int(mY))	return;
	int size=GetSize();
	int x=mSX;
	int y=mSY;
	GetSquareTarget(x,y,size);
	if(mGoalX==x && mGoalY==y )	return;
	SetGoal(x,y,time,jump,force);
}

void	CBattleCharacter::GetSquareTarget(int &x,int &y,int size)
{
	int s=size-GetSize();
	
	int gx=(x*2+s)*BATTLE_SQUARE_SIZE/2;
	int gy=(y*2+s)*BATTLE_SQUARE_SIZE/2;
	if(x!=mSX || y!=mSY){
		int s=size+GetSize()-1;
		if(Enemy()){
			gx-=BATTLE_SQUARE_SIZE*s*2/3;
		}else{
			gx+=BATTLE_SQUARE_SIZE*s*2/3;
		}
	}

	x=gx;
	y=gy;
}
void	CBattleCharacter::SetGoalSquare(int x,int y,int time,int size,bool jump,bool force)
{
	GetSquareTarget(x,y,size);
	SetGoal(x,y,time,jump,force);
}

void	CBattleCharacter::SetPosSquare(int x,int y,int size)
{
	GetSquareTarget(x,y,size);
	SetPos(x,y,0);
}
void	CBattleCharacter::SetGoal(int x,int y,int time,bool jump,bool force)
{
	if(GetDead() && mDamageTimer<0 && !force)								return;
	mGoalX=x;
	mGoalY=y;
	mMoveTime=time;

	if(time>0){
		mSpeedX=(mGoalX-mX)/time;
		mSpeedY=(mGoalY-mY)/time;
	}

	mJump=jump;
	if(jump){
		mSpeedZ=(time)*BATTLE_GRAVITY/2;
	}
}
void	CBattleCharacter::SetPos(int x,int y,int z)
{
	if(GetDead() && mDamageTimer<0)	return;
	mGoalX=x;
	mGoalY=y;
	mMoveTime=0;
	mSpeedX=0;
	mSpeedY=0;

	mX=x;
	mY=y;
	mZ=z;
}
void	CBattleCharacter::SetSpeedZ(double z)
{
	mSpeedZ=z;
}
unsigned int	CBattleCharacter::SetWaitTime(CBattleCharacter* Target)
{
	if(mAction<0)	return 0x7fffffff;
	if(!mLive)		return 0x7fffffff;

	if(ComSkillData[mAction].mType&CS_SUPER_QUICK)	return 1;	//スーパークイック
	if(SearchStatus(11)>0)							return 2;	//ラピッドラビット
	unsigned int wt;

	int speed=GetSpeed();
	if(SearchASkill(374)){	//アイテム投げ
		if(ComSkillData[mAction].mType&CS_ITEM){
			speed+=200;
		}
	}

	if(!(ComSkillData[mAction].mType&CS_MOVE)){
		wt=BATTLE_NO_MOVE_WAIT*0x00ffffff/speed;
	}else{
		int tx=Target->GetSX();
		int ty=Target->GetSY();
		double length=_hypot(mSX-tx,mSY-ty);
		wt=int((BATTLE_ACTION_WAIT+length)*0x00ffffff/speed);
	}
	wt=int(wt*((75+pInput->Rand(50))/100.0));
	wt*=mActionCount+4;
	wt+=2;
	if(wt>0x0fffffff){
		wt=0x0fffffff;
	}

//ここからreturnアリ。
	if(!SearchASkill(268)){
		if(SearchASkill(395)){
			wt+=0x40000000;
		}else if(ComSkillData[mAction].mType&CS_SUPER_SLOW){
			wt+=0x60000000;
		}else if(ComSkillData[mAction].mType&CS_SLOW){
			wt+=0x40000000;
		}else if(!(ComSkillData[mAction].mType&CS_QUICK)){
			wt+=0x20000000;
		}
	}

	return wt;
}

bool CBattleCharacter::ChangeAni(int ani,int time,bool force)
{
	if(mAniTimer>0	&& !force){
		return false;
	}
	mAni=ani;
	mAniTimer=time;
	mDamageTimer=0;
	TurnWeapon(0);
	return true;
}
void	CBattleCharacter::Death(int option)
{
	mHP=0;
	bool revival=false;
	if(SearchStatus(22)>0)	revival=true;
	ClearAllStatus();
	if(revival)	SetStatus(22,100);
	SetKillNo();

	if(SearchASkill(365)){
		SetStatus(23,100);
	}

	//SetAction(-1);
}
int	CBattleCharacter::SetDamage(int dmg,bool crt,int option)
{
	const double HARD_LOST=1.0 ;
	const double DEATH_LOST=2.0 ;
	const double LP_LOST=1.0/10.0 ;

	double lplost=LP_LOST ;
	if(Flag.mHardMode>0)	lplost*=HARD_LOST;
	double deathlost=lplost*DEATH_LOST ;
	bool retlp=false;

	if(option & AT_OPT_LIFE_BREAK){
		lplost=0.5;
		retlp=true;
	}

	bool active=!GetDead();
	int oldhp=mHP;

	mCrtFlash=crt;
	if(dmg<=0)	return 0;
	if(!(option&AT_OPT_NO_EFFECT)){
		SetDamageEffect(crt);
	}
	mHP-=dmg;

	if(SearchASkill(403)){	//夢の卵専用
		if(mHP<=0)	mHP=1;
		int mlp=GetMaxLP();
		int n=((mHP-1)*4/mlp);
		if(((oldhp-1)*4/mlp)!=n){
			PlayWave(SE_EGG_BREAK);
			int lostlp=GetMaxHP()-(n+1)*mlp/4;
			if(lostlp>0)	mCData->LostLP(lostlp);
			mHP-=10;
			SetAction(204);
		}
	}

	if(mHP<=0){
		if(oldhp>0){
			if(option&AT_OPT_NO_DEATH){
				mHP=1;
				int lost=LostLP(int((oldhp-mHP-1)*lplost+1));
				if(retlp)	return -lost;
				return 0;
			}else{
				Death(option);
			}
		}
		int lost=int((dmg-1)*deathlost+1);
		mHP=0;
		if(!active){
			int ret=LostLP(lost);
			//if(retlp)	ret=-ret;
			return ret;
		}else{
			int lp=mCData->GetLP();
			lost+=int(lp*lplost);
			return -LostLP(lost);
		}
	}
	int lost=LostLP(int((dmg-1)*lplost+1));
	if(retlp)	return -lost;
	return 0;
}
void	CBattleCharacter::SetDamageEffect(bool crt)
{
	mCrtFlash=crt;

	if(SearchStatus(15)>0)	return;
	bool active=!GetDead();
	if(active)	ChangeAni(8*ANI_TYPE,16,true);
	mAvoidTimer=0;
	mDamageTimer=DAMAGE_TIMER;
}
int	CBattleCharacter::HealHP(int heal)
{
	mHP+=heal;
	CheckLPOver();
	SetStatus(23,0);
	SetDeathBlack(0);
	return heal;
}
int	CBattleCharacter::HealLP(int heal)
{
	if(SearchASkill(403))	return 0;
	mCData->HealLP(heal);
	return heal;
}
int CBattleCharacter::LostLP(int pow)
{
	if(SearchASkill(403))	return 0;
	if(SearchASkill(267))	pow/=4;
	mCData->LostLP(pow);
	CheckLPOver();
	return pow;
}
int	CBattleCharacter::HealSP(int lv,int heal)
{
	mCData->HealSP(lv,heal);
	return heal;
}
bool	CBattleCharacter::SetStatus(int st,int param)
{
	if(param<=0){
		ClearStatus(st);
		return false;
	}
	for(int i=0;i<STATUS_NO;i++){
		bool set=false;
		if(mStatus[i]<0){
			set=true;
		}
		if(mStatus[i]==st || set){
			mStatus[i]=st;
			mStatusParam[i]=param;
			switch(st){
				case 2:	mCharm=true;	break;
				case 7: Death(0);		break;
			}
			return true;
		}
	}

	return false;
}
bool	CBattleCharacter::ClearStatus(int st)
{
	for(int i=0;i<STATUS_NO;i++){
		if(mStatus[i]==st){
			for(int j=i;j<STATUS_NO-1;j++){
				mStatus[j]		=mStatus[j+1];
				mStatusParam[j]	=mStatusParam[j+1];
			}
			mStatus[STATUS_NO-1]=-1;
			mStatusParam[STATUS_NO-1]=-1;
		}
	}
	return false;
}
int		CBattleCharacter::SearchStatus(int st)
{
	for(int i=0;i<STATUS_NO;i++){
		if(mStatus[i]==st){
			return mStatusParam[i];
		}
	}
	return -1;
}
int		CBattleCharacter::GetStatusAll(int* st,int* param)
{
	int no=0;
	for(int i=0;i<STATUS_NO;i++){
		if(mStatus[i]>=0){
			if(st)		st[no]=mStatus[i];
			if(param)	param[no]=mStatusParam[i];
			no++;
		}
	}
	return no;
}
int		CBattleCharacter::GetDrawStatus(void)
{
	int	st[STATUS_NO];
	int size=GetStatusAll(st);
	if(size<=0)	return -1;

	return st[(Flag.mPlayTime/STATUS_ANI_TIME/4)%size];
}
bool	CBattleCharacter::ClearBadStatus()
{
	bool ret=false;
	for(int i=0;i<STATUS_NO;i++){
		if(mStatus[i]>=0){
			if(StatusData[mStatus[i]].mBad){
				mStatus[i]=-1;
				mStatusParam[i]=-1;
				ret=true;
			}
		}
	}
	return ret;
}
bool	CBattleCharacter::ClearGoodStatus()
{
	bool ret=false;
	for(int i=0;i<STATUS_NO;i++){
		if(mStatus[i]>=0){
			if(!StatusData[mStatus[i]].mBad){
				mStatus[i]=-1;
				mStatusParam[i]=-1;
				ret=true;
			}
		}
	}
	return ret;
}
void	CBattleCharacter::ClearAllStatus(void)
{
	memset(mStatus,-1,sizeof(mStatus));
	memset(mStatusParam,-1,sizeof(mStatusParam));
}
void	CBattleCharacter::SetAvoid()
{
	mDamageTimer=0;
	mAvoidTimer=AVOID_TIMER;
	ChangeAni(4*ANI_TYPE,AVOID_TIMER,true);
}
bool CBattleCharacter::CheckLPOver(void)
{
	if(!mLive)		return false;
	if(GetMaxHP()<GetHP()){
		mHP=GetMaxHP();
		return true;
	}
	return false;
}
bool CBattleCharacter::GetActive()
{
	if(GetDead())			return false;
	if(SearchStatus(1)>0)	return false;
	if(SearchStatus(26)>0)	return false;
	return true;
}
bool	CBattleCharacter::GetDead()
{
	if(!mLive)
		return true;
	if(mHP<=0)
		return true;
	return false;
}
int	CBattleCharacter::GetMaxHP()
{
	int lp=mCData->GetLP();
	return lp;
}
int	CBattleCharacter::GetMaxSP(int no)
{
	int sp=mCData->GetMaxSP(no);
	return sp;
}
int	CBattleCharacter::GetAtk()
{
	int atk=mCData->GetAtk();
	atk+=mAtkUp;
//執念補正
	if(SearchASkill(385)){
		int lp=(GetMaxLP()-GetHP())*100/GetMaxLP();
		atk+=int(atk*lp*0.005);
	}

	if(atk<=0)		return 1;
	if(atk>9999)	return 9999;
	return atk;
}
int	CBattleCharacter::GetDef()
{
	int def=mCData->GetDef();
	def+=mDefUp;
//睡眠・腐敗補正
	if(SearchStatus(6)>0 || SearchStatus(1)>0)			def/=2;

	if(def<=0)		return 1;
	if(def>9999)	return 9999;
	return def;
}
int	CBattleCharacter::GetMagAtk()
{
	int matk=mCData->GetMagAtk();
	matk+=mMagAtkUp;
//執念補正
	if(SearchASkill(385)){
		int lp=(GetMaxLP()-GetHP())*100/GetMaxLP();
		matk+=int(matk*lp*0.005);
	}

	if(matk<=0)		return 1;
	if(matk>9999)	return 9999;
	return matk;
}
int	CBattleCharacter::GetMagDef()
{
	int mdef=mCData->GetMagDef();
	mdef+=mMagDefUp;
//睡眠・腐敗補正
	if(SearchStatus(6)>0 || SearchStatus(1)>0)		mdef/=2;

	if(mdef<=0)		return 1;
	if(mdef>9999)	return 9999;
	return mdef;
}
int	CBattleCharacter::GetSpeed()
{
	int spd=mCData->GetSpeed();
	spd+=mSpdUp;
	if(SearchStatus(9)>0)	spd/=2;
	if(spd<=0)	spd=1;
	if(spd>999)	spd=999;
	return spd;
}

int	CBattleCharacter::GetTech()
{
	int tech=mCData->GetTech();
	return tech;
}
int	CBattleCharacter::SetParamUp(int *old,int up)
{
	int ret=0;
	if((*old<0)!=(up<0)){
		ret=up;
		*old=*old+up;
	}else{
		int min,max;
		int aold=abs(*old);
		int aup =abs(up);
		if(aold>aup){
			min=aup;	max=aold;
		}else{
			min=aold;	max=aup;
		}
		int after=max/2 + min;
		if(after<max)	after=max;
		if(after>9999)	after=9999;
		if(up<0)		after*=-1;

		ret=after - *old;
		*old=after;
	}
	return ret;
}
int		CBattleCharacter::GetUsableCSkill(int* skill)
{
	if(!skill)	return 0;

	int		size=0;
	int		allskill[MAX_CSKILL];
	bool	sp[MAX_CSKILL];

	if(SearchStatus(3)<=0){
		size+=mCData->GetCSkillAll(CS_SKILL,&(allskill[size]),&(sp[size]));
	}
	if(SearchStatus(4)<=0){
		size+=mCData->GetCSkillAll(CS_MAGIC,&(allskill[size]),&(sp[size]));
	}
	size+=mCData->GetCSkillAll(CS_OTHER,&(allskill[size]),&(sp[size]));

	int ret=0;
	for(int i=0;i<size;i++){
		if(sp[i]){
			skill[ret]=allskill[i];
			ret++;
		}
	}

	return ret;
}
bool	CBattleCharacter::CheckAir(void)
{
	if(SearchStatus(9)>0)	return false;
	if(SearchStatus(13)>0)	return true;
	if(SearchASkill(242))	return true;
	return false;
}
int		CBattleCharacter::GetAction(bool force)
{
	if(!GetActive() && !force)	return -1;
	if(SearchStatus(5)>0 && mAction>=0){
		int sp=ComSkillData[mAction].mSP;
		mAction=sp+160;
	}
	return mAction;
}
int		CBattleCharacter::GetActionNo()
{
	int actno=SearchASkillType(AS_MULTI_ACTION,false);
	if(actno<1)	actno=1;
	
	if(SearchASkill(403)){
		int n=((mHP-1)*4/GetMaxLP());
		if(n<1)	return actno;
		else if(BattleMode.GetTurn()<3)	actno+=2;
	}
	return actno;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void	CPlayerCharacter::LoadCharacter(int no,int x,int y,int param)
{
	if(no<0){
		mLive=false;
		return;
	}
	mCData=&Flag.mPlayerData[no];
	mPData=&Flag.mPlayerData[no];

	CBattleCharacter::LoadCharacter(no,x,y,param);
	mSecondWeapon=false;
	LoadWeaponImg();

	mX+=320;
	SetGoalSquare(mSX,mSY,20,GetSize(),false);
	mTurnWeapon=0;
//	if(rand()%2)	mHP=0;
//	SetStatus(4,100);
//	SetStatus(4,100);
}
void	CPlayerCharacter::LoadWeaponImg(void)
{
	int weapon=mPData->GetWeapon();
	LoadWeaponImg(weapon,&mWeaponImg);

	int secwep=mPData->GetSecondWeapon();
	if(secwep!=NO_ITEM){
		LoadWeaponImg(secwep,&mSecondWeaponImg);
		mSecondWeapon=true;
	}else{
		mSecondWeapon=false;
	}
}
void	CPlayerCharacter::LoadWeaponImg(int weapon,CDTTexture *tex)
{
	char filename[256];
	if(weapon==NO_ITEM)	return;
	int img=EquipmentData[weapon].mIcon;

	sprintf(filename,"data\\battlecharacter\\weapon%02d",img/16);
	DTest.LoadTexture(*tex,filename,0xff008080);
}

void	CPlayerCharacter::GetDrawPositon(int &x,int &y,int &z)
{
	x=(int)mX+BATTLE_SQUARE_X;
	y=(int)mY+BATTLE_SQUARE_Y;
	z=(int)mZ;

	if(mDamageTimer>0)	x+=(mDamageTimer%2)*4;

	if(mAvoidTimer>0){
		double s=sin((mAvoidTimer*3.141592)/AVOID_TIMER);
		x+=int(s*AVOID_X);
		y-=int(s*AVOID_Y);
		z+=int(s*AVOID_Z);
	}
}
void	CPlayerCharacter::OnDraw(int x,int y,D3DCOLOR color,bool weapon)
{
	if(!mLive)	return;
	int dx,dy;
	bool turn;
	GetDrawData(dx,dy,turn);


	int size=64;

	if(weapon)	DrawWeapon(x,y,color,turn,false);
	DTest.DrawTexture(mImg,(turn?TURN_HORIZONTAL:0),
						dx*size,dy*size,
						size,size,
						x,y,size*2,size*2,color);
	if(weapon)	DrawWeapon(x,y,color,turn,true);
}
void	CPlayerCharacter::DrawWeapon(int x,int y,D3DCOLOR color,bool turn,bool top)
{
	if(top==AnimationData[mNo][mAni].mTop){
		int weapon=mPData->GetWeapon();
		DrawWeapon(	x,y,color,turn,
					weapon,&mWeaponImg,AnimationData[mNo][mAni]);
	}

	if(mSecondWeapon){
		if(top==AnimationData[mNo+MAX_CHARACTER_NUMBER][mAni].mTop){
			int weapon=mPData->GetSecondWeapon();
			DrawWeapon(	x,y,color,turn,
						weapon,&mSecondWeaponImg,
						AnimationData[mNo+MAX_CHARACTER_NUMBER][mAni]);
		}
	}
}
void	CPlayerCharacter::DrawWeapon(int x,int y,D3DCOLOR color,bool turn,int weapon,CDTTexture *tex,const CAnimationData& anidata)
{
	int img=EquipmentData[weapon].mIcon%16;

	int wx,wy;
	bool wepturn;
	double rad;
	GetDrawWeaponData(&wx,&wy,&rad,&wepturn,turn,weapon,anidata);

	DTest.DrawTextureRotate(*tex,(wepturn?TURN_HORIZONTAL:0),
							(img%8)*32,(img/8)*128,
							32,128,
							wx+x,wy+y,32,128,color,
							rad);
}
void	CPlayerCharacter::GetDrawWeaponData(int *x,int *y,double *rad,bool *turn)
{
	int dx,dy;
	bool t;
	GetDrawData(dx,dy,t);
	GetDrawWeaponData(x,y,rad,turn,t,mPData->GetWeapon(),AnimationData[mNo][mAni]);
}
void	CPlayerCharacter::GetDrawWeaponData(int *x,int *y,double *rad,bool *wepturn,bool turn,int weapon,const CAnimationData& anidata)
{
	*x=anidata.mX*2;
	*y=anidata.mY*2;
	*wepturn=anidata.mTurn;
	*rad=anidata.mRad+mTurnWeapon;
	if(turn){
		*x=128-(*x);
		*rad=-(*rad);
	}

	if(!anidata.mView || weapon==NO_ITEM){
		*x=65535;
	}
}
void	CPlayerCharacter::DeleteAllUseItem(void)
{
	if(!mLive)		return;

	int itemmax=mPData->GetItemMax();
	for(int i=0;i<itemmax;i++){
		CarryItem ui = mPData->GetItem(i);
		const unsigned int &type = AllItem[ui.item]->mType;

		if((ui.param>0) && (type & TYPE_USE_ITEM) && (type & TYPE_ON_BATTLE) && (type & TYPE_SPEND)){	
			mPData->SetItem(i,NO_ITEM);
		}
	}
}
int	CPlayerCharacter::SearchUseItem(int no)
{
	if(!mLive)		return false;

	int itemmax=mPData->GetItemMax();
	for(int i=0;i<itemmax;i++){
		CarryItem ui = mPData->GetItem(i);
		const unsigned int &type = AllItem[ui.item]->mType;

		if((ui.param>0) && (type & TYPE_USE_ITEM) && (type & TYPE_ON_BATTLE)){
			if(no<=0)	return ui.item;
			no--;
		}
	}
	return -1;
}
bool	CPlayerCharacter::SetUseItem(int item)
{
	if(!mLive)		return false;

	CarryItem ui = mPData->GetItem(item);
	const unsigned int &type = AllItem[ui.item]->mType;

	if((ui.param>0) && (type & TYPE_USE_ITEM) && (type & TYPE_ON_BATTLE)){
		mUseItem=item;
		SetAction(( type & 0xff) + ITEM_CSKILL);
		return true;
	}
	return false;
}

bool	CPlayerCharacter::SpendUseItem(void)
{
	if(!mLive)		return false;

	int ui = mPData->GetItem(mUseItem).item;
	const unsigned int &type = AllItem[ui]->mType;

	if(type & TYPE_USE_ITEM){
		if(type & TYPE_SPEND){
			mPData->SpendUseItem(mUseItem);
		}
		return true;
	}
	return false;
}
void	CPlayerCharacter::GetUseItemParam(int* parama,int* paramb)
{
	int item=GetUseItem() - ID_UI;
	if(item<0 || MAX_USE_ITEM_NUMBER<item)	CBattleCharacter::GetUseItemParam(parama,paramb);

	if(parama){
		*parama=UseItemData[item].mParamA;
		if(mPData->SearchASkill(271)){
			*parama=(*parama * 3 + 1)/2;
		}
	}
	if(paramb)	*paramb=UseItemData[item].mParamB;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
CEnemyCharacter::~CEnemyCharacter()
{
}
void	CEnemyCharacter::LoadCharacter(int no,int lv,int x,int y,int param,bool leader)
{
	mEData.LoadEnemyData(no,lv);
	mCData=&mEData;

//オプションによるパラメータアップ
	bool paramup=true;
	if(leader && (param & BOPT_P_UP_WOUT_LEADER))		paramup=false;
	if(!leader && (param & BOPT_P_UP_LEADER_ONLY))		paramup=false;
	if(GetSize()>=2 && (param & BOPT_P_UP_WOUT_BIG))	paramup=false;
		
	if(paramup){
//ここまでは条件判定
		if(param & BOPT_LPUP_x2){
			mEData.SetMaxLP(mEData.GetLP()*2);
		}
		if(param & BOPT_LPUP_x3){
			mEData.SetMaxLP(mEData.GetLP()*3);
		}
		if(param & BOPT_LP_HALF){
			mEData.SetMaxLP(mEData.GetLP()/2);
		}
		if(param & BOPT_POWUP_x15){
			mEData.SetAtk	(mEData.GetAtk()	*3/2);
			mEData.SetMagAtk(mEData.GetMagAtk()	*3/2);
		}
		if(param & BOPT_POWUP_x40){
			mEData.SetAtk	(mEData.GetAtk()	*4);
			mEData.SetMagAtk(mEData.GetMagAtk()	*4);
		}
		if(param & BOPT_POW_HALF){
			mEData.SetAtk	(mEData.GetAtk()	/2);
			mEData.SetMagAtk(mEData.GetMagAtk()	/2);
		}
		mEData.CheckParamMax();
	}
//パラメータアップここまで

	CBattleCharacter::LoadCharacter(no+100,x,y,param);

	mX-=320;
	SetGoalSquare(mSX,mSY,20,GetSize(),false);

	mDeathBlack=false;
}
bool	CEnemyCharacter::CheckOpenData(int no)
{
#ifdef _MONSTER_KNOWLEDGE_
	return true;
#endif
	if(SearchASkill(305)){
		return false;
	}
	if(Flag.mBiology)	return true;
	return Flag.mEnemyOpenData[mNo-100].Get(no);
}
void	CEnemyCharacter::SetOpenData(int no)
{
	Flag.mEnemyOpenData[mNo-100].Set(no);
}
void	CEnemyCharacter::OpenCSkill(int no)
{
	for(int i=0;i<MAX_CSKILL;i++){
		if(mEData.GetCSkill(i)==no){
			SetOpenData(i+ODATA_CS);
			return;
		}
	}
}

int		CEnemyCharacter::GetUsableCSkillWithParam(int* skill,int* param)
{
	if(!skill)	return 0;
	if(!param)	return 0;

	int		size=0;
	int		allskill[MAX_CSKILL];
	int		allparam[MAX_CSKILL];
	bool	sp[MAX_CSKILL];

	if(SearchStatus(3)<=0){
		size+=mEData.GetCSkillAll(CS_SKILL,&(allskill[size]),&(sp[size]),&(allparam[size]));
	}
	if(SearchStatus(4)<=0){
		size+=mEData.GetCSkillAll(CS_MAGIC,&(allskill[size]),&(sp[size]),&(allparam[size]));
	}
	size+=mEData.GetCSkillAll(CS_OTHER,&(allskill[size]),&(sp[size]),&(allparam[size]));
	size+=mEData.GetCSkillAll(CS_ITEM,&(allskill[size]),&(sp[size]),&(allparam[size]));

	int ret=0;
	for(int i=0;i<size;i++){
		if(sp[i]){
			skill[ret]=allskill[i];
			param[ret]=allparam[i];
			ret++;
		}
	}

	return ret;
}


/////////////////////////////////////////////////////////////////////////////////////////////////
void	CPetCharacter::LoadCharacter(int no,CCharacterData* master,int x,int y,int param)
{
	mCData=&mCharacterData;
	const CPetData& pet=PetData[no];

	int lp	=pet.mLP	* master->GetLP()		/100;
	int atk	=pet.mAtk	* master->GetAtk()		/100;
	int def	=pet.mDef	* master->GetDef()		/100;
	int matk=pet.mMagAtk* master->GetMagAtk()	/100;
	int mdef=pet.mMagDef* master->GetMagDef()	/100;
	int spd	=pet.mSpeed	* master->GetSpeed()	/100;
	int tech=pet.mTech	* master->GetTech()		/100;

	mCharacterData.Init(no,pet.GetName(),master->GetLv(),lp,atk,def,matk,mdef,spd,tech,pet.mSize);
	mCharacterData.SetMaxLP(lp);

	for(int i=0;i<pet.mSkillNo;i++){
		int &skill=pet.mSkill[i];

		if(SkillData[skill].mType==SKILL_AUTO){
			mCharacterData.SetASkill(skill);
		}else if(SkillData[skill].mType==SKILL_COMMAND){
			mCharacterData.SetCSkill(SkillData[skill].mParam);
		}
	}

	if(master->SearchASkill(404)){
		mCharacterData.SetASkill(168);
	}
	if(master->SearchASkill(405)){
		mCharacterData.SetASkill(172);
		mCharacterData.SetASkill(180);
	}
	if(master->SearchASkill(406)){
		mCharacterData.SetASkill(176);
		mCharacterData.SetASkill(184);
	}
	if(master->SearchASkill(407)){
		mCharacterData.SetASkill(190);
	}

	mCharacterData.CalculateSkill();

	CBattleCharacter::LoadCharacter(no,x,y,param);

	mX+=320;
	CBattleCharacter::SetGoalSquare(mSX,mSY,20,GetSize(),false);
}
void	CPetCharacter::LoadImage(int no)
{
	CBattleCharacter::LoadImage(PetData[no].mImg);
}
/////////////////////////////////////////////////////////////////////////////////////////////////
void	CWallCharacter::LoadCharacter(int no,int hp,int x,int y,int param)
{
	mCData=&mCharacterData;
	mCharacterData.Init(no,"土壁",0,hp,0,0,0,0,0,0);
	mCharacterData.SetASkill(207);
	mCharacterData.SetASkill(209);

	CBattleCharacter::LoadCharacter(0,x,y,param);

	CBattleCharacter::SetGoalSquare(mSX,mSY,20,GetSize(),false);
}
void	CWallCharacter::LoadImage(int no)
{
	DTest.LoadTexture(mImg,"data\\battleeffect\\battleeffect48",0xff008080);
}
void	CWallCharacter::GetDrawData(int &dx,int &dy,bool &turn)
{
	dx=0;
	dy=0;
	if(mHP<=0){		dx=1;		dy=1;}
	else if(mHP<GetMaxLP()*0.3)	dy=1;
	else if(mHP<GetMaxLP()*0.6)	dx=1;

	turn=false;
}