// Misc.h: interface for the CMisc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MISC_H__4B2FDA3E_63C5_4F52_A139_9512105C3AD4__INCLUDED_)
#define AFX_MISC_H__4B2FDA3E_63C5_4F52_A139_9512105C3AD4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>

enum 
{ 
	MKS_NONE = 0x00,
	MKS_CTRL = 0x01, 
	MKS_SHIFT = 0x02, 
	MKS_ALT = 0x04, 
};

enum 
{ 
	MKC_LEFTRIGHT = 0x01, 
	MKC_UPDOWN = 0x02, 
	MKC_ANY = (MKC_LEFTRIGHT | MKC_UPDOWN) 
};

#ifndef _ttof
#define _ttof(str) Misc::Atof(str)
#endif

namespace Misc  
{
	BOOL Is64BitWindows();

	CString FormatGetLastError(DWORD dwLastErr = -1);

	BOOL CopyTexttoClipboard(const CString& sText, HWND hwnd, UINT nFormat = 0, BOOL bClear = TRUE); 
	CString GetClipboardText(UINT nFormat = 0); 
	BOOL ClipboardHasFormat(UINT nFormat);
	BOOL ClipboardHasText();

	char* WideToMultiByte(const WCHAR* szFrom, UINT nCodePage = CP_ACP);
	char* WideToMultiByte(const WCHAR* szFrom, int& nLength, UINT nCodePage = CP_ACP);
	WCHAR* MultiByteToWide(const char* szFrom, UINT nCodepage = CP_ACP);
	WCHAR* MultiByteToWide(const char* szFrom, int& nLength, UINT nCodepage = CP_ACP);
	CString& EncodeAsMultiByte(CString& sUnicode, UINT nCodePage = CP_ACP);
	CString& EncodeAsUnicode(CString& sMultibyte, UINT nCodePage = CP_ACP);

	BOOL GuidFromString(LPCTSTR szGuid, GUID& guid);
	BOOL IsGuid(LPCTSTR szGuid);
	BOOL GuidToString(const GUID& guid, CString& sGuid);
	BOOL GuidIsNull(const GUID& guid);
	void NullGuid(GUID& guid);
	BOOL SameGuids(const GUID& guid1, const GUID& guid2);
	CString NewGuid(GUID* pGuid = NULL);

	template <class T> 
	BOOL MatchAllT(const T& array1, const T& array2, BOOL bOrderSensitive = FALSE)
	{
		int nItem1 = array1.GetSize();
		int nItem2 = array2.GetSize();
		
		if (nItem1 != nItem2)
			return 0;
		
		if (bOrderSensitive)
		{
			while (nItem1--)
			{
				// check for non-equality
				if (!(array1[nItem1] == array2[nItem1]))
					return FALSE;
			}
			
			return TRUE;
		}
		
		// else order not important
		while (nItem1--)
		{
			// look for matching item
			nItem2 = array2.GetSize();

			while (nItem2--)
			{
				if (array1[nItem1] == array2[nItem2])
					break;
			}
			
			// no-match found == not the same
			if (nItem2 == -1)
				return FALSE;
		}
		
		return TRUE;
	}

	CString FormatComputerNameAndUser(char cSeparator = ':');
	CString GetComputerName();
	CString GetUserName();
	CString GetListSeparator();
	CString GetDecimalSeparator();
	CString GetDefCharset();
	CString GetAM();
	CString GetPM();
	CString GetTimeSeparator();
	CString GetTimeFormat(BOOL bIncSeconds = TRUE);
	CString GetShortDateFormat(BOOL bIncDOW = FALSE);
	CString GetDateSeparator();

	BOOL MatchAll(const CStringArray& array1, const CStringArray& array2, 
					 BOOL bOrderSensitive = FALSE, BOOL bCaseSensitive = FALSE);
	BOOL MatchAny(const CStringArray& array1, const CStringArray& array2, 
					BOOL bCaseSensitive = FALSE, BOOL bPartialOK = TRUE);

	BOOL MatchAll(const CDWordArray& array1, const CDWordArray& array2, 
		BOOL bOrderSensitive = FALSE);
	BOOL MatchAny(const CDWordArray& array1, const CDWordArray& array2);
	
#ifdef _DEBUG
	void Trace(const CStringArray& array);
#endif
	
	int RemoveItems(const CStringArray& aItems, CStringArray& aFrom, BOOL bCaseSensitive = FALSE);
	int RemoveEmptyItems(CStringArray& aFrom);
	BOOL RemoveItem(LPCTSTR szItem, CStringArray& aFrom, BOOL bCaseSensitive = FALSE);
	int AddUniqueItems(const CStringArray& aItems, CStringArray& aTo, BOOL bCaseSensitive = FALSE);
	int AddUniqueItem(const CString& sItem, CStringArray& aTo, BOOL bCaseSensitive = FALSE);
	int GetTotalLength(const CStringArray& array);

	const CString& GetItem(const CStringArray& array, int nItem);

	template <class T> 
	const T& GetItemT(const CArray<T, T&>& array, int nItem)
	{
		ASSERT(nItem >= 0 && nItem < array.GetSize());
		
		if (nItem < 0 || nItem >= array.GetSize())
		{
			static T dummy;
			return dummy;
		}
		
		return array.GetData()[nItem];
	}

	CString FormatArray(const CDWordArray& array, LPCTSTR szSep = NULL);
	CString FormatArray(const CDWordArray& array, TCHAR cSep);
	CString FormatArray(const CStringArray& array, LPCTSTR szSep = NULL);
	CString FormatArray(const CStringArray& array, TCHAR cSep);
	CString FormatArrayAsNumberedList(const CStringArray& array, LPCTSTR szDelim = _T(". "), int nStart = 1);

	int Split(const CString& sText, CStringArray& aValues, LPCTSTR szSep = _T(""), BOOL bAllowEmpty = FALSE);
 	int Split(const CString& sText, CStringArray& aValues, TCHAR cDelim, BOOL bAllowEmpty = FALSE);
 	BOOL Split(CString& sText, CString& sRest, TCHAR cDelim, BOOL bTrim = TRUE);
	BOOL Split(CString& sText, CString& sRest, LPCTSTR szDelim, BOOL bTrim = TRUE);

	template <class T, class S> 
	int FindT(const T& array, const S& toFind)
	{
		int nItem = array.GetSize();
		const S* pData = array.GetData();

		while (nItem--)
		{
			if (pData[nItem] == toFind)
				return nItem;
		}

		// not found
		return -1;
	}
	
	template <class T, class S> 
	int HasT(const T& array, const S& toFind)
	{
		return (FindT<T, S>(array, toFind) != -1);
	}
	
	int Find(const CStringArray& array, LPCTSTR szItem, 
		BOOL bCaseSensitive = FALSE, BOOL bPartialOK = TRUE);

	template <class T> 
	BOOL RemoveLastT(T& array)
	{
		if (!array.GetSize())
		{
			ASSERT(0);
			return FALSE;
		}

		array.RemoveAt(array.GetSize() - 1);
		return TRUE;
	}
		
	template <class T, class S> 
	BOOL ReplaceLastT(T& array, const S& val)
	{
		if (!array.GetSize())
		{
			ASSERT(0);
			return FALSE;
		}
		
		array[array.GetSize() - 1] = val;
		return TRUE;
	}
	
	typedef int (*STRINGSORTPROC)(const void* pStr1, const void* pStr2);
	void SortArray(CStringArray& array, STRINGSORTPROC pSortProc = NULL);

	typedef int (*DWORDSORTPROC)(const void* pDW1, const void* pDW2);
	void SortArray(CDWordArray& array, DWORDSORTPROC pSortProc = NULL);

	void Copy(const CMapStringToString& mapSrc, CMapStringToString& mapDest);
	BOOL MatchAll(const CMapStringToString& map1, const CMapStringToString& map2);
	BOOL HasKey(const CMapStringToString& map, const CString& sKey);
	CString GetNextKey(const CMapStringToString& map, POSITION& pos);
	int GetKeys(const CMapStringToString& map, CStringArray& aKeys);
	void Append(const CMapStringToString& mapSrc, CMapStringToString& mapDest);

	template <class S, class T> 
	void CopyT(const CMap<S, S, T, T&>& mapSrc, CMap<S, S, T, T&>& mapDest)
	{
		mapDest.RemoveAll();

		if (mapSrc.GetCount())
		{
			POSITION pos = mapSrc.GetStartPosition();
			S key;
			T value;

			while (pos)
			{
				mapSrc.GetNextAssoc(pos, key, value);
				mapDest[key] = value;
			}
		}
	}

	template <class T> 
	void CopyStrT(const CMap<CString, LPCTSTR, T, T&>& mapSrc, CMap<CString, LPCTSTR, T, T&>& mapDest)
	{
		mapDest.RemoveAll();

		if (mapSrc.GetCount())
		{
			POSITION pos = mapSrc.GetStartPosition();
			CString sKey;
			T value;

			while (pos)
			{
				mapSrc.GetNextAssoc(pos, sKey, value);
				mapDest[sKey] = value;
			}
		}
	}

	template <class S, class T> 
	BOOL MatchAllT(const CMap<S, S, T, T&>& map1, const CMap<S, S, T, T&>& map2)
	{
		if (map1.GetCount() != map2.GetCount())
			return FALSE;

		if (map1.GetCount())
		{
			POSITION pos = map1.GetStartPosition();
			S keySrc;
			T valueSrc, valueDest;
			
			while (pos)
			{
				map1.GetNextAssoc(pos, keySrc, valueSrc);
				
				if (!map2.Lookup(keySrc, valueDest) || (valueDest != valueSrc))
					return FALSE;
			}
		}

		return TRUE;
	}

	template <class T> 
	BOOL MatchAllStrT(const CMap<CString, LPCTSTR, T, T&>& map1, const CMap<CString, LPCTSTR, T, T&>& map2)
	{
		if (map1.GetCount() != map2.GetCount())
			return FALSE;

		if (map1.GetCount())
		{
			POSITION pos = map1.GetStartPosition();
			CString sKeySrc;
			T valueSrc, valueDest;

			while (pos)
			{
				map1.GetNextAssoc(pos, sKeySrc, valueSrc);

				if (!map2.Lookup(sKeySrc, valueDest) || (valueDest != valueSrc))
					return FALSE;
			}
		}

		return TRUE;
	}

	CString& MakeUpper(CString& sText);
	CString& MakeLower(CString& sText);
	void MakeUpper(CStringArray& aText);
	void MakeLower(CStringArray& aText);
	CString ToUpper(LPCTSTR szText);
	CString ToLower(LPCTSTR szText);
	CString ToUpper(TCHAR cText);
	CString ToLower(TCHAR cText);
	int NaturalCompare(LPCTSTR szString1, LPCTSTR szString2);
	BOOL LCMapString(CString& sText, DWORD dwMapFlags);

	void MakeQuoted(CString& sText);
	void MakeUnquoted(CString& sText);
	CString GetQuoted(LPCTSTR szText);
	BOOL IsQuoted(LPCTSTR szText);

	TCHAR First(const CString& sText);
	TCHAR Last(const CString& sText);
	TCHAR First(LPCTSTR szText);
	TCHAR Last(LPCTSTR szText);
	TCHAR TrimFirst(CString& sText);
	TCHAR TrimLast(CString& sText);
	CString& Trim(CString& sText, LPCTSTR lpszTargets = NULL);
	BOOL RemoveAt(CString& sText, int nPos);
	BOOL RemovePrefix(CString& sText, LPCTSTR szPrefix, BOOL bTrim = TRUE);
	BOOL RemoveSuffix(CString& sText, LPCTSTR szSuffix, BOOL bTrim = TRUE);
	BOOL IsEmpty(LPCTSTR szText);
	BOOL IsEmpty(LPCSTR szText);

	double Round(double dValue);
	float Round(float fValue);
	double Atof(const CString& sValue);
	CString Format(double dVal, int nDecPlaces = -1, LPCTSTR szTrail = _T(""));
	CString Format(int nVal, LPCTSTR szTrail = _T(""));
	CString Format(DWORD dwVal, LPCTSTR szTrail = _T(""));
	CString FormatCost(double dCost);
	BOOL IsNumber(const CString& sValue);
	BOOL IsSymbol(const CString& sValue);

	const CString& GetLongest(const CString& str1, const CString& str2, BOOL bAsDouble = FALSE);

	void Shuffle(LPTSTR szText);
	CString Shuffle(const CString& str);
	void Shuffle(CStringArray& aStr);

	BOOL IsWorkStationLocked();
	BOOL IsScreenSaverActive();
	BOOL IsScreenReaderActive(BOOL bCheckForMSNarrator = TRUE);
	BOOL IsMSNarratorActive();
	LANGID GetUserDefaultUILanguage();
	LANGID GetUserKeyboardLanguage();
	BOOL IsMetricMeasurementSystem();
	BOOL IsHighContrastActive();

	BOOL ShutdownBlockReasonCreate(HWND hWnd, LPCTSTR szReason);
	BOOL ShutdownBlockReasonDestroy(HWND hWnd);

	void ProcessMsgLoop();
	DWORD GetLastUserInputTick();
	DWORD GetTicksSinceLastUserInput();

	int ParseSearchString(LPCTSTR szLookFor, CStringArray& aWords);
	BOOL FindWord(LPCTSTR szWord, LPCTSTR szText, BOOL bCaseSensitive, BOOL bMatchWholeWord);
	int FilterString(CString& sText, const CString& sFilter);

	BOOL ModKeysArePressed(DWORD dwKeys); 
	BOOL IsKeyPressed(DWORD dwVirtKey);
	BOOL IsCursorKey(DWORD dwVirtKey, DWORD dwKeys = MKC_ANY);
	BOOL IsCursorKeyPressed(DWORD dwKeys = MKC_ANY);
	CString GetKeyName(WORD wVirtKey, BOOL bExtended = FALSE); 

	inline BOOL HasFlag(DWORD dwFlags, DWORD dwFlag) { return (((dwFlags & dwFlag) == dwFlag) ? TRUE : FALSE); }
	BOOL ModifyFlags(DWORD& dwFlags, DWORD dwRemove, DWORD dwAdd = 0);
	BOOL SetFlag(DWORD& dwFlags, DWORD dwFlag, BOOL bSet = TRUE);

	CString MakeKey(const CString& sFormat, int nKeyVal, LPCTSTR szParentKey = NULL);
	CString MakeKey(const CString& sFormat, LPCTSTR szKeyVal, LPCTSTR szParentKey = NULL);
};

#endif // !defined(AFX_MISC_H__4B2FDA3E_63C5_4F52_A139_9512105C3AD4__INCLUDED_)
