// CNoteViewDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "Notes.h"
#include "CNoteViewDlg.h"


// CNoteViewDlg 对话框

IMPLEMENT_DYNCREATE(CNoteViewDlg, CDHtmlDialog)

CNoteViewDlg::CNoteViewDlg(CWnd* pParent /*=nullptr*/)
	: CDHtmlDialog(IDD_DLG_ITEM, 103, pParent)
{

}

CNoteViewDlg::~CNoteViewDlg()
{
}

void CNoteViewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDHtmlDialog::DoDataExchange(pDX);
}

BOOL CNoteViewDlg::OnInitDialog()
{
	CDHtmlDialog::OnInitDialog();
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

BEGIN_MESSAGE_MAP(CNoteViewDlg, CDHtmlDialog)
END_MESSAGE_MAP()

BEGIN_DHTML_EVENT_MAP(CNoteViewDlg)
	DHTML_EVENT_ONCLICK(_T("ButtonOK"), OnButtonOK)
	DHTML_EVENT_ONCLICK(_T("ButtonCancel"), OnButtonCancel)
END_DHTML_EVENT_MAP()



// CNoteViewDlg 消息处理程序

HRESULT CNoteViewDlg::OnButtonOK(IHTMLElement* /*pElement*/)
{
	OnOK();
	return S_OK;
}

HRESULT CNoteViewDlg::OnButtonCancel(IHTMLElement* /*pElement*/)
{
	OnCancel();
	return S_OK;
}
