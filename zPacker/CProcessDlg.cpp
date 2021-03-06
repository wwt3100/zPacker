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
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CProcessDlg 消息处理程序
typedef struct ProcessNeedMsg {
	CProgressCtrl *Process_all;
	CProgressCtrl *Process_single;
}ProcessNeedMsg;

BOOL CProcessDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	VOID **cpt = new VOID*[4];
	cpt[0] = &m_process_all;
	cpt[1] = &m_pack_stat;
	cpt[2] = &filearray;

	if (PackDepack == TRUE)	//压缩
	{
		m_process_all.SetRange(0, filearray.GetCount()-1);
		m_ThreadProcessPack = CreateThread(0, 0, ThreadProcessPack, cpt, 0, 0); //创建线程 
	}
	else	//解压
	{
		

		m_ThreadProcessPack = CreateThread(0, 0, ThreadProcessDepack, cpt, 0, 0); //创建线程 
	}

	SetTimer(1, 50, NULL);	//设置100ms监控线程是否在运行

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

BOOL CProcessDlg::DestroyWindow()
{
	if (m_ThreadProcessPack != nullptr)
		CloseHandle(m_ThreadProcessPack);

	return CDialogEx::DestroyWindow();
}
void CProcessDlg::OnTimer(UINT_PTR nIDEvent)
{
	DWORD exitcode;
	if (PackDepack == TRUE)
	{
		GetExitCodeThread(m_ThreadProcessPack, &exitcode);
	}
	else
	{
		GetExitCodeThread(m_ThreadProcessDepack, &exitcode);
	}
	if (exitcode == STILL_ACTIVE)
	{	
	}
	else if (exitcode == 0)
	{
		KillTimer(1);
		MessageBox(_T("Done."));
		CProcessDlg::OnOK();
	}
	else
	{
		CString str,str1;
		KillTimer(1);
		str.Format(_T("(Code:0x%08X)"), exitcode);
		m_pack_stat.GetWindowText(str1);
		MessageBox((str1 + _T("\r\n") + str).GetString(), 0, MB_ICONHAND);
		CProcessDlg::OnOK();
	}
	CDialogEx::OnTimer(nIDEvent);
}

/*压缩线程
参数:1.进度条 2.文本栏 3.CStringArray
CStringArray: 1.Key  2-n.FilesPath
*/
DWORD WINAPI  CProcessDlg::ThreadProcessPack(LPVOID lpParameter)
{
	CProgressCtrl **cpt = (CProgressCtrl **)lpParameter;
	//(*((CStatic **)cpt + 1))->SetWindowText(_T("1"));
	//(*((CStringArray **)cpt + 2))
	USES_CONVERSION;
	
	OUT_FILEINFO outfile = { 0 };
	outfile.Key = T2A((*((CStringArray **)cpt + 2))->GetAt(0).GetString());
	strcpy_s(outfile.FilePath, 255, "Updata.zp");
	for (UINT num = 1; num < (UINT)(*((CStringArray **)cpt + 2))->GetCount(); num++)
	{
		(*cpt)->SetPos(num);
		CString fName;
		IN_FILEINFO infile = { 0 };
		fName = (*((CStringArray **)cpt + 2))->GetAt(num);
		if (fName.GetAt(0) == _T('+'))	//区分不同的类型
		{
			infile.Type = 1;
			fName = fName.Right(fName.GetLength() - 1);
		}
		fopen_s(&infile.pFil, T2A(fName), "rb");
		int nPos = fName.ReverseFind('\\'); // 文件路径，以'\'斜杠分隔的路径  
		fName = fName.Right(fName.GetLength() - nPos - 1);
		strcpy_s(infile.FileName, 255, T2A(fName));
		CString stat_str;
		stat_str.Format(_T("(%d/%d) "), num, (*((CStringArray **)cpt + 2))->GetCount()-1);
		stat_str += fName;
		(*((CStatic **)cpt + 1))->SetWindowText(stat_str.GetString());
		
		UINT n;
		zPack_Compress_File(&infile, &outfile, &n);
		fclose(infile.pFil);
		//(*cpt)->SetPos(num+1);	//更新进度条
	}
	zPack_Compress_End(&outfile);
	return 0;
}

/*解压线程
  参数:1.进度条 2.文本栏 3.CStringArray
  CStringArray: 1.Key  2.OutFolderPath

*/
DWORD WINAPI  CProcessDlg::ThreadProcessDepack(LPVOID lpParameter)
{
	CProgressCtrl **cpt = (CProgressCtrl **)lpParameter;




	return 0;
}




