
// zPackerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "zPacker.h"
#include "zPackerDlg.h"
#include "ConfigDlg.h"
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
	DDX_Control(pDX, IDC_BTN_COMPRESS, m_btnCompress);
	DDX_Control(pDX, IDC_SEL_LOCK, m_cLock);
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
	ON_BN_CLICKED(IDC_BTN_OPEN, &CzPackerDlg::OnBnClickedBtnOpen0)
	ON_BN_CLICKED(IDC_BTN_OPEN+1, &CzPackerDlg::OnBnClickedBtnOpen1)
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

	CFont *cf = m_btnCompress.GetFont();
	CWinApp* pApp = AfxGetApp();
	UINT n=0;
	LPBYTE PackSel;
	UINT SelCount;
	if (pApp->GetProfileBinary(_T("Config"), _T("PackSel"), &PackSel, &n) == 0)
	{
		PackSel = new BYTE{ 0x0B };
		pApp->WriteProfileBinary(_T("Config"), _T("PackSel"), PackSel, 1);
		pApp->WriteProfileString(_T("Config"), _T("All Licenses"),_T("Plasma,PlasmaGas,ScanRF,Pure,PurePlus"));
		SelCount = 1;
	}
	else
	{
		SelCount = _mm_popcnt_u32(*PackSel);
	}
	UINT BlockHeight = 90;
	CRect rect,ccr;
	GetWindowRect(&rect);
	MoveWindow(rect.left, rect.top - (BlockHeight/2)*SelCount, rect.Width(), rect.Height() + BlockHeight*SelCount);//设置窗口显示的位置以及大小
	GetWindowRect(&rect);
	m_btnCompress.GetWindowRect(ccr);
	ScreenToClient(&ccr);
	GetClientRect(&rect);
	m_btnCompress.MoveWindow(ccr.left, rect.bottom  - ccr.Height()-10, ccr.Width(), ccr.Height());
	m_cLock.GetWindowRect(ccr);
	ScreenToClient(&ccr);
	m_cLock.MoveWindow(ccr.left, rect.bottom - ccr.Height() - 10, ccr.Width(), ccr.Height());
	m_HardID.GetWindowRect(ccr);
	ScreenToClient(&ccr);
	m_HardID.MoveWindow(ccr.left, rect.bottom - ccr.Height() - 10, ccr.Width(), ccr.Height());

	CString GroupBoxName[8] = { _T("HMI"), _T("MainBoard"), _T("Handle"), _T("License(s)"), 0 };
	UINT Sel = *PackSel;
	for (UINT num = 0, i = 0; i < 8; i++)
	{
		if ((Sel >> i) & 1)
		{
			m_GroupBox = new CButton;
			m_GroupBox->Create(GroupBoxName[i], BS_GROUPBOX | WS_VISIBLE | WS_CLIPSIBLINGS
				, CRect(10, BlockHeight*num + 5, rect.Width() - 10, BlockHeight*num + 5 + BlockHeight - 10), this, IDC_GROUPBOX + num);
			m_GroupBox->SetFont(cf);

			if (i == 3)
			{
				CString strGet;
				CString Lis = pApp->GetProfileString(_T("Config"), _T("All Licenses"));
				CStringArray strArry;
				UINT is = 0, iss = 0;
				while (AfxExtractSubString(strGet, Lis, iss, _T(',')))
				{
					UINT line = 0;
					if (iss > 4)
					{
						line += 25;
						is = 0;
					}
					strArry.Add(strGet);
					m_btnOpen = new CButton;
					m_btnOpen->Create((strArry.GetData() + iss)->GetString(), BS_CHECKBOX | WS_VISIBLE | BS_AUTOCHECKBOX,
						CRect(30 + is * 80, BlockHeight*num + 25 + line, 110 + is * 80, BlockHeight*num + 25 + 25 + line), this, IDC_LIS_CHECKBOX + num);
					m_btnOpen->SetFont(cf);
					is++;
					iss++;
				}
			}
			else
			{
				m_OpenFilePath = new CEdit;
				m_OpenFilePath->Create(ES_AUTOHSCROLL | ES_AUTOVSCROLL | WS_BORDER | WS_VISIBLE | WS_TABSTOP,
					CRect(30, BlockHeight*num + 35 + 2, 260, BlockHeight*num + 35 + 2 + 20), this, IDC_EDIT_INPUT + num);
				m_OpenFilePath->SetFont(cf);

				m_btnOpen = new CButton;
				m_btnOpen->Create(_T("Open"), BS_PUSHBUTTON | WS_VISIBLE,
					CRect(280, BlockHeight*num + 35, 360, BlockHeight*num + 35 + 25), this, IDC_BTN_OPEN + num);
				m_btnOpen->SetFont(cf);

				m_maskEdit = new CMFCMaskedEdit;
				m_maskEdit->Create(ES_AUTOHSCROLL | ES_AUTOVSCROLL | WS_BORDER | WS_VISIBLE | WS_TABSTOP,
					CRect(380, BlockHeight*num + 35 + 2, 440, BlockHeight*num + 35 + 2 + 20), this, IDC_MASK_EDIT + num);
				m_maskEdit->EnableMask(_T("dd dd dd"), _T("__.__.__"), _T(' '));
				m_maskEdit->SetWindowText(_T("00.00.01"));
				m_maskEdit->SetFont(cf);
			}
			num++;
		}
		else
		{
			CWnd *p;
			p = GetDlgItem(IDC_GROUPBOX + num);
			if (p != nullptr)
			{
				p->DestroyWindow();
				delete p;
				if (i == 3)
				{
					CString strGet;
					CString Lis = pApp->GetProfileString(_T("Config"), _T("All Licenses"));
					CStringArray strArry;
					UINT is = 0, iss = 0;
					while (AfxExtractSubString(strGet, Lis, iss, _T(',')))
					{
						UINT line = 0;
						if (iss > 4)
						{
							line += 25;
							is = 0;
						}
						p = GetDlgItem(IDC_LIS_CHECKBOX + num);
						if (p != nullptr)
						{
							p->DestroyWindow();
							delete p;
						}
						is++;
						iss++;
					}
				}
				else
				{
					p = GetDlgItem(IDC_EDIT_INPUT + num);
					if (p != nullptr)
					{
						p->DestroyWindow();
						delete p;
					}
					p = GetDlgItem(IDC_BTN_OPEN + num);
					if (p != nullptr)
					{
						p->DestroyWindow();
						delete p;
					}
					p = GetDlgItem(IDC_MASK_EDIT + num);
					if (p != nullptr)
					{
						p->DestroyWindow();
						delete p;
					}
				}
			}
		}
	}




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

BOOL CzPackerDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE) //屏蔽ESC健
		return TRUE;
	else if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)	//屏蔽回车键
	{
		int i = GetFocus()->GetDlgCtrlID();
		if (i == IDCANCEL)
			return TRUE;
	}
	else if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_F1)
	{
		return TRUE;
	}
	else if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_F5)
	{
		OnBnClickedBtnCompress();
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CzPackerDlg::OnBnClickedBtnOpenFile()
{
	CString Filename;
	CFileDialog openDlg(TRUE, _T("ROM File(*.bin;*.hex)|*.bin;*.hex|All File(*.*)|*.*"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("ROM File(*.bin;*.hex)|*.bin;*.hex|All File(*.*)|*.*||"), this);
	INT_PTR result = openDlg.DoModal();
	if (result == IDOK)
	{
		Filename = openDlg.GetPathName();
		SetDlgItemText(IDC_EDIT_INPUT_FILE, Filename.GetString());
		CFile testFile;
		testFile.Open(Filename.GetString(), CFile::modeRead, 0);
		BYTE *f_buf = new BYTE[(DWORD)testFile.GetLength()];
		BYTE *f_buf2 = new BYTE[(DWORD)testFile.GetLength()];
		testFile.Read(f_buf,(UINT) testFile.GetLength());

		size_t rdsize = zPack_compress(f_buf, (LPSTR)f_buf2, (size_t)testFile.GetLength());

		size_t csize = zPack_GetSize_compressed((LPCSTR)f_buf2);
		size_t dsize = zPcak_GetSize_decompressed((LPCSTR)f_buf2);

		CString str;
		str.Format(_T("%d,%d,%d"), rdsize, csize, dsize);
		MessageBox(str);
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
	popMenu.AppendMenu(MF_STRING | MF_DISABLED, ID_PACK_LICENSE, _T("License"));
	popMenu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, this);

	CDialogEx::OnRButtonUp(nFlags, point);
}


void CzPackerDlg::OnBnClickedBtnCompress()
{
	ConfigDlg dlg;
	dlg.DoModal();
}

void CzPackerDlg::OnBnClickedBtnOpen0()
{
	CFolderPickerDialog fd(NULL, 0, this, 0);
	if (fd.DoModal() == IDOK)
	{
		SetDlgItemText(IDC_EDIT_INPUT, fd.GetPathName());
	}
	//CString m_strFileOut = _T("");
	//TCHAR servPath[MAX_PATH];//用来存放文件夹路径  
	//BROWSEINFO bi;
	//LPITEMIDLIST pidl;
	//bi.hwndOwner = this->m_hWnd;
	//bi.pidlRoot = NULL;
	//bi.pszDisplayName = servPath;
	//bi.lpszTitle = _T("选择文件路径");
	//bi.ulFlags = BIF_RETURNONLYFSDIRS;
	//bi.lpfn = NULL;
	//bi.lParam = NULL;
	//bi.iImage = NULL;
	//if ((pidl = SHBrowseForFolder(&bi)) != NULL)
	//{
	//	if (SUCCEEDED(SHGetPathFromIDList(pidl, servPath))) //得到文件夹的全路径，不要的话，只得本文件夹名  
	//	{
	//		SetDlgItemText(IDC_EDIT_INPUT, servPath);
	//	}
	//}
}
void CzPackerDlg::OnBnClickedBtnOpen1()
{
	CString Filename;
	CFileDialog openDlg(TRUE, _T("ROM File(*.bin;*.hex)|*.bin;*.hex|All File(*.*)|*.*"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("ROM File(*.bin;*.hex)|*.bin;*.hex|All File(*.*)|*.*||"), this);
	INT_PTR result = openDlg.DoModal();
	if (result == IDOK)
	{
		Filename = openDlg.GetPathName();
		SetDlgItemText(IDC_EDIT_INPUT + 1, Filename.GetString());
	}
}






