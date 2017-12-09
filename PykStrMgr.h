#pragma once

#if _MSVC_LANG >= 201103L
#include <codecvt>
#include <locale>
#else
#include <windows.h>
#endif
#if _MSVC_LANG >= 201103L
class CPykMgr
{
public:
	CPykMgr(const char *pStr)
	{
		if (pStr)
		{
			m_strData = pStr;
		}
	}
	CPykMgr(const wchar_t *pStr)
	{
		if (pStr)
		{
			m_wstrData = pStr;
		}
	}

	operator const char*()
	{
		if (0 == m_strData.length() && 0 != m_wstrData.length())
		{
			std::wstring_convert<std::codecvt<wchar_t, char, std::mbstate_t>>
				converter(new std::codecvt<wchar_t, char, std::mbstate_t>("CHS"));
			m_strData = converter.to_bytes(m_wstrData);
		}
		return m_strData.c_str();
	}

	operator const wchar_t *()
	{
		if (0 == m_wstrData.length() && 0 != m_strData.length())
		{
			std::wstring_convert<std::codecvt<wchar_t, char, std::mbstate_t>>
				converter(new std::codecvt<wchar_t, char, std::mbstate_t>("CHS"));
			m_wstrData = converter.from_bytes(m_strData);
		}
		return m_wstrData.c_str();
	}
private:
	std::string m_strData;
	std::wstring m_wstrData;
};
#else
class CPykMgr
{
public:
	CPykMgr(const char *pStr) : m_pcData(NULL), m_pwcData(NULL)
	{
		if (pStr)
		{
			m_pcData = new char[strlen(pStr) + 1];
			strcpy_s(m_pcData, strlen(pStr) + 1, pStr);
		}
	}
	CPykMgr(const wchar_t *pStr) : m_pcData(NULL), m_pwcData(NULL)
	{
		if (pStr)
		{
			m_pwcData = new wchar_t[wcslen(pStr) + 1];
			wcscpy_s(m_pwcData, wcslen(pStr) + 1, pStr);
		}
	}

	operator const char*()
	{
		if (!m_pcData && m_pwcData)
		{
			int nLen = WideCharToMultiByte(936, 0, m_pwcData, -1, NULL, 0, NULL, NULL);
			if (0 != nLen)
			{
				m_pcData = new char[nLen + 1];
				WideCharToMultiByte(936, 0, m_pwcData, -1, m_pcData, nLen + 1, NULL, NULL);
			}
		}
		return m_pcData;
	}

	operator const wchar_t *()
	{
		if (!m_pwcData && m_pcData)
		{
			int nLen = MultiByteToWideChar(936, 0, m_pcData, -1, NULL, 0);
			if (0 != nLen)
			{
				m_pwcData = new wchar_t[nLen + 1];
				MultiByteToWideChar(936, 0, m_pcData, -1, NULL, 0);
			}
		}
		return m_pwcData;
	}

	~CPykMgr()
	{
		if (m_pcData)
		{
			delete[]m_pcData;
		}
		if (m_pwcData)
		{
			delete[]m_pwcData;
		}
	}
private:
	char *m_pcData;
	wchar_t *m_pwcData;
};
#endif