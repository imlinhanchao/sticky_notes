#include "stdafx.h"
#include "XFile.h"

namespace Easy {

bool XFile::ReadFile(CString sFile, CString& data)
{
	CFile file;

	if (!file.Open(sFile, CFile::modeRead))
	{
		return false;
	}

	data = _T("");

	int nLen = file.GetLength() + 1;
	char* utf8Buf = new char[nLen];
	ZeroMemory(utf8Buf, nLen);

	file.Read(utf8Buf, nLen - 1);

	MultiByteToWideChar(CP_UTF8, 0, utf8Buf, -1, data.GetBuffer(nLen), nLen);
	data.ReleaseBuffer();
	
	file.Close();
	return true;
}

bool XFile::WriteFile(CString sFile, CString data)
{
	CFile file;

	if (!file.Open(sFile, CFile::modeCreate | CFile::modeWrite))
	{
		return false;
	}

	int nLen = WideCharToMultiByte(CP_UTF8, 0, data, -1, NULL, 0, NULL, NULL);
	char* utf8Buf = new char[nLen];
	WideCharToMultiByte(CP_UTF8, 0, data, -1, utf8Buf, nLen, NULL, NULL);

	file.Write(utf8Buf, (nLen - 1) * sizeof(char));
	data.ReleaseBuffer();

	file.Close();
	return true;
}

bool XFile::AppendFile(CString sFile, CString data)
{
	CStdioFile file;

	if (!file.Open(sFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite))
	{
		return false;
	}

	file.SeekToEnd();
	file.Write(data.GetBuffer(), data.GetLength() * sizeof(TCHAR));
	data.ReleaseBuffer();

	file.Close();
	return true;
}

}