// EventEditerDlg.h : ヘッダー ファイル
//

#pragma once
#include "afxwin.h"


// CEventEditerDlg ダイアログ
class CEventEditerDlg : public CDialog
{
	char mFilename[1024];
	int  mOldCursor;
	void Change();

	void OpenFile();
	bool CheckSave();
	void MoveFile(int n);



// コンストラクション
public:
	CEventEditerDlg(CWnd* pParent = NULL);	// 標準コンストラクタ

// ダイアログ データ
	enum { IDD = IDD_EVENTEDITER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート


// 実装
protected:
	HICON m_hIcon;

	// 生成された、メッセージ割り当て関数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CEdit mEditBox;
	afx_msg void OnOpen();
	afx_msg void OnSave();
	afx_msg void OnSaveAs();
	afx_msg void OnComp();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnDestroy();
//	afx_msg void OnEnChange1011();
//	afx_msg void OnEnSetfocus1011();
	afx_msg void OnEnUpdateEditbox();
	afx_msg void OnSaveNc();
	afx_msg void OnSaveAsNc();
	afx_msg void OnNextFile();
	afx_msg void OnBeforeFile();
};
