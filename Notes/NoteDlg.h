#pragma once


// CNoteDlg dialog

class CNoteDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CNoteDlg)

public:
	CNoteDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CNoteDlg();

// Dialog Data
	enum { IDD = IDD_DLG_NOTE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnNcMouseMove(UINT nHitTest, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnRawInput(UINT nInputcode, HRAWINPUT hRawInput);
	afx_msg void OnMove(int x, int y);

	DECLARE_MESSAGE_MAP()

	void Init();

	void SendNoteItems();
	void SendNoteSetting();
	void SendMouseThrough();

	void InitWebView();
	HRESULT OnCreateEnvironmentCompleted(HRESULT result, ICoreWebView2Environment* environment);
	HRESULT OnCreateCoreWebView2ControllerCompleted(HRESULT result, ICoreWebView2Controller* controller);
	HRESULT OnWebMessageReceived(ICoreWebView2* webview, ICoreWebView2WebMessageReceivedEventArgs* args);
	HRESULT OnDocumentReady(ICoreWebView2* webview, ICoreWebView2NavigationCompletedEventArgs* args);

	void SendMessageToWeb(CString sEvent, rapidjson::GenericValue<UTF8<TCHAR>>& data, Document::AllocatorType& allocator);
	void ExecuteScript(CString sJavascript, ICoreWebView2ExecuteScriptCompletedHandler* handler);
	const TCHAR* GetDocumentReadyJavascript();
	void OnMouseMoving(CPoint pt);

private:
	Microsoft::WRL::ComPtr<ICoreWebView2Environment> m_webViewEnvironment;
	Microsoft::WRL::ComPtr<ICoreWebView2Controller> m_controller;
	Microsoft::WRL::ComPtr<ICoreWebView2> m_webView;
	CBrush m_brush;
	bool m_bMoveWindow;
	CRect m_BeginMoveRect;
	CPoint m_BeginMovePoint;

public:
	CNote m_Note;

	void SetWindownAlpha(float fAlpha);
	void SetMouseThrough(bool bThought);
	bool IsMouseThrough();
};
