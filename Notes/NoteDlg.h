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

	DECLARE_MESSAGE_MAP()

	void Init();
	void SetWindownAlpha(float fAlpha);
	void SetMouseThought();

public:
	CNote m_Note;
};
