// NoteDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Notes.h"
#include "NoteDlg.h"
#include "afxdialogex.h"
#include <functional>
#include <string>

// CNoteDlg dialog

IMPLEMENT_DYNAMIC(CNoteDlg, CDialogEx)

CNoteDlg::CNoteDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CNoteDlg::IDD, pParent)
{

}

CNoteDlg::~CNoteDlg()
{
}

void CNoteDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CNoteDlg, CDialogEx)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CNoteDlg message handlers


BOOL CNoteDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetWindowLongPtr(this->GetSafeHwnd(), GWLP_USERDATA, (LONG_PTR)this);
	Init();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CNoteDlg::OnSize(UINT nType, int cx, int cy)
{
	RECT availableBounds = { 0 };
	GetClientRect(&availableBounds);
	if (m_controller != nullptr)
		m_controller->put_Bounds(availableBounds);
}

void CNoteDlg::Init()
{
	//SetMouseThought();
	SetWindownAlpha(0);
	CRect rc = m_Note.GetNoteGroup().rect;
	SetWindowPos(&wndTopMost, rc.left, rc.top, rc.Width(), rc.Height(), SWP_SHOWWINDOW);

	InitWebView();
}

void CNoteDlg::SetWindownAlpha( float fAlpha )
{
	ModifyStyleEx(0, WS_EX_LAYERED); 
	SetLayeredWindowAttributes(0, 255 * fAlpha, LWA_ALPHA);
}

void CNoteDlg::SetMouseThought(bool bThought)
{
	DWORD dwNewLong = GetWindowLong(m_hWnd, GWL_EXSTYLE); 
	if (bThought) dwNewLong |= WS_EX_TRANSPARENT | WS_EX_LAYERED;
	else dwNewLong &= ~(WS_EX_TRANSPARENT | WS_EX_LAYERED);
	SetWindowLong(m_hWnd, GWL_EXSTYLE, dwNewLong);
}

void CNoteDlg::InitWebView()
{
	CreateCoreWebView2EnvironmentWithOptions(nullptr, nullptr, nullptr,
		Microsoft::WRL::Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
			this, &CNoteDlg::OnCreateEnvironmentCompleted).Get());
}

HRESULT CNoteDlg::OnCreateEnvironmentCompleted(HRESULT result, ICoreWebView2Environment* environment)
{
	m_webViewEnvironment = environment;
	m_webViewEnvironment->CreateCoreWebView2Controller(this->GetSafeHwnd(), 
		Microsoft::WRL::Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
			this, &CNoteDlg::OnCreateCoreWebView2ControllerCompleted).Get());

	return S_OK;
}

HRESULT CNoteDlg::OnCreateCoreWebView2ControllerCompleted(HRESULT result, ICoreWebView2Controller* controller)
{
	if (controller != nullptr) {
		m_controller = controller;
		m_controller->get_CoreWebView2(&m_webView);
	}

	wil::com_ptr<ICoreWebView2Settings> settings;
	m_webView->get_Settings(&settings);
	settings->put_IsScriptEnabled(TRUE);
	settings->put_AreDefaultScriptDialogsEnabled(TRUE);
	settings->put_IsWebMessageEnabled(TRUE);

	RECT bounds;
	::GetClientRect(GetSafeHwnd(), &bounds);
	m_controller->put_Bounds(bounds);

	m_webView->Navigate(L"http://127.0.0.1:5500/");

	EventRegistrationToken token;
	m_webView->add_NavigationStarting(Microsoft::WRL::Callback<ICoreWebView2NavigationStartingEventHandler>(
		[](ICoreWebView2* webview, ICoreWebView2NavigationStartingEventArgs* args) -> HRESULT {
			return S_OK;
		}).Get(), &token);

	m_webView->add_NavigationCompleted(Microsoft::WRL::Callback<ICoreWebView2NavigationCompletedEventHandler>(
		this, &CNoteDlg::OnDocumentReady).Get(), &token);

	m_webView->add_WebMessageReceived(Microsoft::WRL::Callback<ICoreWebView2WebMessageReceivedEventHandler>(
		this, &CNoteDlg::OnWebMessageReceived).Get(), &token);

	m_webView->AddScriptToExecuteOnDocumentCreated(
		GetDocumentReadyJavascript(),
		nullptr);

	return S_OK;
}

HRESULT CNoteDlg::OnWebMessageReceived(ICoreWebView2* webview, ICoreWebView2WebMessageReceivedEventArgs* args)
{
	wil::unique_cotaskmem_string message;
	args->TryGetWebMessageAsString(&message);

	USES_CONVERSION;
	Document d;
	d.Parse(W2A(message.get()));
	bool isObject = d.IsObject();
	bool isString = d.IsString();
	bool isNumber = d.IsNumber();
	bool isBool = d.IsBool();
	bool isArray = d.IsArray();
	bool isNull = d.IsNull();
	bool isInt = d.IsInt();
	bool isUint = d.IsUint();
	bool isInt64 = d.IsInt64();
	bool isUint64 = d.IsUint64();
	bool isDouble = d.IsDouble();


	for (auto it = d.MemberBegin(); it != d.MemberEnd(); it++)
	{
		CLogApp::Debug(A2W(it->name.GetString()));
		if (it->value.IsNumber())
			CLogApp::Debug(_T("%g"), it->value.GetDouble());
		else if (it->value.IsString())
			CLogApp::Debug(A2W(it->value.GetString()));
		else if (it->value.IsBool())
			CLogApp::Debug(it->value.GetBool() ? _T("true") : _T("false"));
		else if (it->value.IsArray())
		{
			for (auto it2 = it->value.Begin(); it2 != it->value.End(); it2++)
			{
				if (it2->IsNumber())
					CLogApp::Debug(_T("%g"), it2->GetDouble());
				else if (it2->IsString())
					CLogApp::Debug(A2W((it2->GetString())));
				else if (it2->IsBool())
					CLogApp::Debug(it2->GetBool() ? _T("true") : _T("false"));
			}
		}

	}

	return E_NOTIMPL;
}

HRESULT CNoteDlg::OnDocumentReady(ICoreWebView2* webview, ICoreWebView2NavigationCompletedEventArgs* args)
{
	SetWindownAlpha(m_Note.GetNoteGroup().nOpacity / 100.0);
	return E_NOTIMPL;
}

void CNoteDlg::ExecuteScript(CString sJavascript, ICoreWebView2ExecuteScriptCompletedHandler* handler)
{
	m_webView->ExecuteScript(sJavascript, handler);
}

const TCHAR* CNoteDlg::GetDocumentReadyJavascript()
{
	return _T("window.valueTest = { ")
		_T("a: '≤‚ ‘' ")
		_T("};");
}



