
// zPacker.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������

#define WM_PROCESS_MESSAGE	WM_USER+0x200

// CzPackerApp: 
// �йش����ʵ�֣������ zPacker.cpp
//


class CzPackerApp : public CWinApp
{
public:
	CzPackerApp();
	BOOL m_BlockSel[8];

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CzPackerApp theApp;