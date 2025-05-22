// Misc.cpp: implementation of the CMisc class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Misc.h"
#include "mapex.h"

#include <Lmcons.h>
#include <math.h>
#include <locale.h>
#include <afxtempl.h>
#include <atlconv.h>
#include <afxmt.h>

//////////////////////////////////////////////////////////////////////

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

#define RPC_TSTR RPC_WSTR

//////////////////////////////////////////////////////////////////////

#define DBL_QUOTE _T("\"")

//////////////////////////////////////////////////////////////////////

CTempLocale::CTempLocale(const CString& sLocale) : m_nCategory(-1)
{
	Initialise(LC_ALL, sLocale);
}

CTempLocale::CTempLocale(int nCategory, const CString& sLocale) : m_nCategory(-1)
{
	Initialise(nCategory, sLocale);
}

CTempLocale::~CTempLocale()
{
	_tsetlocale(m_nCategory, m_sPrevLocale);
}

void CTempLocale::Initialise(int nCategory, const CString& sLocale)
{
	ASSERT(m_nCategory == -1);
	ASSERT(m_sPrevLocale.IsEmpty());

	m_nCategory = nCategory;
	m_sPrevLocale = _tsetlocale(m_nCategory, NULL);

#ifdef _DEBUG
#	if _MSC_VER > 1200

	// This test will fail under VC6 because of something in setlocale.
	// Note: This will resolved in 9.1 because we will be using VS2015 
	//for the release build
	CString sNewLocale = _tsetlocale(m_nCategory, sLocale);
	ASSERT(sLocale.IsEmpty() || (sNewLocale == sLocale));

#	endif
#else
	_tsetlocale(m_nCategory, sLocale);
#endif
}

void CTempLocale::ChangeLocale(const CString& sAltLocale)
{
	_tsetlocale(m_nCategory, sAltLocale);
}

CString CTempLocale::Current(int nCategory)
{
	return _tsetlocale(nCategory, NULL);
}

//////////////////////////////////////////////////////////////////////

BOOL Misc::Is64BitWindows()
{
#if defined(_WIN64)

    return TRUE;   // 64-bit programs run only on Win64

#elif defined(_WIN32)

    // 32-bit programs run on both 32-bit and 64-bit Windows
	typedef BOOL (WINAPI *PFNISWOW64PROCESS)(HANDLE, PBOOL);
	
	// load dll once only
	HMODULE hKernel32 = LoadLibrary(_T("kernel32.dll"));
	
	if (hKernel32)
	{
		PFNISWOW64PROCESS fnIsWow64 = (PFNISWOW64PROCESS)GetProcAddress(hKernel32, "IsWow64Process");
		
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
			nFormat = CB_TEXTFORMAT;
		
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
		nFormat = CB_TEXTFORMAT;
	
	HANDLE hData = ::GetClipboardData(nFormat);
	sText = (LPCTSTR)GlobalLock(hData);

	::GlobalUnlock(hData);
	::CloseClipboard(); // always

	return sText;
}

BOOL Misc::ClipboardHasText()
{
	return ClipboardHasFormat(CB_TEXTFORMAT);
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
	CString sMachineAndUser;
	sMachineAndUser.Format(_T("%s%c%s"), GetComputerName(), cSeparator, GetUserName());

	return sMachineAndUser;
}

CString Misc::GetComputerName()
{
	DWORD LEN = MAX_COMPUTERNAME_LENGTH + 1;
	
	CString sMachine;
	::GetComputerName(sMachine.GetBuffer(LEN), &LEN);
	sMachine.ReleaseBuffer();

	return sMachine;
}

CString Misc::GetUserName()
{
	DWORD LEN = UNLEN + 1;
		
	CString sUser;
	::GetUserName(sUser.GetBuffer(LEN), &LEN);
	sUser.ReleaseBuffer();

	return sUser;
}

CString Misc::FormatArray(const CStringArray& aValues, TCHAR cSep, BOOL bIncEmpty)
{
	TCHAR szSep[2] = { cSep, 0 };

	return FormatArray(aValues, szSep, bIncEmpty);
}

CString Misc::FormatArray(const CStringArray& aValues, LPCTSTR szSep, BOOL bIncEmpty)
{
	int nCount = aValues.GetSize();

	switch (nCount)
	{
	case 0: return _T("");
	case 1:	return aValues[0];
	}

	// VC6 has very poor performance with big arrays
	// So we do in-place copying to avoid concatenation
	CString sSep(szSep), sText;

	if (sSep.IsEmpty())
		sSep = GetListSeparator() + ' ';

	int nReqLen = GetFormattedLength(aValues, sSep, bIncEmpty);
	ASSERT(nReqLen == GetFormattedLength(aValues, szSep, bIncEmpty));

	if (nReqLen > 0)
	{
		LPTSTR szBuffer = sText.GetBuffer(nReqLen), szBufPtr = szBuffer;
		BOOL bFirstItem = TRUE;
		
		for (int nItem = 0; nItem < nCount; nItem++)
		{
			const CString& sItem = GetItem(aValues, nItem);
			
			if (bIncEmpty || !sItem.IsEmpty())
			{
				if (!bFirstItem)
				{
					_tcsncpy(szBufPtr, sSep, sSep.GetLength());
					szBufPtr += sSep.GetLength();
				}
				
				_tcsncpy(szBufPtr, sItem, sItem.GetLength());
				szBufPtr += sItem.GetLength();

				bFirstItem = FALSE;
			}
		}
		
		ASSERT(szBufPtr == (szBuffer + nReqLen));

		sText.ReleaseBuffer(nReqLen);
	}

	return sText;
}

CString Misc::FormatArrayAsNumberedList(const CStringArray& aValues, LPCTSTR szDelim, int nStart, BOOL bNumberBlankLines)
{
	ASSERT(szDelim);

	int nCount = aValues.GetSize();
	CString sList, sItem, sNumbered;

	for (int nItem = 0, nLine = nStart; nItem < nCount; nItem++)
	{
		sItem = GetItem(aValues, nItem);

		if (bNumberBlankLines || !sItem.IsEmpty())
		{
			sNumbered.Format(_T("%d%s%s"), nLine++, szDelim, sItem);
			sList += sNumbered;
		}

		sList += '\n';
	}

	return sList;
}

CString Misc::FormatArray(const CDWordArray& aValues, TCHAR cSep)
{
	return FormatArrayT(aValues, _T("%ld"), cSep);
}

CString Misc::FormatArray(const CDWordArray& aValues, LPCTSTR szSep)
{
	return FormatArrayT(aValues, _T("%ld"), szSep);
}

#ifdef _DEBUG
void Misc::Trace(const CStringArray& aValues)
{
	int nCount = aValues.GetSize();

	for (int nItem = 0; nItem < nCount; nItem++)
		TRACE(_T("%s, "), GetItem(aValues, nItem));

	TRACE(_T("\n"));
}
#endif

CString Misc::GetLongestItem(const CStringArray& aValues)
{
	CString sLongest;
	int nItem = aValues.GetSize();

	while (nItem--)
	{
		const CString& sItem = GetItem(aValues, nItem);

		if (sItem.GetLength() > sLongest.GetLength())
			sLongest = sItem;
	}

	return sLongest;
}

int Misc::GetMaximumItemLength(const CStringArray& aValues)
{
	int nLongest = 0;
	int nItem = aValues.GetSize();

	while (nItem--)
	{
		const CString& sItem = GetItem(aValues, nItem);
		nLongest = max(nLongest, sItem.GetLength());
	}

	return nLongest;
}

int Misc::GetFormattedLength(const CStringArray& aValues, LPCTSTR szSep, BOOL bIncEmpty)
{
	int nCount = aValues.GetSize();

	switch (nCount)
	{
	case 0: return 0;
	case 1:	return aValues[0].GetLength();
	}

	// All else
	int nTotalLen = 0;
	int nLenSep = lstrlen(szSep);
	
	if (nLenSep == 0)
		nLenSep = (GetListSeparator().GetLength() + 1);

	for (int nItem = 0; nItem < nCount; nItem++)
	{
		int nItemLen = GetItem(aValues, nItem).GetLength();

		if (!bIncEmpty && !nItemLen)
			continue;

		nTotalLen += (nItemLen + nLenSep);
	}

	// Remove trailing separator
	if (nTotalLen)
		nTotalLen -= nLenSep;

	return nTotalLen;
}

BOOL Misc::Split(CString& sText, CString& sRest, TCHAR cDelim, BOOL bTrimResults)
{
	TCHAR szDelim[] = { cDelim, 0 };

	return Split(sText, sRest, szDelim, bTrimResults);
}

BOOL Misc::Split(CString& sText, CString& sRest, LPCTSTR szDelim, BOOL bTrimResults)
{
	if (sText.IsEmpty())
		return FALSE;
	
	int nDelim = sText.Find(szDelim);

	if (nDelim == -1)
		return FALSE;

	// else
	sRest = sText.Mid(nDelim + lstrlen(szDelim));
	sText = sText.Left(nDelim);

	if (bTrimResults)
	{
		Trim(sText);
		Trim(sRest);
	}

	return TRUE;
}

CString Misc::SplitLeft(const CString& sText, TCHAR cDelim, BOOL bTrimResult)
{
	TCHAR szDelim[] = { cDelim, 0 };

	return SplitLeft(sText, szDelim, bTrimResult);
}

CString Misc::SplitLeft(const CString& sText, LPCTSTR szDelim, BOOL bTrimResult)
{
	if (!sText.IsEmpty())
	{
		int nDelim = sText.Find(szDelim);

		if (nDelim != -1)
			return (bTrimResult ? Trim(sText.Left(nDelim)) : sText.Left(nDelim));
	}	

	// else
	return sText;
}

CString& Misc::Trim(CString& sText, TCHAR cChar)
{
	if (cChar)
	{
		sText.TrimLeft(cChar);
		sText.TrimRight(cChar);
	}

	return sText;
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

CString& Misc::TrimAlpha(CString& sText)
{
	// Trim anything that's not a number
	int nLen = sText.GetLength();

	if (nLen)
	{
		int nStart = 0, nEnd = nLen;

		while (nEnd--)
		{
			if (_istdigit(sText[nEnd]))
				break;
		}

		while (nStart < nEnd)
		{
			if (_istdigit(sText[nStart]))
				break;

			nStart++;
		}

		if ((nEnd - nStart) != nLen)
			sText = sText.Mid(nStart, (nEnd + 1 - nStart));
	}
	
	return sText;
}

int Misc::FindFirstOf(const CString& sSearchFor, const CString& sSearchIn, BOOL bCaseSensitive)
{
	if (bCaseSensitive)
		return sSearchIn.FindOneOf(sSearchFor);

	// Case-insensitive
	int nFind = ToUpper(sSearchIn).FindOneOf(ToUpper(sSearchFor));
	ASSERT(nFind < sSearchIn.GetLength());

	return nFind;
}

int Misc::Find(TCHAR cSearchFor, const CString& sSearchIn, BOOL bCaseSensitive, int iStart)
{
	int nFind = sSearchIn.Find(cSearchFor, iStart);

	if (bCaseSensitive || (nFind != -1))
		return nFind;

	// Case-sensitive
	nFind = sSearchIn.Find(ToggleCase(cSearchFor));

	return nFind;
}

int Misc::Find(const CString& sSearchFor, const CString& sSearchIn, BOOL bCaseSensitive, BOOL bWholeWord, int iStart)
{
	if (sSearchFor.GetLength() > sSearchIn.GetLength())
		return -1;

	CString sWord(sSearchFor), sText(sSearchIn);

	if (bWholeWord)
		Trim(sWord); // because whitespace is a delimiter

	if (sWord.IsEmpty() || sText.IsEmpty())
		return -1;

	if (!bCaseSensitive)
	{
		MakeUpper(sText);
		MakeUpper(sWord);
	}

	int nFind = -1;

	while ((iStart < sSearchIn.GetLength()) && (nFind == -1))
	{
		nFind = sText.Find(sWord, iStart);

		if (nFind == -1)
			break;

		// update search start before we might need to modify 'nFind'
		// This is also the first char pos after the end of 'sWord'
		iStart = nFind + sWord.GetLength();

		if (bWholeWord)
		{
			// Test prior and next chars for delimiters
			static const CString DELIMS("()-\\/{}[]:;,. ?\"'\n\r\t");
			TCHAR cPrevChar = ' ', cNextChar = ' ';

			// prev
			if (nFind > 0)
				cPrevChar = sText[nFind - 1];

			// next
			if (iStart < sText.GetLength())
				cNextChar = sText[iStart];

			if ((DELIMS.Find(cPrevChar) == -1) || (DELIMS.Find(cNextChar) == -1))
				nFind = -1;
		}

		if (nFind != -1)
			break;

		// else keep going
	}
	
	return nFind;
}

int Misc::Replace(const CString& sSearchFor, const CString& sReplaceWith, CString& sSearchIn, BOOL bCaseSensitive, BOOL bWholeWord)
{
	if (sSearchFor.IsEmpty() || sSearchIn.IsEmpty())
	{
		ASSERT(0);
		return 0;
	}

	int nNumReplaced = 0;
	int nFind = Find(sSearchFor, sSearchIn, bCaseSensitive, bWholeWord);

	while (nFind != -1)
	{
		sSearchIn = (sSearchIn.Left(nFind) + sReplaceWith + sSearchIn.Mid(nFind + sSearchFor.GetLength()));
		nNumReplaced++;

		nFind = Find(sSearchFor, sSearchIn, bCaseSensitive, bWholeWord, (nFind + sReplaceWith.GetLength()));
	}

	return nNumReplaced;
}

int Misc::Replace(const CString& sSearchFor, const CString& sReplaceWith, CStringArray& aSearchIn, BOOL bCaseSensitive, BOOL bWholeWord)
{
	int nNumReplaced = 0, nItem = aSearchIn.GetSize();

	while (nItem--)
		nNumReplaced += Replace(sSearchFor, sReplaceWith, aSearchIn[nItem], bCaseSensitive, bWholeWord);

	return nNumReplaced;
}

BOOL Misc::HasPrefix(LPCTSTR szText, LPCTSTR szPrefix, BOOL bCaseSensitive)
{
	if (IsEmpty(szText) || IsEmpty(szPrefix))
		return FALSE;

	int nLenPrefix = lstrlen(szPrefix);

	if (lstrlen(szText) < nLenPrefix)
		return FALSE;

	if (bCaseSensitive)
		return (_tcsncmp(szText, szPrefix, nLenPrefix) == 0);
		
	return (_tcsnicmp(szText, szPrefix, nLenPrefix) == 0);
}

BOOL Misc::HasSuffix(LPCTSTR szText, LPCTSTR szSuffix, BOOL bCaseSensitive)
{
	if (IsEmpty(szText) || IsEmpty(szSuffix))
		return FALSE;

	int nLenText = lstrlen(szText);
	int nLenSuffix = lstrlen(szSuffix);

	if (nLenText < nLenSuffix)
		return FALSE;

	if (bCaseSensitive)
		return (_tcscmp(szText + (nLenText - nLenSuffix), szSuffix) == 0);

	return (_tcsicmp(szText + (nLenText - nLenSuffix), szSuffix) == 0);
}

BOOL Misc::RemovePrefix(CString& sText, LPCTSTR szPrefix, BOOL bCaseSensitive, BOOL bTrimResult)
{
	if (!HasPrefix(sText, szPrefix, bCaseSensitive))
		return FALSE;

	sText = sText.Mid(lstrlen(szPrefix));

	if (bTrimResult)
		sText.TrimLeft();

	return TRUE;
}

BOOL Misc::RemoveSuffix(CString& sText, LPCTSTR szSuffix, BOOL bCaseSensitive, BOOL bTrimResult)
{
	if (!HasSuffix(sText, szSuffix, bCaseSensitive))
		return FALSE;

	sText = sText.Left(sText.GetLength() - lstrlen(szSuffix));

	if (bTrimResult)
		sText.TrimRight();

	return TRUE;
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

void Misc::Reverse(CString& sText)
{
	_tcsrev((LPTSTR)(LPCTSTR)sText);
}

BOOL Misc::IsEmpty(LPCTSTR szText) 
{ 
	return ((!szText || !szText[0]) ? TRUE : FALSE); 
}

BOOL Misc::IsEmpty(LPCSTR szText) 
{ 
	return ((!szText || !szText[0]) ? TRUE : FALSE); 
}

BOOL Misc::HasEmpty(const CStringArray& aItems)
{
	int nItem = aItems.GetSize();

	while (nItem--)
	{
		if (aItems[nItem].IsEmpty())
			return TRUE;
	}

	return FALSE;
}

CString Misc::Last(const CStringArray& aText)
{
	int nSize = aText.GetSize();

	if (nSize > 0)
		return aText[nSize - 1];

	static CString sEmpty;
	sEmpty.Empty();
	
	return sEmpty;
}

CString& Misc::Last(CStringArray& aText)
{
	int nSize = aText.GetSize();

	if (nSize > 0)
		return aText[nSize - 1];

	static CString sEmpty;
	sEmpty.Empty();

	return sEmpty;
}

int Misc::LastIndex(const CString& sText)
{
	return (sText.GetLength() - 1);
}

int Misc::LastIndex(LPCTSTR szText)
{
	return (lstrlen(szText) - 1);
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
	return sText[LastIndex(sText)];
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
	return szText[LastIndex(szText)];
}

BOOL Misc::TrimFirstIf(TCHAR cTest, CString& sText)
{
	if (cTest && (First(sText) == cTest))
	{
		TrimFirst(sText);
		return TRUE;
	}

	// else
	return FALSE;
}

BOOL Misc::TrimLastIf(TCHAR cTest, CString& sText)
{
	if (cTest && (Last(sText) == cTest))
	{
		TrimLast(sText);
		return TRUE;
	}

	// else
	return FALSE;
}

TCHAR Misc::TrimFirst(CString& sText)
{
	if (sText.IsEmpty())
		return 0;

	TCHAR nFirst = First(sText);
	
	if (nFirst != 0)
		sText = sText.Mid(1);
	
	return nFirst;
}

TCHAR Misc::TrimLast(CString& sText)
{
	if (sText.IsEmpty())
		return 0;

	TCHAR nLast = Last(sText);
	
	if (nLast != 0)
		sText = sText.Left(sText.GetLength() - 1);
	
	return nLast;
}

BOOL Misc::TrimTrailingDecimalZeros(CString& sText)
{
	CString sSep = GetDecimalSeparator();
	int nDecimal = sText.Find(sSep);

	if (nDecimal < 0)
		return FALSE;

	sText.TrimRight('0');

	// Remove decimal if it's now at the end
	if (nDecimal == (sText.GetLength() - sSep.GetLength()))
		sText = sText.Left(nDecimal);

	return TRUE;
}

BOOL Misc::TrimTrailingDecimals(CString& sText)
{
	CString sSep = GetDecimalSeparator();
	int nDecimal = sText.Find(sSep);

	if (nDecimal < 0)
		return FALSE;

	// To avoid any memory manipulation simply insert a null
	sText = sText.Left(nDecimal);
	return TRUE;
}

int Misc::Split(const CString& sText, CDWordArray& aValues, TCHAR cDelim, BOOL bAllowEmpty)
{
	TCHAR szSep[2] = { cDelim, 0 };

	return Split(sText, aValues, szSep, bAllowEmpty);
}

int Misc::Split(const CString& sText, CDWordArray& aValues, LPCTSTR szSep, BOOL bAllowEmpty)
{
	CStringArray aStrValues;
	int nNumValues = Split(sText, aStrValues, szSep, bAllowEmpty);

	aValues.SetSize(nNumValues);

	for (int nVal = 0; nVal < nNumValues; nVal++)
		aValues[nVal] = _ttoi(aStrValues[nVal]);

	return nNumValues;
}

int Misc::Split(const CString& sText, CStringArray& aValues, TCHAR cDelim, BOOL bAllowEmpty, BOOL bPreserveQuotes)
{
	TCHAR szSep[2] = { cDelim, 0 };

	return Split(sText, aValues, szSep, bAllowEmpty, bPreserveQuotes);
}

int Misc::Split(const CString& sText, CStringArray& aValues, LPCTSTR szSep, BOOL bAllowEmpty, BOOL bPreserveQuotes)
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
				bAddWord = ((nPos > 0) || bAllowEmpty);

			nPos += nSepLen - 1; // minus 1 because the loop also increments
		}
		else if (chr == '\"') // double quote
		{
			// flip bInQuotes
			bInQuotes = !bInQuotes;

			if (bPreserveQuotes)
			{
				sWord += chr;
			}
			else // Workaround to handle Quoted empty string as last word
			{
				if (!bInQuotes && bAllowEmpty && sWord.IsEmpty())
					sWord = ' ';
			}
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

int Misc::SplitLines(const CString& sText, CStringArray& aLines, int nMaxLineLength)
{
	int nNumLines = Split(sText, aLines, '\n', TRUE, TRUE);

	if ((nNumLines == 0) || (nMaxLineLength <= 0) && (nMaxLineLength != -1))
	{
		ASSERT(0);
		return 0;
	}

	if (nMaxLineLength != -1)
	{
		// Extra over processing
		int nLine = nNumLines;

		while (nLine--)
		{
			CString& sLine = aLines[nLine];

			if (sLine.GetLength() > nMaxLineLength)
			{
				CStringArray aSubLines;

				do
				{
					// Find nearest word-break
					CString sSubLine = Left(sLine, nMaxLineLength, TRUE);

					aSubLines.Add(sSubLine);
					sLine = sLine.Mid(sSubLine.GetLength());
				}
				while (sLine.GetLength() > nMaxLineLength);
				
				// Add whatever's left over
				if (!sLine.IsEmpty())
					aSubLines.Add(sLine);

				// Replace the existing string with the sub strings
				aLines.RemoveAt(nLine);
				aLines.InsertAt(nLine, &aSubLines);
			}
		}
	}

	return aLines.GetSize();
}

CString Misc::Left(const CString& sText, int nLength, BOOL bNearestWord)
{
	if (bNearestWord && (nLength < sText.GetLength()))
	{
		// Look forwards and backwards for word break
		static CString BACKWARD_DELIMS(_T(")-\\/}]:;,. ?\"'\r\t")); // opening braces

		int nFindPrev = FindNextOneOf(BACKWARD_DELIMS, sText, FALSE, nLength);

		// Only accept the delimiter position if it falls
		// within a nominal word length of the requested length
		if (nFindPrev != -1)
		{
			const int WORD_LEN = 15;

			if (nFindPrev >= (nLength - WORD_LEN))
				nLength = (nFindPrev + 1); // include delimiter
		}
	}

	return sText.Left(nLength);
}

int Misc::FindNextOneOf(const CString& sSearchForOneOf, const CString& sSearchIn, BOOL bForward, int nStartPos)
{
	int nFind = -1, nSearchLen = sSearchIn.GetLength();

	if (bForward)
	{
		nFind = ((nStartPos == -1) ? 0 : nStartPos);
		
		while ((nFind < nSearchLen) && (sSearchForOneOf.Find(sSearchIn[nFind]) == -1))
			nFind++;

		if (nFind >= nSearchLen)
			nFind = -1;
	}
	else // backwards
	{
		nFind = ((nStartPos == -1) ? (nSearchLen - 1) : nStartPos);
		
		while ((nFind >= 0) && (sSearchForOneOf.Find(sSearchIn[nFind]) == -1))
			nFind--;

		if (nFind < 0)
			nFind = -1;
	}

	return nFind;
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

		// look for exact matching item in array2
		if (Find(sItem1, array2, bCaseSensitive, TRUE) == -1)
		{
			// no-match found == not the same
			return FALSE;
		}
	}

	return TRUE;
}

BOOL Misc::MatchAny(const CStringArray& array1, const CStringArray& array2, BOOL bCaseSensitive, BOOL bWholeWord) 
{
	int nSize1 = array1.GetSize();

	for (int nItem1 = 0; nItem1 < nSize1; nItem1++)
	{
		const CString& sItem1 = GetItem(array1, nItem1);

		// look for matching item
		if (Find(sItem1, array2, bCaseSensitive, bWholeWord) != -1)
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
		if (FindT(array1[nItem1], array2) == -1)
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
		if (FindT(array1[nItem1], array2) != -1)
			return TRUE;
	}
	
	return FALSE;
}

int Misc::Find(LPCTSTR szItem, const CStringArray& aValues, BOOL bCaseSensitive, BOOL bWholeWord)
{
	ASSERT (szItem);

	if (szItem == NULL)
		return -1;

	BOOL bItemIsEmpty = IsEmpty(szItem);
	int nSize = aValues.GetSize();

	for (int nItem = 0; nItem < nSize; nItem++)
	{
		// look for matching item
		const CString& sArrItem = GetItem(aValues, nItem);

		// special case: empty item
		if (bItemIsEmpty)
		{
			if (sArrItem.IsEmpty())
				return nItem;
		}
		else if (bWholeWord)
		{
			if (bCaseSensitive)
			{
				if (sArrItem.Compare(szItem) == 0)
					return nItem;
			}
			else
			{
				if (sArrItem.CompareNoCase(szItem) == 0)
					return nItem;
			}
		}
		else if (Find(szItem, sArrItem, bCaseSensitive, bWholeWord) != -1)
		{
			return nItem;
		}
	}

	return -1;
}

BOOL Misc::Contains(LPCTSTR szItem, const CStringArray& aValues, BOOL bCaseSensitive, BOOL bWholeWord)
{
	return (Find(szItem, aValues, bCaseSensitive, bWholeWord) != -1);
}

const CString& Misc::GetItem(const CStringArray& aValues, int nItem)
{
	ASSERT(nItem >= 0 && nItem < aValues.GetSize());

	if (nItem < 0 || nItem >= aValues.GetSize())
	{
		static CString sDummy;
		return sDummy;
	}

	return aValues.GetData()[nItem];
}

int Misc::RemoveEmptyItems(CStringArray& aFrom)
{
	int nOrgCount = aFrom.GetSize(), nItem = nOrgCount;

	while (nItem--)
	{
		CString& sItem = aFrom[nItem];
		Trim(sItem);

		if (sItem.IsEmpty())
			aFrom.RemoveAt(nItem);
	}

	return (nOrgCount - aFrom.GetSize());
}

int Misc::RemoveDuplicates(CStringArray& aFrom, BOOL bCaseSensitive)
{
	CStringSet mapUniqueItems;
	int nOrgCount = aFrom.GetSize(), nNumItems = nOrgCount;

	for (int nItem = 0; nItem < nNumItems; nItem++)
	{
		CString sItem = (bCaseSensitive ? aFrom[nItem] : ToUpper(aFrom[nItem]));

		if (!mapUniqueItems.Has(sItem))
		{
			mapUniqueItems.Add(sItem);
			continue;
		}

		// else
		aFrom.RemoveAt(nItem);

		nItem--;
		nNumItems--;
	}

	return (nOrgCount - nNumItems);
}

int Misc::RemoveItems(const CStringArray& aValues, CStringArray& aFrom, BOOL bCaseSensitive)
{
	if (aValues.GetSize() == 0)
		return 0;

	// Create a look up of the values to be removed
	CStringSet mapValues;

	if (bCaseSensitive)
	{
		mapValues.CopyFrom(aValues);
	}
	else
	{
		int nItem = aValues.GetSize();

		while (nItem--)
			mapValues.Add(ToUpper(aValues[nItem]));
	}

	// Traverse the existing array removing as we go
	int nOrgCount = aFrom.GetSize(), nItem = nOrgCount;

	while (nItem--)
	{
		CString sItem = (bCaseSensitive ? aFrom[nItem] : ToUpper(aFrom[nItem]));

		if (mapValues.Has(sItem))
			aFrom.RemoveAt(nItem);
	}

	return (nOrgCount - aFrom.GetSize());
}

BOOL Misc::RemoveItem(LPCTSTR szItem, CStringArray& aFrom, BOOL bCaseSensitive)
{
	int nFind = Find(szItem, aFrom, bCaseSensitive, FALSE);
	
	if (nFind == -1)
		return -1;

	aFrom.RemoveAt(nFind);
	return TRUE;
}

int Misc::AppendItems(const CStringArray& aFrom, CStringArray& aTo, BOOL bRemoveDuplicates, BOOL bCaseSensitiveRemove)
{
	int nOrgCount = aTo.GetSize();

	aTo.Append(aFrom);

	if (bRemoveDuplicates)
		RemoveDuplicates(aTo, bCaseSensitiveRemove);

	return (aTo.GetSize() - nOrgCount);
}

int Misc::AppendItems(const CDWordArray& aFrom, CDWordArray& aTo, BOOL bRemoveDuplicates)
{
	int nOrgCount = aTo.GetSize();

	aTo.Append(aFrom);

	if (bRemoveDuplicates)
		RemoveDuplicates(aTo);

	return (aTo.GetSize() - nOrgCount);
}

int Misc::RemoveDuplicates(CDWordArray& aFrom)
{
	CDWordSet mapUniqueItems;
	int nOrgCount = aFrom.GetSize(), nNumItems = nOrgCount;

	for (int nItem = 0; nItem < nNumItems; nItem++)
	{
		if (!mapUniqueItems.Has(aFrom[nItem]))
		{
			mapUniqueItems.Add(aFrom[nItem]);
			continue;
		}

		// else
		aFrom.RemoveAt(nItem);

		nItem--;
		nNumItems--;
	}

	return (nOrgCount - nNumItems);
}

BOOL Misc::AddUniqueItem(const CString& sItem, CStringArray& aTo, BOOL bCaseSensitive)
{
	if (sItem.IsEmpty())
		return FALSE;

	int nFind = Find(sItem, aTo, bCaseSensitive, TRUE);

	if (nFind == -1) // doesn't already exist
	{
		aTo.Add(sItem);
		return TRUE;
	}

	return FALSE; // not added
}

CString Misc::GetLocaleInfo(LCTYPE lcType, int nBufSize)
{
	CString sValue;
	::GetLocaleInfo(LOCALE_USER_DEFAULT, lcType, sValue.GetBuffer(nBufSize + 1), nBufSize);
	sValue.ReleaseBuffer();

	return sValue;
}

CString Misc::GetListSeparator()
{
	CString sSep = GetLocaleInfo(LOCALE_SLIST, 10);

	// Trim extra spaces
	Trim(sSep);

	// If none found, use a comma
	if (sSep.IsEmpty())
		sSep = ',';

	return sSep;
}

CString Misc::GetAM()
{
	return GetLocaleInfo(LOCALE_S1159, 10);
}

CString Misc::GetPM()
{
	return GetLocaleInfo(LOCALE_S2359, 10);
}

CString Misc::GetTimeFormat(BOOL bIncSeconds, BOOL bISO)
{
	if (bISO)
		return (bIncSeconds ? _T("HH:mm:ss") : _T("HH:mm"));
	
	// else
	CString sFormat = GetLocaleInfo(LOCALE_STIMEFORMAT, 100);

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
	CString sSep = GetLocaleInfo(LOCALE_STIME, 10);

	// Trim extra spaces
	Trim(sSep);
		
	// If none found, use a colon
	if (sSep.IsEmpty())
		sSep = ':';

	return sSep;
}

CString Misc::GetDateSeparator()
{
	CString sSep = GetLocaleInfo(LOCALE_SDATE, 10);

	// Trim extra spaces
	Trim(sSep);
		
	// If none found, use a slash
	if (sSep.IsEmpty())
		sSep = '/';

	return sSep;
}

CString Misc::GetShortDateFormat(BOOL bIncDOW, BOOL bISO)
{
	if (bISO)
		return _T("yyyy-MM-dd");

	// else
	CString sFormat = GetLocaleInfo(LOCALE_SSHORTDATE, 100);

	if (bIncDOW)
	{
		CString sTemp = _T("ddd ") + sFormat;
		return sTemp;
	}

	return sFormat;
}

BOOL Misc::ShortDateFormatHasMonthBeforeDay()
{
	CString sFormat = Misc::GetShortDateFormat();
	
	return (sFormat.Find('M') < sFormat.Find('d'));
}

CString Misc::GetDecimalSeparator()
{
	CString sSep = GetLocaleInfo(LOCALE_SDECIMAL, 10);

	// Trim extra spaces
	Trim(sSep);
		
	// If none found, use a dot
	if (sSep.IsEmpty())
		sSep = '.';

	return sSep;
}

BOOL Misc::IsMetricMeasurementSystem()
{
	CString sSystem = GetLocaleInfo(LOCALE_IMEASURE, 2);
	
	return (_ttoi(sSystem) == 0);
}

CString& Misc::EncodeAsMultiByte(CString& sUnicode, UINT nCodePage)
{
	// calc number of unicode chars in string
	int nLen = sUnicode.GetLength();
	
	char* szMB = WideToMultiByte((LPCWSTR)(LPCTSTR)sUnicode, nLen, nCodePage);
	// nLen is now the number of returned multibyte chars
	
	// calc number of bytes required to hold multibyte chars
	int nBytes = nLen;
	
	// calc number of chars required to hold multibyte chars
	nLen = ((nLen + 1) / sizeof(WCHAR));
	
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
	if (!szFrom)
		return NULL;

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
	if (!szFrom)
		return NULL;

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

int Misc::Round(double dValue)
{
	if (dValue == 0)
		return 0;

	if (dValue > 0)
		return (int)(dValue + 0.5);

	// else
	return (int)(dValue - 0.5);
}

double Misc::Round(double dValue, int nDecimals)
{
	if (dValue == 0)
		return 0.0;

	if (nDecimals <= 0)
		return Round(dValue);

	double dMulDiv = 10;
	nDecimals--;

	while (nDecimals--)
		dMulDiv *= 10;

	dValue = Round(dValue * dMulDiv);

	return (dValue / dMulDiv);
}

int Misc::GetNextValue(int nValue, int nIncrement)
{
	ASSERT(nIncrement != 0);
	ASSERT(nValue >= 0);

	BOOL bUp = (nIncrement > 0);
	int nAmount = abs(nIncrement);

	// Note: This mimics the behaviour performed by the spin button control
	if (nAmount > 1)
	{
		// bump the % to the next upper (if +ve) or
		// next lower (if -ve) whole increment
		// before adding the increment
		if (nValue % nAmount)
		{
			if (bUp)
				nValue = (((nValue / nAmount) + 1) * nAmount);
			else
				nValue = ((nValue / nAmount) * nAmount);
		}
	}

	return (nValue + nIncrement);
}

BOOL Misc::IsNumber(const CString& sValue)
{
	int nLen = sValue.GetLength();

	if (nLen == 0)
		return FALSE;

	// Can start with '+' or a '-'
	int nPos = 0;
	TCHAR chr = sValue[nPos];

	if ((chr == '-') || (chr == '+'))
	{
		if (nLen == 1)
			return FALSE;

		nPos++;
	}

	// Rest must be digits with a single optional decimal
	TCHAR cDecSep = 0;
	BOOL bHasDecimal = FALSE;

	for (; nPos < nLen; nPos++)
	{
		chr = sValue[nPos];

		if (_istdigit(chr))
			continue;
		
		if (((chr == '.') || (chr == ',')) && !bHasDecimal)
		{
			bHasDecimal = TRUE;
		}
		else if (!bHasDecimal)
		{
			if (cDecSep == 0)
				cDecSep = Misc::GetDecimalSeparator()[0];

			if (chr != cDecSep)
				return FALSE;

			bHasDecimal = TRUE;
		}
		else
		{
			return FALSE;
		}
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

BOOL Misc::StatesDiffer(BOOL bState1, BOOL bState2) 
{ 
	return ((bState1 && !bState2) || (!bState1 && bState2)); 
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
	double dValue = 0.0;

	if (!sValue.IsEmpty())
	{
		// Always use the 'C' locale and convert decimal separators 
		// to periods because that gives us much more control
		CTempLocale loc(LC_NUMERIC, "C");
		
		if (sValue.Find('.') != -1)
		{
			// No changes required
			dValue = _tcstod(sValue, NULL);
		}
		else
		{
			CString sTemp(sValue);
			
			// Check for comma as most likely next bet
			if (sTemp.Replace(',', '.') == 0)
			{
				// else check for a custom decimal
				// separator unless that's a comma too
				TCHAR cDecSep = GetDecimalSeparator()[0];
				
				if (cDecSep != ',')
					sTemp.Replace(cDecSep, '.');
			}
			
			dValue = _tcstod(sTemp, NULL);
		}
	}

	return dValue;
}

BOOL Misc::ShutdownBlockReasonCreate(HWND hWnd, LPCTSTR szReason)
{
	typedef BOOL (WINAPI *PFNSHUTDOWNBLOCKREASONCREATE)(HWND, LPCWSTR);

	// load user32.dll once only
	HMODULE hUser32 = LoadLibrary(_T("user32.dll"));

	if (hUser32)
	{
		PFNSHUTDOWNBLOCKREASONCREATE fnCreate = (PFNSHUTDOWNBLOCKREASONCREATE)GetProcAddress(hUser32, "ShutdownBlockReasonCreate");

		if (fnCreate)
			return fnCreate(hWnd, szReason);
	}

	// must be < XP
	return FALSE;
}

BOOL Misc::ShutdownBlockReasonDestroy(HWND hWnd)
{
	typedef BOOL (WINAPI *PFNSHUTDOWNBLOCKREASONDESTROY)(HWND);

	// load user32.dll once only
	HMODULE hUser32 = LoadLibrary(_T("user32.dll"));

	if (hUser32)
	{
		PFNSHUTDOWNBLOCKREASONDESTROY fnDestroy = (PFNSHUTDOWNBLOCKREASONDESTROY)GetProcAddress(hUser32, "ShutdownBlockReasonDestroy");

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

	HMODULE hLib = LoadLibrary(_T("kernel32.dll"));
	LANGID nLangID = 0;

	if (hLib)
	{
		FNGETUSERDEFAULTUILANGUAGE pFN = (FNGETUSERDEFAULTUILANGUAGE)GetProcAddress(hLib, "GetUserDefaultUILanguage");

		if (pFN)
			nLangID = pFN();
	}

	return nLangID;
}

BOOL Misc::IsFullScreenAppActive()
{
	typedef HRESULT (*FNSHQUERYUSERNOTIFICATIONSTATE)(int*);

	HMODULE hLib = LoadLibrary(_T("Shell32.dll"));

	if (hLib)
	{
		FNSHQUERYUSERNOTIFICATIONSTATE pFN = (FNSHQUERYUSERNOTIFICATIONSTATE)GetProcAddress(hLib, "SHQueryUserNotificationState");
		int nState = 0;

		if (pFN && (pFN(&nState) == S_OK))
		{
			switch (nState)
			{
			case 2: // QUNS_BUSY
			case 3: // QUNS_RUNNING_D3D_FULL_SCREEN
			case 4:	// QUNS_PRESENTATION_MODE
			case 7:	// QUNS_APP
				return TRUE;
			}
		}
	}

	// All else
	return FALSE;
}

LANGID Misc::GetUserKeyboardLanguage()
{
	return LOWORD(::GetKeyboardLayout(0));
}

LANGID Misc::GetPrimaryLanguage()
{
	LCID lcid = ::GetThreadLocale();
	LANGID lid = LANGIDFROMLCID(lcid);
	
	return PRIMARYLANGID(lid);
}

CString Misc::GetDefCharset()
{
	return GetLocaleInfo(LOCALE_IDEFAULTANSICODEPAGE, 7);
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

TCHAR Misc::ToUpper(TCHAR cText)
{
	return _totupper(cText);
}

TCHAR Misc::ToLower(TCHAR cText)
{
	return _totlower(cText);
}

TCHAR Misc::ToggleCase(TCHAR cText)
{
	if (_istlower(cText))
		return _totupper(cText);

	if (_istupper(cText))
		return _totlower(cText);

	return cText;
}

CStringArray& Misc::MakeUpper(CStringArray& aText)
{
	int nItem = aText.GetSize();
	
	while (nItem--)
		MakeUpper(aText[nItem]);

	return aText;
}

CStringArray& Misc::MakeLower(CStringArray& aText)
{
	int nItem = aText.GetSize();

	while (nItem--)
		MakeLower(aText[nItem]);

	return aText;
}

int Misc::ParseSearchString(LPCTSTR szSearch, CStringArray& aWords)
{
	aWords.RemoveAll();
	
	// parse on spaces unless enclosed in double-quotes
	int nLen = lstrlen(szSearch);
	BOOL bInQuotes = FALSE, bAddWord = FALSE;
	CString sWord;
	
	for (int nPos = 0; nPos < nLen; nPos++)
	{
		switch (szSearch[nPos])
		{
		case ' ': // word break
			if (bInQuotes)
				sWord += szSearch[nPos];
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
			sWord += szSearch[nPos];
			
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
	// Change locale to local default
	CTempLocale(LC_NUMERIC, "");

	CString sValue;

	switch (nDecPlaces)
	{
	case -1:	
		sValue.Format(_T("%f"), dVal); 	
		TrimTrailingDecimalZeros(sValue);
		break;

	case 0:		
		sValue.Format(_T("%.f"), dVal); 
		break;

	default:	
		sValue.Format(_T("%.*f"), nDecPlaces, dVal); 
		break;
	}

	return (sValue + szTrail);
}

CString Misc::Format(int nVal, LPCTSTR szTrail)
{
	return (Format(_T("%ld"), nVal) + szTrail);
}

CString Misc::Format(DWORD dwVal, LPCTSTR szTrail)
{
	return (Format(_T("%lu"), dwVal) + szTrail);
}

CString Misc::Format(LPCTSTR lpszFormat, ...)
{
	ASSERT(AfxIsValidString(lpszFormat));

	va_list argList;
	va_start(argList, lpszFormat);

	CString sValue;
	sValue.FormatV(lpszFormat, argList);

	va_end(argList);

	return sValue;
}

// ----------------------------------------------------------------

#define FORMAT_REGION_VALUE(FUNC)                             \
                                                              \
{ CTempLocale loc(LC_NUMERIC, "");                            \
const UINT BUFSIZE = 100; TCHAR szValue[BUFSIZE + 1] = { 0 }; \
FUNC(NULL, 0, sValue, NULL, szValue, BUFSIZE);                \
sValue = szValue; }

// ----------------------------------------------------------------

CString Misc::FormatCost(double dCost, LPCTSTR szTrail)
{
	CString sValue(Format(_T("%.6f"), dCost));
	FORMAT_REGION_VALUE(GetCurrencyFormat);

	return (sValue + szTrail);
}

CString Misc::FormatNumber(double dVal, LPCTSTR szTrail)
{
	CString sValue(Format(_T("%.6f"), dVal));
	FORMAT_REGION_VALUE(GetNumberFormat);

	return (sValue + szTrail);
}

CString Misc::FormatNumber(int nVal, LPCTSTR szTrail)
{
	CString sValue(Format(_T("%.d"), nVal));
	FORMAT_REGION_VALUE(GetNumberFormat);

	// Because GetNumberFormat adds decimals regardless
	TrimTrailingDecimals(sValue);

	return (sValue + szTrail);
}

CString Misc::FormatNumber(LPCTSTR szVal, BOOL bAsInteger, LPCTSTR szTrail)
{
	CString sValue(szVal);
	FORMAT_REGION_VALUE(GetNumberFormat);

	if (bAsInteger)
		TrimTrailingDecimals(sValue);

	return (sValue + szTrail);
}

// ----------------------------------------------------------------

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

DWORD Misc::GetShortcut(WORD wVirtKeyCode, BOOL bExtended)
{
	BOOL bCtrl = IsKeyPressed(VK_CONTROL);
	BOOL bShift = IsKeyPressed(VK_SHIFT);
	BOOL bAlt = IsKeyPressed(VK_MENU);

	WORD wModifiers = (WORD)((bCtrl ? HOTKEYF_CONTROL : 0) |
							(bShift ? HOTKEYF_SHIFT : 0) |
							(bAlt ? HOTKEYF_ALT : 0) |
							(bExtended ? HOTKEYF_EXT : 0));

	return MAKELONG(wVirtKeyCode, wModifiers);
}

BOOL Misc::HasFlag(DWORD dwFlags, DWORD dwFlag)
{
	return (((dwFlags & dwFlag) == dwFlag) ? TRUE : FALSE);
}

BOOL Misc::FlagHasChanged(DWORD dwFlag, DWORD dwOldFlags, DWORD dwNewFlags)
{
	return ((dwOldFlags & dwFlag) != (dwNewFlags & dwFlag));
}

BOOL Misc::ModifyFlags(DWORD& dwFlags, DWORD dwRemove, DWORD dwAdd)
{
	DWORD dwOldFlags = dwFlags;
	
	// Remove before adding
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

	CString sKey(Format(sFormat, nKeyVal));

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
	return MakeKey(Format(sFormat, szKeyVal), 0, szParentKey);
}

int Misc::NaturalCompare(LPCTSTR szString1, LPCTSTR szString2, BOOL bSortEmptyBelow)
{
	if (bSortEmptyBelow)
	{
		BOOL bEmpty1 = IsEmpty(szString1);
		BOOL bEmpty2 = IsEmpty(szString2);

		if (bEmpty1 && bEmpty2)
			return 0;

		if (bEmpty1 != bEmpty2)
			return (bEmpty1 ? 1 : -1);
	}

	// initialize once only per session
	HMODULE hShlwapi = ::LoadLibrary(_T("Shlwapi.dll"));
	   
	typedef int (WINAPI *PFNSTRCMPLOGICAL)(PCWSTR, PCWSTR);
	PFNSTRCMPLOGICAL pFn = (PFNSTRCMPLOGICAL)::GetProcAddress(hShlwapi, "StrCmpLogicalW");
	
	if (pFn)
		return pFn(szString1, szString2);
	
	// all else
	return _tcsicmp(szString1, szString2);
}

//////////////////////////////////////////////////////////////

// private string compare function
int StringSortProc(const void* v1, const void* v2)
{
	const CString* pStr1 = (CString*)v1;
	const CString* pStr2 = (CString*)v2;

	return Misc::NaturalCompare(*pStr1, *pStr2);
}

void Misc::SortArray(CStringArray& aValues, SORTPROC pSortProc)
{
	if (aValues.GetSize() < 2)
		return;

	qsort(aValues.GetData(), aValues.GetSize(), sizeof(CString*), pSortProc ? pSortProc : StringSortProc);
}

//////////////////////////////////////////////////////////////

// private dword compare function
int DWordSortProc(const void* v1, const void* v2)
{
	const DWORD* pDW1 = (DWORD*)v1;
	const DWORD* pDW2 = (DWORD*)v2;

	if (*pDW1 < *pDW2)
		return -1;
	
	if (*pDW1 > *pDW2)
		return 1;

	// else
	return 0;
}

void Misc::SortArray(CDWordArray& aValues, SORTPROC pSortProc)
{
	if (aValues.GetSize() < 2)
		return;

	qsort(aValues.GetData(), aValues.GetSize(), sizeof(DWORD*), pSortProc ? pSortProc : DWordSortProc);
}

//////////////////////////////////////////////////////////////

int Misc::Copy(const CMapStringToString& mapSrc, CMapStringToString& mapDest)
{
	mapDest.RemoveAll();
	Append(mapSrc, mapDest);

	return mapDest.GetCount();
}

int Misc::Append(const CMapStringToString& mapSrc, CMapStringToString& mapDest)
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

	return mapDest.GetCount();
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

CString& Misc::MakeQuoted(CString& sText, TCHAR cEscapeEmbeddedQuotesWith)
{
	if (!IsQuoted(sText))
	{
		// Escape all contained quotes
		if (cEscapeEmbeddedQuotesWith)
		{
			TCHAR szEscapedQuote[] = { cEscapeEmbeddedQuotesWith, '\"', 0 };
			sText.Replace(DBL_QUOTE, szEscapedQuote);
		}
		
		// wrap with quotes
		sText = DBL_QUOTE + sText + DBL_QUOTE;
	}

	return sText;
}

CString& Misc::MakeUnquoted(CString& sText, TCHAR cUnescapeEmbeddedQuotesWith)
{
	if (IsQuoted(sText))
	{
		sText = sText.Mid(1, (sText.GetLength() - 2));
		
		// Un-escape all contained quotes
		if (cUnescapeEmbeddedQuotesWith)
		{
			TCHAR szEscapedQuote[] = { cUnescapeEmbeddedQuotesWith, '\"', 0 };
			sText.Replace(szEscapedQuote, DBL_QUOTE);
		}
	}

	return sText;
}

CString Misc::GetQuoted(LPCTSTR szText, TCHAR cEscapeEmbeddedQuotesWith)
{
	CString sText(szText);
	MakeQuoted(sText, cEscapeEmbeddedQuotesWith);
	
	return sText;
}

CString Misc::GetUnquoted(LPCTSTR szText, TCHAR cEscapeEmbeddedQuotesWith)
{
	CString sText(szText);
	MakeUnquoted(sText, cEscapeEmbeddedQuotesWith);

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
