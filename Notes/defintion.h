#pragma once

typedef struct _NoteItem
{
	ULONG uId;
	CString sContent;
	bool bFinished;

	_NoteItem()
	{
		bFinished = false;
	}

}NoteItem, *PNoteItem;

typedef struct NoteGroup
{
	CString sName;
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
};

typedef struct _Setting
{
	bool bAutoRun;
	bool bCustomWebview2;
	DWORD dwEditHotKey;
	DWORD dwNewHotKey;
	DWORD dwUnActiveHotKey;
	CString sNoteDir;
	CString sWebview2Path;

	_Setting() {
		bCustomWebview2 = false;
		bAutoRun = true;
		dwEditHotKey = ((CHotKeyEdit::WIN | CHotKeyEdit::SHIFT) << 8) | VK_F7;
		dwNewHotKey = ((CHotKeyEdit::WIN | CHotKeyEdit::SHIFT) << 8) | VK_F8;
		dwUnActiveHotKey = ((CHotKeyEdit::WIN | CHotKeyEdit::SHIFT) << 8) | VK_F9;
		sNoteDir = Path::GetCurDirectory(_T("notes\\"));
	}
}Setting, *PSetting;
