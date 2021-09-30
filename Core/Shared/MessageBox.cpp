// CDialogHelper.cpp: implementation of the CDialogHelper class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MessageBox.h"
#include "autoflag.h"
#include "winclasses.h"
#include "wclassdefines.h"
#include "misc.h"
#include "runtimedlg.h"
#include "enstring.h"
#include "AcceleratorString.h"

#include <afxpriv.h>
#include <afxtempl.h>

#include <float.h>
#include <locale.h>
#include <math.h>

//////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////

const CString DELIMS(_T(".,;:-?"));

//////////////////////////////////////////////////////////////////////

typedef HRESULT (CALLBACK *PFNTASKDIALOGCALLBACK)(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, long/*LONG_PTR*/ lpRefData);

struct TASKDIALOGBUTTON
{
	int     nButtonID;
	PCWSTR  pszButtonText;
};

struct TASKDIALOGCONFIGEX 
{
	UINT                           cbSize;
	HWND                           hwndParent;
	HINSTANCE                      hInstance;
	int				               dwFlags;
	int							   dwCommonButtons;
	PCWSTR                         pszWindowTitle;
	union 
	{
		HICON  hMainIcon;
		PCWSTR pszMainIcon;
	};
	PCWSTR                         pszMainInstruction;
	PCWSTR                         pszContent;
	UINT                           cButtons;
	const TASKDIALOGBUTTON         *pButtons;
	int                            nDefaultButton;
	UINT                           cRadioButtons;
	const TASKDIALOGBUTTON         *pRadioButtons;
	int                            nDefaultRadioButton;
	PCWSTR                         pszVerificationText;
	PCWSTR                         pszExpandedInformation;
	PCWSTR                         pszExpandedControlText;
	PCWSTR                         pszCollapsedControlText;
	union 
	{
		HICON  hFooterIcon;
		PCWSTR pszFooterIcon;
	};
	PCWSTR                         pszFooter;
	PFNTASKDIALOGCALLBACK          pfCallback;
	long/*LONG_PTR*/               lpCallbackData;
	UINT                           cxWidth;
};

//////////////////////////////////////////////////////////////////////

typedef HRESULT (WINAPI *PFNTASKDIALOG)(HWND, HINSTANCE, PCWSTR, PCWSTR, PCWSTR, DWORD, PCWSTR, int*);
typedef HRESULT (WINAPI *PFNTASKDIALOGINDIRECT)(const TASKDIALOGCONFIGEX*, int*, int*, BOOL*);

//////////////////////////////////////////////////////////////////////

#ifndef TD_WARNING_ICON
	
#define TD_WARNING_ICON         MAKEINTRESOURCEW(-1)
#define TD_ERROR_ICON           MAKEINTRESOURCEW(-2)
#define TD_INFORMATION_ICON     MAKEINTRESOURCEW(-3)
#define TD_SHIELD_ICON          MAKEINTRESOURCEW(-4)
	
#define TDF_POSITION_RELATIVE_TO_WINDOW  0x1000

enum 
{
	TDCBF_OK_BUTTON            = 0x0001, // selected control return value IDOK
	TDCBF_YES_BUTTON           = 0x0002, // selected control return value IDYES
	TDCBF_NO_BUTTON            = 0x0004, // selected control return value IDNO
	TDCBF_CANCEL_BUTTON        = 0x0008, // selected control return value IDCANCEL
	TDCBF_RETRY_BUTTON         = 0x0010, // selected control return value IDRETRY
	TDCBF_CLOSE_BUTTON         = 0x0020  // selected control return value IDCLOSE
};
	
#endif // TD_WARNING_ICON

//////////////////////////////////////////////////////////////////////

CString CMessageBox::s_sAppName;
BOOL CMessageBox::s_bDisableSimpleErrorMessages = FALSE;

//////////////////////////////////////////////////////////////////////

void CMessageBox::SetAppName(const CString& sAppName)
{
	if (sAppName.IsEmpty())
		s_sAppName = AfxGetAppName();
	else
		s_sAppName = sAppName;
}

int CMessageBox::AfxShow(const CString& sInstruction, const CString& sText, UINT nFlags)
{
	if (s_sAppName.IsEmpty())
		s_sAppName = AfxGetAppName();

	CWnd* pParent = AfxGetMainWnd();

	if (!pParent || !pParent->IsWindowEnabled())
	{
		pParent = CWnd::GetForegroundWindow();
	}

	return Show(pParent, s_sAppName, sInstruction, sText, nFlags);
}

int CMessageBox::AfxShow(UINT nInstructionID, const CString& sText, UINT nFlags)
{
	return AfxShow(CEnString(nInstructionID), sText, nFlags);
}

int CMessageBox::AfxShow(UINT nInstructionID, UINT nTextID, UINT nFlags)
{
	return AfxShow(nInstructionID, CEnString(nTextID), nFlags);
}

int CMessageBox::AfxShow(const CString& sMessage, UINT nFlags)
{
	CString sText, sInstruction(sMessage);

	if (Misc::Split(sInstruction, sText, '|'))
		return AfxShow(sInstruction, sText, nFlags);

	// else
	return AfxShow((LPCTSTR)NULL, sMessage, nFlags);
}

int CMessageBox::AfxShow(UINT nTextID, UINT nFlags)
{
	return AfxShow(CEnString(nTextID), nFlags);
}

int CMessageBox::AfxShow(HWND hwndParent, const CString& sInstruction, const CString& sText, UINT nFlags)
{
	if (!hwndParent)
		return AfxShow(sInstruction, sText, nFlags);

	// else
	return Show(hwndParent, s_sAppName, sInstruction, sText, nFlags);
}

int CMessageBox::AfxShow(const CWnd* pWnd, const CString& sInstruction, const CString& sText, UINT nFlags)
{
	if (!pWnd)
		return AfxShow(sInstruction, sText, nFlags);

	// else
	return Show(pWnd, s_sAppName, sInstruction, sText, nFlags);
}

int CMessageBox::Show(const CWnd* pWnd, const CString& sCaption, const CString& sInstruction, const CString& sText, UINT nFlags)
{
	if (pWnd)
		return Show(*pWnd, sCaption, sInstruction, sText, nFlags);

	// else
	return Show((HWND)NULL, sCaption, sInstruction, sText, nFlags);
}

int CMessageBox::Show(HWND hwndParent, const CString& sCaption, const CString& sInstruction, const CString& sText, UINT nFlags)
{
	int nIcon = (nFlags & MB_ICONMASK);
	int nButtons = (nFlags & MB_TYPEMASK);

	if (s_bDisableSimpleErrorMessages)
	{
		if ((nIcon == MB_ICONERROR) && (nButtons == MB_OK))
			return IDOK;
	}

	HMODULE hMod = ::LoadLibrary(_T("Comctl32.dll"));
	
	if (hMod)
	{
		//PFNTASKDIALOG pFn = (PFNTASKDIALOG)::GetProcAddress(hMod, "TaskDialog");
		PFNTASKDIALOGINDIRECT pFn = (PFNTASKDIALOGINDIRECT)::GetProcAddress(hMod, "TaskDialogIndirect");
		
		if (pFn)
		{
			// convert string to unicode as required
#ifdef _UNICODE
			LPCWSTR wszCaption = sCaption;
			LPCWSTR wszInstruction = sInstruction;

			// copy because we will modify
			LPWSTR wszText = _tcsdup(sText);
#else
			LPWSTR wszCaption = Misc::MultiByteToWide(szCaption);
			LPWSTR wszText = Misc::MultiByteToWide(szText);
			LPWSTR wszInstruction = Misc::MultiByteToWide(szInstruction);
#endif
			
			int nResult = 0;

			// remove single '\r' and '\n' unless they follow a 
			// punctuation mark or another '\r' or \n'
			LPTSTR pstrSource = wszText;
			LPTSTR pstrDest = wszText;
			LPTSTR pstrEnd = (pstrSource + wcslen(wszText));
			TCHAR cPrev = 0;
			
			while (pstrSource < pstrEnd)
			{
#define ISLINEEND(c) ((c == '\r') || (c == '\n'))
				
				if (!ISLINEEND(*pstrSource) || ISLINEEND(cPrev) || (DELIMS.Find(cPrev) >= 0))
				{
					*pstrDest = *pstrSource;
					pstrDest = _tcsinc(pstrDest);
				}

				cPrev = *pstrSource;
				pstrSource = _tcsinc(pstrSource);
			}
			*pstrDest = '\0';
			
			// icon
			LPCWSTR wszIcon = NULL;
			switch (nIcon)
			{
			case MB_ICONQUESTION:
			case MB_ICONINFORMATION:
				wszIcon = TD_INFORMATION_ICON;	
				break;

			case MB_ICONWARNING:		
				wszIcon = TD_WARNING_ICON;		
				break;

			case MB_ICONERROR:			
				wszIcon = TD_ERROR_ICON;		
				break;

			default: // no icon specified
				if (nButtons == MB_RETRYCANCEL)
					wszIcon = TD_ERROR_ICON;
				else
					wszIcon = TD_INFORMATION_ICON;
				break;
			}
			
			// buttons 
			DWORD dwButtons = 0;
			
			switch (nButtons)
			{
			case MB_OK:				
				dwButtons = TDCBF_OK_BUTTON;
				break;
			
			case MB_OKCANCEL:		
				dwButtons = (TDCBF_OK_BUTTON | TDCBF_CANCEL_BUTTON);						
				break;
			
			case MB_RETRYCANCEL:	
				dwButtons = (TDCBF_RETRY_BUTTON | TDCBF_CANCEL_BUTTON);					
				break;
			
			case MB_YESNO:			
				dwButtons = (TDCBF_YES_BUTTON | TDCBF_NO_BUTTON);							
				break;
			
			case MB_YESNOCANCEL:	
				dwButtons = (TDCBF_YES_BUTTON | TDCBF_NO_BUTTON | TDCBF_CANCEL_BUTTON);	
				break;
			}


			TASKDIALOGCONFIGEX tdc = { sizeof(tdc), 0 };

			tdc.pszWindowTitle = wszCaption;
			tdc.pszMainInstruction = wszInstruction;
			tdc.pszContent = wszText;
			tdc.pszMainIcon = wszIcon;
			tdc.dwCommonButtons = dwButtons;
			tdc.hwndParent = hwndParent;

			if (hwndParent)
				tdc.dwFlags = TDF_POSITION_RELATIVE_TO_WINDOW;

//			HRESULT hr = pFn(hwndParent, NULL, wszCaption, wszInstruction, wszText, dwButtons, wszIcon, &nResult);
			HRESULT hr = pFn(&tdc, &nResult, NULL, NULL);
			
			// clean up
#ifdef _UNICODE
			free(wszText);
#else
			delete [] wszText;
			delete [] wszCaption;
			delete [] wszInstruction;
#endif
			
			return SUCCEEDED(hr) ? nResult : IDCANCEL;
		}
	}
	
	// all else
	CEnString sTitle(sCaption);

	// instruction overrides caption
	if (!sInstruction.IsEmpty())
	{
		sTitle = sInstruction;
	}
	else if (sCaption.IsEmpty())
	{
		sTitle = AfxGetAppName();
	}
		
	return ::MessageBox(hwndParent, sText, sTitle, nFlags);
}

