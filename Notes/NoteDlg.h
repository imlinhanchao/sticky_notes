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

	DECLARE_MESSAGE_MAP()

	void Init();
	void SetWindownAlpha(float fAlpha);
	void SetMouseThought(bool bThought);

	void InitWebView();
	HRESULT OnCreateEnvironmentCompleted(HRESULT result, ICoreWebView2Environment* environment);
	HRESULT OnCreateCoreWebView2ControllerCompleted(HRESULT result, ICoreWebView2Controller* controller);
	HRESULT OnWebMessageReceived(ICoreWebView2* webview, ICoreWebView2WebMessageReceivedEventArgs* args);
	HRESULT OnDocumentReady(ICoreWebView2* webview, ICoreWebView2NavigationCompletedEventArgs* args);

	void ExecuteScript(CString sJavascript, ICoreWebView2ExecuteScriptCompletedHandler* handler);
	const TCHAR* GetDocumentReadyJavascript();

private:
	Microsoft::WRL::ComPtr<ICoreWebView2Environment> m_webViewEnvironment;
	Microsoft::WRL::ComPtr<ICoreWebView2Controller> m_controller;
	Microsoft::WRL::ComPtr<ICoreWebView2> m_webView;

public:
	CNote m_Note;
};
