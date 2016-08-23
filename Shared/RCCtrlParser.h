// RCCtrlParser.h: interface for the CRCCtrlParser class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RCCTRLPARSER_H__8273AEF0_5E4D_4C6D_B391_FC6192A39314__INCLUDED_)
#define AFX_RCCTRLPARSER_H__8273AEF0_5E4D_4C6D_B391_FC6192A39314__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class RTCONTROL
{
public:
	RTCONTROL()
	{
		m_pWnd = NULL;
		m_dwStyle = 0; 
		m_dwExStyle = 0; 
		m_rect.SetRectEmpty();
		m_nID = 0;
		m_bDLU = FALSE;
		m_nIconID = 0;
	}

	RTCONTROL(CWnd* pWnd, LPCTSTR szClass, LPCTSTR szCaption, DWORD dwStyle, DWORD dwExStyle, 
				const CRect& rect, UINT nID, BOOL bDLU, UINT nIconID = 0) 
	{
		ASSERT (pWnd || lstrlen(szClass));

		m_pWnd = pWnd;
		m_sClass = szClass;
		m_sCaption = szCaption; 
		m_dwStyle = dwStyle; 
		m_dwExStyle = dwExStyle; 
		m_rect = rect;
		m_nID = nID;
		m_bDLU = bDLU;
		m_nIconID = nIconID; 
	}
		
	CWnd* m_pWnd;
	CString m_sClass;
	CString m_sCaption; 
	DWORD m_dwStyle; 
	DWORD m_dwExStyle; 
	CRect m_rect;
	UINT m_nID;
	BOOL m_bDLU;
	UINT m_nIconID;
};

#include <afxtempl.h>
typedef CList<RTCONTROL, RTCONTROL&> CRTCtrlList;

// special style equivalent to "NOT VISIBLE" in RC definition
const UINT WS_NOTVISIBLE = WS_POPUP;

class CRCCtrlParser  
{
public:
	CRCCtrlParser(LPCTSTR szRCControls = NULL);
	virtual ~CRCCtrlParser();

	virtual int ParseRCControls(const CString& sRCControls); // text straight from an RC file
	int GetRCControls(CRTCtrlList& lstControls) const;

	static BOOL CtrlWantsClientEdge(LPCTSTR szClass);
	static DWORD GetDefaultStyles(LPCTSTR szRCType);
	static BOOL GetClassName(LPCTSTR szRCType, CString& sClass);

protected:
	CRTCtrlList m_lstControls;

	BOOL AddRCControl(const CString& sRCCtrl);

	// for processing the RC text
	static int FindNextRCControl(const CString& sRCControls, int nStart = 0);

	static int GetRCControlTypeIndex(const CString& sRCCtrl);
	static int ParseRCControl(const CString& sRCCtrl, CString& sCaption, UINT& uID, CString& sClass, DWORD& dwStyle, POINT& pos, SIZE& size, DWORD& dwExStyle, UINT& uIconID);
	static void ExtractRCItems(const CString& sRCCtrl, LPCTSTR szType, CString* pItems[], int nMaxItems);

	static DWORD ParseRCStyles(LPCTSTR szClass, const CString& sStyle);
	static DWORD ParseRCExStyles(LPCTSTR szClass, const CString& sExStyle);
	static int ExtractRCStyles(const CString& sStyles, CStringArray& aStyles);
};

#endif // !defined(AFX_RCCTRLPARSER_H__8273AEF0_5E4D_4C6D_B391_FC6192A39314__INCLUDED_)
