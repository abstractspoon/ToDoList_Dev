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

class CWinClasses  
{
public:
	static CString GetClass(HWND hWnd);
	static CString GetClassEx(HWND hWnd); // will also determine the base type of mfc (Afx) classes
	static BOOL IsControlClass(HWND hWnd);
	static BOOL IsControlClass(LPCTSTR szClass);
	static BOOL IsClass(HWND hWnd, LPCTSTR szClass);
	static BOOL IsClassEx(HWND hWnd, LPCTSTR szClass);
	static BOOL IsClass(LPCTSTR szClass, LPCTSTR szWndClass);
	static BOOL IsClassEx(LPCTSTR szClass, LPCTSTR szWndClass);

	static BOOL IsDialog(HWND hWnd);
	static BOOL IsPropertyPage(HWND hWnd);
	static BOOL IsCommonDialog(HWND hWnd, WCLS_COMMONDIALOG nType = WCD_ANY);
	static BOOL IsKindOf(HWND hWnd, const CRuntimeClass* pClass);

	static BOOL IsEditControl(LPCTSTR szClass, BOOL bOrRichEdit = TRUE);
	static BOOL IsEditControl(HWND hWnd, BOOL bOrRichEdit = TRUE);
	static BOOL IsRichEditControl(LPCTSTR szClass);
	static BOOL IsRichEditControl(HWND hWnd);
	static BOOL IsComboBox(LPCTSTR szClass);
	static BOOL IsComboBox(HWND hWnd);
	static BOOL IsListBox(LPCTSTR szClass);
	static BOOL IsListBox(HWND hWnd);
	static BOOL IsChild(HWND hWnd);

	static int GetStyleType(HWND hWnd, DWORD dwTypeMask);
	
protected:
	static CMapStringToPtr s_mapCtrlClasses;
};

#endif // !defined(AFX_WINCLASSES_H__4A958ACC_A399_4B69_A988_FA70F16C3EAC__INCLUDED_)
