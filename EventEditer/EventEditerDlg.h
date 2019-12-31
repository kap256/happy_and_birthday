// EventEditerDlg.h : �w�b�_�[ �t�@�C��
//

#pragma once
#include "afxwin.h"


// CEventEditerDlg �_�C�A���O
class CEventEditerDlg : public CDialog
{
	char mFilename[1024];
	int  mOldCursor;
	void Change();

	void OpenFile();
	bool CheckSave();
	void MoveFile(int n);



// �R���X�g���N�V����
public:
	CEventEditerDlg(CWnd* pParent = NULL);	// �W���R���X�g���N�^

// �_�C�A���O �f�[�^
	enum { IDD = IDD_EVENTEDITER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �T�|�[�g


// ����
protected:
	HICON m_hIcon;

	// �������ꂽ�A���b�Z�[�W���蓖�Ċ֐�
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
