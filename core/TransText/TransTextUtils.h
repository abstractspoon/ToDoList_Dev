// TransTextMgr.h: interface for the CTransTextMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TRANSUTILS_H__1F1C6D57_D2C6_4C06_9E7C_1E0C54B65D30__INCLUDED_)
#define AFX_TRANSUTILS_H__1F1C6D57_D2C6_4C06_9E7C_1E0C54B65D30__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////

namespace TransText
{
	CString& DecodeChars(CString& sText);
	CString& EncodeChars(CString& sText);
	CString GetFriendlyClass(const CString& sClass, HWND hWndRef = NULL);
	CString GetClassIDName(HWND hWnd);
	CString GetTextClassIDName();
	CString GetMenuClassIDName();
	BOOL PrepareLookupText(CString& sText, BOOL bDecodeChars = TRUE);
	BOOL IsOwnerDraw(int nCmdID, HMENU hMenu);
	BOOL IsPopup(HWND hWnd);

	BOOL CleanupDictionary(LPCTSTR szMasterDictPath, LPCTSTR szDictPath);

	BOOL RemoveAccelerator(CString& sText);
	BOOL EnsureAccelerator(CString& sText, HWND hWndRef = NULL);
	BOOL EnsureAccelerator(CString& sText, HMENU hMenu);
	BOOL ClassWantsAccelerator(const CString& sClass);

}

#endif // !defined(AFX_TRANSUTILS_H__1F1C6D57_D2C6_4C06_9E7C_1E0C54B65D30__INCLUDED_)
