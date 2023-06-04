#pragma once
#include "afxwin.h"

class CHotKeyEdit : public CEdit
{
public:
	CHotKeyEdit(void);
	~CHotKeyEdit(void);

	enum { SHIFT = 1, CONTROL= 2, ALT = 4, WIN = 8 };

	void SetSingleKey(bool bSingle, bool bHotKey=false);

private:
	afx_msg void OnRawInput(UINT nInputcode, HRAWINPUT hRawInput);
	afx_msg void OnEnSetfocus();
	afx_msg void OnEnKillfocus();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	DECLARE_MESSAGE_MAP()

	void PushKey(USHORT vKeyCode);
	void PopKey( USHORT vKeyCode );
	void ShowKeys(void);
	bool IsExtendKey(USHORT vKeyCode);

	USHORT m_uKeyCode;
	bool m_bSingleKey;
	bool m_bHotKey;
	bool m_bRecord;
	USHORT m_uExtendKey;

public:
	static DWORD GetHotKeyByString(CString sHotKey);
	void SetHotKey(DWORD dwHotKey);
	DWORD GetHotKey(void);
};

