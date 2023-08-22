#pragma once

class CConfig
{
public:
	static void LoadNoteGroup(vector<CString> &lstName);
	static bool GetNoteGroup(CString sName, NoteGroup &group);
	static void SetNoteGroup(NoteGroup group);
	static bool RenameNoteGroup(CString sOldName, CString sNewName);

	static void LoadSetting(Setting &setting);
	static void SaveSetting(Setting setting);
	static Setting& GetCurrentSetting();
	
	static void SearchThemes(vector<CString>& lstName);

	static CString NotesDir();

private:
	static Setting m_setting;
};

