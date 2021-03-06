// CPasswordDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "zPacker.h"
#include "zPackerDlg.h"
#include "CPasswordDlg.h"
#include "afxdialogex.h"


// CPasswordDlg 对话框

IMPLEMENT_DYNAMIC(CPasswordDlg, CDialogEx)

CPasswordDlg::CPasswordDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PW_DIALOG, pParent)
{

}

CPasswordDlg::~CPasswordDlg()
{
}

void CPasswordDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_PW, m_pw);
	//DDX_Control(pDX, IDC_CHECK1, m_show);
}


BEGIN_MESSAGE_MAP(CPasswordDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CPasswordDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_PW_SHOW, &CPasswordDlg::OnBnClickedPwShow)
END_MESSAGE_MAP()


// CPasswordDlg 消息处理程序


void CPasswordDlg::OnBnClickedOk()
{
	CzPackerDlg * parent = (CzPackerDlg*)GetParent();
	CString str;
	GetDlgItemText(IDC_EDIT_PW, str);
	parent->Password = str;//把当前编辑框中的值回传给父窗口
	CDialogEx::OnOK();
}


BOOL CPasswordDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_show.SetCheck(TRUE);
	m_pw.SetPasswordChar(_T('*'));
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CPasswordDlg::OnBnClickedPwShow()
{
	if (m_show.GetCheck() == TRUE)
	{
		m_pw.SetPasswordChar(_T('*'));
	}
	else
	{
		m_pw.SetPasswordChar(NULL);
	}
	UpdateData(TRUE);
}
