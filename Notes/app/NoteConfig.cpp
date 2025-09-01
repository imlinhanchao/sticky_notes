#include "stdafx.h"
#include "NoteConfig.h"
#include <string>
#include <locale>
#include <codecvt>

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

	GenericDocument<UTF16<TCHAR>> vData;

	CString sJson;
	if (!XFile::ReadFile(sConfigFile, sJson)) {
		CLogApp::Write(_T("Read Confg File Failed: ") + sConfigFile + _T(", ErrorCode: %d"), GetLastError());
		return false;
	}

	vData.Parse(sJson.GetBuffer()); sJson.ReleaseBuffer();
	if (vData.HasParseError()) {
		CLogApp::Write(_T("Parse Confg File Failed: ") + sConfigFile + _T(", ErrorCode: %d"), vData.GetParseError());
		return false;
	}

	group.sName = vData[_T("name")].GetString();
	group.sTitle = vData[_T("title")].GetString();
	group.rect = CRect(vData[_T("left")].GetInt(), vData[_T("top")].GetInt(), vData[_T("right")].GetInt(), vData[_T("bottom")].GetInt());
	group.bgColor = Cvt::ToColor(vData[_T("bgcolor")].GetString());
	group.nOpacity = vData[_T("opacity")].GetInt();
	group.bOpacity = vData[_T("opacityable")].GetBool();
	group.bVisible = vData[_T("visible")].GetBool();
	group.bTopMost = vData[_T("topmost")].GetBool();
	const rapidjson::GenericValue<UTF16<TCHAR>>& vNotes = vData[_T("notes")];

	if (!vNotes.IsArray()) {
		CLogApp::Write(_T("Notes is not array: ") + sConfigFile);
		return false;
	}

	for (int i = 0; i < vNotes.Size(); i++)
	{
		NoteItem item;
		item.uId = vNotes[i][_T("id")].GetUint64();
		item.sContent = vNotes[i][_T("content")].GetString();
		item.bFinished = vNotes[i][_T("finish")].GetBool();
		group.vNotes.push_back(item);
	}

	return true;
}

void CNoteConfig::SetNoteGroup( NoteGroup group)
{
	CString sConfigFile = Path::Resolve(NotesDir(), group.sName + _T(".json"));

	CLogApp::Write(_T("SetNoteGroup: ") + sConfigFile);
	Document::AllocatorType allocator;
	rapidjson::GenericValue<UTF8<TCHAR>> vData;
	CString sJson = GetJsonString(group.toJson(vData, allocator));
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

CString CNoteConfig::GetJsonString(rapidjson::GenericValue<UTF8<TCHAR>>& data)
{
	CString sJson = _T("");
	if (data.IsObject()) {
		sJson += _T("{");
		for (auto m = data.MemberBegin(); m != data.MemberEnd(); m++)
		{
			sJson += _T("\"") + CString(m->name.GetString()) + _T("\":");
			sJson += GetJsonString(m->value);
			if (m + 1 != data.MemberEnd()) sJson += _T(",");
		}
		sJson += _T("}");
		return sJson;
	}
	if (data.IsArray()) {
		sJson += _T("[");
		for (auto m = data.Begin(); m != data.End(); m++) {
			sJson += GetJsonString(*m);
			if (m + 1 != data.End()) sJson += _T(",");
		}
		sJson += _T("]");
		return sJson;
	}
	if (data.IsBool()) {
		return CString(data.GetBool() ? _T("true") : _T("false"));
	}
	if (data.IsUint()) {
		return Cvt::ToString((UINT)data.GetUint());
	}
	if (data.IsUint64()) {
		return Cvt::ToString((ULONG)data.GetUint64());
	}
	if (data.IsInt()) {
		return Cvt::ToString(data.GetInt());
	}
	if (data.IsInt64()) {
		return Cvt::ToString((long)data.GetInt64());
	}
	if (data.IsNull()) {
		return CString(_T("null"));
	}
	if (data.IsDouble()) {
		return Cvt::ToString(data.GetDouble());
	}
	if (data.IsString()) {
		return _T("\"") + CString(data.GetString()) + _T("\"");
	}
	return CString(_T("\"Unknow Type\""));
}
