// TransTextMgr.cpp: implementation of the CTransTextMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TransTextMgr.h"
#include "TransTextUtils.h"

#include "..\shared\wclassdefines.h"
#include "..\shared\winclasses.h"
#include "..\shared\misc.h"
#include "..\shared\filemisc.h"
#include "..\shared\dialogHelper.h"
#include "..\shared\enmenu.h"
#include "..\shared\AcceleratorString.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

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
		return GetTextClassIDName();
	
	return GetFriendlyClass(CWinClasses::GetClassEx(hWnd), hWnd);
}

CString TransText::GetMenuClassIDName()
{
	return GetFriendlyClass(WC_MENU);
}

CString TransText::GetTextClassIDName()
{
	return _T("text");
}

BOOL TransText::PrepareLookupText(CString& sText, BOOL bDecodeChars)
{
	// remove trailing/leading spaces and delimiters
	Misc::Trim(sText, ' ').TrimRight(':');
	
	// remove accelerators
	if (sText.Find('&') >= 0)
		sText.Remove('&');
	
	// check for numbers and symbols
	if (Misc::IsNumber(sText) || Misc::IsSymbol(sText))
		return FALSE;
	
	// finally decode 'tricky' characters like tabs and newlines
	if (bDecodeChars)
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

BOOL TransText::CleanupDictionary(LPCTSTR szMasterDictPath, LPCTSTR szDictPath)
{
	if (FileMisc::IsSamePath(szMasterDictPath, szDictPath))
		return FALSE;
	
	CTransDictionary dtActive, dtMaster, dtRemoved;
	
	if (!dtActive.LoadDictionary(szDictPath) || !dtMaster.LoadDictionary(szMasterDictPath))
		return FALSE;
	
	TD_CLEANUP nRes = dtActive.CleanupDictionary(dtMaster, dtRemoved);

	switch (nRes)
	{
	case TDCLEAN_CHANGE:
	case TDCLEAN_NOCHANGE:
		// Always save the 'target' so that newly translated strings get
		// correctly moved to the 'TRANSLATED' section
		dtActive.SaveDictionary(NULL, TRUE);

		// save any removed items
		if (!dtRemoved.IsEmpty())
		{
			CString sRemovedPath = CFileBackup::BuildBackupPath(szDictPath, FBS_DATETIMESTAMP, _T("backup"), _T(".removed"));
		
			VERIFY(FileMisc::CreateFolderFromFilePath(sRemovedPath));
			VERIFY(dtRemoved.SaveDictionary(sRemovedPath, TRUE));
		}
		break;

	case TDCLEAN_BADVER:
	case TDCLEAN_EMPTY:
		break;

	default:
		ASSERT(0);
		break;
	}
	
	return (nRes == TDCLEAN_CHANGE);
}

BOOL TransText::RemoveAccelerator(CString& sText)
{
	return CAcceleratorString::RemoveAccelerator(sText);
}

BOOL TransText::EnsureAccelerator(CString& sText, HWND hWndRef)
{
	return (CDialogHelper::EnsureUniqueAccelerator(sText, hWndRef) != 0);
}

BOOL TransText::EnsureAccelerator(CString& sText, HMENU hMenu)
{
	return (CEnMenu::EnsureUniqueAccelerator(sText, hMenu) != 0);
}

BOOL TransText::ClassWantsAccelerator(const CString& sClass)
{
	if (sClass.IsEmpty())
		return FALSE;

	if (CWinClasses::IsClass(sClass, WC_BUTTON) ||
		CWinClasses::IsClass(sClass, WC_STATIC) ||
		CWinClasses::IsClass(sClass, WC_MENU))
	{
		return TRUE;
	}

	return FALSE;
}
