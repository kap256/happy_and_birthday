// EventEditer.h : PROJECT_NAME �A�v���P�[�V�����̃��C�� �w�b�_�[ �t�@�C���ł��B
//

#pragma once

#ifndef __AFXWIN_H__
	#error ���̃t�@�C���� PCH �Ɋ܂߂�O�ɁA'stdafx.h' ���܂߂Ă��������B
#endif

#include "resource.h"		// ���C�� �V���{��


// CEventEditerApp:
// ���̃N���X�̎����ɂ��ẮAEventEditer.cpp ���Q�Ƃ��Ă��������B
//

class CEventEditerApp : public CWinApp
{
public:
	CEventEditerApp();

// �I�[�o�[���C�h
	public:
	virtual BOOL InitInstance();

// ����

	DECLARE_MESSAGE_MAP()
};

extern CEventEditerApp theApp;
