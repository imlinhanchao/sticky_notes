#include "StdAfx.h"
#include "RawInput.h"

namespace Easy {
	
CRawInput::CRawInput()
{

}

CRawInput::~CRawInput()
{

}

bool CRawInput::SetRawInput(HWND hWnd, WORD wRawType, bool bRegister)
{
	RAWINPUTDEVICE rid[3];
	memset (rid, 0, sizeof (rid));

	int n = 0;

	if (RAW_TYPE_HID & wRawType)
	{
		rid[n].usUsagePage = 0xFF00;
		rid[n].usUsage = 0x01;
		rid[n].dwFlags = bRegister ? RIDEV_INPUTSINK : RIDEV_REMOVE;
		rid[n].hwndTarget = bRegister ? hWnd : NULL;
		n++;
	}

	if (RAW_TYPE_KB & wRawType)
	{
		rid[n].usUsagePage = 0x01;
		rid[n].usUsage = 0x06;
		rid[n].dwFlags = bRegister ? RIDEV_INPUTSINK | RIDEV_NOHOTKEYS : RIDEV_REMOVE;
		rid[n].hwndTarget = bRegister ? hWnd : NULL;
		n++;
	}

	if (RAW_TYPE_MS & wRawType)
	{
		rid[n].usUsagePage = 0x01;
		rid[n].usUsage = 0x02;
		rid[n].dwFlags = bRegister ? RIDEV_INPUTSINK : RIDEV_REMOVE;
		rid[n].hwndTarget = bRegister ? hWnd : NULL;
		n++;
	}

	return RegisterRawInputDevices(rid, n, sizeof (RAWINPUTDEVICE)) == TRUE; // Register listen device input
}

bool CRawInput::Register( HWND hWnd, WORD wRawType )
{
	return SetRawInput(hWnd, wRawType, true);
}

bool CRawInput::Remove( HWND hWnd, WORD wRawType )
{
	return SetRawInput(hWnd, wRawType, false);
}

}