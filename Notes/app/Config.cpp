#include "stdafx.h"
#include "Config.h"

Setting CConfig::m_setting;

CString CConfig::NotesDir()
{
	CString sNoteDir = m_setting.sNoteDir;
	if (!Path::Exists(sNoteDir))
	{
		Path::Create(sNoteDir);
	}
	return sNoteDir;
}

void CConfig::LoadNoteGroup(vector<CString>& lstName)
{
	vector<CString> lstConfig = Path::GetFileList(NotesDir(), _T("*.ini"));
	for (int i = 0; i < lstConfig.size(); i++)
	{
		CString sNote = Path::GetFileName(lstConfig[i]);
		lstName.push_back(sNote.Mid(0, sNote.GetLength() - 4));
	}
}

bool CConfig::GetNoteGroup(CString sName, NoteGroup& group)
{
	CString sConfigFile = Path::Resolve(NotesDir(), sName + _T(".ini"));

	group.sName = sName;
	if (!Path::Exists(sConfigFile)) return false;
	
	Ini ini(sConfigFile);
	int nCount = 0;
	CString sRect;
	ini.Read(_T("Group"), _T("Count"), nCount);
	ini.Read(_T("Group"), _T("Name"), group.sName);
	ini.Read(_T("Group"), _T("Rect"), sRect);
	ini.Read(_T("Group"), _T("Opacity"), group.nOpacity);
	ini.Read(_T("Group"), _T("OpacityAble"), group.bOpacity);
	ini.Read(_T("Group"), _T("Visible"), group.bVisible);
	ini.Read(_T("Group"), _T("TopMost"), group.bTopMost);
	ini.Read(_T("Group"), _T("BgColor"), group.bgColor);
	ini.Read(_T("Group"), _T("Title"), group.sTitle);
	vector<CString> lstRect = Cvt::SplitString(sRect, _T(","));
	group.rect = CRect(_ttoi(lstRect[0]), _ttoi(lstRect[1]), _ttoi(lstRect[2]), _ttoi(lstRect[3]));

	for (int i = 0; i < nCount; i++)
	{
		NoteItem item;
		CString sKey = _T("Note") + Cvt::ToString(i);
		ini.Read(sKey, _T("Id"), item.uId);
		ini.Read(sKey, _T("Content"), item.sContent);
		ini.Read(sKey, _T("Finished"), item.bFinished);
		item.sContent.Replace(_T("{{\\n}}"), _T("\n"));
		group.vNotes.push_back(item);
	}

	return true;
}

void CConfig::SetNoteGroup( NoteGroup group)
{
	CString sConfigFile = Path::Resolve(NotesDir(), group.sName + _T(".ini"));

	CLogApp::Write(_T("SetNoteGroup: ") + sConfigFile);

	Ini ini(sConfigFile);
	ini.Write(_T("Group"), _T("Count"), (int)group.vNotes.size());
	ini.Write(_T("Group"), _T("Name"), group.sName);
	ini.Write(_T("Group"), _T("Opacity"), group.nOpacity);
	ini.Write(_T("Group"), _T("OpacityAble"), group.bOpacity);
	ini.Write(_T("Group"), _T("Visible"), group.bVisible);
	ini.Write(_T("Group"), _T("TopMost"), group.bTopMost);
	ini.Write(_T("Group"), _T("Rect"), Cvt::ToString(group.rect.left) + _T(",") + Cvt::ToString(group.rect.top) + _T(",") + Cvt::ToString(group.rect.right) + _T(",") + Cvt::ToString(group.rect.bottom));
	ini.Write(_T("Group"), _T("BgColor"), group.bgColor);
	ini.Write(_T("Group"), _T("Title"), group.sTitle);

	for (int i = 0; i < group.vNotes.size(); i++) {
		NoteItem item = group.vNotes.at(i);
		CString sKey = _T("Note") + Cvt::ToString(i);
		item.sContent.Replace(_T("\n"), _T("{{\\n}}"));
		ini.Write(sKey, _T("Id"), item.uId);
		ini.Write(sKey, _T("Content"), item.sContent);
		ini.Write(sKey, _T("Finished"), item.bFinished);
	}
}

bool CConfig::RenameNoteGroup(CString sOldName, CString sNewName)
{
	CString sOldConfigFile = Path::Resolve(NotesDir(), sOldName + _T(".ini"));
	CString sNewConfigFile = Path::Resolve(NotesDir(), sNewName + _T(".ini"));
	if (Path::Exists(sOldConfigFile) && !Path::Exists(sNewConfigFile))
	{
		CopyFile(sOldConfigFile, sNewConfigFile, FALSE);
		DeleteFile(sOldConfigFile);
		return true;
	}
	return false;
}

void CConfig::LoadSetting(Setting& setting)
{
	CString sConfigFile = Path::GetCurDirectory(_T("setting.ini"));

	if (!Path::Exists(sConfigFile)) return;

	Ini ini(sConfigFile);
	ini.Read(_T("HotKey"), _T("Edit"), setting.dwEditHotKey);
	ini.Read(_T("HotKey"), _T("New"), setting.dwNewHotKey);
	ini.Read(_T("HotKey"), _T("UnActive"), setting.dwUnActiveHotKey);
	ini.Read(_T("HotKey"), _T("ActiveAll"), setting.dwActiveAllHotKey);
	ini.Read(_T("Setting"), _T("NoteDir"), setting.sNoteDir);
	ini.Read(_T("Setting"), _T("Theme"), setting.sTheme);
	ini.Read(_T("Setting"), _T("AutoRun"), setting.bAutoRun);
	ini.Read(_T("Setting"), _T("CustomWebview2"), setting.bCustomWebview2);
	ini.Read(_T("Setting"), _T("Webview2Path"), setting.sWebview2Path);

	if (setting.sTheme.IsEmpty()) setting.sTheme = _T("Default");

	m_setting = setting;
}

void CConfig::SaveSetting(Setting setting)
{
	CString sConfigFile = Path::GetCurDirectory(_T("setting.ini"));

	setting.sNoteDir = setting.sNoteDir.Trim('\\') + _T("\\");

	Ini ini(sConfigFile);
	ini.Write(_T("HotKey"), _T("Edit"), setting.dwEditHotKey);
	ini.Write(_T("HotKey"), _T("New"), setting.dwNewHotKey);
	ini.Write(_T("HotKey"), _T("UnActive"), setting.dwUnActiveHotKey);
	ini.Write(_T("HotKey"), _T("ActiveAll"), setting.dwActiveAllHotKey);
	ini.Write(_T("Setting"), _T("NoteDir"), setting.sNoteDir);
	ini.Write(_T("Setting"), _T("Theme"), setting.sTheme);
	ini.Write(_T("Setting"), _T("AutoRun"), setting.bAutoRun);
	ini.Write(_T("Setting"), _T("CustomWebview2"), setting.bCustomWebview2);
	ini.Write(_T("Setting"), _T("Webview2Path"), setting.sWebview2Path);

	m_setting = setting;
}

Setting& CConfig::GetCurrentSetting()
{
	return m_setting;
}

void CConfig::SearchThemes(vector<CString>& lstName)
{
	vector<CString> lstConfig = Path::GetFileList(Path::GetCurDirectory(_T("themes")), _T("*"), true);
	for (int i = 0; i < lstConfig.size(); i++)
	{
		if (!Path::Exists(_PATH_JOIN(lstConfig[i], _T("index.html")))) continue;
		CString sNote = Path::GetFileName(lstConfig[i]);
		lstName.push_back(sNote);
	}
}
