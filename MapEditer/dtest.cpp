
#include <windows.h>
#include <mmsystem.h>
#include <time.h>

#include "resource.h"
#include "dtest.h"

#define APP_NAME "DIRECT_TEST"

static int		gWinSizeX=640;
static int		gWinSizeY=480;
static char		gAppTitle[256]="DTest";

static bool		gDrawSkip=false;
static double	gOneFrame=16.66666666666;

// ウィンドウプロシージャ関数のプロトタイプ（メッセージの処理）
LRESULT CALLBACK WinProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
static void CALLBACK timeProc(UINT uID,UINT uMsg,DWORD dwUser,DWORD dw1,DWORD dw2);
bool MakeWindow(HINSTANCE *hInstance,HWND *hwnd,int nCmdShow);
CDirectTest DTest;

//変数
static HWND     hWnd = NULL;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, 
                   LPSTR lpCmdLine, int nCmdShow)
{
    MSG         msg;
	msg.message=0;

	StartUp();

    MakeWindow(&hInstance,&hWnd,nCmdShow);

	DTest.MakeD3D(&hWnd,&hInstance,nCmdShow);
	DTest.CreateFont(0,15,NULL,0);

	AfterStartUp();
	DTest.mActive	= true;

	double oldtime=timeGetTime();
	while(WM_QUIT != msg.message)
	{
		if(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		} else {
			DWORD time=timeGetTime();
			while(time-oldtime< gOneFrame){
				if(!DTest.mActive){
					Sleep(200);
				}else{
					Sleep(1);
				}
				time=timeGetTime();
			}
			if(!DTest.mActive)	continue;
			if(gDrawSkip){
				DTest.OnTimer();
				oldtime+=gOneFrame;
				if(time<oldtime+gOneFrame){
					DTest.OnDraw();
				}
				int n=0;
			}else{
				oldtime=timeGetTime();
				DTest.OnTimer();
				DTest.OnDraw();
			}
		}
	}

	End();

	DTest.Release(true);
    return int(msg.wParam);
}
/***********************************************************
 * ウィンドウプロシージャ関数の実体（メッセージの処理）
 ***********************************************************/
LRESULT CALLBACK WinProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    switch(Msg)
    {
	case WM_ACTIVATE:
			if(((WORD)(DWORD_PTR)((wParam)&0xffff)) == WA_INACTIVE){	// 非アクティブ化
				if(hWnd==DTest.GetHWnd()){
					DTest.mActive	= false;
				}
			}else{						// アクティブ化
				if(!DTest.mActive){
					if(hWnd==DTest.GetHWnd()){
						DTest.mActive	= true;
					}
				}
				if(DTest.GetInited() && DTest.GetFullScreen()){
					DTest.FullScreen(false);
					//DTest.FullScreen(true);
				}
			}
		break;
	case WM_DESTROY:
			if(hWnd==DTest.GetHWnd())
			PostQuitMessage(0);
			break;
//		case WM_KEYDOWN:
//			OnKeyDown(UINT(wParam));
//			break;
//		case WM_KEYUP:
//			OnKeyUp(UINT(wParam));
//			break;
		default:
			return DefWindowProc(hWnd, Msg, wParam, lParam);
    }

    return 0;
}
bool MakeWindow(HINSTANCE *hInstance,HWND *hwnd,int nCmdShow)
{
    WNDCLASSEX  wcex;

    wcex.cbSize         = sizeof(wcex);
    wcex.style          = 0;
    wcex.lpfnWndProc    = WinProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = *hInstance;
    wcex.hIcon          = LoadIcon(*hInstance, (LPCSTR)IDI_ICON1);
    wcex.hbrBackground  = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wcex.lpszMenuName   = NULL;
    wcex.lpszClassName  = APP_NAME;
    wcex.hIconSm        = LoadIcon(*hInstance, (LPCSTR)IDI_ICON1);
	wcex.hCursor	    = LoadCursor(NULL, IDC_ARROW);

    if(!RegisterClassEx(&wcex))
    {
 //       return 0;
    }
	int WinX=gWinSizeX+::GetSystemMetrics(SM_CXEDGE)+::GetSystemMetrics(SM_CXBORDER)+::GetSystemMetrics(SM_CXDLGFRAME);
	int WinY=gWinSizeY+::GetSystemMetrics(SM_CYEDGE)+::GetSystemMetrics(SM_CYBORDER)+::GetSystemMetrics(SM_CYDLGFRAME)+::GetSystemMetrics(SM_CYCAPTION);

	DWORD dwExStyle = WS_EX_APPWINDOW;

	if(DTest.GetFullScreen()){
		dwExStyle=WS_EX_TOOLWINDOW;
	}

    *hwnd = CreateWindowEx(
			dwExStyle, 
            APP_NAME, 
            gAppTitle , 
            WS_OVERLAPPED|WS_SYSMENU,
			CW_USEDEFAULT, 
            CW_USEDEFAULT, 
            WinX, 
            WinY, 
            NULL, 
            NULL, 
            *hInstance, 
            NULL 
			);

    ShowWindow(*hwnd, nCmdShow);
    UpdateWindow(*hwnd);

	return true;
}
BOOL CALLBACK EnumDevicesCallback(LPCDIDEVICEINSTANCE lpdidinstance,LPVOID pvRef)
{
	HRESULT hr;
	CDirectTest*			dt=(CDirectTest*)(pvRef);
	LPDIRECTINPUT8*			lpDInput;
	IDirectInputDevice8**	lpDIDJoy;
	dt->GetInputDevice(&lpDInput,&lpDIDJoy);

	hr=(*lpDInput)->CreateDevice( lpdidinstance->guidInstance,lpDIDJoy, NULL );
	if( FAILED(hr) )  return DIENUM_CONTINUE;
	
	DIDEVCAPS diDevCaps;
	diDevCaps.dwSize = sizeof(DIDEVCAPS);
	hr = (*lpDIDJoy)->GetCapabilities( &diDevCaps );
	if( FAILED(hr) ){
		(*lpDIDJoy)->Release();
		(*lpDIDJoy)=NULL;
		return DIENUM_CONTINUE;
	}

	return DIENUM_STOP;
}
BOOL CALLBACK EnumDeviceObjectsCallback(  LPCDIDEVICEOBJECTINSTANCE lpddoi,LPVOID pvRef)
{
	HRESULT hr;
	CDirectTest*			dt=(CDirectTest*)(pvRef);
	LPDIRECTINPUT8*			lpDInput;
	IDirectInputDevice8**	lpDIDJoy;
	dt->GetInputDevice(&lpDInput,&lpDIDJoy);

	DIPROPRANGE diprg;
	ZeroMemory( &diprg, sizeof(diprg) );
	diprg.diph.dwSize       = sizeof(diprg); 
	diprg.diph.dwHeaderSize = sizeof(diprg.diph); 
	diprg.diph.dwObj        = lpddoi->dwType;
	diprg.diph.dwHow        = DIPH_BYID;
	diprg.lMin              = -1000;
	diprg.lMax              = +1000;
	hr = (*lpDIDJoy)->SetProperty( DIPROP_RANGE, &diprg.diph );
	if( FAILED(hr) )  return DIENUM_STOP;

	return DIENUM_CONTINUE;
}
////////ここからDTest//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


CDirectTest::CDirectTest(void)
{
	lpD3D=NULL;
	lpD3DDevice=NULL;
	lpDInput=NULL;
	lpDIDKey=NULL;
	lpDIDMouse=NULL;
	lpDIDJoy=NULL;
	lpTarget=NULL;
	mTargeTex=false;

	for(int n=0;n<MAX_FONT;n++){
		mFontColor[n]=0xff000000;
	}

	mInited=false;
	mFullScreen=false;
	mShowCursor=true;
	mActive=true;
}
CDirectTest::~CDirectTest(void)
{
	Release(true);
}
bool CDirectTest::MakeD3D(HWND *hWnd,HINSTANCE* hInstance,int nCmdShow)
{
	mHWnd=hWnd;
	mHInstance=hInstance;
	mCmdShow=nCmdShow;

	lpD3D=Direct3DCreate9( D3D_SDK_VERSION );
	if(lpD3D==NULL)	return false;

	D3DDISPLAYMODE   dispmode;
	lpD3D->GetAdapterDisplayMode (D3DADAPTER_DEFAULT,&dispmode);

	D3DPRESENT_PARAMETERS param;
	ZeroMemory( &param, sizeof(param) );
	param.BackBufferWidth  = gWinSizeX;				 // 解像度・幅
	param.BackBufferHeight = gWinSizeY;				 // 解像度・高さ
	param.BackBufferFormat = dispmode.Format;		 // ピクセルフォーマット
	param.BackBufferCount  = 1;                      // バックバッファの数
	param.SwapEffect       = D3DSWAPEFFECT_DISCARD;  // 表示する方
	param.Windowed         = !mFullScreen;            // ウインドウモード

	// デバイス作成
	HRESULT result;
	result=lpD3D->CreateDevice(
		D3DADAPTER_DEFAULT,                  // プライマリディスプレイアダプタ
		D3DDEVTYPE_HAL,                     
		*hWnd,                               // ウインドウハンドル
		D3DCREATE_SOFTWARE_VERTEXPROCESSING, // ソフトウェアによる頂点処理
		// D3DCREATE_HARDWARE_VERTEXPROCESSING, // ハードウェアによる頂点処理
		&param,                              // パラメータ
		&lpD3DDevice
	);
	if(result!=D3D_OK) return false;

	lpD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE , TRUE); 
	lpD3DDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA); 
	lpD3DDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA ); 

//	lpD3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE,true ); 

	lpD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP , D3DTOP_MODULATE);		// テクスチャステージステート　テクスチャブレンドで色を乗算合成
	lpD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);		// テクスチャステージステート　色のパラメータ2はディフューズ値
	lpD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);		// テクスチャステージステート　色のパラメータ1はテクセル
	lpD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP , D3DTOP_MODULATE);		// テクスチャステージステート　テクスチャブレンドでアルファを乗算合成
	lpD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);		// テクスチャステージステート　アルファのパラメータ2はディフューズ値
	lpD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);		// テクスチャステージステート　アルファのパラメータ1はテクセル
	lpD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP , D3DTOP_MODULATE);		// テクスチャステージステート　テクスチャブレンドでアルファを乗算合成

	lpD3DDevice->Clear(0,NULL,D3DCLEAR_TARGET,0xff000000,1.0,0);

	D3DXCreateSprite(lpD3DDevice,&lpD3DSprite);

	CreateDInput(hWnd,hInstance);

	mTargetWidth=gWinSizeX;
	mTargetHeight=gWinSizeY;

	mInited=true;
	mTextMode=false;
	return true;
}
void CDirectTest::ResetDTest()
{
	if(!mInited)	return;
	mInited=false;

	EndTextMode();
	Release(false);
	if(!mFullScreen)	ChangeDisplaySettings(NULL, 0);
	HWND del=*mHWnd;
	*mHWnd=0;
	DestroyWindow(del);
	MakeWindow(mHInstance,mHWnd,mCmdShow);
	MakeD3D(mHWnd,mHInstance,mCmdShow);

	CTextureList* tex=CDTTexture::GetTextureList();
	while(tex){
		if(tex->mRenderTarget){
			ResetTexuture(tex);
		}else{
			tex->lpDirectTexture=CreateTexture(tex->mFileName,tex->mColor);
		}
		tex=tex->mNextList;
	}
	for(int i=0;i<MAX_FONT;i++){
		if(mFont[i].mInit)
			CreateFont(i,mFont[i].mSize,mFont[i].mName,mFont[i].mWeight,mFont[i].mItalic);
	}
	DrawStart();
	DrawEnd();
/**/
}
void CDirectTest::ResetTexuture(CTextureList* tex)
{
//	tex->lpDirectTexture=CreateTexture(tex->mWidth,tex->mHeight);
	if(!tex->mBuffer){
		tex->lpDirectTexture=CreateTexture(tex->mWidth,tex->mHeight);
		return;
	}

	LPDIRECT3DTEXTURE9 lpD3DTexture;
	if(D3D_OK ==D3DXCreateTextureFromFileInMemoryEx(	lpD3DDevice,
													tex->mBuffer->GetBufferPointer(),
													tex->mBuffer->GetBufferSize(),
													tex->mWidth,tex->mHeight,1,
													D3DUSAGE_RENDERTARGET,
													D3DFMT_A8R8G8B8,
													D3DPOOL_DEFAULT,
													D3DX_DEFAULT ,
													D3DX_DEFAULT ,
													0x00000000,NULL,NULL,
													&lpD3DTexture )
	  )
	{
		tex->lpDirectTexture=lpD3DTexture;
		tex->ReleaseBuffer();
		return;
	}
	tex->lpDirectTexture=CreateTexture(tex->mWidth,tex->mHeight);
	return;
}
void CDirectTest::CreateDInput(HWND *hWnd,HINSTANCE* hInstance)
{
	DirectInput8Create(*hInstance,DIRECTINPUT_VERSION,IID_IDirectInput8,(LPVOID *)(&lpDInput),NULL);

	lpDInput->CreateDevice(GUID_SysKeyboard,&lpDIDKey,NULL);
	lpDIDKey->SetDataFormat(&c_dfDIKeyboard );
	lpDIDKey->SetCooperativeLevel(*hWnd,DISCL_BACKGROUND|DISCL_NONEXCLUSIVE);
	lpDIDKey->Acquire();

	lpDInput->CreateDevice(GUID_SysMouse,&lpDIDMouse,NULL);
	lpDIDMouse->SetDataFormat(&c_dfDIMouse  );
	lpDIDMouse->SetCooperativeLevel(*hWnd,DISCL_BACKGROUND|DISCL_NONEXCLUSIVE);
	lpDIDMouse->Acquire();

	lpDInput->EnumDevices(DI8DEVCLASS_GAMECTRL,EnumDevicesCallback,this,DIEDFL_ATTACHEDONLY);
	if(lpDIDJoy){
		lpDIDJoy->SetDataFormat(&c_dfDIJoystick2);
		lpDIDJoy->SetCooperativeLevel(*hWnd,DISCL_BACKGROUND|DISCL_NONEXCLUSIVE);
		lpDIDJoy->EnumObjects(EnumDeviceObjectsCallback, this, DIDFT_AXIS );
		HRESULT hr;
		hr = lpDIDJoy->Poll(); 
		if (FAILED(hr)) 
		{
			hr = lpDIDJoy->Acquire();
			while(hr == DIERR_INPUTLOST) 
				hr = lpDIDJoy->Acquire();
		}
	}

}
void CDirectTest::GetInputDevice(LPDIRECTINPUT8** dinput,IDirectInputDevice8*** didjoy)
{
	*dinput=&lpDInput;
	*didjoy=&lpDIDJoy;
}

bool CDirectTest::Release(bool deltex)
{
	mInited=false;
	if(deltex){
		CDTTexture::DeleteAllTexture();
	}else{
		CDTTexture::EscapeAllTexture();
	}

	if(lpDInput){
		if(lpDIDKey){
			lpDIDKey->Unacquire();
			lpDIDKey->Release();
			lpDIDKey=NULL;
		}
		if(lpDIDMouse){
			lpDIDMouse->Unacquire();
			lpDIDMouse->Release();
			lpDIDMouse=NULL;
		}
		if(lpDIDJoy){
			lpDIDJoy->Unacquire();
			lpDIDJoy->Release();
			lpDIDJoy=NULL;
		}
		lpDInput->Release();
		lpDInput=NULL;
	}

	for(int i=0;i<MAX_FONT;i++){
		mFont[i].Release();
	}
	if(lpD3DSprite){
		lpD3DSprite->Release();
		lpD3DSprite=NULL;
	}
	if(lpD3DDevice){
		lpD3DDevice->Release();
		lpD3DDevice=NULL;
	}
	if(lpD3D){
		lpD3D->Release();
		lpD3D=NULL;
	}

	return true;
}
void CDirectTest::OnTimer(void)
{
	if(!mInited)	return;
	GetInputData();
	::OnTimer();
}
void CDirectTest::OnDraw(void)
{
	if(!mInited)	return;
	DrawStart();
	::OnDraw();
	DrawEnd();
}
void CDirectTest::Exit(void)
{
	DestroyWindow(*mHWnd);
}

void CDirectTest::GetInputData(void)
{
	if(!mActive){
		memset(mKeyData,0,sizeof(mKeyData));
		memset(mMouseButton,0,sizeof(mMouseButton));
		memset(mJoyButton,0,sizeof(mJoyButton));
		return;
	}
	if(lpDIDKey){
		BYTE key[256];
		lpDIDKey->GetDeviceState(256,key);
		for(int i=0;i<256;i++){
			if(key[i]&0x80){
				mKeyData[i]++;
			}else{
				if(mKeyData[i]>0){
					mKeyData[i]=-1;
				}else{
					mKeyData[i]=0;
				}
			}
		}
	}

	if(lpDIDMouse){
		DIMOUSESTATE mousestate;
		lpDIDMouse->GetDeviceState(sizeof(mousestate),&mousestate);
		for(int i=0;i<4;i++){
			if(mousestate.rgbButtons[i]&0x80){
				mMouseButton[i]++;
			}else{
				if(mMouseButton[i]>0){
					mMouseButton[i]=-1;
				}else{
					mMouseButton[i]=0;
				}
			}
		}
	}

	if(lpDIDJoy){
		DIJOYSTATE2 joystate;
		HRESULT hr;
		hr = lpDIDJoy->Poll(); 
		if (FAILED(hr)) 
		{
			hr = lpDIDJoy->Acquire();
			while(hr == DIERR_INPUTLOST) 
				hr = lpDIDJoy->Acquire();
		}
		memset(&joystate,0,sizeof(joystate));
		lpDIDJoy->GetDeviceState( sizeof(joystate), &joystate);
		for(int i=0;i<36;i++){
			bool down;
			switch(i){
				case JOY_LEFT:
					down=(joystate.lX<-500);	break;
				case JOY_UP:
					down=(joystate.lY<-500);	break;
				case JOY_RIGHT:
					down=(joystate.lX>500);		break;
				case JOY_DOWN:
					down=(joystate.lY>500);		break;
				default:
					down=((joystate.rgbButtons[i-4]&0x80)!=0);	break;
			}

			if(down){
				mJoyButton[i]++;
			}else{
				if(mJoyButton[i]>0){
					mJoyButton[i]=-1;
				}else{
					mJoyButton[i]=0;
				}
			}
		}
	}
}
bool CDirectTest::SetRenderTarget(LPDIRECT3DSURFACE9 lpSurface,bool tex)
{
	EndTextMode();
	if(lpSurface){
		int n=lpD3DDevice->SetRenderTarget(0,lpSurface);
		n=0;
	}else{
		mTargetWidth=gWinSizeX;
		mTargetHeight=gWinSizeY;
		int n=lpD3DDevice->SetRenderTarget(0,BackBuffer.GetSurface());
		n=0;
	}
//	lpD3DDevice->Clear(0,NULL,D3DCLEAR_TARGET,0x00000000,1,0);

	if(mTargeTex){
		lpTarget->Release();
	}

	lpTarget=lpSurface;
	mTargeTex=tex;
	return true;
}
void CDirectTest::DrawStart(void)
{
	lpD3DDevice->BeginScene();
	LPDIRECT3DSURFACE9 lpSurface;
	lpD3DDevice->GetBackBuffer(0,0,D3DBACKBUFFER_TYPE_MONO,&lpSurface );

	BackBuffer.Release();
	BackBuffer.SetSurface(lpSurface);
	BackBuffer.mHeight=gWinSizeY;
	BackBuffer.mWidth=gWinSizeX;

	lpD3DDevice->Clear(0,NULL,D3DCLEAR_TARGET,0x00000000,1,0);

	StartTextMode();

	if(lpTarget){
		lpD3DDevice->SetRenderTarget(0,lpTarget);
	}
}
void CDirectTest::DrawEnd(void)
{	
	if(lpTarget){
		lpD3DDevice->SetRenderTarget(0,BackBuffer.GetSurface());
	}
	EndTextMode();

	lpD3DDevice->EndScene();
	lpD3DDevice->Present( NULL, NULL, NULL, NULL );
}

int  CDirectTest::GetKey(int key)
{
	if((0>key)||(key>256))	return 0;
	return mKeyData[key];
}
int  CDirectTest::GetMouseButton(int button)
{
	if((0>button)||(button>4))	return 0;
	return mMouseButton[button];
}
void CDirectTest::GetMousePos(int* x,int* y)
{
	POINT point;
	GetCursorPos(&point);
	ScreenToClient(hWnd,&point);

	*x=point.x;
	*y=point.y;
}
int  CDirectTest::GetJoyButton(int button)
{
	if((0>button)||(button>36))	return 0;
	return mJoyButton[button];
}


/*bool CDirectTest::CreateSurface(CDTSurface *surface,int x,int y)
{
	surface->Release();

	LPDIRECT3DSURFACE9 lpSurface;
	lpSurface=surface->GetSurface();

//	lpD3DDevice->CreateOffscreenPlainSurface(x,y,D3DFMT_A8R8G8B8,D3DPOOL_DEFAULT,&lpSurface,NULL );
	lpD3DDevice->CreateRenderTarget(x,y,D3DFMT_A8R8G8B8,D3DMULTISAMPLE_NONE,0,true,&lpSurface,NULL);
	surface->SetSurface(lpSurface);
	surface->mHeight=y;
	surface->mWidth=x;
	lpD3DDevice->ColorFill(lpSurface,NULL,0xff000000);

	return true;
}
*/
bool CDirectTest::GetBMPSize(FILE* pFile,int *x,int *y)
{
	if(!pFile)	return false;
	fseek(pFile,14,SEEK_SET);
	int n;
	fread(&n,1,4,pFile);
	switch (n){
		case 12:
			*x=0;	*y=0;
			fread(x,1,2,pFile);
			fread(y,1,2,pFile);
			break;
		case 40:
			fread(x,1,4,pFile);
			fread(y,1,4,pFile);
			break;
		default:
			return false;
	}
	return true;
}
bool CDirectTest::GetBMPSize(FILE* pFile,int *x,int *y,char BM)
{
	if(!pFile)	return false;
	fseek(pFile,14-BM,SEEK_SET);
	int n;
	fread(&n,1,4,pFile);
	switch (n){
		case 12:
			*x=0;	*y=0;
			fread(x,1,2,pFile);
			fread(y,1,2,pFile);
			break;
		case 40:
			fread(x,1,4,pFile);
			fread(y,1,4,pFile);
			break;
		default:
			return false;
	}
	return true;
}
bool CDirectTest::GetBMPSize(const char *filename,int *x,int *y)
{
	FILE *pFile;
	if(!(pFile=fopen(filename,"rb")))	return false;
	bool r=GetBMPSize(fopen(filename,"rb"),x,y);
	fclose(pFile);
	return r;
}

bool CDirectTest::GetFixedBMP(const char *filename,unsigned char ** bmpdata,int *x,int *y,int *filesize,D3DCOLOR *color)
{
	FILE *pFile;
	int bmpx,bmpy;
	char BM;
	if(!(pFile=fopen(filename,"rb")))	return false;
	fseek(pFile,0,SEEK_SET);
	if(getc(pFile)=='B')	BM=0;
	else					BM=1;

	GetBMPSize(pFile,&bmpx,&bmpy,BM);

	*x=0x04;
	*y=0x04;
	while(*x<bmpx)	*x=*x<<1;
	while(*x<bmpy)	*x=*x<<1;
	*y=*x;


	int offset;
	fseek(pFile,10-BM,SEEK_SET);
	fread(&offset,1,4,pFile);
	if(offset==0){	
		fclose(pFile);
		return false;
	}

	int pixelsize=0;
	int n;
	fread(&n,1,4,pFile);
	switch (n){
		case 12:
			fseek(pFile,24-BM,SEEK_SET);
			fread(&pixelsize,1,2,pFile);
			break;
		case 40:
			fseek(pFile,28-BM,SEEK_SET);
			fread(&pixelsize,1,2,pFile);
			break;
		default:
			fclose(pFile);
			return false;
	}

	int datasize=offset+(pixelsize>>3)*(*x)*(*y);
	int filesizex=bmpx*(pixelsize>>3);
	if(filesizex%4!=0)	filesizex=(filesizex/4)*4+4;

	if(color){
		if(pixelsize==8){
			fseek(pFile,-filesizex,SEEK_END);
			int palette=getc(pFile);

			fseek(pFile,n-BM+14+palette*4,SEEK_SET);
			int b=getc(pFile);
			int g=getc(pFile);
			int r=getc(pFile);
			*color=0xff000000+r*0x10000+g*0x100+b;
		}
	}

	fseek(pFile,0,SEEK_END);
	int fsize=ftell(pFile);
	fseek(pFile,0,SEEK_SET);

	if((bmpx==bmpy)&&(*x==bmpx)){
		if(BM==0){
			*filesize=fsize;
			*bmpdata=new unsigned char[fsize];
			fread(*bmpdata,1,fsize,pFile);
		}else{
			*filesize=fsize+1;
			*bmpdata=new unsigned char[fsize+1];
			fread(*bmpdata+1,1,fsize,pFile);
			*bmpdata[0]='B';
		}
		fclose(pFile);
		return true;
	}

	if(BM==0){
		*filesize=datasize;
		*bmpdata=new unsigned char[datasize];
		fread(*bmpdata,offset,1,pFile);
	}else{
		*filesize=datasize;
		*bmpdata=new unsigned char[datasize];
		fread(*bmpdata+1,offset-1,1,pFile);
		*bmpdata[0]='B';
	}

	int datasizex=((*x)*(pixelsize>>3));
	unsigned char *data=*bmpdata;

	for(int i=0;i<bmpy;i++){
		int b=offset+datasizex*i+((*y)-bmpy)*datasizex;
		fread(&(data[b]),filesizex,1,pFile);
	}

	switch (n){
		case 12:
			data[18]=*x;
			data[19]=(*x)%256;
			data[20]=*y;
			data[21]=(*y)%256;
			break;
		case 40:
			memcpy(&(data[18]),x,4);
			memcpy(&(data[22]),y,4);
			break;
	}

	fclose(pFile);
	return true;
}

bool CDirectTest::LoadSurface(CDTSurface *surface,const char *filename)
{
	int x;
	int y;
	if(!GetBMPSize(filename,&x,&y))	return false;

	surface->Release();

	LPDIRECT3DSURFACE9 lpSurface;
	lpSurface=surface->GetSurface();

	lpD3DDevice->CreateOffscreenPlainSurface(x,y,D3DFMT_A8R8G8B8,D3DPOOL_DEFAULT ,&lpSurface,NULL );
//	lpD3DDevice->CreateRenderTarget(x,y,D3DFMT_A8R8G8B8,D3DMULTISAMPLE_NONE,0,true,&lpSurface,NULL);

	D3DXLoadSurfaceFromFile(lpSurface,NULL,NULL,filename,NULL,D3DX_DEFAULT,0x00000000,NULL );
	surface->SetSurface(lpSurface);
	surface->mHeight=y;
	surface->mWidth=x;
	return true;
}
bool CDirectTest::DrawSurface(CDTSurface *surface,int fx,int fy,int sx,int sy,int tx,int ty)
{
	return DrawSurface(surface,fx,fy,sx,sy,tx,ty,sx,sy);
}
bool CDirectTest::DrawSurface(CDTSurface *surface,int fx,int fy,int fsx,int fsy,int tx,int ty,int tsx,int tsy)
{
	EndTextMode();

	if(surface==NULL)	surface=&BackBuffer;
	RECT FromRect,ToRect;
	SetRect(&FromRect,fx,fy,fx+fsx,fy+fsy);
	SetRect(&ToRect,tx,ty,tx+tsx,ty+tsy);

	LPDIRECT3DSURFACE9 lpSurface;
	if(lpTarget){
		lpSurface=lpTarget;
	}else{
		lpSurface=BackBuffer.GetSurface();
	}

	if(ToRect.left<0){
		FromRect.left-=ToRect.left*fsx/tsx;
		ToRect.left=0;
	}
	if(ToRect.top<0){
		FromRect.top-=ToRect.top*fsy/tsy;
		ToRect.top=0;
	}
	if(ToRect.right>mTargetWidth){
		FromRect.right-=(ToRect.right-mTargetWidth)*fsx/tsx;
		ToRect.right=mTargetWidth;
	}
	if(ToRect.bottom>mTargetHeight){
		FromRect.bottom-=(ToRect.bottom-mTargetHeight)*fsy/tsy;
		ToRect.bottom=mTargetHeight;
	}

	if(lpD3DDevice->StretchRect(surface->GetSurface(), &FromRect,lpSurface, &ToRect, D3DTEXF_NONE )==D3D_OK)	return true;
	return false;
}
/*bool CDirectTest::SetRenderTargetS(CDTSurface *surface)
{
	if(surface){
		mTargetWidth=surface->mWidth;
		mTargetHeight=surface->mHeight;
		return SetRenderTarget(surface->GetSurface(),false);
	}else{
		return SetRenderTarget(NULL,false);
	}
	return false;
}
*/
void CDirectTest::Clear(D3DCOLOR color)
{
	EndTextMode();

	lpD3DDevice->Clear(0,NULL,D3DCLEAR_TARGET,color,1,0);
}

//大島さん、ありがとうございます。
struct CV_2D
{
	FLOAT x,y,z,rhw;	// 位置座標
	DWORD color;		// 色
	FLOAT tu,tv;		// テクスチャ座標
};
bool CDirectTest::DrawBox(int left,int top,int right,int bottom,D3DCOLOR color)
{
	return DrawBox(left,top,right,bottom,color,color,color,color);
}
bool CDirectTest::DrawBox(int left,int top,int right,int bottom,D3DCOLOR color1,D3DCOLOR color2,D3DCOLOR color3,D3DCOLOR color4)
{
	EndTextMode();

	float fl, ft, fr, fb;
	fl = left - 0.5f;
	ft = top  - 0.5f;
	fr = float(right);
	fb = float(bottom);
	CV_2D cv[4];
	cv[0].x = fl;	cv[0].y = ft;	cv[0].z = 0.5f; cv[0].rhw = 1.0f; cv[0].color = color1;		cv[0].tu = 0.0; cv[0].tv = 0.0;
	cv[1].x = fr;	cv[1].y = ft;	cv[1].z = 0.5f; cv[1].rhw = 1.0f; cv[1].color = color2;		cv[1].tu = 0.0; cv[1].tv = 0.0;
	cv[2].x = fl;	cv[2].y = fb;	cv[2].z = 0.5f; cv[2].rhw = 1.0f; cv[2].color = color3;		cv[2].tu = 0.0; cv[2].tv = 0.0;
	cv[3].x = fr;	cv[3].y = fb;	cv[3].z = 0.5f; cv[3].rhw = 1.0f; cv[3].color = color4;		cv[3].tu = 0.0; cv[3].tv = 0.0;

	lpD3DDevice->SetTexture( 0, NULL );
	lpD3DDevice->SetFVF( D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1 );
	lpD3DDevice->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, cv, sizeof(CV_2D) );
	return true;
}
bool CDirectTest::DrawBox(int x1,int y1,int x2,int y2,int x3,int y3,int x4,int y4,D3DCOLOR color1,D3DCOLOR color2,D3DCOLOR color3,D3DCOLOR color4)
{
	EndTextMode();

	lpD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	CV_2D cv[4];
	cv[0].x = (float)x1;	cv[0].y = (float)y1;	cv[0].z = 0.5f; cv[0].rhw = 1.0f; cv[0].color = color1;		cv[0].tu = 0.0; cv[0].tv = 0.0;
	cv[1].x = (float)x2;	cv[1].y = (float)y2;	cv[1].z = 0.5f; cv[1].rhw = 1.0f; cv[1].color = color2;		cv[1].tu = 0.0; cv[1].tv = 0.0;
	cv[2].x = (float)x3;	cv[2].y = (float)y3;	cv[2].z = 0.5f; cv[2].rhw = 1.0f; cv[2].color = color3;		cv[2].tu = 0.0; cv[2].tv = 0.0;
	cv[3].x = (float)x4;	cv[3].y = (float)y4;	cv[3].z = 0.5f; cv[3].rhw = 1.0f; cv[3].color = color4;		cv[3].tu = 0.0; cv[3].tv = 0.0;

	lpD3DDevice->SetTexture( 0, NULL );
	lpD3DDevice->SetFVF( D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1 );
	lpD3DDevice->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, cv, sizeof(CV_2D) );

	lpD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	return true;
}
bool CDirectTest::DrawBoxRotate(int x,int y,int w,int h,D3DCOLOR color,double rad)
{
	EndTextMode();

	lpD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	float  fw, fh;

	fw = (float)(w);
	fh = (float)(h);
	fw/=2;
	fh/=2;
	float centerx=(float)x;
	float centery=(float)y;
	float s=(float)sin(rad);
	float c=(float)cos(rad);
	CV_2D cv[4];
	cv[0].x = centerx-c*fw-s*fh;	cv[0].y = centery-c*fh+s*fw;	cv[0].z = 0.5f; cv[0].rhw = 1.0f; cv[0].color = color;		cv[0].tu = 0.0; cv[0].tv = 0.0;
	cv[1].x = centerx+c*fw-s*fh;	cv[1].y = centery-c*fh-s*fw;	cv[1].z = 0.5f; cv[1].rhw = 1.0f; cv[1].color = color;		cv[1].tu = 0.0; cv[1].tv = 0.0;
	cv[2].x = centerx-c*fw+s*fh;	cv[2].y = centery+c*fh+s*fw;	cv[2].z = 0.5f; cv[2].rhw = 1.0f; cv[2].color = color;		cv[2].tu = 0.0; cv[2].tv = 0.0;
	cv[3].x = centerx+c*fw+s*fh;	cv[3].y = centery+c*fh-s*fw;	cv[3].z = 0.5f; cv[3].rhw = 1.0f; cv[3].color = color;		cv[3].tu = 0.0; cv[3].tv = 0.0;

	lpD3DDevice->SetTexture( 0, NULL );
	lpD3DDevice->SetFVF( D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1 );
	lpD3DDevice->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, cv, sizeof(CV_2D) );

	lpD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return true;
}
bool CDirectTest::DrawTriangle(int x1,int y1,int x2,int y2,int x3,int y3,D3DCOLOR color1,D3DCOLOR color2,D3DCOLOR color3)
{
	EndTextMode();
	lpD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	CV_2D cv[3];
	cv[0].x = (float)x1;	cv[0].y = (float)y1;	cv[0].z = 0.5f; cv[0].rhw = 1.0f; cv[0].color = color1;		cv[0].tu = 0.0; cv[0].tv = 0.0;
	cv[1].x = (float)x2;	cv[1].y = (float)y2;	cv[1].z = 0.5f; cv[1].rhw = 1.0f; cv[1].color = color2;		cv[1].tu = 0.0; cv[1].tv = 0.0;
	cv[2].x = (float)x3;	cv[2].y = (float)y3;	cv[2].z = 0.5f; cv[2].rhw = 1.0f; cv[2].color = color3;		cv[2].tu = 0.0; cv[2].tv = 0.0;

	lpD3DDevice->SetTexture( 0, NULL );
	lpD3DDevice->SetFVF( D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1 );
	lpD3DDevice->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 1, cv, sizeof(CV_2D) );

	lpD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	return true;
}
bool CDirectTest::DrawLine(int fx,int fy,int tx,int ty,double width,D3DCOLOR color)
{

	EndTextMode();

//ザ、コピペ。
	double dx, dy;
	double fx0, fx1, fx2, fx3;
	double fy0, fy1, fy2, fy3;
	double length;
	double rad;

	// 長さ計算
	length = sqrt((double)(tx-fx)*(tx-fx) + (double)(ty-fy)*(ty-fy));

	// 傾き計算
	rad = atan2((double)(fy-ty), (double)(tx-fx));

	// 回転描画
	dx = (double)(fx+tx) / 2.0 - 0.5f;
	dy = (double)(fy+ty) / 2.0 - 0.5f;
	fx0 = dx + ( -length/2*cos(rad) - width/2*sin(rad) );
	fx1 = dx + (  length/2*cos(rad) - width/2*sin(rad) );
	fx2 = dx + ( -length/2*cos(rad) + width/2*sin(rad) );
	fx3 = dx + (  length/2*cos(rad) + width/2*sin(rad) );
	fy0 = dy + (  length/2*sin(rad) - width/2*cos(rad) );
	fy1 = dy + ( -length/2*sin(rad) - width/2*cos(rad) );
	fy2 = dy + (  length/2*sin(rad) + width/2*cos(rad) );
	fy3 = dy + ( -length/2*sin(rad) + width/2*cos(rad) );

	// 以下、普通の描画
    //d3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	CV_2D cv[4];
	cv[0].x = (float)fx0; cv[0].y = (float)fy0; cv[0].z = 0.5f; cv[0].rhw = 1.0f; cv[0].color = color;		cv[0].tu = 0; cv[0].tv = 0;
	cv[1].x = (float)fx1; cv[1].y = (float)fy1; cv[1].z = 0.5f; cv[1].rhw = 1.0f; cv[1].color = color;		cv[1].tu = 0; cv[1].tv = 0;
	cv[2].x = (float)fx2; cv[2].y = (float)fy2; cv[2].z = 0.5f; cv[2].rhw = 1.0f; cv[2].color = color;		cv[2].tu = 0; cv[2].tv = 0;
	cv[3].x = (float)fx3; cv[3].y = (float)fy3; cv[3].z = 0.5f; cv[3].rhw = 1.0f; cv[3].color = color;		cv[3].tu = 0; cv[3].tv = 0;

	lpD3DDevice->SetTexture( 0, NULL );
	lpD3DDevice->SetFVF( D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1 );
	lpD3DDevice->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, cv, sizeof(CV_2D) );
	return true;
}
bool CDirectTest::CreateTexture(CDTTexture &texture,int x,int y)
{
	texture.Release();
	LPDIRECT3DTEXTURE9 lpD3DTexture;
	int tx=x;
	int ty=y;

	/*tx=0x04;
	  ty=0x04;
	while(tx<x)	tx=tx<<1;
	while(tx<y)	tx=tx<<1;
	ty=tx;*/
	
	lpD3DTexture=CreateTexture(tx,ty);

	if(lpD3DTexture)
	{
		texture.SetTexture(lpD3DTexture,"",tx,ty,true,0x00000000);
		return true;
	}

	return false;
}
LPDIRECT3DTEXTURE9 CDirectTest::CreateTexture(int x,int y)
{
	LPDIRECT3DTEXTURE9 lpD3DTexture;

	if(D3D_OK==D3DXCreateTexture(lpD3DDevice,x,y,1,D3DUSAGE_RENDERTARGET,D3DFMT_A8R8G8B8,D3DPOOL_DEFAULT,&lpD3DTexture ))
	{
		return lpD3DTexture;
	}

	return NULL;
}
LPDIRECT3DTEXTURE9 CDirectTest::CreateTexture(const char *filename,D3DCOLOR color,int *x,int *y)
{
	LPDIRECT3DTEXTURE9 lpD3DTexture;
	unsigned char *bmpdata;
	int tx,ty;
	int filesize;
	if(!GetFixedBMP(filename,&bmpdata,&tx,&ty,&filesize,NULL))	return false;
	if(D3D_OK==D3DXCreateTextureFromFileInMemoryEx(lpD3DDevice,bmpdata,filesize,0,0,1,0,D3DFMT_A8R8G8B8,D3DPOOL_MANAGED,D3DX_DEFAULT ,D3DX_DEFAULT ,color,NULL,NULL,&lpD3DTexture ))
	{
		delete[] bmpdata;
		if(x)	*x=tx;
		if(y)	*y=ty;
		return lpD3DTexture;
	}
	delete[] bmpdata;
	return NULL;
}
bool CDirectTest::LoadTexture(CDTTexture &texture,const char *filename,D3DCOLOR color)
{
	if(texture.Release(filename)){
		if(!(texture.SeachTexture(filename))){
			LPDIRECT3DTEXTURE9 lpD3DTexture;
			unsigned char *bmpdata;
			int x,y;
			int filesize;
			D3DCOLOR* c=NULL;
			if(color==TRC_ZERO){
				c=&color;
			}
			if(!GetFixedBMP(filename,&bmpdata,&x,&y,&filesize,c))	return false;
			if(D3D_OK==D3DXCreateTextureFromFileInMemoryEx(lpD3DDevice,bmpdata,filesize,0,0,1,0,D3DFMT_A8R8G8B8,D3DPOOL_MANAGED,D3DX_DEFAULT ,D3DX_DEFAULT ,color,NULL,NULL,&lpD3DTexture ))
			{
				texture.SetTexture(lpD3DTexture,filename,x,y,false,color);
			}
			delete[] bmpdata;
		}
	}
	return true;
}
bool CDirectTest::DrawTextureRotate(CDTTexture &texture,int turn,int fx,int fy,int fsx,int fsy,int tx,int ty,int tsx,int tsy,D3DCOLOR color,double r)
{
	EndTextMode();

	float rad=float(r);
	float x,y;
	float fleft,fright,ftop,fbottom;
	float tleft,tright,ttop,tbottom;
	CV_2D cv[4];

	if(!texture.GetSize(&x,&y))	return false;

	fleft=	float(fx/x);
	fright=	float((fx+fsx)/x);
	ftop=	float(fy/y);
	fbottom=float((fy+fsy)/y);

	if(turn&TURN_HORIZONTAL){
		float f=fleft;
		fleft=fright;
		fright=f;
	}

	if(turn&TURN_VERTICAL){
		float f=ftop;
		ftop=fbottom;
		fbottom=f;
	}

	float arctan=atan(float(tsy/tsx));
	float l=(float)(_hypot(tsx/2,tsy/2));
	float p=3.141592f;
	float c=cos(arctan+rad);

	tleft=	float(tx)-0.5f;
	tright=	float(tx+tsx);
	ttop=	float(ty)-0.5f;
	tbottom=float(ty+tsy);


	cv[0].x = tx+cos(p+arctan+rad)*l;		cv[0].y = ty+sin(p+arctan+rad)*l;		cv[0].tu = fleft;	cv[0].tv = ftop;	cv[0].z = 0.5f; cv[0].rhw = 1.0f;	 cv[0].color = color;	
	cv[1].x = tx+cos(-arctan+rad)*l;		cv[1].y = ty+sin(-arctan+rad)*l;		cv[1].tu = fright;	cv[1].tv = ftop;	cv[1].z = 0.5f; cv[1].rhw = 1.0f;	 cv[1].color = color;			
	cv[2].x = tx+cos(p-arctan+rad)*l;		cv[2].y = ty+sin(p-arctan+rad)*l;		cv[2].tu = fleft;	cv[2].tv = fbottom;	cv[2].z = 0.5f; cv[2].rhw = 1.0f;	 cv[2].color = color;			
	cv[3].x = tx+cos(arctan+rad)*l;			cv[3].y = ty+sin(arctan+rad)*l;			cv[3].tu = fright;	cv[3].tv = fbottom;	cv[3].z = 0.5f; cv[3].rhw = 1.0f;	 cv[3].color = color;			

	lpD3DDevice->SetTexture( 0, texture.GetTexture() );
	lpD3DDevice->SetFVF( D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1 );
	lpD3DDevice->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, cv, sizeof(CV_2D) );

	return true;
}

bool CDirectTest::DrawTexture(CDTTexture &texture,int turn,int fx,int fy,int fsx,int fsy,double tx,double ty,double tsx,double tsy,D3DCOLOR color)
{
	EndTextMode();

	// 各頂点の相対座標

	float x,y;
	float fleft,fright,ftop,fbottom;
	float tleft,tright,ttop,tbottom;
	CV_2D cv[4];

	if(!texture.GetSize(&x,&y))	return false;

	tleft=	float(tx)-0.5f;
	tright=	float(tx+tsx);
	ttop=	float(ty)-0.5f;
	tbottom=float(ty+tsy);

	fleft=	float(fx/x);
	fright=	float((fx+fsx)/x);
	ftop=	float(fy/y);
	fbottom=float((fy+fsy)/y);

	if(turn&TURN_HORIZONTAL){
		float f=fleft;
		fleft=fright;
		fright=f;
	}

	if(turn&TURN_VERTICAL){
		float f=ftop;
		ftop=fbottom;
		fbottom=f;
	}


	cv[0].x = tleft;	cv[0].y = ttop;		cv[0].tu = fleft;	cv[0].tv = ftop;	cv[0].z = 0.5f; cv[0].rhw = 1.0f;	 cv[0].color = color;	
	cv[1].x = tright;	cv[1].y = ttop;		cv[1].tu = fright;	cv[1].tv = ftop;	cv[1].z = 0.5f; cv[1].rhw = 1.0f;	 cv[1].color = color;			
	cv[2].x = tleft;	cv[2].y = tbottom;	cv[2].tu = fleft;	cv[2].tv = fbottom;	cv[2].z = 0.5f; cv[2].rhw = 1.0f;	 cv[2].color = color;			
	cv[3].x = tright;	cv[3].y = tbottom;	cv[3].tu = fright;	cv[3].tv = fbottom;	cv[3].z = 0.5f; cv[3].rhw = 1.0f;	 cv[3].color = color;			

	lpD3DDevice->SetTexture( 0, texture.GetTexture() );
	lpD3DDevice->SetFVF( D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1 );
	lpD3DDevice->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, cv, sizeof(CV_2D) );

	return true;
}


bool CDirectTest::SetRenderTarget(CDTTexture *texture)
{
	if(!texture){
		return SetRenderTarget(NULL,false);
	}

	if(!texture->CheckRenderTarget())	return false;
	float x,y;
	texture->GetSize(&x,&y);

	mTargetWidth=int(x);
	mTargetHeight=int(y);

	LPDIRECT3DTEXTURE9 tex=texture->GetTexture();
	LPDIRECT3DSURFACE9 lpSurface;

	int n=tex->GetSurfaceLevel(0,&lpSurface);
	bool b=SetRenderTarget(lpSurface,true);
//	lpD3DDevice->Clear(0,NULL,D3DCLEAR_TARGET,0x00000000,1,0);
	return b;
}
bool CDirectTest::SetBlend(int blend)
{
	EndTextMode();

	switch(blend){
		case BLEND_STANDARD:
			lpD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP , D3DTOP_MODULATE);
			lpD3DDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA); 
			lpD3DDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA ); 
			return true;
		case BLEND_NONE:
			lpD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP , D3DTOP_MODULATE);
			lpD3DDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_ONE); 
			lpD3DDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ZERO); 
			return true;
		case BLEND_ADD:
			lpD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP , D3DTOP_MODULATE);
			lpD3DDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA); 
			lpD3DDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ONE); 
			return true;
		case BLEND_HOLL:
			lpD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP , D3DTOP_MODULATE);
			lpD3DDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_INVSRCALPHA); 
			lpD3DDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_SRCALPHA); 
			return true;
		case BLEND_SOLID:
			lpD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP , D3DTOP_SELECTARG2);
			lpD3DDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA); 
			lpD3DDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA ); 
			return true;
		case BLEND_SOLID_ADD:
			lpD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP , D3DTOP_SELECTARG2);
			lpD3DDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA); 
			lpD3DDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ONE); 
			return true;
/*		case BLEND_TEST:
			lpD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP , D3DTOP_MODULATE);
			lpD3DDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA); 
			lpD3DDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ONE); 
			return true;
*/	}
	return false;
}
void CDirectTest::StartTextMode(void)
{
	if(!mTextMode){
		lpD3DSprite->Begin(0);
		mTextMode=true;
	}
}
void CDirectTest::EndTextMode(void)
{
	if(mTextMode){
		lpD3DSprite->End();
		mTextMode=false;
	}
}
bool CDirectTest::CreateFont(int fontnum,int size,const char* fontname,int weight,bool italic)
{
	if(fontnum<0 || MAX_FONT<=fontnum)	return false;
	LPD3DXFONT font;
	D3DXCreateFont(lpD3DDevice,size,0,weight,-1,italic,SHIFTJIS_CHARSET,OUT_TT_ONLY_PRECIS,0,0,fontname,&font);
	mFont[fontnum].SetFont(font,size,fontname,weight,italic);

	return true;
}
bool CDirectTest::ChangeTextColor(int fontnum,D3DCOLOR color)
{
	if(fontnum<0 || MAX_FONT<=fontnum)	return false;

	mFontColor[fontnum]=color;

	return true;
}
bool CDirectTest::DrawText(int fontnum,const char *text,int x,int y,D3DCOLOR color,int position,int sizex,int sizey)
{
	StartTextMode();
	if(fontnum<0 || MAX_FONT<=fontnum)	return false;
	LPD3DXFONT font=mFont[fontnum].GetFont();
	if(!font)			return false;

	if(color==0x00000000)		color=mFontColor[fontnum];
	if(!(position&DT_BOTTOM))	position=position|DT_TOP;

	RECT textspace;
	SetRect(&textspace,x,y,x+sizex,y+sizey);
	font->DrawText(lpD3DSprite,text,-1,&textspace,position,color);

	return true;
}
bool CDirectTest::Printf(int fontnum,int x,int y,const char *text,...)
{
	if(fontnum<0 || MAX_FONT<=fontnum)	return false;

	char draw[512];
	vsprintf(draw,text,(char*)(&text+1));
	return DrawText(fontnum,draw,x,y);
}

bool CDirectTest::GetTextSize(int fontnum,const char *text,int *x,int *y)
{
	if(fontnum<0 || MAX_FONT<=fontnum)	return false;
	LPD3DXFONT font=mFont[fontnum].GetFont();
	if(!font)			return false;

	RECT textspace={0,0,0,0};
	font->DrawText(NULL,text,-1,&textspace,DT_TOP|DT_LEFT|DT_CALCRECT,0xff000000);
	if(x)	*x=textspace.right;
	if(y)	*y=textspace.bottom;
	return true;
}
void CDirectTest::SetWindowSize(int x,int y)
{
	gWinSizeX=x;
	gWinSizeY=y;
}
void CDirectTest::FullScreen(bool screen)
{
	if(mFullScreen==screen)	return;
	mFullScreen=screen;
	if(mInited){
		ResetDTest();
	}
}
void CDirectTest::ShowCursor(bool show)
{
	if(mShowCursor!=show){
		mShowCursor=show;
		::ShowCursor(show);
	}
}
void CDirectTest::SetWindowTitle(const char* title)
{
	strcpy(gAppTitle,title);
	SetWindowText(hWnd,title);
}
void CDirectTest::SetFPS(int fps)
{
	gOneFrame=1000.0/fps;
}
void CDirectTest::DrawSkip(bool skip)
{
	gDrawSkip=skip;
}
