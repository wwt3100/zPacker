// CProcessDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "zPacker.h"
#include "CProcessDlg.h"
#include "afxdialogex.h"


// CProcessDlg 对话框

IMPLEMENT_DYNAMIC(CProcessDlg, CDialogEx)

CProcessDlg::CProcessDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PROCESS, pParent)
{

}

CProcessDlg::~CProcessDlg()
{
}

void CProcessDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS1, m_process_all);
	DDX_Control(pDX, IDC_PROGRESS2, m_process_single);
	DDX_Control(pDX, IDC_PACK_STATIC, m_pack_stat);
}

LRESULT CProcessDlg::OnProcessMessage(WPARAM wParam, LPARAM lParam)
{
	return LRESULT();
}


BEGIN_MESSAGE_MAP(CProcessDlg, CDialogEx)
	ON_MESSAGE(WM_PROCESS_MESSAGE, OnProcessMessage)
END_MESSAGE_MAP()


// CProcessDlg 消息处理程序
typedef struct ProcessNeedMsg {
	CProgressCtrl *Process_all;
	CProgressCtrl *Process_single;
}ProcessNeedMsg;

BOOL CProcessDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	VOID **cpt=new VOID*[3];
	cpt[0] = &m_process_all;
	cpt[1] = &m_process_single;
	cpt[2] = &m_pack_stat;
	m_ThreadProcessPack = CreateThread(0, 0, ThreadProcessPack, cpt, 0, 0); //创建线程 

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

BOOL CProcessDlg::DestroyWindow()
{
	CloseHandle(m_ThreadProcessPack);

	return CDialogEx::DestroyWindow();
}

DWORD WINAPI  CProcessDlg::ThreadProcessPack(LPVOID lpParameter)
{
	CProgressCtrl **cpt = (CProgressCtrl **)lpParameter;
	(*cpt)->SetPos(20);
	(*((CStatic **)cpt + 2))->SetWindowText(_T("1"));
	GetDlgItem(WM_USER + 0x120 + 1,);
	return 0;
}
