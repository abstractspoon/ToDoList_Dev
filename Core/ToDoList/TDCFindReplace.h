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
class CPreferences;

/////////////////////////////////////////////////////////////////////////////

const UINT WM_TDCFR_SELECTNEXTTASK		= ::RegisterWindowMessage(_T("WM_TDCFR_SELECTNEXTTASK")); // wParam = TDC_SELECTTASK
const UINT WM_TDCFR_REPLACESELTASK		= ::RegisterWindowMessage(_T("WM_TDCFR_REPLACESELTASK")); 
const UINT WM_TDCFR_REPLACEALLTASKS		= ::RegisterWindowMessage(_T("WM_TDCFR_REPLACEALLTASKS")); 
const UINT WM_TDCFR_GETEXCLUSIONRECT	= ::RegisterWindowMessage(_T("WM_TDCFR_GETEXCLUSIONRECT")); // wParam = 'First time', lParam = LPRECT

/////////////////////////////////////////////////////////////////////////////
// CTDCFindReplace dialog

class CTDCFindReplace : protected FIND_STATE, protected IFindReplaceCmdHandler
{
// Construction
public:
	CTDCFindReplace(const CToDoCtrl& tdc);
	virtual ~CTDCFindReplace();

	BOOL DoFindReplace(TDC_ATTRIBUTE nAttrib);
	BOOL CanDoFindReplace(TDC_ATTRIBUTE nAttrib) const;

	void HandleCmd(WPARAM wParam, LPARAM lParam);
	void DestroyDialog();
	
	void SaveState(CPreferences& prefs) const;
	void LoadState(const CPreferences& prefs);

	CString GetSearchFor() const { return strFind; }
	CString GetReplaceWith() const { return strReplace; }
	TDC_ATTRIBUTE GetAttribute() const { return m_nAttribute; }

	BOOL WantCaseSensitive() const { return bCaseSensitive; }
	BOOL WantWholeWord() const { return bWholeWord; }
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

	BOOL Initialise(TDC_ATTRIBUTE nAttrib);
	BOOL SelectNextTask(TDC_SELECTNEXTTASK nSelect) const;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TDCFINDREPLACE_H__5951FDE6_508A_4A9D_A55D_D16EB026AEF7__INCLUDED_)
