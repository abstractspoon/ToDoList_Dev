// Misc.h: interface for the CMisc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MISC_H__4B2FDA3E_63C5_4F52_A139_9512105C3AD4__INCLUDED_)
#define AFX_MISC_H__4B2FDA3E_63C5_4F52_A139_9512105C3AD4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>

//////////////////////////////////////////////////////////////////////

enum 
{ 
	MKS_NONE = 0x00,
	MKS_CTRL = 0x01, 
	MKS_SHIFT = 0x02, 
	MKS_ALT = 0x04, 
};

//////////////////////////////////////////////////////////////////////

enum 
{ 
	MKC_LEFTRIGHT = 0x01, 
	MKC_UPDOWN = 0x02, 
	MKC_ANY = (MKC_LEFTRIGHT | MKC_UPDOWN) 
};

//////////////////////////////////////////////////////////////////////

static const CLIPFORMAT CB_TEXTFORMAT = 
#ifdef _UNICODE
	CF_UNICODETEXT;
#else
	CF_TEXT;
#endif

//////////////////////////////////////////////////////////////////////

#ifndef _ttof
#define _ttof(str) Misc::Atof(str)
#endif

//////////////////////////////////////////////////////////////////////

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

	BOOL MatchAll(const CStringArray& array1, const CStringArray& array2, BOOL bOrderSensitive = FALSE, BOOL bCaseSensitive = FALSE);
	BOOL MatchAny(const CStringArray& array1, const CStringArray& array2, BOOL bCaseSensitive = FALSE, BOOL bWholeWord = FALSE);
	BOOL MatchAll(const CDWordArray& array1, const CDWordArray& array2, BOOL bOrderSensitive = FALSE);
	BOOL MatchAny(const CDWordArray& array1, const CDWordArray& array2);
	
	int RemoveItems(const CStringArray& aItems, CStringArray& aFrom, BOOL bCaseSensitive = FALSE);
	int RemoveEmptyItems(CStringArray& aFrom);
	BOOL RemoveItem(LPCTSTR szItem, CStringArray& aFrom, BOOL bCaseSensitive = FALSE);
	int AddUniqueItems(const CStringArray& aItems, CStringArray& aTo, BOOL bCaseSensitive = FALSE);
	BOOL AddUniqueItem(const CString& sItem, CStringArray& aTo, BOOL bCaseSensitive = FALSE);

	CString GetLongestItem(const CStringArray& array);
	int GetMaximumItemLength(const CStringArray& array);
	int GetFormattedLength(const CStringArray& array, LPCTSTR szSep = NULL, BOOL bIncEmpty = FALSE);
	int GetFormattedLength(const CStringArray& array, TCHAR cSep, BOOL bIncEmpty = FALSE);

	const CString& GetItem(const CStringArray& array, int nItem);

	CString FormatArray(const CDWordArray& array, LPCTSTR szSep = NULL);
	CString FormatArray(const CDWordArray& array, TCHAR cSep);
	CString FormatArray(const CStringArray& array, LPCTSTR szSep = NULL, BOOL bIncEmpty = FALSE);
	CString FormatArray(const CStringArray& array, TCHAR cSep, BOOL bIncEmpty = FALSE);
	CString FormatArrayAsNumberedList(const CStringArray& array, LPCTSTR szDelim = _T(". "), int nStart = 1, BOOL bNumberBlankLines = FALSE);
	int FormatArray(const CDWordArray& aSrc, LPCTSTR lpszFormat, CStringArray& aDest);

	int Split(const CString& sText, CDWordArray& aValues, LPCTSTR szSep = _T(""), BOOL bAllowEmpty = FALSE);
	int Split(const CString& sText, CDWordArray& aValues, TCHAR cDelim, BOOL bAllowEmpty = FALSE);
	int Split(const CString& sText, CStringArray& aValues, LPCTSTR szSep = _T(""), BOOL bAllowEmpty = FALSE, BOOL bPreserveQuotes = FALSE);
 	int Split(const CString& sText, CStringArray& aValues, TCHAR cDelim, BOOL bAllowEmpty = FALSE, BOOL bPreserveQuotes = FALSE);
 	BOOL Split(CString& sText, CString& sRest, TCHAR cDelim, BOOL bTrim = TRUE);
	BOOL Split(CString& sText, CString& sRest, LPCTSTR szDelim, BOOL bTrim = TRUE);

	int SplitLines(const CString& sText, CStringArray& aValues, int nMaxLineLength = -1);
	CString Left(const CString& sText, int nLength, BOOL bNearestWord);

	typedef int (*SORTPROC)(const void* pV1, const void* pV2);

	void SortArray(CStringArray& array, SORTPROC pSortProc = NULL);
	void SortArray(CDWordArray& array, SORTPROC pSortProc = NULL);

	int Copy(const CMapStringToString& mapSrc, CMapStringToString& mapDest);
	int Append(const CMapStringToString& mapSrc, CMapStringToString& mapDest);
	BOOL MatchAll(const CMapStringToString& map1, const CMapStringToString& map2);
	BOOL HasKey(const CMapStringToString& map, const CString& sKey);
	CString GetNextKey(const CMapStringToString& map, POSITION& pos);
	int GetKeys(const CMapStringToString& map, CStringArray& aKeys);

	CString& MakeUpper(CString& sText);
	CString& MakeLower(CString& sText);
	CStringArray& MakeUpper(CStringArray& aText);
	CStringArray& MakeLower(CStringArray& aText);
	CString ToUpper(LPCTSTR szText);
	CString ToLower(LPCTSTR szText);
 	TCHAR ToUpper(TCHAR cText);
 	TCHAR ToLower(TCHAR cText);
	TCHAR ToggleCase(TCHAR cText);
	int NaturalCompare(LPCTSTR szString1, LPCTSTR szString2, BOOL bSortEmptyBelow = FALSE);
	BOOL LCMapString(CString& sText, DWORD dwMapFlags);

	void MakeQuoted(CString& sText, TCHAR cEscapeEmbeddedQuotesWith);
	void MakeUnquoted(CString& sText, TCHAR cUnescapeEmbeddedQuotesWith);
	CString GetQuoted(LPCTSTR szText, TCHAR cEscapeEmbeddedQuotesWith);
	BOOL IsQuoted(LPCTSTR szText);

	int LastIndex(const CString& sText);
	int LastIndex(LPCTSTR szText);

	TCHAR First(const CString& sText);
	TCHAR Last(const CString& sText);
	TCHAR First(LPCTSTR szText);
	TCHAR Last(LPCTSTR szText);
	TCHAR TrimFirst(CString& sText);
	TCHAR TrimLast(CString& sText);
	BOOL TrimFirstIf(TCHAR cTest, CString& sText);
	BOOL TrimTrailingDecimalZeros(CString& sText);
	BOOL TrimLastIf(TCHAR cTest, CString& sText);
	CString& Trim(CString& sText, TCHAR cChar);
	CString& Trim(CString& sText, LPCTSTR lpszTargets = NULL);
	CString& TrimAlpha(CString& sText);
	BOOL RemoveAt(CString& sText, int nPos);
	BOOL RemovePrefix(CString& sText, LPCTSTR szPrefix, BOOL bTrim = TRUE);
	BOOL RemoveSuffix(CString& sText, LPCTSTR szSuffix, BOOL bTrim = TRUE);
	BOOL IsEmpty(LPCTSTR szText);
	BOOL IsEmpty(LPCSTR szText);
	BOOL HasEmpty(const CStringArray& aItems);

	int FindNextOneOf(const CString& sSearchForOneOf, const CString& sSearchIn, BOOL bForward, int nStartPos = -1);
	int FindFirstOf(const CString& sSearchFor, const CString& sSearchIn, BOOL bCaseSensitive);
	int Find(TCHAR cSearchFor, const CString& sSearchIn, BOOL bCaseSensitive = TRUE, int iStart = 0);
	int Find(const CString& sSearchFor, const CString& sSearchIn, BOOL bCaseSensitive = TRUE, BOOL bWholeWord = FALSE, int iStart = 0);
	int Find(LPCTSTR szItem, const CStringArray& array, BOOL bCaseSensitive = FALSE, BOOL bWholeWord = FALSE);
	BOOL Contains(LPCTSTR szItem, const CStringArray& array, BOOL bCaseSensitive = FALSE, BOOL bWholeWord = FALSE);

	int Replace(const CString& sSearchFor, const CString& sReplaceWith, CString& sSearchIn, BOOL bCaseSensitive = FALSE, BOOL bWholeWord = FALSE);
	int Replace(const CString& sSearchFor, const CString& sReplaceWith, CStringArray& aSearchIn, BOOL bCaseSensitive = FALSE, BOOL bWholeWord = FALSE);

	int Round(double dValue);
	double Round(double dValue, int nDecimals);
	double Atof(const CString& sValue);
	BOOL IsNumber(const CString& sValue);
	BOOL IsSymbol(const CString& sValue);

	CString FormatCost(double dCost, LPCTSTR szTrail = NULL);
	CString Format(double dVal, int nDecPlaces = -1, LPCTSTR szTrail = NULL);
	CString Format(int nVal, LPCTSTR szTrail = NULL);
	CString Format(DWORD dwVal, LPCTSTR szTrail = NULL);
	CString Format(LPCTSTR lpszFormat, ...);

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

	int ParseSearchString(LPCTSTR szSearch, CStringArray& aWords);
	int FilterString(CString& sText, const CString& sFilter);

	BOOL ModKeysArePressed(DWORD dwKeys); 
	BOOL IsKeyPressed(DWORD dwVirtKey);
	BOOL IsCursorKey(DWORD dwVirtKey, DWORD dwKeys = MKC_ANY);
	BOOL IsCursorKeyPressed(DWORD dwKeys = MKC_ANY);
	CString GetKeyName(WORD wVirtKey, BOOL bExtended = FALSE); 

	BOOL HasFlag(DWORD dwFlags, DWORD dwFlag);
	BOOL FlagHasChanged(DWORD dwFlag, DWORD dwOldFlags, DWORD dwNewFlags);
	BOOL ModifyFlags(DWORD& dwFlags, DWORD dwRemove, DWORD dwAdd = 0);
	BOOL SetFlag(DWORD& dwFlags, DWORD dwFlag, BOOL bSet = TRUE);

	CString MakeKey(const CString& sFormat, int nKeyVal, LPCTSTR szParentKey = NULL);
	CString MakeKey(const CString& sFormat, LPCTSTR szKeyVal, LPCTSTR szParentKey = NULL);

#ifdef _DEBUG
	void Trace(const CStringArray& array);
#endif
}

// Template helpers
namespace Misc  
{
	template <class T> 
	BOOL MatchAllT(const T& array1, const T& array2, BOOL bOrderSensitive)
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

	template <class T>
	int RemoveItemsT(const CArray<T, T&>& aItems, CArray<T, T&>& aFrom)
	{
		int nRemoved = 0; // counter
		int nItem = array.GetSize();

		while (nItem--)
		{
			const T& item = GetItemT(array, nItem);

			if (RemoveItemT(item, aFrom))
				nRemoved++;
		}

		return nRemoved;
	}

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

	template <class T, class S> 
	BOOL AddUniqueItemT(S item, T& array)
	{
		if (FindT(item, array) != -1)
			return FALSE;

		array.Add(item);
		return TRUE;
	}

	template <class T, class S>
	BOOL RemoveItemT(const S& item, T& array)
	{
		int nFind = FindT(item, array);

		if (nFind == -1)
			return FALSE;

		array.RemoveAt(nFind);
		return TRUE;
	}
	
	template <class T> 
	T IncrementItemT(CArray<T, T&>& array, int nItem)
	{
		ASSERT(nItem >= 0);
		
		if (nItem < 0)
			return;

		if (nItem >= array.GetSize())
			array.SetSize(nItem + 1);

		array[nItem] += 1;

		return array[nItem];
	}

	template <class T> 
	T IncrementItemStrT(CMap<CString, LPCTSTR, T, T&>& map, const CString& key)
	{
		T value = 1;

		if (map.Lookup(key, value))
			value++;

		map[key] = value;

		return value;
	}

	template <class T, class S> 
	S IncrementItemT(CMap<T, T, S, S&>& map, T key)
	{
		S value = 1;

		if (map.Lookup(key, value))
			value++;

		map[key] = value;

		return value;
	}

	template <class T, class S> 
	int FindT(const S& toFind, const T& array)
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
	int HasT(const S& toFind, const T& array)
	{
		return (FindT<T, S>(toFind, array) != -1);
	}


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
	
	template <class T> 
	void SortArrayT(CArray<T, T&>& array, SORTPROC pSortProc)
	{
		qsort(array.GetData(), array.GetSize(), sizeof(T), pSortProc);
	}

	template <class S, class T>
	BOOL HasKeyT(const CMap<S, S, T, T&>& map, const S& key)
	{
		// Avoid constructing empty T
		if (map.GetCount() == 0)
			return FALSE;

		T unused;
		return map.Lookup(key, unused);
	}

	template <class S, class T, class U>
	int GetKeysT(const CMap<S, S, T, T&>& map, U& aKeys)
	{
		aKeys.RemoveAll();

		// Avoid the cost of constructing empty S and T
		if (map.GetCount())
		{
			POSITION pos = map.GetStartPosition();
			S key;
			T unused;

			while (pos)
			{
				map.GetNextAssoc(pos, key, unused);
				aKeys.Add(key);
			}
		}

		return aKeys.GetSize();
	}

	template <class S, class T> 
	int CopyT(const CMap<S, S, T, T&>& mapSrc, CMap<S, S, T, T&>& mapDest)
	{
		mapDest.RemoveAll();

		// Avoid the cost of constructing empty S and T
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

		return mapDest.GetCount();
	}

	template <class S, class T, class VALS>
	int GetValuesT(const CMap<S, S, T, T&>& mapSrc, VALS& aDest)
	{
		aDest.RemoveAll();

		if (mapSrc.GetCount())
		{
			POSITION pos = mapSrc.GetStartPosition();
			S key;
			T value;

			while (pos)
			{
				mapSrc.GetNextAssoc(pos, key, value);
				aDest.Add(value);
			}
		}

		return aDest.GetSize();
	}

	template <class T, class VALS>
	int GetValuesStrT(const CMap<CString, LPCTSTR, T, T&>& mapSrc, VALS& aDest)
	{
		aDest.SetSize();

		if (mapSrc.GetCount())
		{
			POSITION pos = mapSrc.GetStartPosition();
			CString key;
			T value;

			while (pos)
			{
				mapSrc.GetNextAssoc(pos, key, value);
				aDest.Add(value);
			}
		}

		return aDest.GetSize();
	}

	template <class T> 
	int CopyStrT(const CMap<CString, LPCTSTR, T, T&>& mapSrc, CMap<CString, LPCTSTR, T, T&>& mapDest)
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

		return mapDest.GetCount();
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

	template <class T>
	int GetFormattedLength(T tVal, int nDecimals)
	{
		int nNumDigits = 0;

		if (tVal < 0)
		{
			nNumDigits = 1;
			tVal = -tVal;
		}

		while (tVal >= 1)
		{
			tVal /= 10;
			nNumDigits++;
		}

		if (nDecimals > 0)
			nNumDigits += (nDecimals + 1);

		return nNumDigits;
	}

}

#endif // !defined(AFX_MISC_H__4B2FDA3E_63C5_4F52_A139_9512105C3AD4__INCLUDED_)
