#pragma once

#include "zPackerDlg.h"
// CProcessDlg 对话框

class CProcessDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CProcessDlg)

public:
	CProcessDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CProcessDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PROCESS };
#endif

private:
	static DWORD WINAPI ThreadProcessPack(LPVOID lpParameter);
	static DWORD WINAPI ThreadProcessDepack(LPVOID lpParameter);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	afx_msg LRESULT OnProcessMessage(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	BOOL PackDepack;
	CString Password;
	CStringArray filearray;
	virtual BOOL OnInitDialog();

	HANDLE  m_ThreadProcessPack;	//压缩线程
	HANDLE  m_ThreadProcessDepack;	//解压线程
	CProgressCtrl m_process_all;
	virtual BOOL DestroyWindow();
	CStatic m_pack_stat;

	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
