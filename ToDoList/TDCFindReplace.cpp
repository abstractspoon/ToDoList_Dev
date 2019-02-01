// ToDoCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "TDCFindReplace.h"
#include "ToDoCtrl.h"
#include "ToDoCtrlDataDefines.h"

#include "..\Shared\AutoFlag.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

CTDCFindReplace::CTDCFindReplace(const CToDoCtrl& tdc) : m_tdc(tdc), m_nAttribute(TDCA_NONE)
{
}

CTDCFindReplace::~CTDCFindReplace()
{
}

BOOL CTDCFindReplace::DoFindReplace(TDC_ATTRIBUTE nAttrib)
{
	BOOL bFindOnly = !m_tdc.CanEditSelectedTask(nAttrib);
	CEnString sTitle(bFindOnly ? IDS_FINDINTASKTITLES : IDS_REPLACEINTASKTITLES);

	// There may be multiple tasks selected so initialise with the first
	DWORD dwSelTaskID = m_tdc.GetSelectedTaskID();
	CString sFind(m_tdc.GetTaskTitle(dwSelTaskID));
	
	if (!Initialise((CWnd*)&m_tdc, this, bFindOnly, sTitle, sFind))
	{
		ASSERT(0);
		return FALSE;
	}

	// else
	m_nAttribute = nAttrib;

	VERIFY(SelectNextTask(TDC_SELECTNEXTINCLCURRENT));
	AdjustDialogPosition(TRUE);

	return TRUE;
}

void CTDCFindReplace::HandleCmd(WPARAM wParam, LPARAM lParam)
{
	FIND_STATE::HandleCmd(this, wParam, lParam);
}

void CTDCFindReplace::DestroyDialog()
{
	FIND_STATE::DestroyDialog();
}

void CTDCFindReplace::SaveState(CPreferences& prefs) const
{
	CString sKey = m_tdc.GetPreferencesKey(_T("FindReplace"));

	prefs.WriteProfileInt(sKey, _T("CaseSensitive"), bCaseSensitive);
	prefs.WriteProfileInt(sKey, _T("MatchWholeWord"), bWholeWord);
}

void CTDCFindReplace::LoadState(const CPreferences& prefs)
{
	CString sKey = m_tdc.GetPreferencesKey(_T("FindReplace"));

	bCaseSensitive = prefs.GetProfileInt(sKey, _T("CaseSensitive"), TRUE);
	bWholeWord = prefs.GetProfileInt(sKey, _T("MatchWholeWord"), TRUE);
}

void CTDCFindReplace::AdjustDialogPosition(BOOL bFirstTime)
{
	CRect rExclude(0, 0, 0, 0);
	::SendMessage(m_tdc, WM_TDCFR_GETEXCLUSIONRECT, bFirstTime, (LPARAM)&rExclude);

	FIND_STATE::AdjustDialogPosition(rExclude, !bFirstTime);
}

void CTDCFindReplace::OnFindNext(const CString& sFind, BOOL bNext, BOOL bCase, BOOL bWord)
{
	// Update state information for next time
	UpdateState(sFind, bNext, bCase, bWord);

	if (!SelectNextTask(bNext ? TDC_SELECTNEXT : TDC_SELECTPREV))
	{
		// Try again from start/end
		if (!SelectNextTask(bNext ? TDC_SELECTFIRST : TDC_SELECTLAST))
		{
			MessageBeep(MB_ICONHAND);
			return;
		}
	}

	// else
	AdjustDialogPosition(FALSE);
}

BOOL CTDCFindReplace::SelectNextTask(TDC_SELECTTASK nSelectWhat) const
{
	return ::SendMessage(m_tdc, WM_TDCFR_SELECTNEXTTASK, m_nAttribute, nSelectWhat);
}

void CTDCFindReplace::OnReplaceSel(const CString& sFind, const CString& sReplace, 
									BOOL bNext, BOOL bCase, BOOL bWord)
{
	ASSERT(m_tdc.CanEditSelectedTask(m_nAttribute));

	CAutoFlag af(m_bReplacing, TRUE);

	// Update state information for next time
	UpdateState(sFind, sReplace, bNext, bCase, bWord);

	if (::SendMessage(m_tdc, WM_TDCFR_REPLACESELTASK, m_nAttribute, 0))
		OnFindNext(sFind, bNext, bCase, bWord);

	MessageBeep(MB_ICONHAND);
}

void CTDCFindReplace::OnReplaceAll(const CString& sFind, const CString& sReplace, BOOL bCase, BOOL bWord)
{
	ASSERT(m_tdc.CanEditSelectedTask(m_nAttribute));

	CAutoFlag af(m_bReplacing, TRUE);

	// Update state information for next time
	UpdateState(sFind, sReplace, TRUE, bCase, bWord);

	::SendMessage(m_tdc, WM_TDCFR_REPLACEALLTASKS, m_nAttribute, 0);
}


