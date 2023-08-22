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
	if (pDlg->m_Note.GetNoteGroup().bVisible)
	{
		pDlg->Create(IDD_DLG_NOTE, NULL);
		pDlg->SetMouseThrough(!sName.IsEmpty());
		m_NoteListDlg.push_back(pDlg);
	}
	else {
		delete pDlg;
	}
}

bool CAppCtrl::CheckEdit()
{
	CPoint mousePt;
	GetCursorPos(&mousePt);
	for (int i = 0; i < m_NoteListDlg.size(); i++)
	{
		CRect rc;
		m_NoteListDlg[i]->GetClientRect(&rc);
		m_NoteListDlg[i]->ClientToScreen(&rc);
		if (PtInRect(&rc, mousePt))
		{
			m_NoteListDlg[i]->SetMouseThrough(false);
			return true;
		}
	}
	return false;
}

void CAppCtrl::MouseThrough(bool bThrough)
{
	for (int i = 0; i < m_NoteListDlg.size(); i++)
	{
		m_NoteListDlg[i]->SetMouseThrough(bThrough);
	}
}

void CAppCtrl::Visible(bool bShow)
{
	for (int i = 0; i < m_NoteListDlg.size(); i++)
	{
		m_NoteListDlg[i]->ShowWindow(bShow ? SW_SHOW : SW_HIDE);
	}
}
