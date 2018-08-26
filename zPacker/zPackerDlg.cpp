
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

// CzPackerDlg �Ի���



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
	ON_COMMAND_RANGE(ID_MENU_POP_ADD, ID_MENU_POP_DEL, OnMenuPopCmd)		//������Ϣӳ��
	ON_NOTIFY(NM_CLICK, IDC_FILE_LIST, &CzPackerDlg::OnNMClickFileList)
	ON_NOTIFY(NM_DBLCLK, IDC_FILE_LIST, &CzPackerDlg::OnNMDblclkFileList)
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
	m_edit = new CEdit;
	m_edit->Create(WS_VISIBLE |  SS_CENTER, CRect(0, 0,0, 0), this, ID_T_EDIT);

	m_edit->ShowWindow(SW_HIDE);
	CMenu m_Menu;
	m_Menu.LoadMenu(IDR_MENU1);
	SetMenu(&m_Menu);

	m_FileList.InsertColumn(0, _T("���"), LVCFMT_CENTER, 50);
	m_FileList.InsertColumn(1, _T("����"), LVCFMT_CENTER, 100);
	m_FileList.InsertColumn(2, _T("·��"), LVCFMT_CENTER, 100);
	m_FileList.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

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

BOOL CzPackerDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE) //����ESC��
		return TRUE; 
	else if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)	//���λس���
	{
		int i = GetFocus()->GetDlgCtrlID();
		if (i == IDCANCEL)
			return TRUE;
	}
	else
		return CDialogEx::PreTranslateMessage(pMsg);
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
//		testFile.Open(Fwilename.GetString(), CFile::modeRead, 0);
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

void CzPackerDlg::OnNMRClickFileList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	POINT pt;
	GetCursorPos(&pt);
	CMenu menu;

	menu.CreatePopupMenu();

	if (pNMItemActivate->iItem == -1)
	{
		menu.AppendMenu(MF_STRING, ID_MENU_POP_ADD, _T("Add"));
	}
	else
	{
		menu.AppendMenu(MF_STRING, ID_MENU_POP_ADD, _T("Add"));
		menu.AppendMenu(MF_STRING, ID_MENU_POP_INSERT, _T("Insert"));
		menu.AppendMenu(MF_STRING, ID_MENU_POP_DEL, _T("Del"));
	}
	menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, this);

	*pResult = 0;
}

void CzPackerDlg::OnMenuPopCmd(UINT ID)
{
	int i;
	POSITION j;
	switch (ID)
	{
	case ID_MENU_POP_ADD:
	{
		int m_Row = m_FileList.GetItemCount();
		CString strTemp;
		strTemp.Format(_T("%d"), m_Row + 1);
		m_FileList.InsertItem(m_Row, strTemp);
		m_FileList.SetItemState(m_Row, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
		m_FileList.EnsureVisible(m_Row, FALSE);
	}
		break;
	case ID_MENU_POP_INSERT:
		break;
	case ID_MENU_POP_DEL:
		//����;
		break;
	default:
		break;
	}
}


void CzPackerDlg::OnNMClickFileList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	

	CRect rc;
	CString strTemp;
	NM_LISTVIEW *pNMListView = (NM_LISTVIEW *)pNMHDR;
	int m_Row = pNMListView->iItem;
	int m_Col = pNMListView->iSubItem;

	if (pNMListView->iItem == -1)	//ѡ��հ״�
	{
		m_edit->ShowWindow(SW_HIDE);
	}

	if (m_Col != 0)	// ѡ������
	{
		m_FileList.GetSubItemRect(m_Row, m_Col, LVIR_LABEL, rc);
		m_edit->SetParent(&m_FileList);
		m_edit->MoveWindow(rc);
		m_edit->SetWindowText(m_FileList.GetItemText(m_Row, m_Col));
		m_edit->ShowWindow(SW_SHOW);
		m_edit->SetFocus();//����Edit����
		m_edit->ShowCaret();//��ʾ���
		m_edit->SetSel(0, -1);//ȫѡ
	}



	*pResult = 0;
}

void CzPackerDlg::OnNMDblclkFileList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	CString strTemp;
	if (pNMItemActivate->iItem == -1)	//ѡ��հ״������һ�У������ý���Ϊ���һ�У��ڶ���
	{
		int m_Row = m_FileList.GetItemCount();
		CString strTemp;
		strTemp.Format(_T("%d"), m_Row + 1);
		m_FileList.InsertItem(m_Row, strTemp);
		m_FileList.SetItemState(m_Row, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
		m_FileList.EnsureVisible(m_Row, FALSE);
	}
	*pResult = 0;
}
