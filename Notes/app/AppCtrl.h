#pragma once
#include "NoteDlg.h"

class CAppCtrl
{
public:
	CAppCtrl(void);
	~CAppCtrl(void);

	void Init();
	void New(CString sName = _T(""));
	bool CheckEdit();
	void MouseThrough();
	void Visible(bool bShow);

private:
	vector<CNoteDlg*> m_NoteListDlg;
};

