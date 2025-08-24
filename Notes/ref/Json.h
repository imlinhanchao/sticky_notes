#pragma once

namespace Easy {

class JsonValue;

class JSON
{
public:
	static bool Parse(CString sData, JsonValue& jsonData);
	static CString s_sLastError;
};

class JsonValue
{
public:
	JsonValue(void);
	JsonValue(JsonValue& json);
	~JsonValue(void);
	JsonValue operator[]( const CString &key );
	JsonValue operator[]( const char* key );
	JsonValue operator[]( const unsigned int &key );

	bool IsNull(void);
	bool IsBool(void);
	bool IsDouble(void);
	bool IsIntegral(void);
	bool IsString(void);
	bool IsArray(void);
	bool IsObject(void);

	CString ToString(void);
	CString ToStyledString(void);
	long ToIntegral(void);
	double ToDouble(void);
	bool ToBool(void);

	JsonValue Append(CString sKey, int data);
	JsonValue Append(CString sKey, double data);
	JsonValue Append(CString sKey, bool data);
	JsonValue Append(CString sKey, CString data);
	JsonValue Append(CString sKey, JsonValue data);

	int size(void);
	int key( CString sKeys[], int& nCount );

private:
	friend class JSON;
	void Init(void* jsonData);
	void* m_jsonData;
};

}

