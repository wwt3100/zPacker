
// zPackerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "zPacker.h"
#include "zPackerDlg.h"
#include "ConfigDlg.h"
#include "afxdialogex.h"
#include<atlconv.h>
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
	ON_COMMAND(ID_PACK_HMI, &CzPackerDlg::OnPackHmi)
	ON_UPDATE_COMMAND_UI(ID_PACK_HMI, &CzPackerDlg::OnUpdatePackHmi)
	ON_COMMAND(ID_PACK_MAINBOARD, &CzPackerDlg::OnPackMainboard)
	ON_UPDATE_COMMAND_UI(ID_PACK_MAINBOARD, &CzPackerDlg::OnUpdatePackMainboard)
	ON_COMMAND(ID_PACK_HANDLE, &CzPackerDlg::OnPackHandle)
	ON_UPDATE_COMMAND_UI(ID_PACK_HANDLE, &CzPackerDlg::OnUpdatePackHandle)
	ON_COMMAND(ID_PACK_LICENSE, &CzPackerDlg::OnPackLicense)
	ON_UPDATE_COMMAND_UI(ID_PACK_LICENSE, &CzPackerDlg::OnUpdatePackLicense)
	ON_WM_INITMENUPOPUP()
	ON_BN_CLICKED(IDC_SEL_LOCK, &CzPackerDlg::OnBnClickedSelLock)
	ON_COMMAND(ID_FILE_OPENJSON, &CzPackerDlg::OnFileOpenjson)
	ON_WM_TIMER()
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

	VMProtectBeginMutation("Timer Init");
	CString RegInfo = _T("zPacker  -Peninsula");
	SetWindowText(RegInfo);
#ifndef _DEBUG
	SetTimer(1, 5000, NULL);
#endif // !_DEBUG
	VMProtectEnd();

	RePaintWindows();

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

void CzPackerDlg::RePaintWindows()
{
	CFont *cf = m_btnCompress.GetFont();
	CWinApp* pApp = AfxGetApp();
	UINT n = 0;
	UINT SelCount;
	if (pApp->GetProfileBinary(_T("Config"), _T("PackSel"), &PackSel, &n) == 0)
	{
		PackSel = new BYTE{ 0x01 };
		pApp->WriteProfileBinary(_T("Config"), _T("PackSel"), PackSel, 1);
		pApp->WriteProfileString(_T("Config"), _T("All Licenses"), _T("Plasma,PlasmaGas,ScanRF,Pure,PurePlus"));
		SelCount = 1;
	}
	else
	{
		SelCount = _mm_popcnt_u32(*PackSel);
	}
	UINT BlockHeight = 90;
	CRect rect, ccr;
	GetWindowRect(&rect);
	MoveWindow(rect.left, rect.top , rect.Width(), 115 + BlockHeight*SelCount);//设置窗口显示的位置以及大小
	GetWindowRect(&rect);
	m_btnCompress.GetWindowRect(ccr);
	ScreenToClient(&ccr);
	GetClientRect(&rect);
	m_btnCompress.MoveWindow(ccr.left, rect.bottom - ccr.Height() - 10, ccr.Width(), ccr.Height());
	m_cLock.GetWindowRect(ccr);
	ScreenToClient(&ccr);
	m_cLock.MoveWindow(ccr.left, rect.bottom - ccr.Height() - 10, ccr.Width(), ccr.Height());
	m_HardID.GetWindowRect(ccr);
	ScreenToClient(&ccr);
	m_HardID.MoveWindow(ccr.left, rect.bottom - ccr.Height() - 10, ccr.Width(), ccr.Height());

	CString GroupBoxName[8] = { _T("HMI"), _T("MainBoard"), _T("Handle"), _T("License(s)"), 0 };
	UINT Sel = *PackSel;
	for (UINT i = 0; i < 8; i++)	//del all
	{
		m_BlockSel[i] = false;
		CWnd *p;
		p = GetDlgItem(IDC_GROUPBOX + i);
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
					p = GetDlgItem(IDC_LIS_CHECKBOX + i);
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
				p = GetDlgItem(IDC_EDIT_INPUT + i);
				if (p != nullptr)
				{
					p->DestroyWindow();
					delete p;
				}
				p = GetDlgItem(IDC_BTN_OPEN + i);
				if (p != nullptr)
				{
					p->DestroyWindow();
					delete p;
				}
				p = GetDlgItem(IDC_MASK_EDIT + i);
				if (p != nullptr)
				{
					p->DestroyWindow();
					delete p;
				}
			}
		}
	}
	for (UINT num = 0, i = 0; i < 8; i++)		//rebuide
	{
		if ((Sel >> i) & 1)
		{
			m_BlockSel[i] = true;
			m_GroupBox = new CButton;
			m_GroupBox->Create(GroupBoxName[i], BS_GROUPBOX | WS_VISIBLE | WS_CLIPSIBLINGS
				, CRect(10, BlockHeight*num + 5, rect.Width() - 10, BlockHeight*num + 5 + BlockHeight - 10), this, IDC_GROUPBOX + i);
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
						CRect(30 + is * 80, BlockHeight*num + 25 + line, 110 + is * 80, BlockHeight*num + 25 + 25 + line), this, IDC_LIS_CHECKBOX + i);
					m_btnOpen->SetFont(cf);
					is++;
					iss++;
				}
			}
			else
			{
				m_OpenFilePath = new CEdit;
				m_OpenFilePath->Create(ES_AUTOHSCROLL | ES_AUTOVSCROLL | WS_BORDER | WS_VISIBLE | WS_TABSTOP,
					CRect(30, BlockHeight*num + 35 + 2, 260, BlockHeight*num + 35 + 2 + 20), this, IDC_EDIT_INPUT + i);
				m_OpenFilePath->SetFont(cf);

				m_btnOpen = new CButton;
				m_btnOpen->Create(_T("Open"), BS_PUSHBUTTON | WS_VISIBLE,
					CRect(280, BlockHeight*num + 35, 360, BlockHeight*num + 35 + 25), this, IDC_BTN_OPEN + i);
				m_btnOpen->SetFont(cf);

				m_maskEdit = new CMFCMaskedEdit;
				m_maskEdit->Create(ES_AUTOHSCROLL | ES_AUTOVSCROLL | WS_BORDER | WS_VISIBLE | WS_TABSTOP,
					CRect(380, BlockHeight*num + 35 + 2, 440, BlockHeight*num + 35 + 2 + 20), this, IDC_MASK_EDIT + i);
				m_maskEdit->EnableMask(_T("dd dd dd"), _T("__.__.__"), _T(' '));
				m_maskEdit->SetWindowText(_T("00.00.01"));
				m_maskEdit->SetFont(cf);
			}
			num++;
		}
	}
}

void CzPackerDlg::OnTimer(UINT_PTR nIDEvent)
{
	VMProtectBeginVirtualization("Timer Check");
#ifndef _DEBUG
	if (nIDEvent == 1)
	{
		if (VMProtectIsProtected() == false || VMProtectIsValidImageCRC() == false || VMProtectIsDebuggerPresent(true) == true)
		{
			CDialog::OnCancel();//自动退出
		}
	}
#endif
	CDialogEx::OnTimer(nIDEvent);
	VMProtectEnd();

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
		size_t dsize = zPack_GetSize_decompressed((LPCSTR)f_buf2);

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
	if (m_BlockSel[0])
		popMenu.AppendMenu(MF_STRING | MF_CHECKED, ID_PACK_HMI, _T("HMI"));
	else
		popMenu.AppendMenu(MF_STRING , ID_PACK_HMI, _T("HMI"));
	if (m_BlockSel[1])
		popMenu.AppendMenu(MF_STRING | MF_CHECKED, ID_PACK_MAINBOARD, _T("MainBoard"));
	else
		popMenu.AppendMenu(MF_STRING, ID_PACK_MAINBOARD, _T("MainBoard"));
	if (m_BlockSel[2])
		popMenu.AppendMenu(MF_STRING | MF_CHECKED, ID_PACK_HANDLE, _T("Handle"));
	else
		popMenu.AppendMenu(MF_STRING | MF_DISABLED, ID_PACK_HANDLE, _T("Handle"));
	if (m_BlockSel[3])
		popMenu.AppendMenu(MF_STRING | MF_CHECKED, ID_PACK_LICENSE, _T("License"));
	else
		popMenu.AppendMenu(MF_STRING , ID_PACK_LICENSE, _T("License"));
	popMenu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, this);

	CDialogEx::OnRButtonUp(nFlags, point);
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
	if (result != IDOK)
		return;

	Filename = openDlg.GetPathName();
	SetDlgItemText(IDC_EDIT_INPUT + 1, Filename.GetString());
}

void CzPackerDlg::OnPackHmi()
{
	m_BlockSel[0] = !m_BlockSel[0];
	BYTE sel = *PackSel;
	if (m_BlockSel[0])
		sel |= 0x01;
	else
		sel &= 0xFE;
	AfxGetApp()->WriteProfileBinary(_T("Config"), _T("PackSel"), &sel, 1);
	RePaintWindows();
}

void CzPackerDlg::OnUpdatePackHmi(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_BlockSel[0]);
}

void CzPackerDlg::OnPackMainboard()
{
	m_BlockSel[1] = !m_BlockSel[1];
	BYTE sel = *PackSel;
	if (m_BlockSel[1])
		sel |= 0x02;
	else
		sel &= 0xFD;
	AfxGetApp()->WriteProfileBinary(_T("Config"), _T("PackSel"), &sel, 1);
	RePaintWindows();
}

void CzPackerDlg::OnUpdatePackMainboard(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_BlockSel[1]);
}

void CzPackerDlg::OnPackHandle()
{
	m_BlockSel[2] = !m_BlockSel[2];
	BYTE sel = *PackSel;
	if (m_BlockSel[2])
		sel |= 0x04;
	else
		sel &= 0xFB;
	AfxGetApp()->WriteProfileBinary(_T("Config"), _T("PackSel"), &sel, 1);
	RePaintWindows();
}

void CzPackerDlg::OnUpdatePackHandle(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_BlockSel[2]);
}

void CzPackerDlg::OnPackLicense()
{
	m_BlockSel[3] = !m_BlockSel[3];
	BYTE sel = *PackSel;
	if (m_BlockSel[3])
		sel |= 0x08;
	else
		sel &= 0xF7;
	AfxGetApp()->WriteProfileBinary(_T("Config"), _T("PackSel"), &sel, 1);
	RePaintWindows();
}

void CzPackerDlg::OnUpdatePackLicense(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_BlockSel[3]);
}



void CzPackerDlg::OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu)	//补全消息处理
{
	CDialogEx::OnInitMenuPopup(pPopupMenu, nIndex, bSysMenu);

	ASSERT(pPopupMenu != NULL);
	// Check the enabled state of various menu items.

	CCmdUI state;
	state.m_pMenu = pPopupMenu;
	ASSERT(state.m_pOther == NULL);
	ASSERT(state.m_pParentMenu == NULL);

	// Determine if menu is popup in top-level menu and set m_pOther to
	// it if so (m_pParentMenu == NULL indicates that it is secondary popup).
	HMENU hParentMenu;
	if (AfxGetThreadState()->m_hTrackingMenu == pPopupMenu->m_hMenu)
		state.m_pParentMenu = pPopupMenu;    // Parent == child for tracking popup.
	else if ((hParentMenu = ::GetMenu(m_hWnd)) != NULL)
	{
		CWnd* pParent = this;
		// Child Windows don't have menus--need to go to the top!
		if (pParent != NULL &&
			(hParentMenu = ::GetMenu(pParent->m_hWnd)) != NULL)
		{
			int nIndexMax = ::GetMenuItemCount(hParentMenu);
			for (int nIndex = 0; nIndex < nIndexMax; nIndex++)
			{
				if (::GetSubMenu(hParentMenu, nIndex) == pPopupMenu->m_hMenu)
				{
					// When popup is found, m_pParentMenu is containing menu.
					state.m_pParentMenu = CMenu::FromHandle(hParentMenu);
					break;
				}
			}
		}
	}

	state.m_nIndexMax = pPopupMenu->GetMenuItemCount();
	for (state.m_nIndex = 0; state.m_nIndex < state.m_nIndexMax;
		state.m_nIndex++)
	{
		state.m_nID = pPopupMenu->GetMenuItemID(state.m_nIndex);
		if (state.m_nID == 0)
			continue; // Menu separator or invalid cmd - ignore it.

		ASSERT(state.m_pOther == NULL);
		ASSERT(state.m_pMenu != NULL);
		if (state.m_nID == (UINT)-1)
		{
			// Possibly a popup menu, route to first item of that popup.
			state.m_pSubMenu = pPopupMenu->GetSubMenu(state.m_nIndex);
			if (state.m_pSubMenu == NULL ||
				(state.m_nID = state.m_pSubMenu->GetMenuItemID(0)) == 0 ||
				state.m_nID == (UINT)-1)
			{
				continue;       // First item of popup can't be routed to.
			}
			state.DoUpdate(this, TRUE);   // Popups are never auto disabled.
		}
		else
		{
			// Normal menu item.
			// Auto enable/disable if frame window has m_bAutoMenuEnable
			// set and command is _not_ a system command.
			state.m_pSubMenu = NULL;
			state.DoUpdate(this, FALSE);
		}

		// Adjust for menu deletions and additions.
		UINT nCount = pPopupMenu->GetMenuItemCount();
		if (nCount < state.m_nIndexMax)
		{
			state.m_nIndex -= (state.m_nIndexMax - nCount);
			while (state.m_nIndex < nCount &&
				pPopupMenu->GetMenuItemID(state.m_nIndex) == state.m_nID)
			{
				state.m_nIndex++;
			}
		}
		state.m_nIndexMax = nCount;
	}
}


void CzPackerDlg::OnBnClickedSelLock()
{
	if (m_cLock.GetCheck())
		m_HardID.ShowWindow(TRUE);
	else
		m_HardID.ShowWindow(FALSE);
}


void CzPackerDlg::OnFileOpenjson()
{
	CString Filename;
	CFileDialog openDlg(TRUE, _T("JSON File(*.json)|*.json|All File(*.*)|*.*"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("JSON File(*.json)|*.json|All File(*.*)|*.*||"), this);
	INT_PTR result = openDlg.DoModal();
	if (result != IDOK)
		return;
	Filename = openDlg.GetPathName();
	
	cJSON_Hooks cjh;
	cjh.free_fn = free;
	cjh.malloc_fn = malloc;
	cJSON_InitHooks(&cjh);
}

void CzPackerDlg::OnBnClickedBtnCompress()
{
	USES_CONVERSION;
	IN_FILEINFO infile = { 0 };
	OUT_FILEINFO outfile = { 0 };
	strcpy_s(outfile.FilePath,255, "out.zp");
	for (UINT i = 0; i < 3; i++)
	{
		if (m_BlockSel[i])
		{
			CString path;
			GetDlgItemText(IDC_EDIT_INPUT + i, path);
			int nPos = path.ReverseFind('\\'); // 文件路径，以'\'斜杠分隔的路径  
			CString csFileFullName;
			csFileFullName = path.Right(path.GetLength() - nPos - 1); // 获取文件全名，包括文件名和扩展名  
			fopen_s(&infile.pFil, T2A(path), "rb");
			strcpy_s(infile.FileName, 255, T2A(csFileFullName));
			UINT n;
			zPack_Compress_File(&infile, &outfile, &n);
			zPack_Compress_End(&outfile);
		}
	}
}


