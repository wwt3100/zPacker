
// zPacker.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "zPacker.h"
#include "CRegDlg.h"
#include "zPackerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CzPackerApp

BEGIN_MESSAGE_MAP(CzPackerApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CzPackerApp 构造

CzPackerApp::CzPackerApp()
{
	// TODO:  在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CzPackerApp 对象

CzPackerApp theApp;


// CzPackerApp 初始化

BOOL CzPackerApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。  否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// 创建 shell 管理器，以防对话框包含
	// 任何 shell 树视图控件或 shell 列表视图控件。
	CShellManager *pShellManager = new CShellManager;

	// 激活“Windows Native”视觉管理器，以便在 MFC 控件中启用主题
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	

	VMProtectBegin("Startup");
	SetRegistryKey(_T("Phil's Software"));
#ifndef _DEBUG
	CFile SN;
	int vmpret=-1;
	if(VMProtectIsValidImageCRC()==false)
	{
		AfxMessageBox(_T("文件被破坏,请重新索取程序文件"), 0, 0);
		return FALSE;
	}
	if (SN.Open(_T("Reg.key"), CFile::modeRead, 0))
	{
		DWORD SNFileSize = SN.GetLength();
		char *snstr = new char[SNFileSize+1];
		memset(snstr, 0, SNFileSize + 1);
		SN.Read(snstr, SNFileSize);
		vmpret = VMProtectSetSerialNumber(snstr);
		delete snstr;
		SN.Close();
		//CString str;		//调试问题原因
		//str.Format(_T("%d,%d"), vmpret, SNFileSize);
		//AfxMessageBox(str, 0, 0);
	}
	else
	{
		AfxMessageBox(_T("授权文件打开失败"), 0, 0);
	}
	if (vmpret != 0)
	{
		//AfxMessageBox(L"授权无效或已过期，请重新输入。", 0, 0);
		CRegDlg dlg;
		INT_PTR nResponse = dlg.DoModal();
		if (nResponse == IDOK)
		{

			AfxMessageBox(_T("程序即将关闭，请重启查看授权是否有效。"), 0, 0);
			return FALSE;
		}
		else if (nResponse == IDCANCEL)
		{
			return FALSE;
		}
	}
#endif

	CzPackerDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO:  在此放置处理何时用
		//  “确定”来关闭对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO:  在此放置处理何时用
		//  “取消”来关闭对话框的代码
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "警告: 对话框创建失败，应用程序将意外终止。\n");
		TRACE(traceAppMsg, 0, "警告: 如果您在对话框上使用 MFC 控件，则无法 #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS。\n");
	}
	VMProtectEnd();
	// 删除上面创建的 shell 管理器。
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}

