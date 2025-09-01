#pragma once

typedef struct _NoteItem
{
	ULONG uId;
	CString sContent;
	bool bFinished;

	_NoteItem()
	{
		uId = 0;
		bFinished = false;
	}

	rapidjson::GenericValue<UTF8<TCHAR>>& toJson(rapidjson::GenericValue<UTF8<TCHAR>>& vItem, Document::AllocatorType& allocator) {
		vItem.SetObject();
		vItem.AddMember(
			rapidjson::GenericValue<UTF8<TCHAR>>(_T("id"), allocator).Move(),
			(uint64_t)uId,
			allocator
		);
		vItem.AddMember(
			rapidjson::GenericValue<UTF8<TCHAR>>(_T("content"), allocator).Move(),
			rapidjson::GenericValue<UTF8<TCHAR>>(sContent.GetBuffer(), allocator).Move(),
			allocator
		);
		sContent.ReleaseBuffer();
		vItem.AddMember(
			rapidjson::GenericValue<UTF8<TCHAR>>(_T("finish"), allocator).Move(),
			bFinished,
			allocator
		);
		return vItem;
	}

}NoteItem, *PNoteItem;

typedef struct NoteGroup
{
	CString sName;
	CString sTitle;
	CRect rect;
	COLORREF bgColor;
	int nOpacity;
	bool bOpacity;
	bool bVisible;
	bool bTopMost;
	std::vector<NoteItem> vNotes;

	NoteGroup()
	{
		bgColor = RGB(11, 15, 20);
		rect = CRect(100, 100, 530, 530);
		// 使用时间戳作为默认名称
		sName = CTime::GetCurrentTime().Format(_T("%Y%m%d%H%M%S"));
		bOpacity = false;
		nOpacity = 50;
		bTopMost = bVisible = true;
	}

	CString toHex() {
		return Cvt::ToHex(bgColor);
	}

	rapidjson::GenericValue<UTF8<TCHAR>>& toSettingJson(rapidjson::GenericValue<UTF8<TCHAR>>& vData, Document::AllocatorType& allocator) {
		vData.SetObject();
		vData.AddMember(
			rapidjson::GenericValue<UTF8<TCHAR>>(_T("name"), allocator).Move(),
			rapidjson::GenericValue<UTF8<TCHAR>>(sName.GetBuffer(), allocator).Move(),
			allocator
		);
		sName.ReleaseBuffer();
		vData.AddMember(
			rapidjson::GenericValue<UTF8<TCHAR>>(_T("title"), allocator).Move(),
			rapidjson::GenericValue<UTF8<TCHAR>>(sTitle.GetBuffer(), allocator).Move(),
			allocator
		);
		sTitle.ReleaseBuffer();
		vData.AddMember(
			rapidjson::GenericValue<UTF8<TCHAR>>(_T("left"), allocator).Move(),
			rect.left,
			allocator
		);
		vData.AddMember(
			rapidjson::GenericValue<UTF8<TCHAR>>(_T("top"), allocator).Move(),
			rect.top,
			allocator
		);
		vData.AddMember(
			rapidjson::GenericValue<UTF8<TCHAR>>(_T("right"), allocator).Move(),
			rect.right,
			allocator
		);
		vData.AddMember(
			rapidjson::GenericValue<UTF8<TCHAR>>(_T("bottom"), allocator).Move(),
			rect.bottom,
			allocator
		);
		CString sColor = toHex();
		vData.AddMember(
			rapidjson::GenericValue<UTF8<TCHAR>>(_T("bgcolor"), allocator).Move(),
			rapidjson::GenericValue<UTF8<TCHAR>>(sColor.GetBuffer(), allocator).Move(),
			allocator
		);
		sColor.ReleaseBuffer();
		vData.AddMember(
			rapidjson::GenericValue<UTF8<TCHAR>>(_T("opacity"), allocator).Move(),
			nOpacity,
			allocator
		);
		vData.AddMember(
			rapidjson::GenericValue<UTF8<TCHAR>>(_T("opacityable"), allocator).Move(),
			bOpacity,
			allocator
		);
		vData.AddMember(
			rapidjson::GenericValue<UTF8<TCHAR>>(_T("visible"), allocator).Move(),
			bVisible,
			allocator
		);
		vData.AddMember(
			rapidjson::GenericValue<UTF8<TCHAR>>(_T("topmost"), allocator).Move(),
			bTopMost,
			allocator
		);
		return vData;
	}

	rapidjson::GenericValue<UTF8<TCHAR>>& toNotesJson(rapidjson::GenericValue<UTF8<TCHAR>>& vData, Document::AllocatorType& allocator) {
		vData.SetArray();
		for (int i = 0; i < vNotes.size(); i++) {
			rapidjson::GenericValue<UTF8<TCHAR>> vItem;
			vData.PushBack(vNotes.at(i).toJson(vItem, allocator), allocator);
		}
		return vData;
	}

	rapidjson::GenericValue<UTF8<TCHAR>>& toJson(rapidjson::GenericValue<UTF8<TCHAR>>& vData, Document::AllocatorType& allocator) {
		vData.SetObject();
		toSettingJson(vData, allocator);
		rapidjson::GenericValue<UTF8<TCHAR>> vNotes;
		vData.AddMember(
			rapidjson::GenericValue<UTF8<TCHAR>>(_T("notes"), allocator).Move(),
			toNotesJson(vNotes, allocator),
			allocator
		);
		return vData;
	}
};

typedef struct _Setting
{
	bool bAutoRun;
	bool bCustomWebview2;
	DWORD dwEditHotKey;
	DWORD dwNewHotKey;
	DWORD dwUnActiveHotKey;
	DWORD dwActiveAllHotKey;
	CString sNoteDir;
	CString sWebview2Path;
	CString sTheme;

	_Setting() {
		bCustomWebview2 = false;
		bAutoRun = true;
		dwEditHotKey = ((CHotKeyEdit::WIN | CHotKeyEdit::SHIFT) << 8) | VK_F7;
		dwNewHotKey = ((CHotKeyEdit::WIN | CHotKeyEdit::SHIFT) << 8) | VK_F8;
		dwUnActiveHotKey = ((CHotKeyEdit::WIN | CHotKeyEdit::SHIFT) << 8) | VK_F9;
		dwActiveAllHotKey = ((CHotKeyEdit::WIN | CHotKeyEdit::SHIFT) << 8) | VK_F10;
		sNoteDir = Path::GetCurDirectory(_T("notes\\"));
		sTheme = _T("Default");
	}
}Setting, *PSetting;
