#include "stdafx.h"
#include "Note.h"
#include <locale> 

CNote::CNote()
{
}

CNote::CNote(CString sName)
{
	if (sName.IsEmpty()) sName = CTime::GetCurrentTime().Format(_T("%Y%m%d%H%M%S"));
	if(!CConfig::GetNoteGroup(sName, m_noteGroup))
		CConfig::SetNoteGroup(m_noteGroup);
}

CNote::~CNote(void)
{
}

bool CNote::Create(CString sName)
{
	if (sName.IsEmpty()) sName = CTime::GetCurrentTime().Format(_T("%Y%m%d%H%M%S"));
	if (!CConfig::GetNoteGroup(sName, m_noteGroup)) {
		CConfig::SetNoteGroup(m_noteGroup);
		return false;
	}
	return true;
}

bool CNote::Rename(CString sName)
{
	if (!CConfig::RenameNoteGroup(m_noteGroup.sName, sName)) return false;

	m_noteGroup.sName = sName;
	CConfig::SetNoteGroup(m_noteGroup);

	return true;
}

CString CNote::GetName()
{
	return m_noteGroup.sName;
}

NoteGroup& CNote::GetNoteGroup()
{
	return m_noteGroup;
}

void CNote::SetNoteGroup(NoteGroup group)
{
	m_noteGroup = group;
	CConfig::SetNoteGroup(m_noteGroup);
}

void CNote::SetNoteItems(vector<NoteItem> items)
{
	m_noteGroup.vNotes = items;
	CConfig::SetNoteGroup(m_noteGroup);
}

void CNote::SetNoteItem(NoteItem item, int nIndex, bool bNew)
{
	Ini ini(CConfig::NotesDir() + m_noteGroup.sName + _T(".ini"));

	CString sKey = _T("Note") + Cvt::ToString(nIndex);

	if (bNew) ini.Write(_T("Group"), _T("Count"), nIndex + 1);

	item.sContent.Replace(_T("\n"), _T("{{\\n}}"));
	ini.Write(sKey, _T("Id"), item.uId);
	ini.Write(sKey, _T("Content"), item.sContent);
	ini.Write(sKey, _T("Finished"), item.bFinished);
}

void CNote::UpdateRect(CRect rc)
{
	m_noteGroup.rect = rc;
	Ini ini(CConfig::NotesDir() + m_noteGroup.sName + _T(".ini"));

	ini.Write(_T("Group"), _T("Rect"), 
		Cvt::ToString(m_noteGroup.rect.left) + _T(",") +
		Cvt::ToString(m_noteGroup.rect.top) + _T(",") +
		Cvt::ToString(m_noteGroup.rect.right) + _T(",") +
		Cvt::ToString(m_noteGroup.rect.bottom));
}

void CNote::UpdateOpacity(int nOpacity)
{
	m_noteGroup.nOpacity = nOpacity;
	Ini ini(CConfig::NotesDir() + m_noteGroup.sName + _T(".ini"));

	ini.Write(_T("Group"), _T("Opacity"), nOpacity);
}

void CNote::UpdateOpacityAble(bool bOpacityAble)
{
	m_noteGroup.bOpacity = bOpacityAble;
	Ini ini(CConfig::NotesDir() + m_noteGroup.sName + _T(".ini"));

	ini.Write(_T("Group"), _T("OpacityAble"), bOpacityAble);
}

void CNote::UpdateTopMost(bool bTopMost)
{
	m_noteGroup.bTopMost = bTopMost;
	Ini ini(CConfig::NotesDir() + m_noteGroup.sName + _T(".ini"));

	ini.Write(_T("Group"), _T("TopMost"), bTopMost);
}

void CNote::UpdateBgColor(COLORREF clrBg)
{
	m_noteGroup.bgColor = clrBg;
	Ini ini(CConfig::NotesDir() + m_noteGroup.sName + _T(".ini"));

	ini.Write(_T("Group"), _T("BgColor"), Cvt::ToString(clrBg));
}

void CNote::UpdateTitle(CString sTitle)
{
	m_noteGroup.sTitle = sTitle;
	Ini ini(CConfig::NotesDir() + m_noteGroup.sName + _T(".ini"));

	ini.Write(_T("Group"), _T("Title"), sTitle);
}

void CNote::MakeTask(NoteItem item)
{
	CStdioFile file;
	CString sPath = Path::GetTmpDirectory(Cvt::ToString(CTime(GetCurrentTime()), _T("%Y%m%d%H%M%S")) + _T(".ics"));
	if (!file.Open(sPath, CFile::modeCreate | CFile::modeWrite)) return;
	char* old_locale = _strdup(setlocale(LC_ALL, NULL));
	setlocale(LC_ALL, "chs");//设定
	CString icsTpl = _T("BEGIN:VCALENDAR\n\
VERSION:2.0\n\
PRODID:-//Hancel.Lin//StickyNotes//EN\n\
BEGIN:VEVENT\n\
UID:{UID}\n\
DTSTAMP:{DTSTAMP}\n\
DTSTART:{DTSTART}\n\
DTEND :{DTEND}\n\
SUMMARY:{SUMMARY}\n\
LOCATION:\n\
DESCRIPTION:{DESCRIPTION}\n\
EN:VEVENT\n\
END:VCALENDAR");
	icsTpl.Replace(_T("{UID}"), Cvt::ToString(item.uId));
	icsTpl.Replace(_T("{DTSTAMP}"), Cvt::ToString(CTime::GetCurrentTime(), _T("%Y%m%dT%H%M%SZ")));
	icsTpl.Replace(_T("{DTSTART}"), Cvt::ToString(CTime::GetCurrentTime(), _T("%Y%m%dT%H%M%SZ")));
	icsTpl.Replace(_T("{DTEND}"), Cvt::ToString(CTime::GetCurrentTime(), _T("%Y%m%dT%H%M%SZ")));
	icsTpl.Replace(_T("{SUMMARY}"), item.sContent);
	icsTpl.Replace(_T("{DESCRIPTION}"), item.sContent);
	file.WriteString(icsTpl);

	setlocale(LC_ALL, old_locale);//还原
	free(old_locale);

	file.Close();

	ShellExecute(NULL, _T("open"), sPath, NULL, NULL, SW_SHOWNORMAL);
}

void CNote::Hide()
{
	m_noteGroup.bVisible = true;
	Ini ini(CConfig::NotesDir() + m_noteGroup.sName + _T(".ini"));

	ini.Write(_T("Group"), _T("Visible"), false);
}

void CNote::Clear()
{
	DeleteFile(CConfig::NotesDir() + m_noteGroup.sName + _T(".ini"));
}