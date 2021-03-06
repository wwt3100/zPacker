#pragma once


// CPasswordDlg 对话框

class CPasswordDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPasswordDlg)

public:
	CPasswordDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CPasswordDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PW_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	CEdit m_pw;
	CButton m_show;
	afx_msg void OnBnClickedPwShow();
};
