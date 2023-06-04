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
		New(vNoteGroup[i]);
	}
}

void CAppCtrl::New(CString sName)
{
	CNoteDlg* pDlg = new CNoteDlg();
	pDlg->m_Note.Create(sName);
	pDlg->Create(IDD_DLG_NOTE, NULL);
	m_NoteListDlg.push_back(pDlg);
}
