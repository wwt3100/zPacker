
// zPackerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "zPacker.h"
#include "zPackerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	
// 实现
protected:
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}
BOOL CAboutDlg::OnInitDialog()
{
	//char HWID[512];
	//wchar_t wstr[512];
	CDialogEx::OnInitDialog();
	//VMProtectGetCurrentHWID(HWID, 512);
	//MultiByteToWideChar(CP_ACP, 0, HWID, -1, wstr, 512);
#ifdef _DEBUG
	CString RegInfo = _T("Peninsula");
#else
	VMProtectSerialNumberData VMPSN = {0};
	VMProtectGetSerialNumberData(&VMPSN,sizeof(VMProtectSerialNumberData));
	CString RegInfo = VMPSN.wUserName;
#endif
	RegInfo = _T("Registered to ") + RegInfo;
	SetDlgItemText(IDC_REG_INFO, RegInfo);
	return TRUE;
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// CzPackerDlg 对话框



CzPackerDlg::CzPackerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CzPackerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CzPackerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FILE_LIST, m_FileList);
}

BEGIN_MESSAGE_MAP(CzPackerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//ON_BN_CLICKED(IDC_BTN_OPEN_FILE, &CzPackerDlg::OnBnClickedBtnOpenFile)
	ON_BN_CLICKED(ID_HELP_ABOUT, &CzPackerDlg::OnBnClickedMenuAbout)
	ON_NOTIFY(NM_RCLICK, IDC_FILE_LIST, &CzPackerDlg::OnNMRClickFileList)
END_MESSAGE_MAP()


// CzPackerDlg 消息处理程序

BOOL CzPackerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码
	CMenu m_Menu;
	m_Menu.LoadMenu(IDR_MENU1);
	SetMenu(&m_Menu);

	m_FileList.InsertColumn(0, _T("课程名称"), LVCFMT_CENTER, 100);
	m_FileList.InsertColumn(1, _T("是否主科"), LVCFMT_CENTER, 100);
	m_FileList.InsertColumn(2, _T("周课时"), LVCFMT_CENTER, 100);
	m_FileList.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_FileList.InsertItem(0, L"row1");
	m_FileList.SetItemText(0, 1,L"123");
	m_FileList.InsertItem(0, L"row2");
	m_FileList.SetItemText(0, 1, L"321");

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CzPackerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CzPackerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CzPackerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CzPackerDlg::OnBnClickedMenuAbout()
{
	CAboutDlg dlg;
	INT_PTR nResponse = dlg.DoModal();
}

//void CzPackerDlg::OnBnClickedBtnOpenFile()
//{
//	CString Filename;
//	CFileDialog openDlg(TRUE, _T("All File(*.*)|*.*"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("All File(*.*)|*.*||"), this);
//	INT_PTR result = openDlg.DoModal();
//	if (result == IDOK)
//	{
//		Filename = openDlg.GetPathName();
//		SetDlgItemText(IDC_EDIT_INPUT_FILE, Filename.GetString());
//		CFile testFile;
//		testFile.Open(Filename.GetString(), CFile::modeRead, 0);
//		BYTE *f_buf = new BYTE[(DWORD)testFile.GetLength()];
//		BYTE *f_buf2 = new BYTE[(DWORD)testFile.GetLength()];
//		testFile.Read(f_buf, testFile.GetLength());
//
//		size_t rdsize = zPack_compress(f_buf, (LPSTR)f_buf2, testFile.GetLength());
//
//		size_t csize = zPack_GetSize_compressed((LPCSTR)f_buf2);
//		size_t dsize = zPcak_GetSize_decompressed((LPCSTR)f_buf2);
//
//		CString str;
//		str.Format(_T("%d,%d,%d"), rdsize, csize, dsize);
//		AfxMessageBox(str, 0, 0);
//	}
//}

#define WM_RMENU_CMD_TEST_1  WM_USER + 0x010
#define WM_RMENU_CMD_TEST_2  WM_USER + 0x011
#define WM_RMENU_CMD_TEST_3  WM_USER + 0x012

//void CzPackerDlg::OnRButtonUp(UINT nFlags, CPoint point)
//{
//	// TODO:  在此添加消息处理程序代码和/或调用默认值
//	CMenu menu;
//	menu.CreatePopupMenu();
//	menu.AppendMenu(MF_STRING, WM_RMENU_CMD_TEST_1, L"Test 1");
//	menu.AppendMenu(MF_STRING, WM_RMENU_CMD_TEST_1, L"Test 2");
//	menu.AppendMenu(MF_STRING, WM_RMENU_CMD_TEST_1, L"Test 3");
//
//	POINT tpoint;
//	tpoint.x = point.x;
//	tpoint.y = point.y;
//	ClientToScreen(&tpoint);
//	menu.TrackPopupMenu(TPM_LEFTALIGN, tpoint.x, tpoint.y, this);
//
//	CDialogEx::OnRButtonUp(nFlags, point);
//}




void CzPackerDlg::OnNMRClickFileList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	POINT pt;
	GetCursorPos(&pt);
	CMenu menu;

	menu.CreatePopupMenu();
	menu.AppendMenu(MF_STRING, WM_RMENU_CMD_TEST_1, L"Test 1");
	menu.AppendMenu(MF_STRING, WM_RMENU_CMD_TEST_1, L"Test 2");
	menu.AppendMenu(MF_STRING, WM_RMENU_CMD_TEST_1, L"Test 3");

	menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, this);

	*pResult = 0;
}
