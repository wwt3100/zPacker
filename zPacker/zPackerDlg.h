
// zPackerDlg.h : ͷ�ļ�
//

#pragma once
#include "afxmaskededit.h"
#include "afxwin.h"


// CzPackerDlg �Ի���
class CzPackerDlg : public CDialogEx
{
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
