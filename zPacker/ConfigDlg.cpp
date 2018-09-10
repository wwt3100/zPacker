// ConfigDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "zPacker.h"
#include "ConfigDlg.h"
#include "afxdialogex.h"


// ConfigDlg �Ի���

IMPLEMENT_DYNAMIC(ConfigDlg, CDialogEx)

ConfigDlg::ConfigDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(ConfigDlg::IDD, pParent)
{

}

ConfigDlg::~ConfigDlg()
{
}

void ConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CFG_SHOW, m_show);
	DDX_Control(pDX, IDC_EDIT_KEY, m_key);
}


BEGIN_MESSAGE_MAP(ConfigDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &ConfigDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CFG_SHOW, &ConfigDlg::OnClickedCfgShow)
END_MESSAGE_MAP()


// ConfigDlg ��Ϣ�������


void ConfigDlg::OnBnClickedOk()
{
	CString str;
	CWinApp* pApp = AfxGetApp();
	GetDlgItemText(IDC_EDIT_KEY,str);
	pApp->WriteProfileString(_T("Config"), _T("Key"), str.GetString());

	CDialogEx::OnOK();
}


BOOL ConfigDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CString str;
	CWinApp* pApp = AfxGetApp();
	str = pApp->GetProfileString(_T("Config"), _T("Key"), _T(""));
	if (str != _T(""))
	{
		m_key.SetWindowText(str);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


void ConfigDlg::OnClickedCfgShow()
{
	if (m_show.GetCheck() == TRUE)
	{
		m_key.SetPasswordChar(_T('*'));
	}
	else
	{
		m_key.SetPasswordChar(NULL);
	}
	UpdateData(TRUE);
}
