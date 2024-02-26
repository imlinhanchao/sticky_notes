
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
	enum { IDD = IDD_NOTES_DIALOG, WM_TRAYICON = WM_USER + 100 };

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
	afx_msg void OnClose();
	afx_msg LRESULT OnTrayIcon(WPARAM wParam, LPARAM lParam);
	afx_msg void OnNew();
	afx_msg void OnHideall();
	afx_msg void OnShowall();
	afx_msg void OnShow();
	afx_msg void OnQuit();
	afx_msg void OnBnClickedBtnBrowse();
	afx_msg void OnBnClickedOk();
	virtual void OnCancel();

	DECLARE_MESSAGE_MAP()

	void Init();
	void InitSetting(Setting setting);
	Setting ReadSetting();
	void SetHotKey(Setting setting);
	void ClearHotKey(Setting setting);
	void SetTrayIcon();
	void SetWindownAlpha(float fAlpha);
	bool ShowInTaskbar(HWND hWnd, bool isShow);

	CHotKeyEdit m_ActiveHKey;
	CHotKeyEdit m_UnActiveHKey;
	CHotKeyEdit m_ActiveAllHKey;
	CHotKeyEdit m_NewHKey;
	CMenu m_MenuTray;
	NOTIFYICONDATA m_nid;

	HANDLE m_Instance;
public:
	static bool m_bIsNoticeRuntime;
	static CAppCtrl m_ctrl;
	Setting m_setting;
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedBtnBrowseRuntime();
	afx_msg void OnMenuThroughAllOn();
	afx_msg void OnMenuThroughAllOff();
};
