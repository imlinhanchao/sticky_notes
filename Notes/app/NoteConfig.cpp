#include "stdafx.h"
#include "NoteConfig.h"

Setting CNoteConfig::m_setting;

CString CNoteConfig::NotesDir()
{
	CString sNoteDir = m_setting.sNoteDir;
	if (!Path::Exists(sNoteDir))
	{
		Path::Create(sNoteDir);
	}
	return sNoteDir;
}

void CNoteConfig::LoadNoteGroup(vector<CString>& lstName)
{
	vector<CString> lstConfig = Path::GetFileList(NotesDir(), _T("*.json"));
	for (int i = 0; i < lstConfig.size(); i++)
	{
		CString sNote = Path::GetFileName(lstConfig[i]);
		lstName.push_back(sNote.Mid(0, sNote.GetLength() - 5));
	}
}

bool CNoteConfig::GetNoteGroup(CString sName, NoteGroup& group)
{
	CString sConfigFile = Path::Resolve(NotesDir(), sName + _T(".json"));

	group.sName = sName;
	if (!Path::Exists(sConfigFile)) {
		CLogApp::Write(_T("Confg File Not Exists: ") + sConfigFile);
		return false;
	}

	JsonValue vData;
	CString sJson;
	if (!XFile::ReadFile(sConfigFile, sJson)) {
		CLogApp::Write(_T("Read Confg File Failed: ") + sConfigFile + _T(", ErrorCode: %d"), GetLastError());
		return false;
	}

	if (!JSON::Parse(sJson, vData)) {
		CLogApp::Write(_T("Parse Confg File Failed: ") + sConfigFile + _T(", ErrorMessage: %s"), JSON::s_sLastError);
		return false;
	}

	group.sName = vData[_T("Name")].ToString();
	group.sTitle = vData[_T("Title")].ToString();
	group.rect = CRect(vData[_T("Left")].ToIntegral(), vData[_T("Top")].ToIntegral(), vData[_T("Right")].ToIntegral(), vData[_T("Bottom")].ToIntegral());
	group.bgColor = (COLORREF)vData[_T("BgColor")].ToIntegral();
	group.nOpacity = vData[_T("Opacity")].ToIntegral();
	group.bOpacity = vData[_T("OpacityAble")].ToBool();
	group.bVisible = vData[_T("Visible")].ToBool();
	group.bTopMost = vData[_T("TopMost")].ToBool();
	JsonValue vNotes = vData[_T("Notes")];

	if (!vNotes.IsArray()) {
		CLogApp::Write(_T("Notes is not array: ") + sConfigFile);
		return false;
	}

	for (int i = 0; i < vNotes.size(); i++)
	{
		NoteItem item;
		item.uId = vNotes[i][_T("Id")].ToIntegral();
		item.sContent = vNotes[i][_T("Content")].ToString();
		item.bFinished = vNotes[i][_T("Finished")].ToBool();
		group.vNotes.push_back(item);
	}

	return true;
}

void CNoteConfig::SetNoteGroup( NoteGroup group)
{
	CString sConfigFile = Path::Resolve(NotesDir(), group.sName + _T(".json"));

	CLogApp::Write(_T("SetNoteGroup: ") + sConfigFile);

	JsonValue vData;
	vData.Append(_T("Name"), group.sName);
	vData.Append(_T("Title"), group.sTitle);
	vData.Append(_T("Left"), group.rect.left);
	vData.Append(_T("Top"), group.rect.top);
	vData.Append(_T("Right"), group.rect.right);
	vData.Append(_T("Bottom"), group.rect.bottom);
	vData.Append(_T("BgColor"), (int)group.bgColor);
	vData.Append(_T("Opacity"), group.nOpacity);
	vData.Append(_T("OpacityAble"), group.bOpacity);
	vData.Append(_T("Visible"), group.bVisible);
	vData.Append(_T("TopMost"), group.bTopMost);

	JsonValue vNotes;
	for (int i = 0; i < group.vNotes.size(); i++) {
		NoteItem item = group.vNotes.at(i);
		JsonValue vItem;
		vItem.Append(_T("Id"), (long)item.uId);
		vItem.Append(_T("Content"), item.sContent);
		vItem.Append(_T("Finished"), item.bFinished);
		vNotes.Append(_T(""), vItem);
	}
	vData.Append(_T("Notes"), vNotes);
	CString sJson = vData.ToStyledString();
	if (!XFile::WriteFile(sConfigFile, sJson)) {
		CLogApp::Write(_T("Write Confg File Failed: ") + sConfigFile + _T(", ErrorCode: %d"), GetLastError());
		return;
	}
}

bool CNoteConfig::RenameNoteGroup(CString sOldName, CString sNewName)
{
	CString sOldConfigFile = Path::Resolve(NotesDir(), sOldName + _T(".json"));
	CString sNewConfigFile = Path::Resolve(NotesDir(), sNewName + _T(".json"));
	if (Path::Exists(sOldConfigFile) && !Path::Exists(sNewConfigFile))
	{
		CopyFile(sOldConfigFile, sNewConfigFile, FALSE);
		DeleteFile(sOldConfigFile);
		return true;
	}
	return false;
}

void CNoteConfig::LoadSetting(Setting& setting)
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

void CNoteConfig::SaveSetting(Setting setting)
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

Setting& CNoteConfig::GetCurrentSetting()
{
	return m_setting;
}

void CNoteConfig::SearchThemes(vector<CString>& lstName)
{
	vector<CString> lstConfig = Path::GetFileList(Path::GetCurDirectory(_T("themes")), _T("*"), true);
	for (int i = 0; i < lstConfig.size(); i++)
	{
		if (!Path::Exists(_PATH_JOIN(lstConfig[i], _T("index.html")))) continue;
		CString sNote = Path::GetFileName(lstConfig[i]);
		lstName.push_back(sNote);
	}
}
