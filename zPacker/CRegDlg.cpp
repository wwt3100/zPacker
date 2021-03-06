// CRegDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "zPacker.h"
#include "CRegDlg.h"
#include "afxdialogex.h"


// CRegDlg 对话框

IMPLEMENT_DYNAMIC(CRegDlg, CDialogEx)

CRegDlg::CRegDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_REG, pParent)
{

}

CRegDlg::~CRegDlg()
{
}

void CRegDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CRegDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CRegDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CRegDlg 消息处理程序


BOOL CRegDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

		char HWID[512];
		wchar_t wstr[512];
		VMProtectSerialNumberData VMPSN = { 0 };
		CDialogEx::OnInitDialog();
		VMProtectGetCurrentHWID(HWID, 512);
		MultiByteToWideChar(CP_ACP, 0, HWID, -1, wstr, 512);
		SetDlgItemText(IDC_REG_INFO, wstr);
		//VMProtectGetSerialNumberData(&VMPSN, sizeof(VMProtectSerialNumberData));

		SetDlgItemText(IDC_REG_HWID, wstr);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CRegDlg::OnBnClickedOk()
{
	CString sn;
	CFile RegFile;
	RegFile.Open(_T("Reg.key"), CFile::modeCreate | CFile::modeWrite, 0);
	GetDlgItemText(IDC_REG_REGCODE, sn);
	int leg = sn.GetLength();
	DWORD dBufSize = WideCharToMultiByte(CP_OEMCP, 0, sn, -1, NULL, 0, NULL, FALSE);
	char *str = new char[dBufSize];
	memset(str, 0, dBufSize);
	WideCharToMultiByte(CP_OEMCP, 0, sn.GetString(), -1, str, dBufSize, NULL, FALSE);
	RegFile.Write(str, strnlen_s(str,1024));
	RegFile.Close();
	delete str;
	CDialogEx::OnOK();
}
