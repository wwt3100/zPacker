
// zPackerDlg.h : 头文件
//

#pragma once
#include "afxmaskededit.h"
#include "afxwin.h"


// CzPackerDlg 对话框
class CzPackerDlg : public CDialogEx
{
// 构造
public:
	CzPackerDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_ZPACKER_DIALOG };

	enum{
		IDC_GROUPBOX = WM_USER + 0x110,

		IDC_EDIT_INPUT = WM_USER + 0x120,
		
		IDC_BTN_OPEN = WM_USER + 0x130,

		IDC_MASK_EDIT = WM_USER + 0x140,

		IDC_LIS_CHECKBOX = WM_USER + 0x150,
	};

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
	void RePaintWindows();

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	
	
public:
	afx_msg void OnBnClickedBtnOpenFile();
	afx_msg void OnBnClickedBtnOpen0();
	afx_msg void OnBnClickedBtnOpen1();
	afx_msg void OnBnClickedMenuAbout();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedBtnCompress();
	afx_msg void OnFileExit();
	LPBYTE PackSel;
	BOOL m_Lock;
	BOOL m_BlockSel[8];
	CString Password;

	CEdit *m_OpenFilePath;

	CButton *m_GroupBox;
	CButton *m_btnOpen;
	CMFCMaskedEdit *m_maskEdit;
	CEdit m_HardID;
	CButton m_btnCompress;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CButton m_cLock;
	afx_msg void OnPackHmi();
	afx_msg void OnUpdatePackHmi(CCmdUI *pCmdUI);
	afx_msg void OnPackMainboard();
	afx_msg void OnUpdatePackMainboard(CCmdUI *pCmdUI);
	afx_msg void OnPackHandle();
	afx_msg void OnUpdatePackHandle(CCmdUI *pCmdUI);
	afx_msg void OnPackLicense();
	afx_msg void OnUpdatePackLicense(CCmdUI *pCmdUI);
	afx_msg void OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);
	afx_msg void OnBnClickedSelLock();
	afx_msg void OnFileOpenjson();
	afx_msg void OnTimer(UINT_PTR nIDEvent);


	afx_msg void OnFileConfig();
	afx_msg void OnFileDecompress();
};
