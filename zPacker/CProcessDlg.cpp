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
	m_ThreadProcessPack = 0;
}

CProcessDlg::~CProcessDlg()
{
}

void CProcessDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS1, m_process_all);
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
	CStringArray filearray;
	VOID **cpt=new VOID*[4];
	cpt[0] = &m_process_all;
	cpt[1] = &m_pack_stat;
	cpt[2] = &filearray;
//	m_ThreadProcessPack = CreateThread(0, 0, ThreadProcessPack, cpt, 0, 0); //创建线程 

	

	//CString str[3];
	//CzPackerDlg *pDlg = (CzPackerDlg*)this->GetParent();


	//pDlg->GetDlgItemText(WM_USER + 0x120 + 1, str[1]);


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

BOOL CProcessDlg::DestroyWindow()
{
	if (m_ThreadProcessPack != nullptr)
		CloseHandle(m_ThreadProcessPack);

	return CDialogEx::DestroyWindow();
}

DWORD WINAPI  CProcessDlg::ThreadProcessPack(LPVOID lpParameter)
{
	CProgressCtrl **cpt = (CProgressCtrl **)lpParameter;
	(*((CStatic **)cpt + 1))->SetWindowText(_T("1"));
	//(*((CStringArray **)cpt + 2))
	USES_CONVERSION;
	
	OUT_FILEINFO outfile = { 0 };
	strcpy_s(outfile.FilePath, 255, "Updata.zp");
	for (UINT num = 0; num < (*((CStringArray **)cpt + 2))->GetCount(); num++)
	{
		(*cpt)->SetPos(num);
		CString fName;
		IN_FILEINFO infile = { 0 };
		fName = (*((CStringArray **)cpt + 3))->GetAt(num);
		fopen_s(&infile.pFil, T2A(fName), "rb");
		int nPos = fName.ReverseFind('\\'); // 文件路径，以'\'斜杠分隔的路径  
		fName = fName.Right(fName.GetLength() - nPos - 1);
		strcpy_s(infile.FileName, 255, T2A(fName));
		
		(*((CStatic **)cpt + 1))->SetWindowText(fName.GetString());
		
		UINT n;
		zPack_Compress_File(&infile, &outfile, &n);
		

	}
	zPack_Compress_End(&outfile);

	return 0;
}
