
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

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnOpenFile();
	afx_msg void OnBnClickedMenuAbout();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedBtnCompress();
	afx_msg void OnEnSetfocusEditVersion1();
	afx_msg void OnFileExit();
	BOOL m_Lock;
	CButton *m_GroupBox;
	CEdit m_HardID;
};
