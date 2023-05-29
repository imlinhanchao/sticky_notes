#pragma once

namespace Easy {

/*!
 * @class Ini
 * @brief Initialization File operation class
 * 
 * Used to get read/write *.ini File
 */
class Ini
{
public:
	Ini(void);
	Ini(CString sPath);
	~Ini(void);

	/*!
	 * @brief SetFile 
	 * 
	 * Set the file path to operation
	 * @param sPath The path of file
	 */
	void SetFile(CString sPath);

	/*!
	 * @brief GetAllSections 
	 * 
	 * Get all of the sections from file
	 * @param sSections The name of sections that you get.
	 * @return bool Get sections success or not
	 */
	bool GetAllSections(vector<CString>& sSections);

	/*!
	 * @brief GetAllKeys 
	 * 
	 * Get all of the keys from section of file
	 * @param sSection The section where you get keys.
	 * @param sKeys The keys that you get.
	 * @return bool Get keys success or not
	 */
	bool GetAllKeys(CString sSection, vector<CString>& sKeys);


	/*!
	 * @brief Read 
	 * 
	 * Read value of the key from section of file
	 * @param sSection The section where key is.
	 * @param sKeyName The key to read value.
	 * @param Value The value that you read.
	 * @return bool Read sections success or not
	 * @{
	 */
	bool Read(CString sSection,   CString sKeyName,   CString& sValue);
	bool Read(CString sSection,   CString sKeyName,   DWORD&   dwValue);
	bool Read(CString sSection,   CString sKeyName,   int&     nValue);
	bool Read(CString sSection,   CString sKeyName,   UINT&    nValue);
	bool Read(CString sSection,   CString sKeyName,   long&    nValue);
	bool Read(CString sSection,   CString sKeyName,   bool&    bValue);
	/* @} */


	/*!
	 * @brief Read 
	 * 
	 * Write value of the key from section of file
	 * @param sSection The section where key is.
	 * @param sKeyName The key to write value.
	 * @param Value The value that you write.
	 * @return bool Write Value success or not
	 * @{
	 */
	bool Write(CString sSection,   CString sKeyName,   CString sValue);
	bool Write(CString sSection,   CString sKeyName,   DWORD   dwValue);
	bool Write(CString sSection,   CString sKeyName,   int     nValue);
	bool Write(CString sSection,   CString sKeyName,   UINT    nValue);
	bool Write(CString sSection,   CString sKeyName,   long    nValue);
	bool Write(CString sSection,   CString sKeyName,   bool    bValue);
	/* @} */

	/*!
	 * @brief Remove 
	 * 
	 * Remove key of the section
	 * @param sSection The section where key is.
	 * @param sKeyName The key to remove.
	 * @return bool Remove key success or not
	 */
	bool Remove(CString sSection, CString sKeyName);

	/*!
	 * @brief Remove 
	 * 
	 * Remove the section and all of the content
	 * @param sSection The section to remove.
	 * @return bool Remove section success or not
	 */
	bool Remove(CString sSection);

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
	 * @brief GetSystemError 
	 * 
	 * used to get system error and save it
	 */
	void GetSystemError();

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

	CString m_sIniFile;
};

}