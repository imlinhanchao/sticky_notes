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
	void SetNoteItems(vector<NoteItem> items);

	void SetNoteItem(NoteItem item, int nIndex, bool bNew = false);
	void UpdateRect(CRect rc);
	void UpdateOpacity(int nOpacity);
	void UpdateOpacityAble(bool bOpacityAble);
	void UpdateTopMost(bool bTopMost);
	void UpdateBgColor(COLORREF clrBg);
	void UpdateTitle(CString sTitle);
	void Hide();
	void Clear();

private:
	NoteGroup m_noteGroup;
};

