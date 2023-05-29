#pragma once

class CConfig
{
public:
	static void LoadNoteGroup(vector<CString> &lstName);
	static bool GetNoteGroup(CString sName, NoteGroup &group);
	static void SetNoteGroup(CString sName, NoteGroup group);
	static bool RenameNoteGroup(CString sOldName, CString sNewName);

	static void LoadSetting(Setting &setting);
	static void SaveSetting(Setting setting);
	
private:
	static CString& NotesDir();
	static Setting m_setting;
};

