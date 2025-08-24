#pragma once

namespace Easy {

class XFile
{
public:
	static bool ReadFile(CString sFile, CString& data);
	static bool WriteFile(CString sFile, CString data);
	static bool AppendFile(CString sFile, CString data);
};

}

