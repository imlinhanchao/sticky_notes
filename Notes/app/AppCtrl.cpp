#include "stdafx.h"
#include "AppCtrl.h"

CAppCtrl::CAppCtrl(void)
{
}

CAppCtrl::~CAppCtrl(void)
{
}

void CAppCtrl::Init()
{
	vector<CString> vNoteGroup;
	CConfig::LoadNoteGroup(vNoteGroup);

	for (int i = 0; i < vNoteGroup.size(); i++)
	{
		CNoteDlg* pDlg = new CNoteDlg();
		pDlg->m_Note.Create(vNoteGroup[i]);
		pDlg->Create(IDD_DLG_NOTE, NULL);
		pDlg->ShowWindow(SW_SHOW);
		m_NoteListDlg.push_back(pDlg);
	}
}
