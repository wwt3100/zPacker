
// zPackerDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"

// CzPackerDlg 对话框
class CzPackerDlg : public CDialogEx
{
	enum ListCmdID
	{
		ID_T_EDIT = WM_USER + 10,
		ID_MENU_POP_ADD ,
		ID_MENU_POP_INSERT,
		ID_MENU_POP_DEL,
	};
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
	/*afx_msg void OnBnClickedBtnOpenFile();*/
	afx_msg void OnBnClickedMenuAbout();

	CListCtrl m_FileList;
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnNMRClickFileList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnMenuPopCmd(UINT ID);
	afx_msg void OnNMClickFileList(NMHDR *pNMHDR, LRESULT *pResult);
	CEdit *m_edit;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnNMDblclkFileList(NMHDR *pNMHDR, LRESULT *pResult);
};
