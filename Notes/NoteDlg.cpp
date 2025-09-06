// NoteDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Notes.h"
#include "NoteDlg.h"
#include "NotesDlg.h"
#include "afxdialogex.h"
#include <functional>
#include <string>

// CNoteDlg dialog

IMPLEMENT_DYNAMIC(CNoteDlg, CDialogEx)

CNoteDlg::CNoteDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CNoteDlg::IDD, pParent)
{
	m_bMoveWindow = false;
	m_brush.CreateStockObject(NULL_BRUSH);
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
	ON_WM_NCHITTEST()
	ON_WM_NCMOUSEMOVE()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_INPUT()
	ON_WM_MOVE()
END_MESSAGE_MAP()


// CNoteDlg message handlers


BOOL CNoteDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	Init();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CNoteDlg::OnSize(UINT nType, int cx, int cy)
{
	if(!IsWindowVisible()) return;
	RECT rcbounds = { 0 };
	GetClientRect(&rcbounds);

	if (m_controller != nullptr)
	{
		CRect rc(rcbounds);
		ClientToScreen(&rc);
		m_Note.UpdateRect(rc);
		rcbounds.top += 3;
		rcbounds.left += 3;
		rcbounds.bottom -= 3;
		rcbounds.right -= 3;
		m_controller->put_Bounds(rcbounds);
	}

}


void CNoteDlg::OnMove(int x, int y)
{
	CDialogEx::OnMove(x, y);

	if (!IsWindowVisible()) return;

	RECT rcbounds = { 0 };
	GetClientRect(&rcbounds);

	CRect rc(rcbounds);
	ClientToScreen(&rc);
	m_Note.UpdateRect(rc);

}


LRESULT CNoteDlg::OnNcHitTest(CPoint point)
{
	CLogApp::Debug(_T("OnNcHitTest: (%d,%d)"), point.x, point.y);
	CRect rect;
	GetWindowRect(&rect);
	if (point.x >= rect.right - 3 && point.y < rect.bottom - 3)
		return HTRIGHT;
	else if (point.y >= rect.bottom - 3 && point.x < rect.right - 3)
		return HTBOTTOM;
	else if (point.x >= rect.right - 3 && point.y >= rect.bottom - 3)
		return HTBOTTOMRIGHT;

	return CDialogEx::OnNcHitTest(point);
}


void CNoteDlg::OnNcMouseMove(UINT nHitTest, CPoint point)
{
	CLogApp::Debug(_T("OnNcMouseMove: %d, (%d,%d)"), nHitTest, point.x, point.y);

	if (nHitTest == HTCAPTION || nHitTest == HTSYSMENU || nHitTest == HTMENU || nHitTest == HTCLIENT)
	{
		SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW))); //IDC_ARROW 标准的箭头 
		return;
	}
	else if (nHitTest == HTTOPRIGHT || nHitTest == HTBOTTOMLEFT)
	{
		SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENESW))); //IDC_SIZENESW 双箭头指向东北和西南
		return;
	} else if (nHitTest == HTBOTTOM)
	{
		SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENS)));//IDC_SIZENS 双箭头指向南北
		return;
	}
	else if (nHitTest == HTRIGHT)
	{
		SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZEWE)));//IDC_SIZEWE 双箭头指向东西
		return;
	}

	CDialogEx::OnNcMouseMove(nHitTest, point);
}


BOOL CNoteDlg::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}


void CNoteDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CRect rect;
	GetClientRect(&rect);

	dc.FillRect(&rect, &m_brush);
}


void CNoteDlg::OnRawInput(UINT nInputcode, HRAWINPUT hRawInput)
{
	UINT dwSize = 0;
	BYTE* lpb = NULL;
	RAWINPUT* raw = NULL;

	GetRawInputData(hRawInput, RID_INPUT, NULL, &dwSize, sizeof(RAWINPUTHEADER));
	if (dwSize == 0) goto DONE;

	lpb = new BYTE[dwSize];
	if (GetRawInputData(hRawInput, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER)) != dwSize)
		goto DONE;

	raw = (RAWINPUT*)lpb;

	if (raw->header.dwType == RIM_TYPEMOUSE) {
		RAWMOUSE& rawMouse = raw->data.mouse;
		CPoint pt; GetCursorPos(&pt);
		OnMouseMoving(pt);
	}

DONE:
	if (lpb != NULL) delete lpb;

	CDialogEx::OnRawInput(nInputcode, hRawInput);
}

void CNoteDlg::Init()
{
	SetMouseThrough(true);
	SetWindownAlpha(0);
	CRect rc = m_Note.GetNoteGroup().rect;
	SetWindowPos(m_Note.GetNoteGroup(). bTopMost ? &wndTopMost : &wndNoTopMost, rc.left, rc.top, rc.Width(), rc.Height(), SWP_SHOWWINDOW);

	InitWebView();
}


void CNoteDlg::SetWindownAlpha( float fAlpha )
{
	ModifyStyleEx(0, WS_EX_LAYERED); 
	SetLayeredWindowAttributes(0, 255 * fAlpha / 100, LWA_ALPHA);
}

void CNoteDlg::SetMouseThrough(bool bThought)
{
	DWORD dwNewLong = GetWindowLong(m_hWnd, GWL_EXSTYLE); 
	if (bThought) dwNewLong |= WS_EX_TRANSPARENT;
	else dwNewLong &= ~(WS_EX_TRANSPARENT);
	SetWindowLong(m_hWnd, GWL_EXSTYLE, dwNewLong);
	if (m_webView != NULL) SendMouseThrough();
}

bool CNoteDlg::IsMouseThrough() {
	return GetWindowLong(m_hWnd, GWL_EXSTYLE) & WS_EX_TRANSPARENT;
}


void CNoteDlg::SendNoteItems()
{
	NoteGroup group = m_Note.GetNoteGroup();
	Document::AllocatorType allocator;
	rapidjson::GenericValue<UTF8<TCHAR>> vData;
	SendMessageToWeb(_T("data"), group.toNotesJson(vData, allocator), allocator);
}

void CNoteDlg::SendNoteSetting()
{
	NoteGroup group = m_Note.GetNoteGroup();
	Document::AllocatorType allocator;
	rapidjson::GenericValue<UTF8<TCHAR>> vData;
	SendMessageToWeb(_T("setting"), group.toSettingJson(vData, allocator), allocator);
}

void CNoteDlg::SendMouseThrough()
{
	rapidjson::GenericValue<UTF8<TCHAR>> data;
	Document::AllocatorType allocator;
	data.SetBool(IsMouseThrough());
	SendMessageToWeb(_T("lock"), data, allocator);
}


void CNoteDlg::InitWebView()
{
	const wchar_t* browserExecutableFolder = nullptr;
	CString sCustomPath = Path::GetDirectory(CConfig::GetCurrentSetting().sWebview2Path);
	if (CConfig::GetCurrentSetting().bCustomWebview2 && !sCustomPath.IsEmpty()) {
		browserExecutableFolder = sCustomPath.GetBuffer();
	}
	auto result = CreateCoreWebView2EnvironmentWithOptions(browserExecutableFolder, nullptr, nullptr,
		Microsoft::WRL::Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
			this, &CNoteDlg::OnCreateEnvironmentCompleted).Get());
	if (CConfig::GetCurrentSetting().bCustomWebview2 && !sCustomPath.IsEmpty()) {
		sCustomPath.ReleaseBuffer();
	}
}

HRESULT CNoteDlg::OnCreateEnvironmentCompleted(HRESULT result, ICoreWebView2Environment* environment)
{
	if (result != S_OK) {
		CNotesDlg::m_bIsNoticeRuntime = true;
		if(IDYES == MessageBox(_T("未检测到 Edge Webview2，请点击【是】打开 https://developer.microsoft.com/zh-cn/microsoft-edge/webview2/#download-section 下载安装！"), NULL, MB_YESNO))
			ShellExecute(NULL, _T("open"), _T("https://developer.microsoft.com/zh-cn/microsoft-edge/webview2/#download-section"), NULL, NULL, SW_SHOW);
		return result;
	}
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

	Microsoft::WRL::ComPtr<ICoreWebView2Controller2> controller2;
	m_controller.As(&controller2);

	if (controller2) {
		// 设置背景色为透明
		COREWEBVIEW2_COLOR bgColor = { 0, 0, 0, 0 };
		controller2->put_DefaultBackgroundColor(bgColor); // BGRA格式，A=0表示全透明
	}

	wil::com_ptr<ICoreWebView2Settings> settings;
	m_webView->get_Settings(&settings);
	settings->put_IsScriptEnabled(TRUE);
	settings->put_AreDefaultScriptDialogsEnabled(TRUE);
	settings->put_IsWebMessageEnabled(TRUE);

	RECT bounds;
	::GetClientRect(GetSafeHwnd(), &bounds);
	bounds.top += 3;
	bounds.left+= 3;
	bounds.bottom -= 3;
	bounds.right -= 3;
	m_controller->put_Bounds(bounds);

#ifdef _DEBUG_HTTP
	CString sUrl = _T("http://localhost:5173");
#else
	CString sTheme = Path::GetCurDirectory(_T("themes\\") + CConfig::GetCurrentSetting().sTheme + _T("\\index.html"));
	if (!Path::Exists(sTheme)) sTheme = Path::GetCurDirectory(_T("themes/Default/index.html"));
	CString sUrl = CString(_T("file:///")) + sTheme;
#endif // _DEBUG_HTTP
	sUrl.Replace(_T("\\"), _T("/"));
	m_webView->Navigate(sUrl.GetBuffer());
	sUrl.ReleaseBuffer();

	EventRegistrationToken token;
	m_webView->add_NavigationStarting(Microsoft::WRL::Callback<ICoreWebView2NavigationStartingEventHandler>(
		[this](ICoreWebView2* webview, ICoreWebView2NavigationStartingEventArgs* args) -> HRESULT {
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

	GenericDocument<UTF16<TCHAR>> d;
	d.Parse(message.get());

	/**
	* data format
	* { event: string, data: any }
	*/
	if (!d.HasMember(_T("event"))) return E_NOTIMPL;
	if (!d.HasMember(_T("data"))) return E_NOTIMPL;

	const rapidjson::GenericValue<UTF16<TCHAR>>& data = d[_T("data")];

	CString sEvent = CString(d[_T("event")].GetString());
	if (sEvent == _T("move")) {
		GetClientRect(&m_BeginMoveRect);
		ClientToScreen(&m_BeginMoveRect);
		GetCursorPos(&m_BeginMovePoint);
		m_bMoveWindow = data.GetBool();
		if (m_bMoveWindow) CRawInput::Register(GetSafeHwnd(), RAW_TYPE_MS);
		else CRawInput::Remove(GetSafeHwnd(), RAW_TYPE_MS);
		return S_OK;
	}
	else if (sEvent == _T("lock")) {
		bool bMouseThrough = data.GetBool();
		SetMouseThrough(bMouseThrough);
	}
	else if (sEvent == _T("top")) {
		bool bTop = data.GetBool();
		m_Note.UpdateTopMost(bTop);
		if (bTop) SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		else SetWindowPos(&wndNoTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	}
	else if (sEvent == _T("opacityable")) {
		bool bOpacity = data.GetBool();
		m_Note.UpdateOpacityAble(bOpacity);
		if (bOpacity) SetWindownAlpha(m_Note.GetNoteGroup().nOpacity);
		else SetWindownAlpha(100);
	}
	else if (sEvent == _T("bgcolor")) {
		CString sHex = data.IsNull() ? _T("#0b0f14") : data.GetString();
		m_Note.UpdateBgColor(Cvt::ToColor(sHex));
	}
	else if (sEvent == _T("title")) {
		CString sTitle = data.IsNull() ? _T("") : data.GetString();
		m_Note.UpdateTitle(sTitle);
	}
	else if (sEvent == _T("close")) {
		CDialogEx::OnCancel();
	}
	else if (sEvent == _T("add")) {
		NoteItem item;
		if (!data.HasMember(_T("id"))) return E_NOTIMPL;
		item.uId = data[_T("id")].GetUint64();
		if (!data.HasMember(_T("content"))) return E_NOTIMPL;
		item.sContent = CString(data[_T("content")].GetString());
		if (!data.HasMember(_T("finish"))) return E_NOTIMPL;
		item.bFinished = data[_T("finish")].GetBool();
		m_Note.SetNoteItem(item, m_Note.GetNoteGroup().vNotes.size(), true);
		m_Note.GetNoteGroup().vNotes.insert(m_Note.GetNoteGroup().vNotes.begin(), item);
	}
	else if (sEvent == _T("task")) {
		NoteItem item;
		if (!data.HasMember(_T("id"))) return E_NOTIMPL;
		item.uId = data[_T("id")].GetUint64();
		if (!data.HasMember(_T("content"))) return E_NOTIMPL;
		item.sContent = data[_T("content")].GetString();
		if (!data.HasMember(_T("finish"))) return E_NOTIMPL;
		item.bFinished = data[_T("finish")].GetBool();
		m_Note.MakeTask(item);
	}
	else if (sEvent == _T("update")) {
		vector<NoteItem> vNotes = m_Note.GetNoteGroup().vNotes;
		NoteItem item;
		item.uId = data[_T("id")].GetUint64();
		if (!data.HasMember(_T("content"))) return E_NOTIMPL;
		item.sContent = data[_T("content")].GetString();
		if (!data.HasMember(_T("finish"))) return E_NOTIMPL;
		item.bFinished = data[_T("finish")].GetBool();
		
		for (int i = 0; i < vNotes.size(); i++) {
			if (vNotes[i].uId == item.uId) {
				m_Note.SetNoteItem(item, i);
				m_Note.GetNoteGroup().vNotes[i] = item;
				break;
			}
		}
	}
	else if (sEvent == _T("update_all")) {
		if (!data.IsArray()) return E_NOTIMPL;
		vector<NoteItem> vNotes;
		for (int i = 0; i < data.Size(); i++) {
			NoteItem item;
			item.uId = data[i][_T("id")].GetUint64();
			item.sContent = data[i][_T("content")].GetString();
			item.bFinished = data[i][_T("finish")].GetBool();
			vNotes.push_back(item);
		}
		m_Note.SetNoteItems(vNotes);
	}
	else if (sEvent == _T("remove")) {
		vector<NoteItem> vNotes = m_Note.GetNoteGroup().vNotes;
		NoteItem item;
		item.uId = data[_T("id")].GetUint64();
		for (int i = 0; i < vNotes.size(); i++) {
			if (vNotes[i].uId == item.uId) {
				m_Note.GetNoteGroup().vNotes.erase(m_Note.GetNoteGroup().vNotes.begin() + i);
				CConfig::SetNoteGroup(m_Note.GetNoteGroup());
				break;
			}
		}
	}
	else if (sEvent == _T("hide")) {
		m_Note.Hide();
		CDialogEx::OnCancel();
	}
	else if (sEvent == _T("clear")) {
		m_Note.Clear();
		CDialogEx::OnCancel();
	}
	else if (sEvent == _T("listen")) {
		SendNoteItems();
		SendNoteSetting();
		SendMouseThrough();
	}

	return E_NOTIMPL;
}

HRESULT CNoteDlg::OnDocumentReady(ICoreWebView2* webview, ICoreWebView2NavigationCompletedEventArgs* args)
{
	SetWindownAlpha(m_Note.GetNoteGroup().bOpacity ? m_Note.GetNoteGroup().nOpacity : 100);
	return E_NOTIMPL;
}

void CNoteDlg::SendMessageToWeb(CString sEvent, rapidjson::GenericValue<UTF8<TCHAR>>& data, Document::AllocatorType& allocator)
{
	rapidjson::GenericValue<UTF8<TCHAR>> json;
	json.SetObject();
	json.AddMember(
		rapidjson::GenericValue<UTF8<TCHAR>>(_T("event"), allocator).Move(),
		rapidjson::GenericValue<UTF8<TCHAR>>(sEvent.GetBuffer(), allocator).Move(),
		allocator
	);
	sEvent.ReleaseBuffer();
	json.AddMember(
		rapidjson::GenericValue<UTF8<TCHAR>>(_T("data"), allocator).Move(),
		data,
		allocator
	);
	CString dataJson = CNoteConfig::GetJsonString(json);

	m_webView->PostWebMessageAsJson(dataJson);
}

void CNoteDlg::ExecuteScript(CString sJavascript, ICoreWebView2ExecuteScriptCompletedHandler* handler)
{
	m_webView->ExecuteScript(sJavascript, handler);
}

const TCHAR* CNoteDlg::GetDocumentReadyJavascript()
{
	return _T("");
}

void CNoteDlg::OnMouseMoving(CPoint pt)
{
	if (!m_bMoveWindow) return;
	CPoint ptOffset = m_BeginMovePoint - m_BeginMoveRect.TopLeft();
	CPoint newPosition = pt - ptOffset;
	SetWindowPos(NULL, newPosition.x, newPosition.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}
