
// NotesDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Notes.h"
#include "NotesDlg.h"
#include "afxdialogex.h"
#include <ole2.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CAppCtrl CNotesDlg::m_ctrl;
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CNotesDlg dialog




CNotesDlg::CNotesDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CNotesDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CNotesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_HKEY_ACTIVE, m_ActiveHKey);
	DDX_Control(pDX, IDC_HKEY_NEW, m_NewHKey);
	DDX_Control(pDX, IDC_HKEY_UNACTIVE, m_UnActiveHKey);
}

BEGIN_MESSAGE_MAP(CNotesDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_MESSAGE(WM_TRAYICON, OnTrayIcon)
	ON_COMMAND(ID_NEW, &CNotesDlg::OnNew)
	ON_COMMAND(ID_HIDEALL, &CNotesDlg::OnHideall)
	ON_COMMAND(ID_SHOWALL, &CNotesDlg::OnShowall)
	ON_COMMAND(ID_SHOW, &CNotesDlg::OnShow)
	ON_COMMAND(ID_QUIT, &CNotesDlg::OnQuit)
	ON_BN_CLICKED(IDC_BTN_BROWSE, &CNotesDlg::OnBnClickedBtnBrowse)
	ON_BN_CLICKED(IDOK, &CNotesDlg::OnBnClickedOk)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CNotesDlg message handlers

BOOL CNotesDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	HRESULT hresult = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

	// IDM_ABOUTBOX must be in the system command range.
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	m_Instance = Utility::ProgramLock(_T("HANCEL_STICKY_NOTES_APP"));
	if (m_Instance == nullptr)
	{
		return FALSE;
	}

	Init();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CNotesDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CNotesDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CNotesDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// 显示/隐藏任务栏图标(COM方式)
bool CNotesDlg::ShowInTaskbar(HWND hWnd, bool isShow)
{
	CoInitialize(nullptr);

	ITaskbarList* pTaskbarList;
	HRESULT hr = CoCreateInstance(CLSID_TaskbarList, nullptr, CLSCTX_INPROC_SERVER, IID_ITaskbarList, (void**)&pTaskbarList);

	if (SUCCEEDED(hr))
	{
		pTaskbarList->HrInit();

		if (isShow)
		{
			pTaskbarList->AddTab(hWnd);
		}
		else
		{
			pTaskbarList->DeleteTab(hWnd);
		}

		CoUninitialize();

		return true;
	}

	CoUninitialize();
	return false;
}


void CNotesDlg::Init()
{
	CLogApp::Init(LOG_ALL);

	m_ctrl.Init();
	CConfig::LoadSetting(m_setting);
	CConfig::SaveSetting(m_setting);
	InitSetting(m_setting);

	SetHotKey();
	SetTrayIcon();
	ShowInTaskbar(m_hWnd, false);
	SetWindownAlpha(0);

	SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_HIDEWINDOW);
}

void CNotesDlg::InitSetting(Setting setting) 
{
	((CButton*)GetDlgItem(IDC_CHK_AUTO))->SetCheck(setting.bAutoRun ? BST_CHECKED : BST_UNCHECKED);
	m_ActiveHKey.SetHotKey(setting.dwEditHotKey);
	m_NewHKey.SetHotKey(setting.dwNewHotKey);
	m_UnActiveHKey.SetHotKey(setting.dwUnActiveHotKey);
	SetDlgItemText(IDC_EDIT_NOTE, setting.sNoteDir);
}

Setting CNotesDlg::ReadSetting()
{
	Setting setting;
	setting.bAutoRun = ((CButton*)GetDlgItem(IDC_CHK_AUTO))->GetCheck() == BST_CHECKED;
	setting.dwEditHotKey = m_ActiveHKey.GetHotKey();
	setting.dwNewHotKey = m_NewHKey.GetHotKey();
	setting.dwUnActiveHotKey = m_UnActiveHKey.GetHotKey();
	GetDlgItemText(IDC_EDIT_NOTE, setting.sNoteDir);
	return setting;
}

void CNotesDlg::SetHotKey()
{
	CHotKey::SetWithCall(m_setting.dwUnActiveHotKey, [](LPVOID lpParam) -> void {
		CNotesDlg* pMain = (CNotesDlg*)lpParam;
		pMain->m_ctrl.MouseThrough();
		}, this, GetSafeHwnd());
	CHotKey::SetWithCall(m_setting.dwEditHotKey, [](LPVOID lpParam) -> void {
		CNotesDlg* pMain = (CNotesDlg*)lpParam;
		pMain->m_ctrl.CheckEdit();
		}, this, GetSafeHwnd());

	CHotKey::SetWithCall(m_setting.dwNewHotKey, [](LPVOID lpParam) -> void {
		CNotesDlg* pMain = (CNotesDlg*)lpParam;
		pMain->m_ctrl.New();
		}, this, GetSafeHwnd());
}

void CNotesDlg::SetTrayIcon()
{
	m_nid.cbSize = sizeof(NOTIFYICONDATA);
	m_nid.hWnd = GetSafeHwnd();
	m_nid.uID = IDR_MAINFRAME;
	m_nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	m_nid.uCallbackMessage = WM_TRAYICON;
	m_nid.hIcon = m_hIcon;
	CString sTip;
	auto version = Utility::GetVersion(Path::GetProgramPath());
	sTip = _T("Sticky Notes V") + version.ToString();
	_tcscpy_s(m_nid.szTip, sTip.GetBuffer());
	sTip.ReleaseBuffer();
	Shell_NotifyIcon(NIM_ADD, &m_nid);

	m_MenuTray.LoadMenu(IDR_MENU1);
	AfxBeginThread([](LPVOID lpParam) -> UINT {
		Sleep(500);
		CNotesDlg* pMain = (CNotesDlg*)lpParam;
		pMain->ShowWindow(SW_HIDE);
		return 0;
	}, this);
	ShowWindow(SW_HIDE);
}

void CNotesDlg::SetWindownAlpha(float fAlpha)
{
	ModifyStyleEx(0, WS_EX_LAYERED);
	SetLayeredWindowAttributes(0, 255 * fAlpha / 100, LWA_ALPHA);
}


BOOL CNotesDlg::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_HOTKEY)    
	{     
		CHotKey::Execute(pMsg->wParam);
	}     

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CNotesDlg::OnClose()
{
	ShowWindow(SW_HIDE);
}

LRESULT CNotesDlg::OnTrayIcon(WPARAM wParam, LPARAM lParam)
{
	if (wParam != IDR_MAINFRAME)
		return 1;
	switch (lParam)
	{
	case WM_LBUTTONDBLCLK:
		OnShow();
		break;
	case WM_RBUTTONDOWN:
		{
			CPoint pt;
			GetCursorPos(&pt);
			SetForegroundWindow();
			m_MenuTray.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, pt.x, pt.y, this);
		}
		break;
	}
	return 0;
}


void CNotesDlg::OnCancel()
{
	ShowWindow(SW_HIDE);
}


void CNotesDlg::OnNew()
{
	m_ctrl.New();
}


void CNotesDlg::OnHideall()
{
	m_ctrl.Visible(false);
}


void CNotesDlg::OnShowall()
{
	m_ctrl.Visible(true);
}


void CNotesDlg::OnShow()
{
	ShowWindow(SW_SHOW);
	SetWindownAlpha(100);
}


void CNotesDlg::OnQuit()
{
	CDialogEx::OnCancel();
}


void CNotesDlg::OnBnClickedBtnBrowse()
{
	CString sPath = Path::Folder(m_hWnd, m_setting.sNoteDir);
	if (sPath.IsEmpty()) return;
	SetDlgItemText(IDC_EDIT_NOTE, sPath);
}


void CNotesDlg::OnBnClickedOk()
{
	Setting setting = ReadSetting();

	if (setting.sNoteDir.IsEmpty()) {
		MessageBox(_T("请选择一个文件夹用于存放便签！"));
		return;
	}

	if (setting.sNoteDir != m_setting.sNoteDir) {
		vector<CString> lstNoteGroup;
		CConfig::LoadNoteGroup(lstNoteGroup);
		if (lstNoteGroup.size() > 0) {
			if (MessageBox(_T("原目录已保存有便签，是否迁移到新目录？"), NULL, MB_YESNO) == IDYES) {
				Path::CopyTo(Path::GetFileList(m_setting.sNoteDir, _T("*.ini")), setting.sNoteDir);
			}
		}
	}

	Utility::SetAutoRun(m_setting.bAutoRun);

	m_setting = setting;
	CConfig::SaveSetting(m_setting);
}


void CNotesDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	Shell_NotifyIcon(NIM_DELETE, &m_nid);
}
