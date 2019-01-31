#if !defined(AFX_TDCFINDREPLACE_H__5951FDE6_508A_4A9D_A55D_D16EB026AEF7__INCLUDED_)
#define AFX_TDCFINDREPLACE_H__5951FDE6_508A_4A9D_A55D_D16EB026AEF7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ToDoCtrl.h : header file
//

#include "tdcenum.h"

#include "..\Shared\FindReplace.h"

/////////////////////////////////////////////////////////////////////////////

class CToDoCtrl;

/////////////////////////////////////////////////////////////////////////////

const UINT WM_TDCFR_SELECTTASK			= ::RegisterWindowMessage(_T("WM_TDCFR_SELECTNEXTTASK")); // wParam = TDC_ATTRIBUTE, lParam = TDC_SELECTTASK
const UINT WM_TDCFR_REPLACESELTASK		= ::RegisterWindowMessage(_T("WM_TDCFR_REPLACESELTASK")); // wParam = TDC_ATTRIBUTE
const UINT WM_TDCFR_REPLACEALLTASKS		= ::RegisterWindowMessage(_T("WM_TDCFR_REPLACEALLTASKS")); // wParam = TDC_ATTRIBUTE
const UINT WM_TDCFR_GETEXCLUSIONRECT	= ::RegisterWindowMessage(_T("WM_TDCFR_GETEXCLUSIONRECT")); // wParam = 'First time', lParam = LPRECT

/////////////////////////////////////////////////////////////////////////////
// CTDCFindReplace dialog

class CTDCFindReplace : public FIND_STATE, protected IFindReplaceCmdHandler
{
// Construction
public:
	CTDCFindReplace(const CToDoCtrl& tdc);
	virtual ~CTDCFindReplace();

	BOOL DoFindReplace(TDC_ATTRIBUTE nAttrib, CWnd* pParent);
	BOOL CanDoFindReplace(TDC_ATTRIBUTE nAttrib) const;

	BOOL IsReplacing() const { return m_bReplacing; }

protected:
	const CToDoCtrl& m_tdc;

	BOOL m_bReplacing;
	TDC_ATTRIBUTE m_nAttribute;

	// Implementation
protected:
	virtual void OnFindNext(const CString& sFind, BOOL bNext, BOOL bCase, BOOL bWord);
	virtual void OnReplaceSel(const CString& sFind, const CString& szReplace, BOOL bNext, BOOL bCase, BOOL bWord);
	virtual void OnReplaceAll(const CString& sFind, const CString& sReplace, BOOL bCase, BOOL bWord);

	virtual void AdjustDialogPosition(BOOL bFirstTime);

	BOOL SelectNextTask(TDC_SELECTTASK nSelect) const;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TDCFINDREPLACE_H__5951FDE6_508A_4A9D_A55D_D16EB026AEF7__INCLUDED_)
