// WinClasses.h: interface for the CWinClasses class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WINCLASSES_H__4A958ACC_A399_4B69_A988_FA70F16C3EAC__INCLUDED_)
#define AFX_WINCLASSES_H__4A958ACC_A399_4B69_A988_FA70F16C3EAC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////

struct CRuntimeClass;

//////////////////////////////////////////////////////////////////////

// global helper function for everyone to use
void TRACEWND(LPCTSTR szFunctionName, HWND hWnd);

//////////////////////////////////////////////////////////////////////

enum WCLS_COMMONDIALOG
{
	WCD_ANY,
	WCD_PRINT,
	WCD_PAGESETUP,
	WCD_FONT,
	WCD_COLOR,
	WCD_FINDREPLACE,
	WCD_OPENSAVE,
	WCD_BROWSEFOLDER,
};

//////////////////////////////////////////////////////////////////////

#ifndef BS_TYPEMASK
#define BS_TYPEMASK 0x0000000FL
#endif

#define CBS_TYPEMASK 0x0000000FL

//////////////////////////////////////////////////////////////////////

// Pseudo-classes returned from IsClassEx functions only
const LPCTSTR WC_MFCDIALOGBAR     = L"MfcDialogBar";
const LPCTSTR WC_MFCVIEW          = L"MfcView";
const LPCTSTR WC_MFCMDICHILD      = L"MfcFrameMDIChild";
const LPCTSTR WC_MFCMDIFRAME      = L"MfcFrameMDI";
const LPCTSTR WC_MFCSPLITTER      = L"MfcSplitter";
const LPCTSTR WC_MFCFRAME         = L"MfcFrame";
const LPCTSTR WC_MFCMINIDOCKFRAME = L"MfcFrameMiniDock";
const LPCTSTR WC_MFCMINIFRAME     = L"MfcFrameMini";
const LPCTSTR WC_MFCWND           = L"MfcWnd";

//////////////////////////////////////////////////////////////////////

class CWinClasses  
{
public:
	static CString GetClass(HWND hWnd);
	static BOOL IsControlClass(HWND hWnd);
	static BOOL IsControlClass(LPCTSTR szClass);
	static BOOL IsClass(HWND hWnd, LPCTSTR szClass);
	static BOOL IsClass(LPCTSTR szClass, LPCTSTR szWndClass);

	static BOOL HasParentClass(HWND hWnd, LPCTSTR szClass, BOOL bRecursive = FALSE);

	static CString GetClassEx(HWND hWnd);
	static BOOL IsClassEx(HWND hWnd, LPCTSTR szClass);
	static BOOL IsClassEx(LPCTSTR szClass, LPCTSTR szWndClass);

	static BOOL IsMFCPropertyPage(HWND hWnd);
	static BOOL IsMFCCommonDialog(HWND hWnd, WCLS_COMMONDIALOG nType = WCD_ANY);
	static BOOL IsKindOf(HWND hWnd, const CRuntimeClass* pClass);

	static BOOL IsDialog(HWND hWnd);
	static BOOL IsEditControl(HWND hWnd, BOOL bOrRichEdit = TRUE);
	static BOOL IsRichEditControl(HWND hWnd);
	static BOOL IsComboBox(HWND hWnd);
	static BOOL IsListBox(HWND hWnd);
	static BOOL IsWinFormsControl(HWND hWnd, LPCTSTR szWndClass = NULL);
	static BOOL IsChild(HWND hWnd);

	static BOOL IsEditControl(LPCTSTR szClass, BOOL bOrRichEdit = TRUE);
	static BOOL IsRichEditControl(LPCTSTR szClass);
	static BOOL IsComboBox(LPCTSTR szClass);
	static BOOL IsListBox(LPCTSTR szClass);
	static BOOL IsWinFormsControl(LPCTSTR szClass, LPCTSTR szWndClass = NULL);

	static int GetStyleType(HWND hWnd, DWORD dwTypeMask);
	
protected:
	static CMapStringToPtr s_mapCtrlClasses;
};

#endif // !defined(AFX_WINCLASSES_H__4A958ACC_A399_4B69_A988_FA70F16C3EAC__INCLUDED_)
