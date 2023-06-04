#include "StdAfx.h"
#include "HotKey.h"

namespace Easy {

map<DWORD, HOTKEY_CALL> CHotKey::m_HotKeyCallback;

CHotKey::CHotKey(void)
{
}


CHotKey::~CHotKey(void)
{
}

bool CHotKey::SetWithCall( DWORD dwHotKey, HOTKEYCALLBACK pCallback, LPVOID lpParam, HWND hWnd)
{
	HOTKEY_CALL hotkeyCall = { pCallback, lpParam };
	m_HotKeyCallback[dwHotKey] = hotkeyCall;
	return SetHotKey(dwHotKey, hWnd);
}

bool CHotKey::RemoveHotKey( DWORD dwHotKey, HWND hWnd )
{
	return UnregisterHotKey(hWnd == NULL ? AfxGetMainWnd()->GetSafeHwnd() : hWnd, dwHotKey);
}

bool CHotKey::SetHotKey( DWORD dwHotKey, HWND hWnd )
{
	return RegisterHotKey(hWnd == NULL ? AfxGetMainWnd()->GetSafeHwnd() : hWnd, dwHotKey, GetModifiers(dwHotKey), dwHotKey & 0xff);
}

UINT CHotKey::GetModifiers( DWORD dwHotKey )
{
	UINT uModifiers = 0;
	dwHotKey >>= 0x08;
	if(dwHotKey & HOTKEYF_ALT)		uModifiers |= MOD_ALT;
	if(dwHotKey & HOTKEYF_SHIFT)	uModifiers |= MOD_SHIFT;
	if(dwHotKey & HOTKEYF_CONTROL)	uModifiers |= MOD_CONTROL;
	if(dwHotKey & HOTKEYF_EXT)		uModifiers |= MOD_WIN;
	return uModifiers;
}

CString CHotKey::GetHotKeyName( DWORD dwHotKey )
{
	DWORD dwKeyCode = dwHotKey & 0xff;
	CString sKey = CHotKeyCtrl::GetKeyName(dwKeyCode, IsExtendedKey(dwKeyCode));

	if (dwKeyCode == VK_PAUSE)
	{
		sKey = _T("Pause");
	}

	if (dwKeyCode == VK_LWIN || dwKeyCode == VK_RWIN)
	{
		sKey = _T("Win");
	}

	if(_T(" ") == sKey) 
	{
		sKey = _T("Space");
	}
	else if(sKey.IsEmpty())
	{
		sKey = CHotKeyCtrl::GetKeyName(dwKeyCode, TRUE);
	}

	dwHotKey >>= 0x08;
	if(dwHotKey & HOTKEYF_ALT)		sKey = _T("Alt + ") + sKey;
	if(dwHotKey & HOTKEYF_SHIFT)	sKey = _T("Shift + ") + sKey;
	if(dwHotKey & HOTKEYF_CONTROL)	sKey = _T("Ctrl + ") + sKey;

	return sKey;
}

USHORT CHotKey::GetHotKeyCode(CString sHotKey)
{
	USHORT uHotKey = 0;
	if (sHotKey.IsEmpty()) return uHotKey;

	return 0;
}

bool CHotKey::IsExtendedKey(DWORD vKey)
{
	DWORD dwKeyCodes[] = {
		VK_DELETE,
		VK_HOME,
		VK_PRIOR,
		VK_NEXT,
		VK_END,

		VK_NUMLOCK,
		VK_INSERT,

		VK_LEFT,
		VK_UP,
		VK_RIGHT,
		VK_DOWN,

		VK_LWIN,
		VK_RWIN,
	};

	int nCount = sizeof(dwKeyCodes) / sizeof(DWORD);
	for (int i = 0; i < nCount; i++)
	{
		if (vKey == dwKeyCodes[i]) return true;
	}
	return false;
}

void CHotKey::Execute( DWORD dwHotKey )
{
	if (m_HotKeyCallback.find(dwHotKey) != m_HotKeyCallback.end() && m_HotKeyCallback[dwHotKey].callback != NULL) 
		m_HotKeyCallback[dwHotKey].callback(m_HotKeyCallback[dwHotKey].lpParam); 
}

}