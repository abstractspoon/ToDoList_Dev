// TransTextMgr.cpp: implementation of the CTransTextMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TransTextMgr.h"
#include "TransTextUtils.h"

#include "..\shared\wclassdefines.h"
#include "..\shared\winclasses.h"
#include "..\shared\misc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////

static const CString SPACECHAR			= _T(" ");

//////////////////////////////////////////////////////////////////////

CString& TransText::DecodeChars(CString& sText)
{
	if (!sText.IsEmpty())
	{
		if (sText.Find(_T("\\t")) >= 0)
			sText.Replace(_T("\\t"), _T("\t"));
		
		if (sText.Find(_T("\\r")) >= 0)
			sText.Replace(_T("\\r"), _T("\r"));
		
		if (sText.Find(_T("\\n")) >= 0)
			sText.Replace(_T("\\n"), _T("\n"));
	}
	
	return sText;
}

CString& TransText::EncodeChars(CString& sText)
{
	if (!sText.IsEmpty())
	{
		if (sText.Find('\t') >= 0)
			sText.Replace(_T("\t"), _T("\\t"));
		
		if (sText.Find('\r') >= 0)
			sText.Replace(_T("\r"), _T("\\r"));
		
		if (sText.Find('\n') >= 0)
			sText.Replace(_T("\n"), _T("\\n"));
	}
	
	return sText;
}

CString TransText::GetFriendlyClass(const CString& sClass, HWND hWndRef) 
{
	if (CWinClasses::IsClass(sClass, WC_DIALOGBOX))
	{
		return _T("dialog");
	}	
	else if (CWinClasses::IsClass(sClass, WC_STATIC))
	{
		return _T("label");
	}
	else if (CWinClasses::IsClass(sClass, WC_MENU))
	{
		return _T("menu");
	}
	else if (CWinClasses::IsClass(sClass, WC_TOOLTIPS))
	{
		return _T("tooltip");
	}
	else if (CWinClasses::IsClass(sClass, WC_BUTTON) && hWndRef)
	{
		int nBtnType = CWinClasses::GetButtonType(hWndRef);
		
		switch (nBtnType)
		{
		case BS_CHECKBOX:         
		case BS_AUTOCHECKBOX:  
		case BS_3STATE:           
		case BS_AUTO3STATE:  
			return _T("checkbox");
			
		case BS_RADIOBUTTON:      
		case BS_AUTORADIOBUTTON:
			return _T("radiobutton");
			
		case BS_GROUPBOX:      
			return _T("groupbox");
		}
		// drop thru
	}
	
	return sClass;
}

CString TransText::GetClassIDName(HWND hWnd)
{
	if (hWnd == NULL)
		return _T("text");
	
	CString sClass = GetFriendlyClass(CWinClasses::GetClassEx(hWnd), hWnd);
	
	// do we have a 'valid' id
	int nCtrlID = ::GetDlgCtrlID(hWnd);
	
	if (nCtrlID <= 0 || nCtrlID >= 0xffff)
		return sClass;
	
	// else
	CString sName;
	sName.Format(_T("%s.%d"), sClass, nCtrlID);
	
	return sName;
}

CString TransText::GetClassIDName(HMENU /*hMenu*/, int nMenuID)
{
	CString sClass = GetFriendlyClass(WC_MENU);
	
	if (nMenuID < 0)
		return sClass;
	
	CString sName;
	sName.Format(_T("%s.%d"), sClass, nMenuID);	
	
	return sName;
}

BOOL TransText::PrepareLookupText(CString& sText)
{
	// remove trailing/leading spaces
	Misc::Trim(sText, SPACECHAR).TrimRight(':');
	
	// remove accelerators
	if (sText.Find('&') >= 0)
		sText.Remove('&');
	
	// check for numbers and symbols
	if (Misc::IsNumber(sText) || Misc::IsSymbol(sText))
	{
		CTransTextMgr::IgnoreString(sText, FALSE);
		return FALSE;
	}
	
	// finally decode 'tricky' characters like tabs and newlines
	DecodeChars(sText);
	
	return !sText.IsEmpty();
}

BOOL TransText::IsOwnerDraw(int nCmdID, HMENU hMenu)
{
	if (nCmdID > 0)
	{
		MENUITEMINFO mii;
		ZeroMemory(&mii, sizeof(mii));
		
		mii.cbSize = sizeof(mii);
		mii.fMask = MIIM_TYPE;
		
		if (GetMenuItemInfo(hMenu, nCmdID, FALSE, &mii))
			return (mii.fType & MFT_OWNERDRAW);
	}
	
	return FALSE;
}

BOOL TransText::IsPopup(HWND hWnd)
{
	DWORD dwStyle = GetWindowLong(hWnd, GWL_STYLE);
	DWORD dwMask = (WS_POPUP | WS_CAPTION);
	
	return (dwStyle & dwMask);
}
