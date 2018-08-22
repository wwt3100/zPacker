
// zPackerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "zPacker.h"
#include "zPackerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	
// ʵ��
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

// CzPackerDlg �Ի���



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


// CzPackerDlg ��Ϣ�������

BOOL CzPackerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
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

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CzPackerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
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
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CDialogEx::OnDropFiles(hDropInfo);
}


void CzPackerDlg::OnRButtonUp(UINT nFlags, CPoint point)	//����Ҽ������˵�
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
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}





