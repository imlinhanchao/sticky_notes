#pragma once

class CNote
{
public:
	CNote();
	CNote(CString sName);
	~CNote(void);

	bool Create(CString sName);
	bool Rename(CString sName);
	CString GetName();

	NoteGroup& GetNoteGroup();
	void SetNoteGroup(NoteGroup group);

private:
	NoteGroup m_noteGroup;
};

