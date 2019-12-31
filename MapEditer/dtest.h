#ifndef _DTEST_
#define _DTEST_

#include <d3d9.h>
#include <d3dx9.h>
#include <d3dx9math.h>
#include <dxerr9.h>
#include <dinput.h>
#include <math.h>
#include <stdio.h>

#include "dtestsurface.h"
#include "dtesttexture.h"
#include "dtestfont.h"

#define TRC_NONE	0x00
#define TRC_ZERO	0x01

#define TURN_VERTICAL	1
#define TURN_HORIZONTAL	2

#define BLEND_STANDARD	0
#define BLEND_NONE		1
#define BLEND_ADD		2
#define BLEND_HOLL		3
#define BLEND_SOLID		4
#define BLEND_SOLID_ADD	5
//#define BLEND_TEST	256

#define JOY_LEFT		0
#define JOY_UP			1
#define JOY_RIGHT		2
#define JOY_DOWN		3
#define JOY_0			4
#define JOY_1			5
#define JOY_2			6
#define JOY_3			7
#define JOY_4			8
#define JOY_5			9
#define JOY_6			10
#define JOY_7			11
#define JOY_8			12
#define JOY_9			13
#define JOY_10			14
#define JOY_11			15
#define JOY_12			16
#define JOY_13			17
#define JOY_14			18
#define JOY_15			19

#define MAX_FONT		10

class CDirectTest{
	HWND*		mHWnd;
	HINSTANCE*	mHInstance;
	int			mCmdShow;

	LPDIRECT3D9			lpD3D;
	LPDIRECT3DDEVICE9	lpD3DDevice;
	LPD3DXSPRITE		lpD3DSprite;

	CDTSurface BackBuffer;

	LPDIRECT3DSURFACE9	lpTarget;
	bool				mTargeTex;
	int					mTargetWidth;
	int					mTargetHeight;

	CDTFont				mFont[MAX_FONT];
	D3DCOLOR			mFontColor[MAX_FONT];
	bool				mTextMode;

	LPDIRECTINPUT8			lpDInput;
	IDirectInputDevice8*	lpDIDKey;
	IDirectInputDevice8*	lpDIDMouse;
	IDirectInputDevice8*	lpDIDJoy;

	int mKeyData[256];
	int mMouseButton[4];
	int mJoyButton[36];

	bool			mInited;
	bool			mFullScreen;
	bool			mShowCursor;

	void			DrawStart(void);
	void			CreateDInput(HWND *hWnd,HINSTANCE* hInstance);
	void			GetInputData(void);
	void			DrawEnd(void);
	bool			SetRenderTarget(LPDIRECT3DSURFACE9 lpSurface,bool tex);
	bool			GetBMPSize(FILE* pFile,int *x,int *y,char BM);
	bool			GetBMPSize(const char *filename,int *x,int *y);
	bool			GetBMPSize(FILE* pFile,int *x,int *y);
	bool			GetFixedBMP(const char *filename,unsigned char ** bmpdata,int *x,int *y,int *filesize,D3DCOLOR *color);


public:
	CDirectTest(void);
	~CDirectTest(void);
	bool MakeD3D(HWND *hWnd,HINSTANCE* hInstance,int nCmdShow);
	void ResetDTest(void);
	void ResetTexuture(CTextureList* tex);
	bool Release(bool deltex);

	void GetInputDevice(LPDIRECTINPUT8** dinput,IDirectInputDevice8*** didjoy);
	void OnTimer(void);
	void OnDraw(void);

	bool GetInited(void){return mInited;};

	HWND GetHWnd(){return *mHWnd;};

	bool mActive;

//ユーザーが使えるもの
	void Exit(void);

	int  GetKey(int key);
	int  GetMouseButton(int button);
	void GetMousePos(int* x,int* y);
	int  GetJoyButton(int button);

	bool LoadSurface(CDTSurface *surface,const char *filename);
	bool DrawSurface(CDTSurface *surface,int fx,int fy,int sx,int sy,int tx,int ty);
	bool DrawSurface(CDTSurface *surface,int fx,int fy,int fsx,int fsy,int tx,int ty,int tsx,int tsy);
//	bool CreateSurface(CDTSurface *surface,int x,int y);	我が家のノーパ環境で動かず。
//	bool SetRenderTargetS(CDTSurface *surface);				我が家のノーパ環境で動かず。

	void Clear(D3DCOLOR color=0x00000000);
	bool DrawBox(int left,int top,int right,int bottom,D3DCOLOR color);
	bool DrawBox(int left,int top,int right,int bottom,D3DCOLOR color1,D3DCOLOR color2,D3DCOLOR color3,D3DCOLOR color4);
	bool DrawBox(int x1,int y1,int x2,int y2,int x3,int y3,int x4,int y4,D3DCOLOR color1,D3DCOLOR color2,D3DCOLOR color3,D3DCOLOR color4);
	bool DrawBoxRotate(int x,int y,int w,int h,D3DCOLOR color,double rad=0);
	bool DrawTriangle(int x1,int y1,int x2,int y2,int x3,int y3,D3DCOLOR color){return DrawTriangle(x1,y1,x2,y2,x3,y3,color,color,color);};
	bool DrawTriangle(int x1,int y1,int x2,int y2,int x3,int y3,D3DCOLOR color1,D3DCOLOR color2,D3DCOLOR color3);
	bool DrawLine(int fx,int fy,int tx,int ty,double width,D3DCOLOR color);

	bool DrawTexture(CDTTexture &texture,int turn,int fx,int fy,int sx,int sy,int tx,int ty){return DrawTexture(texture,turn,fx,fy,sx,sy,tx,ty,sx,sy);};

	bool					CreateTexture(CDTTexture &texture,int x,int y);
	LPDIRECT3DTEXTURE9		CreateTexture(int x,int y);
	LPDIRECT3DTEXTURE9		CreateTexture(const char *filename,D3DCOLOR color,int *x=NULL,int *y=NULL);
	bool		LoadTexture(CDTTexture &texture,const char *filename,D3DCOLOR color=0x00000000);
	bool		DrawTextureRotate(CDTTexture &texture,int turn,int fx,int fy,int fsx,int fsy,int tx,int ty,int tsx,int tsy,D3DCOLOR color=0xffffffff,double r=0);
	bool		DrawTexture(CDTTexture &texture,int turn,int fx,int fy,int fsx,int fsy,double tx,double ty,double tsx,double tsy,D3DCOLOR color=0xffffffff);
	bool		SetRenderTarget(CDTTexture *texture);
	bool		SetBlend(int blend);

	void StartTextMode(void);
	void EndTextMode(void);
	int	 GetFontSize(int fontnum){return mFont[fontnum].mSize;};
	bool CreateFont(int fontnum,int size=16,const char* fontname=NULL,int weight=0,bool italic=false);
	bool ChangeTextColor(int fontnum,D3DCOLOR color);
	bool DrawText(int fontnum,const char *text,int x,int y,D3DCOLOR color=0x00000000,int position=DT_LEFT,int sizex=65535,int sizey=65535);
	bool Printf(int fontnum,int x,int y,const char *text,...);
	bool GetTextSize(int fontnum,const char *text,int *x,int *y);

	void SetWindowSize(int x,int y);
	void FullScreen(bool screen);
	void ShowCursor(bool show);
	bool GetFullScreen(void){return mFullScreen;};
	void SetWindowTitle(const char* title);
	void SetFPS(int fps);
	void DrawSkip(bool skip);
};

extern void OnTimer();
extern void OnDraw();
extern void StartUp();
extern void AfterStartUp();
extern void End();
//extern void OnKeyDown(UINT nChar);
//extern void OnKeyUp(UINT nChar) ;

//ユーザーが使えるもの
extern CDirectTest DTest;

#endif