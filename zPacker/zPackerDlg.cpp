
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
	VMProtectSerialNumberData VMPSN = {0};
	CDialogEx::OnInitDialog();
	//VMProtectGetCurrentHWID(HWID, 512);
	//MultiByteToWideChar(CP_ACP, 0, HWID, -1, wstr, 512);
	//SetDlgItemText(IDC_REG_INFO, wstr);
	VMProtectGetSerialNumberData(&VMPSN,sizeof(VMProtectSerialNumberData));
#ifdef _DEBUG
	CString RegInfo = _T("Peninsula");
#else
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
	, m_Lock(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CzPackerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_SEL_LOCK, m_Lock);
	DDX_Control(pDX, IDC_HARD_ID, m_HardID);
}

BEGIN_MESSAGE_MAP(CzPackerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_OPEN_FILE, &CzPackerDlg::OnBnClickedBtnOpenFile)
	ON_BN_CLICKED(ID_ABOUT, &CzPackerDlg::OnBnClickedMenuAbout)
	ON_WM_DROPFILES()
	ON_WM_RBUTTONUP()
	ON_BN_CLICKED(IDC_BTN_COMPRESS, &CzPackerDlg::OnBnClickedBtnCompress)
	ON_COMMAND(ID_FILE_EXIT, &CzPackerDlg::OnFileExit)
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

	CFont *cf = m_HardID.GetFont();
	LOGFONT lf;
	cf->GetLogFont(&lf);
	CWinApp* pApp = AfxGetApp();
	UINT n=0;
	BYTE PackSel[10] = {0};
	if (pApp->GetProfileBinary(_T("Config"), _T("PackSel"), (LPBYTE*)PackSel, &n) == 0)
	{
		PackSel[0] = 0x80;
		pApp->WriteProfileBinary(_T("Config"), _T("PackSel"), PackSel, 1);
	}
	HFONT  hFont = CreateFont(15, 0, 0,0,FW_NORMAL,FALSE,FALSE,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,DEFAULT_PITCH | FF_SWISS,_T("MS Shell Dlg 2"));//MS Shell Dlg		//Consolas
	m_GroupBox = new CButton;
	m_GroupBox->Create(_T("HMI"), BS_GROUPBOX | WS_VISIBLE
		, CRect(10, 25, 480, 100), this, 8888);
	//m_GroupBox->SetFont(cf);
	m_GroupBox->SendMessage(WM_SETFONT, (WPARAM)hFont, TRUE);

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

void CzPackerDlg::OnFileExit()
{
	CDialog::OnCancel();
}

void CzPackerDlg::OnBnClickedBtnOpenFile()
{
	CString Filename;
	CFileDialog openDlg(TRUE, _T("All File(*.*)|*.*"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("All File(*.*)|*.*||"), this);
	INT_PTR result = openDlg.DoModal();
	if (result == IDOK)
	{
		Filename = openDlg.GetPathName();
		SetDlgItemText(IDC_EDIT_INPUT_FILE, Filename.GetString());
		CFile testFile;
		testFile.Open(Filename.GetString(), CFile::modeRead, 0);
		BYTE *f_buf = new BYTE[(DWORD)testFile.GetLength()];
		BYTE *f_buf2 = new BYTE[(DWORD)testFile.GetLength()];
		testFile.Read(f_buf, testFile.GetLength());

		size_t rdsize = zPack_compress(f_buf, (LPSTR)f_buf2, testFile.GetLength());

		size_t csize = zPack_GetSize_compressed((LPCSTR)f_buf2);
		size_t dsize = zPcak_GetSize_decompressed((LPCSTR)f_buf2);

		CString str;
		str.Format(_T("%d,%d,%d"), rdsize, csize, dsize);
		AfxMessageBox(str, 0, 0);
	}
}


void CzPackerDlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnDropFiles(hDropInfo);
}


void CzPackerDlg::OnRButtonUp(UINT nFlags, CPoint point)	//鼠标右键弹出菜单
{
	POINT pt;
	GetCursorPos(&pt);

	CMenu popMenu;
	popMenu.CreatePopupMenu();
	popMenu.AppendMenu(MF_STRING | MF_DISABLED, 0, _T("Pack"));
	popMenu.AppendMenu(MF_SEPARATOR);
	popMenu.AppendMenu(MF_STRING, ID_PACK_HMI, _T("HMI"));
	popMenu.AppendMenu(MF_STRING, ID_PACK_MAINBOARD, _T("MainBoard"));
	popMenu.AppendMenu(MF_STRING | MF_DISABLED, ID_PACK_HANDLE, _T("Handle"));
	popMenu.AppendMenu(MF_STRING, ID_PACK_LICENSE, _T("License"));
	popMenu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, this);

	CDialogEx::OnRButtonUp(nFlags, point);
}


void CzPackerDlg::OnBnClickedBtnCompress()
{
	// TODO:  在此添加控件通知处理程序代码
}





