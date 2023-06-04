#pragma once

namespace Easy {

typedef void (CALLBACK* HOTKEYCALLBACK)(LPVOID lpParam);
typedef struct _HOTKEY_CALL 
{
	HOTKEYCALLBACK callback;
	LPVOID lpParam;
} HOTKEY_CALL, *PHOTKEY_CALL;

class CHotKey
{
public:
	CHotKey(void);
	~CHotKey(void);

	static bool SetWithCall(DWORD dwHotKey, HOTKEYCALLBACK pCallback, LPVOID lpParam, HWND hWnd = NULL);
	static bool RemoveHotKey(DWORD dwHotKey, HWND hWnd = NULL);
	static bool SetHotKey(DWORD dwHotKey, HWND hWnd = NULL);
	static CString GetHotKeyName(DWORD dwHotKey);
	static USHORT GetHotKeyCode(CString sHotKey);

	static void Execute(DWORD dwHotKey);

protected:
	static UINT GetModifiers(DWORD dwHotKey);
	static bool IsExtendedKey(DWORD vKey);

	static map<DWORD, HOTKEY_CALL> m_HotKeyCallback;
};

}