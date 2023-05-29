#pragma once

typedef enum { 
	LOG_FILE = 1 << 0, 
	LOG_PRINT = 1 << 1, 
	LOG_DEBUG = 1 << 2, 
	LOG_LIST = 1 << 3, 
	LOG_ALL = 0xffffff
}LOG_TYPE;


class CLogApp
{
public:
	CLogApp(void);
	~CLogApp(void);

	static void SetList(CListBox* pList) { m_pListBox = pList; }
	static void Init(DWORD dwType, CString sPath=_T(""));
	static CString Write(const TCHAR* pszFormat, ...);
	static CString Debug(const TCHAR* pszFormat, ...);
	static CString Print(const TCHAR* pszFormat, ...);
	static CString List(const TCHAR* pszFormat, ...);
	static CString WriteFile(const TCHAR* pszFormat, ...);
private:
	static CString GetCurDirectory();

	static DWORD m_dwLogType;
	static CString m_sPath;
	static CListBox* m_pListBox;
};
