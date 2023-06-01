
// NotesDlg.h : header file
//

#pragma once
#include "control/HotKeyEdit.h"

// CNotesDlg dialog
class CNotesDlg : public CDialogEx
{
// Construction
public:
	CNotesDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_NOTES_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedBtnNew();

	DECLARE_MESSAGE_MAP()

	void Init();

public:
	CHotKeyEdit m_ActiveHKey;
	static CAppCtrl m_ctrl;
};
