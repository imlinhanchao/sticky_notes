#include "StdAfx.h"
#include "Utility.h"
#pragma comment(lib, "version")

namespace Easy {
bool Utility::IsWow64()
{
	typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);
	LPFN_ISWOW64PROCESS fnIsWow64Process = NULL;

	BOOL bIsWow64 = FALSE;
	BOOL bSuccessful = FALSE;
	fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress ( GetModuleHandle(_T("kernel32")), "IsWow64Process");
	if (NULL != fnIsWow64Process &&
		fnIsWow64Process(GetCurrentProcess(), &bIsWow64))
	{
		bSuccessful = TRUE;
	}

	if(bSuccessful)
	{
		return bIsWow64 ? true : false; 
	}

	SYSTEM_INFO info = {0};
	typedef VOID (WINAPI *LPFN_GetNativeSystemInfo)(LPSYSTEM_INFO lpSystemInfo);
	LPFN_GetNativeSystemInfo fnGetNativeSystemInfo = (LPFN_GetNativeSystemInfo)GetProcAddress(GetModuleHandle(_T("kernel32")), "GetNativeSystemInfo");
	if(NULL == fnGetNativeSystemInfo)
	{
		GetSystemInfo(&info);
	}
	else
	{
		fnGetNativeSystemInfo(&info);
	}

	if(info.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64 ||
		info.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_IA64)
	{
		bIsWow64 = TRUE;
	}

	return bIsWow64 ? true : false; // we treat it as 32bit if failure.
}

MODULE_VER Utility::GetVersion(CString sModuleName)
{
	MODULE_VER ver;
	ver.wMajor = 0;
	ver.wMinor = 0;
	ver.wRevision = 0;
	ver.wBuild = 0;
	LPBYTE lpVersionData = NULL; 
	DWORD  dwLangCharset = 0; 

	// Get the version information size for allocate the buffer
	DWORD dwHandle = 0;     
	DWORD dwDataSize = ::GetFileVersionInfoSize((LPTSTR)sModuleName.GetBuffer(), &dwHandle); 
	sModuleName.ReleaseBuffer();
	if ( dwDataSize == 0 ) 
	{
		return ver;
	}

	// Allocate buffer and retrieve version information
	lpVersionData = new BYTE[dwDataSize + 1]; 
	if (!::GetFileVersionInfo((LPTSTR)sModuleName.GetBuffer(), dwHandle, dwDataSize, (void**)lpVersionData) )
	{
		sModuleName.ReleaseBuffer();
		delete[] lpVersionData;
		return ver;
	}
	sModuleName.ReleaseBuffer();

	// Retrieve the first language and character-set identifier
	UINT   nQuerySize = 0;
	DWORD* pTransTable = NULL;
	if (!::VerQueryValue(lpVersionData, _T("\\VarFileInfo\\Translation"), (void **)&pTransTable, &nQuerySize) )
	{
		delete[] lpVersionData;
		return ver;
	}

	// Swap the words to have lang-charset in the correct format
	dwLangCharset = MAKELONG(HIWORD(pTransTable[0]), LOWORD(pTransTable[0]));

	// Get fixed file information
	VS_FIXEDFILEINFO* pVsffi;
	if ( !::VerQueryValue((void **)lpVersionData, _T("\\"), (void**)&pVsffi, &nQuerySize) )
	{
		return ver;
	}

	ver.wMajor	= HIWORD(pVsffi->dwFileVersionMS);
	ver.wMinor	= LOWORD(pVsffi->dwFileVersionMS);
	ver.wRevision = HIWORD(pVsffi->dwFileVersionLS);
	ver.wBuild	= LOWORD(pVsffi->dwFileVersionLS);

	return ver;
}

void Utility::SetAutoRun(bool bAuto)
{
	#define AUTO_REG _T("Software\\Microsoft\\Windows\\CurrentVersion\\Run")
	Registry reg(HKEY_CURRENT_USER, true);

	if (bAuto) reg.Write(AUTO_REG, _T("Sticky Note"), Path::GetProgramPath());
	else reg.DeleteValue(AUTO_REG, _T("Sticky Note"));
}

HANDLE Utility::ProgramLock(CString sInstanceName)
{
	HANDLE hInstance = CreateMutex(NULL, TRUE, _T("GRAPHICS ENGINE_INSTANCE"));
	if (ERROR_ALREADY_EXISTS == GetLastError())
	{
		CloseHandle(hInstance);
		hInstance = nullptr;
		::PostQuitMessage(0);
	}
	return hInstance;
}

}