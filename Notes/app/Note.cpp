#include "stdafx.h"
#include "Note.h"
#include <fstream>
#include <codecvt>
#include <string>

CNote::CNote()
{
}

CNote::CNote(CString sName)
{
	if (sName.IsEmpty()) sName = CTime::GetCurrentTime().Format(_T("%Y%m%d%H%M%S"));
	if(!CNoteConfig::GetNoteGroup(sName, m_noteGroup))
		CNoteConfig::SetNoteGroup(m_noteGroup);
}

CNote::~CNote(void)
{
}

bool CNote::Create(CString sName)
{
	if (sName.IsEmpty()) sName = CTime::GetCurrentTime().Format(_T("%Y%m%d%H%M%S"));
	if (!CNoteConfig::GetNoteGroup(sName, m_noteGroup)) {
		CNoteConfig::SetNoteGroup(m_noteGroup);
		return false;
	}
	return true;
}

bool CNote::Rename(CString sName)
{
	if (!CNoteConfig::RenameNoteGroup(m_noteGroup.sName, sName)) return false;

	m_noteGroup.sName = sName;
	CNoteConfig::SetNoteGroup(m_noteGroup);

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
	CNoteConfig::SetNoteGroup(m_noteGroup);
}

void CNote::SetNoteItems(vector<NoteItem> items)
{
	m_noteGroup.vNotes = items;
	CNoteConfig::SetNoteGroup(m_noteGroup);
}

void CNote::SetNoteItem(NoteItem item, int nIndex, bool bNew)
{
	if (bNew) m_noteGroup.vNotes.push_back(item);
	else {
		for (int i = 0; i < m_noteGroup.vNotes.size(); i++)
		{
			if (m_noteGroup.vNotes[i].uId == item.uId) {
				m_noteGroup.vNotes[i] = item;
				break;
			}
		}
	}
	CNoteConfig::SetNoteGroup(m_noteGroup);
}

void CNote::UpdateRect(CRect rc)
{
	m_noteGroup.rect = rc;
	CNoteConfig::SetNoteGroup(m_noteGroup);
}

void CNote::UpdateOpacity(int nOpacity)
{
	m_noteGroup.nOpacity = nOpacity;
	CNoteConfig::SetNoteGroup(m_noteGroup);
}

void CNote::UpdateOpacityAble(bool bOpacityAble)
{
	m_noteGroup.bOpacity = bOpacityAble;
	CNoteConfig::SetNoteGroup(m_noteGroup);
}

void CNote::UpdateTopMost(bool bTopMost)
{
	m_noteGroup.bTopMost = bTopMost;
	CNoteConfig::SetNoteGroup(m_noteGroup);
}

void CNote::UpdateBgColor(COLORREF clrBg)
{
	m_noteGroup.bgColor = clrBg;
	CNoteConfig::SetNoteGroup(m_noteGroup);
}

void CNote::UpdateTitle(CString sTitle)
{
	m_noteGroup.sTitle = sTitle;
	CNoteConfig::SetNoteGroup(m_noteGroup);
}

void CNote::MakeTask(NoteItem item)
{
	CString sPath = Path::GetTmpDirectory(Cvt::ToString(CTime(GetCurrentTime()), _T("%Y%m%d%H%M%S")) + _T(".ics"));
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

	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	std::string utf8String = converter.to_bytes((LPCTSTR)icsTpl);

	std::ofstream file(sPath);
	if (file.is_open()) {
		file << utf8String;
		file.close();
	}

	ShellExecute(NULL, _T("open"), sPath, NULL, NULL, SW_SHOWNORMAL);
}

void CNote::Hide()
{
	m_noteGroup.bVisible = true;
	CNoteConfig::SetNoteGroup(m_noteGroup);
}

void CNote::Clear()
{
	DeleteFile(CNoteConfig::NotesDir() + m_noteGroup.sName + _T(".json"));
}