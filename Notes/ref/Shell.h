#pragma once

namespace Easy {

class Shell
{
public:
	Shell(void);
	~Shell(void);

	typedef enum SHELL_TYPE { CONSOLE = 0, APP };

	/*!
	 * @brief Execute 
	 * 
	 * Execute a program
	 * @param sPath The path of program.
	 * @param sCommand The command line with program
	 * @param type The type of program to execute
	 * @param bShow Show the program window or not. Not work for all program.
	 * @return bool Execute success or not
	 */
	bool Execute(CString sPath, CString sCommand=_T(""), SHELL_TYPE type=APP, bool bShow=true, CString sWorkDirectory=_T(""));

	/*!
	 * @brief IsRunning 
	 * 
	 * Is the program running
	 * @return bool True means running, false was not.
	 */
	bool IsRunning(void);

	/*!
	 * @brief Stop 
	 * 
	 * Stop run the program.
	 * @return bool Stop success or not
	 */
	bool Stop(void);

	/*!
	 * @brief GetOutput 
	 * 
	 * Get the program output, work for console application.
	 * @return CString The output string.
	 */
	CString GetOutput(void);

	/*!
	 * @brief GetOutput 
	 * 
	 * Get the program window handle.
	 * @return HWND The window handle.
	 */
	HWND GetWnd(void);

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
	static DWORD WINAPI ShellThread(LPVOID lpParam);
	static BOOL WINAPI EnumWindowsProc(HWND hWnd, LPARAM lParam);

	bool ExecuteConsole(CString sPath, CString sCommand, bool bShow, CString sWorkDirectory);
	bool ExecuteApplication(CString sPath, CString sCommand, bool bShow, CString sWorkDirectory);

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

	PROCESS_INFORMATION m_pi; 
	HANDLE m_hRead;

	bool m_bRunning;
	HWND m_hWnd;
	CString m_sOutput;
};

}
