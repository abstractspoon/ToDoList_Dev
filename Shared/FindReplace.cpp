// FindReplace.cpp : implementation file
//

#include "stdafx.h"
#include "FindReplace.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

FIND_STATE::FIND_STATE() : pFindReplaceDlg(NULL), bFindOnly(FALSE), bCaseSensitive(FALSE), bFindNext(TRUE), bWholeWord(FALSE) 
{
}

void FIND_STATE::UpdateState(const CString& sFind, BOOL bNext, BOOL bCase, BOOL bWord)
{
	strFind = sFind;
	bFindNext = bNext;
	bCaseSensitive = bCase;
	bWholeWord = bWord;
}

void FIND_STATE::UpdateState(const CString& sFind, const CString& sReplace, BOOL bNext, BOOL bCase, BOOL bWord)
{
	UpdateState(sFind, bNext, bCase, bWord);

	strReplace = sReplace;
}

/////////////////////////////////////////////////////////////////////////////

CFindReplaceDialog* IFindReplaceCmdHandler::NewFindReplaceDlg()
{
	return new CFindReplaceDialog;
}

/////////////////////////////////////////////////////////////////////////////

BOOL FindReplace::Initialise(CWnd* pParent, 
							IFindReplaceCmdHandler* pCmdHandler, 
							FIND_STATE* pState, 
							BOOL bFindOnly, 
							LPCTSTR szTitle,
							LPCTSTR szFind)
{
	ASSERT(pParent);
	ASSERT(pCmdHandler);
	ASSERT(pState);

	if (pState->pFindReplaceDlg != NULL)
	{
		if (pState->bFindOnly == bFindOnly)
		{
			pState->pFindReplaceDlg->SetActiveWindow();
			pState->pFindReplaceDlg->ShowWindow(SW_SHOW);

			return TRUE;
		}

		// else
		pState->pFindReplaceDlg->SendMessage(WM_CLOSE); // deletes as well
		ASSERT(pState->pFindReplaceDlg == NULL);
	}

	CString strFind(szFind);

	// if selection is empty or spans multiple lines use old find text
	if (strFind.IsEmpty() || (strFind.FindOneOf(_T("\n\r")) != -1))
		strFind = pState->strFind;

	CString strReplace = pState->strReplace;
	pState->pFindReplaceDlg = pCmdHandler->NewFindReplaceDlg();
	ASSERT(pState->pFindReplaceDlg != NULL);

	DWORD dwFlags = 0;

	if (pState->bFindNext)
		dwFlags |= FR_DOWN;

	if (pState->bCaseSensitive)
		dwFlags |= FR_MATCHCASE;

	if (pState->bWholeWord)
		dwFlags |= FR_WHOLEWORD;

	if (!pState->pFindReplaceDlg->Create(bFindOnly, strFind, strReplace, dwFlags, pParent))
	{
		pState->pFindReplaceDlg = NULL;
		return FALSE;
	}

	ASSERT(pState->pFindReplaceDlg != NULL);

	// set the title
	if (szTitle && *szTitle)
		pState->pFindReplaceDlg->SetWindowText(szTitle);

	pState->bFindOnly = bFindOnly;
	pState->pFindReplaceDlg->SetActiveWindow();
	pState->pFindReplaceDlg->ShowWindow(SW_SHOW);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////

void FindReplace::HandleCmd(IFindReplaceCmdHandler* pCmdHandler, 
							FIND_STATE* pState, 
							WPARAM /*wParam*/, 
							LPARAM lParam)
{
	ASSERT(lParam);
	ASSERT(pCmdHandler);
	ASSERT(lParam);

	CFindReplaceDialog* pDialog = CFindReplaceDialog::GetNotifier(lParam);

	ASSERT(pDialog != NULL);
	ASSERT(pDialog == pState->pFindReplaceDlg);

	if (pDialog->IsTerminating())
	{
		::SetFocus(pDialog->m_fr.hwndOwner);
		pState->pFindReplaceDlg = NULL;
	}
	else if (pDialog->FindNext())
	{
		pCmdHandler->OnFindNext(pDialog->GetFindString(), 
									pDialog->SearchDown(),
									pDialog->MatchCase(), 
									pDialog->MatchWholeWord());
	}
	else if (pDialog->ReplaceCurrent())
	{
		ASSERT(!pState->bFindOnly);

		pCmdHandler->OnReplaceSel(pDialog->GetFindString(),
									pDialog->GetReplaceString(),
									pDialog->SearchDown(), 
									pDialog->MatchCase(), 
									pDialog->MatchWholeWord());
	}
	else if (pDialog->ReplaceAll())
	{
		ASSERT(!pState->bFindOnly);

		pCmdHandler->OnReplaceAll(pDialog->GetFindString(), 
									pDialog->GetReplaceString(),
									pDialog->MatchCase(), 
									pDialog->MatchWholeWord());
	}
}

/////////////////////////////////////////////////////////////////////////////

void FindReplace::AdjustDialogPosition(FIND_STATE* pState, const CPoint& ptScreen, BOOL bUpDown)
{
	ASSERT(pState);
	ASSERT(pState->pFindReplaceDlg != NULL);

	CRect rExclude(ptScreen.x - 1, ptScreen.y - 1, ptScreen.x + 1, ptScreen.y + 1);
	AdjustDialogPosition(pState, rExclude);
}

void FindReplace::AdjustDialogPosition(FIND_STATE* pState, const CRect& rExcludeScreen, BOOL bUpDown)
{
	CRect rDlg;
	pState->pFindReplaceDlg->GetWindowRect(&rDlg);

	if (CRect().IntersectRect(rDlg, rExcludeScreen))
	{
		const int PADDING = 20;
		CRect rScreen(0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));

		if (bUpDown)
		{
			// Move find dialog above or below the exclusion rect
			// depending on where there is most space. Ideally stay
			// on the same if possible
			BOOL bMoveAbove = (rExcludeScreen.CenterPoint().y > rDlg.CenterPoint().y);

			// Check for space
			if (bMoveAbove)
				bMoveAbove = (rDlg.top > rDlg.Height());
			else
				bMoveAbove = ((rScreen.bottom - rDlg.bottom) < rDlg.Height());

			// Adjust the pos
			if (bMoveAbove)
				rDlg.OffsetRect(0, rExcludeScreen.top - rDlg.bottom - PADDING);
			else
				rDlg.OffsetRect(0, rExcludeScreen.bottom - rDlg.top + PADDING);
		}
		else
		{
			// Move find dialog left or right of the exclusion rect
			// depending on where there is most space. Ideally stay
			// on the same if possible
			BOOL bMoveLeft = (rExcludeScreen.CenterPoint().x > rDlg.CenterPoint().x);

			// Check for space
			if (bMoveLeft)
				bMoveLeft = (rDlg.left > rDlg.Width());
			else
				bMoveLeft = ((rScreen.right - rDlg.right) < rDlg.Width());

			// Adjust the pos
			if (bMoveLeft)
				rDlg.OffsetRect(rExcludeScreen.left - rDlg.right - PADDING, 0);
			else
				rDlg.OffsetRect(rExcludeScreen.right - rDlg.left + PADDING, 0);
		}

		pState->pFindReplaceDlg->MoveWindow(&rDlg);
	}
}

/////////////////////////////////////////////////////////////////////////////
