#include "stdafx.h"
#include "Shell.h"

namespace Easy {

Shell::Shell(void)
{
	m_bRunning = false;
	m_hRead = INVALID_HANDLE_VALUE;
	m_hWnd = NULL;
	ZeroMemory(&m_pi, sizeof(PROCESS_INFORMATION));
}


Shell::~Shell(void)
{
}

bool Shell::Execute( CString sPath, CString sCommand/*=_T("")*/, SHELL_TYPE type/*=APP*/, bool bShow/*=true*/, CString sWorkDirectory/*=Path::GetCurDirectory()*/ )
{
	switch (type)
	{
	case CONSOLE: return ExecuteConsole(sPath, sCommand, bShow, sWorkDirectory);
	case APP: return ExecuteApplication(sPath, sCommand, bShow, sWorkDirectory);
	}
	return true;
}

bool Shell::IsRunning( void )
{
	if (NULL != m_pi.hProcess && 
		WAIT_TIMEOUT == WaitForSingleObject(m_pi.hProcess, 0))
	{
		return true;
	}

	return m_bRunning;
}

bool Shell::Stop( void )
{
	if (!IsRunning()) return true;

	bool bSuccess = TerminateProcess(m_pi.hProcess, 0);
	if (!bSuccess) GetSystemError();

	return bSuccess;
}

CString Shell::GetOutput( void )
{
	return m_sOutput;
}

HWND Shell::GetWnd( void )
{
	if (m_hWnd == NULL) EnumWindows(EnumWindowsProc, (LPARAM)this);
	return m_hWnd;
}

DWORD Shell::ShellThread( LPVOID lpParam )
{
	Shell* pThis = (Shell*)lpParam;

	const DWORD dwSize = 40960;
	char szBuffer[dwSize] = {0};
	DWORD dwRead = 0;
	int nTry = 5;

	while(EnumWindows(EnumWindowsProc, (LPARAM)pThis) && nTry--) Sleep(100);

	if (pThis->m_hWnd == NULL) pThis->GetSystemError();

	pThis->m_bRunning = true;
	pThis->m_sOutput = _T("");

	USES_CONVERSION;
	while (true) 
	{
		if (ReadFile(pThis->m_hRead, szBuffer, dwSize, &dwRead, NULL) == NULL)
			break;
		pThis->m_sOutput += A2W(szBuffer);
		memset(szBuffer, 0, sizeof(szBuffer));
	}
	pThis->m_sOutput += A2W(szBuffer);
	pThis->m_bRunning = false;

	if(pThis->m_hRead != INVALID_HANDLE_VALUE) CloseHandle(pThis->m_hRead);
	if(pThis->m_pi.hThread != INVALID_HANDLE_VALUE) CloseHandle (pThis->m_pi.hThread); 
	if(pThis->m_pi.hProcess != INVALID_HANDLE_VALUE) CloseHandle (pThis->m_pi.hProcess);

	return 0;
}

BOOL Shell::EnumWindowsProc(HWND hWnd, LPARAM lParam)
{
	Shell* pThis = (Shell*)lParam;

	DWORD dwProcessId = 0;
	GetWindowThreadProcessId(hWnd, &dwProcessId);
	if(dwProcessId == pThis->m_pi.dwProcessId && CWnd::FromHandle(hWnd)->IsWindowVisible())
	{
		HWND h = GetParent(hWnd);
		while(GetParent(h)!=NULL)
		{
			h = GetParent(h);
			return FALSE;
		}
		pThis->m_hWnd = hWnd;
		return FALSE;
	}
	return TRUE;
}

bool Shell::ExecuteConsole( CString sPath, CString sCommand, bool bShow, CString sWorkDirectory )
{
	if(_T("") == sPath)
	{
		SetLastError(ERROR_INVALID_PARAMETER);
		return false;
	}

	m_bRunning = false;
	m_hRead = INVALID_HANDLE_VALUE;
	m_hWnd = NULL;
	ZeroMemory(&m_pi, sizeof(PROCESS_INFORMATION));

	SECURITY_ATTRIBUTES sa;
	HANDLE hWrite;

	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;
	if (!CreatePipe(&m_hRead, &hWrite, &sa, 0)) 
	{
		GetSystemError();
		return FALSE;
	}

	if(sWorkDirectory.IsEmpty()) sWorkDirectory = Path::GetDirectory(sPath);

	STARTUPINFO si;
	si.cb = sizeof(STARTUPINFO);
	GetStartupInfo(&si); 
	si.hStdError = hWrite;
	si.hStdOutput = hWrite;
	si.wShowWindow = bShow ? SW_SHOW : SW_HIDE;
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;

	sCommand = _T(" ") + sCommand;

	if (!CreateProcess(sPath.GetBuffer(), sCommand.GetBuffer(), NULL, NULL, TRUE, NULL, NULL, sWorkDirectory.IsEmpty() ? NULL : sWorkDirectory.GetBuffer(), &si, &m_pi)) 
	{
		sPath.ReleaseBuffer();
		sCommand.ReleaseBuffer();
		sWorkDirectory.ReleaseBuffer();
		GetSystemError();
		CloseHandle(hWrite);
		CloseHandle(m_hRead);
		return false;
	}
	sPath.ReleaseBuffer();
	sCommand.ReleaseBuffer();
	sWorkDirectory.ReleaseBuffer();
	CloseHandle(hWrite);

	if(m_pi.dwProcessId != 0) m_bRunning = true;

	HANDLE hThread = CreateThread(NULL, 0, ShellThread, this, 0, NULL);

	if (hThread != NULL) 
	{
		CloseHandle(hThread);
	}

	return true;
}

bool Shell::ExecuteApplication(CString sPath, CString sCommand, bool bShow, CString sWorkDirectory)
{
	if(_T("") == sPath)
	{
		SetLastError(ERROR_INVALID_PARAMETER);
		return false;
	}

	m_sOutput = _T("");
	m_bRunning = false;
	m_hRead = INVALID_HANDLE_VALUE;
	m_hWnd = NULL;
	ZeroMemory(&m_pi, sizeof(PROCESS_INFORMATION));
	
	if(sWorkDirectory.IsEmpty()) sWorkDirectory = Path::GetDirectory(sPath);

	SHELLEXECUTEINFO stInfo;
	memset(&stInfo, 0, sizeof(SHELLEXECUTEINFO));

	stInfo.cbSize       = sizeof(SHELLEXECUTEINFO);
	stInfo.fMask        = SEE_MASK_NOCLOSEPROCESS;
	stInfo.hwnd         = NULL;
	stInfo.lpVerb       = _T("open");
	stInfo.lpFile       = sPath.GetBuffer();
	stInfo.lpDirectory  = sWorkDirectory.GetBuffer();
	stInfo.lpParameters = sCommand.GetBuffer();
	stInfo.nShow        = bShow ? SW_SHOW : SW_HIDE;
	stInfo.hInstApp     = NULL;

	BOOL bExecute = ShellExecuteEx(&stInfo);

	sPath.ReleaseBuffer();
	sCommand.ReleaseBuffer();
	sWorkDirectory.ReleaseBuffer();

	if(!bExecute) 
	{
		GetSystemError(); 
		return false;
	}
	
	m_pi.hProcess = stInfo.hProcess;
	m_pi.dwProcessId = GetProcessId(stInfo.hProcess);
	return true;
}

CString Shell::GetLastErrorMsg()
{
	return  m_sLastError;
}

DWORD Shell::GetLastError()
{
	return  m_nLastError;
}

CString Shell::FormatLastError( DWORD dwLastError )
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

void Shell::GetSystemError()
{
	LPVOID lpMsgBuf;
	m_nLastError = ::GetLastError(); 

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		m_nLastError,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR) &lpMsgBuf,
		0, NULL);

	m_sLastError.Format(_T("Error Code: %d.\r\nError Message: %s"), m_nLastError, lpMsgBuf);
	LocalFree(lpMsgBuf);
}

}