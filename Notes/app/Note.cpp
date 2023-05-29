#include "stdafx.h"
#include "Note.h"

CNote::CNote()
{
}

CNote::CNote(CString sName)
{
	CConfig::GetNoteGroup(sName, m_noteGroup);
}

CNote::~CNote(void)
{
}

bool CNote::Create(CString sName)
{
	if (!CConfig::GetNoteGroup(sName, m_noteGroup)) return false;
	return true;
}

bool CNote::Rename(CString sName)
{
	if (!CConfig::RenameNoteGroup(m_noteGroup.sName, sName)) return false;

	m_noteGroup.sName = sName;
	CConfig::SetNoteGroup(sName, m_noteGroup);

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
	CConfig::SetNoteGroup(m_noteGroup.sName, m_noteGroup);
}
