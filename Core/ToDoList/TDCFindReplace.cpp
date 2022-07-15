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

CTDCFindReplace::CTDCFindReplace(const CToDoCtrl& tdc) 
	: 
	m_tdc(tdc), 
	m_nAttribute(TDCA_NONE),
	m_bReplacing(FALSE)
{
}

CTDCFindReplace::~CTDCFindReplace()
{
}

BOOL CTDCFindReplace::CanDoFindReplace(TDC_ATTRIBUTE nAttrib) const
{
	if (m_tdc.GetTaskCount() == 0)
		return FALSE;

	switch (nAttrib)
	{
	case TDCA_TASKNAME:
	case TDCA_COMMENTS:
		return TRUE;
	}

	ASSERT(0);
	return FALSE;
}


BOOL CTDCFindReplace::DoFindReplace(TDC_ATTRIBUTE nAttrib)
{
	// Sanity check
	if (!CanDoFindReplace(nAttrib))
	{
		ASSERT(0);
		return FALSE;
	}

	return Initialise(nAttrib);
}

BOOL CTDCFindReplace::Initialise(TDC_ATTRIBUTE nAttrib)
{
	if (nAttrib != m_nAttribute)
	{
		DestroyDialog();
		strFind.Empty();
	}

	BOOL bFindOnly = !m_tdc.CanEditSelectedTask(nAttrib);
	CEnString sTitle;
	
	switch (nAttrib)
	{
	case TDCA_TASKNAME:
		sTitle.LoadString(bFindOnly ? IDS_FINDINTASKTITLES : IDS_REPLACEINTASKTITLES);
		break;

	case TDCA_COMMENTS:
		sTitle.LoadString(bFindOnly ? IDS_FINDINTASKCOMMENTS : IDS_REPLACEINTASKCOMMENTS);
		break;

	default:
		ASSERT(0);
		return FALSE;
	}

	if (!FIND_STATE::Initialise(m_tdc, this, bFindOnly, sTitle))
	{
		ASSERT(0);
		return FALSE;
	}

	m_nAttribute = nAttrib;

	if (!strFind.IsEmpty())
	{
		VERIFY(SelectNextTask(TDC_SELECTNEXTINCLCURRENT));
		AdjustDialogPosition(TRUE);
	}

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
	BOOL bUpDown = ::SendMessage(m_tdc, WM_TDCFR_GETEXCLUSIONRECT, bFirstTime, (LPARAM)&rExclude);

	FIND_STATE::AdjustDialogPosition(rExclude, bUpDown);
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

BOOL CTDCFindReplace::SelectNextTask(TDC_SELECTNEXTTASK nSelectWhat) const
{
	return ::SendMessage(m_tdc, WM_TDCFR_SELECTNEXTTASK, nSelectWhat, 0);
}

void CTDCFindReplace::OnReplaceSel(const CString& sFind, const CString& sReplace, 
									BOOL bNext, BOOL bCase, BOOL bWord)
{
	ASSERT(m_tdc.CanEditSelectedTask(m_nAttribute));

	CAutoFlag af(m_bReplacing, TRUE);

	// Update state information for next time
	UpdateState(sFind, sReplace, bNext, bCase, bWord);

	if (::SendMessage(m_tdc, WM_TDCFR_REPLACESELTASK, 0, 0))
		OnFindNext(sFind, bNext, bCase, bWord);

	MessageBeep(MB_ICONHAND);
}

void CTDCFindReplace::OnReplaceAll(const CString& sFind, const CString& sReplace, BOOL bCase, BOOL bWord)
{
	ASSERT(m_tdc.CanEditSelectedTask(m_nAttribute));

	CAutoFlag af(m_bReplacing, TRUE);

	// Update state information for next time
	UpdateState(sFind, sReplace, TRUE, bCase, bWord);

	::SendMessage(m_tdc, WM_TDCFR_REPLACEALLTASKS, 0, 0);
}


