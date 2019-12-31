// EventEditer.h : PROJECT_NAME アプリケーションのメイン ヘッダー ファイルです。
//

#pragma once

#ifndef __AFXWIN_H__
	#error このファイルを PCH に含める前に、'stdafx.h' を含めてください。
#endif

#include "resource.h"		// メイン シンボル


// CEventEditerApp:
// このクラスの実装については、EventEditer.cpp を参照してください。
//

class CEventEditerApp : public CWinApp
{
public:
	CEventEditerApp();

// オーバーライド
	public:
	virtual BOOL InitInstance();

// 実装

	DECLARE_MESSAGE_MAP()
};

extern CEventEditerApp theApp;
