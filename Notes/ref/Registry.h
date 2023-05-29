#pragma once

namespace Easy {

class Registry
{
public:
	Registry();	
	Registry(HKEY hKey, bool bWOW6432 = false);	

	void Init(HKEY hKey, bool bWOW6432 = false);
	bool IsExisted(CString sSubKey);
	bool CreateKey(CString sSubKey);
	bool DeleteKey(CString sSubKey);

	bool DeleteValue(CString sSubKey, CString sValueName);

	bool Read(CString sSubKey, CString sValueName, int&    nValue);
	bool Read(CString sSubKey, CString sValueName, bool&   bValue);
	bool Read(CString sSubKey, CString sValueName, UINT&   nValue);
	bool Read(CString sSubKey, CString sValueName, long&   nValue);
	bool Read(CString sSubKey, CString sValueName, DWORD&  dwValue);
	bool Read(CString sSubKey, CString sValueName, CString& sValue);
	bool Read(CString sSubKey, CString sValueName, LPVOID lpData, DWORD& dwDataSize);

	bool Write(CString sSubKey, CString sValueName, int    nValue);
	bool Write(CString sSubKey, CString sValueName, bool   bValue);
	bool Write(CString sSubKey, CString sValueName, UINT   nValue);
	bool Write(CString sSubKey, CString sValueName, long   nValue);
	bool Write(CString sSubKey, CString sValueName, DWORD  dwValue);
	bool Write(CString sSubKey, CString sValueName, CString sValue);
	bool Write(CString sSubKey, CString sValueName, LPVOID lpData, DWORD dwDataSize, DWORD dwType);


	/*!
	 * @brief GetLastError
	 * 
	 * Get the last error code
	 * @return DWORD the error code
	 */
	DWORD GetLastError();

	/*!
	 * @brief GetLastErrorMsg 
	 * 
	 * Get the last error message
	 * @return CString the error message
	 */
	CString GetLastErrorMsg();

private:

	/*!
	 * @brief FormatLastError 
	 * 
	 * format the last error code to string
	 * @param dwLastError the error code want to format
	 * @return CString the error message
	 */
	CString FormatLastError(DWORD dwLastError);

	CString m_sLastError;
	DWORD m_nLastError;

	HKEY m_hKey;
	DWORD m_dwWOW6432;
};

}