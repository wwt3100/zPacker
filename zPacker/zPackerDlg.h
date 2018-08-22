
// zPackerDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"

// CzPackerDlg �Ի���
class CzPackerDlg : public CDialogEx
{
	enum ListCmdID
	{
		ID_T_EDIT = WM_USER + 10,
		ID_MENU_POP_ADD ,
		ID_MENU_POP_INSERT,
		ID_MENU_POP_DEL,
	};
// ����
public:
	CzPackerDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_ZPACKER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
