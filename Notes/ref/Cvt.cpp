#include "StdAfx.h"
#include "Cvt.h"

namespace Easy
{

CString Cvt::ToString(bool bValue)
{
	return bValue ? _T("1") : _T("0");
}

CString Cvt::ToString(int nValue)
{
	CString sValue = _T("");
	sValue.Format(_T("%d"), nValue);

	return sValue;
}

CString Cvt::ToString(long nValue)
{
	CString sValue = _T("");
	sValue.Format(_T("%ld"), nValue);

	return sValue;
}

CString Cvt::ToString(float fValue, int nPrecision)
{
	CString sValue = _T("");
	CString sFormat = _T("%f");
	if(nPrecision > 0)
	{
		sFormat.Format(_T("%%0.%df"), nPrecision);
	}

	sValue.Format(sFormat, fValue);

	return sValue;
}

CString Cvt::ToString(double fValue, int nPrecision)
{
	CString sValue = _T("");
	CString sFormat = _T("%f");
	if(nPrecision > 0)
	{
		sFormat.Format(_T("%%0.%df"), nPrecision);
	}

	sValue.Format(sFormat, fValue);
	return sValue;
}

CString Cvt::ToString(DWORD dwValue)
{
	CString sValue = _T("");
	sValue.Format(_T("%u"), dwValue);

	return sValue;
}

CString Cvt::ToString(UINT nValue)
{
	CString sValue = _T("");
	sValue.Format(_T("%u"), nValue);

	return sValue;
}

CString Cvt::ToString(COleDateTime date)
{
	CString sValue = _T("");
	sValue.Format(_T("%d-%02d-%02d %02d:%02d:%02d"), date.GetYear(), 
		date.GetMonth(), date.GetDay(), date.GetHour(), date.GetMinute(), date.GetSecond());

	return sValue;
}

CString Cvt::ToString( const TCHAR* pszFormat, ... )
{
	ASSERT(pszFormat && *pszFormat);

	TCHAR  szMsg[1024];	
	va_list  vargs;

	va_start(vargs, pszFormat);

	_vsnwprintf_s( szMsg, sizeof(szMsg) - 1, pszFormat, vargs);

	return CString(szMsg);
}

CString Cvt::ToString( CTime time, CString sFormat )
{
	return time.Format(sFormat);
}

COleDateTime Cvt::ToDateTime(CString sDateTime)
{
	COleDateTime dt;
	return dt.ParseDateTime(sDateTime) ? dt : COleDateTime(0, 0, 0, 0, 0, 0);
}

vector<CString> Cvt::SplitString( CString sData, CString sSp )
{
	vector<CString> lstList;

	int nPos = 0;
	CString sItem = sData.Tokenize(sSp, nPos);

	while (!sItem.IsEmpty())
	{
		lstList.push_back(sItem);
		sItem = sData.Tokenize(sSp, nPos);
	}

	return lstList;
}

CString Cvt::ToHex(COLORREF color)
{
	CString sColor = _T("");
	sColor.Format(_T("#%02X%02X%02X"), GetRValue(color), GetGValue(color), GetBValue(color));

	return sColor;
}

COLORREF Cvt::ToColor(CString sHex)
{
	if (sHex.GetLength() != 7)
	{
		return RGB(0, 0, 0);
	}

	int nR = 0, nG = 0, nB = 0;
	_stscanf(sHex, _T("#%2X%2X%2X"), &nR, &nG, &nB);
	return RGB(nR, nG, nB);
}

}