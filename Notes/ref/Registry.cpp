#include "stdafx.h"
#include "Registry.h"


namespace Easy
{

Registry::Registry()
{
	Init(HKEY_CURRENT_USER, false);
}

Registry::Registry(HKEY hKey, bool bWOW6432) 
{ 
	Init(hKey, bWOW6432);
}

void Registry::Init( HKEY hKey, bool bWOW6432 /*= false*/ )
{
	m_hKey = hKey; 
	m_dwWOW6432 = bWOW6432 ? KEY_WOW64_32KEY : KEY_WOW64_64KEY;
}

bool Registry::IsExisted(CString sSubKey)
{
	HKEY hKey = NULL; 
	sSubKey = sSubKey.TrimLeft(_T('\\'));
	DWORD dwErr = RegOpenKeyEx(m_hKey, sSubKey, 0, KEY_READ | m_dwWOW6432, &hKey);
	if(ERROR_SUCCESS != dwErr)
	{
		return false;
	}

	RegCloseKey(hKey);
	return true;
}

bool Registry::CreateKey(CString sSubKey)
{
	int nCurPos = 0;
	sSubKey = sSubKey.TrimLeft(_T('\\'));
	CString sChildKey = sSubKey.Tokenize(_T("\\"), nCurPos);
	while (_T("") != sChildKey)
	{
		TCHAR szKey[260];
		_tcscpy_s(szKey, 260, sSubKey.Mid(0, nCurPos - 1));
		m_nLastError = ERROR_SUCCESS;
		if(!IsExisted(szKey))
		{
			HKEY hKey = NULL;
			m_nLastError = RegCreateKey(m_hKey, szKey, &hKey);
			RegCloseKey(hKey);
		}

		if(ERROR_SUCCESS != m_nLastError) 
		{
			m_sLastError = FormatLastError(m_nLastError); return false;
		}

		sChildKey = sSubKey.Tokenize(_T("\\"), nCurPos);
	}   

	return true;
}

bool Registry::DeleteKey(CString sSubKey)   
{   
	sSubKey = sSubKey.TrimLeft(_T('\\'));
	m_nLastError = RegDeleteKey(m_hKey, sSubKey);
	if(ERROR_SUCCESS != m_nLastError)
	{
		m_sLastError = FormatLastError(m_nLastError); return false;
	}

	return true;
}  

bool Registry::DeleteValue(CString sSubKey, CString sValueName)
{
	HKEY hKey = NULL; 
	sSubKey = sSubKey.TrimLeft(_T('\\'));
	m_nLastError = RegOpenKeyEx(m_hKey, sSubKey, 0, KEY_SET_VALUE | m_dwWOW6432, &hKey);
	if(ERROR_SUCCESS != m_nLastError)
	{
		m_sLastError = FormatLastError(m_nLastError); return false;
	}

	m_nLastError = RegDeleteValue(hKey, sValueName);
	RegCloseKey(hKey);

	if(ERROR_SUCCESS != m_nLastError)
	{
		m_sLastError = FormatLastError(m_nLastError); return false;
	}

	return true;
}

bool Registry::Read(CString sSubKey, CString sValueName, int& nValue)
{
	DWORD dwValue = 0;
	if(!Read(sSubKey, sValueName, dwValue)) return false;
	nValue = (int)dwValue;
	return true;
}

bool Registry::Read(CString sSubKey, CString sValueName, bool& bValue)
{
	DWORD dwValue = 0;
	if(!Read(sSubKey, sValueName, dwValue)) return false;
	bValue = (0 != dwValue) ? true : false;
	return true;
}

bool Registry::Read(CString sSubKey, CString sValueName, UINT& nValue)
{
	DWORD dwValue = 0;
	if(!Read(sSubKey, sValueName, dwValue)) return false;
	nValue = (UINT)dwValue;
	return true;
}

bool Registry::Read(CString sSubKey, CString sValueName, long& nValue)
{
	DWORD dwValue = 0;
	if(!Read(sSubKey, sValueName, dwValue)) return false;
	nValue = (long)dwValue;
	return true;
}

bool Registry::Read(CString sSubKey, CString sValueName, DWORD& dwValue)
{
	DWORD dwSize = sizeof(DWORD);
	return Read(sSubKey, sValueName, &dwValue, dwSize);
}

bool Registry::Read(CString sSubKey, CString sValueName, CString& sValue)
{
	DWORD dwSize = 2048;
	TCHAR szValue[2048] = {0};
	if(!Read(sSubKey, sValueName, szValue, dwSize)) return false;
	sValue.Format(_T("%s"), szValue);
	return true;
}

bool Registry::Read(CString sSubKey, CString sValueName, LPVOID lpData, DWORD& dwDataSize)
{
	HKEY hKey = NULL;
	sSubKey = sSubKey.TrimLeft(_T('\\'));
	m_nLastError = RegOpenKeyEx(m_hKey, sSubKey, 0, KEY_READ | m_dwWOW6432, &hKey);
	if(ERROR_SUCCESS != m_nLastError)
	{
		m_sLastError = FormatLastError(m_nLastError); return false;
	}

	m_nLastError = RegQueryValueEx(hKey, sValueName, NULL, NULL, (LPBYTE)lpData, &dwDataSize);
	RegCloseKey(hKey);
	if(ERROR_SUCCESS != m_nLastError)
	{
		m_sLastError = FormatLastError(m_nLastError); return false;
	}

	return true;
}

bool Registry::Write(CString sSubKey, CString sValueName, int nValue)
{
	DWORD dwValue = (DWORD)nValue;
	return Write(sSubKey, sValueName, dwValue);
}

bool Registry::Write(CString sSubKey, CString sValueName, bool bValue)
{
	DWORD dwValue = (DWORD)bValue;
	return Write(sSubKey, sValueName, dwValue);
}

bool Registry::Write(CString sSubKey, CString sValueName, UINT nValue)
{
	DWORD dwValue = (DWORD)nValue;
	return Write(sSubKey, sValueName, dwValue);
}

bool Registry::Write(CString sSubKey, CString sValueName, long nValue)
{
	DWORD dwValue = (DWORD)nValue;
	return Write(sSubKey, sValueName, dwValue);
}

bool Registry::Write(CString sSubKey, CString sValueName, DWORD dwValue)
{
	return Write(sSubKey, sValueName, &dwValue, sizeof(DWORD), REG_DWORD); 
}

bool Registry::Write(CString sSubKey, CString sValueName, CString sValue)
{
	DWORD dwLen = sValue.GetLength();
	bool bSuccessful = Write(sSubKey, sValueName, sValue.GetBuffer(dwLen), dwLen * sizeof(TCHAR), REG_SZ); 
	sValue.ReleaseBuffer();
	return bSuccessful;
}

bool Registry::Write(CString sSubKey, CString sValueName, LPVOID lpData, DWORD dwDataSize, DWORD dwType)
{
	if(!IsExisted(sSubKey))
	{
		CreateKey(sSubKey);
	}

	HKEY hKey = NULL;
	sSubKey = sSubKey.TrimLeft(_T('\\'));
	m_nLastError = RegOpenKeyEx(m_hKey, sSubKey, 0, KEY_WRITE | m_dwWOW6432, &hKey);
	if(ERROR_SUCCESS != m_nLastError)
	{
		m_sLastError = FormatLastError(m_nLastError); return false;
	}

	m_nLastError = RegSetValueEx(hKey, sValueName, NULL, dwType, (LPBYTE)lpData, dwDataSize);
	RegCloseKey(hKey);
	if(ERROR_SUCCESS != m_nLastError)
	{
		m_sLastError = FormatLastError(m_nLastError); return false;
	}

	return true;
}

CString Registry::GetLastErrorMsg()
{
	return  m_sLastError;
}

DWORD Registry::GetLastError()
{
	return  m_nLastError;
}

CString Registry::FormatLastError( DWORD dwLastError )
{
	LPVOID lpMsgBuf;
	CString sErrMsg = _T("");

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dwLastError,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR) &lpMsgBuf,
		0, NULL);

	sErrMsg.Format(_T("%s"), lpMsgBuf);
	LocalFree(lpMsgBuf);

	return sErrMsg;
}

}