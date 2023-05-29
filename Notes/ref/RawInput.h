#pragma once

namespace Easy {

typedef enum RAW_TYPE
{
	RAW_TYPE_HID = 0x01,
	RAW_TYPE_KB = 0x02,
	RAW_TYPE_MS = 0x04,
};

class CRawInput
{
	CRawInput();
	~CRawInput();

public:
	static bool Register(HWND hWnd, WORD wRawType);
	static bool Remove(HWND hWnd, WORD wRawType);

private:
	static bool SetRawInput(HWND hWnd, WORD wRawType, bool bRegister);
};

}