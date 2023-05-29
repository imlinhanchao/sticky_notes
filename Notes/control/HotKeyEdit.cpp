#include "StdAfx.h"
#include "HotKeyEdit.h"


CHotKeyEdit::CHotKeyEdit(void)
{
	m_bHotKey = false;
	m_bSingleKey = false;
	m_bRecord = false;
	m_uExtendKey = 0;
}


CHotKeyEdit::~CHotKeyEdit(void)
{
}
BEGIN_MESSAGE_MAP(CHotKeyEdit, CEdit)
	ON_WM_INPUT()
	ON_CONTROL_REFLECT(EN_SETFOCUS, &CHotKeyEdit::OnEnSetfocus)
	ON_CONTROL_REFLECT(EN_KILLFOCUS, &CHotKeyEdit::OnEnKillfocus)
	ON_WM_CHAR()
	ON_WM_TIMER()
END_MESSAGE_MAP()


void CHotKeyEdit::OnRawInput(UINT nInputcode, HRAWINPUT hRawInput)
{
	UINT dwSize = 0;
	BYTE* lpb = NULL;
	RAWINPUT* raw = NULL;

	GetRawInputData(hRawInput, RID_INPUT, NULL, &dwSize, sizeof(RAWINPUTHEADER));
	if (dwSize == 0) goto DONE;

	lpb = new BYTE[dwSize];
	if (GetRawInputData(hRawInput, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER)) != dwSize)
		goto DONE;

	raw = (RAWINPUT*)lpb;
	if (raw->header.dwType == RIM_TYPEKEYBOARD) 
	{
		static RAWKEYBOARD keyLast = {0};
		RAWKEYBOARD keyboard = raw->data.keyboard;

		if(keyboard.Flags == keyLast.Flags 
			&& keyboard.VKey  == keyLast.VKey) goto DONE;

		if(keyboard.Flags & RI_KEY_BREAK && m_bRecord){
			if(!m_bHotKey || !IsExtendKey(keyboard.VKey)) m_bRecord = false;
			else PopKey(keyboard.VKey);
		}
		else if (keyboard.Flags == RI_KEY_MAKE || keyboard.Flags == RI_KEY_E0)
		{
			if (!m_bRecord || m_bSingleKey) {
				m_uKeyCode = 0;
				m_uExtendKey = 0;
			}
			PushKey(keyboard.VKey);
			m_bRecord = true;
		}

		keyLast = keyboard;
	}

DONE:
	if(lpb != NULL) delete lpb;

	CEdit::OnRawInput(nInputcode, hRawInput);
}


void CHotKeyEdit::OnEnSetfocus()
{
	CRawInput::Register(m_hWnd, RAW_TYPE_KB);
}


void CHotKeyEdit::OnEnKillfocus()
{
	CRawInput::Remove(m_hWnd, RAW_TYPE_KB);
}


void CHotKeyEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	// CEdit::OnChar(nChar, nRepCnt, nFlags);
}


BOOL CHotKeyEdit::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
	{
		return TRUE;
	}

	return CEdit::PreTranslateMessage(pMsg);
}

void CHotKeyEdit::PushKey( USHORT vKeyCode )
{
	if (m_bHotKey && IsExtendKey(vKeyCode))
	{
		switch (vKeyCode)
		{
		case VK_CONTROL: m_uExtendKey |= CONTROL; break;
		case VK_SHIFT: m_uExtendKey |= SHIFT; break;
		case VK_MENU: m_uExtendKey |= ALT; break;
		case VK_LWIN:
		case VK_RWIN: m_uExtendKey |= WIN; break;
		}
	}
	else
		m_uKeyCode = vKeyCode;
	ShowKeys();
}

void CHotKeyEdit::PopKey( USHORT vKeyCode )
{
	switch (vKeyCode)
	{
	case VK_CONTROL: m_uExtendKey &= ~CONTROL; break;
	case VK_SHIFT: m_uExtendKey &= ~SHIFT; break;
	case VK_MENU: m_uExtendKey &= ~ALT; break;
	case VK_LWIN:
	case VK_RWIN: m_uExtendKey &= ~WIN; break;
	}
	ShowKeys();
}

void CHotKeyEdit::ShowKeys( void )
{
	CString sValue;

	if (m_bHotKey)
	{
		if(m_uExtendKey & WIN) sValue += _T("Win + ");
		if(m_uExtendKey & CONTROL) sValue += _T("Ctrl + ");
		if(m_uExtendKey & ALT) sValue += _T("Alt + ");
		if(m_uExtendKey & SHIFT) sValue += _T("Shift + ");
	}

	sValue += CHotKey::GetHotKeyName(m_uKeyCode);
	sValue = sValue.Trim(_T(" + "));
	SetWindowText(sValue);
}

void CHotKeyEdit::SetSingleKey( bool bSingle, bool bHotKey/*=false*/ )
{
	m_bSingleKey = bSingle;
	m_bHotKey = bHotKey;
}

bool CHotKeyEdit::IsExtendKey( USHORT vKeyCode )
{
	return (VK_SHIFT == vKeyCode || 
			VK_CONTROL == vKeyCode || 
			VK_MENU == vKeyCode || 
			VK_LWIN == vKeyCode || 
			VK_RWIN == vKeyCode );
}

DWORD CHotKeyEdit::GetHotKeyByString(CString sHotKey)
{
	DWORD dwHotKey = 0;
	if (sHotKey.IsEmpty())
		return dwHotKey;
	
	vector<CString> lstKeys = Cvt::SplitString(sHotKey, _T("+"));
	if (lstKeys.size() == 0)
		return dwHotKey;

	for (int i = 0; i < lstKeys.size(); i++)
	{
		CString sKey = lstKeys.at(i);
		sKey.Trim();
		sKey.MakeUpper();

		if (sKey == _T("WIN"))
			dwHotKey |= WIN;
		else if (sKey == _T("CTRL"))
			dwHotKey |= CONTROL;
		else if (sKey == _T("ALT"))
			dwHotKey |= ALT;
		else if (sKey == _T("SHIFT"))
			dwHotKey |= SHIFT;
		else
		{
			USHORT uKeyCode = CHotKey::GetHotKeyCode(sKey);
			if (uKeyCode != 0)
				dwHotKey |= uKeyCode;
		}
	}



	return dwHotKey;
}

void CHotKeyEdit::SetHotKey(DWORD dwHotKey)
{
	m_uKeyCode = dwHotKey & 0xFF;
	m_uExtendKey = (dwHotKey >> 8) & 0xFF;
	m_bHotKey = true;
	ShowKeys();
}

DWORD CHotKeyEdit::GetHotKey(void)
{
	DWORD dwHotKey = 0;
	if (m_bHotKey)
	{
		dwHotKey = m_uExtendKey;
		dwHotKey <<= 8;
		dwHotKey |= m_uKeyCode;
	}
	return 0;
}


void CHotKeyEdit::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	CEdit::OnTimer(nIDEvent);
}

