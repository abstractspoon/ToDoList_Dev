// Misc.cpp: implementation of the CMisc class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Misc.h"

#include <Lmcons.h>
#include <math.h>
#include <locale.h>
#include <afxtempl.h>
#include <atlconv.h>
#include <afxmt.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////

#if _MSC_VER < 1400
#	define RPC_WSTR LPWSTR
#	define RPC_CSTR LPSTR
#endif

#ifdef _UNICODE
#	define RPC_TSTR RPC_WSTR
#else
#	define RPC_TSTR RPC_CSTR
#endif

#define DBL_QUOTE _T("\"")

//////////////////////////////////////////////////////////////////////

BOOL Misc::Is64BitWindows()
{
#if defined(_WIN64)

    return TRUE;   // 64-bit programs run only on Win64

#elif defined(_WIN32)

    // 32-bit programs run on both 32-bit and 64-bit Windows
	typedef BOOL (WINAPI *PFNISWOW64PROCESS)(HANDLE, PBOOL);
	
	// load user32.dll once only
	static HMODULE hKernel32 = LoadLibrary(_T("kernel32.dll"));
	
	if (hKernel32)
	{
		static PFNISWOW64PROCESS fnIsWow64 = (PFNISWOW64PROCESS)GetProcAddress(hKernel32, "IsWow64Process");
		
		if (fnIsWow64)
		{
			BOOL f64bitOS = FALSE;
			return (fnIsWow64(GetCurrentProcess(), &f64bitOS) && f64bitOS);
		}
	}

#endif

	// all else
    return FALSE;  // 64-bit Windows does not support Win16
}

CString Misc::FormatGetLastError(DWORD dwLastErr)
{
	if (dwLastErr == -1)
		dwLastErr = GetLastError();

	LPTSTR lpMessage = NULL;

	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
				   FORMAT_MESSAGE_FROM_SYSTEM,
				   NULL,               // no source buffer needed
				   dwLastErr,          // error code for this message
				   NULL,               // default language ID
				   (LPTSTR)&lpMessage, // allocated by fcn
				   NULL,               // minimum size of buffer
				   NULL);              // no inserts


	CString sError(lpMessage);
	LocalFree(lpMessage);

	return sError;
}

BOOL Misc::CopyTexttoClipboard(const CString& sText, HWND hwnd, UINT nFormat, BOOL bClear) 
{
	ASSERT(hwnd);

	if (!::OpenClipboard(hwnd)) 
		return FALSE; 
	
	HGLOBAL hglbCopy = NULL; 
	BOOL bResult = FALSE;

	if (bClear)
		::EmptyClipboard(); 
	
	// Allocate a global memory object for the text. 
	hglbCopy = ::GlobalAlloc(GMEM_MOVEABLE, (sText.GetLength() + 1) * sizeof(TCHAR)); 
	
	if (hglbCopy) 
	{ 
		// Lock the handle and copy the text to the buffer. 
		LPTSTR lptstrCopy = (LPTSTR)::GlobalLock(hglbCopy); 
		memcpy(lptstrCopy, (LPVOID)(LPCTSTR)sText, sText.GetLength() * sizeof(TCHAR)); 
		lptstrCopy[sText.GetLength()] = (TCHAR) 0;    // null character 
		
		::GlobalUnlock(hglbCopy); 
		
		// Place the handle on the clipboard. 
		if (nFormat == 0)
		{
#ifndef _UNICODE
			nFormat = CF_TEXT;
#else
			nFormat = CF_UNICODETEXT;
#endif
		}
		
		if (::SetClipboardData(nFormat, hglbCopy) == NULL)
			::GlobalFree(hglbCopy);			
		
		hglbCopy = NULL;
		bResult = TRUE;
	}
	
	::CloseClipboard(); // always

	return bResult;
}

CString Misc::GetClipboardText(UINT nFormat)
{
	if (!::OpenClipboard(NULL)) 
		return _T(""); 

	CString sText;

	if (nFormat == 0)
	{
#ifndef _UNICODE
		nFormat = CF_TEXT;
#else
		nFormat = CF_UNICODETEXT;
#endif
	}
	
	HANDLE hData = ::GetClipboardData(nFormat);
	sText = (LPCTSTR)GlobalLock(hData);

	::GlobalUnlock(hData);
	::CloseClipboard(); // always

	return sText;
}

BOOL Misc::ClipboardHasText()
{
#ifndef _UNICODE
	int nFormat = CF_TEXT;
#else
	int	nFormat = CF_UNICODETEXT;
#endif

	return ClipboardHasFormat(nFormat);
}

BOOL Misc::ClipboardHasFormat(UINT nFormat)
{
	if (!::OpenClipboard(NULL))
		return FALSE;

	UINT nFmt = nFmt = ::EnumClipboardFormats(0);

	while (nFmt) 
	{
		if (nFmt == nFormat)
			break;
		
		nFmt = ::EnumClipboardFormats(nFmt);
	}

	::CloseClipboard(); // always

	return (nFormat && nFmt == nFormat);
}

int Misc::FilterString(CString& sText, const CString& sFilter)
{
	int nPos = sText.GetLength();

	while (nPos--)
	{
		if (sFilter.Find(sText[nPos]) == -1)
			sText.Delete(nPos);
	}

	return sText.GetLength();
}

CString Misc::NewGuid(GUID* pGuid)
{
	GUID guid = { 0 };

	if (RPC_S_OK == UuidCreate((GUID*)&guid))
	{
		CString sGuid;

		if (GuidToString(guid, sGuid))
		{
			if (pGuid)
				*pGuid = guid;

			return sGuid;
		}
	}

	// else
	return _T("");
}

BOOL Misc::IsGuid(LPCTSTR szGuid)
{
	GUID guid;

	return GuidFromString(szGuid, guid);
}

BOOL Misc::GuidFromString(LPCTSTR szGuid, GUID& guid)
{
#pragma comment(lib, "Rpcrt4.lib")

	RPC_STATUS rpcs = UuidFromString((RPC_TSTR)szGuid, &guid);

	if (rpcs != RPC_S_OK)
	{
		NullGuid(guid);
		return FALSE;
	}

	return TRUE;
}

BOOL Misc::GuidToString(const GUID& guid, CString& sGuid)
{
#pragma comment(lib, "Rpcrt4.lib")

	RPC_TSTR pszGuid = NULL;
	
	if (RPC_S_OK == UuidToString((GUID*)&guid, &pszGuid))
		sGuid = (LPCTSTR)pszGuid;
	else
		sGuid.Empty();
	
	RpcStringFree(&pszGuid);
	
	return !sGuid.IsEmpty();
}

BOOL Misc::GuidIsNull(const GUID& guid)
{
	static GUID NULLGUID = { 0 };
	
	return SameGuids(guid, NULLGUID);
}

BOOL Misc::SameGuids(const GUID& guid1, const GUID& guid2)
{
	return (memcmp(&guid1, &guid2, sizeof(GUID)) == 0);
}

void Misc::NullGuid(GUID& guid)
{
   ZeroMemory(&guid, sizeof(guid));
}

void Misc::ProcessMsgLoop()
{
	MSG msg;

	while (::PeekMessage((LPMSG) &msg, NULL, 0, 0, PM_REMOVE))
	{
		if (::IsDialogMessage(msg.hwnd, (LPMSG)&msg))
		{
			// do nothing - its already been done
		}
		else
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
	}
}

DWORD Misc::GetLastUserInputTick()
{
	LASTINPUTINFO lii = { sizeof(LASTINPUTINFO), 0 };
	
	if (GetLastInputInfo(&lii))
		return lii.dwTime;

	// else
	ASSERT(0);
	return 0L;
}

DWORD Misc::GetTicksSinceLastUserInput()
{
	return (GetTickCount() - GetLastUserInputTick());
}

CString Misc::FormatComputerNameAndUser(char cSeparator)
{
	static CString sMachineAndUser;
	
	if (sMachineAndUser.IsEmpty())
		sMachineAndUser.Format(_T("%s%c%s"), GetComputerName(), cSeparator, GetUserName());

	return sMachineAndUser;
}

CString Misc::GetComputerName()
{
	static CString sMachine;

	if (sMachine.IsEmpty()) // init first time only
	{
		DWORD LEN = MAX_COMPUTERNAME_LENGTH + 1;
		
		::GetComputerName(sMachine.GetBuffer(LEN), &LEN);
		sMachine.ReleaseBuffer();
	}

	return sMachine;
}

CString Misc::GetUserName()
{
	static CString sUser;

	if (sUser.IsEmpty()) // init first time only
	{
		DWORD LEN = UNLEN + 1;
		
		::GetUserName(sUser.GetBuffer(LEN), &LEN);
		sUser.ReleaseBuffer();
	}

	return sUser;
}

CString Misc::GetListSeparator()
{
	static CString sSep;
	const int BUFLEN = 10;
	
	if (sSep.IsEmpty()) // init first time only
	{
		GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SLIST, sSep.GetBuffer(BUFLEN), BUFLEN - 1);
		sSep.ReleaseBuffer();
		
		// Trim extra spaces
		Trim(sSep);
		
		// If none found, use a comma
		if (!sSep.GetLength())
			sSep = ',';
	}

	return sSep;
}

CString Misc::FormatArray(const CStringArray& array, TCHAR cSep)
{
	TCHAR szSep[2] = { cSep, 0 };

	return FormatArray(array, szSep);
}

CString Misc::FormatArray(const CStringArray& array, LPCTSTR szSep)
{
	int nCount = array.GetSize();

	switch (nCount)
	{
	case 0:
		return _T("");

	case 1:
		return array[0];
	}

	// All else
	CString sSep(szSep), sText;

	if (!szSep)
		sSep = GetListSeparator() + ' ';

	for (int nItem = 0; nItem < nCount; nItem++)
	{
		const CString& sItem = GetItem(array, nItem);

		if (nItem > 0 && !sItem.IsEmpty())
			sText += sSep;

		sText += sItem;
	}

	return sText;
}

CString Misc::FormatArray(const CDWordArray& array, TCHAR cSep)
{
	TCHAR szSep[2] = { cSep, 0 };

	return FormatArray(array, szSep);
}

CString Misc::FormatArrayAsNumberedList(const CStringArray& array, LPCTSTR szDelim, int nStart)
{
	ASSERT(szDelim);

	int nCount = array.GetSize();
	CString sList, sItem;

	for (int nItem = 0; nItem < nCount; nItem++)
	{
		sItem.Format(_T("%d%s%s\n"), (nItem + nStart), szDelim, GetItem(array, nItem));

		sList += sItem;
	}

	return sList;
}

CString Misc::FormatArray(const CDWordArray& array, LPCTSTR szSep)
{
	int nCount = array.GetSize();

	if (nCount == 0)
		return _T("");

	CString sSep(szSep);

	if (!szSep)
		sSep = GetListSeparator() + ' ';

	CString sText;

	for (int nItem = 0; nItem < nCount; nItem++)
	{
		CString sItem;
		sItem.Format(_T("%lu"), array[nItem]);

		if (nItem > 0)
			sText += sSep;

		sText += sItem;
	}

	return sText;
}

#ifdef _DEBUG
void Misc::Trace(const CStringArray& array)
{
	int nCount = array.GetSize();

	for (int nItem = 0; nItem < nCount; nItem++)
		TRACE(_T("%s, "), GetItem(array, nItem));

	TRACE(_T("\n"));
}
#endif

int Misc::GetTotalLength(const CStringArray& array)
{
	int nLength = 0;
	int nItem = array.GetSize();

	while (nItem--)
	{
		const CString& sItem = GetItem(array, nItem);
		nLength += sItem.GetLength();
	}

	return nLength;
}

BOOL Misc::Split(CString& sText, CString& sRest, TCHAR cDelim, BOOL bTrim)
{
	TCHAR szDelim[] = { cDelim, 0 };

	return Split(sText, sRest, szDelim, bTrim);
}

BOOL Misc::Split(CString& sText, CString& sRest, LPCTSTR szDelim, BOOL bTrim)
{
	if (sText.IsEmpty())
		return FALSE;
	
	int nDelim = sText.Find(szDelim);

	if (nDelim == -1)
		return FALSE;

	// else
	sRest = sText.Mid(nDelim + lstrlen(szDelim));
	sText = sText.Left(nDelim);

	if (bTrim)
	{
		Trim(sText);
		Trim(sRest);
	}

	return TRUE;
}

CString& Misc::Trim(CString& sText, LPCTSTR lpszTargets)
{
	if (lpszTargets)
	{
		sText.TrimLeft(lpszTargets);
		sText.TrimRight(lpszTargets);
	}
	else
	{
		sText.TrimLeft();
		sText.TrimRight();
	}

	return sText;
}

BOOL Misc::RemovePrefix(CString& sText, LPCTSTR szPrefix, BOOL bTrim)
{
	if (IsEmpty(szPrefix))
		return FALSE;

	CString sTemp(sText);
	sTemp.TrimLeft();

	int nPos = sTemp.Find(szPrefix);

	if (nPos == 0)
	{
		sText = sTemp.Mid(lstrlen(szPrefix));

		if (bTrim)
			sText.TrimLeft();

		return TRUE;
	}

	// else
	return FALSE;
}

BOOL Misc::RemoveSuffix(CString& sText, LPCTSTR szSuffix, BOOL bTrim)
{
	if (IsEmpty(szSuffix))
		return FALSE;

	CString sTemp(sText);
	sTemp.TrimRight();

	int nPos = sTemp.Find(szSuffix);

	if ((nPos != -1) && (nPos == (sTemp.GetLength() - lstrlen(szSuffix))))
	{
		sText = sTemp.Left(nPos);

		if (bTrim)
			sText.TrimRight();

		return TRUE;
	}

	// else
	return FALSE;
}

BOOL Misc::RemoveAt(CString& sText, int nPos)
{
	int nLen = sText.GetLength();
	
	if (nPos < 0 || nPos >= nLen)
	{
		ASSERT(0);
		return FALSE;
	}
	
	// Shift all chars beyond nPos down one position
	LPTSTR szText = (sText.GetBuffer(nLen) + nPos);
	LPTSTR szNext = (szText + 1);
	
	while (*szNext)
		*(szText++) = *(szNext++);
	
	sText.ReleaseBuffer(nLen - 1);
	return TRUE;
}

BOOL Misc::IsEmpty(LPCTSTR szText) 
{ 
	return (!szText || !szText[0]); 
}

BOOL Misc::IsEmpty(LPCSTR szText) 
{ 
	return (!szText || !szText[0]); 
}

TCHAR Misc::First(const CString& sText)
{
	if (sText.IsEmpty())
	{
		ASSERT(0);
		return 0;
	}

	// else
	return sText[0];
}

TCHAR Misc::Last(const CString& sText)
{
	if (sText.IsEmpty())
	{
		ASSERT(0);
		return 0;
	}

	// else
	return sText[sText.GetLength() - 1];
}

TCHAR Misc::First(LPCTSTR szText)
{
	if (IsEmpty(szText))
	{
		ASSERT(0);
		return 0;
	}

	// else
	return szText[0];
}

TCHAR Misc::Last(LPCTSTR szText)
{
	if (IsEmpty(szText))
	{
		ASSERT(0);
		return 0;
	}

	// else
	return szText[lstrlen(szText) - 1];
}

TCHAR Misc::TrimFirst(CString& sText)
{
	TCHAR nFirst = First(sText);
	
	if (nFirst != 0)
		sText = sText.Mid(1);
	
	return nFirst;
}

TCHAR Misc::TrimLast(CString& sText)
{
	TCHAR nLast = Last(sText);
	
	if (nLast != 0)
		sText = sText.Left(sText.GetLength() - 1);
	
	return nLast;
}

int Misc::Split(const CString& sText, CStringArray& aValues, TCHAR cDelim, BOOL bAllowEmpty)
{
	TCHAR szSep[2] = { cDelim, 0 };

	return Split(sText, aValues, szSep, bAllowEmpty);
}

int Misc::Split(const CString& sText, CStringArray& aValues, LPCTSTR szSep, BOOL bAllowEmpty)
{
	aValues.RemoveAll();
	
	if (sText.IsEmpty())
		return 0;

	CString sSep(szSep);
	
	if (sSep.IsEmpty())
		sSep = GetListSeparator();
	
	// parse on separator unless enclosed in double-quotes
	int nLen = sText.GetLength(), nSepLen = sSep.GetLength();
	BOOL bInQuotes = FALSE, bAddWord = FALSE;
	CString sWord;

	for (int nPos = 0; nPos < nLen; nPos++)
	{
		TCHAR chr = sText[nPos];

		// check if we're at the beginning of a separator string
		if (chr == sSep[0] && (nSepLen == 1 || sText.Find(sSep, nPos) == nPos))
		{
			if (bInQuotes)
				sWord += sSep;
			else
				bAddWord = TRUE;

			nPos += nSepLen - 1; // minus 1 because the loop also increments
		}
		else if (chr == '\"') // double quote
		{
			// flip bInQuotes
			bInQuotes = !bInQuotes;

			// Workaround to handle Quoted empty string as last word
			if (!bInQuotes && bAllowEmpty && sWord.IsEmpty())
				sWord = ' ';
		}
		else // everything else
		{
			sWord += chr;

			// also if its the last char then add it
			bAddWord = (nPos == nLen - 1);
		}

		if (bAddWord)
		{
			Trim(sWord);
			
			if (bAllowEmpty || !sWord.IsEmpty())
				aValues.Add(sWord);
			
			sWord.Empty(); // next word
			bAddWord = FALSE;
		}
	}

	// if the string ends with a separator and allow empty then add an empty string
	if (!sWord.IsEmpty())
	{
		Trim(sWord);
		aValues.Add(sWord);
	}
	else if (bAllowEmpty && nLen)
	{
		if (sText.Find(sSep, nLen - nSepLen) == (nLen - nSepLen))
			aValues.Add(sWord);
	}

	return aValues.GetSize();
}

BOOL Misc::MatchAll(const CStringArray& array1, const CStringArray& array2, BOOL bOrderSensitive, BOOL bCaseSensitive)
{
	int nSize1 = array1.GetSize();
	int nSize2 = array2.GetSize();

	if (nSize1 != nSize2)
		return 0;

	if (bOrderSensitive)
	{
		for (int nItem = 0; nItem < nSize1; nItem++)
		{
			const CString& sItem1 = GetItem(array1, nItem);
			const CString& sItem2 = GetItem(array2, nItem);

			// check for non-equality
			if (bCaseSensitive)
			{
				if (sItem1 != sItem2)
					return FALSE;
			}
			else 
			{
				if (NaturalCompare(sItem1, sItem2) != 0)
					return FALSE;
			}
		}

		return TRUE;
	}

	// else order not important
	for (int nItem1 = 0; nItem1 < nSize1; nItem1++)
	{
		const CString& sItem1 = GetItem(array1, nItem1);

		// look for matching item in array2
		if (Find(array2, sItem1, bCaseSensitive, FALSE) == -1)
		{
			// no-match found == not the same
			return FALSE;
		}
	}

	return TRUE;
}

BOOL Misc::MatchAny(const CStringArray& array1, const CStringArray& array2, BOOL bCaseSensitive, BOOL bPartialOK) 
{
	int nSize1 = array1.GetSize();

	for (int nItem1 = 0; nItem1 < nSize1; nItem1++)
	{
		const CString& sItem1 = GetItem(array1, nItem1);

		// look for matching item
		if (Find(array2, sItem1, bCaseSensitive, bPartialOK) != -1)
			return TRUE;
	}
	
	return FALSE;
}

BOOL Misc::MatchAll(const CDWordArray& array1, const CDWordArray& array2, 
					BOOL bOrderSensitive)
{
	int nSize1 = array1.GetSize();
	int nSize2 = array2.GetSize();

	if (nSize1 != nSize2)
		return 0;

	if (bOrderSensitive)
	{
		for (int nItem = 0; nItem < nSize1; nItem++)
		{
			if (array1[nItem] != array2[nItem])
				return FALSE;
		}

		return TRUE;
	}

	// else order not important
	for (int nItem1 = 0; nItem1 < nSize1; nItem1++)
	{
		// look for matching item in array2
		if (FindT(array2, array1[nItem1]) == -1)
		{
			// no-match found == not the same
			return FALSE;
		}
	}

	return TRUE;
}

BOOL Misc::MatchAny(const CDWordArray& array1, const CDWordArray& array2)
{
	int nSize1 = array1.GetSize();

	for (int nItem1 = 0; nItem1 < nSize1; nItem1++)
	{
		// look for matching item
		if (FindT(array2, array1[nItem1]) != -1)
			return TRUE;
	}
	
	return FALSE;
}

int Misc::Find(const CStringArray& array, LPCTSTR szItem, BOOL bCaseSensitive, BOOL bPartialOK)
{
	ASSERT (szItem);

	if (szItem == NULL)
		return -1;

	int nSize = array.GetSize();

	for (int nItem = 0; nItem < nSize; nItem++)
	{
		// look for matching item
		const CString& sArrItem = GetItem(array, nItem);

		// special case: empty item
		if (IsEmpty(szItem))
		{
			if (sArrItem.IsEmpty())
				return nItem;
		}
		else if (bCaseSensitive)
		{
			if (bPartialOK)
			{
				if (sArrItem.Find(szItem) != -1)
					return nItem;
			}
			else
			{
				if (sArrItem == szItem)
					return nItem;
			}
		}
		else // case insensitive
		{
			if (bPartialOK)
			{
				if (FindWord(szItem, sArrItem, FALSE, FALSE))
					return nItem;
			}
			else
			{
				if (NaturalCompare(sArrItem, szItem) == 0)
					return nItem;
			}
		}
	}

	return -1;
}

const CString& Misc::GetItem(const CStringArray& array, int nItem)
{
	ASSERT(nItem >= 0 && nItem < array.GetSize());

	if (nItem < 0 || nItem >= array.GetSize())
	{
		static CString sDummy;
		return sDummy;
	}

	return array.GetData()[nItem];
}

int Misc::RemoveEmptyItems(CStringArray& aFrom)
{
	int nRemoved = 0; // counter
	int nItem = aFrom.GetSize();

	while (nItem--)
	{
		CString& sItem = aFrom[nItem];
		Trim(sItem);

		if (sItem.IsEmpty())
		{
			aFrom.RemoveAt(nItem);
			nRemoved++;
		}
	}

	return nRemoved;
}

int Misc::RemoveItems(const CStringArray& array, CStringArray& aFrom, BOOL bCaseSensitive)
{
	int nRemoved = 0; // counter
	int nItem = array.GetSize();

	while (nItem--)
	{
		const CString& sItem = GetItem(array, nItem);

		if (RemoveItem(sItem, aFrom, bCaseSensitive))
			nRemoved++;
	}

	return nRemoved;
}

BOOL Misc::RemoveItem(LPCTSTR szItem, CStringArray& aFrom, BOOL bCaseSensitive)
{
	int nFind = Find(aFrom, szItem, bCaseSensitive, FALSE);
	
	if (nFind != -1)
		aFrom.RemoveAt(nFind);

	return (nFind != -1);
}

int Misc::AddUniqueItems(const CStringArray& array, CStringArray& aTo, BOOL bCaseSensitive)
{
	int nAdded = 0; // counter
	int nSize = array.GetSize();
	
	for (int nItem = 0; nItem < nSize; nItem++)
	{
		const CString& sItem = GetItem(array, nItem);

		if (AddUniqueItem(sItem, aTo, bCaseSensitive))
			nAdded++;
	}

	return nAdded;
}

BOOL Misc::AddUniqueItem(const CString& sItem, CStringArray& aTo, BOOL bCaseSensitive)
{
	if (sItem.IsEmpty())
		return FALSE;

	int nFind = Find(aTo, sItem, bCaseSensitive, FALSE);

	if (nFind == -1) // doesn't already exist
	{
		aTo.Add(sItem);
		return TRUE;
	}

	return FALSE; // not added
}

CString Misc::GetAM()
{
	static CString sAM;
	const int BUFLEN = 10;

	if (sAM.IsEmpty()) // init first time only
	{
		GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_S1159, sAM.GetBuffer(BUFLEN), BUFLEN - 1);
		sAM.ReleaseBuffer();
	}

	return sAM;
}

CString Misc::GetPM()
{
	static CString sPM;
	const int BUFLEN = 10;

	if (sPM.IsEmpty()) // init first time only
	{
		GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_S2359, sPM.GetBuffer(BUFLEN), BUFLEN - 1);
		sPM.ReleaseBuffer();
	}

	return sPM;
}

CString Misc::GetTimeFormat(BOOL bIncSeconds)
{
	static CString sFormat;
	const int BUFLEN = 100;

	if (sFormat.IsEmpty()) // init first time only
	{
		GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_STIMEFORMAT, sFormat.GetBuffer(BUFLEN), BUFLEN - 1);
		sFormat.ReleaseBuffer();
	}

	if (!bIncSeconds)
	{
		CString sTemp(sFormat);

		if (!sTemp.Replace(GetTimeSeparator() + _T("ss"), _T("")))
			sTemp.Replace(GetTimeSeparator() + _T("s"), _T(""));

		return sTemp;
	}

	return sFormat;
}

CString Misc::GetTimeSeparator()
{
	static CString sSep;
	const int BUFLEN = 10;

	if (sSep.IsEmpty()) // init first time only
	{
		GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_STIME , sSep.GetBuffer(BUFLEN), BUFLEN - 1);
		sSep.ReleaseBuffer();

		// Trim extra spaces
		Trim(sSep);
		
		// If none found, use a dot
		if (!sSep.GetLength())
			sSep = ':';
	}

	return sSep;
}

CString Misc::GetDateSeparator()
{
	static CString sSep;
	const int BUFLEN = 10;

	if (sSep.IsEmpty()) // init first time only
	{
		GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SDATE , sSep.GetBuffer(BUFLEN), BUFLEN - 1);
		sSep.ReleaseBuffer();

		// Trim extra spaces
		Trim(sSep);
		
		// If none found, use a slash
		if (!sSep.GetLength())
			sSep = '/';
	}

	return sSep;
}

CString Misc::GetShortDateFormat(BOOL bIncDOW)
{
	static CString sFormat;
	const int BUFLEN = 100;

	if (sFormat.IsEmpty()) // init first time only
	{
		GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SSHORTDATE, sFormat.GetBuffer(BUFLEN), BUFLEN - 1);
		sFormat.ReleaseBuffer();
	}

	if (bIncDOW)
	{
		CString sTemp = _T("ddd ") + sFormat;
		return sTemp;
	}

	return sFormat;
}

CString Misc::GetDecimalSeparator()
{
	static CString sSep;
	const int BUFLEN = 10;

	if (sSep.IsEmpty()) // init first time only
	{
		GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SDECIMAL, sSep.GetBuffer(BUFLEN), BUFLEN - 1);
		sSep.ReleaseBuffer();

		// Trim extra spaces
		Trim(sSep);
		
		// If none found, use a dot
		if (!sSep.GetLength())
			sSep = '.';
	}

	return sSep;
}

BOOL Misc::IsMetricMeasurementSystem()
{
	const int BUFLEN = 2;
	CString sSystem;
	
	GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_IMEASURE, sSystem.GetBuffer(BUFLEN), BUFLEN - 1);
	sSystem.ReleaseBuffer(BUFLEN - 1);
	
	return (_ttoi(sSystem) == 0);
}

CString& Misc::EncodeAsMultiByte(CString& sUnicode, UINT nCodePage)
{
	// calc number of unicode chars in string
#ifdef _UNICODE
	int nLen = sUnicode.GetLength();
#else
	int nLen = ((sUnicode.GetLength() + 1) / sizeof(WCHAR));
#endif
	
	char* szMB = WideToMultiByte((LPCWSTR)(LPCTSTR)sUnicode, nLen, nCodePage);
	// nLen is now the number of returned multibyte chars
	
	// calc number of bytes required to hold multibyte chars
	int nBytes = nLen;
	
#ifdef _UNICODE
	// calc number of chars required to hold multibyte chars
	nLen = ((nLen + 1) / sizeof(WCHAR));
#endif
	
	LPTSTR szOutput = sUnicode.GetBuffer(nLen);
	
	ZeroMemory(szOutput, nBytes);
	CopyMemory(szOutput, szMB, nBytes);
	
	sUnicode.ReleaseBuffer(nLen);
	delete [] szMB;
	
	return sUnicode;
}

CString& Misc::EncodeAsUnicode(CString& sMultibyte, UINT nCodePage)
{
	// calc number of multibyte chars in string
	int nLen = (sMultibyte.GetLength() * sizeof(TCHAR));
	
	WCHAR* szUnicode = MultiByteToWide((LPCSTR)(LPCTSTR)sMultibyte, nLen, nCodePage);
	// nLen is now the number of returned unicode chars
	
	// calc number of bytes required to hold unicode chars
	int nBytes = (nLen * sizeof(WCHAR));
	
	// calc number of chars needed to hold unicode chars
#ifndef _UNICODE
	nLen *= sizeof(WCHAR);
#endif
	
	LPTSTR szOutput = sMultibyte.GetBuffer(nLen);
	
	ZeroMemory(szOutput, nBytes); 
	CopyMemory(szOutput, szUnicode, nBytes);
	
	sMultibyte.ReleaseBuffer(nLen);
	delete [] szUnicode;
	
	return sMultibyte;
}

char* Misc::WideToMultiByte(const WCHAR* szFrom, UINT nCodePage)
{
	int nLen = wcslen(szFrom);
	return WideToMultiByte(szFrom, nLen, nCodePage);
}

char* Misc::WideToMultiByte(const WCHAR* szFrom, int& nLength, UINT nCodePage)
{
	int clen = WideCharToMultiByte(nCodePage, 0, szFrom, nLength, NULL, 0, NULL, NULL);

	// if clen == 0, some unknown codepage was probably used.
	ASSERT((nLength && clen) || (!nLength && !clen));

	if (clen == 0 && nLength) 
		return NULL;

	char* cTo = new char[clen+1];
	cTo[clen] = 0; // null terminator

	if (clen)
		WideCharToMultiByte(nCodePage, 0, szFrom, nLength, cTo, clen, NULL, NULL);

	nLength = clen;

	return cTo;
}

WCHAR* Misc::MultiByteToWide(const char* szFrom, UINT nCodePage)
{
	int nLen = strlen(szFrom);
	return MultiByteToWide(szFrom, nLen, nCodePage);
}

WCHAR* Misc::MultiByteToWide(const char* szFrom, int& nLength, UINT nCodepage)
{
	// Use api convert routine
	int wlen = MultiByteToWideChar(nCodepage, 0, szFrom, nLength, NULL, 0);

	// if wlen == 0, some unknown codepage was probably used.
	ASSERT((nLength && wlen) || (!nLength && !wlen));

	if (wlen == 0 && nLength) 
		return NULL;

	WCHAR* wTo = new WCHAR[wlen+1];
	wTo[wlen] = 0; // null terminator

	if (wlen)
		MultiByteToWideChar(nCodepage, 0, szFrom, nLength, wTo, wlen);

	nLength = wlen;

	return wTo;
}

double Misc::Round(double dValue)
{
	if (dValue > 0)
	{
		if ((dValue - (int)dValue) > 0.5)
			return ceil(dValue);
		else
			return floor(dValue);
	}
	else
	{
		if ((dValue - (int)dValue) > -0.5)
			return floor(dValue);
		else
			return ceil(dValue);
	}
}

float Misc::Round(float fValue)
{
	return (float)Round((double)fValue);
}

BOOL Misc::IsNumber(const CString& sValue)
{
	if (sValue.IsEmpty())
		return FALSE;

	static const CString DELIMS(_T("+-."));

	for (int nChar = 0; nChar < sValue.GetLength(); nChar++)
	{
		TCHAR chr = sValue[nChar];

		if (!_istdigit(chr) && DELIMS.Find(chr) == -1)
			return FALSE;
	}

	return TRUE;
}

BOOL Misc::IsSymbol(const CString& sValue)
{
	if (sValue.IsEmpty())
		return FALSE;

	for (int nChar = 0; nChar < sValue.GetLength(); nChar++)
	{
		TCHAR chr = sValue[nChar];

		if (_istalnum(chr)) // alpha-Numeric
			return FALSE;
	}

	return TRUE;
}

// From: http://www.geeksforgeeks.org/shuffle-a-given-array/
void Misc::Shuffle(LPTSTR szText)
{
	// Use a different seed value so that we don't get same
	// result each time we run this program
	srand((unsigned int)time(NULL));

	// Start from the last element and swap one by one. We don't
	// need to run for the first element that's why i > 0
	int nLen = lstrlen(szText);

	for (int i = (nLen - 1); i > 0; i--)
	{
		// Pick a random index from 0 to i
		int j = (rand() % (i+1));

		// Swap arr[i] with the element at random index
		TCHAR cTemp = szText[i];
		szText[i] = szText[j];
		szText[j] = cTemp;
	}
}

CString Misc::Shuffle(const CString& str)
{
	CString sShuffle(str);
	Shuffle(sShuffle.GetBuffer(str.GetLength()));

	sShuffle.ReleaseBuffer();
	return sShuffle;
}

void Misc::Shuffle(CStringArray& aStr)
{
	int nItem = aStr.GetSize();

	while (nItem--)
		aStr[nItem] = Shuffle(aStr[nItem]);
}

const CString& Misc::GetLongest(const CString& str1, const CString& str2, BOOL bAsDouble)
{
	if (bAsDouble)
	{
		// check as numbers
		double dStr1 = fabs(Atof(str1)), dStr2 = fabs(Atof(str2));

		return (dStr1 > dStr2) ? str1 : str2;
	}

	// all else
	return (str1.GetLength() > str2.GetLength()) ? str1 : str2;
}

double Misc::Atof(const CString& sValue)
{
	if (sValue.IsEmpty())
		return 0.0;

	// needs special care to handle decimal point properly
	// especially since we've no way of knowing how it is encoded.
	// so we assume that if a period is present then it's encoded
	// in 'english' else it's in native format
	char* szLocale = _strdup(setlocale(LC_NUMERIC, NULL));
	
	if (sValue.Find('.') != -1)
		setlocale(LC_NUMERIC, "English");
	else
		setlocale(LC_NUMERIC, "");

	double dVal = _tcstod(sValue, NULL);

	// restore locale
	setlocale(LC_NUMERIC, szLocale);
	free(szLocale);
	
	return dVal; 
}

BOOL Misc::ShutdownBlockReasonCreate(HWND hWnd, LPCTSTR szReason)
{
	typedef BOOL (WINAPI *PFNSHUTDOWNBLOCKREASONCREATE)(HWND, LPCWSTR);

	// load user32.dll once only
	static HMODULE hUser32 = LoadLibrary(_T("user32.dll"));

	if (hUser32)
	{
		static PFNSHUTDOWNBLOCKREASONCREATE fnCreate = (PFNSHUTDOWNBLOCKREASONCREATE)GetProcAddress(hUser32, "ShutdownBlockReasonCreate");

		if (fnCreate)
		{
#ifdef _UNICODE
			LPCWSTR wszReason = szReason;
#else
			int nLength = lstrlen(szReason);
			LPWSTR wszReason = MultiByteToWide(szReason, nLength);
#endif

			BOOL bRes = fnCreate(hWnd, wszReason);

#ifndef _UNICODE
			delete [] wszReason;
#endif
			return bRes;
		}
	}

	// must be < XP
	return FALSE;
}

BOOL Misc::ShutdownBlockReasonDestroy(HWND hWnd)
{
	typedef BOOL (WINAPI *PFNSHUTDOWNBLOCKREASONDESTROY)(HWND);

	// load user32.dll once only
	static HMODULE hUser32 = LoadLibrary(_T("user32.dll"));

	if (hUser32)
	{
		static PFNSHUTDOWNBLOCKREASONDESTROY fnDestroy = (PFNSHUTDOWNBLOCKREASONDESTROY)GetProcAddress(hUser32, "ShutdownBlockReasonDestroy");

		if (fnDestroy)
			return fnDestroy(hWnd);
	}

	// must be < XP
	return FALSE;
}

BOOL Misc::IsWorkStationLocked()
{
	HDESK hDesk = OpenInputDesktop(0, FALSE, 0);
	
	if (hDesk)
		CloseDesktop(hDesk);
	
	return (hDesk == NULL);
}

BOOL Misc::IsScreenSaverActive()
{
	BOOL bActive = FALSE;
	
	return (::SystemParametersInfo(SPI_GETSCREENSAVERRUNNING, 0, &bActive, 0) && bActive); 
}

BOOL Misc::IsScreenReaderActive(BOOL bCheckForMSNarrator)
{
    BOOL bActive = FALSE;

	if (::SystemParametersInfo(SPI_GETSCREENREADER, 0, &bActive, 0) && bActive)
		return TRUE;

	// else
	if (bCheckForMSNarrator && IsMSNarratorActive())
		return TRUE;

	// else
	return FALSE;
}

BOOL Misc::IsMSNarratorActive()
{
	CMutex mutex(FALSE, _T("NarratorRunning"));

	return (GetLastError() == ERROR_ALREADY_EXISTS);
}

BOOL Misc::IsHighContrastActive()
{
	HIGHCONTRAST hc = { sizeof(hc), 0 };

	if (::SystemParametersInfo(SPI_GETHIGHCONTRAST, 0, &hc, 0))
		return (hc.dwFlags & HCF_HIGHCONTRASTON);

	return FALSE;
}

LANGID Misc::GetUserDefaultUILanguage()
{
	typedef LANGID (WINAPI *FNGETUSERDEFAULTUILANGUAGE)(VOID);

	// must link dynamically to kernel32 else problem with win95/NT4
	static HMODULE hLib = LoadLibrary(_T("kernel32.dll"));
	LANGID nLangID = 0;

	if (hLib)
	{
		FNGETUSERDEFAULTUILANGUAGE pFN = (FNGETUSERDEFAULTUILANGUAGE)GetProcAddress(hLib, "GetUserDefaultUILanguage");

		if (pFN)
			nLangID = pFN();
	}

	return nLangID;
//	return ::GetUserDefaultUILanguage();
}

LANGID Misc::GetUserKeyboardLanguage()
{
	return LOWORD(::GetKeyboardLayout(0));
}

CString Misc::GetDefCharset()
{
	CString sDefCharset;
	GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_IDEFAULTANSICODEPAGE, sDefCharset.GetBuffer(7), 6);
	sDefCharset.ReleaseBuffer();

	return sDefCharset;
}

BOOL Misc::LCMapString(CString& sText, DWORD dwMapFlags)
{
	int nLen = sText.GetLength();
	CString sMapped;

	WORD wLangID = MAKELANGID(LANG_NEUTRAL, SUBLANG_SYS_DEFAULT);
	DWORD dwLCID = MAKELCID(wLangID, SORT_DEFAULT);

	BOOL bRes = ::LCMapString(dwLCID, 
								dwMapFlags, 
								(LPCTSTR)sText, 
								-1, 
								sMapped.GetBuffer(nLen+1), 
								nLen+1);
	sMapped.ReleaseBuffer();

	if (bRes)
		sText = sMapped;

	return bRes;
}

CString& Misc::MakeUpper(CString& sText)
{
	if (!LCMapString(sText, LCMAP_UPPERCASE | LCMAP_LINGUISTIC_CASING))
		sText.MakeUpper();

	return sText;
}

CString& Misc::MakeLower(CString& sText)
{
	if (!LCMapString(sText, LCMAP_LOWERCASE | LCMAP_LINGUISTIC_CASING))
		sText.MakeLower();

	return sText;
}

CString Misc::ToUpper(LPCTSTR szText)
{
	CString sText(szText);
	MakeUpper(sText);

	return sText;
}

CString Misc::ToLower(LPCTSTR szText)
{
	CString sText(szText);
	MakeLower(sText);
	
	return sText;
}

CString Misc::ToUpper(TCHAR cText)
{
	TCHAR szText[] = { cText, 0 };

	return ToUpper(szText);
}

CString Misc::ToLower(TCHAR cText)
{
	TCHAR szText[] = { cText, 0 };
	
	return ToLower(szText);
}

void Misc::MakeUpper(CStringArray& aText)
{
	int nItem = aText.GetSize();
	
	while (nItem--)
		MakeUpper(aText[nItem]);
}

void Misc::MakeLower(CStringArray& aText)
{
	int nItem = aText.GetSize();

	while (nItem--)
		MakeLower(aText[nItem]);
}

BOOL Misc::FindWord(LPCTSTR szWord, LPCTSTR szText, BOOL bCaseSensitive, BOOL bMatchWholeWord)
{
	CString sWord(szWord), sText(szText);
	
	if (sWord.GetLength() > sText.GetLength())
		return FALSE;
	
	Trim(sWord);
	
	if (!bCaseSensitive)
	{
		MakeUpper(sWord);
		MakeUpper(sText);
	}
	
	int nFind = sText.Find(sWord);
	
	if (nFind == -1)
	{
		return FALSE;
	}
	else if (bMatchWholeWord) // test whole word
	{
		const CString DELIMS("()-\\/{}[]:;,. ?\"'");
		
		// prior and next chars must be delimeters
		TCHAR cPrevChar = 0, cNextChar = 0;
		
		// prev
		if (nFind == 0) // word starts at start
			cPrevChar = ' '; // known delim
		else
			cPrevChar = sText[nFind - 1];
		
		// next
		if ((nFind + sWord.GetLength()) < sText.GetLength())
			cNextChar = sText[nFind + sWord.GetLength()];
		else
			cNextChar = ' '; // known delim
		
		if (DELIMS.Find(cPrevChar) == -1 || DELIMS.Find(cNextChar) == -1)
			return FALSE;
	}
	
	return TRUE;
}

int Misc::ParseSearchString(LPCTSTR szLookFor, CStringArray& aWords)
{
	aWords.RemoveAll();
	
	// parse on spaces unless enclosed in double-quotes
	int nLen = lstrlen(szLookFor);
	BOOL bInQuotes = FALSE, bAddWord = FALSE;
	CString sWord;
	
	for (int nPos = 0; nPos < nLen; nPos++)
	{
		switch (szLookFor[nPos])
		{
		case ' ': // word break
			if (bInQuotes)
				sWord += szLookFor[nPos];
			else
				bAddWord = TRUE;
			break;
			
		case '\"':
			// whether its the start or end we add the current word
			// and flip bInQuotes
			bInQuotes = !bInQuotes;
			bAddWord = TRUE;
			break;
			
		default: // everything else
			sWord += szLookFor[nPos];
			
			// also if its the last char then add it
			bAddWord = (nPos == nLen - 1);
			break;
		}
		
		if (bAddWord)
		{
			Trim(sWord);
			
			if (!sWord.IsEmpty())
				aWords.Add(sWord);
			
			sWord.Empty(); // next word
		}
	}
	
	return aWords.GetSize();
}

CString Misc::Format(double dVal, int nDecPlaces, LPCTSTR szTrail)
{
	return (Format(dVal, nDecPlaces) + szTrail);
}

CString Misc::Format(double dVal, int nDecPlaces)
{
	char* szLocale = _strdup(setlocale(LC_NUMERIC, NULL)); // current locale
	setlocale(LC_NUMERIC, ""); // local default

	CString sValue;

	if (nDecPlaces < 0)
		sValue.Format(_T("%f"), dVal);
	else
		sValue.Format(_T("%.*f"), nDecPlaces, dVal);
				
	// restore locale
	setlocale(LC_NUMERIC, szLocale);
	free(szLocale);

	return sValue;
}

CString Misc::Format(int nVal, LPCTSTR szTrail)
{
	return (Format(nVal) + szTrail);
}

CString Misc::Format(DWORD dwVal, LPCTSTR szTrail)
{
	return (Format(dwVal) + szTrail);
}

CString Misc::Format(int nVal)
{
	CString sValue;
	sValue.Format(_T("%ld"), nVal);
	
	return sValue;
}

CString Misc::Format(DWORD dwVal)
{
	CString sValue;
	sValue.Format(_T("%lu"), dwVal);
	
	return sValue;
}

CString Misc::FormatCost(double dCost)
{
	CString sValue;
	sValue.Format(_T("%.6f"), dCost);

	char* szLocale = _strdup(setlocale(LC_NUMERIC, NULL)); // current locale
	setlocale(LC_NUMERIC, ""); // local default

	const UINT BUFSIZE = 100;
	TCHAR szCost[BUFSIZE + 1];

	GetCurrencyFormat(NULL, 0, sValue, NULL, szCost, BUFSIZE);
	sValue = szCost;
				
	// restore locale
	setlocale(LC_NUMERIC, szLocale);
	free(szLocale);

	return sValue;
}

CString Misc::GetKeyName(WORD wVirtKeyCode, BOOL bExtended)
{
	WORD wScanCode = (WORD)MapVirtualKey(wVirtKeyCode, 0);

	if (wScanCode)
	{
		const int KEYNAMELEN = 64;
		TCHAR szKeyName[64] = { 0 };

		// build lParam to send to GetKeyNameText
		LPARAM lParam = (wScanCode * 0x00010000);
		
		if (bExtended)
			lParam += 0x01000000;
		
		if (GetKeyNameText(lParam, szKeyName, KEYNAMELEN))
			return szKeyName;
	}

	// else
	return _T("");
}

BOOL Misc::IsKeyPressed(DWORD dwVirtKey) 
{ 
	return (GetKeyState(dwVirtKey) & 0x8000); 
}

BOOL Misc::IsCursorKeyPressed(DWORD dwKeys)
{
	if (dwKeys & MKC_LEFTRIGHT)
	{
		if (IsKeyPressed(VK_RIGHT) || IsKeyPressed(VK_LEFT) ||
			IsKeyPressed(VK_HOME) || IsKeyPressed(VK_END))
			return TRUE;
	}
	
	if (dwKeys & MKC_UPDOWN)
	{
		if (IsKeyPressed(VK_NEXT) || IsKeyPressed(VK_DOWN) ||
			IsKeyPressed(VK_UP) || IsKeyPressed(VK_PRIOR))
			return TRUE;
	}
	
	// else 
	return FALSE;
}

BOOL Misc::IsCursorKey(DWORD dwVirtKey, DWORD dwKeys)
{
	if (dwKeys & MKC_LEFTRIGHT)
	{
		switch (dwVirtKey)
		{
		case VK_RIGHT:
		case VK_LEFT:
		case VK_HOME:
		case VK_END:
			return TRUE;
		}
	}
	
	if (dwKeys & MKC_UPDOWN)
	{
		switch (dwVirtKey)
		{
		case VK_NEXT:  
		case VK_DOWN:
		case VK_UP:
		case VK_PRIOR: 
			return TRUE;
		}
	}
	
	// else 
	return FALSE;
}

BOOL Misc::ModKeysArePressed(DWORD dwKeys) 
{
	// test for failure
	if (dwKeys & MKS_CTRL)
	{
		if (!IsKeyPressed(VK_CONTROL))
			return FALSE;
	}
	else if (IsKeyPressed(VK_CONTROL))
	{
		return FALSE;
	}

	if (dwKeys & MKS_SHIFT)
	{
		if (!IsKeyPressed(VK_SHIFT))
			return FALSE;
	}
	else if (IsKeyPressed(VK_SHIFT))
	{
		return FALSE;
	}

	if (dwKeys & MKS_ALT)
	{
		if (!IsKeyPressed(VK_MENU))
			return FALSE;
	}
	else if (IsKeyPressed(VK_MENU))
	{
		return FALSE;
	}

	return TRUE;
}

BOOL Misc::ModifyFlags(DWORD& dwFlags, DWORD dwRemove, DWORD dwAdd)
{
	DWORD dwOldFlags = dwFlags;
	
	// order is important
	dwFlags &= ~dwRemove;
	dwFlags |= dwAdd;
	
	return (dwFlags != dwOldFlags);
}

BOOL Misc::SetFlag(DWORD& dwFlags, DWORD dwFlag, BOOL bSet)
{
	return ModifyFlags(dwFlags, (bSet ? 0 : dwFlag), (bSet ? dwFlag : 0));
}

CString Misc::MakeKey(const CString& sFormat, int nKeyVal, LPCTSTR szParentKey)
{
	ASSERT(!sFormat.IsEmpty());

	CString sKey;
	sKey.Format(sFormat, nKeyVal);

	if (!IsEmpty(szParentKey))
	{
		CString sTemp;
		sTemp.Format(_T("%s\\%s"), szParentKey, sKey);

		// remove doubled-up back slashes
		sTemp.Replace(_T("\\\\"), _T("\\"));
		sKey = sTemp;
	}

	return sKey;
}

CString Misc::MakeKey(const CString& sFormat, LPCTSTR szKeyVal, LPCTSTR szParentKey)
{
	CString sKey;
	sKey.Format(sFormat, szKeyVal);

	return MakeKey(sKey, 0, szParentKey);
}

int Misc::NaturalCompare(LPCTSTR szString1, LPCTSTR szString2)
{
#ifdef _UNICODE
	// initialize once only per session
	static HMODULE hShlwapi = ::LoadLibrary(_T("Shlwapi.dll"));
	   
	typedef int (WINAPI *PFNSTRCMPLOGICAL)(PCWSTR, PCWSTR);
	static PFNSTRCMPLOGICAL pFn = (PFNSTRCMPLOGICAL)::GetProcAddress(hShlwapi, "StrCmpLogicalW");
	
	if (pFn)
		return pFn(szString1, szString2);
#endif
	
	// all else
	return _tcsicmp(szString1, szString2);
}

//////////////////////////////////////////////////////////////

// private string compare function
int StringSortProc(const void* v1, const void* v2)
{
	CString* pStr1 = (CString*)v1;
	CString* pStr2 = (CString*)v2;

	return Misc::NaturalCompare(*pStr1, *pStr2);
}

void Misc::SortArray(CStringArray& array, STRINGSORTPROC pSortProc)
{
	qsort(array.GetData(), array.GetSize(), sizeof(CString*), pSortProc ? pSortProc : StringSortProc);
}

//////////////////////////////////////////////////////////////

// private dword compare function
int DWordSortProc(const void* v1, const void* v2)
{
	DWORD* pDW1 = (DWORD*)v1;
	DWORD* pDW2 = (DWORD*)v2;

	if (*pDW1 < *pDW2)
		return -1;
	
	else if (*pDW1 > *pDW2)
		return 1;

	// else
	return 0;
}

void Misc::SortArray(CDWordArray& array, DWORDSORTPROC pSortProc)
{
	qsort(array.GetData(), array.GetSize(), sizeof(DWORD*), pSortProc ? pSortProc : DWordSortProc);
}

//////////////////////////////////////////////////////////////

void Misc::Copy(const CMapStringToString& mapSrc, CMapStringToString& mapDest)
{
	mapDest.RemoveAll();
	Append(mapSrc, mapDest);
}

void Misc::Append(const CMapStringToString& mapSrc, CMapStringToString& mapDest)
{
	if (mapSrc.GetCount())
	{
		POSITION pos = mapSrc.GetStartPosition();
		CString sKey, sValue;

		while (pos)
		{
			mapSrc.GetNextAssoc(pos, sKey, sValue);
			mapDest[sKey] = sValue;
		}
	}
}

BOOL Misc::MatchAll(const CMapStringToString& map1, const CMapStringToString& map2)
{
	if (map1.GetCount() != map2.GetCount())
		return FALSE;

	POSITION pos = map1.GetStartPosition();
	CString sKey, sValue1, sValue2;
		
	while (pos)
	{
		map1.GetNextAssoc(pos, sKey, sValue1);

		if (!map2.Lookup(sKey, sValue2) || (sValue1 != sValue2))
			return FALSE;
	}

	return TRUE;
}

BOOL Misc::HasKey(const CMapStringToString& map, const CString& sKey)
{
	CString sUnused;
	return map.Lookup(sKey, sUnused);
}

CString Misc::GetNextKey(const CMapStringToString& map, POSITION& pos)
{
	CString sKey, sUnused;
	map.GetNextAssoc(pos, sKey, sUnused);

	return sKey;
}

int Misc::GetKeys(const CMapStringToString& map, CStringArray& aKeys)
{
	aKeys.RemoveAll();

	POSITION pos = map.GetStartPosition();
	CString sKey, sUnused;
	
	while (pos)
	{
		map.GetNextAssoc(pos, sKey, sUnused);
		aKeys.Add(sKey);
	}

	return aKeys.GetSize();
}

//////////////////////////////////////////////////////////////

void Misc::MakeQuoted(CString& sText)
{
	if (!IsQuoted(sText))
	{
		// Escape all contained quotes
		sText.Replace(DBL_QUOTE, _T("\\\""));
		
		// wrap with quotes
		sText = DBL_QUOTE + sText + DBL_QUOTE;
	}
}

void Misc::MakeUnquoted(CString& sText)
{
	if (IsQuoted(sText))
	{
		sText = sText.Mid(1, (sText.GetLength() - 2));
		
		// Un-escape all contained quotes
		sText.Replace(_T("\\\""), DBL_QUOTE);
	}
}

CString Misc::GetQuoted(LPCTSTR szText)
{
	CString sText(szText);
	MakeQuoted(sText);
	
	return sText;
}

BOOL Misc::IsQuoted(LPCTSTR szText)
{
	if (!szText)
	{
		ASSERT(0);
		return FALSE;
	}
	
	// Must be two or more chars
	int nLen = lstrlen(szText);
	
	if (nLen < 2)
		return FALSE;
	
	// Check only the first and last characters
	// Don't use First()/Last() because they also use lstrlen
	return ((szText[0] == '\"') &&
			(szText[nLen - 1] == '\"'));
}

//////////////////////////////////////////////////////////////
