#include "stdafx.h"
#include "Path.h"
#include <Shlwapi.h>
#include "imagehlp.h"
#pragma comment(lib, "imagehlp.lib")

namespace Easy {

CString Path::GetFileName( CString sPath )
{
	CString sFilename(PathFindFileName(sPath.GetBuffer()));
	sPath.ReleaseBuffer();
	return sFilename;
}

CString Path::GetDirectory( CString sPath )
{
	PathRemoveFileSpec(sPath.GetBuffer());
	sPath.ReleaseBuffer();
	return sPath + _T("\\");
}

CString Path::GetExtName( CString sPath )
{
	CString sFilename(PathFindExtension(sPath.GetBuffer()));
	sPath.ReleaseBuffer();
	return sFilename;
}

CString Path::Resolve( CString sPath, CString sPathNext )
{
	PathAddBackslash(sPath.GetBuffer(sPath.GetLength() + 1));
	sPath.ReleaseBuffer();
	CString sPathFull = sPath + sPathNext;
	CString sPathFinal;

	PathCanonicalize(sPathFinal.GetBuffer(sPathFull.GetLength() + 100), sPathFull.GetBuffer());
	sPathFinal.ReleaseBuffer();
	sPathFull.ReleaseBuffer();

	if (IsDirectory(sPathFinal)) PathAddBackslash(sPathFinal.GetBuffer(sPath.GetLength() + 1));
	sPathFinal.ReleaseBuffer();

	return sPathFinal;
}

CString Path::Join( const TCHAR* szPath, ... )
{
	va_list vargs;
	int argno = 0;
	const TCHAR* next;
	va_start( vargs, szPath );
	CString sPath(szPath);

	while ((next = va_arg( vargs, const TCHAR*)) != NULL)
	{
		sPath = Resolve(sPath, CString(next));
	}

	va_end( vargs );

	return sPath;
}

CString Path::Omit( CString sPath, int nSize )
{
	CString sOmitPath;
	PathCompactPathEx(sOmitPath.GetBuffer(sPath.GetLength() + 100), sPath.GetBuffer(), nSize, 0);
	sOmitPath.ReleaseBuffer();
	sPath.ReleaseBuffer();

	return sOmitPath;
}

CString Path::GetCurDirectory( CString sPath/*=_T("")*/ )
{
	return Resolve(GetDirectory(GetProgramPath()), sPath);
}

CString Path::GetTmpDirectory( CString sPath/*=_T("")*/ )
{
	DWORD dwLen = 1024;
	TCHAR szPath[1024] = {0};
	if(0 != GetTempPath(dwLen, szPath)) 
	{
		return Resolve(CString(szPath), sPath);
	}
	return _T("");
}

CString Path::GetUserDirectory( CString sPath/*=_T("")*/ )
{
	return Resolve(GetDesktopDirectory()  + _T(".."), sPath);
}

CString Path::GetAppDataDirectory( CString sPath/*=_T("")*/ )
{
	TCHAR szPath[MAX_PATH];
	SHGetSpecialFolderPath(NULL, szPath, CSIDL_LOCAL_APPDATA, FALSE);
	return Resolve(CString(szPath), sPath);
}

CString Path::GetDesktopDirectory( CString sPath/*=_T("")*/ )
{
	TCHAR szPath[MAX_PATH];
	SHGetSpecialFolderPath(NULL, szPath, CSIDL_DESKTOPDIRECTORY, FALSE);
	return Resolve(CString(szPath), sPath);
}

CString Path::GetStartupDirectory( CString sPath/*=_T("")*/ )
{
	TCHAR szPath[MAX_PATH];
	SHGetSpecialFolderPath(NULL, szPath, CSIDL_STARTUP, FALSE);
	return Resolve(CString(szPath), sPath);
}

CString Path::GetStartMenuDirectory( CString sPath/*=_T("")*/ )
{
	TCHAR szPath[MAX_PATH];
	SHGetSpecialFolderPath(NULL, szPath, CSIDL_STARTMENU, FALSE);
	return Resolve(CString(szPath), sPath);
}

CString Path::GetProgramPath(void)
{
	CString sPath = _T("");
	GetModuleFileName(NULL, sPath.GetBuffer(MAX_PATH), MAX_PATH);
	sPath.ReleaseBuffer();
	return sPath;
}

CString Path::GetProgramName(void)
{
	return GetFileName(GetProgramPath());
}

bool Path::IsDirectory( CString sPath )
{
	bool bDirectory = PathIsDirectory(sPath.GetBuffer());
	sPath.ReleaseBuffer();
	return bDirectory;
}

bool Path::IsEmpty( CString sPath )
{
	bool bEmpty = PathIsDirectoryEmpty(sPath.GetBuffer());
	sPath.ReleaseBuffer();
	return bEmpty;
}

bool Path::Exists( CString sPath )
{
	bool bExists = PathFileExists(sPath.GetBuffer());
	sPath.ReleaseBuffer();
	return bExists;
}

void Path::Open( CString sPath )
{
	if (Path::IsDirectory(sPath))
		ShellExecute(NULL, _T("open"), _T("explorer.exe"), sPath, NULL, SW_SHOWNORMAL);
	else
		ShellExecute(NULL, _T("open"), _T("explorer.exe"), _T("/select,") + sPath, NULL, SW_SHOWNORMAL);
}
	
CString Path::Browse( LPCTSTR lpszFilter, LPCTSTR lpszDefExt, BOOL bOpen, LPCTSTR lpszFileName, LPCTSTR lpInitiaDir )
{
	CString sPathName = _T("");
	CFileDialog dlg(bOpen, 
		lpszDefExt,
		lpszFileName, 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_CREATEPROMPT | (bOpen ? OFN_FILEMUSTEXIST : 0),
		lpszFilter, 
		NULL);
	if (lpInitiaDir != NULL) dlg.m_ofn.lpstrInitialDir = lpInitiaDir;
	if(dlg.DoModal() == IDOK)
	{
		sPathName = dlg.GetPathName();
	}
	return sPathName;
}

static int CALLBACK BrowseCallbackProc(HWND hWnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	switch(uMsg)
	{
	case BFFM_INITIALIZED:
		::SendMessage(hWnd, BFFM_SETSELECTION, TRUE, (LPARAM)(LPTSTR)(LPCTSTR)lpData);
		break;
	case BFFM_SELCHANGED:
		{
			TCHAR szCurrent[MAX_PATH];   
			SHGetPathFromIDList((LPCITEMIDLIST)lParam, szCurrent);   
			::SendMessage(hWnd, BFFM_SETSTATUSTEXT, 0, (LPARAM)szCurrent);   
		}
		break;
	default:
		break;
	}
	return 0;   
}

CString Path::Folder( HWND hWnd, CString sRootPath/*=_T("")*/ )
{
	ASSERT(hWnd);

	CString sPath = _T("");
	LPITEMIDLIST pIdList = NULL; 
	if (sRootPath.IsEmpty()) sRootPath = GetDesktopDirectory();

	TCHAR szBuffer[MAX_PATH];  
	ZeroMemory(szBuffer, MAX_PATH);  
	_tcscpy(szBuffer, sRootPath.GetBuffer());
	sRootPath.ReleaseBuffer();

	BROWSEINFO bi; 
	bi.hwndOwner      = hWnd;  
	bi.pidlRoot       = NULL;  
	bi.pszDisplayName = NULL;  
	bi.lpszTitle      = NULL;  
	bi.ulFlags        = BIF_EDITBOX | BIF_NEWDIALOGSTYLE | BIF_BROWSEFORCOMPUTER;  
	bi.lpfn           = BrowseCallbackProc;  
	bi.lParam         = (LPARAM)szBuffer;  
	bi.iImage         = 0;

	if(NULL != (pIdList = SHBrowseForFolder(&bi)))  
	{  
		SHGetPathFromIDList(pIdList, sPath.GetBuffer(MAX_PATH));
		sPath.ReleaseBuffer();
	}

	return sPath;
}

int Path::CopyTo( vector<CString> lstSrc, CString sDst )
{
	CString sSrc;
	for (vector<CString>::iterator it = lstSrc.begin(); it != lstSrc.end(); it++)
	{
		sSrc += it->Trim('\\') + _T("|");
	}
	sDst.Trim('\\');

	int nSrcSize = sSrc.GetLength() + 2, nDstSize = sDst.GetLength() + 2;
	TCHAR* pFrom = new TCHAR[nSrcSize];
	TCHAR* pTo = new TCHAR[nDstSize];

	ZeroMemory(pFrom, sizeof(TCHAR) * (nSrcSize));
	ZeroMemory(pTo, sizeof(TCHAR) * (nDstSize));
	_tcscpy(pFrom, sSrc.GetBuffer());
	_tcscpy(pTo, sDst.GetBuffer());
	sSrc.ReleaseBuffer();
	sDst.ReleaseBuffer();

	for (int i = 0; i < nSrcSize; i++)
	{
		if(pFrom[i] == '|') pFrom[i] = 0;
	}

	SHFILEOPSTRUCT fop;
	ZeroMemory(&fop, sizeof(SHFILEOPSTRUCT));
	fop.wFunc = FO_COPY;
	fop.pFrom = pFrom;
	fop.pTo = pTo;
	int nRet = SHFileOperation(&fop);
	delete pFrom;
	delete pTo;
	return nRet;
}

vector<CString> Path::Traversing( CString sDirectory, FILITER filter/*=NULL*/ )
{
	vector<CString> lstPath;

	PathAddBackslash(sDirectory.GetBuffer(sDirectory.GetLength() + 1));
	sDirectory.ReleaseBuffer();

	if(!sDirectory.IsEmpty() && Exists(sDirectory) && !IsEmpty(sDirectory))
	{
		CFileFind ff;     
		BOOL bFound  = ff.FindFile(sDirectory + _T("*"), 0);    
		while(bFound)     
		{     
			bFound = ff.FindNextFile();  
			if(ff.IsDots())
			{
				continue;
			}

			SetFileAttributes(ff.GetFilePath(), FILE_ATTRIBUTE_NORMAL);
			CString sPath = ff.GetFilePath();
			if(ff.IsDirectory())       
			{      
				if (filter != NULL && !filter(sPath)) continue;
				vector<CString> lstPathEx = Traversing(sPath, filter);
				lstPath.insert(lstPath.begin(), lstPathEx.begin(), lstPathEx.end());
			}     
			else  
			{    
				if (filter != NULL && !filter(sPath)) continue;
				lstPath.push_back(sPath);
			}     
		}     
		ff.Close();
		return lstPath;
	}
	return lstPath;
}

vector<CString> Path::GetFileList( CString sDirectory, CString sFormat/*=_T("*")*/ )
{
	vector<CString> lstPath;

	PathAddBackslash(sDirectory.GetBuffer(sDirectory.GetLength() + 1));
	sDirectory.ReleaseBuffer();

	if(!sDirectory.IsEmpty() && Exists(sDirectory) && !IsEmpty(sDirectory))
	{
		CFileFind ff;     
		BOOL bFound  = ff.FindFile(sDirectory + sFormat, 0);    
		while(bFound)     
		{     
			bFound = ff.FindNextFile();  
			if(ff.IsDots())
			{
				continue;
			}

			SetFileAttributes(ff.GetFilePath(), FILE_ATTRIBUTE_NORMAL);
			CString sPath = ff.GetFilePath();
			lstPath.push_back(sPath);
		}     
		ff.Close();
		return lstPath;
	}
	return lstPath;
}

bool Path::Create( CString sPath )
{
	PathAddBackslash(sPath.GetBuffer(sPath.GetLength() + 1));
	sPath.ReleaseBuffer();

	USES_CONVERSION;	
	return MakeSureDirectoryPathExists(T2A(sPath));
}

}