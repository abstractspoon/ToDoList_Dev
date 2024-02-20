// WinClasses.cpp: implementation of the CWinClasses class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WinClasses.h"
#include "wclassdefines.h"
#include "folderdialog.h"
#include "misc.h"

#include <afxpriv.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// helper function for everyone to use
#ifdef _DEBUG
void TRACEWND(LPCTSTR szFunctionName, HWND hWnd)
{
	if (hWnd)
	{
		CString sText; 
		CWnd::FromHandle(hWnd)->GetWindowText(sText); 
		TRACE (_T("%s(%s, %s, id = %d)\n"), szFunctionName, CWinClasses::GetClass(hWnd), sText, GetDlgCtrlID(hWnd)); 
	}
}
#else
void TRACEWND(LPCTSTR, HWND) {}
#endif

#define ADDCONTROL(ctrlname) { CString sCtrl(ctrlname); sCtrl.MakeLower(); s_mapCtrlClasses[sCtrl] = NULL; }

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMapStringToPtr CWinClasses::s_mapCtrlClasses;

CString CWinClasses::GetClass(HWND hWnd)
{
	if (hWnd == NULL)
	{
		static CString EMPTY_STR;
		return EMPTY_STR;
	}

	// else
	CString sWndClass;
	int nLen = ::GetClassName(hWnd, sWndClass.GetBuffer(128), 128);

	sWndClass.ReleaseBuffer(nLen);
	sWndClass.MakeLower();

	return sWndClass;
}

BOOL CWinClasses::IsClass(LPCTSTR szClass, LPCTSTR szWndClass) 
{ 
	if (Misc::IsEmpty(szClass) || Misc::IsEmpty(szWndClass))
		return FALSE;
	
	return (_tcsicmp(szClass, szWndClass) == 0); 
}

BOOL CWinClasses::IsClass(HWND hWnd, LPCTSTR szClass)
{
	if (hWnd)
		return IsClass(szClass, GetClass(hWnd));
	
	return FALSE;
}

BOOL CWinClasses::IsClassEx(HWND hWnd, LPCTSTR szClass)
{
	if (hWnd)
	{
		CString sClass = GetClassEx(hWnd);
		
		if (IsClass(sClass, szClass))
			return TRUE;
		else
		{
			CWnd* pWnd = CWnd::FromHandlePermanent(hWnd);
			
			if (pWnd)
			{
				if (sClass == WC_MFCWND)
					return TRUE;
				
				else if (IsClass(sClass, WC_MFCFRAME) && pWnd->IsKindOf(RUNTIME_CLASS(CFrameWnd)))
					return TRUE;
				
				else if (IsClass(sClass, WC_MFCVIEW) && pWnd->IsKindOf(RUNTIME_CLASS(CView)))
					return TRUE;
				
				else if (IsClass(sClass, WC_MFCMDIFRAME) && pWnd->IsKindOf(RUNTIME_CLASS(CMDIFrameWnd)))
					return TRUE;
				
				else if (IsClass(sClass, WC_MFCMDICHILD) && pWnd->IsKindOf(RUNTIME_CLASS(CMDIChildWnd)))
					return TRUE;
				
				else if (IsClass(sClass, WC_MFCMINIDOCKFRAME) && pWnd->IsKindOf(RUNTIME_CLASS(CMiniDockFrameWnd)))
					return TRUE;
				
				else if (IsClass(sClass, WC_MFCMINIFRAME) && pWnd->IsKindOf(RUNTIME_CLASS(CMiniFrameWnd)))
					return TRUE;
				
				else if (IsClass(sClass, WC_MFCSPLITTER) && pWnd->IsKindOf(RUNTIME_CLASS(CSplitterWnd)))
					return TRUE;
				
				else if (IsClass(sClass, WC_MFCDIALOGBAR) && pWnd->IsKindOf(RUNTIME_CLASS(CDialogBar)))
					return TRUE;
				
				else if (IsClass(sClass, WC_CONTROLBAR) && pWnd->IsKindOf(RUNTIME_CLASS(CControlBar)))
					return TRUE;
			}
		}
	}
	
	return FALSE;
}

BOOL CWinClasses::IsClassEx(LPCTSTR szClass, LPCTSTR szWndClass)
{
	if (IsClass(szClass, szWndClass)) // string comparison
		return TRUE;
	
	else if (IsClass(szWndClass, WC_MFCFRAME)) // handle frame derivatives
	{
		if (IsClass(szClass, WC_MFCMDIFRAME) || IsClass(szClass, WC_MFCMDICHILD) ||
			IsClass(szClass, WC_MFCMINIDOCKFRAME) || IsClass(szClass, WC_MFCMINIFRAME))
			return TRUE;
	}
	
	return FALSE;
}

CString CWinClasses::GetClassEx(HWND hWnd)
{
	CString sClass = GetClass(hWnd);
	
	// handle CCheckListBox
	if (IsClass(sClass, WC_LISTBOX))
	{
		CWnd* pWnd = CWnd::FromHandlePermanent(hWnd);
		
		if (pWnd && pWnd->IsKindOf(RUNTIME_CLASS(CCheckListBox)))
		{
			sClass = WC_CHECKLISTBOX;
			sClass.MakeLower();
		}
	}
	else if (!IsControlClass(sClass))
	{
		// can do the check if pWnd is permanent else mfc will not yet
		// have hooked up
		CWnd* pWnd = CWnd::FromHandlePermanent(hWnd);
		
		if (pWnd)
		{
			// must do the check in order of most derived class first
			if (pWnd->IsKindOf(RUNTIME_CLASS(CView)))
				sClass = WC_MFCVIEW;
			
			else if (pWnd->IsKindOf(RUNTIME_CLASS(CMDIFrameWnd)))
				sClass = WC_MFCMDIFRAME;
			
			else if (pWnd->IsKindOf(RUNTIME_CLASS(CMDIChildWnd)))
				sClass = WC_MFCMDICHILD;
			
			else if (pWnd->IsKindOf(RUNTIME_CLASS(CMiniDockFrameWnd)))
				sClass = WC_MFCMINIDOCKFRAME;
			
			else if (pWnd->IsKindOf(RUNTIME_CLASS(CMiniFrameWnd)))
				sClass = WC_MFCMINIFRAME;
			
			else if (pWnd->IsKindOf(RUNTIME_CLASS(CFrameWnd))) // this is the catch all for frame wnds
				sClass = WC_MFCFRAME;
			
			else if (pWnd->IsKindOf(RUNTIME_CLASS(CSplitterWnd)))
				sClass = WC_MFCSPLITTER;
			
			else if (pWnd->IsKindOf(RUNTIME_CLASS(CDialogBar)))
				sClass = WC_MFCDIALOGBAR;
			
			else if (pWnd->IsKindOf(RUNTIME_CLASS(CControlBar)))
				sClass = WC_CONTROLBAR;

			sClass.MakeLower();
		}
	}

	return sClass;
}

BOOL CWinClasses::IsControlClass(HWND hWnd)
{
	return IsControlClass(GetClass(hWnd));
}

BOOL CWinClasses::IsControlClass(LPCTSTR szClass)
{
	// init
	if (!s_mapCtrlClasses.GetCount())
	{
		ADDCONTROL(WC_BUTTON);
		ADDCONTROL(WC_STATIC);
		ADDCONTROL(WC_EDIT);  
		ADDCONTROL(WC_COMBOBOX);
		ADDCONTROL(WC_COMBOLBOX);
		ADDCONTROL(WC_LISTBOX);  
		ADDCONTROL(WC_COMBOBOXEX);     
		ADDCONTROL(WC_SCROLLBAR);
		ADDCONTROL(WC_TOOLBAR);  
		ADDCONTROL(WC_SPIN);     
		ADDCONTROL(WC_PROGRESS); 
		ADDCONTROL(WC_SLIDER);   
		ADDCONTROL(WC_HOTKEY);   
		ADDCONTROL(WC_SHELLDLLDEFVIEW);  
		ADDCONTROL(WC_STATUSBAR);        
		ADDCONTROL(WC_ANIMATE);          
		ADDCONTROL(WC_RICHEDIT);         
		ADDCONTROL(WC_RICHEDIT20);       
		ADDCONTROL(WC_RICHEDIT50);  
		ADDCONTROL(WC_DATETIMEPICK);     
		ADDCONTROL(WC_MONTHCAL);         
		ADDCONTROL(WC_REBAR);            
		ADDCONTROL(WC_TOOLTIPS); 
		ADDCONTROL(WC_THUMBNAILVIEW); 
		ADDCONTROL(WC_IE); 
		ADDCONTROL(WC_SHDOCOBJVW); 
		ADDCONTROL(WC_SHELLEMBEDDING); 
	}
	
	void* pTemp;
	CString sClass(szClass);
	sClass.MakeLower();
	
	return s_mapCtrlClasses.Lookup(sClass, pTemp);
}

BOOL CWinClasses::IsEditControl(HWND hWnd, BOOL bOrRichEdit)
{
	return IsEditControl(GetClass(hWnd), bOrRichEdit);
}

BOOL CWinClasses::IsEditControl(LPCTSTR szClass, BOOL bOrRichEdit)
{
	if (IsClass(szClass, WC_EDIT))
		return TRUE;

	if (bOrRichEdit)
		return IsRichEditControl(szClass);

	return FALSE;
}

BOOL CWinClasses::IsRichEditControl(HWND hWnd)
{
	return IsRichEditControl(GetClass(hWnd));
}

BOOL CWinClasses::IsRichEditControl(LPCTSTR szClass)
{
	return (IsClass(szClass, WC_RICHEDIT) ||
			IsClass(szClass, WC_RICHEDIT20) ||
			IsClass(szClass, WC_RICHEDIT50));
}

BOOL CWinClasses::IsComboBox(HWND hWnd)
{
	return IsComboBox(GetClass(hWnd));
}

BOOL CWinClasses::IsComboBox(LPCTSTR szClass)
{
	return (IsClass(szClass, WC_COMBOBOX) || IsClass(szClass, WC_COMBOBOXEX));
}

BOOL CWinClasses::IsListBox(HWND hWnd)
{
	return IsListBox(GetClass(hWnd));
}

BOOL CWinClasses::IsListBox(LPCTSTR szClass)
{
	return (IsClass(szClass, WC_LISTBOX) || IsClass(szClass, WC_CHECKLISTBOX));
}

BOOL CWinClasses::IsDialog(HWND hWnd)
{
	return IsClass(hWnd, WC_DIALOGBOX);
}

BOOL CWinClasses::IsPropertyPage(HWND hWnd)
{
	return IsKindOf(hWnd, RUNTIME_CLASS(CPropertyPage));
}

BOOL CWinClasses::IsKindOf(HWND hWnd, const CRuntimeClass* pClass)
{
	CWnd* pWnd = CWnd::FromHandle(hWnd);

	if (!pWnd || !pClass)
		return FALSE;

	// simple Single inheritance case: copied from CWnd::IsKindOf
	const CRuntimeClass* pWndClass = pWnd->GetRuntimeClass();
	ASSERT(pWndClass);

	while (pWndClass != NULL)
	{
		if (pWndClass == pClass)
			return TRUE;

		// This is our extra part to work around problems cause by
		// multiple hooking of classes
		if (strcmp(pWndClass->m_lpszClassName, pClass->m_lpszClassName) == 0)
			return TRUE;

		if (pWndClass->m_pfnGetBaseClass == NULL)
			break;

		pWndClass = (*pWndClass->m_pfnGetBaseClass)();
	}

	// walked to the top, no match
	return FALSE;       
}

BOOL CWinClasses::IsChild(HWND hWnd)
{
	return (::GetWindowLong(hWnd, GWL_STYLE) & WS_CHILD);
}

BOOL CWinClasses::IsCommonDialog(HWND hWnd, WCLS_COMMONDIALOG nType)
{
	// special case
	if (nType == WCD_ANY)
	{
		return IsCommonDialog(hWnd, WCD_PRINT) ||
				IsCommonDialog(hWnd, WCD_PAGESETUP) ||
				IsCommonDialog(hWnd, WCD_FONT) ||
				IsCommonDialog(hWnd, WCD_COLOR) ||
				IsCommonDialog(hWnd, WCD_FINDREPLACE) ||
				IsCommonDialog(hWnd, WCD_OPENSAVE) ||
				IsCommonDialog(hWnd, WCD_BROWSEFOLDER);
	}

	// all else
	if (!IsDialog(hWnd))
		return FALSE;

	// special cases
	if (nType == WCD_OPENSAVE)
	{
		// The open save dialog we are interested in is actually
		// a child of the dialog that users see
		CWnd* pChild = CWnd::FromHandle(hWnd)->GetDlgItem(0);
		return (pChild && pChild->IsKindOf(RUNTIME_CLASS(CFileDialog)));
	}
	/*
	// For a while the method of getting the runtime class was not working
	// for unknown reasons. So I used this heuristic to solve the problem.
	// But I still want to preserve it because I don't know when it might
	// stop working again.
	else if (nType == WCD_FONT)
	{
		// Can't get method below to work so we try a heuristic
		const UINT NONCOMBOS[] = { 1088, 1089, 1090, 1072, 1040, 1041, 1094, 1073 };
		const int NUM_NONCOMBOS = sizeof(NONCOMBOS) / sizeof(UINT);

		const UINT OWNERDRAWCOMBOS[] = { 1140, 1136, 1137, 1138, 1139 };
		const int NUM_OWNERDRAWCOMBOS = sizeof(OWNERDRAWCOMBOS) / sizeof(UINT);

		for (int nNonCombo = 0; nNonCombo < NUM_NONCOMBOS; nNonCombo++)
		{
			if (::GetDlgItem(hWnd, NONCOMBOS[nNonCombo]) == NULL)
				return FALSE;
		}

		for (int nCombo = 0; nCombo < NUM_OWNERDRAWCOMBOS; nCombo++)
		{
			HWND hwndCombo = ::GetDlgItem(hWnd, OWNERDRAWCOMBOS[nCombo]);

			if (hwndCombo == NULL)
				return FALSE;

			if (!IsClass(hwndCombo, WC_COMBOBOX))
				return FALSE;

			BOOL bOwnerDraw = (::GetWindowLong(hwndCombo, GWL_STYLE) & CBS_OWNERDRAWFIXED);

			if (!bOwnerDraw)
				return FALSE;
		}

		return TRUE;
	}
	*/

	// get permanent window mapping so we can
	// lookup runtime class type. 
	CWnd* pDlg = CWnd::FromHandlePermanent(hWnd);

	if (pDlg == NULL)
		return FALSE;

	switch (nType)
	{
	case WCD_PRINT:
		return pDlg->IsKindOf(RUNTIME_CLASS(CPrintDialog));

	case WCD_PAGESETUP:
		return pDlg->IsKindOf(RUNTIME_CLASS(CPageSetupDialog));

	case WCD_FONT:
		return pDlg->IsKindOf(RUNTIME_CLASS(CFontDialog));

	case WCD_COLOR:
		return pDlg->IsKindOf(RUNTIME_CLASS(CColorDialog));

	case WCD_FINDREPLACE:
		return pDlg->IsKindOf(RUNTIME_CLASS(CFindReplaceDialog));

	case WCD_BROWSEFOLDER:
		return pDlg->IsKindOf(RUNTIME_CLASS(CFolderDialog));
	}

	// how do we get here?
	ASSERT(0);
	return FALSE;
}

int CWinClasses::GetStyleType(HWND hWnd, DWORD dwTypeMask)
{
	return (::GetWindowLong(hWnd, GWL_STYLE) & dwTypeMask);
}
