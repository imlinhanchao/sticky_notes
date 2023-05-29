#pragma once

#ifdef _WIN32_WCE
#error "Windows CE 不支持 CDHtmlDialog。"
#endif

// CNoteViewDlg 对话框

class CNoteViewDlg : public CDHtmlDialog
{
	DECLARE_DYNCREATE(CNoteViewDlg)

public:
	CNoteViewDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CNoteViewDlg();
// 重写
	HRESULT OnButtonOK(IHTMLElement *pElement);
	HRESULT OnButtonCancel(IHTMLElement *pElement);

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_ITEM, IDH = 103 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
	DECLARE_DHTML_EVENT_MAP()
};
