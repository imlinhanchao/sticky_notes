#pragma once

namespace Easy
{

typedef struct _MODULE_VER { // Module version
	WORD wMajor;
	WORD wMinor;
	WORD wRevision;
	WORD wBuild;  

	CString ToString()
	{
		return Cvt::ToString(_T("%d.%d.%d"), wMajor, wMinor, wRevision);
	}
}MODULE_VER;

class Utility
{
public:
	static bool Utility::IsWow64();
	static MODULE_VER Utility::GetVersion(CString sModuleName);
};

}
