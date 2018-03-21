#pragma once

#ifndef USE_C11
#if _MSC_VER >= 1800
#define USE_C11
#endif
#endif // !USE_C11

//#include <memory>
#include <stdarg.h> 
#include <stdio.h>
#include "PykMgr.h"
class CPykStrTrait
{
public:

	static int GetFormattedLength(const char * pszFormat, va_list args)
	{
		return _vscprintf(pszFormat, args);
	}

	static int GetFormattedLength(const wchar_t * pszFormat, va_list args)
	{
		return _vscwprintf(pszFormat, args);
	}

	static int FormatInternel(char *pOut, size_t nLen, const char * pszFormat, va_list args)
	{
		return vsprintf_s(pOut, nLen, pszFormat, args);
	}

	static int FormatInternel(wchar_t *pOut, size_t nLen, const wchar_t * pszFormat, va_list args)
	{
		return vswprintf_s(pOut, nLen, pszFormat, args);
	}

	static int MakeLower(char *pStr, size_t nLen)
	{
		return _strlwr_s(pStr, nLen);
	}

	static int MakeLower(wchar_t *pStr, size_t nLen)
	{
		return _wcslwr_s(pStr, nLen);
	}

	static int MakeUpper(char *pStr, size_t nLen)
	{
		return _strupr_s(pStr, nLen);
	}

	static int MakeUpper(wchar_t *pStr, size_t nLen)
	{
		return _wcsupr_s(pStr, nLen);
	}

	static char * Find(char *pStr, const char cChar)
	{
		return strchr(pStr, cChar);
	}

	static wchar_t * Find(wchar_t *pStr, const wchar_t cChar)
	{
		return wcschr(pStr, cChar);
	}

	static char * Find(char *pStr, const char *pFind)
	{
		return strstr(pStr, pFind);
	}

	static wchar_t * Find(wchar_t *pStr, const wchar_t *pFind)
	{
		return wcsstr(pStr, pFind);
	}

	static size_t GetLength(const char *pStr)
	{
		return strlen(pStr);
	}

	static size_t GetLength(const wchar_t *pStr)
	{
		return wcslen(pStr);
	}

	static const char *FindCharRev(const char *p, char c)
	{
		return strrchr(p, c);
	}

	static const wchar_t *FindCharRev(const wchar_t *p, wchar_t c)
	{
		return wcsrchr(p, c);
	}

	static int Compare(const char *pstr1, const char *pstr2)
	{
		return strcmp(pstr1, pstr2);
	}

	static int Compare(const wchar_t *pstr1, const wchar_t *pstr2)
	{
		return wcscmp(pstr1, pstr2);
	}

	static int CompareNoCase(const char *pstr1, const char *pstr2)
	{
		return _stricmp(pstr1, pstr2);
	}

	static int CompareNoCase(const wchar_t *pstr1, const wchar_t *pstr2)
	{
		return _wcsicmp(pstr1, pstr2);
	}
};

class CPykStrMgr;

template <class _Type>
class CPykChar
{
public:
	CPykChar(_Type *p) : m_p(p) {}
	CPykChar & operator=(_Type c)
	{
		if (m_p)
			*m_p = c;
		return *this;
	}
	operator _Type() const
	{
		return m_p ? *m_p : '\0';
	}
private:
	_Type * m_p;
};

template <class _Type, class _Trait>
class CPykStringT
{
public:
	CPykStringT()
	{
		m_pData = m_pNuil;
		m_nLen = 0;
		memset(m_pNuil, 0, sizeof(m_pNuil));
	}

	CPykStringT(const _Type *pString, size_t nInitLen = -1)
	{
		InitByStr(pString, nInitLen);
	}
#ifdef USE_C11
	CPykStringT(CPykStrMgr mgr) : CPykStringT((const _Type *)mgr) {}

	CPykStringT(const CPykStringT &s) : CPykStringT(s.m_pData) {}

	CPykStringT(CPykStringT &&s)
	{
		memset(m_pNuil, 0, sizeof(m_pNuil));
		if (0 == s.m_nLen)
		{
			return;
		}
		m_pData = s.m_pData;
		m_nLen = s.m_nLen;
		s.m_pData = s.m_pNuil;
		s.m_nLen = 0;
	}
#else
	CPykStringT(CPykStrMgr mgr)
	{
		InitByStr((const _Type *)mgr);
	}
	CPykStringT(const CPykStringT &s)
	{
		InitByStr(s.m_pData);
	}
#endif

	~CPykStringT()
	{
		if (m_pData != m_pNuil)
		{
			delete[]m_pData;
			m_pData = NULL;
		}
	}

	size_t GetLength() const
	{
		return _Trait::GetLength(m_pData);
	}

	operator const _Type *() const
	{
		return m_pData;
	}

	bool operator ==(const _Type c) const
	{
		size_t nSelfLen = GetLength();
		if (nSelfLen == 1 &&
			m_pData[0] == c)
		{
			return true;
		}
		return false;
	}

	bool operator ==(const _Type *pString) const
	{
		if (!pString)
		{
			return false;
		}
		size_t nSelfLen = GetLength();
		size_t nOtherLen = _Trait::GetLength(pString);
		if (nSelfLen == nOtherLen)
		{
			return !memcmp(m_pData, pString, nSelfLen * sizeof(_Type));
		}
		return false;
	}

	bool operator ==(CPykStrMgr mgr) const
	{
		return operator ==((const _Type *)mgr);
	}

	bool operator !=(const _Type c) const
	{
		return !operator ==(c);
	}

	bool operator !=(const CPykStringT &str) const
	{
		return !operator ==((const _Type *)str);
	}

	bool operator !=(CPykStrMgr mgr) const
	{
		return !operator ==((const _Type *)mgr);
	}

#ifdef USE_C11
	CPykStringT &operator =(CPykStringT &&s)
	{
		if (0 == s.m_nLen)
		{
			memset(m_pData, 0, m_nLen * sizeof(_Type));
		}
		else
		{
			if (m_pData != m_pNuil)
			{
				delete[]m_pData;
				m_pData = NULL;
			}
			m_pData = s.m_pData;
			m_nLen = s.m_nLen;
		}
		s.m_pData = s.m_pNuil;
		s.m_nLen = 0;
		return *this;
	}
#endif
	CPykStringT &operator =(const _Type *pString)
	{
		size_t nAddLen = pString ? _Trait::GetLength(pString) : 0;
		if (0 == nAddLen)
		{
			memset(m_pData, 0, m_nLen * sizeof(_Type));
		}
		else
		{
			Resize(nAddLen);
			memcpy_s(m_pData, m_nLen * sizeof(_Type), pString, nAddLen * sizeof(_Type));
		}
		
		return *this;
	}

	CPykStringT &operator =(const CPykStringT &str)
	{
		return operator =((const _Type *)str.m_pData);
	}

	CPykStringT &operator =(CPykStrMgr mgr)
	{
		return operator =((const _Type *)mgr);
	}
#ifdef USE_C11
	template <class AddType>
	CPykStringT operator +(AddType &&pString)
	{
		CPykStringT sTemp(*this);
		sTemp += pString;
		return sTemp;
	}
#else
	CPykStringT operator +(const CPykStringT &pString)
	{
		CPykStringT sTemp(*this);
		sTemp += pString;
		return sTemp;
	}
#endif
	CPykStringT &operator +=(const _Type *pString)
	{
		if (!pString)
		{
			return *this;
		}
		size_t nNowLen = GetLength();
		size_t nAddLen = _Trait::GetLength(pString);
		Resize(nNowLen + nAddLen, true);
		memcpy_s(m_pData + nNowLen, (m_nLen - nNowLen) * sizeof(_Type), pString, nAddLen * sizeof(_Type));

		return *this;
	}

	CPykStringT &operator +=(const CPykStringT &str)
	{
		return *this += (const _Type *)str;
	}

	CPykStringT &operator +=(CPykStrMgr mgr)
	{
		return *this += (const _Type *)mgr;
	}

	CPykStringT &operator +=(int l)
	{
		AppendFormat((const _Type *)(CPykStrMgr)("%d"), l);
		return *this;
	}

	CPykStringT &operator +=(size_t l)
	{
		AppendFormat((const _Type *)(CPykStrMgr)("%ud"), l);
		return *this;
	}

	CPykStringT &operator +=(long l)
	{
		AppendFormat((const _Type *)(CPykStrMgr)("%l"), l);
		return *this;
	}

	CPykStringT &operator +=(unsigned long l)
	{
		AppendFormat((const _Type *)(CPykStrMgr)("%ul"), l);
		return *this;
	}

	CPykStringT &operator +=(double d)
	{
		AppendFormat((const _Type *)(CPykStrMgr)("%f"), d);
		return *this;
	}

	CPykStringT &operator +=(const char cChar)
	{
		size_t nNowLen = GetLength();
		Resize(nNowLen + 1, true);
		m_pData[nNowLen] = cChar;

		return *this;
	}

	CPykChar<_Type> operator[](_In_ int iChar) const
	{
		if (iChar < 0)
		{
			if ((size_t)abs(iChar) > GetLength())
			{
				return NULL;
			}
			return &m_pData[GetLength() + iChar];
		}
		if ((size_t)iChar >= GetLength())
			return NULL;

		return &m_pData[iChar];
	}

	friend bool operator<(_In_ const CPykStringT& str1, _In_ const CPykStringT& str2)
	{
		return(str1.Compare(str2) < 0);
	}

	friend bool operator>(_In_ const CPykStringT& str1, _In_ const CPykStringT& str2)
	{
		return(str1.Compare(str2) > 0);
	}

	friend bool operator<=(_In_ const CPykStringT& str1, _In_ const CPykStringT& str2)
	{
		return(str1.Compare(str2) <= 0);
	}

	friend bool operator>=(_In_ const CPykStringT& str1, _In_ const CPykStringT& str2)
	{
		return(str1.Compare(str2) >= 0);
	}

	friend bool operator ==(_In_ const CPykStringT& str1, _In_ const CPykStringT& str2)
	{
		return(str1.Compare(str2) == 0);
	}

	friend bool operator !=(_In_ const CPykStringT& str1, _In_ const CPykStringT& str2)
	{
		return(str1.Compare(str2) != 0);
	}

	CPykStringT& MakeLower()
	{
		if (m_nLen > 0)
			_Trait::MakeLower(m_pData, m_nLen);
		return *this;
	}

	CPykStringT& MakeUpper()
	{
		if(m_nLen > 0)
			_Trait::MakeUpper(m_pData, m_nLen);
		return *this;
	}

	int Compare(const _Type *pString) const
	{
		if (!pString)
		{
			return m_pData[0];
		}
		return _Trait::Compare(m_pData, pString);
	}

	int CompareNoCase(const _Type *pString) const
	{
		if (!pString)
		{
			return m_pData[0];
		}
		return _Trait::CompareNoCase(m_pData, pString);
	}

	int Compare(const CPykStringT &str) const
	{
		return Compare((const _Type *)str);
	}

	int CompareNoCase(const CPykStringT &str) const
	{
		return CompareNoCase((const _Type *)str);
	}

	int Compare(CPykStrMgr mgr) const
	{
		return Compare((const _Type *)mgr);
	}

	int CompareNoCase(CPykStrMgr mgr) const
	{
		return CompareNoCase((const _Type *)mgr);
	}

	const _Type *GetString() const
	{
		return m_pData;
	}

	_Type *GetBuffer(size_t nLen = 0)
	{
		if ((nLen + 1) <= m_nLen)
		{
			return m_pData;
		}
		Resize(nLen, true);
		return m_pData;
	}

	void ReleaseBuffer(_In_ int nNewLength = -1)
	{
		nNewLength;
	}

	CPykStringT Left(_In_ size_t nCount) const
	{
		size_t nLength = GetLength();
		if (nCount >= nLength)
		{
			return(*this);
		}

		return(CPykStringT(m_pData, nCount));
	}

	CPykStringT Right(_In_ size_t nCount) const
	{
		size_t nLength = GetLength();
		if (nCount >= nLength)
		{
			return(*this);
		}

		return(CPykStringT(m_pData + nLength - nCount, nCount));
	}

	CPykStringT Mid(_In_ size_t iFirst) const
	{
		return Mid(iFirst, GetLength() - iFirst);
	}

	CPykStringT Mid(_In_ size_t iFirst, _In_ size_t nCount) const
	{
		if (iFirst + nCount > GetLength())
		{
			nCount = GetLength() - iFirst;
		}
		if (iFirst > GetLength() ||
			0 == nCount)
		{
			return CPykStringT();
		}

		// optimize case of returning entire string
		if ((iFirst == 0) && ((iFirst + nCount) == GetLength()))
		{
			return(*this);
		}

		return(CPykStringT(m_pData + iFirst, nCount));
	}

	void Empty()
	{
		memset(m_pData, 0, m_nLen * sizeof(_Type));
	}

	bool IsEmpty() const
	{
		return GetLength() == 0;
	}

	int GetAllocLength() const
	{
		return m_nLen;
	}

	void FormatV(_In_ const _Type * pszFormat, va_list args)
	{
		if (!pszFormat)
			return;

		int nLength = _Trait::GetFormattedLength(pszFormat, args);
		if (nLength < 0)
		{
			return;
		}
		_Type *pszBuffer = GetBuffer(nLength);
		_Trait::FormatInternel(pszBuffer, m_nLen, pszFormat, args);
	}

	void Format(const _Type * pszFormat, ...)
	{
		if (!pszFormat)
		{
			return;
		}
		va_list argList;
		va_start(argList, pszFormat);

		FormatV(pszFormat, argList);

		va_end(argList);
	}

	void AppendFormatV(const _Type* pszFormat, va_list args)
	{
		if (!pszFormat)
			return;

		int nCurrentLength = GetLength();
		int nAppendLength = _Trait::GetFormattedLength(pszFormat, args);
		if (nAppendLength < 0)
		{
			return;
		}

		_Type* pszBuffer = GetBuffer(nCurrentLength + nAppendLength);
		_Trait::FormatInternel(pszBuffer + nCurrentLength,
			nAppendLength + 1, pszFormat, args);
	}

	void AppendFormat(const _Type* pszFormat, ...)
	{
		if (!pszFormat)
		{
			return;
		}

		va_list argList;
		va_start(argList, pszFormat);
		AppendFormatV(pszFormat, argList);
		va_end(argList);
	}

	int Find(_In_ const _Type cChar, _In_ size_t iStart = 0) const
	{
		size_t nLength = GetLength();
		if (iStart >= nLength)
		{
			return -1;
		}
		_Type *pFind = _Trait::Find(m_pData + iStart, cChar);
		return((pFind == NULL) ? -1 : int(pFind - m_pData));
	}

	int Find(_In_ _Type *pStr, _In_ size_t iStart = 0) const
	{
		size_t nLength = GetLength();
		if (!pStr ||
			iStart >= nLength)
		{
			return -1;
		}
		_Type *pFind = _Trait::Find(m_pData + iStart, pStr);
		return((pFind == NULL) ? -1 : int(pFind - m_pData));
	}

	int Find(_In_ const CPykStringT &Str, _In_ size_t iStart = 0) const
	{
		return Find(Str.m_pData, iStart);
	}

	int Find(_In_ CPykStrMgr mgr, _In_ size_t iStart = 0) const
	{
		return Find((const _Type *)mgr, iStart);
	}

	int ReverseFind(_In_ _Type ch) const 
	{
		const _Type* psz = _Trait::FindCharRev(m_pData, ch);

		return((psz == NULL) ? -1 : int(psz - m_pData));
	}

	size_t Replace(const _Type cOld, const _Type cNew)
	{
		size_t nCount = 0;
		if (cOld == cNew)
		{
			return nCount;
		}

		size_t nLen = GetLength();
		
		for (size_t i = 0; i < nLen; i++)
		{
			if (m_pData[i] == cOld)
			{
				nCount++;
				m_pData[i] = cNew;
			}
		}
		return nCount;
	}

	size_t Replace(const _Type *pOld, const _Type *pNew)
	{
		size_t nCount = 0;
		if (!pOld ||
			!pNew)
		{
			return nCount;
		}
		size_t nOldLen = _Trait::GetLength(pOld);
		size_t nNewLen = _Trait::GetLength(pNew);

		_Type *pStr = m_pData;
		_Type *pFind = NULL;
		while (pFind = _Trait::Find(pStr, pOld))
		{
			nCount++;
			pStr = pFind + nOldLen;
		}

		if (nCount)
		{
			size_t nLen = GetLength();
			if (nNewLen > nOldLen)
			{
				size_t nAdd = (nNewLen - nOldLen) * nCount;
				if (m_nLen <= nLen + nAdd)
				{
					Resize(nLen + nAdd, true);
				}
			}

			pStr = m_pData;
			pFind = NULL;
			while (pFind = _Trait::Find(pStr, pOld))
			{
				size_t nBalance = nLen - ((size_t)(pFind - m_pData) + nOldLen);
				memmove_s(pFind + nNewLen, nBalance * sizeof(_Type), pFind + nOldLen, nBalance * sizeof(_Type));
				*(pFind + nNewLen + nBalance) = '\0';
				memcpy_s(pFind, nNewLen * sizeof(_Type), pNew, nNewLen * sizeof(_Type));
				pStr = pFind + nNewLen;
				nLen += (nNewLen - nOldLen);
			}
		}
		return nCount;
	}

	size_t Replace(const CPykStringT &strOld, const CPykStringT &strNew)
	{
		return Replace((const _Type *)strOld, (const _Type *)strNew);
	}

	size_t Replace(CPykStrMgr mgrOld, CPykStrMgr mgrNew)
	{
		return Replace((const _Type *)mgrOld, (const _Type *)mgrNew);
	}

	int Insert(_In_ int iIndex, _In_z_ _Type* psz)
	{
		if (iIndex < 0)
			iIndex = 0;

		int nLength = GetLength();

		if (iIndex > nLength)
		{
			iIndex = nLength;
		}
		
		int nInsertLength = psz ? _Trait::GetLength(psz) : 0;
		
		if (nInsertLength > 0)
		{
			_Type *pszBuffer = GetBuffer(nLength + nInsertLength);
			memmove_s(pszBuffer + iIndex + nInsertLength, (nLength - iIndex) * sizeof(_Type),
				pszBuffer + iIndex, (nLength - iIndex) * sizeof(_Type));
			memcpy_s(pszBuffer + iIndex, nInsertLength * sizeof(_Type),
				psz, nInsertLength * sizeof(_Type));
		}

		return(nLength + nInsertLength);
	}

	CPykStringT& TrimRight()
	{
		size_t nLen = GetLength();
		nLen--;
		while (' ' == m_pData[nLen])
		{
			m_pData[nLen] = '\0';
			nLen--;
		}
		return(*this);
	}

	CPykStringT& TrimLeft()
	{
		const _Type* psz = m_pData;

		while (' ' == *psz)
		{
			psz++;
		}

		if (psz != m_pData)
		{
			memmove_s(m_pData, m_nLen * sizeof(_Type), psz, (GetLength() - (psz - m_pData) + 1) * sizeof(_Type));
		}

		return(*this);
	}

	CPykStringT& Trim()
	{
		return TrimRight().TrimLeft();
	}

private:

	_Type m_pNuil[2];
	_Type *m_pData;
	size_t m_nLen;

	void Resize(size_t nLen, bool bRetain = false)
	{
		if (m_nLen < (nLen + 1))
		{
			_Type *pTemp = m_pData;
			m_pData = new _Type[nLen + 1];
			memset(m_pData, 0, (nLen + 1) * sizeof(_Type));

			if (pTemp && m_nLen > 0)
			{
				if (bRetain)
				{
					memcpy_s(m_pData, (nLen + 1) * sizeof(_Type), pTemp, m_nLen * sizeof(_Type));
				}
				delete[]pTemp;
			}
			m_nLen = nLen + 1;
		}
		else
		{
			if (!bRetain)
			{
				memset(m_pData, 0, m_nLen * sizeof(_Type));
			}
		}
		memset(m_pData + GetLength(), 0, (m_nLen - GetLength())* sizeof(_Type));
	}

	void InitByStr(const _Type *pString, size_t nInitLen = -1)
	{
		m_pData = m_pNuil;
		m_nLen = 0;
		memset(m_pNuil, 0, sizeof(m_pNuil));
		if (!pString)
		{
			return;
		}
		size_t nLen = _Trait::GetLength(pString);

		nLen = (nInitLen > nLen) ? nLen : nInitLen;
		Resize(nLen);
		memcpy_s(m_pData, m_nLen * sizeof(_Type), pString, nLen * sizeof(_Type));
	}
};

typedef CPykStringT<char, CPykStrTrait> CPykStringA;

typedef CPykStringT<wchar_t, CPykStrTrait> CPykStringW;

#ifdef UNICODE
typedef CPykStringW CPykString;
#else
typedef CPykStringA CPykString;
#endif

class CPykStrMgr : public CPykMgr
{
public:
	CPykStrMgr(const char *pStr) : CPykMgr(pStr){}
	CPykStrMgr(const wchar_t *pStr) : CPykMgr(pStr){}
	CPykStrMgr(const CPykStringA& str) : CPykMgr((const char *)str){}
	CPykStrMgr(const CPykStringW& str) : CPykMgr((const wchar_t *)str) {}
};