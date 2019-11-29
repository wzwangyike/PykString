#pragma once

#include <windows.h>
template <UINT nCodePage = 936>
class CPykMgrTemplate
{
public:
	CPykMgrTemplate(const char *pStr) : m_pcData(NULL), m_pwcData(NULL)
	{
		if (pStr)
		{
			m_pcData = new char[strlen(pStr) + 1];
			strcpy_s(m_pcData, strlen(pStr) + 1, pStr);
		}
	}
	CPykMgrTemplate(const wchar_t *pStr) : m_pcData(NULL), m_pwcData(NULL)
	{
		if (pStr)
		{
			m_pwcData = new wchar_t[wcslen(pStr) + 1];
			wcscpy_s(m_pwcData, wcslen(pStr) + 1, pStr);
		}
	}

	CPykMgrTemplate(const char* pStr, const char *pEnd) : m_pcData(NULL), m_pwcData(NULL)
	{
		if (pStr && pEnd)
		{
			m_pcData = new char[pEnd - pStr + 1];
			strncpy_s(m_pcData, pEnd - pStr + 1, pStr, pEnd - pStr);
		}
	}
	CPykMgrTemplate(const wchar_t* pStr, const wchar_t *pEnd) : m_pcData(NULL), m_pwcData(NULL)
	{
		if (pStr)
		{
			m_pwcData = new wchar_t[pEnd - pStr + 1];
			wcsncpy_s(m_pwcData, pEnd - pStr + 1, pStr, pEnd - pStr);
		}
	}
	CPykMgrTemplate(const CPykMgrTemplate&) = delete;

	operator const char*()
	{
		if (!m_pcData && m_pwcData)
		{
			int nLen = WideCharToMultiByte(nCodePage, 0, m_pwcData, -1, NULL, 0, NULL, NULL);
			if (0 != nLen)
			{
				m_pcData = new char[nLen + 1];
				WideCharToMultiByte(nCodePage, 0, m_pwcData, -1, m_pcData, nLen + 1, NULL, NULL);
			}
		}
		return m_pcData;
	}

	operator const wchar_t *()
	{
		if (!m_pwcData && m_pcData)
		{
			int nLen = MultiByteToWideChar(nCodePage, 0, m_pcData, -1, NULL, 0);
			if (0 != nLen)
			{
				m_pwcData = new wchar_t[nLen + 1];
				MultiByteToWideChar(nCodePage, 0, m_pcData, -1, m_pwcData, nLen + 1);
			}
		}
		return m_pwcData;
	}

	~CPykMgrTemplate()
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

using CPykMgr = CPykMgrTemplate<>;