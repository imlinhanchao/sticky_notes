#include "stdafx.h"
#include "Json.h"
#include "../json/json.h"

#ifdef _WIN64
#ifdef _DEBUG
#pragma comment(lib, "json/jsondx64.lib")
#else
#pragma comment(lib, "json/jsonx64.lib")
#endif
#else
#ifdef _DEBUG
#pragma comment(lib, "json/jsond.lib")
#else
#pragma comment(lib, "json/json.lib")
#endif
#endif


using namespace Json;

namespace Easy
{
CString JSON::s_sLastError = _T("");
bool JSON::Parse( CString sData, JsonValue& jsonData )
{
	Json::Value  value;
	Json::Reader reader;
#ifdef _UNICODE
	USES_CONVERSION;
	std::string sText_s(T2A(sData));
#else
	std::string sText_s(sData.GetBuffer());
	sData.ReleaseBuffer();
#endif

	if(!reader.parse(sText_s, value))
	{
		std::string sError_s= reader.getFormatedErrorMessages();
		s_sLastError = CString(sError_s.c_str());
		return false;
	}
	jsonData.Init(&value);
	return true;
}

JsonValue JsonValue::operator[]( const CString &key )
{
	JsonValue jvalue;
	Json::Value value = *((Json::Value*)m_jsonData);
#ifdef _UNICODE
	USES_CONVERSION;
	Json::Value data = value[T2A(key)];
#else
	CString sKey = key;
	Json::Value data = value[sKey.GetBuffer()];
	sKey.ReleaseBuffer();
#endif
	jvalue.Init(&data);
	return jvalue;
}

JsonValue JsonValue::operator[]( const char* key )
{
	JsonValue jvalue;
	Json::Value value = *((Json::Value*)m_jsonData);

	Json::Value data = value[key];
	jvalue.Init(&data);
	return jvalue;
}

JsonValue JsonValue::operator[]( const unsigned int &key )
{
	JsonValue jvalue;
	Json::Value value = *((Json::Value*)m_jsonData);
	Json::Value data = value[key];
	jvalue.Init(&data);
	return jvalue;
}

CString JsonValue::ToString( void )
{
	Json::Value value = *((Json::Value*)m_jsonData);
	if (value.isString())
	{
		std::string sValue = value.asString();
		return CString(sValue.c_str());
	}
	else if(value.isUInt())
	{
		CString sValue;
		sValue.Format(_T("%u"), value.asUInt());
	}
	else if(value.isInt())
	{
		CString sValue;
		sValue.Format(_T("%d"), value.asInt());
	}
	else if(value.isDouble())
	{
		CString sValue;
		sValue.Format(_T("%lf"), value.asDouble());
	}
	else if(value.isDouble())
	{
		CString sValue;
		sValue.Format(_T("%lf"), value.asDouble());
	}
	else if(value.isBool())
	{
		CString sValue;
		sValue.Format(_T("%s"), value.asBool() ? _T("true") : _T("false"));
	}
	else if(value.isNull())
	{
		CString sValue(_T("NULL"));
	}
	return _T("");
}

void JsonValue::Init( void* jsonData )
{
	Json::Value value = *((Json::Value*)jsonData);
	if (m_jsonData != NULL) delete m_jsonData;
	m_jsonData = new Json::Value(value);
}

JsonValue::~JsonValue( void )
{
	if (m_jsonData != NULL)
	{
		delete m_jsonData;
		m_jsonData = NULL;
	}
}

JsonValue::JsonValue( void )
{
	m_jsonData = new Json::Value();
}

JsonValue::JsonValue( JsonValue& json )
{
	this->m_jsonData = new Json::Value(*((Json::Value*)json.m_jsonData));
}

int JsonValue::size( void )
{
	return ((Json::Value*)m_jsonData)->size();
}

int JsonValue::key( CString sKeys[], int& nCount )
{
	Json::Value value = *((Json::Value*)m_jsonData);
	Json::Value::Members members = value.getMemberNames();  
	int i = 0;
	for (Json::Value::Members::iterator it = members.begin(); it != members.end(); ++it) 
	{  
		if (i < nCount)
		{
			std::string name = *it;  
			CString sValue(name.c_str());
			sKeys[i] = sValue;
		}
		i++;
	}

	if(i < nCount) nCount = i;

	return i;
}

bool JsonValue::IsNull( void )
{
	Json::Value value = *((Json::Value*)m_jsonData);
	return value.isNull();
}

bool JsonValue::IsBool( void )
{
	Json::Value value = *((Json::Value*)m_jsonData);
	return value.isBool();
}

bool JsonValue::IsDouble( void )
{
	Json::Value value = *((Json::Value*)m_jsonData);
	return value.isDouble();
}

bool JsonValue::IsIntegral( void )
{
	Json::Value value = *((Json::Value*)m_jsonData);
	return value.isIntegral();
}

bool JsonValue::IsString( void )
{
	Json::Value value = *((Json::Value*)m_jsonData);
	return value.isString();
}

bool JsonValue::IsArray( void )
{
	Json::Value value = *((Json::Value*)m_jsonData);
	return value.isArray();
}

bool JsonValue::IsObject( void )
{
	Json::Value value = *((Json::Value*)m_jsonData);
	return value.isObject();
}

long JsonValue::ToIntegral( void )
{
	Json::Value value = *((Json::Value*)m_jsonData);
	if(value.isInt()) return value.asInt();
	if(value.isUInt()) return value.asUInt();
	return 0;
}

double JsonValue::ToDouble( void )
{
	Json::Value value = *((Json::Value*)m_jsonData);
	if(value.isDouble()) return value.asDouble();
	return 0.0;
}

bool JsonValue::ToBool( void )
{
	Json::Value value = *((Json::Value*)m_jsonData);
	if(value.isBool()) return value.asBool();
	return false;
}

CString JsonValue::ToStyledString( void )
{
	Json::Value value = *((Json::Value*)m_jsonData);
	std::string sStyleData = value.toStyledString();
	CString sData(sStyleData.c_str());
	return sData;
}

JsonValue JsonValue::Append( CString sKey, int data )
{
	Json::Value* j = (Json::Value*)m_jsonData;
#ifdef _UNICODE
	USES_CONVERSION;
	(*j)[T2A(sKey)] = data;
#else
	(*j)[sKey.GetBuffer()] = data;
	sKey.ReleaseBuffer();
#endif
	JsonValue value;
	value.Init(j);
	return value;
}

JsonValue JsonValue::Append( CString sKey, double data )
{
	Json::Value* j = (Json::Value*)m_jsonData;
#ifdef _UNICODE
	USES_CONVERSION;
	(*j)[T2A(sKey)] = data;
#else
	(*j)[sKey.GetBuffer()] = data;
	sKey.ReleaseBuffer();
#endif
	JsonValue value;
	value.Init(j);
	return value;
}

JsonValue JsonValue::Append( CString sKey, bool data )
{
	Json::Value* j = (Json::Value*)m_jsonData;
#ifdef _UNICODE
	USES_CONVERSION;
	(*j)[T2A(sKey)] = data;
#else
	(*j)[sKey.GetBuffer()] = data;
	sKey.ReleaseBuffer();
#endif
	JsonValue value;
	value.Init(j);
	return value;
}

JsonValue JsonValue::Append( CString sKey, CString data )
{
	Json::Value* j = (Json::Value*)m_jsonData;
#ifdef _UNICODE
	USES_CONVERSION;
	(*j)[T2A(sKey)] = T2A(data);
#else
	(*j)[sKey.GetBuffer()] = data.GetBuffer();
	sKey.ReleaseBuffer();
	data.ReleaseBuffer();
#endif
	JsonValue value;
	value.Init(j);
	return value;
}

JsonValue JsonValue::Append( CString sKey, JsonValue data )
{
	Json::Value* j = (Json::Value*)m_jsonData;
	if (sKey.IsEmpty()) {
		j->append(Json::Value(*((Json::Value*)data.m_jsonData)));
	} else {
#ifdef _UNICODE
		USES_CONVERSION;
		(*j)[T2A(sKey)] = Json::Value(*((Json::Value*)data.m_jsonData));
#else
		(*j)[sKey.GetBuffer()] = Json::Value(*((Json::Value*)data.m_jsonData));
		sKey.ReleaseBuffer();
#endif
	}
	JsonValue value;
	value.Init(j);
	return value;
}

}