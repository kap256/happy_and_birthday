#pragma once


#include "main.h"
#include "element.h"
#include "characterdata.h"

const int WINDOW_OPEN_TIME	=12;
const int MESSAGE_SPACE     =6;
class CPlaneWindow{
public:
	static void	GetMessageSize(int fontnum,const char *text,int *x,int *y);
private:
	int		mTimer;

protected:

	bool	mLive;
	double	mX;
	double	mY;
	int		mWidth;
	int		mHeight;

	int	mGoalX;
	int	mGoalY;
	int mBaseX;
	int mBaseY;
protected:
	void	DrawWindow(int x,int y,int w,int h,D3DCOLOR color=0xffffffff);
	void	GetWindowSize(int* w,int *h);

public:
					CPlaneWindow();
	virtual			~CPlaneWindow();

			void	InitPlaneWindow(int x,int y,int w,int h,bool opened=false);
			void	QuickDestroy(void);
	virtual void	Destroy(void);
	virtual	void	OnTimer(void);
	virtual	void	OnDraw(D3DCOLOR color=0xffffffff);

			void	DrawText(int fontnum,const char *text,int x,int y,D3DCOLOR color=0x00000000,int position=DT_LEFT);
			void	DrawBox(int x,int y,int w,int h,D3DCOLOR color);
			void	DrawTexture(CDTTexture* tex,int turn,int fx,int fy,int fsx,int fsy,int tx,int ty,D3DCOLOR color=0xffffffff);
			void	DrawTexture(CDTTexture* tex,int turn,int fx,int fy,int fsx,int fsy,int tx,int ty,int tsx,int tsy,D3DCOLOR color=0xffffffff);

		//空白つきテキスト出力
			void	DrawMessage(int fontnum,const char *text,int x,int y,D3DCOLOR color=0x00000000,int position=DT_LEFT);

	bool	GetLive(){return mLive;};
	bool	GetInited(){return (mTimer==WINDOW_OPEN_TIME);};
	bool	GetDestroy(){if(!mLive)	return true;return (mTimer>WINDOW_OPEN_TIME);};
	
	int		GetX(){return (int)mX;};
	int		GetY(){return (int)mY;};
	int		GetWidth(){return mWidth;};
	int		GetHeight(){return mHeight;};
	int		GetTimer(){return mTimer;};

	virtual void	Move(int x,int y)		{mGoalX=x;mGoalY=y;};
	virtual void	Open(void)				{if(mTimer<WINDOW_OPEN_TIME) mTimer=WINDOW_OPEN_TIME;};
	virtual void	QuickMove(int x,int y)	{mX=x;mY=y;mGoalX=x;mGoalY=y;};
	virtual void	Home()					{mGoalX=mBaseX;mGoalY=mBaseY;};
	virtual void	BaseMove(int x,int y)	{mBaseX=x;mBaseY=y;Home();};
};

class CListWindow:public CPlaneWindow{
protected:
	CElement const**	mList;
	int		*mParam;
	int		mElementHeight;
	int		mListHeight;
	int		mListSize;
	int		mCursor;
	bool	mShowCursor;
	int		mPage;
	double	mPageY;
	bool	mPageMove;

public:
	CListWindow();
	~CListWindow();
	void	InitYesNoWindow(int x,int y,int w);
	virtual void	InitListWindow(int x,int y,int w,const CElement **listdata,int* list,int listsize,int h,int *param=NULL,bool pagemove=true);
	virtual void	InitListWindow(int x,int y,int w,const CElement *listdata,int* list,int listsize,int h,int size,int *param=NULL,bool pagemove=true);

	void	ResetList(const CElement **listdata,int* list,int listsize,int *param=NULL);
	void	ResetList(const CElement *listdata,int* list,int listsize,int size,int *param=NULL);

	void	SetParam(int no,int param);
	void	SetParam(int* param);
	int		GetParam(int no){return mParam[no];};

	virtual void	Destroy(void){CPlaneWindow::Destroy();};
	virtual	int		OnTimer(bool cursor);
	virtual	void	OnDraw(D3DCOLOR color=0xffffffff);

	void			SetCursor(int cursor);
	void			ShowCursor(bool cursor){mShowCursor=cursor;};

	void			SetItem(int no,const CElement* element);

	const CElement*	GetCursorItem(void){return GetItem(mCursor);};
	const CElement*	GetItem(int i){if(i>=mListSize)return NULL; return mList[i];};
	int				GetCursor(void){return mCursor;};
	virtual void	GetCursorPos(int *x,int *y);
	int				GetListSize(void){return mListSize;};
};
class CItemListWindow:public CListWindow{
	static const int INFO_X = -10;
	static const int INFO_Y = -20;

	CPlayerData*	mPData;

public:
	CItemListWindow(){mPData=NULL;};

	void			SetPData(CPlayerData &pdata);
	virtual void	InitListWindow(int x,int y,int w,const CElement **listdata,int* list,int listsize,int h,int *param=NULL,bool pagemove=true);
	virtual void	InitListWindow(int x,int y,int w,const CElement *listdata,int* list,int listsize,int h,int size,int *param=NULL,bool pagemove=true);

	virtual	void	OnDraw(D3DCOLOR color=0xffffffff);

	virtual void	GetCursorPos(int *x,int *y);
private:
	void DrawInfo(int x,int y,int cur,int max);
	void DrawSeparate(const char* name,int no,int y,D3DCOLOR textcolor);
};

const int SWIN_MOVETIME	= WINDOW_OPEN_TIME;

class CSelectWindow:public CPlaneWindow{
	CElement const**	mList;
	int		mListSize;
	int		mCursor;
	int		mCursorTimer;
	int		mX;
	int		mSpeedX;
	int		mMoveTimer;
	int		mInWidth;

public:
	CSelectWindow();
	~CSelectWindow();
	void	InitSelectWindow(int x,int y,int w,const CElement **listdata,int* list,int listsize);
	void	InitSelectWindow(int x,int y,int w,const CElement *listdata,int* list,int listsize,int size);
	int		OnTimer(bool cursor);
	void	OnDraw(D3DCOLOR color=0xffffffff);
	
	const	CElement*	GetCursorItem(void){return mList[mCursor];};
	int		GetCursor(void){return mCursor;};
	void	SetCursor(int c){mCursor = c%mListSize ; if(mCursor<0) mCursor+=mListSize;};

	void	InitArrow();
};

class CBattleCharacter;
class CCharaDataWindowMini:public CPlaneWindow{
	static const int TEXT_Height	=	15;
	static const int WIN_WIDTH	=	120;
	static const int WIN_Height	=	TEXT_Height*8+16;

	CBattleCharacter*	mCData;
public:
	CCharaDataWindowMini();
	void	InitWindow(CBattleCharacter* cdata);
	void	OnTimer(void);
	void	OnDraw(D3DCOLOR color=0xffffffff);
};

class CCharaDataWindowMenu:public CPlaneWindow{

	int			mNo;
	bool		mReserve;
	CDTTexture  mTex;

public:
	static const int WIN_WIDTH	=	320;
	static const int WIN_Height	=	100;


	CCharaDataWindowMenu();
	void	InitWindow(int x,int y,int no,bool reserve=false);
	void	OnDraw(bool select=false,D3DCOLOR color=0xffffffff);
};

/*
class CCharaDataWindowHalf:public CPlaneWindow{

	int			mNo;
public:
	static const int WIN_WIDTH	=	260;
	static const int WIN_Height	=	300;


	CCharaDataWindowHalf();
	void	InitWindow(int x,int y,int no);
	void	OnDraw(D3DCOLOR color=0xffffffff);
};
*/
class CCharaDataWindowCompare:public CPlaneWindow{
protected:
	CPlayerData*	mBefore;
	CPlayerData*	mAfter;
public:
	static const int WIN_WIDTH	=	320;
	static const int WIN_Height	=	ELEMENT_Height*9+16;

	CCharaDataWindowCompare();
	void	InitWindow(int x,int y,CPlayerData* before,CPlayerData* after);
	void	OnDraw(D3DCOLOR color=0xffffffff);

	void	SetBefore(CPlayerData* before)	{mBefore=before;};
	void	SetAfter(CPlayerData* after)	{mAfter=after;};
private:
	void	DrawLine(int line,const char* title,int before,int after);
};
class CCharaDataWindowEquip:public CCharaDataWindowCompare{

	CPlayerData	mAfter;
	int			mEquip;
	bool		mCarry;

public:
	void	InitWindow(int x,int y,int equip,CPlayerData* before,bool carry=false);
	void	SetEquip(int equip,int no=-1);
	void	SetEquipCarry(int no);
};

class CItemHelpWindow:public CPlaneWindow{

	int		mItemNo;
	char*	mHelp;

public:
	static const int WIN_WIDTH	=	320;
	static const int WIN_Height	=	120;

	static void DrawParam(CPlaneWindow* win,int x,int y,int item,int max,bool force = false);
	static void	DrawParam(CPlaneWindow* win,int x,int y,int max,const char* name,int param,int i,double ratio=1);

			CItemHelpWindow();
	virtual	~CItemHelpWindow();

	void	InitWindow(int x,int y,int item);
	void	OnDraw(D3DCOLOR color=0xffffffff);
	void	ChangeItem(int item);
};

class CBattleAction;

class CBattleActionNameWindow:public CPlaneWindow{
	CBattleAction*	mAction;
	int				mNo;
public:
	CBattleActionNameWindow(CBattleAction* act);
	void	OnDraw();

	CBattleAction* GetAction(){return mAction;}
};
