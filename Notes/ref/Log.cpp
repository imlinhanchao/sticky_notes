#include "StdAfx.h"
#include "Log.h"

DWORD CLogApp::m_dwLogType = (DWORD)LOG_DEBUG;
CString CLogApp::m_sPath = CLogApp::GetCurDirectory() + _T("error.log");
CListBox* CLogApp::m_pListBox = NULL;

CLogApp::CLogApp(void)
{
	m_dwLogType = (DWORD)LOG_DEBUG;
}

void CLogApp::Init( DWORD dwType, CString sPath/*=_T("")*/ )
{
	m_dwLogType = dwType;
	if(sPath.IsEmpty()) sPath = GetCurDirectory() + _T("error.log");
	m_sPath = sPath;
	DeleteFile(m_sPath);
}

CLogApp::~CLogApp(void)
{
}

CString CLogApp::GetCurDirectory()
{
	CString sModuleFile = _T("");
	GetModuleFileName(NULL, sModuleFile.GetBuffer(MAX_PATH), MAX_PATH);
	sModuleFile.ReleaseBuffer();
	if(sModuleFile != _T(""))
	{
		sModuleFile = sModuleFile.Left(sModuleFile.ReverseFind('\\') + 1);
	}

	return sModuleFile;
}

CString CLogApp::Write( const TCHAR* pszFormat, ... )
{
	ASSERT(pszFormat && *pszFormat);

	TCHAR  szMsg[1024];	
	va_list  vargs;

	va_start(vargs, pszFormat);

	_vsnwprintf_s( szMsg, sizeof(szMsg) - 1, pszFormat, vargs);

	CString sLog(szMsg);
	CString sTime;
	SYSTEMTIME st;
	memset(&st, 0, sizeof(SYSTEMTIME));
	GetLocalTime(&st);
	sTime.Format(_T("%04d-%02d-%02d %02d:%02d:%02d.%03d"),st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

	if (m_dwLogType & LOG_FILE) {
		CStdioFile Logfile;
		if(Logfile.Open(m_sPath, CFile::modeCreate | CFile::modeWrite | CFile::modeNoTruncate))
		{

			Logfile.SeekToEnd();
			Logfile.WriteString(sTime + _T(" >>> ") + sLog + _T("\n"));
			Logfile.Close();
		}

		va_end(vargs);
	}

	if (m_dwLogType & LOG_PRINT) {
		_tprintf(sTime + _T(" >>> ") + sLog + _T("\n"));
	}

	if (m_dwLogType & LOG_DEBUG) {
		OutputDebugString(sTime + _T(" >>> ") + sLog + _T("\n"));
	}

	if (m_dwLogType & LOG_LIST) {
		if (m_pListBox != NULL) m_pListBox->AddString(sTime + _T(" >>> ") + sLog + _T("\n"));
		if (m_pListBox != NULL) m_pListBox->SetTopIndex(m_pListBox->GetCount() - 1);
	}

	return sLog;
}

CString CLogApp::Debug( const TCHAR* pszFormat, ... )
{
	ASSERT(pszFormat && *pszFormat);

	TCHAR  szMsg[1024];	
	va_list  vargs;

	va_start(vargs, pszFormat);

	_vsnwprintf_s( szMsg, sizeof(szMsg) - 1, pszFormat, vargs);

	CString sLog(szMsg);
	CString sTime;
	SYSTEMTIME st;
	memset(&st, 0, sizeof(SYSTEMTIME));
	GetLocalTime(&st);
	sTime.Format(_T("%04d-%02d-%02d %02d:%02d:%02d.%03d"),st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

	OutputDebugString(sTime + _T(" >>> ") + sLog + _T("\n"));

	return sLog;
}

CString CLogApp::Print( const TCHAR* pszFormat, ... )
{
	ASSERT(pszFormat && *pszFormat);

	TCHAR  szMsg[1024];	
	va_list  vargs;

	va_start(vargs, pszFormat);

	_vsnwprintf_s( szMsg, sizeof(szMsg) - 1, pszFormat, vargs);

	CString sLog(szMsg);
	CString sTime;
	SYSTEMTIME st;
	memset(&st, 0, sizeof(SYSTEMTIME));
	GetLocalTime(&st);
	sTime.Format(_T("%04d-%02d-%02d %02d:%02d:%02d.%03d"),st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

	_tprintf(sTime + _T(" >>> ") + sLog + _T("\n"));

	return sLog;
}

CString CLogApp::WriteFile( const TCHAR* pszFormat, ... )
{
	ASSERT(pszFormat && *pszFormat);

	TCHAR  szMsg[1024];	
	va_list  vargs;

	va_start(vargs, pszFormat);

	_vsnwprintf_s( szMsg, sizeof(szMsg) - 1, pszFormat, vargs);

	CString sLog(szMsg);
	CString sTime;
	SYSTEMTIME st;
	memset(&st, 0, sizeof(SYSTEMTIME));
	GetLocalTime(&st);
	sTime.Format(_T("%04d-%02d-%02d %02d:%02d:%02d.%03d"),st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

	CStdioFile Logfile;
	if(Logfile.Open(m_sPath, CFile::modeCreate | CFile::modeWrite | CFile::modeNoTruncate))
	{

		Logfile.SeekToEnd();
		Logfile.WriteString(sTime + _T(" >>> ") + sLog + _T("\n"));
		Logfile.Close();
	}

	va_end(vargs);

	return sLog;
}

CString CLogApp::List( const TCHAR* pszFormat, ... )
{
	ASSERT(pszFormat && *pszFormat);

	TCHAR  szMsg[1024];	
	va_list  vargs;

	va_start(vargs, pszFormat);

	_vsnwprintf_s( szMsg, sizeof(szMsg) - 1, pszFormat, vargs);

	CString sLog(szMsg);
	CString sTime;
	SYSTEMTIME st;
	memset(&st, 0, sizeof(SYSTEMTIME));
	GetLocalTime(&st);
	sTime.Format(_T("%04d-%02d-%02d %02d:%02d:%02d.%03d"),st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

	if(m_pListBox != NULL) m_pListBox->AddString(sTime + _T(" >>> ") + sLog + _T("\n"));

	return sLog;
}
