#pragma once


namespace Easy {

typedef bool (WINAPI * FILITER)(CString);

/*!
 * @class Path
 * @brief Path Tools
 * 
 * Used to get different path
 */
class Path
{
public:

	/*!
	 * @brief GetFileName 
	 * 
	 * Get filename from path
	 * @param sPath The path.
	 * @return CString The filename
	 */
	static CString GetFileName(CString sPath);

	/*!
	 * @brief GetDirectory 
	 * 
	 * Get directory from path
	 * @param sPath The path.
	 * @return CString The directory
	 */
	static CString GetDirectory(CString sPath);
		
	/*!
	 * @brief GetExtName 
	 * 
	 * Get extension from path
	 * @param sPath The path.
	 * @return CString The extension
	 */
	static CString GetExtName(CString sPath);


	/*!
	 * @brief Resolve 
	 * 
	 * Stitching path
	 * @param sPath The path of one.
	 * @param sPathNext The path of next. You can use `..` to indicate the parent directory
	 * @return CString The final path
	 */
	static CString Resolve(CString sPath, CString sPathNext);


	/*!
	 * @brief Join 
	 * 
	 * Stitching path
	 * @param sPath The path of one.
	 * @param ... The paths of next. You can use `..` to indicate the parent directory. pls end by NULL.
	 * @return CString The final path
	 */
	static CString Join(const TCHAR* szPath, ...);

	/*!
	 * @brief Omit 
	 * 
	 * Compressed path
	 * @param sPath The path.
	 * @param nSize The path length you want to compressed.
	 * @return CString The final path, like as `C:\...\sample.txt`
	 */
	static CString Omit(CString sPath, int nSize);


	/*!
	 * @brief GetCurDirectory 
	 * 
	 * The Directory of Program
	 * @param sPath The path you want to expand.
	 * @return CString The final path
	 */
	static CString GetCurDirectory(CString sPath=_T(""));

	/*!
	 * @brief GetTmpDirectory 
	 * 
	 * System temporary directory
	 * @param sPath The path you want to expand.
	 * @return CString The final path
	 */
	static CString GetTmpDirectory(CString sPath=_T(""));

	/*!
	 * @brief GetUserDirectory 
	 * 
	 * The user root directory
	 * @param sPath The path you want to expand.
	 * @return CString The final path
	 */
	static CString GetUserDirectory(CString sPath=_T(""));

	/*!
	 * @brief GetAppDataDirectory 
	 * 
	 * The user application data directory
	 * @param sPath The path you want to expand.
	 * @return CString The final path
	 */
	static CString GetAppDataDirectory(CString sPath=_T(""));

	/*!
	 * @brief GetDesktopDirectory 
	 * 
	 * The user desktop path
	 * @param sPath The path you want to expand.
	 * @return CString The final path
	 */
	static CString GetDesktopDirectory(CString sPath=_T(""));

	/*!
	 * @brief GetStartupDirectory 
	 * 
	 * The user startup path
	 * @param sPath The path you want to expand.
	 * @return CString The final path
	 */
	static CString GetStartupDirectory(CString sPath=_T(""));

	/*!
	 * @brief GetStartMenuDirectory 
	 * 
	 * The system start menu path
	 * @param sPath The path you want to expand.
	 * @return CString The final path
	 */
	static CString GetStartMenuDirectory(CString sPath=_T(""));

	/*!
	 * @brief GetProgramPath 
	 * 
	 * Get program path
	 * @return CString The program path
	 */
	static CString GetProgramPath(void);

	/*!
	 * @brief GetProgramName 
	 * 
	 * Get program filename
	 * @return CString The program filename
	 */
	static CString GetProgramName(void);


	/*!
	 * @brief IsDirectory 
	 * 
	 * Check Path is directory or not
	 * @return bool Is directory or not
	 */
	static bool IsDirectory(CString sPath);

	/*!
	 * @brief IsDirectory 
	 * 
	 * Check directory is empty or not
	 * @return bool Is empty or not
	 */
	static bool IsEmpty(CString sPath);

	/*!
	 * @brief Exists 
	 * 
	 * Check Path is exists or not
	 * @return bool Is exists or not
	 */
	static bool Exists(CString sPath);

	/*!
	 * @brief Exists 
	 * 
	 * Check Path is exists or not
	 * @return bool Is exists or not
	 */
	static void Open(CString sPath);

	/*!
	 * @brief Browse 
	 * 
	 * Open a dialog to browse file
	 * @param lpszFilter A series of string pairs that specify filters you can apply to the file. For example: "Excel File(*.xls)|*.xls;||"
	 * @param lpszDefExt The default file name extension. If this parameter is NULL, no extension is appended.
	 * @param bOpen Type of dialog box to create, TRUE to construct a File Open dialog box. Set it to FALSE to construct a File Save As dialog box.
	 * @param lpInitiaDir The default directory open in file dialog
	 * @param lpszFileName The initial file name that appears in the Filename box. If NULL, no initial file name appears.
	 * @return CString The file Path. User cancels if it's empty.
	 */
	static CString Browse(LPCTSTR lpszFilter, LPCTSTR lpszDefExt, BOOL bOpen, LPCTSTR lpszFileName, LPCTSTR lpInitiaDir=NULL);

	/*!
	 * @brief Folder 
	 * 
	 * Open a dialog to browse folder
	 * @param hWnd A handle to the owner window for the dialog box.
	 * @param sRootPath Specifies the path of a folder to select.
	 * @return CString The final path
	 */
	static CString Folder(HWND hWnd, CString sRootPath=_T(""));

	/*!
	 * @brief Copy 
	 * 
	 * copy file or folder with a loading dialog
	 * @param sSrc Source file or folder.
	 * @param sDst Copy Target Path.
	 * @return CString The final path
	 */
	static int CopyTo(vector<CString> lstSrc, CString sDst);

	/*!
	 * @brief GetFileList 
	 * 
	 * Traversing folder and get file list
	 * @param sDirectory The folder to traversing.
	 * @param filter A Callback used to check file. It will be skip file/folder when return false
	 * @return CString The final path
	 */
	static vector<CString> Traversing(CString sDirectory, FILITER filter=NULL);

	/*!
	 * @brief Traversing 
	 * 
	 * Traversing folder and get file list
	 * @param sDirectory The folder to traversing.
	 * @param sFormat A Callback used to check file. It will be skip file/folder when return false
	 * @return CString The final path
	 */
	static vector<CString> GetFileList(CString sDirectory, CString sFormat=_T("*"));

	/*!
	 * @brief Create
	 * 
	 * Creates all the directories in the specified path, beginning with the root.
	 * @param sPath A valid path name. If the final component of the path is a directory, not a file name.
	 * @return bool If the function succeeds, the return value is true;
	 */
	static bool Create(CString sPath);
};

/*!
	* @brief PATH_JOIN 
	* 
	* Stitching path macro define, you don't want to add null in the end
	* @return CString The final path
	*/
#define _PATH_JOIN(...) Path::Join(__VA_ARGS__, NULL)

}
