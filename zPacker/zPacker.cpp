
// zPacker.cpp : ����Ӧ�ó��������Ϊ��
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


// CzPackerApp ����

CzPackerApp::CzPackerApp()
{
	// TODO:  �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CzPackerApp ����

CzPackerApp theApp;


// CzPackerApp ��ʼ��

BOOL CzPackerApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()��  ���򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// ���� shell ���������Է��Ի������
	// �κ� shell ����ͼ�ؼ��� shell �б���ͼ�ؼ���
	CShellManager *pShellManager = new CShellManager;

	// ���Windows Native���Ӿ����������Ա��� MFC �ؼ�����������
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	

	VMProtectBegin("Startup");
	SetRegistryKey(_T("Phil's Software"));
#ifndef _DEBUG
	CFile SN;
	int vmpret=-1;
	if(VMProtectIsValidImageCRC()==false)
	{
		AfxMessageBox(_T("�ļ����ƻ�,��������ȡ�����ļ�"), 0, 0);
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
		//CString str;		//��������ԭ��
		//str.Format(_T("%d,%d"), vmpret, SNFileSize);
		//AfxMessageBox(str, 0, 0);
	}
	else
	{
		AfxMessageBox(_T("��Ȩ�ļ���ʧ��"), 0, 0);
	}
	if (vmpret != 0)
	{
		//AfxMessageBox(L"��Ȩ��Ч���ѹ��ڣ����������롣", 0, 0);
		CRegDlg dlg;
		INT_PTR nResponse = dlg.DoModal();
		if (nResponse == IDOK)
		{

			AfxMessageBox(_T("���򼴽��رգ��������鿴��Ȩ�Ƿ���Ч��"), 0, 0);
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
		// TODO:  �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO:  �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "����: �Ի��򴴽�ʧ�ܣ�Ӧ�ó���������ֹ��\n");
		TRACE(traceAppMsg, 0, "����: ������ڶԻ�����ʹ�� MFC �ؼ������޷� #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS��\n");
	}
	VMProtectEnd();
	// ɾ�����洴���� shell ��������
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}

