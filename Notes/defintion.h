#pragma once

typedef struct _NoteItem
{
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
	bool bVisible;
	bool bTopMost;
	std::vector<NoteItem> vNotes;

	NoteGroup()
	{
		bgColor = RGB(0, 0, 1);
		rect = CRect(100, 100, 400, 400);
		// 使用时间戳作为默认名称
		sName = CTime::GetCurrentTime().Format(_T("%Y%m%d%H%M%S"));
		nOpacity = 50;
		bTopMost = bVisible = true;
	}
};

typedef struct _Setting
{
	DWORD dwEditHotKey;
	DWORD dwNewHotKey;
	DWORD dwUnActiveHotKey;
	CString sNoteDir;

	_Setting() {
		dwEditHotKey = ((CHotKeyEdit::WIN | CHotKeyEdit::SHIFT) << 8) | VK_F7;
		dwNewHotKey = ((CHotKeyEdit::WIN | CHotKeyEdit::SHIFT) << 8) | VK_F8;
		dwUnActiveHotKey = ((CHotKeyEdit::WIN | CHotKeyEdit::SHIFT) << 8) | VK_F9;
		sNoteDir = Path::GetCurDirectory(_T("notes"));
	}
}Setting, *PSetting;
